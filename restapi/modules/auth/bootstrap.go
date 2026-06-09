// restapi/modules/auth/bootstrap.go
package auth

import (
	"context"
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
)

// BootstrapAdmin creates a default admin user if no users exist in the database
func BootstrapAdmin(db database.DBConnection) error {
	ctx := context.Background()

	query := `RETURN LENGTH(FOR u IN users RETURN 1)`
	cursor, err := db.Database.Query(ctx, query, nil)
	if err != nil {
		return fmt.Errorf("failed to check for existing users: %w", err)
	}
	defer cursor.Close()

	var count int
	if _, err := cursor.ReadDocument(ctx, &count); err != nil {
		return fmt.Errorf("failed to read user count: %w", err)
	}

	if count > 0 {
		fmt.Println("Users already exist, skipping admin bootstrap")
		return nil
	}

	fmt.Println("\n" + strings.Repeat("=", 72))
	fmt.Println("🔐 BOOTSTRAP ADMIN ACCOUNT")
	fmt.Println(strings.Repeat("=", 72))

	adminUsername := os.Getenv("ADMIN_USERNAME")
	if adminUsername == "" {
		adminUsername = "admin"
	}

	adminPassword := os.Getenv("ADMIN_PASSWORD")
	if adminPassword == "" {
		// Generate a secure random password if not provided
		randomPass, err := GenerateSecureToken(32)
		if err != nil {
			return fmt.Errorf("failed to generate admin password: %w", err)
		}
		adminPassword = randomPass
	}

	adminEmail := os.Getenv("ADMIN_EMAIL")
	if adminEmail == "" {
		adminEmail = "admin@example.com"
	}

	passwordHash, err := HashPassword(adminPassword)
	if err != nil {
		return fmt.Errorf("failed to hash admin password: %w", err)
	}

	adminUser := model.NewUser(adminUsername, "admin")
	adminUser.Email = adminEmail
	adminUser.PasswordHash = passwordHash
	adminUser.Orgs = []string{}
	adminUser.IsActive = true
	adminUser.Status = "active"
	adminUser.AuthProvider = "local"

	if err := createUser(ctx, db, adminUser); err != nil {
		return fmt.Errorf("failed to create bootstrap admin: %w", err)
	}

	fmt.Println(strings.Repeat("=", 72))
	fmt.Println("✅ Bootstrap admin created successfully!")
	fmt.Println(strings.Repeat("=", 72))
	fmt.Printf("\nUsername: %s\n", adminUsername)
	fmt.Printf("Password: %s\n\n", adminPassword)
	fmt.Println("⚠️  IMPORTANT: Change the admin password immediately!")
	fmt.Println("             Set ADMIN_PASSWORD environment variable for production!")
	fmt.Println(strings.Repeat("=", 72) + "\n")

	return nil
}

// EnsureDefaultRoles ensures the default roles exist in the database
func EnsureDefaultRoles(db database.DBConnection) error {
	defaultRoles := []struct {
		Name        string
		Permissions []string
		Description string
	}{
		{
			Name:        "owner",
			Permissions: []string{"read", "write", "delete", "admin"},
			Description: "Organization owner with full system access and billing management",
		},
		{
			Name:        "admin",
			Permissions: []string{"read", "write", "delete", "admin"},
			Description: "Full system access with user management capabilities",
		},
		{
			Name:        "editor",
			Permissions: []string{"read", "write"},
			Description: "Can read and modify resources within assigned organizations",
		},
		{
			Name:        "viewer",
			Permissions: []string{"read"},
			Description: "Read-only access to resources within assigned organizations",
		},
	}

	for _, role := range defaultRoles {
		if err := upsertRole(db, role.Name, role.Permissions, role.Description); err != nil {
			return fmt.Errorf("failed to create role %s: %w", role.Name, err)
		}
	}

	fmt.Println("Default roles ensured: owner, admin, editor, viewer")
	return nil
}

// upsertRole creates or updates a role in the database for the bootstrap process
func upsertRole(db database.DBConnection, name string, permissions []string, description string) error {
	ctx := context.Background()
	query := `
		UPSERT { name: @name }
		INSERT { name: @name, permissions: @permissions, description: @description, created_at: @timestamp }
		UPDATE { permissions: @permissions, description: @description, updated_at: @timestamp }
		IN roles
	`
	bindVars := map[string]interface{}{
		"name":        name,
		"permissions": permissions,
		"description": description,
		"timestamp":   time.Now(),
	}
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	return err
}
