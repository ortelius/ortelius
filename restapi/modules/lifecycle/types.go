// Package lifecycle defines the REST API types for CVE lifecycle tracking.
package lifecycle

import "time"

// CVEKey represents a unique CVE occurrence
type CVEKey struct {
	CveID       string
	Package     string
	ReleaseName string
}

// CurrentCVEInfo holds CVE information for current endpoint state
type CurrentCVEInfo struct {
	CVEKey
	SeverityRating string
	SeverityScore  float64
	Published      time.Time
	ReleaseVersion string
}

// CVEInfoTracking holds minimal CVE info for lifecycle tracking
type CVEInfoTracking struct {
	Package        string
	SeverityRating string
	SeverityScore  float64
	Published      time.Time
}

// CVEInfo holds CVE information including the CVE ID and release context
// Used by sync handler and OSV loader for creating lifecycle records
type CVEInfo struct {
	CVEID          string
	Package        string
	SeverityRating string
	SeverityScore  float64
	Published      time.Time
	ReleaseName    string // Context for lifecycle identification
	ReleaseVersion string // Context for lifecycle identification
}
