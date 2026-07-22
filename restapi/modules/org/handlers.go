// Package org provides organization management handlers for the REST API.
package org

import (
	"context"
	"fmt"
	"net/http"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
	"github.com/ortelius/ortelius/v12/restapi/modules/rbac"
	"github.com/ortelius/ortelius/v12/util"
)

// GetOrgStatus handles GET /api/v1/orgs/:org/status
// Returns github connection state, token status, tracked repos (filtered by hidden_repos),
// and whether credentials are present. Never returns raw or encrypted tokens.
func GetOrgStatus(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		if orgName == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org name is required"})
		}

		// Verify the requesting user belongs to this org
		userOrgs, ok := c.Locals("orgs").([]string)
		if !ok || !containsOrg(userOrgs, orgName) {
			return c.Status(fiber.StatusForbidden).JSON(fiber.Map{"error": "access denied"})
		}

		ctx := c.Context()
		org, err := rbac.GetOrgByName(ctx, db, orgName)
		if err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found"})
		}

		// Build visible repos — tracked repos minus hidden ones
		var visibleRepos []fiber.Map
		for _, repo := range org.TrackedRepos {
			if !org.IsRepoHidden(repo.RepoKey()) {
				visibleRepos = append(visibleRepos, fiber.Map{
					"provider": repo.Provider,
					"owner":    repo.Owner,
					"name":     repo.Name,
					"private":  repo.Private,
					"added_by": repo.AddedBy,
					"added_at": repo.AddedAt,
				})
			}
		}
		if visibleRepos == nil {
			visibleRepos = []fiber.Map{}
		}

		// Determine connection status without exposing tokens
		githubAppConnected := org.GitHubInstallationID != ""
		githubPatPresent := org.GitHubTokenEnc != ""
		gitlabPatPresent := org.GitLabTokenEnc != ""

		// Determine overall github status for UI badge
		var githubStatus string
		switch {
		case githubAppConnected:
			githubStatus = "app_connected"
		case githubPatPresent:
			githubStatus = "pat_only"
		default:
			githubStatus = "not_connected"
		}

		// Query GKE / cluster-type endpoints for this org and their last sync time.
		// deployment-gke posts with endpoint_type "3" (legacy) or "cluster"/"gke";
		// we surface all cluster-family types so the UI can show the integration status.
		gkeEndpoints := queryGKEEndpoints(ctx, db, orgName)

		return c.JSON(fiber.Map{
			"org":                  orgName,
			"display_name":         org.DisplayName,
			"github_status":        githubStatus,
			"github_app_connected": githubAppConnected,
			"github_pat_present":   githubPatPresent,
			"gitlab_pat_present":   gitlabPatPresent,
			"token_status":         org.TokenStatus,
			"token_last_validated": org.TokenLastValidated,
			"tracked_repos":        visibleRepos,
			"hidden_repos":         org.HiddenRepos,
			"gke_endpoints":        gkeEndpoints,
		})
	}
}

// UpdateOrgCredentialsRequest defines the body for credential updates.
type UpdateOrgCredentialsRequest struct {
	Provider string `json:"provider"` // "github" | "gitlab"
	Token    string `json:"token"`    // raw PAT — encrypted before storage
}

// UpdateOrgCredentials handles POST /api/v1/orgs/:org/credentials
// Org owners only. Encrypts and stores the PAT on the org doc.
// After storing, immediately validates the token and sets token_status.
// The raw token is never logged or returned.
func UpdateOrgCredentials(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		if orgName == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org name is required"})
		}

		// Verify the requesting user is an owner of this org
		userOrgs, ok := c.Locals("orgs").([]string)
		if !ok || !containsOrg(userOrgs, orgName) {
			return c.Status(fiber.StatusForbidden).JSON(fiber.Map{"error": "access denied"})
		}

		var req UpdateOrgCredentialsRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "invalid request body"})
		}

		if req.Provider != "github" && req.Provider != "gitlab" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "provider must be github or gitlab"})
		}

		if req.Token == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "token is required"})
		}

		ctx := c.Context()
		org, err := rbac.GetOrgByName(ctx, db, orgName)
		if err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found"})
		}

		// Encrypt the token before storing
		encrypted, err := util.EncryptToken(req.Token)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to secure token"})
		}

		switch req.Provider {
		case "github":
			org.GitHubTokenEnc = encrypted
		case "gitlab":
			org.GitLabTokenEnc = encrypted
		}

		// Immediately validate the token — sets token_status before returning
		status := validateProviderToken(req.Provider, req.Token)
		now := time.Now()
		org.TokenStatus = status
		org.TokenLastValidated = &now
		org.UpdatedAt = now

		if err := rbac.UpdateOrg(ctx, db, org); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to save credentials"})
		}

		return c.JSON(fiber.Map{
			"success":      true,
			"provider":     req.Provider,
			"token_status": status,
			"token_masked": util.MaskToken(req.Token),
		})
	}
}

// DeleteOrgCredentials handles DELETE /api/v1/orgs/:org/credentials/:provider
// Org owners only. Removes the encrypted PAT for the given provider.
func DeleteOrgCredentials(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		provider := c.Params("provider")

		if provider != "github" && provider != "gitlab" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "provider must be github or gitlab"})
		}

		userOrgs, ok := c.Locals("orgs").([]string)
		if !ok || !containsOrg(userOrgs, orgName) {
			return c.Status(fiber.StatusForbidden).JSON(fiber.Map{"error": "access denied"})
		}

		ctx := c.Context()
		org, err := rbac.GetOrgByName(ctx, db, orgName)
		if err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found"})
		}

		switch provider {
		case "github":
			org.GitHubTokenEnc = ""
		case "gitlab":
			org.GitLabTokenEnc = ""
		}

		// Clear token status if no tokens remain
		if org.GitHubTokenEnc == "" && org.GitLabTokenEnc == "" {
			org.TokenStatus = ""
			org.TokenLastValidated = nil
		}

		org.UpdatedAt = time.Now()

		if err := rbac.UpdateOrg(ctx, db, org); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to remove credentials"})
		}

		return c.JSON(fiber.Map{
			"success":  true,
			"provider": provider,
			"message":  fmt.Sprintf("%s credentials removed", provider),
		})
	}
}

// ValidateOrgTokens runs token validation for all orgs that have PATs stored.
// Called by the background job in router.go every 6 hours.
func ValidateOrgTokens(ctx context.Context, db database.DBConnection) error {
	query := `
		FOR o IN orgs
			FILTER o.github_token_enc != null AND o.github_token_enc != ""
				OR o.gitlab_token_enc != null AND o.gitlab_token_enc != ""
			RETURN o
	`
	cursor, err := db.Database.Query(ctx, query, nil)
	if err != nil {
		return fmt.Errorf("failed to query orgs with tokens: %w", err)
	}
	defer cursor.Close()

	now := time.Now()

	for cursor.HasMore() {
		var org model.Org
		if _, err := cursor.ReadDocument(ctx, &org); err != nil {
			continue
		}

		status := "valid"

		// Validate GitHub PAT if present
		if org.GitHubTokenEnc != "" {
			token, err := util.DecryptToken(org.GitHubTokenEnc)
			if err != nil {
				status = "invalid"
			} else {
				s := validateProviderToken("github", token)
				if s != "valid" {
					status = s
				}
			}
		}

		// Validate GitLab PAT if present
		if org.GitLabTokenEnc != "" {
			token, err := util.DecryptToken(org.GitLabTokenEnc)
			if err != nil {
				status = "invalid"
			} else {
				s := validateProviderToken("gitlab", token)
				if s != "valid" {
					status = s
				}
			}
		}

		org.TokenStatus = status
		org.TokenLastValidated = &now
		org.UpdatedAt = now

		if err := rbac.UpdateOrg(ctx, db, &org); err != nil {
			fmt.Printf("[WARN] ValidateOrgTokens: failed to update org %s: %v\n", org.Name, err)
		}
	}

	return nil
}

// validateProviderToken checks a raw token against the provider API.
// Returns "valid", "expired", or "invalid".
func validateProviderToken(provider, token string) string {
	var apiURL string
	switch provider {
	case "github":
		apiURL = "https://api.github.com/user"
	case "gitlab":
		apiURL = "https://gitlab.com/api/v4/user"
	default:
		return "invalid"
	}

	req, err := http.NewRequest("GET", apiURL, nil)
	if err != nil {
		return "invalid"
	}

	if provider == "github" {
		req.Header.Set("Authorization", "Bearer "+token)
		req.Header.Set("Accept", "application/vnd.github.v3+json")
	} else {
		req.Header.Set("PRIVATE-TOKEN", token)
	}

	client := &http.Client{Timeout: 10 * time.Second}
	resp, err := client.Do(req)
	if err != nil {
		// Network error — don't mark as invalid, leave existing status
		return "unverified"
	}
	defer resp.Body.Close()

	switch resp.StatusCode {
	case http.StatusOK:
		return "valid"
	case http.StatusUnauthorized, http.StatusForbidden:
		return "expired"
	default:
		return "invalid"
	}
}

// containsOrg checks if a target organization exists in a slice of organizations.
func containsOrg(orgs []string, target string) bool {
	for _, org := range orgs {
		if org == target {
			return true
		}
	}
	return false
}

// gkeEndpointView is the wire shape for a single GKE/cluster endpoint.
type gkeEndpointView struct {
	Name         string  `json:"name"`
	EndpointType string  `json:"endpoint_type"`
	LastSync     *string `json:"last_sync"`
}

// queryGKEEndpoints returns all endpoints for the org whose endpoint_type is
// a cluster/GKE family value, along with their most recent sync timestamp.
//
// deployment-gke (deployment-gke/main.go) posts with endpoint_type "3" — a
// legacy numeric string from an earlier enum. The model has since moved to
// named constants ("cluster", "gke", "eks", "aks", etc.). We match all of
// these so the indicator works regardless of which value was stored.
func queryGKEEndpoints(ctx context.Context, db database.DBConnection, orgName string) []gkeEndpointView {
	// Cluster-family endpoint_type values recognised across all versions of the
	// sync clients. "3" is the numeric legacy value written by deployment-gke.
	clusterTypes := []interface{}{"3", "cluster", "gke", "eks", "aks", "fargate", "edge", "iot", "mission_asset"}

	query := `
		LET cluster_types = @cluster_types
		FOR e IN endpoint
			FILTER LOWER(e.org) == LOWER(@org)
			FILTER e.endpoint_type IN cluster_types
			LET last_sync = FIRST(
				FOR s IN sync
					FILTER s.endpoint_name == e.name
					SORT s.synced_at DESC
					LIMIT 1
					RETURN s.synced_at
			)
			SORT e.name ASC
			RETURN {
				name:          e.name,
				endpoint_type: e.endpoint_type,
				last_sync:     last_sync
			}
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"org":           orgName,
			"cluster_types": clusterTypes,
		},
	})
	if err != nil {
		fmt.Printf("WARNING: queryGKEEndpoints(%s): %v\n", orgName, err)
		return []gkeEndpointView{}
	}
	defer cursor.Close()

	var results []gkeEndpointView
	for cursor.HasMore() {
		var row struct {
			Name         string  `json:"name"`
			EndpointType string  `json:"endpoint_type"`
			LastSync     *string `json:"last_sync"`
		}
		if _, err := cursor.ReadDocument(ctx, &row); err != nil {
			continue
		}
		results = append(results, gkeEndpointView{
			Name:         row.Name,
			EndpointType: row.EndpointType,
			LastSync:     row.LastSync,
		})
	}
	if results == nil {
		return []gkeEndpointView{}
	}
	return results
}
