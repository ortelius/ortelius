// Package releases implements the REST API handlers for release operations.
package releases

import (
	"context"
	"encoding/json"
	"fmt"
	"strconv"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/google/osv-scanner/pkg/models"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
	"github.com/ortelius/ortelius/v12/restapi/modules/sbom"
	"github.com/ortelius/ortelius/v12/util"
)

// ProcessReleaseIngestion encapsulates the core logic for saving a release,
// processing its SBOM, and linking it to CVEs.
// FIXED: Now evaluates and flags the "latest" version at write-time
func ProcessReleaseIngestion(ctx context.Context, db database.DBConnection, rel model.ReleaseWithSBOM) (string, error) {
	// 1. Standardize Metadata
	rel.ParseAndSetVersion()
	rel.ParseAndSetNameComponents()
	PopulateContentSha(&rel.ProjectRelease)

	if rel.ContentSha == "" {
		return "", fmt.Errorf("ContentSha is required (GitCommit or DockerSha must be provided)")
	}

	// Use the existing latest marker as the fast-path read. A full-family
	// recompute only runs when this release may become latest.
	existingLatest, err := getLatestRelease(ctx, db, rel.Name)
	if err != nil {
		return "", fmt.Errorf("failed to fetch current latest release: %w", err)
	}

	isNewLatest := existingLatest == nil || isVersionGreater(rel.ProjectRelease, *existingLatest)
	rel.IsLatest = isNewLatest

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

	// If this release may be the new latest, recompute the full release family.
	// This clears stale latest flags and promotes exactly one release. Older
	// releases stay on the fast path and avoid the full scan.
	if isNewLatest {
		if err := recomputeLatestRelease(ctx, db, rel.Name); err != nil {
			return "", fmt.Errorf("failed to recompute latest release: %w", err)
		}
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

	if err := LinkReleaseToExistingCVEs(ctx, db, releaseID, rel.Key); err != nil {
		return "", fmt.Errorf("error linking release to CVEs: %w", err)
	}

	return releaseID, nil
}

// ============================================================================
// VERSION EVALUATION HELPERS
// ============================================================================

// getLatestRelease finds the current latest release for a given package name.
// This intentionally uses the is_latest flag as a fast-path lookup for normal
// ingestion checks. The full release-family scan is only done by
// recomputeLatestRelease when a release may supersede the current latest.
func getLatestRelease(ctx context.Context, db database.DBConnection, name string) (*model.ProjectRelease, error) {
	query := `
		FOR r IN release
			FILTER r.name == @name AND r.is_latest == true
			LIMIT 1
			RETURN r
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"name": name},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	if cursor.HasMore() {
		var rel model.ProjectRelease
		if _, err := cursor.ReadDocument(ctx, &rel); err != nil {
			return nil, err
		}
		return &rel, nil
	}

	return nil, nil
}

// recomputeLatestRelease recalculates the latest release for a package name from
// persisted release records. It does not trust existing is_latest flags because
// historical duplicate SHA/tag ingestion can leave more than one stale latest.
func recomputeLatestRelease(ctx context.Context, db database.DBConnection, name string) error {
	query := `
		FOR r IN release
			FILTER r.name == @name
			RETURN r
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"name": name},
	})
	if err != nil {
		return err
	}
	defer cursor.Close()

	var latest *model.ProjectRelease
	for cursor.HasMore() {
		var rel model.ProjectRelease
		if _, err := cursor.ReadDocument(ctx, &rel); err != nil {
			return err
		}
		if latest == nil || isVersionGreater(rel, *latest) {
			latest = &rel
		}
	}

	if latest == nil || latest.Key == "" {
		return nil
	}

	updateQuery := `
		FOR r IN release
			FILTER r.name == @name
			UPDATE r WITH { is_latest: r._key == @latestKey } IN release
	`
	updateCursor, err := db.Database.Query(ctx, updateQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"name":      name,
			"latestKey": latest.Key,
		},
	})
	if err != nil {
		return err
	}
	defer updateCursor.Close()

	return nil
}

// isVersionGreater compares two releases and returns true if newRel is strictly greater.
func isVersionGreater(newRel, existingRel model.ProjectRelease) bool {
	getVal := func(ptr *int) int {
		if ptr == nil {
			return -1 // Unset versions evaluated as lowest possible value
		}
		return *ptr
	}

	nMajor, eMajor := getVal(newRel.VersionMajor), getVal(existingRel.VersionMajor)
	if nMajor != eMajor {
		return nMajor > eMajor
	}

	nMinor, eMinor := getVal(newRel.VersionMinor), getVal(existingRel.VersionMinor)
	if nMinor != eMinor {
		return nMinor > eMinor
	}

	nPatch, ePatch := getVal(newRel.VersionPatch), getVal(existingRel.VersionPatch)
	if nPatch != ePatch {
		return nPatch > ePatch
	}

	// Stable releases are greater than pre-releases.
	if newRel.VersionPrerelease == "" && existingRel.VersionPrerelease != "" {
		return true
	}
	if newRel.VersionPrerelease != "" && existingRel.VersionPrerelease == "" {
		return false
	}

	return comparePrerelease(newRel.VersionPrerelease, existingRel.VersionPrerelease) > 0
}

func comparePrerelease(a, b string) int {
	if a == b {
		return 0
	}

	aParts := splitPrerelease(a)
	bParts := splitPrerelease(b)
	maxLen := len(aParts)
	if len(bParts) > maxLen {
		maxLen = len(bParts)
	}

	for i := 0; i < maxLen; i++ {
		if i >= len(aParts) {
			return -1
		}
		if i >= len(bParts) {
			return 1
		}

		cmp := comparePrereleaseIdentifier(aParts[i], bParts[i])
		if cmp != 0 {
			return cmp
		}
	}

	return 0
}

func splitPrerelease(value string) []string {
	return strings.FieldsFunc(value, func(r rune) bool {
		return r == '.' || r == '-' || r == '_'
	})
}

func comparePrereleaseIdentifier(a, b string) int {
	aNum, aErr := strconv.Atoi(a)
	bNum, bErr := strconv.Atoi(b)

	if aErr == nil && bErr == nil {
		if aNum > bNum {
			return 1
		}
		if aNum < bNum {
			return -1
		}
		return 0
	}

	if aErr == nil && bErr != nil {
		return -1 // semver: numeric identifiers have lower precedence than non-numeric identifiers
	}
	if aErr != nil && bErr == nil {
		return 1
	}

	if a > b {
		return 1
	}
	if a < b {
		return -1
	}
	return 0
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

// LinkReleaseToExistingCVEs finds matching CVEs for a release and creates materialized edges.
// This is exported so that it can be shared with the releasesync module.
func LinkReleaseToExistingCVEs(ctx context.Context, db database.DBConnection, releaseID, releaseKey string) error {
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
						
						FILTER (
							sbomEdge.version_major != null AND 
							cveEdge.introduced_major != null AND 
							(cveEdge.fixed_major != null OR cveEdge.last_affected_major != null)
						) ? (
							(sbomEdge.version_major > cveEdge.introduced_major OR
							 (sbomEdge.version_major == cveEdge.introduced_major AND 
							  sbomEdge.version_minor > cveEdge.introduced_minor) OR
							 (sbomEdge.version_major == cveEdge.introduced_major AND 
							  sbomEdge.version_minor == cveEdge.introduced_minor AND 
							  sbomEdge.version_patch >= cveEdge.introduced_patch))
							AND
							(cveEdge.fixed_major != null ? (
								sbomEdge.version_major < cveEdge.fixed_major OR
								(sbomEdge.version_major == cveEdge.fixed_major AND 
								 sbomEdge.version_minor < cveEdge.fixed_minor) OR
								(sbomEdge.version_major == cveEdge.fixed_major AND 
								 sbomEdge.version_minor == cveEdge.fixed_minor AND 
								 sbomEdge.version_patch < cveEdge.fixed_patch)
							) : (
								sbomEdge.version_major < cveEdge.last_affected_major OR
								(sbomEdge.version_major == cveEdge.last_affected_major AND 
								 sbomEdge.version_minor < cveEdge.last_affected_minor) OR
								(sbomEdge.version_major == cveEdge.last_affected_major AND 
								 sbomEdge.version_minor == cveEdge.last_affected_minor AND 
								 sbomEdge.version_patch <= cveEdge.last_affected_patch)
							))
						) : true
						
						LET cve = DOCUMENT(cveEdge._from)
						FILTER cve != null
						
						RETURN {
							cve_id: cve._id,
							cve_doc_id: cve.id,
							package_purl_full: sbomEdge.full_purl,
							package_purl_base: purl.purl,
							package_version: sbomEdge.version,
							needs_validation: sbomEdge.version_major == null OR cveEdge.introduced_major == null
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
		CveID           string `json:"cve_id"`
		CveDocID        string `json:"cve_doc_id"`
		PackagePurlFull string `json:"package_purl_full"`
		PackagePurlBase string `json:"package_purl_base"`
		PackageVersion  string `json:"package_version"`
		NeedsValidation bool   `json:"needs_validation"`
	}

	var candidates []Candidate
	uniqueCveIDs := make(map[string]bool)
	var cveIDsToFetch []string

	for cursor.HasMore() {
		var cand Candidate
		if _, err := cursor.ReadDocument(ctx, &cand); err != nil {
			continue
		}
		candidates = append(candidates, cand)
		if cand.NeedsValidation && !uniqueCveIDs[cand.CveDocID] {
			uniqueCveIDs[cand.CveDocID] = true
			cveIDsToFetch = append(cveIDsToFetch, cand.CveDocID)
		}
	}

	// Batch fetch the affected arrays in Go to prevent memory spikes in AQL
	var cveAffectedMap map[string][]models.Affected
	if len(cveIDsToFetch) > 0 {
		cveAffectedMap, err = util.FetchCVEAffectedData(ctx, db.Database, cveIDsToFetch)
		if err != nil {
			return err
		}
	}

	seenInstances := make(map[string]bool)

	for _, cand := range candidates {
		instanceKey := cand.CveID + ":" + cand.PackagePurlBase
		if seenInstances[instanceKey] {
			continue
		}

		if cand.NeedsValidation {
			affectedData := cveAffectedMap[cand.CveDocID]
			if len(affectedData) > 0 {
				matchFound := false
				for _, affected := range affectedData {
					affectedPurl := ""
					if affected.Package.Purl != "" {
						affectedPurl = affected.Package.Purl
					} else {
						ecosystem := string(affected.Package.Ecosystem)
						namespace := affected.Package.Name
						if strings.Contains(namespace, "/") {
							parts := strings.Split(namespace, "/")
							if len(parts) == 2 {
								namespace = parts[0]
							}
						}
						affectedPurl = util.GetBasePURLFromComponents(ecosystem, namespace, affected.Package.Name)
					}

					standardizedAffectedPurl, err := util.GetStandardBasePURL(affectedPurl)
					if err != nil {
						continue
					}

					if standardizedAffectedPurl == cand.PackagePurlBase {
						if util.IsVersionAffected(cand.PackageVersion, affected) {
							matchFound = true
							break
						}
					}
				}

				if !matchFound {
					continue
				}
			}
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

// CheckReleaseExists returns 200 if a release with the given contentsha exists, 404 if not.
// Used by the GKE sync job to avoid regenerating SBOMs for already-ingested images.
func CheckReleaseExists(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		contentSha := c.Query("contentsha")
		if contentSha == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "contentsha query parameter required",
			})
		}

		ctx := context.Background()
		query := `
			FOR r IN release
				FILTER r.contentsha == @contentsha
				LIMIT 1
				RETURN r._key
		`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{
				"contentsha": contentSha,
			},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": "Query failed",
			})
		}
		defer cursor.Close()

		if cursor.HasMore() {
			return c.Status(fiber.StatusOK).JSON(fiber.Map{"exists": true})
		}
		return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"exists": false})
	}
}
