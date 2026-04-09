// Package releases implements the REST API handlers for release operations.
package releases

import (
	"context"
	"encoding/json"
	"fmt"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/google/osv-scanner/pkg/models"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
	"github.com/ortelius/pdvd-backend/v12/restapi/modules/sbom"
	"github.com/ortelius/pdvd-backend/v12/util"
)

// ProcessReleaseIngestion encapsulates the core logic for saving a release,
// processing its SBOM, and linking it to CVEs. This function is shared by
// both the REST API handler and the Kafka event processor to ensure identical behavior.
func ProcessReleaseIngestion(ctx context.Context, db database.DBConnection, rel model.ReleaseWithSBOM) (string, error) {
	// 1. Standardize Metadata
	rel.ParseAndSetVersion()
	rel.ParseAndSetNameComponents()
	PopulateContentSha(&rel.ProjectRelease)

	if rel.ContentSha == "" {
		return "", fmt.Errorf("ContentSha is required (GitCommit or DockerSha must be provided)")
	}

	// 2. Check for existing release by composite natural key (name + version + contentsha)
	existingReleaseKey, err := database.FindReleaseByCompositeKey(ctx, db.Database,
		rel.Name,
		rel.Version,
		rel.ContentSha,
	)
	if err != nil {
		return "", fmt.Errorf("failed to check for existing release: %w", err)
	}

	var releaseID string
	if existingReleaseKey != "" {
		// Release already exists, use existing key
		releaseID = "release/" + existingReleaseKey
		rel.Key = existingReleaseKey
	} else {
		// Save new ProjectRelease to ArangoDB
		releaseMeta, err := db.Collections["release"].CreateDocument(ctx, rel.ProjectRelease)
		if err != nil {
			return "", fmt.Errorf("failed to save release: %w", err)
		}
		releaseID = "release/" + releaseMeta.Key
		rel.Key = releaseMeta.Key
	}

	// 3. Process SBOM (handles deduplication via content hashing)
	_, sbomID, err := sbom.ProcessSBOM(ctx, db, rel.SBOM)
	if err != nil {
		return "", fmt.Errorf("failed to process SBOM: %w", err)
	}

	// 4. Update Relationship (Delete old edges and create new one)
	if err := DeleteRelease2SBOMEdges(ctx, db, releaseID); err != nil {
		return "", err
	}

	edge := map[string]interface{}{
		"_from": releaseID,
		"_to":   sbomID,
	}
	if _, err := db.Collections["release2sbom"].CreateDocument(ctx, edge); err != nil {
		return "", fmt.Errorf("failed to create release-sbom relationship: %w", err)
	}

	// 5. Process SBOM components and create PURL relationships
	if err := sbom.ProcessSBOMComponents(ctx, db, rel.SBOM, sbomID); err != nil {
		return "", fmt.Errorf("failed to process SBOM components: %w", err)
	}

	// 6. Link Release directly to CVEs (Materialized Edges for static analysis)
	if err := deleteRelease2CVEEdges(ctx, db, releaseID); err != nil {
		fmt.Printf("Warning: Failed to cleanup old CVE edges: %v\n", err)
	}

	if err := linkReleaseToExistingCVEs(ctx, db, releaseID, rel.Key); err != nil {
		return "", fmt.Errorf("error linking release to CVEs: %w", err)
	}

	return releaseID, nil
}

// PostReleaseWithSBOM handles POST requests for creating a release with its SBOM.
// It delegates core processing to ProcessReleaseIngestion.
func PostReleaseWithSBOM(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		var req model.ReleaseWithSBOM

		// Parse request body
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"message": "Invalid request body: " + err.Error(),
			})
		}

		// Validate required fields
		if req.Name == "" || req.Version == "" || len(req.SBOM.Content) == 0 {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"message": "Release name, version, and SBOM content are required",
			})
		}

		// Validate SBOM content is valid JSON
		var sbomContent interface{}
		if err := json.Unmarshal(req.SBOM.Content, &sbomContent); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"message": "SBOM content must be valid JSON: " + err.Error(),
			})
		}

		// Set default ObjTypes
		if req.ObjType == "" {
			req.ObjType = "ProjectRelease"
		}
		if req.SBOM.ObjType == "" {
			req.SBOM.ObjType = "SBOM"
		}

		ctx := context.Background()
		_, err := ProcessReleaseIngestion(ctx, db, req)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"success": false,
				"message": err.Error(),
			})
		}

		return c.Status(fiber.StatusCreated).JSON(fiber.Map{
			"success": true,
			"message": "Release and SBOM processed successfully",
		})
	}
}

// PopulateContentSha sets the ContentSha field based on project type.
func PopulateContentSha(release *model.ProjectRelease) {
	if release.ProjectType == "docker" || release.ProjectType == "container" {
		if release.DockerSha != "" {
			release.ContentSha = release.DockerSha
		} else if release.GitCommit != "" {
			release.ContentSha = release.GitCommit
		}
	} else {
		if release.GitCommit != "" {
			release.ContentSha = release.GitCommit
		} else if release.DockerSha != "" {
			release.ContentSha = release.DockerSha
		}
	}
}

// DeleteRelease2SBOMEdges deletes all existing release2sbom edges for a given release.
func DeleteRelease2SBOMEdges(ctx context.Context, db database.DBConnection, releaseID string) error {
	query := `
		FOR e IN release2sbom
			FILTER e._from == @releaseID
			REMOVE e IN release2sbom
	`
	bindVars := map[string]interface{}{
		"releaseID": releaseID,
	}

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: bindVars,
	})
	if err != nil {
		return err
	}
	cursor.Close()

	return nil
}

// deleteRelease2CVEEdges deletes all existing release2cve edges for a given release.
func deleteRelease2CVEEdges(ctx context.Context, db database.DBConnection, releaseID string) error {
	query := `
		FOR e IN release2cve
			FILTER e._from == @releaseID
			REMOVE e IN release2cve
	`
	bindVars := map[string]interface{}{
		"releaseID": releaseID,
	}

	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: bindVars,
	})
	return err
}

// linkReleaseToExistingCVEs finds matching CVEs for a release and creates materialized edges.
func linkReleaseToExistingCVEs(ctx context.Context, db database.DBConnection, releaseID, releaseKey string) error {
	query := `
		FOR r IN release
			FILTER r._key == @releaseKey
			
			FOR sbom IN 1..1 OUTBOUND r release2sbom
				FOR sbomEdge IN sbom2purl
					FILTER sbomEdge._from == sbom._id
					LET purl = DOCUMENT(sbomEdge._to)
					FILTER purl != null
					
					FOR cveEdge IN cve2purl
						FILTER cveEdge._to == purl._id
						
						LET cve = DOCUMENT(cveEdge._from)
						FILTER cve != null
						
						LET matchedAffected = (
							FOR affected IN cve.affected != null ? cve.affected : []
								LET cveBasePurl = affected.package.purl != null ? 
									affected.package.purl : 
									CONCAT("pkg:", LOWER(affected.package.ecosystem), "/", affected.package.name)
								FILTER cveBasePurl == purl.purl
								RETURN affected
						)
						FILTER LENGTH(matchedAffected) > 0
						
						RETURN {
							cve_id: cve._id,
							cve_doc_id: cve.id,
							package_purl_full: sbomEdge.full_purl,
							package_purl_base: purl.purl,
							package_version: sbomEdge.version,
							all_affected: matchedAffected
						}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"releaseKey": releaseKey,
		},
	})
	if err != nil {
		return err
	}
	defer cursor.Close()

	var edgesToInsert []map[string]interface{}

	type Candidate struct {
		CveID           string            `json:"cve_id"`
		CveDocID        string            `json:"cve_doc_id"`
		PackagePurlFull string            `json:"package_purl_full"`
		PackagePurlBase string            `json:"package_purl_base"`
		PackageVersion  string            `json:"package_version"`
		AllAffected     []models.Affected `json:"all_affected"`
	}

	seenInstances := make(map[string]bool)

	for cursor.HasMore() {
		var cand Candidate
		if _, err := cursor.ReadDocument(ctx, &cand); err != nil {
			continue
		}

		// Deduplication using base PURL
		instanceKey := cand.CveID + ":" + cand.PackagePurlBase
		if seenInstances[instanceKey] {
			continue
		}

		// Always validate using ecosystem-specific parsers
		isAffected := false
		for _, affected := range cand.AllAffected {
			if util.IsVersionAffected(cand.PackageVersion, affected) {
				isAffected = true
				break
			}
		}
		if !isAffected {
			continue
		}

		seenInstances[instanceKey] = true

		edgesToInsert = append(edgesToInsert, map[string]interface{}{
			"_from":           releaseID,
			"_to":             cand.CveID,
			"type":            "static_analysis",
			"package_purl":    cand.PackagePurlFull,
			"package_base":    cand.PackagePurlBase,
			"package_version": cand.PackageVersion,
			"created_at":      time.Now(),
		})
	}

	if len(edgesToInsert) > 0 {
		return sbom.BatchInsertEdges(ctx, db, "release2cve", edgesToInsert)
	}

	return nil
}
