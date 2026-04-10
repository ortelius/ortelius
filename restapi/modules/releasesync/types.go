// Package releasesync defines the REST API types for sync operations.
package releasesync

// Request represents the request body for creating a sync
type Request struct {
	ReleaseName    string `json:"release_name"`
	ReleaseVersion string `json:"release_version"`
	EndpointName   string `json:"endpoint_name"`
	SyncStatus     string `json:"sync_status,omitempty"`
	SyncMessage    string `json:"sync_message,omitempty"`
}

// Response represents the response for sync operations
type Response struct {
	Success bool   `json:"success"`
	Message string `json:"message"`
	SyncKey string `json:"sync_key,omitempty"`
}

// ReleaseResult tracks the outcome of processing each release
type ReleaseResult struct {
	Name    string `json:"name"`
	Version string `json:"version"`
	Status  string `json:"status"` // "synced", "created", "created_with_sbom", "updated", "updated_with_sbom", "unchanged", "error"
	SyncKey string `json:"sync_key,omitempty"`
	Message string `json:"message"`
}

// ReleaseKey represents a unique release identifier
type ReleaseKey struct {
	Name    string `json:"name"`
	Version string `json:"version"`
}

// ServiceState represents a service in sync inventory
type ServiceState struct {
	Version  string `json:"version"`
	SyncedAt string `json:"synced_at"`
}

// ServiceInventory represents service metadata
type ServiceInventory struct {
	Name       string        `json:"name"`
	Current    ServiceState  `json:"current"`
	Previous   *ServiceState `json:"previous"`
	ReleaseDoc struct {
		ScorecardScore *float64 `json:"openssf_scorecard_score"`
		ProjectType    string   `json:"project_type"`
	} `json:"release_doc"`
}

// ReleaseMetadata holds full release metadata
type ReleaseMetadata struct {
	Name              string `json:"name"`
	Version           string `json:"version"`
	VersionMajor      *int   `json:"version_major"`
	VersionMinor      *int   `json:"version_minor"`
	VersionPatch      *int   `json:"version_patch"`
	VersionPrerelease string `json:"version_prerelease"`
}
