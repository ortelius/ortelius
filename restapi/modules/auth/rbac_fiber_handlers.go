// Package auth provides RBAC handlers for Fiber.
package auth

import (
	"context"
	"fmt"
	"io"
	"os"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
	"gopkg.in/yaml.v2"
)

// ApplyRBACFromBody applies RBAC config from request body (YAML only)
func ApplyRBACFromBody(db database.DBConnection, emailConfig *EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		contentType := string(c.Request().Header.ContentType())

		if !strings.Contains(contentType, "application/x-yaml") &&
			!strings.Contains(contentType, "text/yaml") &&
			!strings.Contains(contentType, "application/yaml") {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "Content-Type must be application/x-yaml",
			})
		}

		yamlContent := string(c.Body())
		if yamlContent == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "RBAC config cannot be empty",
			})
		}

		config, err := LoadPeriobolosConfig(yamlContent)
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": fmt.Sprintf("Invalid RBAC config: %v", err),
			})
		}

		result, err := ApplyRBAC(db, config, emailConfig)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": fmt.Sprintf("Failed to apply RBAC: %v", err),
			})
		}

		return c.JSON(fiber.Map{
			"success":     true,
			"message":     "RBAC applied successfully",
			"result":      result,
			"invitations": buildInvitationLinks(result.Invitations, emailConfig),
			"summary": fiber.Map{
				"orgs_created":  len(result.OrgsCreated),
				"orgs_updated":  len(result.OrgsUpdated),
				"users_created": len(result.Created),
				"users_updated": len(result.Updated),
				"users_removed": len(result.Removed),
				"invited":       len(result.Invited),
			},
		})
	}
}

// ValidateRBAC validates RBAC config without applying changes
func ValidateRBAC(_ database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		yamlContent := string(c.Body())
		if yamlContent == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"valid": false,
				"error": "RBAC config cannot be empty",
			})
		}

		config, err := LoadPeriobolosConfig(yamlContent)
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"valid": false,
				"error": err.Error(),
			})
		}

		// Validation for Option 2: Check if users in members list exist in users definition
		userMap := make(map[string]bool)
		for _, user := range config.Users {
			userMap[user.Username] = true
		}

		for _, org := range config.Orgs {
			for _, member := range org.Members {
				if !userMap[member.Username] {
					return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
						"valid": false,
						"error": fmt.Sprintf("Organization %s references undefined user: %s", org.Name, member.Username),
					})
				}
			}
		}

		return c.JSON(fiber.Map{
			"valid": true,
			"summary": fiber.Map{
				"orgs":  len(config.Orgs),
				"users": len(config.Users),
			},
		})
	}
}

// buildInvitationLinks generates invitation links for invited users
func buildInvitationLinks(invitations map[string]string, emailConfig *EmailConfig) map[string]string {
	links := make(map[string]string)
	for username, token := range invitations {
		links[username] = fmt.Sprintf("%s/invitation/%s", emailConfig.BaseURL, token)
	}
	return links
}

// ApplyRBACFromUpload applies RBAC config from uploaded file
func ApplyRBACFromUpload(db database.DBConnection, emailConfig *EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		file, err := c.FormFile("file")
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "No file uploaded"})
		}

		openedFile, err := file.Open()
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to open file"})
		}
		defer openedFile.Close()

		yamlContent, err := io.ReadAll(openedFile)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to read file"})
		}

		config, err := LoadPeriobolosConfig(string(yamlContent))
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": err.Error()})
		}

		result, err := ApplyRBAC(db, config, emailConfig)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": err.Error()})
		}

		return c.JSON(fiber.Map{"success": true, "result": result})
	}
}

// ApplyRBACFromFile applies RBAC config from filesystem
func ApplyRBACFromFile(db database.DBConnection, emailConfig *EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		var req struct {
			FilePath string `json:"file_path"`
		}

		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "Invalid request body"})
		}

		if req.FilePath == "" {
			req.FilePath = "/etc/pdvd/rbac.yaml"
		}

		yamlContent, err := os.ReadFile(req.FilePath)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to read file"})
		}

		config, err := LoadPeriobolosConfig(string(yamlContent))
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": err.Error()})
		}

		result, err := ApplyRBAC(db, config, emailConfig)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": err.Error()})
		}

		return c.JSON(fiber.Map{"success": true, "result": result})
	}
}

// GetRBACConfig exports current RBAC configuration from DB to YAML
func GetRBACConfig(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		config, err := ExportRBACConfig(db)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to export config"})
		}

		yamlData, err := yaml.Marshal(config)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to marshal config"})
		}

		c.Set("Content-Type", "application/x-yaml")
		return c.Send(yamlData)
	}
}

// PeriobolosConfig represents the RBAC configuration structure compatible with Peribolos-style YAML
type PeriobolosConfig struct {
	Orgs  []OrgDefinition  `yaml:"orgs,omitempty"`
	Users []PeriobolosUser `yaml:"users"`
	Roles []RoleDefinition `yaml:"roles,omitempty"`
}

// OrgDefinition represents an organization configuration
type OrgDefinition struct {
	Name        string            `yaml:"name"`
	DisplayName string            `yaml:"display_name,omitempty"`
	Description string            `yaml:"description,omitempty"`
	Metadata    map[string]string `yaml:"metadata,omitempty"`
	Members     []OrgMember       `yaml:"members,omitempty"` // Added Members list
}

// OrgMember represents a user member within an organization
type OrgMember struct {
	Username string `yaml:"username"`
	Role     string `yaml:"role"`
}

// PeriobolosUser represents a user configuration in the RBAC system
type PeriobolosUser struct {
	Username     string `yaml:"username"`
	Email        string `yaml:"email"`
	AuthProvider string `yaml:"auth_provider,omitempty"`
	// Note: Role and Orgs are removed from User definition in this schema option
}

// RoleDefinition represents a role configuration with associated permissions
type RoleDefinition struct {
	Name        string   `yaml:"name"`
	Description string   `yaml:"description,omitempty"`
	Permissions []string `yaml:"permissions,omitempty"`
}

// RBACResult contains the outcome of applying RBAC configuration
type RBACResult struct {
	OrgsCreated []string          `json:"orgs_created"`
	OrgsUpdated []string          `json:"orgs_updated"`
	Created     []string          `json:"created"`
	Updated     []string          `json:"updated"`
	Removed     []string          `json:"removed"`
	Invited     []string          `json:"invited"`
	Invitations map[string]string `json:"invitations,omitempty"`
}

// LoadPeriobolosConfig parses RBAC configuration from YAML content
func LoadPeriobolosConfig(yamlContent string) (*PeriobolosConfig, error) {
	var config PeriobolosConfig
	if err := yaml.Unmarshal([]byte(yamlContent), &config); err != nil {
		return nil, fmt.Errorf("failed to parse YAML: %w", err)
	}
	return &config, nil
}

// Helper to rank roles for flattening to DB model
func getRoleRank(role string) int {
	switch role {
	case "owner":
		return 4
	case "admin":
		return 3
	case "editor":
		return 2
	case "viewer":
		return 1
	default:
		return 0
	}
}

// ApplyRBAC implements Option 2: Org-Centric Sync with normalized org names
func ApplyRBAC(db database.DBConnection, config *PeriobolosConfig, emailConfig *EmailConfig) (*RBACResult, error) {
	ctx := context.Background()
	result := &RBACResult{
		OrgsCreated: []string{},
		OrgsUpdated: []string{},
		Created:     []string{},
		Updated:     []string{},
		Removed:     []string{},
		Invited:     []string{},
		Invitations: make(map[string]string),
	}

	// 1. Process Orgs & Build User Permissions Map
	// We iterate through orgs to find membership and calculate the highest role per user
	userOrgMap := make(map[string][]string)
	userMaxRole := make(map[string]string)

	for _, orgDef := range config.Orgs {
		// Normalize org name to lowercase for internal use
		normalizedOrgName := strings.ToLower(strings.TrimSpace(orgDef.Name))
		displayName := orgDef.DisplayName
		if displayName == "" {
			displayName = orgDef.Name // Use original if no display name provided
		}

		// Sync Org DB Record
		existingOrg, err := getOrgByName(ctx, db, normalizedOrgName)
		if err != nil {
			newOrg := &model.Org{
				Name:        normalizedOrgName, // Store lowercase
				DisplayName: displayName,       // Store display name
				Description: orgDef.Description,
				Metadata:    orgDef.Metadata,
				CreatedAt:   time.Now(),
				UpdatedAt:   time.Now(),
			}
			if err := createOrg(ctx, db, newOrg); err != nil {
				return nil, fmt.Errorf("failed to create org %s: %w", normalizedOrgName, err)
			}
			result.OrgsCreated = append(result.OrgsCreated, normalizedOrgName)
		} else {
			// Update existing org
			existingOrg.DisplayName = displayName
			existingOrg.Description = orgDef.Description
			existingOrg.Metadata = orgDef.Metadata
			existingOrg.UpdatedAt = time.Now()
			// Ensure name is normalized
			if existingOrg.Name != normalizedOrgName {
				existingOrg.Name = normalizedOrgName
			}
			if err := updateOrg(ctx, db, existingOrg); err != nil {
				return nil, fmt.Errorf("failed to update org %s: %w", normalizedOrgName, err)
			}
			result.OrgsUpdated = append(result.OrgsUpdated, normalizedOrgName)
		}

		// Map Users to this Org and calculate their max role
		for _, member := range orgDef.Members {
			// Store normalized org names in user mapping
			userOrgMap[member.Username] = append(userOrgMap[member.Username], normalizedOrgName)

			currentRank := getRoleRank(userMaxRole[member.Username])
			newRank := getRoleRank(member.Role)

			if newRank > currentRank {
				userMaxRole[member.Username] = member.Role
			}
		}
	}

	// 2. Sync Users (Iterate over the Identity list in YAML)
	existingUsers, err := listUsers(ctx, db)
	if err != nil {
		return nil, fmt.Errorf("failed to list users: %w", err)
	}

	existingUserMap := make(map[string]*model.User)
	for _, u := range existingUsers {
		existingUserMap[u.Username] = u
	}

	for _, configUser := range config.Users {
		username := configUser.Username
		orgs := userOrgMap[username]
		if orgs == nil {
			orgs = []string{}
		}

		// Ensure all org names are lowercase
		for i := range orgs {
			orgs[i] = strings.ToLower(strings.TrimSpace(orgs[i]))
		}

		role := userMaxRole[username]
		if role == "" {
			role = "viewer" // Default role if user is defined but has no org memberships
		}

		existingUser, exists := existingUserMap[username]

		if !exists {
			// Create User
			user := model.NewUser(username, role)
			user.Email = configUser.Email
			user.Orgs = orgs // Already normalized to lowercase
			user.IsActive = false
			user.Status = "pending"
			user.AuthProvider = configUser.AuthProvider
			if user.AuthProvider == "" {
				user.AuthProvider = "local"
			}

			randomPass, _ := GenerateSecureToken(32)
			hash, _ := HashPassword(randomPass)
			user.PasswordHash = hash

			if err := createUser(ctx, db, user); err != nil {
				return nil, fmt.Errorf("failed to create user %s: %w", username, err)
			}

			// Invitation
			inv, err := CreateInvitation(ctx, db, emailConfig, username, user.Email, role)
			if err == nil {
				result.Invited = append(result.Invited, username)
				result.Invitations[username] = inv.Token
			}
			result.Created = append(result.Created, username)

		} else {
			// Update User
			needsUpdate := false
			if existingUser.Email != configUser.Email || existingUser.Role != role || !stringSlicesEqual(existingUser.Orgs, orgs) {
				existingUser.Email = configUser.Email
				existingUser.Role = role
				existingUser.Orgs = orgs // Already normalized to lowercase
				needsUpdate = true
			}

			if needsUpdate {
				existingUser.UpdatedAt = time.Now()
				if err := updateUser(ctx, db, existingUser); err != nil {
					return nil, fmt.Errorf("failed to update user %s: %w", username, err)
				}
				result.Updated = append(result.Updated, username)
			}
		}
		delete(existingUserMap, username)
	}

	// 3. Remove Users not in YAML
	for username, user := range existingUserMap {
		user.IsActive = false
		user.Status = "removed"
		user.UpdatedAt = time.Now()
		if err := updateUser(ctx, db, user); err == nil {
			result.Removed = append(result.Removed, username)
		}
	}

	return result, nil
}

func stringSlicesEqual(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

func getOrgByName(ctx context.Context, db database.DBConnection, name string) (*model.Org, error) {
	// Normalize the search name to lowercase
	normalizedName := strings.ToLower(strings.TrimSpace(name))
	query := `FOR org IN orgs FILTER org.name == @name RETURN org`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"name": normalizedName},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var org model.Org
	if !cursor.HasMore() {
		return nil, fmt.Errorf("org not found")
	}
	_, err = cursor.ReadDocument(ctx, &org)
	return &org, err
}

func createOrg(ctx context.Context, db database.DBConnection, org *model.Org) error {
	_, err := db.Collections["orgs"].CreateDocument(ctx, org)
	return err
}

func updateOrg(ctx context.Context, db database.DBConnection, org *model.Org) error {
	_, err := db.Collections["orgs"].UpdateDocument(ctx, org.Key, org)
	return err
}
