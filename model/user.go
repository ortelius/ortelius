// Package model provides data models for the PDVD system.
package model

import (
	"time"
)

// User represents a user in the system
type User struct {
	Key                  string    `json:"_key,omitempty"`
	Username             string    `json:"username"`
	Email                string    `json:"email"`
	PasswordHash         string    `json:"password_hash,omitempty"`
	Role                 string    `json:"role"` // admin, editor, viewer
	Orgs                 []string  `json:"orgs"` // List of orgs user can access
	IsActive             bool      `json:"is_active"`
	Status               string    `json:"status"`                           // pending, active, inactive
	AuthProvider         string    `json:"auth_provider"`                    // local, oidc
	ExternalID           string    `json:"external_id,omitempty"`            // For OIDC
	GitHubToken          string    `json:"github_token,omitempty"`           // GitHub User OAuth Token (optional/legacy)
	GitHubInstallationID string    `json:"github_installation_id,omitempty"` // GitHub App Installation ID
	CreatedAt            time.Time `json:"created_at"`
	UpdatedAt            time.Time `json:"updated_at"`
}

// NewUser creates a new user with default values
func NewUser(username, role string) *User {
	now := time.Now()
	return &User{
		Username:     username,
		Role:         role,
		Orgs:         []string{}, // Empty by default (global access)
		IsActive:     true,
		Status:       "pending", // Default to pending until invitation accepted
		AuthProvider: "local",
		CreatedAt:    now,
		UpdatedAt:    now,
	}
}

// HasOrgAccess checks if user has access to a specific org
func (u *User) HasOrgAccess(org string) bool {
	if len(u.Orgs) == 0 {
		return true
	}
	for _, userOrg := range u.Orgs {
		if userOrg == org {
			return true
		}
	}
	return false
}

// GetOrgFilter returns AQL filter clause for org-scoped queries
func (u *User) GetOrgFilter(alias string) string {
	if len(u.Orgs) == 0 {
		return ""
	}
	return " AND " + alias + ".org IN @userOrgs"
}

// HasPermission checks if user has a specific permission
func (u *User) HasPermission(permission string) bool {
	switch u.Role {
	case "admin":
		return true
	case "editor":
		if permission == "read" || permission == "write" {
			return true
		}
	case "viewer":
		if permission == "read" {
			return true
		}
	}
	return false
}

// IsAdmin returns true if user is admin
func (u *User) IsAdmin() bool {
	return u.Role == "admin"
}

// CanWrite returns true if user can write
func (u *User) CanWrite() bool {
	return u.Role == "admin" || u.Role == "editor"
}

// CanRead returns true if user can read
func (u *User) CanRead() bool {
	return u.Role == "admin" || u.Role == "editor" || u.Role == "viewer"
}

// Invitation represents a user invitation
type Invitation struct {
	Key         string     `json:"_key,omitempty"`
	Username    string     `json:"username"`
	Email       string     `json:"email"`
	Token       string     `json:"token"`
	Role        string     `json:"role"`
	ExpiresAt   time.Time  `json:"expires_at"`
	AcceptedAt  *time.Time `json:"accepted_at,omitempty"`
	CreatedAt   time.Time  `json:"created_at"`
	ResendCount int        `json:"resend_count"`
}

// NewInvitation creates a new invitation
func NewInvitation(username, email, token, role string) *Invitation {
	now := time.Now()
	return &Invitation{
		Username:    username,
		Email:       email,
		Token:       token,
		Role:        role,
		ExpiresAt:   now.Add(48 * time.Hour),
		CreatedAt:   now,
		ResendCount: 0,
	}
}

// IsExpired checks if invitation has expired
func (i *Invitation) IsExpired() bool {
	return time.Now().After(i.ExpiresAt)
}

// IsAccepted checks if invitation has been accepted
func (i *Invitation) IsAccepted() bool {
	return i.AcceptedAt != nil
}
