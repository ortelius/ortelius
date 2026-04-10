// Package release handles Kafka event processing for release SBOM creation events.
package release

import (
	"context"
	"encoding/json"
	"fmt"
	"log"

	"github.com/ortelius/ortelius/v12/model"
)

// SBOMFetcher defines the interface for fetching SBOM content from storage.
type SBOMFetcher interface {
	FetchSBOM(ctx context.Context, cid string) ([]byte, error)
}

// ReleaseService defines the interface for release service operations.
//
//revive:disable-next-line:exported
type ReleaseService interface {
	CreateRelease(ctx context.Context, release model.ReleaseWithSBOM) error
}

// HandleReleaseSBOMCreatedWithService processes release SBOM created events from Kafka.
func HandleReleaseSBOMCreatedWithService(
	ctx context.Context,
	msg []byte,
	fetcher SBOMFetcher,
	service ReleaseService,
) error {
	var event ReleaseSBOMCreatedEvent
	if err := json.Unmarshal(msg, &event); err != nil {
		return fmt.Errorf("failed to unmarshal ReleaseSBOMCreatedEvent: %w", err)
	}

	if event.Release.Name == "" || event.Release.Version == "" || event.SBOMRef.CID == "" {
		return fmt.Errorf("invalid event: missing required fields")
	}

	log.Printf("Processing release %s@%s (CID=%s)", event.Release.Name, event.Release.Version, event.SBOMRef.CID)

	sbomData, err := fetcher.FetchSBOM(ctx, event.SBOMRef.CID)
	if err != nil {
		return fmt.Errorf("failed to fetch SBOM for CID %s: %w", event.SBOMRef.CID, err)
	}

	releaseWithSBOM := model.ReleaseWithSBOM{
		ProjectRelease: model.ProjectRelease{
			Name:        event.Release.Name,
			Version:     event.Release.Version,
			GitCommit:   event.Release.GitCommit,
			DockerSha:   event.Release.DockerSha,
			ProjectType: event.Release.ProjectType,
			IsPublic:    event.Release.IsPublic,
		},
		SBOM: model.SBOM{
			Content: sbomData,
		},
	}

	releaseWithSBOM.ParseAndSetNameComponents()
	releaseWithSBOM.ParseAndSetVersion()

	if err := service.CreateRelease(ctx, releaseWithSBOM); err != nil {
		return fmt.Errorf("internal service error: %w", err)
	}

	log.Printf("Successfully processed release %s@%s", event.Release.Name, event.Release.Version)
	return nil
}
