// Package model - Sync defines the struct for release-endpoint associations
package model

import "time"

// Sync represents the association between a release and an endpoint (deployment)
type Sync struct {
	Key            string    `json:"_key,omitempty"`
	ReleaseName    string    `json:"release_name"`
	ReleaseVersion string    `json:"release_version"`
	EndpointName   string    `json:"endpoint_name"`
	SyncedAt       time.Time `json:"synced_at"`
	ObjType        string    `json:"objtype,omitempty"`
}

// NewSync creates a new Sync instance with default values
func NewSync() *Sync {
	return &Sync{
		ObjType:  "Sync",
		SyncedAt: time.Now(),
	}
}
