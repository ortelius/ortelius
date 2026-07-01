// Package lifecycle provides CVE lifecycle event tracking and management.
package lifecycle

import (
	"context"
	"fmt"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/ortelius/v12/database"
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

// resolveBuildDate returns the earliest reliable timestamp for a release.
// It queries the release document's builddate field and returns it if populated
// and earlier than the fallback time provided. This is the single source of truth
// for seeding root_introduced_at on new and resurrected lifecycle records.
func resolveBuildDate(ctx context.Context, db database.DBConnection, releaseName, releaseVersion string, fallback time.Time) time.Time {
	query := `FOR r IN release FILTER r.name == @name AND r.version == @version
		LIMIT 1 RETURN r.builddate`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"name":    releaseName,
			"version": releaseVersion,
		},
	})
	if err != nil {
		return fallback
	}
	defer cursor.Close()

	if cursor.HasMore() {
		var buildDate time.Time
		cursor.ReadDocument(ctx, &buildDate)
		if !buildDate.IsZero() && buildDate.Before(fallback) {
			return buildDate
		}
	}
	return fallback
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

		// Re-evaluate root_introduced_at using release builddate as a fallback.
		// This corrects records where root_introduced_at was seeded from sync time
		// (e.g. first sync happened on the same day the CVE was published) rather
		// than from the actual image build date.
		rootTs := existing["root_introduced_at"]
		if rootTs == nil {
			rootTs = existing["introduced_at"]
		}
		rootTime, _ := time.Parse(time.RFC3339, fmt.Sprintf("%v", rootTs))

		// Use builddate if it predates the current root_introduced_at — this
		// self-corrects any record where sync time was used instead of build date.
		rootTime = resolveBuildDate(ctx, db, releaseName, releaseVersion, rootTime)

		isDisclosedAfter := !cveInfo.Published.IsZero() && cveInfo.Published.After(rootTime)

		updateQuery := `UPDATE @key WITH {
			updated_at: DATE_NOW(),
			is_remediated: false,
			remediated_at: null,
			remediation_status: null,
			remediation_notes: null,
			root_introduced_at: @root_time,
			disclosed_after_deployment: @disclosed_after
		} IN cve_lifecycle`

		db.Database.Query(ctx, updateQuery, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{
				"key":             existing["_key"],
				"root_time":       rootTime.Format(time.RFC3339),
				"disclosed_after": isDisclosedAfter,
			},
		})
		return nil
	}

	// Step 2: "Root Reference" Logic.
	// Seed rootIntroducedAt from the release's builddate when available.
	// This ensures that for images built after CVE disclosure, disclosed_after_deployment
	// is correctly set to false (the image was built knowing about the CVE).
	// Fall back to current sync time if builddate is not populated.
	rootIntroducedAt := resolveBuildDate(ctx, db, releaseName, releaseVersion, introducedAt)

	// Look for the same CVE in the PREVIOUS version of this release on this endpoint.
	// If found, carry forward the earliest known root_introduced_at.
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
			var prevRootTime time.Time
			rootCursor.ReadDocument(ctx, &prevRootTime)
			// Only carry forward if it predates what we already have
			if !prevRootTime.IsZero() && prevRootTime.Before(rootIntroducedAt) {
				rootIntroducedAt = prevRootTime
			}
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

// PurgeSentinelRecords is retained for completeness but is no longer called
// by ProcessSync. Sentinel records are now kept permanently alongside real
// endpoint records. The Released dashboard tab reads only sentinel records;
// the Deployed tab reads only real endpoint records, so there is no overlap.
func PurgeSentinelRecords(ctx context.Context, db database.DBConnection, releaseName, releaseVersion string) error {
	query := `
		FOR r IN cve_lifecycle
			FILTER r.endpoint_name == "_release_tracking_"
			AND r.release_name == @release_name
			AND r.introduced_version == @version
			REMOVE r IN cve_lifecycle
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"release_name": releaseName,
			"version":      releaseVersion,
		},
	})
	return err
}

// ProcessReleaseWithoutEndpoint creates cve_lifecycle records for a release that has
// never been synced to any tracked endpoint (e.g. curl, jenkins). This is the
// release-only ingestion pathway that powers the "Released" dashboard view.
//
// The sentinel endpoint name "_release_tracking_" distinguishes these records
// from real endpoint-synced records so the existing "Deployed" view is unaffected.
// The introduced_version is set to the release version so the release_open_cves
// AQL CTE can join on it via `lifecycle.introduced_version == active.version`.
func ProcessReleaseWithoutEndpoint(
	ctx context.Context,
	db database.DBConnection,
	releaseName string,
	releaseVersion string,
	buildDate time.Time,
) error {
	cves, err := GetCVEsForReleaseTracking(ctx, db, releaseName, releaseVersion)
	if err != nil {
		return fmt.Errorf("failed to get CVEs for %s@%s: %w", releaseName, releaseVersion, err)
	}

	introducedAt := buildDate
	// Guard against Go zero-time (0001-01-01) being passed in from a release
	// document where builddate was never set — treat it the same as missing.
	if introducedAt.IsZero() || introducedAt.Year() < 2000 {
		introducedAt = time.Now()
	}

	const releaseTrackingEndpoint = "_release_tracking_"

	for _, cve := range cves {
		if err := CreateOrUpdateLifecycleRecord(
			ctx, db,
			releaseTrackingEndpoint, releaseName, releaseVersion,
			cve, introducedAt, false,
		); err != nil {
			return fmt.Errorf("failed to create release lifecycle record for %s: %w", cve.CVEID, err)
		}
	}
	return nil
}

// IngestAllUndeployedReleases seeds cve_lifecycle sentinel records for every
// release version that does not yet have sentinel records. Processes ALL versions
// (not just is_latest) so the trend chart has full historical data across releases.
//
// Guard: skips versions that already have sentinel records — safe to re-run.
// Does NOT skip versions with real endpoint records; Deployed and Released tabs
// read from disjoint endpoint_name sets so there is no double-counting.
func IngestAllUndeployedReleases(ctx context.Context, db database.DBConnection, org string) error {
	query := `
		LET tracked = (
			FOR l IN cve_lifecycle
				FILTER l.endpoint_name == "_release_tracking_"
				RETURN DISTINCT CONCAT(l.release_name, "@", l.introduced_version)
		)
		FOR r IN release
			FILTER @org == "" OR r.org == @org
			FILTER CONCAT(r.name, "@", r.version) NOT IN tracked
			RETURN { name: r.name, version: r.version, builddate: r.builddate }
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"org": org},
	})
	if err != nil {
		return fmt.Errorf("failed to query undeployed releases: %w", err)
	}
	defer cursor.Close()

	type releaseRow struct {
		Name      string     `json:"name"`
		Version   string     `json:"version"`
		BuildDate *time.Time `json:"builddate"`
	}

	seenReleases := map[string]bool{}
	for cursor.HasMore() {
		var row releaseRow
		if _, err := cursor.ReadDocument(ctx, &row); err != nil {
			continue
		}
		bd := time.Time{}
		if row.BuildDate != nil && !row.BuildDate.IsZero() && row.BuildDate.Year() >= 2000 {
			bd = *row.BuildDate
		}
		if err := ProcessReleaseWithoutEndpoint(ctx, db, row.Name, row.Version, bd); err != nil {
			fmt.Printf("WARNING: release lifecycle ingestion failed for %s@%s: %v\n", row.Name, row.Version, err)
		}
		seenReleases[row.Name] = true
	}

	// After seeding all versions, reconcile remediation timestamps across
	// consecutive versions for every release touched this run.
	for releaseName := range seenReleases {
		if err := ReconcileSentinelRemediations(ctx, db, releaseName); err != nil {
			fmt.Printf("WARNING: sentinel remediation reconcile failed for %s: %v\n", releaseName, err)
		}
	}
	return nil
}

// ReconcileSentinelRemediations walks consecutive versions of a release sorted
// by builddate and marks as remediated any sentinel cve_lifecycle records whose
// CVE is no longer present in the next version's SBOM. This gives the "Released"
// trend chart a realistic drop-off when a newer release fixes CVEs, and feeds
// the MTTR calculation (all_events) for release-only projects.
//
// remediated_at is set to the builddate of the first version that dropped the CVE.
func ReconcileSentinelRemediations(ctx context.Context, db database.DBConnection, releaseName string) error {
	query := `
		LET versions = (
			FOR r IN release
				FILTER r.name == @release_name
				FILTER r.builddate != null
				AND DATE_TIMESTAMP(r.builddate) > DATE_TIMESTAMP("2000-01-01")
				SORT r.builddate ASC
				RETURN { version: r.version, builddate: r.builddate }
		)

		// Build the CVE set for every version once, so the inner loop
		// doesn't re-query release2cve for each open sentinel record.
		LET version_cve_sets = (
			FOR v IN versions
				LET cve_keys = (
					FOR r IN release
						FILTER r.name == @release_name AND r.version == v.version
						FOR cve, edge IN 1..1 OUTBOUND r release2cve
							FILTER cve.id != null AND edge.package_base != null
							RETURN CONCAT(cve.id, "|", edge.package_base)
				)
				RETURN { version: v.version, builddate: v.builddate, cve_keys: cve_keys }
		)

		// For each open sentinel record, find the FIRST version after its
		// introduction where the CVE is absent — not just the adjacent version.
		// This handles CVEs that persist across multiple releases before being
		// fixed (e.g. present in 8.16–8.19, absent in 8.20 → all four records
		// get remediated_at = 8.20 builddate).
		FOR l IN cve_lifecycle
			FILTER l.endpoint_name == "_release_tracking_"
			AND l.release_name == @release_name
			AND l.is_remediated == false
			LET cve_key = CONCAT(l.cve_id, "|", l.package)
			LET intro_idx = POSITION(versions[*].version, l.introduced_version, true)
			LET fix_version = FIRST(
				FOR i IN (intro_idx >= 0 ? intro_idx + 1 : 0)..LENGTH(version_cve_sets) - 1
					FILTER cve_key NOT IN version_cve_sets[i].cve_keys
					RETURN version_cve_sets[i]
			)
			FILTER fix_version != null
			UPDATE l WITH {
				is_remediated:      true,
				remediated_at:      fix_version.builddate,
				remediated_version: fix_version.version,
				days_to_remediate:  DATE_DIFF(
					DATE_TIMESTAMP(l.introduced_at),
					DATE_TIMESTAMP(fix_version.builddate),
					"d"
				),
				updated_at: DATE_ISO8601(DATE_NOW())
			} IN cve_lifecycle
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"release_name": releaseName},
	})
	return err
}
