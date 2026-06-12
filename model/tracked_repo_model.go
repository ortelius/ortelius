// Package model defines the system-level tracked repo record.
package model

import "time"

// SystemTrackedRepo is a system-level scan entry for a public repository.
// Unlike org-scoped tracked repos, these are shared resources — any authenticated
// user can add one, but removal is gated on active sync usage.
//
// Ownership model (mirrors how npm/GitHub works):
//   - Any authenticated user can ADD a public repo
//   - The adding user's org becomes the steward (added_by_org)
//   - REMOVE is blocked if active_sync_count > 0 (other orgs depend on it)
//   - Admins can force-remove regardless of sync count
//   - Private repos still go through org-scoped tracked_repos on the Org doc
type SystemTrackedRepo struct {
	Key      string `json:"_key,omitempty"`
	ObjType  string `json:"objtype"`
	Provider string `json:"provider"` // "github" | "gitlab"
	Owner    string `json:"owner"`    // github org/user or gitlab group — native, not Ortelius org
	Name     string `json:"name"`     // repo name

	// Stewardship — who added it and which Ortelius org is responsible for the scan entry
	AddedBy    string    `json:"added_by"`
	AddedByOrg string    `json:"added_by_org"` // Ortelius org of the adder
	AddedAt    time.Time `json:"added_at"`

	// Private is always false for system-tracked repos — private repos use org-scoped tracking
	Private bool `json:"private"`
}

// RepoKey returns the canonical deduplication key.
func (r SystemTrackedRepo) RepoKey() string {
	return r.Provider + "/" + r.Owner + "/" + r.Name
}
