// Package services provides internal service implementations for the PDVD backend.
package services

import (
	"context"
	"log"

	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
	"github.com/ortelius/pdvd-backend/v12/restapi/modules/releases"
)

// ReleaseServiceWrapper implements release.ReleaseService
type ReleaseServiceWrapper struct {
	DB database.DBConnection
}

// CreateRelease processes a release and its SBOM by delegating to the
// shared core logic in the releases module. This ensures that Kafka-driven
// ingestion performs the same metadata standardization, deduplication,
// and CVE linking as the REST API.
func (w *ReleaseServiceWrapper) CreateRelease(ctx context.Context, rel model.ReleaseWithSBOM) error {
	log.Printf("Worker: Processing release creation for %s@%s", rel.Name, rel.Version)

	// Use the shared ProcessReleaseIngestion function from the releases module.
	// This function handles:
	// 1. Metadata standardization (Name/Version/ContentSha)
	// 2. Existence checks via composite key
	// 3. SBOM content processing and hashing
	// 4. SBOM relationship mapping
	// 5. Component/PURL extraction
	// 6. Automated CVE linking (Static Analysis)
	_, err := releases.ProcessReleaseIngestion(ctx, w.DB, rel)
	return err
}
