// Package services provides internal service implementations for the PDVD backend.
package services

import (
	"context"
	"fmt"
	"log"

	release "github.com/ortelius/pdvd-backend/v12/events/modules/releases"
)

// CIDFetcher implements release.SBOMFetcher
type CIDFetcher struct{}

// FetchSBOM retrieves the SBOM content by CID (as bytes)
func (f *CIDFetcher) FetchSBOM(_ context.Context, cid string) ([]byte, error) {
	log.Printf("Fetching SBOM for CID: %s", cid)

	if cid == "" {
		return nil, fmt.Errorf("CID is empty")
	}

	// In production, fetch the SBOM from S3/IPFS etc.
	// For now, return placeholder JSON bytes
	return []byte(`{"components":[]}`), nil
}

// Ensure compile-time interface check
var _ release.SBOMFetcher = (*CIDFetcher)(nil)
