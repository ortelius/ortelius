// Package releases implements the resolvers for release data.
package releases

import (
	"context"
	"encoding/json"
	"net/url"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/google/osv-scanner/pkg/models"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/util"
)

// Helper functions for safe pointer access
func getStringValue(s *string) string {
	if s == nil {
		return ""
	}
	return *s
}

func getFloatValue(f *float64) float64 {
	if f == nil {
		return 0.0
	}
	return *f
}

// ResolveReleaseVulnerabilities fetches vulnerabilities associated with a specific release.
func ResolveReleaseVulnerabilities(db database.DBConnection, name, version string) ([]map[string]interface{}, error) {
	ctx := context.Background()

	decodedName, err := url.QueryUnescape(name)
	if err != nil {
		decodedName = name
	}

	query := `
		FOR release IN release
			FILTER release.name == @name AND release.version == @version
			
			FOR cve, edge IN 1..1 OUTBOUND release release2cve
				RETURN {
					cve_id: cve.id,
					summary: cve.summary,
					details: cve.details,
					severity: cve.severity,
					severity_score: cve.database_specific.cvss_base_score,
					severity_rating: cve.database_specific.severity_rating,
					published: cve.published,
					modified: cve.modified,
					aliases: cve.aliases,
					package: edge.package_purl,
					package_version: edge.package_version,
					full_purl: edge.package_purl,
					all_affected: cve.affected
				}
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"name":    decodedName,
			"version": version,
		},
	})
	if err != nil {
		return []map[string]interface{}{}, err
	}
	defer cursor.Close()

	var vulnerabilities []map[string]interface{}
	seen := make(map[string]bool)

	type CVEResult struct {
		CveID          string                   `json:"cve_id"`
		Summary        string                   `json:"summary"`
		Details        string                   `json:"details"`
		SeverityScore  float64                  `json:"severity_score"`
		SeverityRating string                   `json:"severity_rating"`
		Published      string                   `json:"published"`
		Modified       string                   `json:"modified"`
		Aliases        []string                 `json:"aliases"`
		Package        string                   `json:"package"`
		PackageVersion string                   `json:"package_version"`
		FullPurl       string                   `json:"full_purl"`
		AllAffected    []map[string]interface{} `json:"all_affected"`
		Severity       []map[string]interface{} `json:"severity"`
	}

	for cursor.HasMore() {
		var result CVEResult
		_, err := cursor.ReadDocument(ctx, &result)
		if err != nil {
			continue
		}

		key := result.CveID + ":" + result.Package
		if seen[key] {
			continue
		}
		seen[key] = true

		var cvssScore string
		for _, sev := range result.Severity {
			if sevType, ok := sev["type"].(string); ok && sevType == "CVSS_V3" {
				if score, ok := sev["score"].(string); ok {
					cvssScore = score
					break
				}
			}
		}

		vulnerabilities = append(vulnerabilities, map[string]interface{}{
			"cve_id":           result.CveID,
			"summary":          result.Summary,
			"details":          result.Details,
			"severity_score":   result.SeverityScore,
			"severity_rating":  result.SeverityRating,
			"cvss_v3_score":    cvssScore,
			"published":        result.Published,
			"modified":         result.Modified,
			"aliases":          result.Aliases,
			"package":          result.Package,
			"affected_version": result.PackageVersion,
			"full_purl":        result.FullPurl,
			"fixed_in":         util.ExtractApplicableFixedVersion(result.PackageVersion, convertToModelsAffected(result.AllAffected)),
		})
	}

	return vulnerabilities, nil
}

// ResolveAffectedEndpoints fetches endpoints where a specific release is deployed.
func ResolveAffectedEndpoints(db database.DBConnection, name, version string) ([]map[string]interface{}, error) {
	ctx := context.Background()

	query := `
		FOR sync IN sync
			FILTER sync.release_name == @name AND sync.release_version == @version
			FOR endpoint IN endpoint
				FILTER endpoint.name == sync.endpoint_name
				LIMIT 1
				RETURN {
					endpoint_name: endpoint.name,
					endpoint_url: endpoint.name,
					endpoint_type: endpoint.endpoint_type,
					environment: endpoint.environment,
					last_sync: sync.synced_at,
					status: "active"
				}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"name":    name,
			"version": version,
		},
	})
	if err != nil {
		return []map[string]interface{}{}, nil
	}
	defer cursor.Close()

	type EndpointResult struct {
		EndpointName string `json:"endpoint_name"`
		EndpointURL  string `json:"endpoint_url"`
		EndpointType string `json:"endpoint_type"`
		Environment  string `json:"environment"`
		LastSync     string `json:"last_sync"`
		Status       string `json:"status"`
	}

	var endpoints []map[string]interface{}
	for cursor.HasMore() {
		var result EndpointResult
		_, err := cursor.ReadDocument(ctx, &result)
		if err != nil {
			continue
		}

		endpoints = append(endpoints, map[string]interface{}{
			"endpoint_name": result.EndpointName,
			"endpoint_url":  result.EndpointURL,
			"endpoint_type": result.EndpointType,
			"environment":   result.Environment,
			"last_sync":     result.LastSync,
			"status":        result.Status,
		})
	}

	if endpoints == nil {
		return []map[string]interface{}{}, nil
	}
	return endpoints, nil
}

// ResolveAffectedReleases updated to count unique CVE ID + Package combinations.
func ResolveAffectedReleases(db database.DBConnection, severity string, org string) ([]interface{}, error) {
	ctx := context.Background()
	severityScore := util.GetSeverityScore(severity)

	query := `
		FOR r IN release
			FILTER @org == "" OR r.org == @org OR (
				LENGTH(
					FOR s IN sync
						FILTER s.release_name == r.name
						AND s.release_version == r.version
						FOR e IN endpoint
							FILTER e.name == s.endpoint_name
							AND e.org == @org
							LIMIT 1
							RETURN 1
				) > 0
			)
			COLLECT name = r.name INTO groupedReleases = r

			LET latestRelease = (
				FOR release IN groupedReleases
					SORT release.version_major DESC,
						release.version_minor DESC,
						release.version_patch DESC,
						release.version_prerelease ASC,
						release.version DESC
					LIMIT 1
					RETURN release
			)[0]
			
			LET versionCount = LENGTH(groupedReleases)

			LET sbomData = (
				FOR s IN 1..1 OUTBOUND latestRelease release2sbom
					LIMIT 1
					RETURN { id: s._id }
			)[0]

			LET dependencyCount = (
				FILTER sbomData != null
				FOR edge IN sbom2purl
					FILTER edge._from == sbomData.id
					COLLECT fullPurl = edge.full_purl
					RETURN 1
			)

			LET syncCount = (
				FOR sync IN sync
					FILTER sync.release_name == latestRelease.name 
					AND sync.release_version == latestRelease.version
					COLLECT WITH COUNT INTO count
					RETURN count
			)[0]

			LET cveMatches = (
				FOR cve, edge IN 1..1 OUTBOUND latestRelease release2cve
                    FILTER @severityScore == 0.0 OR cve.database_specific.cvss_base_score >= @severityScore
                    
                    RETURN {
                        cve_id: cve.id,
                        cve_summary: cve.summary,
                        cve_details: cve.details,
                        cve_severity_score: cve.database_specific.cvss_base_score,
                        cve_severity_rating: cve.database_specific.severity_rating,
                        cve_published: cve.published,
                        cve_modified: cve.modified,
                        cve_aliases: cve.aliases,
                        all_affected: cve.affected, 
                        package: edge.package_purl,
                        version: edge.package_version,
                        full_purl: edge.package_purl
                    }
			)

			LET maxSeverity = LENGTH(cveMatches) > 0 ? MAX(cveMatches[*].cve_severity_score) : 0
			
			LET uniqueInstances = (
				FOR match IN cveMatches
					COLLECT cveId = match.cve_id, package = match.package
					RETURN 1
			)
			LET vulnerabilityCount = LENGTH(uniqueInstances)
			
			LET previousRelease = (
				FOR release IN groupedReleases
					FILTER release._key != latestRelease._key
					SORT release.version_major DESC,
						release.version_minor DESC,
						release.version_patch DESC,
						release.version_prerelease ASC,
						release.version DESC
					LIMIT 1
					RETURN release
			)[0]
			
			LET prevVulnCount = previousRelease != null ? (
				LET prevCveMatches = (
					FOR cve, edge IN 1..1 OUTBOUND previousRelease release2cve
						RETURN { cve_id: cve.id, package: edge.package_purl }
				)
				
				LET prevUniqueInstances = (
					FOR match IN prevCveMatches
						COLLECT cveId = match.cve_id, package = match.package
						RETURN 1
				)
				
				RETURN LENGTH(prevUniqueInstances)
			)[0] : null
			
			LET vulnerabilityCountDelta = prevVulnCount != null ? (vulnerabilityCount - prevVulnCount) : null

			RETURN {
				release_name: latestRelease.name,
				latest_version: latestRelease.version,
				is_latest: true,
				version_major: latestRelease.version_major,
				version_minor: latestRelease.version_minor,
				version_patch: latestRelease.version_patch,
				version_prerelease: latestRelease.version_prerelease,
				version_count: versionCount,
				content_sha: latestRelease.contentsha,
				project_type: latestRelease.projecttype,
				openssf_scorecard_score: latestRelease.openssf_scorecard_score,
				synced_endpoint_count: syncCount,
				dependency_count: LENGTH(dependencyCount),
				max_severity: maxSeverity,
				vulnerability_count: vulnerabilityCount,
				vulnerability_count_delta: vulnerabilityCountDelta,
				cve_matches: cveMatches
			}
			`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"severityScore": severityScore,
			"org":           org,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	type AggregatedRelease struct {
		ReleaseName             string   `json:"release_name"`
		LatestVersion           string   `json:"latest_version"`
		VersionMajor            *int     `json:"version_major"`
		VersionMinor            *int     `json:"version_minor"`
		VersionPatch            *int     `json:"version_patch"`
		VersionPrerelease       string   `json:"version_prerelease"`
		VersionCount            int      `json:"version_count"`
		IsLatest                bool     `json:"is_latest"`
		ContentSha              string   `json:"content_sha"`
		ProjectType             string   `json:"project_type"`
		OpenSSFScorecardScore   *float64 `json:"openssf_scorecard_score"`
		SyncedEndpointCount     int      `json:"synced_endpoint_count"`
		DependencyCount         int      `json:"dependency_count"`
		MaxSeverity             *float64 `json:"max_severity"`
		VulnerabilityCount      int      `json:"vulnerability_count"`
		VulnerabilityCountDelta *int     `json:"vulnerability_count_delta"`
		CveMatches              []struct {
			CveID             *string                  `json:"cve_id"`
			CveSummary        *string                  `json:"cve_summary"`
			CveDetails        *string                  `json:"cve_details"`
			CveSeverityScore  *float64                 `json:"cve_severity_score"`
			CveSeverityRating *string                  `json:"cve_severity_rating"`
			CvePublished      *string                  `json:"cve_published"`
			CveModified       *string                  `json:"cve_modified"`
			CveAliases        []string                 `json:"cve_aliases"`
			AllAffected       []map[string]interface{} `json:"all_affected"`
			Package           string                   `json:"package"`
			Version           string                   `json:"version"`
			FullPurl          string                   `json:"full_purl"`
		} `json:"cve_matches"`
	}

	var results []interface{}

	for cursor.HasMore() {
		var aggRelease AggregatedRelease
		_, err := cursor.ReadDocument(ctx, &aggRelease)
		if err != nil {
			continue
		}

		releaseKey := aggRelease.ReleaseName + ":" + aggRelease.LatestVersion
		seen := make(map[string]bool)

		for _, cveMatch := range aggRelease.CveMatches {
			var cveID string
			if cveMatch.CveID != nil {
				cveID = *cveMatch.CveID
			}
			cveKey := releaseKey + ":" + cveID + ":" + cveMatch.Package + ":" + cveMatch.Version

			if !seen[cveKey] {
				seen[cveKey] = true

				result := map[string]interface{}{
					"cve_id":                    cveID,
					"summary":                   getStringValue(cveMatch.CveSummary),
					"details":                   getStringValue(cveMatch.CveDetails),
					"severity_score":            getFloatValue(cveMatch.CveSeverityScore),
					"severity_rating":           getStringValue(cveMatch.CveSeverityRating),
					"published":                 getStringValue(cveMatch.CvePublished),
					"modified":                  getStringValue(cveMatch.CveModified),
					"aliases":                   cveMatch.CveAliases,
					"package":                   cveMatch.Package,
					"affected_version":          cveMatch.Version,
					"full_purl":                 cveMatch.FullPurl,
					"release_name":              aggRelease.ReleaseName,
					"release_version":           aggRelease.LatestVersion,
					"version_count":             aggRelease.VersionCount,
					"content_sha":               aggRelease.ContentSha,
					"project_type":              aggRelease.ProjectType,
					"openssf_scorecard_score":   aggRelease.OpenSSFScorecardScore,
					"dependency_count":          aggRelease.DependencyCount,
					"synced_endpoint_count":     aggRelease.SyncedEndpointCount,
					"vulnerability_count":       aggRelease.VulnerabilityCount,
					"vulnerability_count_delta": aggRelease.VulnerabilityCountDelta,
					"is_latest":                 aggRelease.IsLatest,
				}

				if len(cveMatch.AllAffected) > 0 {
					result["fixed_in"] = util.ExtractApplicableFixedVersion(cveMatch.Version, convertToModelsAffected(cveMatch.AllAffected))
				}

				results = append(results, result)
			}
		}

		if len(aggRelease.CveMatches) == 0 {
			releaseOnlyKey := releaseKey + ":NO_CVES"
			if !seen[releaseOnlyKey] {
				seen[releaseOnlyKey] = true

				results = append(results, map[string]interface{}{
					"cve_id":                    nil,
					"summary":                   nil,
					"details":                   nil,
					"severity_score":            nil,
					"severity_rating":           nil,
					"published":                 nil,
					"modified":                  nil,
					"aliases":                   []string{},
					"package":                   nil,
					"affected_version":          nil,
					"full_purl":                 nil,
					"fixed_in":                  []string{},
					"release_name":              aggRelease.ReleaseName,
					"release_version":           aggRelease.LatestVersion,
					"version_count":             aggRelease.VersionCount,
					"content_sha":               aggRelease.ContentSha,
					"project_type":              aggRelease.ProjectType,
					"openssf_scorecard_score":   aggRelease.OpenSSFScorecardScore,
					"dependency_count":          aggRelease.DependencyCount,
					"synced_endpoint_count":     aggRelease.SyncedEndpointCount,
					"vulnerability_count":       aggRelease.VulnerabilityCount,
					"vulnerability_count_delta": aggRelease.VulnerabilityCountDelta,
					"is_latest":                 aggRelease.IsLatest,
				})
			}
		}
	}

	return results, nil
}

// Helper function to convert generic map structure to models.Affected
func convertToModelsAffected(allAffected []map[string]interface{}) []models.Affected {
	var result []models.Affected
	for _, affectedMap := range allAffected {
		bytes, err := json.Marshal(affectedMap)
		if err != nil {
			continue
		}
		var affected models.Affected
		if err := json.Unmarshal(bytes, &affected); err == nil {
			result = append(result, affected)
		}
	}
	return result
}

// ResolveOrgAggregatedReleases aggregates release data by organization.
// Also merges in any system_tracked_repos entries whose owner has no release
// records yet — these appear as pending cards in the UI.
func ResolveOrgAggregatedReleases(db database.DBConnection, severity string, username string) ([]interface{}, error) {
	ctx := context.Background()
	severityScore := util.GetSeverityScore(severity)

	userOrgs := []string{}

	if username != "" {
		userQuery := `FOR u IN users FILTER u.username == @username LIMIT 1 RETURN u.orgs`
		cursor, err := db.Database.Query(ctx, userQuery, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{"username": username},
		})
		if err == nil {
			defer cursor.Close()
			if cursor.HasMore() {
				var orgs []string
				_, readErr := cursor.ReadDocument(ctx, &orgs)
				if readErr == nil && orgs != nil {
					userOrgs = orgs
				}
			}
		}
	}

	query := `
		LET severityThreshold = @severityScore
		LET userOrgs = @user_orgs

		FOR r IN release
			FILTER r.is_latest == true
			FILTER r.is_public == true OR (LENGTH(userOrgs) == 0 OR r.org IN userOrgs)

			LET latest = r

			LET total_versions = LENGTH(FOR v IN release FILTER v.name == latest.name RETURN 1)

			LET previous = (
				FOR v IN release
					FILTER v.name == latest.name AND v._key != latest._key
					SORT v.version_major DESC,
						 v.version_minor DESC,
						 v.version_patch DESC,
						 v.version_prerelease ASC,
						 v.version DESC
					LIMIT 1
					RETURN v
			)[0]

			LET sbomData = FIRST(
				FOR s IN 1..1 OUTBOUND latest release2sbom
					LIMIT 1
					RETURN s._id
			)

			LET dependency_count = sbomData != null
				? LENGTH(
					FOR edge IN sbom2purl
						FILTER edge._from == sbomData
						COLLECT fullPurl = edge.full_purl
						RETURN 1
				)
				: 0

			LET synced_endpoint_count = FIRST(
				FOR s IN sync
					FILTER s.release_name == latest.name AND s.release_version == latest.version
					COLLECT WITH COUNT INTO count
					RETURN count
			)

			// NEW: collect per-type endpoint counts for org card badges.
			// Joins sync → endpoint on name, groups by endpoint_type, counts distinct endpoints.
			// Falls back to "unknown" when endpoint_type is missing or empty.
			LET endpoint_type_counts = (
				FOR s IN sync
					FILTER s.release_name == latest.name AND s.release_version == latest.version
					FOR e IN endpoint
						FILTER e.name == s.endpoint_name
						COLLECT ep_type = (e.endpoint_type != null AND e.endpoint_type != "" ? e.endpoint_type : "unknown")
						WITH COUNT INTO type_count
						RETURN { label: ep_type, count: type_count }
			)

			LET cveMatches = (
				FOR cve, edge IN 1..1 OUTBOUND latest release2cve
					FILTER severityThreshold == 0.0 OR cve.database_specific.cvss_base_score >= severityThreshold
					RETURN {
						cveId: cve.id,
						package: edge.package_purl,
						severity: cve.database_specific.severity_rating,
						score: cve.database_specific.cvss_base_score
					}
			)

			LET uniqueMatches = (
				FOR m IN cveMatches
					COLLECT cveId = m.cveId, package = m.package INTO g = m
					LET severity = g[0].severity
					RETURN { cveId, package, severity }
			)

			LET severity_counts = (
				FOR m IN uniqueMatches
					COLLECT severity = m.severity WITH COUNT INTO count
					RETURN { severity, count }
			)

			LET prevVulnCount = previous != null
				? LENGTH(
					FOR cve, edge IN 1..1 OUTBOUND previous release2cve
						COLLECT cveId = cve.id, package = edge.package_purl
						RETURN 1
				)
				: null

			LET critical_count = SUM(
				FOR sc IN severity_counts FILTER sc.severity == "CRITICAL" RETURN sc.count
			)
			LET high_count = SUM(
				FOR sc IN severity_counts FILTER sc.severity == "HIGH" RETURN sc.count
			)
			LET medium_count = SUM(
				FOR sc IN severity_counts FILTER sc.severity == "MEDIUM" RETURN sc.count
			)
			LET low_count = SUM(
				FOR sc IN severity_counts FILTER sc.severity == "LOW" RETURN sc.count
			)

			LET row = {
				total_versions,
				dependency_count,
				synced_endpoint_count,
				endpoint_type_counts,
				vulnerability_count: LENGTH(uniqueMatches),
				vulnerability_count_delta: prevVulnCount != null ? (LENGTH(uniqueMatches) - prevVulnCount) : null,
				max_severity: LENGTH(cveMatches) > 0 ? MAX(cveMatches[*].score) : 0,
				scorecard_score: latest.openssf_scorecard_score,
				critical_count,
				high_count,
				medium_count,
				low_count
			}

			COLLECT org2 = latest.org INTO rows = row

			LET validScores = (
				FOR r IN rows
					FILTER r.scorecard_score != null AND r.scorecard_score > 0
					RETURN r.scorecard_score
			)

			RETURN {
				org_name: org2,
				total_releases: LENGTH(rows),
				total_versions: SUM(rows[*].total_versions),
				total_vulnerabilities: SUM(rows[*].vulnerability_count),
				critical_count: SUM(rows[*].critical_count),
				high_count: SUM(rows[*].high_count),
				medium_count: SUM(rows[*].medium_count),
				low_count: SUM(rows[*].low_count),
				max_severity_score: MAX(rows[*].max_severity),
				avg_scorecard_score: LENGTH(validScores) > 0 ? AVG(validScores) : null,
				total_dependencies: SUM(rows[*].dependency_count),
				synced_endpoint_count: SUM(rows[*].synced_endpoint_count),
				vulnerability_count_delta: SUM(rows[*].vulnerability_count_delta),
				pending_scan: false,
				// Merge per-type counts across all releases in the org.
				// Sums counts by label so [kubernetes:10, kubernetes:5] → [kubernetes:15].
				endpoint_type_counts: (
					FOR row IN rows
						FOR tc IN row.endpoint_type_counts
							COLLECT label = tc.label AGGREGATE count = SUM(tc.count)
							RETURN { label, count }
				)
			}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"severityScore": severityScore,
			"user_orgs":     userOrgs,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var results []interface{}
	scannedOrgs := make(map[string]bool)

	for cursor.HasMore() {
		var result map[string]interface{}
		_, err := cursor.ReadDocument(ctx, &result)
		if err != nil {
			continue
		}
		if orgName, ok := result["org_name"].(string); ok {
			scannedOrgs[orgName] = true
		}
		results = append(results, result)
	}

	// Find system_tracked_repos owners that have no release records yet.
	// ... [EXISTING CODE] ...
	// Find system_tracked_repos owners that have no release records yet.
	pendingQuery := `
		FOR t IN system_tracked_repos
			FILTER NOT (
				FOR r IN release
					FILTER r.org == t.owner AND r.is_public == true
					LIMIT 1
					RETURN 1
			)[0]
			COLLECT owner = t.owner, provider = t.provider
			RETURN { owner, provider }
	`
	pendingCursor, err := db.Database.Query(ctx, pendingQuery, nil)
	if err == nil {
		defer pendingCursor.Close()
		for pendingCursor.HasMore() {
			var row struct {
				Owner    string `json:"owner"`
				Provider string `json:"provider"`
			}
			if _, err := pendingCursor.ReadDocument(ctx, &row); err != nil {
				continue
			}
			if scannedOrgs[row.Owner] {
				continue
			}

			// Add this line to mark pending orgs as seen
			scannedOrgs[row.Owner] = true

			results = append(results, map[string]interface{}{
				"org_name":                  row.Owner,
				"total_releases":            0,
				"total_versions":            0,
				"total_vulnerabilities":     0,
				"critical_count":            0,
				"high_count":                0,
				"medium_count":              0,
				"low_count":                 0,
				"max_severity_score":        nil,
				"avg_scorecard_score":       nil,
				"total_dependencies":        0,
				"synced_endpoint_count":     0,
				"vulnerability_count_delta": nil,
				"pending_scan":              true,
				"endpoint_type_counts":      []interface{}{},
			})
		}
	}

	// ========================================================================
	// NEW CODE: Fill the gap for empty orgs that have no releases or pending scans
	// ========================================================================
	for _, orgName := range userOrgs {
		if !scannedOrgs[orgName] {
			scannedOrgs[orgName] = true

			results = append(results, map[string]interface{}{
				"org_name":                  orgName,
				"total_releases":            0,
				"total_versions":            0,
				"total_vulnerabilities":     0,
				"critical_count":            0,
				"high_count":                0,
				"medium_count":              0,
				"low_count":                 0,
				"max_severity_score":        nil,
				"avg_scorecard_score":       nil,
				"total_dependencies":        0,
				"synced_endpoint_count":     0,
				"vulnerability_count_delta": nil,
				"pending_scan":              false,
				"endpoint_type_counts":      []interface{}{},
			})
		}
	}

	return results, nil
}

// ResolveReleaseTimeline returns per-version CVE severity counts for a release name.
func ResolveReleaseTimeline(db database.DBConnection, name string) ([]map[string]interface{}, error) {
	ctx := context.Background()

	query := `
		FOR r IN release
			FILTER r.name == @name

			LET syncCount = (
				FOR s IN sync
					FILTER s.release_name == r.name AND s.release_version == r.version
					COLLECT WITH COUNT INTO count
					RETURN count
			)[0]

			LET cveMatches = (
				FOR cve, edge IN 1..1 OUTBOUND r release2cve
					COLLECT cveId = cve.id, pkg = edge.package_purl
					AGGREGATE severityRating = MAX(cve.database_specific.severity_rating)
					RETURN { severity: UPPER(severityRating) }
			)

			LET critical = LENGTH(FOR m IN cveMatches FILTER m.severity == "CRITICAL" RETURN 1)
			LET high     = LENGTH(FOR m IN cveMatches FILTER m.severity == "HIGH"     RETURN 1)
			LET medium   = LENGTH(FOR m IN cveMatches FILTER m.severity == "MEDIUM"   RETURN 1)
			LET low      = LENGTH(FOR m IN cveMatches FILTER m.severity == "LOW"      RETURN 1)

			SORT r.version_major DESC,
			     r.version_minor DESC,
			     r.version_patch DESC,
			     r.version_prerelease ASC,
			     r.version DESC

			RETURN {
				version:        r.version,
				build_date:     r.builddate,
				endpoint_count: syncCount || 0,
				critical_count: critical,
				high_count:     high,
				medium_count:   medium,
				low_count:      low,
				total_cves:     LENGTH(cveMatches),
				content_sha:    r.contentsha
			}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"name": name,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	type TimelineRow struct {
		Version       string  `json:"version"`
		BuildDate     *string `json:"build_date"`
		EndpointCount int     `json:"endpoint_count"`
		CriticalCount int     `json:"critical_count"`
		HighCount     int     `json:"high_count"`
		MediumCount   int     `json:"medium_count"`
		LowCount      int     `json:"low_count"`
		TotalCves     int     `json:"total_cves"`
		ContentSha    string  `json:"content_sha"`
	}

	var rows []TimelineRow
	for cursor.HasMore() {
		var row TimelineRow
		if _, err := cursor.ReadDocument(ctx, &row); err != nil {
			continue
		}
		rows = append(rows, row)
	}

	results := make([]map[string]interface{}, 0, len(rows))
	for i, row := range rows {
		results = append(results, map[string]interface{}{
			"version":        row.Version,
			"build_date":     row.BuildDate,
			"endpoint_count": row.EndpointCount,
			"critical_count": row.CriticalCount,
			"high_count":     row.HighCount,
			"medium_count":   row.MediumCount,
			"low_count":      row.LowCount,
			"total_cves":     row.TotalCves,
			"content_sha":    row.ContentSha,
			"is_latest":      i == 0,
		})
	}

	return results, nil
}
