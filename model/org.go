// Package model defines the data models for the Ortelius backend.
package model

import "time"

// TrackedRepo represents a repository being tracked for scanning.
type TrackedRepo struct {
	Provider string    `json:"provider" yaml:"provider"` // "github" | "gitlab"
	Owner    string    `json:"owner" yaml:"owner"`       // github org/user or gitlab group
	Name     string    `json:"name" yaml:"name"`
	Private  bool      `json:"private,omitempty" yaml:"private,omitempty"`
	AddedBy  string    `json:"added_by,omitempty" yaml:"added_by,omitempty"`
	AddedAt  time.Time `json:"added_at,omitempty" yaml:"added_at,omitempty"`
}

// RepoKey returns a canonical string key for deduplication and hidden_repos matching.
func (r TrackedRepo) RepoKey() string {
	return r.Provider + "/" + r.Owner + "/" + r.Name
}

// Org represents an organization in the Ortelius system.
type Org struct {
	Key         string            `json:"_key,omitempty"`
	Name        string            `json:"name"`
	DisplayName string            `json:"display_name,omitempty"`
	Description string            `json:"description,omitempty"`
	IsPublic    bool              `json:"is_public,omitempty"` // true = auto-created for public repo tracking, no credentials
	Metadata    map[string]string `json:"metadata,omitempty"`
	CreatedAt   time.Time         `json:"created_at"`
	UpdatedAt   time.Time         `json:"updated_at"`

	// GitHub App installation — covers private + public repos, preferred auth method
	GitHubInstallationID string `json:"github_installation_id,omitempty"`

	// Encrypted PATs — only needed for private repos without app installation (GitHub)
	// or for GitLab repos. Never stored in plaintext.
	// Format: "enc:v1:<base64(nonce+ciphertext)>"
	GitHubTokenEnc string `json:"github_token_enc,omitempty"`
	GitLabTokenEnc string `json:"gitlab_token_enc,omitempty"`

	// Token validation state — updated by background job every 6 hours
	TokenStatus        string     `json:"token_status,omitempty"`         // "valid" | "expired" | "invalid" | "unverified"
	TokenLastValidated *time.Time `json:"token_last_validated,omitempty"` // nil if never validated

	// TrackedRepos is the source of truth for what gets scanned.
	// Populated by syncing from the RBAC YAML via ApplyRBAC.
	// Removing a repo here stops scanning.
	TrackedRepos []TrackedRepo `json:"tracked_repos,omitempty"`

	// HiddenRepos controls UI visibility only — does NOT affect scanning.
	// Format: "github/owner/name" or "gitlab/group/name"
	// Adding a repo via the UI removes it from this list.
	HiddenRepos []string `json:"hidden_repos,omitempty"`
}

// IsRepoHidden returns true if the given repo key is in the hidden list.
func (o *Org) IsRepoHidden(repoKey string) bool {
	for _, h := range o.HiddenRepos {
		if h == repoKey {
			return true
		}
	}
	return false
}

// RemoveFromHidden removes a repo key from the hidden list if present.
// Returns true if the key was found and removed.
func (o *Org) RemoveFromHidden(repoKey string) bool {
	for i, h := range o.HiddenRepos {
		if h == repoKey {
			o.HiddenRepos = append(o.HiddenRepos[:i], o.HiddenRepos[i+1:]...)
			return true
		}
	}
	return false
}

// HasTrackedRepo returns true if the repo is already in TrackedRepos.
func (o *Org) HasTrackedRepo(provider, owner, name string) bool {
	for _, r := range o.TrackedRepos {
		if r.Provider == provider && r.Owner == owner && r.Name == name {
			return true
		}
	}
	return false
}
