// Package trackedrepos manages the system-level public repo scan list.
// Public repos are system-owned once tracked — any authenticated user can add,
// removal is gated on active sync usage.
package trackedrepos

import (
	"context"
	"fmt"
	"net/http"
	"os"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
)

// ============================================================================
// DB HELPERS
// ============================================================================

// List returns all system-tracked repos.
func List(ctx context.Context, db database.DBConnection) ([]model.SystemTrackedRepo, error) {
	query := `FOR r IN system_tracked_repos SORT r.provider, r.owner, r.name RETURN r`
	cursor, err := db.Database.Query(ctx, query, nil)
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var repos []model.SystemTrackedRepo
	for cursor.HasMore() {
		var r model.SystemTrackedRepo
		if _, err := cursor.ReadDocument(ctx, &r); err == nil {
			repos = append(repos, r)
		}
	}
	return repos, nil
}

// FindByKey looks up a repo by provider/owner/name.
func FindByKey(ctx context.Context, db database.DBConnection, provider, owner, name string) (*model.SystemTrackedRepo, error) {
	query := `
		FOR r IN system_tracked_repos
			FILTER r.provider == @provider AND r.owner == @owner AND r.name == @name
			LIMIT 1
			RETURN r
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"provider": provider, "owner": owner, "name": name},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	if !cursor.HasMore() {
		return nil, nil
	}
	var r model.SystemTrackedRepo
	if _, err := cursor.ReadDocument(ctx, &r); err != nil {
		return nil, err
	}
	return &r, nil
}

// ActiveSyncCount returns how many endpoints are currently syncing releases
// whose name starts with owner/name (i.e. any version of this repo).
func ActiveSyncCount(ctx context.Context, db database.DBConnection, owner, name string) (int, error) {
	releaseName := owner + "/" + name
	query := `
		RETURN LENGTH(
			FOR s IN sync
				FILTER s.release_name == @name OR STARTS_WITH(s.release_name, @prefix)
				COLLECT endpoint = s.endpoint_name
				RETURN 1
		)
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"name":   releaseName,
			"prefix": releaseName + "/",
		},
	})
	if err != nil {
		return 0, err
	}
	defer cursor.Close()

	var count int
	if cursor.HasMore() {
		cursor.ReadDocument(ctx, &count)
	}
	return count, nil
}

// Add inserts a new system tracked repo. Returns existing record if already tracked.
// Verifies the repo exists on the provider before creating any DB records.
func Add(ctx context.Context, db database.DBConnection, repo *model.SystemTrackedRepo) (*model.SystemTrackedRepo, error) {
	existing, err := FindByKey(ctx, db, repo.Provider, repo.Owner, repo.Name)
	if err != nil {
		return nil, err
	}
	if existing != nil {
		return existing, nil // idempotent
	}

	// Verify the repo actually exists on the provider before writing anything.
	// This prevents phantom org docs for typos like "jenkins-ci" vs "jenkinsci".
	if err := verifyRepoExists(repo.Provider, repo.Owner, repo.Name); err != nil {
		return nil, fmt.Errorf("repo %s/%s not found on %s: %w", repo.Owner, repo.Name, repo.Provider, err)
	}

	// Org is created only after the repo is confirmed to exist.
	if err := ensurePublicOrg(ctx, db, repo.Owner); err != nil {
		return nil, fmt.Errorf("failed to ensure org for %s: %w", repo.Owner, err)
	}

	repo.ObjType = "SystemTrackedRepo"
	repo.AddedAt = time.Now()
	meta, err := db.Collections["system_tracked_repos"].CreateDocument(ctx, repo)
	if err != nil {
		return nil, err
	}
	repo.Key = meta.Key

	return repo, nil
}

// verifyRepoExists confirms the repo is accessible on the provider's public API.
// Returns an error if the repo does not exist or returns a non-200 status.
func verifyRepoExists(provider, owner, name string) error {
	var apiURL string
	switch provider {
	case "github":
		apiURL = fmt.Sprintf("https://api.github.com/repos/%s/%s", owner, name)
	case "gitlab":
		encoded := strings.ReplaceAll(fmt.Sprintf("%s/%s", owner, name), "/", "%2F")
		apiURL = fmt.Sprintf("https://gitlab.com/api/v4/projects/%s", encoded)
	default:
		return fmt.Errorf("unsupported provider %q", provider)
	}

	client := &http.Client{Timeout: 10 * time.Second}
	req, err := http.NewRequest("GET", apiURL, nil)
	if err != nil {
		return err
	}

	// Use system token for GitHub to avoid rate limiting
	if provider == "github" {
		if token := os.Getenv("GITHUB_TOKEN"); token != "" {
			req.Header.Set("Authorization", "Bearer "+token)
		}
		req.Header.Set("Accept", "application/vnd.github+json")
	}

	resp, err := client.Do(req)
	if err != nil {
		return fmt.Errorf("failed to reach %s API: %w", provider, err)
	}
	defer resp.Body.Close()

	switch resp.StatusCode {
	case http.StatusOK:
		return nil
	case http.StatusNotFound:
		return fmt.Errorf("%s/%s does not exist or is private", owner, name)
	case http.StatusForbidden, http.StatusUnauthorized:
		return fmt.Errorf("%s/%s is private or requires authentication", owner, name)
	default:
		return fmt.Errorf("%s API returned status %d for %s/%s", provider, resp.StatusCode, owner, name)
	}
}

// ensurePublicOrg upserts a minimal org document for the given name if one does not
// already exist. The org is marked as public (no credentials) so it never gets
// confused with an org that has GitHub App or PAT access.
func ensurePublicOrg(ctx context.Context, db database.DBConnection, orgName string) error {
	query := `
		UPSERT { name: @name }
		INSERT {
			name:         @name,
			display_name: @name,
			description:  "Auto-created for public repo tracking",
			is_public:    true,
			created_at:   DATE_ISO8601(DATE_NOW()),
			updated_at:   DATE_ISO8601(DATE_NOW())
		}
		UPDATE {
			updated_at: DATE_ISO8601(DATE_NOW())
		}
		IN orgs
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"name": orgName},
	})
	return err
}

// Remove deletes a system tracked repo by its document key.
func Remove(ctx context.Context, db database.DBConnection, key string) error {
	query := `REMOVE @key IN system_tracked_repos`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"key": key},
	})
	return err
}

// ============================================================================
// HANDLERS
// ============================================================================

// AddTrackedRepo handles POST /api/v1/tracked-repos
// Any authenticated user can add a public repo.
func AddTrackedRepo(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		username, _ := c.Locals("username").(string)
		userOrgs, _ := c.Locals("orgs").([]string)

		var req struct {
			Provider string `json:"provider"` // "github" | "gitlab"
			Owner    string `json:"owner"`
			Name     string `json:"name"`
		}
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "invalid request body"})
		}

		req.Provider = strings.ToLower(strings.TrimSpace(req.Provider))
		req.Owner = strings.TrimSpace(req.Owner)
		req.Name = strings.TrimSpace(req.Name)

		if req.Provider != "github" && req.Provider != "gitlab" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "provider must be github or gitlab"})
		}
		if req.Owner == "" || req.Name == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "owner and name are required"})
		}

		// Determine steward org — first org the user belongs to, or empty
		addedByOrg := ""
		if len(userOrgs) > 0 {
			addedByOrg = userOrgs[0]
		}

		ctx := c.Context()
		repo, err := Add(ctx, db, &model.SystemTrackedRepo{
			Provider:   req.Provider,
			Owner:      req.Owner,
			Name:       req.Name,
			Private:    false,
			AddedBy:    username,
			AddedByOrg: addedByOrg,
		})
		if err != nil {
			errMsg := err.Error()
			// Distinguish "repo not found" (user error) from DB failures (server error)
			if strings.Contains(errMsg, "does not exist") || strings.Contains(errMsg, "not found on") || strings.Contains(errMsg, "private or requires") {
				return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": errMsg})
			}
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to add repo"})
		}

		return c.Status(fiber.StatusCreated).JSON(fiber.Map{
			"success":      true,
			"message":      fmt.Sprintf("now tracking %s/%s/%s", req.Provider, req.Owner, req.Name),
			"provider":     repo.Provider,
			"owner":        repo.Owner,
			"name":         repo.Name,
			"added_by":     repo.AddedBy,
			"added_by_org": repo.AddedByOrg,
			"added_at":     repo.AddedAt,
		})
	}
}

// ListTrackedRepos handles GET /api/v1/tracked-repos
// Returns all system-tracked public repos with their active sync counts.
func ListTrackedRepos(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		ctx := c.Context()
		repos, err := List(ctx, db)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to list repos"})
		}

		type repoRow struct {
			Key        string    `json:"key"`
			Provider   string    `json:"provider"`
			Owner      string    `json:"owner"`
			Name       string    `json:"name"`
			AddedBy    string    `json:"added_by"`
			AddedByOrg string    `json:"added_by_org"`
			AddedAt    time.Time `json:"added_at"`
			SyncCount  int       `json:"active_sync_count"`
		}

		rows := make([]repoRow, 0, len(repos))
		for _, r := range repos {
			count, _ := ActiveSyncCount(ctx, db, r.Owner, r.Name)
			rows = append(rows, repoRow{
				Key:        r.Key,
				Provider:   r.Provider,
				Owner:      r.Owner,
				Name:       r.Name,
				AddedBy:    r.AddedBy,
				AddedByOrg: r.AddedByOrg,
				AddedAt:    r.AddedAt,
				SyncCount:  count,
			})
		}

		return c.JSON(fiber.Map{"repos": rows, "count": len(rows)})
	}
}

// RemoveTrackedRepo handles DELETE /api/v1/tracked-repos/:key
//
// Rules:
//   - System admin: can always remove
//   - Adder's org owner: can remove only if active_sync_count == 0
func RemoveTrackedRepo(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		key := c.Params("key")
		if key == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "key is required"})
		}

		userRole, _ := c.Locals("role").(string)
		userOrgs, _ := c.Locals("orgs").([]string)
		isAdmin := userRole == "admin"

		ctx := c.Context()

		// Fetch the record
		query := `FOR r IN system_tracked_repos FILTER r._key == @key LIMIT 1 RETURN r`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{"key": key},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "db error"})
		}
		defer cursor.Close()

		if !cursor.HasMore() {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "tracked repo not found"})
		}
		var repo model.SystemTrackedRepo
		cursor.ReadDocument(ctx, &repo)

		// Check stewardship: user must be in the adding org (or be admin)
		if !isAdmin {
			isSteward := false
			for _, org := range userOrgs {
				if org == repo.AddedByOrg {
					isSteward = true
					break
				}
			}
			if !isSteward {
				return c.Status(fiber.StatusForbidden).JSON(fiber.Map{
					"error": fmt.Sprintf("only members of org '%s' or system admins can remove this repo", repo.AddedByOrg),
				})
			}

			// Gate on active syncs
			syncCount, err := ActiveSyncCount(ctx, db, repo.Owner, repo.Name)
			if err != nil {
				return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to check active syncs"})
			}
			if syncCount > 0 {
				return c.Status(fiber.StatusConflict).JSON(fiber.Map{
					"error": fmt.Sprintf(
						"%s/%s is currently deployed to %d endpoint(s) across one or more orgs. "+
							"Remove those syncs first, or contact a system admin to force-remove.",
						repo.Owner, repo.Name, syncCount,
					),
					"active_sync_count": syncCount,
				})
			}
		}

		if err := Remove(ctx, db, key); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to remove repo"})
		}

		return c.JSON(fiber.Map{
			"success": true,
			"message": fmt.Sprintf("stopped tracking %s/%s/%s", repo.Provider, repo.Owner, repo.Name),
		})
	}
}
