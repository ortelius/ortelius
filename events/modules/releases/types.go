// Package release defines types for Kafka event processing of release SBOM creation events.
package release

import (
	"time"

	"github.com/ortelius/ortelius/v12/model"
)

// ReleaseSBOMCreatedEvent represents a release SBOM creation event published to Kafka.
//
//revive:disable-next-line:exported
type ReleaseSBOMCreatedEvent struct {
	EventType     string    `json:"event_type"`
	EventID       string    `json:"event_id"`
	EventTime     time.Time `json:"event_time"`
	SchemaVersion string    `json:"schema_version"`

	Release model.ProjectRelease `json:"release"`

	SBOMRef SBOMReference `json:"sbom_ref"`
}

// SBOMReference describes where an SBOM is stored and how it can be retrieved.
type SBOMReference struct {
	// IPFS CID returned by the SBOM ingestion Lambda
	CID string `json:"cid"`

	// Storage backend identifier (e.g. "ipfs+s3", "ipfs")
	StorageType string `json:"storage_type"`

	// Optional S3 details
	Bucket    string `json:"bucket,omitempty"`
	ObjectKey string `json:"object_key,omitempty"`

	// Optional integrity metadata
	ContentSha string `json:"content_sha,omitempty"`
	SizeBytes  int64  `json:"size_bytes,omitempty"`

	// Timestamp when the SBOM was persisted
	UploadedAt time.Time `json:"uploaded_at"`
}
