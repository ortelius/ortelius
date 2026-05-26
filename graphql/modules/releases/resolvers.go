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

		// Methodology: One record for every unique CVE ID + Package combination
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
// Always returns the latest version per release name regardless of CVE count,
// so the releases page shows the current state even when the latest has zero CVEs.
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
					SORT release.version_major != null ? release.version_major : -1 DESC,
						release.version_minor != null ? release.version_minor : -1 DESC,
						release.version_patch != null ? release.version_patch : -1 DESC,
						release.version_prerelease != null && release.version_prerelease != "" ? 1 : 0 ASC,
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
					SORT release.version_major != null ? release.version_major : -1 DESC,
						release.version_minor != null ? release.version_minor : -1 DESC,
						release.version_patch != null ? release.version_patch : -1 DESC,
						release.version_prerelease != null && release.version_prerelease != "" ? 1 : 0 ASC,
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

		// Always emit a row for releases with zero CVEs so the latest version
		// is always visible on the releases page regardless of CVE count.
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

// ResolveOrgAggregatedReleases aggregates release data by organization
// FIXED: Replaced slow COLLECT grouping with instant O(1) IsLatest lookup
func ResolveOrgAggregatedReleases(db database.DBConnection, severity string, username string) ([]interface{}, error) {
	ctx := context.Background()
	severityScore := util.GetSeverityScore(severity)

	userOrgs := []string{}
	filterByPublic := true

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
		filterByPublic = false
	}

	query := `
		LET severityThreshold = @severityScore
		LET filterPublic = @filter_by_public
		LET userOrgs = @user_orgs

		// FAST PATH: Instantly grab only the latest releases using the new boolean flag
		FOR r IN release
			FILTER r.is_latest == true
			FILTER filterPublic == true ? r.is_public == true : (LENGTH(userOrgs) == 0 OR r.org IN userOrgs)

			LET latest = r

			LET total_versions = LENGTH(FOR v IN release FILTER v.name == latest.name RETURN 1)

			// Find previous version for delta calculation
			LET previous = (
				FOR v IN release
					FILTER v.name == latest.name AND v._key != latest._key
					SORT (v.version_major != null ? v.version_major : -1) DESC,
						 (v.version_minor != null ? v.version_minor : -1) DESC,
						 (v.version_patch != null ? v.version_patch : -1) DESC,
						 (v.version_prerelease != null && v.version_prerelease != "" ? 1 : 0) ASC,
						 v.version_prerelease DESC,
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

			LET row = {
				total_versions,
				dependency_count,
				synced_endpoint_count,
				vulnerability_count: LENGTH(uniqueMatches),
				vulnerability_count_delta: prevVulnCount != null ? (LENGTH(uniqueMatches) - prevVulnCount) : null,
				max_severity: LENGTH(cveMatches) > 0 ? MAX(cveMatches[*].score) : 0,
				scorecard_score: latest.openssf_scorecard_score,
				severity_counts
			}

			// Group back up by Organization to generate final totals
			COLLECT org2 = latest.org INTO rows = row

			LET critical = SUM(
				FOR r IN rows
					FOR sc IN r.severity_counts
						FILTER sc.severity == "CRITICAL"
						RETURN sc.count
			)

			LET high = SUM(
				FOR r IN rows
					FOR sc IN r.severity_counts
						FILTER sc.severity == "HIGH"
						RETURN sc.count
			)

			LET medium = SUM(
				FOR r IN rows
					FOR sc IN r.severity_counts
						FILTER sc.severity == "MEDIUM"
						RETURN sc.count
			)

			LET low = SUM(
				FOR r IN rows
					FOR sc IN r.severity_counts
						FILTER sc.severity == "LOW"
						RETURN sc.count
			)

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
				critical_count: critical,
				high_count: high,
				medium_count: medium,
				low_count: low,
				max_severity_score: MAX(rows[*].max_severity),
				avg_scorecard_score: LENGTH(validScores) > 0 ? AVG(validScores) : null,
				total_dependencies: SUM(rows[*].dependency_count),
				synced_endpoint_count: SUM(rows[*].synced_endpoint_count),
				vulnerability_count_delta: SUM(rows[*].vulnerability_count_delta)
			}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"severityScore":    severityScore,
			"user_orgs":        userOrgs,
			"filter_by_public": filterByPublic,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var results []interface{}
	for cursor.HasMore() {
		var result map[string]interface{}
		_, err := cursor.ReadDocument(ctx, &result)
		if err != nil {
			continue
		}
		results = append(results, result)
	}

	return results, nil
}

// ResolveReleaseTimeline returns per-version CVE severity counts for a release name,
// sorted semver descending. Uses the same release2cve graph traversal as
// ResolveAffectedReleases so counts are consistent with the releases page.
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

			SORT r.version_major != null ? r.version_major : -1 DESC,
			     r.version_minor != null ? r.version_minor : -1 DESC,
			     r.version_patch != null ? r.version_patch : -1 DESC,
			     r.version_prerelease != null && r.version_prerelease != "" ? 1 : 0 ASC,
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

	// Identify latest version using same semver sort as ResolveAffectedReleases
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

	// First row is latest (AQL sorted semver desc)
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
