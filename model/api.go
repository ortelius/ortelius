// Package model - API types for combining models in API requests/responses
package model

import "time"

// ReleaseWithSBOM combines ProjectRelease and SBOM for API communication
type ReleaseWithSBOM struct {
	ProjectRelease
	SBOM SBOM `json:"sbom"`
}

// SyncWithEndpoint combines releases and endpoint for bulk sync API communication
type SyncWithEndpoint struct {
	EndpointName string        `json:"endpoint_name"`
	Releases     []ReleaseSync `json:"releases"`
	Endpoint     Endpoint      `json:"endpoint"`
	// ADDED: Field to allow clients to specify backdated sync times
	SyncedAt time.Time `json:"synced_at,omitempty"`
}

// ReleaseSync represents a release with optional SBOM in the sync request
type ReleaseSync struct {
	Release ProjectRelease `json:"release"`
	SBOM    *SBOM          `json:"sbom,omitempty"` // Optional SBOM
}

// Release represents a release in the sync request (legacy, kept for compatibility)
type Release struct {
	Name    string `json:"name"`
	Version string `json:"version"`
}

// AffectedRelease represents a release that is affected by a CVE
type AffectedRelease struct {
	CveID           string   `json:"cve_id"`
	Summary         string   `json:"summary"`
	Details         string   `json:"details"`
	Severity        string   `json:"severity"`
	SeverityRating  string   `json:"severity_rating"`
	Published       string   `json:"published"`
	Modified        string   `json:"modified"`
	Aliases         []string `json:"aliases"`
	Package         string   `json:"package"`
	AffectedVersion string   `json:"affected_version"`
	FullPurl        string   `json:"full_purl"`
	ReleaseName     string   `json:"release_name"`
	ReleaseVersion  string   `json:"release_version"`
	ContentSha      string   `json:"content_sha"`
	ProjectType     string   `json:"project_type"`
}

// AffectedEndpoint represents an endpoint that is affected by a CVE
type AffectedEndpoint struct {
	CveID           string    `json:"cve_id"`
	Summary         string    `json:"summary"`
	Details         string    `json:"details"`
	Severity        string    `json:"severity"`
	Published       string    `json:"published"`
	Modified        string    `json:"modified"`
	Aliases         []string  `json:"aliases"`
	Package         string    `json:"package"`
	AffectedVersion string    `json:"affected_version"`
	FullPurl        string    `json:"full_purl"`
	ReleaseName     string    `json:"release_name"`
	ReleaseVersion  string    `json:"release_version"`
	ContentSha      string    `json:"content_sha"`
	ProjectType     string    `json:"project_type"`
	EndpointName    string    `json:"endpoint_name"`
	EndpointType    string    `json:"endpoint_type"`
	Environment     string    `json:"environment"`
	SyncedAt        time.Time `json:"synced_at"`
}
