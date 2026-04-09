// Package model - SBOM defines the struct and handles marshaling/unmarshaling the struct to/from NFT Storage.
package model

import "encoding/json"

// SBOM defines a CycloneDX SBOM in JSON format
type SBOM struct {
	Key        string          `json:"_key,omitempty"`
	ContentSha string          `json:"contentsha,omitempty"` // SHA256 hash of content for deduplication
	CID        string          `json:"cid,omitempty"`
	ObjType    string          `json:"objtype,omitempty"`
	Content    json.RawMessage `json:"content"`
}

// NewSBOM is the contructor that sets the appropriate default values
func NewSBOM() *SBOM {
	return &SBOM{ObjType: "SBOM"}
}
