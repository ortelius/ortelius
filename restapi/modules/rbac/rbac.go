// Package rbac provides shared RBAC configuration parsing and application logic.
// It is a leaf package — it does not import auth, github, or any other restapi module,
// which allows both auth and github to import it without creating a cycle.
package rbac

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
	"gopkg.in/yaml.v2"
)

// RBACConfig represents the RBAC configuration structure.
type RBACConfig struct {
	Orgs  []OrgDefinition  `yaml:"orgs,omitempty"`
	Users []RBACUser       `yaml:"users"`
	Roles []RoleDefinition `yaml:"roles,omitempty"`
}

// OrgDefinition represents an organization in the RBAC YAML.
type OrgDefinition struct {
	Name         string                  `yaml:"name"`
	DisplayName  string                  `yaml:"display_name,omitempty"`
	Description  string                  `yaml:"description,omitempty"`
	Metadata     map[string]string       `yaml:"metadata,omitempty"`
	Members      []OrgMember             `yaml:"members,omitempty"`
	TrackedRepos []TrackedRepoDefinition `yaml:"tracked_repos,omitempty"`
}

// TrackedRepoDefinition represents a repository to track within an org.
type TrackedRepoDefinition struct {
	Provider string `yaml:"provider"` // "github" | "gitlab"
	Owner    string `yaml:"owner"`
	Name     string `yaml:"name"`
	Private  bool   `yaml:"private,omitempty"`
}

// OrgMember represents a user member within an organization.
type OrgMember struct {
	Username string `yaml:"username"`
	Role     string `yaml:"role"`
}

// RBACUser represents a user in the RBAC YAML.
type RBACUser struct {
	Username     string `yaml:"username"`
	Email        string `yaml:"email"`
	AuthProvider string `yaml:"auth_provider,omitempty"`
}

// RoleDefinition represents a role in the RBAC YAML.
type RoleDefinition struct {
	Name        string   `yaml:"name"`
	Description string   `yaml:"description,omitempty"`
	Permissions []string `yaml:"permissions,omitempty"`
}

// RBACResult contains the outcome of applying RBAC configuration.
type RBACResult struct {
	OrgsCreated []string          `json:"orgs_created"`
	OrgsUpdated []string          `json:"orgs_updated"`
	Created     []string          `json:"created"`
	Updated     []string          `json:"updated"`
	Removed     []string          `json:"removed"`
	Invited     []string          `json:"invited"`
	Invitations map[string]string `json:"invitations,omitempty"`
}

// UserCreator is a function type for creating a user and sending an invitation.
// Implemented by the auth package and injected to avoid an import cycle.
type UserCreator func(ctx context.Context, db database.DBConnection, username, email, role string, orgs []string) (invitationToken string, err error)

// LoadRBACConfig parses RBAC configuration from a YAML string.
func LoadRBACConfig(yamlContent string) (*RBACConfig, error) {
	var config RBACConfig
	if err := yaml.Unmarshal([]byte(yamlContent), &config); err != nil {
		return nil, fmt.Errorf("failed to parse YAML: %w", err)
	}
	return &config, nil
}

// getRoleRank returns a numeric rank for role comparison.
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

// ApplyRBAC syncs the parsed RBAC config to ArangoDB.
// createUser is injected from the auth package to handle password hashing
// and invitation sending without creating an import cycle.
func ApplyRBAC(db database.DBConnection, config *RBACConfig, createUser UserCreator) (*RBACResult, error) {
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

	// 1. Process orgs and build user → org/role mapping
	userOrgMap := make(map[string][]string)
	userMaxRole := make(map[string]string)

	for _, orgDef := range config.Orgs {
		normalizedOrgName := strings.ToLower(strings.TrimSpace(orgDef.Name))
		displayName := orgDef.DisplayName
		if displayName == "" {
			displayName = orgDef.Name
		}

		var trackedRepos []model.TrackedRepo
		for _, r := range orgDef.TrackedRepos {
			trackedRepos = append(trackedRepos, model.TrackedRepo{
				Provider: r.Provider,
				Owner:    r.Owner,
				Name:     r.Name,
				Private:  r.Private,
			})
		}

		existingOrg, err := GetOrgByName(ctx, db, normalizedOrgName)
		if err != nil {
			newOrg := &model.Org{
				Name:         normalizedOrgName,
				DisplayName:  displayName,
				Description:  orgDef.Description,
				Metadata:     orgDef.Metadata,
				TrackedRepos: trackedRepos,
				HiddenRepos:  []string{},
				CreatedAt:    time.Now(),
				UpdatedAt:    time.Now(),
			}
			if err := CreateOrg(ctx, db, newOrg); err != nil {
				return nil, fmt.Errorf("failed to create org %s: %w", normalizedOrgName, err)
			}
			result.OrgsCreated = append(result.OrgsCreated, normalizedOrgName)
		} else {
			// Sync tracked_repos from YAML — this is the source of truth for scanning.
			// hidden_repos is intentionally preserved (UI-only state).
			existingOrg.DisplayName = displayName
			existingOrg.Description = orgDef.Description
			existingOrg.Metadata = orgDef.Metadata
			existingOrg.TrackedRepos = trackedRepos
			existingOrg.UpdatedAt = time.Now()
			if existingOrg.Name != normalizedOrgName {
				existingOrg.Name = normalizedOrgName
			}
			if err := UpdateOrg(ctx, db, existingOrg); err != nil {
				return nil, fmt.Errorf("failed to update org %s: %w", normalizedOrgName, err)
			}
			result.OrgsUpdated = append(result.OrgsUpdated, normalizedOrgName)
		}

		for _, member := range orgDef.Members {
			userOrgMap[member.Username] = append(userOrgMap[member.Username], normalizedOrgName)
			currentRank := getRoleRank(userMaxRole[member.Username])
			newRank := getRoleRank(member.Role)
			if newRank > currentRank {
				userMaxRole[member.Username] = member.Role
			}
		}
	}

	// 2. Sync users
	existingUsers, err := ListUsers(ctx, db)
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
		for i := range orgs {
			orgs[i] = strings.ToLower(strings.TrimSpace(orgs[i]))
		}

		role := userMaxRole[username]
		if role == "" {
			role = "viewer"
		}

		existingUser, exists := existingUserMap[username]
		if !exists {
			// Delegate user creation + invitation to auth package via injected function
			token, err := createUser(ctx, db, username, configUser.Email, role, orgs)
			if err != nil {
				return nil, fmt.Errorf("failed to create user %s: %w", username, err)
			}
			if token != "" {
				result.Invited = append(result.Invited, username)
				result.Invitations[username] = token
			}
			result.Created = append(result.Created, username)
		} else {
			needsUpdate := false
			if existingUser.Email != configUser.Email ||
				existingUser.Role != role ||
				!stringSlicesEqual(existingUser.Orgs, orgs) {
				existingUser.Email = configUser.Email
				existingUser.Role = role
				existingUser.Orgs = orgs
				needsUpdate = true
			}
			if needsUpdate {
				existingUser.UpdatedAt = time.Now()
				if err := UpdateUser(ctx, db, existingUser); err != nil {
					return nil, fmt.Errorf("failed to update user %s: %w", username, err)
				}
				result.Updated = append(result.Updated, username)
			}
		}
		delete(existingUserMap, username)
	}

	// 3. Deactivate users no longer in YAML
	for username, user := range existingUserMap {
		user.IsActive = false
		user.Status = "removed"
		user.UpdatedAt = time.Now()
		if err := UpdateUser(ctx, db, user); err == nil {
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

// ExportRBACConfig exports the current users, roles, and orgs from ArangoDB as an RBACConfig.
func ExportRBACConfig(ctx context.Context, db database.DBConnection) (*RBACConfig, error) {
	config := &RBACConfig{
		Users: []RBACUser{},
		Roles: []RoleDefinition{},
		// Note: You can also query and append Orgs here if you want the full dump!
	}

	users, err := ListUsers(ctx, db)
	if err != nil {
		return nil, fmt.Errorf("failed to query users: %w", err)
	}
	for _, user := range users {
		config.Users = append(config.Users, RBACUser{
			Username:     user.Username,
			Email:        user.Email,
			AuthProvider: user.AuthProvider,
		})
	}

	roleQuery := `FOR r IN roles RETURN r`
	cursor, err := db.Database.Query(ctx, roleQuery, nil)
	if err != nil {
		return nil, fmt.Errorf("failed to query roles: %w", err)
	}
	defer cursor.Close()

	for cursor.HasMore() {
		var role RoleDefinition
		if _, err := cursor.ReadDocument(ctx, &role); err == nil {
			config.Roles = append(config.Roles, role)
		}
	}

	return config, nil
}
