// Package model defines the data structures used by the pdvd-backend/v12,
// including releases, SBOMs, and endpoints.
package model

// PURL represents a package URL (base form without version)
// Used as a hub to connect CVEs and SBOMs
type PURL struct {
	Key     string `json:"_key,omitempty"`
	Purl    string `json:"purl"` // Base PURL without version (e.g., pkg:npm/lodash)
	ObjType string `json:"objtype"`
}

// NewPURL creates a new PURL instance
func NewPURL() *PURL {
	return &PURL{
		ObjType: "PURL",
	}
}
