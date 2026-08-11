// Package lifecycle provides CVE lifecycle event tracking and management.
package lifecycle

import (
	"context"
	"crypto/sha256"
	"encoding/hex"
	"errors"
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

// releaseTrackingEndpoint is the synthetic endpoint used for release-only CVE tracking.
// It is intentionally disjoint from real deployed endpoints.
const releaseTrackingEndpoint = "_release_tracking_"

// DefaultSentinelHistoryVersions is the number of release versions retained for
// release-only lifecycle history. The Released dashboard only needs a bounded
// history window; retaining four versions prevents cve_lifecycle from growing
// without bound while preserving the current release plus three predecessors.
const DefaultSentinelHistoryVersions = 4

type sentinelReleaseVersion struct {
	Name      string    `json:"name"`
	Version   string    `json:"version"`
	BuildDate time.Time `json:"builddate"`
}

// sentinelLifecycleKey returns a deterministic ArangoDB _key for one sentinel
// release/version/CVE/package identity. ArangoDB enforces _key uniqueness, so
// concurrent/repeated relscanner passes cannot create exact duplicates.
func sentinelLifecycleKey(releaseName, releaseVersion, cveID, pkg string) string {
	raw := releaseName + "\x00" + releaseVersion + "\x00" + cveID + "\x00" + pkg
	sum := sha256.Sum256([]byte(raw))
	return "sentinel_" + hex.EncodeToString(sum[:])
}

// executeLifecycleQuery executes a write query and always closes its cursor.
func executeLifecycleQuery(ctx context.Context, db database.DBConnection, query string, bindVars map[string]interface{}) error {
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	if err != nil {
		return err
	}
	defer cursor.Close()
	return nil
}

// GetCVEsForReleaseTracking retrieves CVE information for a specific release
// version. If more than one release document exists for the same name/version,
// the newest builddate (and then highest _key as a stable tie-breaker) is used.
func GetCVEsForReleaseTracking(ctx context.Context, db database.DBConnection, releaseName, releaseVersion string) (map[string]CVEInfo, error) {
	query := `
		FOR r IN release
			FILTER r.name == @name AND r.version == @version
			FILTER r.builddate != null
			AND DATE_TIMESTAMP(r.builddate) > DATE_TIMESTAMP("2000-01-01")
			SORT DATE_TIMESTAMP(r.builddate) DESC, r._key DESC
			LIMIT 1
			FOR cve, edge IN 1..1 OUTBOUND r release2cve
				FILTER cve.id != null AND cve.id != ""
				FILTER edge.package_base != null AND edge.package_base != ""
				RETURN DISTINCT {
					cve_id: cve.id,
					package: edge.package_base,
					severity_rating: cve.database_specific.severity_rating,
					severity_score: cve.database_specific.cvss_base_score,
					published: cve.published != null ? DATE_ISO8601(cve.published) : null
				}
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"name":    releaseName,
			"version": releaseVersion,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	result := make(map[string]CVEInfo)
	for cursor.HasMore() {
		// These json tags are required because Arango returns snake_case keys.
		var raw struct {
			CveID          string  `json:"cve_id"`
			Package        string  `json:"package"`
			SeverityRating string  `json:"severity_rating"`
			Published      string  `json:"published"`
			SeverityScore  float64 `json:"severity_score"`
		}
		if _, err := cursor.ReadDocument(ctx, &raw); err != nil {
			return nil, err
		}
		if raw.CveID == "" || raw.Package == "" {
			continue
		}

		pubTime, _ := time.Parse(time.RFC3339, raw.Published)
		result[raw.CveID+":"+raw.Package] = CVEInfo{
			CVEID:          raw.CveID,
			Package:        raw.Package,
			SeverityRating: raw.SeverityRating,
			SeverityScore:  raw.SeverityScore,
			Published:      pubTime,
			ReleaseName:    releaseName,
			ReleaseVersion: releaseVersion,
		}
	}
	return result, nil
}

// PurgeSentinelRecords removes sentinel records for one release version.
// Normal maintenance now keeps only the newest DefaultSentinelHistoryVersions,
// but this utility remains useful for targeted cleanup.
func PurgeSentinelRecords(ctx context.Context, db database.DBConnection, releaseName, releaseVersion string) error {
	query := `
		FOR r IN cve_lifecycle
			FILTER r.endpoint_name == @endpoint_name
			AND r.release_name == @release_name
			AND r.introduced_version == @version
			REMOVE r IN cve_lifecycle
	`
	return executeLifecycleQuery(ctx, db, query, map[string]interface{}{
		"endpoint_name": releaseTrackingEndpoint,
		"release_name":  releaseName,
		"version":       releaseVersion,
	})
}

// syncSentinelVersionRecords makes the sentinel documents for one retained
// release version exactly match that version's current release2cve set.
//
// New records use deterministic _keys. Existing deterministic records are
// refreshed in place, while legacy/random-key duplicates and stale CVEs are
// removed after the upsert succeeds.
func syncSentinelVersionRecords(
	ctx context.Context,
	db database.DBConnection,
	releaseName string,
	releaseVersion string,
	buildDate time.Time,
) error {
	if buildDate.IsZero() || buildDate.Year() < 2000 {
		return fmt.Errorf("invalid builddate for sentinel %s@%s: %v", releaseName, releaseVersion, buildDate)
	}

	cves, err := GetCVEsForReleaseTracking(ctx, db, releaseName, releaseVersion)
	if err != nil {
		return fmt.Errorf("failed to get CVEs for %s@%s: %w", releaseName, releaseVersion, err)
	}

	now := time.Now().UTC()
	docs := make([]map[string]interface{}, 0, len(cves))
	keepKeys := make([]string, 0, len(cves))

	for _, cve := range cves {
		key := sentinelLifecycleKey(releaseName, releaseVersion, cve.CVEID, cve.Package)
		keepKeys = append(keepKeys, key)

		var published interface{}
		if !cve.Published.IsZero() {
			published = cve.Published
		}
		isDisclosedAfter := !cve.Published.IsZero() && cve.Published.After(buildDate)

		docs = append(docs, map[string]interface{}{
			"_key":                       key,
			"cve_id":                     cve.CVEID,
			"endpoint_name":              releaseTrackingEndpoint,
			"release_name":               releaseName,
			"package":                    cve.Package,
			"severity_rating":            cve.SeverityRating,
			"severity_score":             cve.SeverityScore,
			"introduced_at":              buildDate,
			"root_introduced_at":         buildDate,
			"introduced_version":         releaseVersion,
			"remediated_at":              nil,
			"remediated_version":         nil,
			"days_to_remediate":          nil,
			"is_remediated":              false,
			"disclosed_after_deployment": isDisclosedAfter,
			"published":                  published,
			"objtype":                    "CVELifecycleEvent",
			"created_at":                 now,
			"updated_at":                 now,
		})
	}

	if len(docs) > 0 {
		upsertQuery := `
			FOR doc IN @docs
				UPSERT { _key: doc._key }
				INSERT doc
				UPDATE {
					cve_id:                     doc.cve_id,
					endpoint_name:              doc.endpoint_name,
					release_name:               doc.release_name,
					package:                    doc.package,
					severity_rating:            doc.severity_rating,
					severity_score:             doc.severity_score,
					introduced_at:              doc.introduced_at,
					root_introduced_at:         doc.root_introduced_at,
					introduced_version:         doc.introduced_version,
					disclosed_after_deployment: doc.disclosed_after_deployment,
					published:                  doc.published,
					objtype:                    doc.objtype,
					updated_at:                 @now
				} IN cve_lifecycle
		`
		if err := executeLifecycleQuery(ctx, db, upsertQuery, map[string]interface{}{
			"docs": docs,
			"now":  now,
		}); err != nil {
			return fmt.Errorf("failed to upsert sentinel records for %s@%s: %w", releaseName, releaseVersion, err)
		}
	}

	// Remove anything for this retained version that is not one of the
	// deterministic records above. This removes old duplicate/random-key rows
	// and also removes CVEs that disappeared if release2cve was refreshed.
	pruneQuery := `
		FOR l IN cve_lifecycle
			FILTER l.endpoint_name == @endpoint_name
			AND l.release_name == @release_name
			AND l.introduced_version == @version
			FILTER l._key NOT IN @keep_keys
			REMOVE l IN cve_lifecycle
	`
	if err := executeLifecycleQuery(ctx, db, pruneQuery, map[string]interface{}{
		"endpoint_name": releaseTrackingEndpoint,
		"release_name":  releaseName,
		"version":       releaseVersion,
		"keep_keys":     keepKeys,
	}); err != nil {
		return fmt.Errorf("failed to prune stale sentinel records for %s@%s: %w", releaseName, releaseVersion, err)
	}

	return nil
}

// ProcessReleaseWithoutEndpoint synchronizes release-only lifecycle records for
// one release version. It powers the Released dashboard and is safe to re-run.
func ProcessReleaseWithoutEndpoint(
	ctx context.Context,
	db database.DBConnection,
	releaseName string,
	releaseVersion string,
	buildDate time.Time,
) error {
	return syncSentinelVersionRecords(ctx, db, releaseName, releaseVersion, buildDate)
}

// recentSentinelReleaseVersions returns at most maxVersions real release
// versions per release name. Versions are returned oldest-to-newest within each
// release so reconciliation can walk the retained window chronologically.
func recentSentinelReleaseVersions(
	ctx context.Context,
	db database.DBConnection,
	org string,
	maxVersions int,
) ([]sentinelReleaseVersion, error) {
	if maxVersions < 1 {
		return nil, fmt.Errorf("maxVersions must be at least 1")
	}

	query := `
		FOR seed IN release
			FILTER @org == "" OR seed.org == @org
			FILTER seed.version != "0.0.0-snapshot"
			FILTER seed.builddate != null
			AND DATE_TIMESTAMP(seed.builddate) > DATE_TIMESTAMP("2000-01-01")
			COLLECT release_name = seed.name
			SORT release_name

			LET recent = (
				FOR r IN release
					FILTER r.name == release_name
					FILTER @org == "" OR r.org == @org
					FILTER r.version != "0.0.0-snapshot"
					FILTER r.builddate != null
					AND DATE_TIMESTAMP(r.builddate) > DATE_TIMESTAMP("2000-01-01")
					COLLECT version = r.version
					AGGREGATE build_ts = MAX(DATE_TIMESTAMP(r.builddate))
					SORT build_ts DESC
					LIMIT @max_versions
					SORT build_ts ASC
					RETURN {
						name: release_name,
						version: version,
						builddate: DATE_ISO8601(build_ts)
					}
			)

			FOR row IN recent
				RETURN row
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"org":          org,
			"max_versions": maxVersions,
		},
	})
	if err != nil {
		return nil, fmt.Errorf("failed to query recent release versions: %w", err)
	}
	defer cursor.Close()

	rows := make([]sentinelReleaseVersion, 0)
	for cursor.HasMore() {
		var row sentinelReleaseVersion
		if _, err := cursor.ReadDocument(ctx, &row); err != nil {
			return nil, fmt.Errorf("failed reading recent release version: %w", err)
		}
		if row.Name == "" || row.Version == "" || row.BuildDate.IsZero() {
			continue
		}
		rows = append(rows, row)
	}
	return rows, nil
}

// pruneOldSentinelVersions removes sentinel rows outside the retained version
// window for one release.
func pruneOldSentinelVersions(
	ctx context.Context,
	db database.DBConnection,
	releaseName string,
	keepVersions []string,
) error {
	if len(keepVersions) == 0 {
		return fmt.Errorf("refusing to prune all sentinel versions for %s with an empty keep set", releaseName)
	}

	query := `
		FOR l IN cve_lifecycle
			FILTER l.endpoint_name == @endpoint_name
			AND l.release_name == @release_name
			FILTER l.introduced_version NOT IN @keep_versions
			REMOVE l IN cve_lifecycle
	`
	return executeLifecycleQuery(ctx, db, query, map[string]interface{}{
		"endpoint_name": releaseTrackingEndpoint,
		"release_name":  releaseName,
		"keep_versions": keepVersions,
	})
}

// resetSentinelRemediations clears derived remediation fields for the retained
// window. ReconcileSentinelRemediationsLimited then recomputes them from the
// current release2cve source of truth. With only four versions this is cheap and
// makes repeated maintenance deterministic even if historical CVE edges change.
func resetSentinelRemediations(ctx context.Context, db database.DBConnection, releaseName string) error {
	query := `
		FOR l IN cve_lifecycle
			FILTER l.endpoint_name == @endpoint_name
			AND l.release_name == @release_name
			UPDATE l WITH {
				is_remediated: false,
				remediated_at: null,
				remediated_version: null,
				days_to_remediate: null,
				remediation_status: null,
				remediation_notes: null,
				updated_at: DATE_ISO8601(DATE_NOW())
			} IN cve_lifecycle
	`
	return executeLifecycleQuery(ctx, db, query, map[string]interface{}{
		"endpoint_name": releaseTrackingEndpoint,
		"release_name":  releaseName,
	})
}

// ReconcileSentinelRemediationsLimited marks retained sentinel records as
// remediated at the first later retained release version where the CVE/package
// is absent. Only maxVersions are considered.
func ReconcileSentinelRemediationsLimited(
	ctx context.Context,
	db database.DBConnection,
	releaseName string,
	maxVersions int,
) error {
	if maxVersions < 1 {
		return fmt.Errorf("maxVersions must be at least 1")
	}

	query := `
		LET recent_desc = (
			FOR r IN release
				FILTER r.name == @release_name
				FILTER r.version != "0.0.0-snapshot"
				FILTER r.builddate != null
				AND DATE_TIMESTAMP(r.builddate) > DATE_TIMESTAMP("2000-01-01")
				COLLECT version = r.version
				AGGREGATE build_ts = MAX(DATE_TIMESTAMP(r.builddate))
				SORT build_ts DESC
				LIMIT @max_versions
				RETURN { version: version, build_ts: build_ts }
		)

		LET versions = (
			FOR v IN recent_desc
				SORT v.build_ts ASC
				RETURN {
					version: v.version,
					builddate: DATE_ISO8601(v.build_ts)
				}
		)

		LET version_cve_sets = (
			FOR v IN versions
				LET cve_keys = (
					FOR r IN release
						FILTER r.name == @release_name AND r.version == v.version
						FILTER r.builddate != null
						AND DATE_TIMESTAMP(r.builddate) > DATE_TIMESTAMP("2000-01-01")
						SORT DATE_TIMESTAMP(r.builddate) DESC, r._key DESC
						LIMIT 1
						FOR cve, edge IN 1..1 OUTBOUND r release2cve
							FILTER cve.id != null AND cve.id != ""
							FILTER edge.package_base != null AND edge.package_base != ""
							RETURN DISTINCT CONCAT(cve.id, "|", edge.package_base)
				)
				RETURN {
					version: v.version,
					builddate: v.builddate,
					cve_set: ZIP(cve_keys, cve_keys[* RETURN true])
				}
		)

		LET version_list = versions[*].version
		LET version_count = LENGTH(version_cve_sets)

		FOR l IN cve_lifecycle
			FILTER l.endpoint_name == @endpoint_name
			AND l.release_name == @release_name
			AND l.is_remediated == false
			LET intro_idx = POSITION(version_list, l.introduced_version, true)
			FILTER intro_idx >= 0 AND intro_idx < version_count - 1
			LET cve_key = CONCAT(l.cve_id, "|", l.package)
			LET fix_version = FIRST(
				FOR i IN intro_idx + 1..version_count - 1
					FILTER !HAS(version_cve_sets[i].cve_set, cve_key)
					RETURN version_cve_sets[i]
			)
			FILTER fix_version != null
			UPDATE l WITH {
				is_remediated: true,
				remediated_at: fix_version.builddate,
				remediated_version: fix_version.version,
				days_to_remediate: DATE_DIFF(
					DATE_TIMESTAMP(l.introduced_at),
					DATE_TIMESTAMP(fix_version.builddate),
					"d"
				),
				updated_at: DATE_ISO8601(DATE_NOW())
			} IN cve_lifecycle
	`
	return executeLifecycleQuery(ctx, db, query, map[string]interface{}{
		"release_name":  releaseName,
		"endpoint_name": releaseTrackingEndpoint,
		"max_versions":  maxVersions,
	})
}

// ReconcileSentinelRemediations preserves the previous public API while using
// the bounded history window.
func ReconcileSentinelRemediations(ctx context.Context, db database.DBConnection, releaseName string) error {
	return ReconcileSentinelRemediationsLimited(ctx, db, releaseName, DefaultSentinelHistoryVersions)
}

// MaintainRecentSentinelHistory synchronizes and reconciles the newest
// maxVersions release versions for every release in scope, pruning older
// sentinel versions. It is safe to run every relscanner cycle.
func MaintainRecentSentinelHistory(
	ctx context.Context,
	db database.DBConnection,
	org string,
	maxVersions int,
) error {
	rows, err := recentSentinelReleaseVersions(ctx, db, org, maxVersions)
	if err != nil {
		return err
	}

	byRelease := make(map[string][]sentinelReleaseVersion)
	for _, row := range rows {
		byRelease[row.Name] = append(byRelease[row.Name], row)
	}

	var errs []error
	for releaseName, versions := range byRelease {
		keepVersions := make([]string, 0, len(versions))
		for _, version := range versions {
			keepVersions = append(keepVersions, version.Version)
		}

		if err := pruneOldSentinelVersions(ctx, db, releaseName, keepVersions); err != nil {
			errs = append(errs, fmt.Errorf("prune sentinel history for %s: %w", releaseName, err))
			continue
		}

		releaseOK := true
		for _, version := range versions {
			if err := syncSentinelVersionRecords(ctx, db, version.Name, version.Version, version.BuildDate); err != nil {
				errs = append(errs, fmt.Errorf("sync sentinel %s@%s: %w", version.Name, version.Version, err))
				releaseOK = false
			}
		}
		if !releaseOK {
			continue
		}

		if err := resetSentinelRemediations(ctx, db, releaseName); err != nil {
			errs = append(errs, fmt.Errorf("reset sentinel remediation state for %s: %w", releaseName, err))
			continue
		}
		if err := ReconcileSentinelRemediationsLimited(ctx, db, releaseName, maxVersions); err != nil {
			errs = append(errs, fmt.Errorf("reconcile sentinel history for %s: %w", releaseName, err))
		}
	}

	return errors.Join(errs...)
}

// IngestAllUndeployedReleases preserves the old relscanner-facing function name
// but now maintains only the newest DefaultSentinelHistoryVersions instead of
// seeding the entire release history.
func IngestAllUndeployedReleases(ctx context.Context, db database.DBConnection, org string) error {
	return MaintainRecentSentinelHistory(ctx, db, org, DefaultSentinelHistoryVersions)
}

// RebuildSentinelHistory deletes the existing release-tracking sentinel dataset
// in scope and deterministically rebuilds only the newest maxVersions release
// versions. Real endpoint lifecycle rows are never touched.
//
// The rebuild is intentionally restartable: if maintenance fails after the
// delete, rerunning this function converges on the same deterministic dataset.
func RebuildSentinelHistory(
	ctx context.Context,
	db database.DBConnection,
	org string,
	maxVersions int,
) error {
	if maxVersions < 1 {
		return fmt.Errorf("maxVersions must be at least 1")
	}

	var deleteQuery string
	var bindVars map[string]interface{}
	if org == "" {
		deleteQuery = `
			FOR l IN cve_lifecycle
				FILTER l.endpoint_name == @endpoint_name
				REMOVE l IN cve_lifecycle
		`
		bindVars = map[string]interface{}{
			"endpoint_name": releaseTrackingEndpoint,
		}
	} else {
		deleteQuery = `
			LET scoped_names = (
				FOR r IN release
					FILTER r.org == @org
					RETURN DISTINCT r.name
			)
			FOR l IN cve_lifecycle
				FILTER l.endpoint_name == @endpoint_name
				FILTER l.release_name IN scoped_names
				REMOVE l IN cve_lifecycle
		`
		bindVars = map[string]interface{}{
			"endpoint_name": releaseTrackingEndpoint,
			"org":           org,
		}
	}

	if err := executeLifecycleQuery(ctx, db, deleteQuery, bindVars); err != nil {
		return fmt.Errorf("failed deleting existing sentinel history: %w", err)
	}

	if err := MaintainRecentSentinelHistory(ctx, db, org, maxVersions); err != nil {
		return fmt.Errorf("failed rebuilding sentinel history: %w", err)
	}
	return nil
}
