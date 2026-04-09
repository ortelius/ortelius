// Package lifecycle provides CVE lifecycle event tracking and management.
package lifecycle

import (
	"context"
	"fmt"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/pdvd-backend/v12/database"
)

// SupersedeAllActiveCVEs closes all active CVEs for a specific endpoint and release.
// This enforces a "clean slate" logic where all previous records are marked as superseded
// before the new state is written.
func SupersedeAllActiveCVEs(ctx context.Context, db database.DBConnection, endpointName, releaseName string, supersededAt time.Time) error {
	query := `
		FOR doc IN cve_lifecycle
			FILTER doc.endpoint_name == @endpoint_name
			FILTER doc.release_name  == @release_name
			FILTER doc.is_remediated == false
			
			UPDATE doc WITH {
				is_remediated: true,
				remediated_at: @superseded_at,
				remediation_status: "Superseded",
				remediation_notes: CONCAT("Superseded by deployment at ", @superseded_at)
			} IN cve_lifecycle
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"endpoint_name": endpointName,
			"release_name":  releaseName,
			"superseded_at": supersededAt,
		},
	})
	return err
}

// CreateOrUpdateLifecycleRecord handles version-specific audit records with Root discovery tracking.
func CreateOrUpdateLifecycleRecord(
	ctx context.Context,
	db database.DBConnection,
	endpointName string,
	releaseName string,
	releaseVersion string,
	cveInfo CVEInfo,
	introducedAt time.Time,
	_ bool, // Renamed from disclosedAfter to _ to address revive linting error
) error {

	if introducedAt.IsZero() {
		return fmt.Errorf("refusing to create lifecycle record with zero-value timestamp for %s", cveInfo.CVEID)
	}

	// Step 1: Check if this EXACT record (Version + CVE) already exists.
	checkQuery := `
		FOR rec IN cve_lifecycle
			FILTER rec.cve_id == @cve_id AND rec.package == @package
			AND rec.release_name == @release_name AND rec.endpoint_name == @endpoint_name
			AND rec.introduced_version == @version
			LIMIT 1
			RETURN rec`

	cursor, err := db.Database.Query(ctx, checkQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"cve_id":        cveInfo.CVEID,
			"package":       cveInfo.Package,
			"release_name":  releaseName,
			"endpoint_name": endpointName,
			"version":       releaseVersion,
		},
	})
	if err != nil {
		return err
	}
	defer cursor.Close()

	if cursor.HasMore() {
		var existing map[string]interface{}
		cursor.ReadDocument(ctx, &existing)
		// Resurrection logic: If we just superseded this record (e.g. re-sync of same version), mark it active again.
		updateQuery := `UPDATE @key WITH { updated_at: DATE_NOW(), is_remediated: false, remediated_at: null, remediation_status: null, remediation_notes: null } IN cve_lifecycle`
		db.Database.Query(ctx, updateQuery, &arangodb.QueryOptions{BindVars: map[string]interface{}{"key": existing["_key"]}})
		return nil
	}

	// Step 2: "Root Reference" Logic.
	// Look for the same CVE in the PREVIOUS version of this release on this endpoint.
	rootIntroducedAt := introducedAt // Default to current sync time if first discovery

	rootQuery := `
		LET prev_version = (
			FOR s IN sync 
				FILTER s.release_name == @release_name AND s.endpoint_name == @endpoint_name 
				AND DATE_TIMESTAMP(s.synced_at) < DATE_TIMESTAMP(@now)
				SORT s.synced_at DESC LIMIT 1 RETURN s.release_version
		)[0]
		
		FOR r IN cve_lifecycle
			FILTER r.cve_id == @cve_id AND r.package == @package
			AND r.release_name == @release_name AND r.endpoint_name == @endpoint_name
			AND r.introduced_version == prev_version
			RETURN r.root_introduced_at != null ? r.root_introduced_at : r.introduced_at`

	rootCursor, err := db.Database.Query(ctx, rootQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"cve_id":        cveInfo.CVEID,
			"package":       cveInfo.Package,
			"release_name":  releaseName,
			"endpoint_name": endpointName,
			"now":           introducedAt.Format(time.RFC3339),
		},
	})
	if err == nil {
		defer rootCursor.Close()
		if rootCursor.HasMore() {
			rootCursor.ReadDocument(ctx, &rootIntroducedAt)
		}
	}

	isDisclosedAfter := !cveInfo.Published.IsZero() && cveInfo.Published.After(rootIntroducedAt)

	// Step 3: Create the new Audit Record
	record := map[string]interface{}{
		"cve_id":                     cveInfo.CVEID,
		"endpoint_name":              endpointName,
		"release_name":               releaseName,
		"package":                    cveInfo.Package,
		"severity_rating":            cveInfo.SeverityRating,
		"severity_score":             cveInfo.SeverityScore,
		"introduced_at":              introducedAt,     // Specific deployment time for this version
		"root_introduced_at":         rootIntroducedAt, // Original discovery time across all versions
		"introduced_version":         releaseVersion,
		"remediated_at":              nil,
		"remediated_version":         nil,
		"days_to_remediate":          nil,
		"is_remediated":              false,
		"disclosed_after_deployment": isDisclosedAfter,
		"published":                  cveInfo.Published,
		"objtype":                    "CVELifecycleEvent",
		"created_at":                 time.Now().UTC(),
		"updated_at":                 time.Now().UTC(),
	}

	_, err = db.Collections["cve_lifecycle"].CreateDocument(ctx, record)
	return err
}

// MarkCVERemediated is retained as a utility/fallback but primarily replaced by SupersedeAllActiveCVEs
func MarkCVERemediated(ctx context.Context, db database.DBConnection, endpointName, releaseName, previousVersion, currentVersion, cveID, pkgPURL string, remediatedAt time.Time) error {
	query := `
		FOR r IN cve_lifecycle
			FILTER r.cve_id == @cve_id AND r.package == @package 
			AND r.release_name == @release_name AND r.endpoint_name == @endpoint_name 
			AND r.introduced_version == @previous_version AND r.is_remediated == false
			LIMIT 1
			UPDATE r WITH { 
				is_remediated: true, 
				remediated_at: @remediated_at, 
				remediated_version: @current_version, 
				days_to_remediate: DATE_DIFF(DATE_TIMESTAMP(r.root_introduced_at != null ? r.root_introduced_at : r.introduced_at), @rem_ts, "d"), 
				updated_at: DATE_NOW() 
			} IN cve_lifecycle
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"cve_id":           cveID,
			"package":          pkgPURL,
			"release_name":     releaseName,
			"endpoint_name":    endpointName,
			"previous_version": previousVersion,
			"current_version":  currentVersion,
			"remediated_at":    remediatedAt,
			"rem_ts":           remediatedAt.Unix() * 1000,
		},
	})
	return err
}

// GetPreviousVersion retrieves the version and sync time of the most recent sync for a release on an endpoint.
func GetPreviousVersion(ctx context.Context, db database.DBConnection, releaseName, endpointName string, currentSyncTime time.Time) (string, time.Time, error) {
	query := `FOR s IN sync FILTER s.release_name == @release_name AND s.endpoint_name == @endpoint_name AND DATE_TIMESTAMP(s.synced_at) < @current_time SORT DATE_TIMESTAMP(s.synced_at) DESC LIMIT 1 RETURN { version: s.release_version, synced_at: s.synced_at }`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: map[string]interface{}{"release_name": releaseName, "endpoint_name": endpointName, "current_time": currentSyncTime.Unix() * 1000}})
	if err != nil || !cursor.HasMore() {
		return "", time.Time{}, err
	}
	defer cursor.Close()
	var result struct {
		Version  string    `json:"version"`
		SyncedAt time.Time `json:"synced_at"`
	}
	cursor.ReadDocument(ctx, &result)
	return result.Version, result.SyncedAt, nil
}

// GetCVEsForReleaseTracking retrieves CVE information for a specific release to be used in lifecycle tracking.
func GetCVEsForReleaseTracking(ctx context.Context, db database.DBConnection, releaseName, releaseVersion string) (map[string]CVEInfo, error) {
	query := `FOR r IN release FILTER r.name == @name AND r.version == @version LIMIT 1 FOR cve, edge IN 1..1 OUTBOUND r release2cve RETURN { cve_id: cve.id, package: edge.package_base, severity_rating: cve.database_specific.severity_rating, severity_score: cve.database_specific.cvss_base_score, published: DATE_ISO8601(cve.published) }`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: map[string]interface{}{"name": releaseName, "version": releaseVersion}})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()
	result := make(map[string]CVEInfo)
	for cursor.HasMore() {
		var raw struct {
			CveID, Package, SeverityRating, Published string
			SeverityScore                             float64
		}
		if _, err := cursor.ReadDocument(ctx, &raw); err == nil {
			pubTime, _ := time.Parse(time.RFC3339, raw.Published)
			result[raw.CveID+":"+raw.Package] = CVEInfo{
				CVEID: raw.CveID, Package: raw.Package, SeverityRating: raw.SeverityRating, SeverityScore: raw.SeverityScore, Published: pubTime,
				ReleaseName: releaseName, ReleaseVersion: releaseVersion,
			}
		}
	}
	return result, nil
}
