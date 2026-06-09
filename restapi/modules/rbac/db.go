// Package rbac provides shared database helpers for org and user management.
package rbac

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
)

// ============================================================================
// ORG DB HELPERS
// ============================================================================

// GetOrgByName retrieves an org document by normalized name.
func GetOrgByName(ctx context.Context, db database.DBConnection, name string) (*model.Org, error) {
	normalized := strings.ToLower(strings.TrimSpace(name))
	query := `FOR org IN orgs FILTER org.name == @name RETURN org`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"name": normalized},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	if !cursor.HasMore() {
		return nil, fmt.Errorf("org not found: %s", name)
	}
	var org model.Org
	if _, err := cursor.ReadDocument(ctx, &org); err != nil {
		return nil, err
	}
	return &org, nil
}

// CreateOrg inserts a new org document.
func CreateOrg(ctx context.Context, db database.DBConnection, org *model.Org) error {
	_, err := db.Collections["orgs"].CreateDocument(ctx, org)
	return err
}

// UpdateOrg updates an existing org document by key.
func UpdateOrg(ctx context.Context, db database.DBConnection, org *model.Org) error {
	_, err := db.Collections["orgs"].UpdateDocument(ctx, org.Key, org)
	return err
}

// ============================================================================
// USER DB HELPERS
// ============================================================================

// GetUserByUsername retrieves a user by username.
func GetUserByUsername(ctx context.Context, db database.DBConnection, username string) (*model.User, error) {
	query := `FOR u IN users FILTER u.username == @username RETURN u`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"username": username},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var user model.User
	if _, err := cursor.ReadDocument(ctx, &user); err != nil {
		return nil, fmt.Errorf("user not found")
	}
	return &user, nil
}

// ListUsers returns all users from the database.
func ListUsers(ctx context.Context, db database.DBConnection) ([]*model.User, error) {
	query := `FOR u IN users RETURN u`
	cursor, err := db.Database.Query(ctx, query, nil)
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var users []*model.User
	for cursor.HasMore() {
		var user model.User
		if _, err := cursor.ReadDocument(ctx, &user); err == nil {
			users = append(users, &user)
		}
	}
	return users, nil
}

// CreateUser inserts a new user document.
func CreateUser(ctx context.Context, db database.DBConnection, user *model.User) error {
	query := `
		INSERT {
			username: @username,
			email: @email,
			password_hash: @password_hash,
			role: @role,
			orgs: @orgs,
			is_active: @is_active,
			status: @status,
			auth_provider: @auth_provider,
			created_at: @created_at,
			updated_at: @updated_at,
			github_token: @github_token,
			github_installation_id: @github_installation_id
		} INTO users
	`
	bindVars := map[string]interface{}{
		"username":               user.Username,
		"email":                  user.Email,
		"password_hash":          user.PasswordHash,
		"role":                   user.Role,
		"orgs":                   user.Orgs,
		"is_active":              user.IsActive,
		"status":                 user.Status,
		"auth_provider":          user.AuthProvider,
		"created_at":             user.CreatedAt,
		"updated_at":             user.UpdatedAt,
		"github_token":           user.GitHubToken,
		"github_installation_id": user.GitHubInstallationID,
	}
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	return err
}

// UpdateUser updates an existing user document.
func UpdateUser(ctx context.Context, db database.DBConnection, user *model.User) error {
	query := `
		FOR u IN users
		FILTER u.username == @username
		UPDATE u WITH {
			email: @email,
			password_hash: @password_hash,
			role: @role,
			orgs: @orgs,
			is_active: @is_active,
			status: @status,
			updated_at: @updated_at,
			github_token: @github_token,
			github_installation_id: @github_installation_id
		} IN users
	`
	bindVars := map[string]interface{}{
		"username":               user.Username,
		"email":                  user.Email,
		"password_hash":          user.PasswordHash,
		"role":                   user.Role,
		"orgs":                   user.Orgs,
		"is_active":              user.IsActive,
		"status":                 user.Status,
		"updated_at":             time.Now(),
		"github_token":           user.GitHubToken,
		"github_installation_id": user.GitHubInstallationID,
	}
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	return err
}
