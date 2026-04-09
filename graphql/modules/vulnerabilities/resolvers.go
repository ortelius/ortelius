// Package vulnerabilities implements the resolvers for vulnerability data.
package vulnerabilities

import (
	"context"
	"encoding/json"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/google/osv-scanner/pkg/models"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/util"
)

// ResolveVulnerabilities fetches a list of vulnerabilities with optional limiting.
// REFACTORED: Now uses release2cve materialized edges instead of complex AQL filtering
func ResolveVulnerabilities(db database.DBConnection, limit int, org string) ([]map[string]interface{}, error) {
	ctx := context.Background()

	// REFACTORED: Use release2cve materialized edges (pre-validated at write-time)
	query := `
		LET vulnData = (
			FOR release IN release
				FILTER @org == "" OR release.org == @org
				FOR cve, edge IN 1..1 OUTBOUND release release2cve
					FILTER cve.database_specific.cvss_base_score != null
					
					RETURN {
						cve_id: cve.id,
						summary: cve.summary,
						severity_score: cve.database_specific.cvss_base_score,
						severity_rating: cve.database_specific.severity_rating,
						release_name: release.name,
						release_version: release.version,
						
						// Retrieved directly from materialized edge (already validated)
						package: edge.package_purl,
						affected_version: edge.package_version,
						full_purl: edge.package_purl,
						
						// Still needed for fixed_in calculation
						all_affected: cve.affected
					}
		)
		
		FOR vuln IN vulnData
			COLLECT 
				cve_id = vuln.cve_id,
				package = vuln.package,
				affected_version = vuln.affected_version
			AGGREGATE 
				summaries = UNIQUE(vuln.summary),
				severity_scores = UNIQUE(vuln.severity_score),
				severity_ratings = UNIQUE(vuln.severity_rating),
				releaseList = UNIQUE(CONCAT(vuln.release_name, ":", vuln.release_version)),
				full_purls = UNIQUE(vuln.full_purl),
				all_affected_data = UNIQUE(vuln.all_affected)
			
			LET endpointCount = LENGTH(
				FOR rel_str IN releaseList
					LET parts = SPLIT(rel_str, ":")
					FOR sync IN sync
						FILTER sync.release_name == parts[0] AND sync.release_version == parts[1]
						LIMIT 1
						RETURN 1
			)
			
			LET max_severity_score = MAX(severity_scores)
			
			SORT max_severity_score DESC
			LIMIT @limit
			
			RETURN {
				cve_id: cve_id,
				summary: FIRST(summaries) != null ? FIRST(summaries) : "",
				severity_score: max_severity_score,
				severity_rating: FIRST(severity_ratings) != null ? FIRST(severity_ratings) : "UNKNOWN",
				package: package,
				affected_version: affected_version,
				full_purl: FIRST(full_purls),
				affected_releases: LENGTH(releaseList),
				affected_endpoints: endpointCount,
				affected_data: FIRST(all_affected_data)
			}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"limit": limit,
			"org":   org,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	type VulnerabilityResult struct {
		CveID             string                   `json:"cve_id"`
		Summary           string                   `json:"summary"`
		SeverityScore     float64                  `json:"severity_score"`
		SeverityRating    string                   `json:"severity_rating"`
		Package           string                   `json:"package"`
		AffectedVersion   string                   `json:"affected_version"`
		FullPurl          string                   `json:"full_purl"`
		AffectedReleases  int                      `json:"affected_releases"`
		AffectedEndpoints int                      `json:"affected_endpoints"`
		AffectedData      []map[string]interface{} `json:"affected_data"`
	}

	var vulnerabilities []map[string]interface{}
	seen := make(map[string]bool)

	for cursor.HasMore() {
		var result VulnerabilityResult
		_, err := cursor.ReadDocument(ctx, &result)
		if err != nil {
			continue
		}

		// NO VALIDATION NEEDED - edges are pre-validated during ingestion
		key := result.CveID + ":" + result.Package + ":" + result.AffectedVersion
		if seen[key] {
			continue
		}
		seen[key] = true

		vulnerabilities = append(vulnerabilities, map[string]interface{}{
			"cve_id":             result.CveID,
			"summary":            result.Summary,
			"severity_score":     result.SeverityScore,
			"severity_rating":    result.SeverityRating,
			"package":            result.Package,
			"affected_version":   result.AffectedVersion,
			"full_purl":          result.FullPurl,
			"fixed_in":           util.ExtractApplicableFixedVersion(result.AffectedVersion, convertToModelsAffected(result.AffectedData)),
			"affected_releases":  result.AffectedReleases,
			"affected_endpoints": result.AffectedEndpoints,
		})
	}
	return vulnerabilities, nil
}

// Helper function to convert generic map structure to models.Affected for util functions
func convertToModelsAffected(allAffected []map[string]interface{}) []models.Affected {
	var result []models.Affected
	for _, affectedMap := range allAffected {
		// Convert map to JSON bytes then to struct to ensure proper field mapping
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
