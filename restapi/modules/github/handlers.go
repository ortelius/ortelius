// Package github provides GitHub integration handlers for the REST API.
package github

import (
	"encoding/json"
	"fmt"
	"net/http"
	"net/url"
	"os"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
	"github.com/ortelius/ortelius/v12/restapi/modules/gitops"
	"github.com/ortelius/ortelius/v12/restapi/modules/rbac"
)

// ListRepos returns the list of repositories accessible to the user via App Installation
func ListRepos(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		username := c.Locals("username").(string)
		ctx := c.Context()

		query := `FOR u IN users FILTER u.username == @username LIMIT 1 RETURN u`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{"username": username},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Database error"})
		}
		defer cursor.Close()

		var user model.User
		if _, err := cursor.ReadDocument(ctx, &user); err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "User not found"})
		}

		if user.GitHubInstallationID == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "GitHub App not connected"})
		}

		appToken, err := GetInstallationToken(user.GitHubInstallationID)
		if err != nil {
			return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "Failed to generate app token: " + err.Error()})
		}

		repos, err := FetchRepos(appToken)
		if err != nil {
			return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "Failed to fetch repos from GitHub: " + err.Error()})
		}

		return c.JSON(repos)
	}
}

// OnboardRepos processes the selected repositories using the App Installation Token
func OnboardRepos(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		username := c.Locals("username").(string)
		var req OnboardRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "Invalid body"})
		}

		ctx := c.Context()

		query := `FOR u IN users FILTER u.username == @username LIMIT 1 RETURN u`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{"username": username},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Database error"})
		}
		defer cursor.Close()

		var user model.User
		if _, err := cursor.ReadDocument(ctx, &user); err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "User not found"})
		}

		if user.GitHubInstallationID == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "GitHub App not connected"})
		}

		appToken, err := GetInstallationToken(user.GitHubInstallationID)
		if err != nil {
			return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "Failed to generate app token: " + err.Error()})
		}

		processed := 0
		errors := []string{}

		for _, fullRepoName := range req.Repos {
			parts := strings.Split(fullRepoName, "/")
			if len(parts) != 2 {
				continue
			}
			owner, repo := parts[0], parts[1]

			// Fetch repo metadata to determine visibility before creating releases.
			// IsPublic drives GraphQL access control — public repos are visible to
			// unauthenticated users via the is_public == true filter.
			repoMeta, repoErr := FetchRepoMeta(appToken, owner, repo)
			isPublic := true // default to public; overridden when we have metadata
			if repoErr == nil {
				isPublic = !repoMeta.Private
			}

			releases, err := FetchReleases(appToken, owner, repo)
			if err == nil {
				for _, r := range releases {
					releaseModel := model.ProjectRelease{
						Name:           fullRepoName,
						Version:        r.TagName,
						ObjType:        "ProjectRelease",
						GitRepo:        fullRepoName,
						GitOrg:         owner,
						GitRepoProject: repo,
						BuildDate:      r.PublishedAt,
						IsPublic:       isPublic,
						ContentSha:     r.TagName,
					}
					releaseModel.ParseAndSetNameComponents()
					releaseModel.ParseAndSetVersion()
					_, err := db.Collections["release"].CreateDocument(ctx, releaseModel)
					if err == nil {
						processed++
					}
				}
			} else {
				errors = append(errors, fmt.Sprintf("Failed to fetch releases for %s: %v", fullRepoName, err))
			}

			runs, err := FetchWorkflowRuns(appToken, owner, repo)
			if err == nil {
				for _, run := range runs {
					if run.Conclusion == "success" {
						endpointName := "github-actions/" + fullRepoName
						endpoint := model.NewEndpoint()
						endpoint.Name = endpointName
						endpoint.EndpointType = "serverless"
						endpoint.Environment = "ci"
						endpoint.ParseAndSetNameComponents()
						db.Collections["endpoint"].CreateDocument(ctx, endpoint)

						sync := model.NewSync()
						sync.EndpointName = endpointName
						sync.ReleaseName = fullRepoName
						version := run.HeadSha
						if len(version) > 7 {
							version = version[:7]
						}
						sync.ReleaseVersion = version
						sync.SyncedAt = run.UpdatedAt
						db.Collections["sync"].CreateDocument(ctx, sync)
						processed++
					}
				}
			}
		}

		return c.JSON(fiber.Map{
			"message": fmt.Sprintf("Processed %d items via GitHub App", processed),
			"errors":  errors,
		})
	}
}

// TrackRepoRequest defines the body for adding a tracked repo.
type TrackRepoRequest struct {
	Provider string `json:"provider"` // "github" | "gitlab"
	Owner    string `json:"owner"`
	Name     string `json:"name"`
	Private  bool   `json:"private,omitempty"`
}

// TrackRepoCreator is the injected function signature for creating users with invitations.
// Matches rbac.UserCreator — passed in from auth to avoid import cycle.
type TrackRepoCreator = rbac.UserCreator

// TrackRepo handles POST /api/v1/orgs/:org/tracked-repos
// Requires role: owner
// 1. Commits the repo to tracked_repos in the RBAC YAML via gitops
// 2. Calls rbac.ApplyRBAC to sync the YAML change to the org doc in ArangoDB
// 3. Removes the repo from hidden_repos if it was previously hidden
func TrackRepo(db database.DBConnection, createUser rbac.UserCreator) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		if orgName == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org name is required"})
		}

		username, _ := c.Locals("username").(string)

		var req TrackRepoRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "invalid request body"})
		}

		if req.Provider != "github" && req.Provider != "gitlab" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "provider must be github or gitlab"})
		}
		if req.Owner == "" || req.Name == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "owner and name are required"})
		}

		// For private repos, verify credentials exist on the org before committing
		if req.Private {
			ctx := c.Context()
			org, err := rbac.GetOrgByName(ctx, db, orgName)
			if err != nil {
				return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found"})
			}
			switch req.Provider {
			case "github":
				if org.GitHubInstallationID == "" && org.GitHubTokenEnc == "" {
					return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
						"error": "private GitHub repos require the GitHub App or a GitHub PAT configured for this org",
					})
				}
			case "gitlab":
				if org.GitLabTokenEnc == "" {
					return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
						"error": "private GitLab repos require a GitLab PAT configured for this org",
					})
				}
			}
		}

		// 1. Commit to YAML via gitops
		updatedYaml, err := gitops.AddTrackedRepo(orgName, req.Provider, req.Owner, req.Name, req.Private, username)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": fmt.Sprintf("failed to update RBAC repo: %v", err),
			})
		}

		// 2. Sync YAML changes to ArangoDB via rbac.ApplyRBAC
		config, err := rbac.LoadRBACConfig(updatedYaml)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to parse updated YAML"})
		}

		if _, err := rbac.ApplyRBAC(db, config, createUser); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to sync RBAC to database"})
		}

		// 3. Remove from hidden_repos if present — "Add repo" always makes it visible
		repoKey := req.Provider + "/" + req.Owner + "/" + req.Name
		ctx := c.Context()
		org, err := rbac.GetOrgByName(ctx, db, orgName)
		if err == nil && org.RemoveFromHidden(repoKey) {
			org.UpdatedAt = time.Now()
			rbac.UpdateOrg(ctx, db, org)
		}

		return c.Status(fiber.StatusCreated).JSON(fiber.Map{
			"success":  true,
			"message":  fmt.Sprintf("now tracking %s/%s/%s", req.Provider, req.Owner, req.Name),
			"provider": req.Provider,
			"owner":    req.Owner,
			"name":     req.Name,
			"private":  req.Private,
		})
	}
}

// HideRepoRequest defines the body for hiding a tracked repo.
type HideRepoRequest struct {
	Provider string `json:"provider"`
	Owner    string `json:"owner"`
	Name     string `json:"name"`
}

// HideRepo handles POST /api/v1/orgs/:org/hidden-repos
// Adds the repo to hidden_repos on the org doc — UI visibility only.
// Does NOT touch the YAML or stop scanning.
func HideRepo(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		if orgName == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org name is required"})
		}

		var req HideRepoRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "invalid request body"})
		}
		if req.Provider == "" || req.Owner == "" || req.Name == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "provider, owner, and name are required"})
		}

		ctx := c.Context()
		org, err := rbac.GetOrgByName(ctx, db, orgName)
		if err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found"})
		}

		if !org.HasTrackedRepo(req.Provider, req.Owner, req.Name) {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": fmt.Sprintf("%s/%s/%s is not a tracked repo for org %s", req.Provider, req.Owner, req.Name, orgName),
			})
		}

		repoKey := req.Provider + "/" + req.Owner + "/" + req.Name
		if !org.IsRepoHidden(repoKey) {
			org.HiddenRepos = append(org.HiddenRepos, repoKey)
			org.UpdatedAt = time.Now()
			if err := rbac.UpdateOrg(ctx, db, org); err != nil {
				return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to hide repo"})
			}
		}

		return c.JSON(fiber.Map{
			"success": true,
			"message": fmt.Sprintf("%s/%s/%s hidden from UI (still being scanned)", req.Provider, req.Owner, req.Name),
		})
	}
}

// SearchPublicReposResult is a provider-agnostic repo search result.
type SearchPublicReposResult struct {
	Provider    string `json:"provider"`
	Owner       string `json:"owner"`
	Name        string `json:"name"`
	FullName    string `json:"full_name"`
	Description string `json:"description"`
	HTMLURL     string `json:"html_url"`
	Stars       int    `json:"stars"`
	Private     bool   `json:"private"`
}

// SearchPublicRepos handles GET /api/v1/github/search?q=...&provider=github|gitlab
// Uses the system GITHUB_TOKEN / GITLAB_TOKEN for rate limiting only.
func SearchPublicRepos(_ database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		query := strings.TrimSpace(c.Query("q"))
		provider := strings.ToLower(strings.TrimSpace(c.Query("provider")))
		if query == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "q (search query) is required"})
		}
		if provider == "" {
			provider = "github"
		}
		if provider != "github" && provider != "gitlab" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "provider must be github or gitlab"})
		}

		var results []SearchPublicReposResult
		var searchErr error
		switch provider {
		case "github":
			results, searchErr = searchGitHub(query)
		case "gitlab":
			results, searchErr = searchGitLab(query)
		}

		if searchErr != nil {
			return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": searchErr.Error()})
		}

		return c.JSON(fiber.Map{
			"provider": provider,
			"query":    query,
			"results":  results,
			"count":    len(results),
		})
	}
}

func searchGitHub(query string) ([]SearchPublicReposResult, error) {
	apiURL := fmt.Sprintf("https://api.github.com/search/repositories?q=%s+is:public&per_page=20&sort=stars",
		url.QueryEscape(query))

	req, err := http.NewRequest("GET", apiURL, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to build request: %w", err)
	}
	req.Header.Set("Accept", "application/vnd.github.v3+json")
	if t := getSystemToken(); t != "" {
		req.Header.Set("Authorization", "Bearer "+t)
	}

	client := &http.Client{Timeout: 10 * time.Second}
	resp, err := client.Do(req)
	if err != nil {
		return nil, fmt.Errorf("github search failed: %w", err)
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("github search returned status %d", resp.StatusCode)
	}

	var result struct {
		Items []struct {
			FullName    string `json:"full_name"`
			Description string `json:"description"`
			HTMLURL     string `json:"html_url"`
			Private     bool   `json:"private"`
			StarCount   int    `json:"stargazers_count"`
			Owner       struct {
				Login string `json:"login"`
			} `json:"owner"`
			Name string `json:"name"`
		} `json:"items"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		return nil, fmt.Errorf("failed to decode response: %w", err)
	}

	var repos []SearchPublicReposResult
	for _, item := range result.Items {
		repos = append(repos, SearchPublicReposResult{
			Provider:    "github",
			Owner:       item.Owner.Login,
			Name:        item.Name,
			FullName:    item.FullName,
			Description: item.Description,
			HTMLURL:     item.HTMLURL,
			Stars:       item.StarCount,
			Private:     item.Private,
		})
	}
	return repos, nil
}

func searchGitLab(query string) ([]SearchPublicReposResult, error) {
	apiURL := fmt.Sprintf("https://gitlab.com/api/v4/projects?search=%s&visibility=public&per_page=20&order_by=star_count",
		url.QueryEscape(query))

	req, err := http.NewRequest("GET", apiURL, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to build request: %w", err)
	}
	if t := getSystemGitLabToken(); t != "" {
		req.Header.Set("PRIVATE-TOKEN", t)
	}

	client := &http.Client{Timeout: 10 * time.Second}
	resp, err := client.Do(req)
	if err != nil {
		return nil, fmt.Errorf("gitlab search failed: %w", err)
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("gitlab search returned status %d", resp.StatusCode)
	}

	var items []struct {
		PathWithNamespace string `json:"path_with_namespace"`
		Description       string `json:"description"`
		WebURL            string `json:"web_url"`
		StarCount         int    `json:"star_count"`
		Namespace         struct {
			Path string `json:"path"`
		} `json:"namespace"`
		Path string `json:"path"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&items); err != nil {
		return nil, fmt.Errorf("failed to decode response: %w", err)
	}

	var repos []SearchPublicReposResult
	for _, item := range items {
		repos = append(repos, SearchPublicReposResult{
			Provider:    "gitlab",
			Owner:       item.Namespace.Path,
			Name:        item.Path,
			FullName:    item.PathWithNamespace,
			Description: item.Description,
			HTMLURL:     item.WebURL,
			Stars:       item.StarCount,
			Private:     false,
		})
	}
	return repos, nil
}

// FetchPublicRepo handles GET /api/v1/github/repo?owner=...&name=...&provider=github|gitlab
// Verifies a single repo exists on the provider and returns its metadata.
// Used by the frontend to validate direct owner/repo input before tracking.
func FetchPublicRepo(_ database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		provider := strings.ToLower(strings.TrimSpace(c.Query("provider", "github")))
		owner := strings.TrimSpace(c.Query("owner"))
		name := strings.TrimSpace(c.Query("name"))

		if owner == "" || name == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "owner and name are required"})
		}

		client := &http.Client{Timeout: 10 * time.Second}

		switch provider {
		case "github":
			apiURL := fmt.Sprintf("https://api.github.com/repos/%s/%s", owner, name)
			req, _ := http.NewRequest("GET", apiURL, nil)
			req.Header.Set("Accept", "application/vnd.github+json")
			if token := os.Getenv("GITHUB_TOKEN"); token != "" {
				req.Header.Set("Authorization", "Bearer "+token)
			}
			resp, err := client.Do(req)
			if err != nil {
				return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "failed to reach GitHub API"})
			}
			defer resp.Body.Close()
			if resp.StatusCode == http.StatusNotFound {
				return c.Status(fiber.StatusNotFound).JSON(fiber.Map{
					"error": fmt.Sprintf("%s/%s does not exist on GitHub or is private", owner, name),
				})
			}
			if resp.StatusCode != http.StatusOK {
				return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{
					"error": fmt.Sprintf("GitHub API returned status %d", resp.StatusCode),
				})
			}
			var result map[string]interface{}
			json.NewDecoder(resp.Body).Decode(&result)
			return c.JSON(result)

		case "gitlab":
			encoded := strings.ReplaceAll(fmt.Sprintf("%s/%s", owner, name), "/", "%2F")
			apiURL := fmt.Sprintf("https://gitlab.com/api/v4/projects/%s", encoded)
			req, _ := http.NewRequest("GET", apiURL, nil)
			if token := os.Getenv("GITLAB_TOKEN"); token != "" {
				req.Header.Set("PRIVATE-TOKEN", token)
			}
			resp, err := client.Do(req)
			if err != nil {
				return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "failed to reach GitLab API"})
			}
			defer resp.Body.Close()
			if resp.StatusCode == http.StatusNotFound {
				return c.Status(fiber.StatusNotFound).JSON(fiber.Map{
					"error": fmt.Sprintf("%s/%s does not exist on GitLab or is private", owner, name),
				})
			}
			if resp.StatusCode != http.StatusOK {
				return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{
					"error": fmt.Sprintf("GitLab API returned status %d", resp.StatusCode),
				})
			}
			var result map[string]interface{}
			json.NewDecoder(resp.Body).Decode(&result)
			return c.JSON(result)

		default:
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "provider must be github or gitlab"})
		}
	}
}
