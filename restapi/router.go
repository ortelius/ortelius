// Package restapi provides the main router and initialization for REST API endpoints.
package restapi

import (
	"context"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"time"

	"github.com/go-git/go-git/v5"
	githttp "github.com/go-git/go-git/v5/plumbing/transport/http"
	"github.com/gofiber/fiber/v3"
	"github.com/graphql-go/graphql"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/restapi/modules/auth"
	"github.com/ortelius/ortelius/v12/restapi/modules/github"
	"github.com/ortelius/ortelius/v12/restapi/modules/metadata"
	org "github.com/ortelius/ortelius/v12/restapi/modules/org"
	"github.com/ortelius/ortelius/v12/restapi/modules/rbac"
	"github.com/ortelius/ortelius/v12/restapi/modules/releases"
	"github.com/ortelius/ortelius/v12/restapi/modules/releasesync"
	"github.com/ortelius/ortelius/v12/restapi/modules/trackedrepos"
)

// SetupRoutes configures all REST API routes and the GraphQL endpoint.
func SetupRoutes(app *fiber.App, db database.DBConnection, schema graphql.Schema) {

	// Background initialization tasks
	go func() {
		if err := auth.BootstrapAdmin(db); err != nil {
			log.Printf("WARNING: Failed to bootstrap admin: %v", err)
		}
	}()

	go func() {
		if err := auth.EnsureDefaultRoles(db); err != nil {
			log.Printf("WARNING: Failed to ensure default roles: %v", err)
		}
	}()

	// OIDC providers (Google now; Authentik/Okta register the same way once
	// their env vars are set - see LoadOIDCProvidersFromEnv) and GitHub's
	// separate OAuth2 sign-in app.
	auth.LoadOIDCProvidersFromEnv(context.Background())
	auth.LoadGitHubOAuthFromEnv()

	emailConfig := auth.LoadEmailConfig()
	go autoApplyRBACOnStartup(db, emailConfig)
	go startInvitationCleanup(db)

	// Background token validation — runs every 6 hours
	// Validates all org PATs and updates token_status on the org doc
	go startTokenValidation(db)

	// API Group /api/v1
	api := app.Group("/api/v1")

	// GraphQL
	api.Post("/graphql", auth.OptionalAuth(db), GraphQLHandler(schema))

	// Public Routes
	api.Post("/signup", auth.Signup(db, emailConfig))

	// Auth Routes
	authGroup := api.Group("/auth")
	authGroup.Post("/login", auth.Login(db))
	authGroup.Post("/logout", auth.Logout())
	authGroup.Get("/me", auth.OptionalAuth(db), auth.Me(db))
	authGroup.Post("/forgot-password", auth.ForgotPassword(db))
	authGroup.Post("/change-password", auth.RequireAuth(db), auth.ChangePassword(db))
	authGroup.Post("/refresh", auth.RefreshToken(db))

	// GitHub Auth Routes (App-install flow - links repo access to an
	// already-logged-in user; see github.go)
	authGroup.Get("/github/login", auth.GitHubLogin)
	authGroup.Get("/github/callback", auth.GitHubCallback(db))

	// GitHub Sign-In Routes (OAuth2 login flow - separate OAuth App
	// registration from the GitHub App above; see github_oauth.go)
	authGroup.Get("/github-signin/login", auth.GitHubSigninLogin)
	authGroup.Get("/github-signin/callback", auth.GitHubSigninCallback(db))

	// Generic OIDC Sign-In Routes - :provider matches a name registered via
	// LoadOIDCProvidersFromEnv (e.g. "google", and later "authentik", "okta")
	authGroup.Get("/:provider/login", auth.OIDCLogin)
	authGroup.Get("/:provider/callback", auth.OIDCCallback(db))

	// GitHub Integration Routes
	githubGroup := api.Group("/github", auth.RequireAuth(db))
	githubGroup.Get("/repos", github.ListRepos(db))
	githubGroup.Post("/onboard", github.OnboardRepos(db))

	// Public repo search — uses system token, no org credentials needed
	// GET /api/v1/github/search?q=kubernetes&provider=github
	// GET /api/v1/github/search?q=gitlab-runner&provider=gitlab
	githubGroup.Get("/search", github.SearchPublicRepos(db))

	// Single repo lookup — verifies a repo exists before tracking
	// GET /api/v1/github/repo?owner=curl&name=curl&provider=github
	githubGroup.Get("/repo", github.FetchPublicRepo(db))

	// Invitation Routes
	invitationGroup := api.Group("/invitation")
	invitationGroup.Get("/:token", auth.GetInvitationHandler(db))
	invitationGroup.Post("/:token/accept", auth.AcceptInvitationHandler(db))
	invitationGroup.Post("/:token/resend", auth.ResendInvitationHandler(db, emailConfig))

	// User Management (Admin)
	userGroup := api.Group("/users", auth.RequireAuth(db), auth.RequireRole("admin"))
	userGroup.Get("/", auth.ListUsers(db))
	userGroup.Post("/", auth.CreateUser(db))
	userGroup.Get("/:username", auth.GetUser(db))
	userGroup.Put("/:username", auth.UpdateUser(db))
	userGroup.Delete("/:username", auth.DeleteUser(db))

	// RBAC Management (Admin)
	rbac := api.Group("/rbac", auth.RequireAuth(db), auth.RequireRole("admin"))
	rbac.Post("/apply/content", auth.ApplyRBACFromBody(db, emailConfig))
	rbac.Post("/apply/upload", auth.ApplyRBACFromUpload(db, emailConfig))
	rbac.Post("/apply", auth.ApplyRBACFromFile(db, emailConfig))
	rbac.Post("/validate", auth.ValidateRBAC(db))
	rbac.Get("/config", auth.GetRBACConfig(db))
	rbac.Get("/invitations", auth.ListPendingInvitationsHandler(db))
	rbac.Post("/webhook", handleRBACWebhook(db, emailConfig))

	// Org Management
	// Status and credential endpoints are scoped to org members/owners only.
	// The org middleware checks that the requesting user belongs to the org.
	orgGroup := api.Group("/orgs", auth.RequireAuth(db))

	// GET  /api/v1/orgs/:org/status
	// Returns connection state, token status, visible tracked repos (minus hidden)
	orgGroup.Get("/:org/status", org.GetOrgStatus(db))

	// POST /api/v1/orgs/:org/credentials
	// Store encrypted PAT for private repo access (owner only)
	orgGroup.Post("/:org/credentials", auth.RequireRole("owner"), org.UpdateOrgCredentials(db))

	// DELETE /api/v1/orgs/:org/credentials/:provider
	// Remove stored PAT for a provider (owner only)
	orgGroup.Delete("/:org/credentials/:provider", auth.RequireRole("owner"), org.DeleteOrgCredentials(db))

	// POST /api/v1/orgs/:org/tracked-repos
	// Add a repo to tracked_repos: commits to YAML + syncs to DB + unhides (owner only)
	orgGroup.Post("/:org/tracked-repos", auth.RequireRole("owner"), github.TrackRepo(db, auth.MakeUserCreator(db, emailConfig)))

	// POST /api/v1/orgs/:org/hidden-repos
	// Hide a tracked repo from the UI without stopping scanning (owner or admin)
	orgGroup.Post("/:org/hidden-repos", auth.RequireRole("owner", "admin"), github.HideRepo(db))

	// System-level public repo tracking — any authenticated user can add, removal gated on usage
	// GET  /api/v1/tracked-repos              — list all tracked public repos with sync counts
	// POST /api/v1/tracked-repos              — add a public repo (any authenticated user)
	// DELETE /api/v1/tracked-repos/:key       — remove (steward org owner if zero syncs, admin always)
	trackedGroup := api.Group("/tracked-repos", auth.RequireAuth(db))
	trackedGroup.Get("/", trackedrepos.ListTrackedRepos(db))
	trackedGroup.Post("/", trackedrepos.AddTrackedRepo(db))
	trackedGroup.Delete("/:key", trackedrepos.RemoveTrackedRepo(db))

	// Release & Sync
	api.Post("/releases", auth.OptionalAuth(db), releases.PostReleaseWithSBOM(db))
	api.Post("/sync", auth.OptionalAuth(db), releasesync.PostSyncWithEndpoint(db))
	api.Get("/releases/exists", releases.CheckReleaseExists(db))

	// Metadata
	api.Get("/metadata/:key", auth.OptionalAuth(db), metadata.GetMetadata(db))
	api.Put("/metadata/:key", auth.OptionalAuth(db), metadata.SetMetadata(db))

	log.Println("API routes initialized successfully")
}

func handleRBACWebhook(db database.DBConnection, emailConfig *auth.EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		yamlContent, err := syncRBACFromRepo()
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": err.Error(),
			})
		}

		config, err := rbac.LoadRBACConfig(yamlContent)
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "Invalid RBAC config: " + err.Error(),
			})
		}

		result, err := rbac.ApplyRBAC(db, config, auth.MakeUserCreator(db, emailConfig))
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": "Failed to apply RBAC: " + err.Error(),
			})
		}

		return c.JSON(fiber.Map{
			"success": true,
			"message": "RBAC synchronized from repository",
			"result":  result,
		})
	}
}

func syncRBACFromRepo() (string, error) {
	repoURL := os.Getenv("RBAC_REPO")
	token := os.Getenv("RBAC_REPO_TOKEN")

	if repoURL == "" || token == "" {
		return "", fmt.Errorf("RBAC_REPO and RBAC_REPO_TOKEN must be configured")
	}

	tempDir, err := os.MkdirTemp("", "rbac-sync-*")
	if err != nil {
		return "", fmt.Errorf("failed to create temp directory: %w", err)
	}
	defer os.RemoveAll(tempDir)

	authMethod := &githttp.BasicAuth{
		Username: "oauth2",
		Password: token,
	}

	_, err = git.PlainClone(tempDir, false, &git.CloneOptions{
		URL:      repoURL,
		Auth:     authMethod,
		Depth:    1,
		Progress: nil,
	})
	if err != nil {
		return "", fmt.Errorf("failed to clone RBAC repo: %w", err)
	}

	yamlPath := filepath.Join(tempDir, "rbac.yaml")
	yamlContent, err := os.ReadFile(yamlPath)
	if err != nil {
		return "", fmt.Errorf("failed to read rbac.yaml: %w", err)
	}

	return string(yamlContent), nil
}

// startInvitationCleanup runs expired invitation cleanup every hour.
func startInvitationCleanup(db database.DBConnection) {
	runCleanup(db)
	ticker := time.NewTicker(1 * time.Hour)
	defer ticker.Stop()
	for range ticker.C {
		runCleanup(db)
	}
}

func runCleanup(db database.DBConnection) {
	ctx, cancel := context.WithTimeout(context.Background(), 30*time.Second)
	defer cancel()
	count, err := auth.CleanupExpiredInvitations(ctx, db)
	if err != nil {
		fmt.Printf("⚠️  Background Task: Failed to cleanup expired invitations: %v\n", err)
		return
	}
	if count > 0 {
		fmt.Printf("🧹 Background Task: Cleaned up %d expired invitations\n", count)
	}
}

// startTokenValidation validates all org PATs every 6 hours.
// Updates token_status on each org doc so the UI can show
// a warning badge when credentials need attention.
func startTokenValidation(db database.DBConnection) {
	// Run once at startup after a short delay to let the DB settle
	time.Sleep(30 * time.Second)
	runTokenValidation(db)

	ticker := time.NewTicker(6 * time.Hour)
	defer ticker.Stop()
	for range ticker.C {
		runTokenValidation(db)
	}
}

func runTokenValidation(db database.DBConnection) {
	ctx, cancel := context.WithTimeout(context.Background(), 2*time.Minute)
	defer cancel()

	if err := org.ValidateOrgTokens(ctx, db); err != nil {
		fmt.Printf("⚠️  Background Task: Token validation failed: %v\n", err)
		return
	}
	fmt.Println("✅ Background Task: Org token validation complete")
}

func autoApplyRBACOnStartup(db database.DBConnection, emailConfig *auth.EmailConfig) {
	repoURL := os.Getenv("RBAC_REPO")
	token := os.Getenv("RBAC_REPO_TOKEN")

	var yamlContent string
	var err error

	if repoURL != "" && token != "" {
		fmt.Println("🔄 Auto-applying RBAC configuration from GitHub:", repoURL)
		yamlContent, err = syncRBACFromRepo()
		if err != nil {
			fmt.Printf("⚠️  Failed to sync RBAC from GitHub: %v\n", err)
			return
		}
	} else {
		configPath := os.Getenv("RBAC_CONFIG_PATH")
		if configPath == "" {
			configPath = "/etc/ortelius/rbac.yaml"
		}
		if _, err := os.Stat(configPath); err != nil {
			return
		}
		fmt.Println("🔄 Auto-applying RBAC configuration from local file:", configPath)
		yamlBytes, err := os.ReadFile(configPath)
		if err != nil {
			fmt.Printf("⚠️  Failed to read RBAC config: %v\n", err)
			return
		}
		yamlContent = string(yamlBytes)
	}

	config, err := rbac.LoadRBACConfig(yamlContent)
	if err != nil {
		fmt.Printf("⚠️  Failed to load RBAC config: %v\n", err)
		return
	}

	result, err := rbac.ApplyRBAC(db, config, auth.MakeUserCreator(db, emailConfig))
	if err != nil {
		fmt.Printf("⚠️  RBAC apply failed: %v\n", err)
		return
	}

	fmt.Printf("✅ RBAC apply complete: %d created, %d updated, %d removed, %d invited\n",
		len(result.Created), len(result.Updated), len(result.Removed), len(result.Invited))
}
