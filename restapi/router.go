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
	"github.com/ortelius/ortelius/v12/restapi/modules/releases"
	"github.com/ortelius/ortelius/v12/restapi/modules/releasesync"
)

// SetupRoutes configures all REST API routes and the GraphQL endpoint.
// Redundant CORS middleware has been removed here as it is now handled globally
// in internal/api/fiber.go to prevent 408 timeouts.
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

	emailConfig := auth.LoadEmailConfig()
	go autoApplyRBACOnStartup(db, emailConfig)
	go startInvitationCleanup(db)

	// API Group /api/v1
	api := app.Group("/api/v1")

	// GraphQL Route - Mounted within the api group to inherit path prefixes
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

	// GitHub Auth Routes
	authGroup.Get("/github/login", auth.GitHubLogin)
	authGroup.Get("/github/callback", auth.GitHubCallback(db))

	// GitHub Integration Routes
	githubGroup := api.Group("/github", auth.RequireAuth(db))
	githubGroup.Get("/repos", github.ListRepos(db))
	githubGroup.Post("/onboard", github.OnboardRepos(db))

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
	rbac.Post("/validate", auth.HandleRBACValidate(db))
	rbac.Get("/config", auth.GetRBACConfig(db))
	rbac.Get("/invitations", auth.ListPendingInvitationsHandler(db))
	rbac.Post("/webhook", handleRBACWebhook(db, emailConfig))

	// Release & Sync
	api.Post("/releases", auth.OptionalAuth(db), releases.PostReleaseWithSBOM(db))
	api.Post("/sync", auth.OptionalAuth(db), releasesync.PostSyncWithEndpoint(db))

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

		config, err := auth.LoadPeriobolosConfig(yamlContent)
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "Invalid RBAC config: " + err.Error(),
			})
		}

		result, err := auth.ApplyRBAC(db, config, emailConfig)
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

func autoApplyRBACOnStartup(db database.DBConnection, emailConfig *auth.EmailConfig) {
	// Check if RBAC repo is configured for GitOps mode
	repoURL := os.Getenv("RBAC_REPO")
	token := os.Getenv("RBAC_REPO_TOKEN")

	var yamlContent string
	var err error

	if repoURL != "" && token != "" {
		// GitOps mode: Fetch from GitHub
		fmt.Println("🔄 Auto-applying RBAC configuration from GitHub:", repoURL)
		yamlContent, err = syncRBACFromRepo()
		if err != nil {
			fmt.Printf("⚠️  Failed to sync RBAC from GitHub: %v\n", err)
			return
		}
	} else {
		// Fallback to local file mode
		configPath := os.Getenv("RBAC_CONFIG_PATH")
		if configPath == "" {
			configPath = "/etc/ortelius/rbac.yaml"
		}
		if _, err := os.Stat(configPath); err != nil {
			// Neither GitHub nor local file configured
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

	config, err := auth.LoadPeriobolosConfig(yamlContent)
	if err != nil {
		fmt.Printf("⚠️  Failed to load RBAC config: %v\n", err)
		return
	}

	result, err := auth.ApplyRBAC(db, config, emailConfig)
	if err != nil {
		fmt.Printf("⚠️  RBAC apply failed: %v\n", err)
		return
	}

	fmt.Printf("✅ RBAC apply complete: %d created, %d updated, %d removed, %d invited\n",
		len(result.Created), len(result.Updated), len(result.Removed), len(result.Invited))
}
