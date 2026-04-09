// Package auth provides authentication and authorization utilities.
//
//revive:disable-next-line:var-naming
package auth

import (
	"context"
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
	"gopkg.in/yaml.v2"
)

// RBACConfig represents the top-level RBAC configuration
type RBACConfig struct {
	Users []UserConfig `yaml:"users"`
	Roles []RoleConfig `yaml:"roles"`
}

// UserConfig represents a user in the RBAC configuration
type UserConfig struct {
	Username     string   `yaml:"username"`
	Email        string   `yaml:"email,omitempty"`
	Role         string   `yaml:"role"`
	Orgs         []string `yaml:"orgs,omitempty"`
	Password     string   `yaml:"password,omitempty"`
	PasswordHash string   `yaml:"password_hash,omitempty"`
	IsActive     *bool    `yaml:"is_active,omitempty"`
	AuthProvider string   `yaml:"auth_provider,omitempty"`
	ExternalID   string   `yaml:"external_id,omitempty"`
}

// RoleConfig represents a role in the RBAC configuration
type RoleConfig struct {
	Name        string   `yaml:"name"`
	Permissions []string `yaml:"permissions"`
	Description string   `yaml:"description,omitempty"`
}

// LoadRBACConfig loads RBAC configuration from a YAML file
func LoadRBACConfig(filepath string) (*RBACConfig, error) {
	data, err := os.ReadFile(filepath)
	if err != nil {
		return nil, fmt.Errorf("failed to read config file: %w", err)
	}

	var config RBACConfig
	if err := yaml.Unmarshal(data, &config); err != nil {
		return nil, fmt.Errorf("failed to parse YAML: %w", err)
	}

	return &config, nil
}

// LoadRBACConfigFromString loads RBAC configuration from a YAML string
func LoadRBACConfigFromString(yamlContent string) (*RBACConfig, error) {
	var config RBACConfig
	if err := yaml.Unmarshal([]byte(yamlContent), &config); err != nil {
		return nil, fmt.Errorf("failed to parse YAML: %w", err)
	}

	return &config, nil
}

// ValidateRBACConfig validates the RBAC configuration
func ValidateRBACConfig(config *RBACConfig) error {
	if config == nil {
		return fmt.Errorf("config cannot be nil")
	}

	usernames := make(map[string]bool)
	for i, user := range config.Users {
		if usernames[user.Username] {
			return fmt.Errorf("duplicate username at index %d: %s", i, user.Username)
		}
		usernames[user.Username] = true

		if user.Username == "" {
			return fmt.Errorf("user at index %d has empty username", i)
		}

		if user.Role == "" {
			return fmt.Errorf("user %s has empty role", user.Username)
		}

		validRoles := map[string]bool{
			"owner":  true, // Owner allowed
			"admin":  true,
			"editor": true,
			"viewer": true,
		}
		if !validRoles[user.Role] {
			return fmt.Errorf("user %s has invalid role: %s (must be owner, admin, editor, or viewer)",
				user.Username, user.Role)
		}

		if user.Email != "" && !strings.Contains(user.Email, "@") {
			return fmt.Errorf("user %s has invalid email: %s", user.Username, user.Email)
		}

		if user.AuthProvider != "" {
			validProviders := map[string]bool{
				"local": true,
				"oidc":  true,
			}
			if !validProviders[user.AuthProvider] {
				return fmt.Errorf("user %s has invalid auth_provider: %s (must be local or oidc)",
					user.Username, user.AuthProvider)
			}
		}

		if user.AuthProvider == "oidc" && user.ExternalID == "" {
			return fmt.Errorf("user %s with auth_provider=oidc must have external_id", user.Username)
		}
	}

	roleNames := make(map[string]bool)
	for i, role := range config.Roles {
		if roleNames[role.Name] {
			return fmt.Errorf("duplicate role name at index %d: %s", i, role.Name)
		}
		roleNames[role.Name] = true

		if role.Name == "" {
			return fmt.Errorf("role at index %d has empty name", i)
		}

		validPermissions := map[string]bool{
			"read":   true,
			"write":  true,
			"delete": true,
			"admin":  true,
		}
		for _, perm := range role.Permissions {
			if !validPermissions[perm] {
				return fmt.Errorf("role %s has invalid permission: %s", role.Name, perm)
			}
		}
	}

	return nil
}

// ApplyRBACConfig applies the RBAC configuration to the database
func ApplyRBACConfig(db database.DBConnection, config *RBACConfig) (int, int, int, []error) {
	var errors []error
	usersCreated := 0
	usersUpdated := 0
	rolesCreated := 0

	for _, roleConfig := range config.Roles {
		if err := upsertRole(db, &roleConfig); err != nil {
			errors = append(errors, fmt.Errorf("failed to upsert role %s: %w", roleConfig.Name, err))
		} else {
			rolesCreated++
		}
	}

	for _, userConfig := range config.Users {
		created, err := upsertUser(db, &userConfig)
		if err != nil {
			errors = append(errors, fmt.Errorf("failed to upsert user %s: %w", userConfig.Username, err))
			continue
		}

		if created {
			usersCreated++
		} else {
			usersUpdated++
		}
	}

	return usersCreated, usersUpdated, rolesCreated, errors
}

// ApplyRBACConfigFromFile loads and applies RBAC configuration from a file
func ApplyRBACConfigFromFile(db database.DBConnection, filepath string) (int, int, int, []error) {
	config, err := LoadRBACConfig(filepath)
	if err != nil {
		return 0, 0, 0, []error{fmt.Errorf("failed to load config: %w", err)}
	}

	if err := ValidateRBACConfig(config); err != nil {
		return 0, 0, 0, []error{fmt.Errorf("invalid config: %w", err)}
	}

	return ApplyRBACConfig(db, config)
}

// upsertUser creates or updates a user in the database
func upsertUser(db database.DBConnection, userConfig *UserConfig) (bool, error) {
	ctx := context.Background()

	existingUser, err := getUserByUsername(ctx, db, userConfig.Username)
	userExists := err == nil

	now := time.Now()

	var passwordHash string
	switch {
	case userConfig.PasswordHash != "":
		passwordHash = userConfig.PasswordHash
	case userConfig.Password != "":
		hash, err := HashPassword(userConfig.Password)
		if err != nil {
			return false, fmt.Errorf("failed to hash password: %w", err)
		}
		passwordHash = hash
	case userExists:
		passwordHash = existingUser.PasswordHash
	default:
		randomPass, err := GenerateSecureToken(32)
		if err != nil {
			return false, fmt.Errorf("failed to generate random password: %w", err)
		}
		hash, err := HashPassword(randomPass)
		if err != nil {
			return false, fmt.Errorf("failed to hash random password: %w", err)
		}
		passwordHash = hash
	}

	isActive := true
	if userConfig.IsActive != nil {
		isActive = *userConfig.IsActive
	}

	authProvider := "local"
	if userConfig.AuthProvider != "" {
		authProvider = userConfig.AuthProvider
	}

	email := userConfig.Email
	if email == "" && userExists {
		email = existingUser.Email
	}

	status := "active"
	if !isActive {
		status = "inactive"
	}

	orgs := userConfig.Orgs
	if orgs == nil {
		orgs = []string{}
	}

	var query string
	if userExists {
		query = `
			FOR u IN users
			FILTER u.username == @username
			UPDATE u WITH {
				email: @email,
				role: @role,
				orgs: @orgs,
				password_hash: @password_hash,
				is_active: @is_active,
				status: @status,
				auth_provider: @auth_provider,
				external_id: @external_id,
				updated_at: @updated_at
			} IN users
			RETURN NEW
		`
	} else {
		query = `
			INSERT {
				username: @username,
				email: @email,
				role: @role,
				orgs: @orgs,
				password_hash: @password_hash,
				is_active: @is_active,
				status: @status,
				auth_provider: @auth_provider,
				external_id: @external_id,
				created_at: @created_at,
				updated_at: @updated_at
			} INTO users
			RETURN NEW
		`
	}

	bindVars := map[string]interface{}{
		"username":      userConfig.Username,
		"email":         email,
		"role":          userConfig.Role,
		"orgs":          orgs,
		"password_hash": passwordHash,
		"is_active":     isActive,
		"status":        status,
		"auth_provider": authProvider,
		"external_id":   userConfig.ExternalID,
		"updated_at":    now,
	}

	if !userExists {
		bindVars["created_at"] = now
	}

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	if err != nil {
		return false, err
	}
	defer cursor.Close()

	return !userExists, nil
}

// upsertRole creates or updates a role in the database
func upsertRole(db database.DBConnection, roleConfig *RoleConfig) error {
	ctx := context.Background()

	query := `
		UPSERT { name: @name }
		INSERT {
			name: @name,
			permissions: @permissions,
			description: @description,
			created_at: @timestamp
		}
		UPDATE {
			permissions: @permissions,
			description: @description,
			updated_at: @timestamp
		}
		IN roles
	`

	bindVars := map[string]interface{}{
		"name":        roleConfig.Name,
		"permissions": roleConfig.Permissions,
		"description": roleConfig.Description,
		"timestamp":   time.Now(),
	}

	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	return err
}

// ExportRBACConfig exports the current RBAC configuration to YAML
func ExportRBACConfig(db database.DBConnection) (*RBACConfig, error) {
	ctx := context.Background()

	config := &RBACConfig{
		Users: []UserConfig{},
		Roles: []RoleConfig{},
	}

	userQuery := `FOR u IN users RETURN u`
	userCursor, err := db.Database.Query(ctx, userQuery, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to query users: %w", err)
	}
	defer userCursor.Close()

	for userCursor.HasMore() {
		var user model.User
		_, err := userCursor.ReadDocument(ctx, &user)
		if err != nil {
			continue
		}

		userConfig := UserConfig{
			Username:     user.Username,
			Email:        user.Email,
			Role:         user.Role,
			Orgs:         user.Orgs,
			AuthProvider: user.AuthProvider,
			ExternalID:   user.ExternalID,
		}

		if !user.IsActive {
			isActive := false
			userConfig.IsActive = &isActive
		}

		config.Users = append(config.Users, userConfig)
	}

	roleQuery := `FOR r IN roles RETURN r`
	roleCursor, err := db.Database.Query(ctx, roleQuery, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to query roles: %w", err)
	}
	defer roleCursor.Close()

	for roleCursor.HasMore() {
		var role struct {
			Name        string   `json:"name"`
			Permissions []string `json:"permissions"`
			Description string   `json:"description,omitempty"`
		}
		_, err := roleCursor.ReadDocument(ctx, &role)
		if err != nil {
			continue
		}

		config.Roles = append(config.Roles, RoleConfig{
			Name:        role.Name,
			Permissions: role.Permissions,
			Description: role.Description,
		})
	}

	return config, nil
}

// ExportRBACConfigToFile exports the current RBAC configuration to a YAML file
func ExportRBACConfigToFile(db database.DBConnection, filepath string) error {
	config, err := ExportRBACConfig(db)
	if err != nil {
		return err
	}

	data, err := yaml.Marshal(config)
	if err != nil {
		return fmt.Errorf("failed to marshal config: %w", err)
	}

	if err := os.WriteFile(filepath, data, 0600); err != nil {
		return fmt.Errorf("failed to write config file: %w", err)
	}

	return nil
}

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
	defaultRoles := []RoleConfig{
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
		if err := upsertRole(db, &role); err != nil {
			return fmt.Errorf("failed to create role %s: %w", role.Name, err)
		}
	}

	fmt.Println("Default roles ensured: owner, admin, editor, viewer")
	return nil
}

// SyncRBACFromFile is a convenience function that combines load, validate, and apply
func SyncRBACFromFile(db database.DBConnection, filepath string) error {
	usersCreated, usersUpdated, rolesCreated, errs := ApplyRBACConfigFromFile(db, filepath)

	if len(errs) > 0 {
		for _, err := range errs {
			fmt.Printf("ERROR: %v\n", err)
		}
		return fmt.Errorf("RBAC sync completed with %d errors", len(errs))
	}

	fmt.Printf("RBAC sync successful: %d users created, %d users updated, %d roles created\n",
		usersCreated, usersUpdated, rolesCreated)
	return nil
}

// GetUserOrgs returns the list of orgs a user can access
func GetUserOrgs(ctx context.Context, db database.DBConnection, username string) ([]string, error) {
	user, err := getUserByUsername(ctx, db, username)
	if err != nil {
		return nil, err
	}
	return user.Orgs, nil
}

// UserHasOrgAccess checks if a user has access to a specific org
func UserHasOrgAccess(ctx context.Context, db database.DBConnection, username, org string) (bool, error) {
	user, err := getUserByUsername(ctx, db, username)
	if err != nil {
		return false, err
	}
	return user.HasOrgAccess(org), nil
}
