// Package endpoints implements the resolvers for endpoint data.
package endpoints

import (
	"context"
	"encoding/json"
	"net/url"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/google/osv-scanner/pkg/models"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/util"
)

// force build

// ============================================================================
// Shared Types & Helpers
// ============================================================================

// ReleaseKey identifies a unique (name, version) pair for batch vuln fetching.
type ReleaseKey struct {
	Name    string `json:"name"`
	Version string `json:"version"`
}

// deriveSeverityRating returns the severity rating string, falling back to
// deriving it from the numeric CVSS score when the rating field is empty.
func deriveSeverityRating(v map[string]interface{}) string {
	if rating, ok := v["severity_rating"].(string); ok && rating != "" {
		return rating
	}
	if score, ok := v["severity_score"].(float64); ok {
		switch {
		case score >= 9.0:
			return "CRITICAL"
		case score >= 7.0:
			return "HIGH"
		case score >= 4.0:
			return "MEDIUM"
		case score > 0:
			return "LOW"
		}
	}
	return ""
}

// countSeverities tallies vulnerability counts by severity from a vuln list.
// Uses deriveSeverityRating for consistent severity resolution.
func countSeverities(vulns []map[string]interface{}) map[string]int {
	counts := map[string]int{"critical": 0, "high": 0, "medium": 0, "low": 0}
	for _, v := range vulns {
		rating := deriveSeverityRating(v)
		switch rating {
		case "CRITICAL":
			counts["critical"]++
		case "HIGH":
			counts["high"]++
		case "MEDIUM":
			counts["medium"]++
		case "LOW":
			counts["low"]++
		}
	}
	return counts
}

// fetchReleaseVulnerabilities executes a single batch AQL query against release2cve
// edges for all requested (name, version) pairs. It returns a map keyed by
// "name:version" containing deduplicated vulnerability records.
//
// This is the canonical vulnerability resolution logic shared by both
// ResolveEndpointDetails and ResolveSyncedEndpoints to guarantee identical
// counts across all views.
func fetchReleaseVulnerabilities(db database.DBConnection, releasesToFetch []ReleaseKey) (map[string][]map[string]interface{}, error) {
	releaseVulnMap := make(map[string][]map[string]interface{})
	if len(releasesToFetch) == 0 {
		return releaseVulnMap, nil
	}

	ctx := context.Background()

	vulnQuery := `
		FOR target IN @targets
			LET releaseDoc = (
				FOR r IN release
					FILTER r.name == target.name AND r.version == target.version
					LIMIT 1
					RETURN r
			)[0]
			
			FILTER releaseDoc != null
			
			LET sbomData = (
				FOR s IN 1..1 OUTBOUND releaseDoc release2sbom
					LIMIT 1
					RETURN { id: s._id }
			)[0]
			
			// Get Dependency Count
			LET dependencyCount = (
				FILTER sbomData != null
				FOR edge IN sbom2purl
					FILTER edge._from == sbomData.id
					COLLECT fullPurl = edge.full_purl
					RETURN 1
			)
			
			// Get Vulnerabilities using release2cve materialized edges
			// Edges are pre-validated during ingestion - no filtering needed
			LET vulns = (
				FOR cve, edge IN 1..1 OUTBOUND releaseDoc release2cve
					RETURN {
						cve_id: cve.id,
						summary: cve.summary,
						severity_score: cve.database_specific.cvss_base_score,
						severity_rating: cve.database_specific.severity_rating,
						
						// Retrieved directly from materialized edge
						package: edge.package_purl,
						affected_version: edge.package_version,
						full_purl: edge.package_purl,
						
						// Still needed for fixed_in calculation
						all_affected: cve.affected
					}
			)
			
			RETURN {
				name: target.name,
				version: target.version,
				dependency_count: LENGTH(dependencyCount),
				vulns: vulns
			}
	`

	cursor, err := db.Database.Query(ctx, vulnQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"targets": releasesToFetch,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	type VulnRaw struct {
		CveID           string                   `json:"cve_id"`
		Summary         string                   `json:"summary"`
		SeverityScore   float64                  `json:"severity_score"`
		SeverityRating  string                   `json:"severity_rating"`
		Package         string                   `json:"package"`
		AffectedVersion string                   `json:"affected_version"`
		FullPurl        string                   `json:"full_purl"`
		AllAffected     []map[string]interface{} `json:"all_affected"`
	}

	type ReleaseResult struct {
		Name            string    `json:"name"`
		Version         string    `json:"version"`
		DependencyCount int       `json:"dependency_count"`
		Vulns           []VulnRaw `json:"vulns"`
	}

	for cursor.HasMore() {
		var res ReleaseResult
		_, err := cursor.ReadDocument(ctx, &res)
		if err != nil {
			continue
		}

		// Deduplicate per release by (CVE ID + Package)
		var validVulns []map[string]interface{}
		seen := make(map[string]bool)

		for _, v := range res.Vulns {
			key := v.CveID + ":" + v.Package
			if seen[key] {
				continue
			}
			seen[key] = true

			validVulns = append(validVulns, map[string]interface{}{
				"cve_id":           v.CveID,
				"summary":          v.Summary,
				"severity_score":   v.SeverityScore,
				"severity_rating":  v.SeverityRating,
				"package":          v.Package,
				"affected_version": v.AffectedVersion,
				"full_purl":        v.FullPurl,
				"fixed_in":         util.ExtractApplicableFixedVersion(v.AffectedVersion, convertToModelsAffected(v.AllAffected)),
				"dependency_count": res.DependencyCount,
			})
		}
		releaseVulnMap[res.Name+":"+res.Version] = validVulns
	}

	return releaseVulnMap, nil
}

// ============================================================================
// ResolveEndpointDetails
// ============================================================================

// ResolveEndpointDetails returns detailed endpoint information with vulnerabilities.
func ResolveEndpointDetails(db database.DBConnection, endpointName string) (map[string]interface{}, error) {
	ctx := context.Background()

	decodedName, err := url.QueryUnescape(endpointName)
	if err != nil {
		decodedName = endpointName
	}

	// ========================================================================
	// STEP 1: Inventory Query
	// ========================================================================
	inventoryQuery := `
		FOR endpoint IN endpoint
			FILTER endpoint.name == @endpointName
			LIMIT 1
			
			LET services = (
				FOR sync IN sync
					FILTER sync.endpoint_name == endpoint.name
					COLLECT releaseName = sync.release_name INTO groups = sync
					
					LET sortedSyncs = (
						FOR s IN groups
							SORT s.synced_at DESC
							RETURN { 
								version: s.release_version, 
								synced_at: s.synced_at 
							}
					)
					
					LET current = sortedSyncs[0]
					LET previous = LENGTH(sortedSyncs) > 1 ? sortedSyncs[1] : null
					
					LET releaseDoc = (
						FOR r IN release
							FILTER r.name == releaseName AND r.version == current.version
							LIMIT 1
							RETURN r
					)[0]
					
					FILTER releaseDoc != null
					
					RETURN {
						name: releaseName,
						current: current,
						previous: previous,
						release_doc: {
							openssf_scorecard_score: releaseDoc.openssf_scorecard_score,
							project_type: releaseDoc.projecttype
						}
					}
			)
			
			RETURN {
				endpoint: endpoint,
				services: services
			}
	`

	type ServiceState struct {
		Version  string `json:"version"`
		SyncedAt string `json:"synced_at"`
	}

	type ServiceInventory struct {
		Name       string        `json:"name"`
		Current    ServiceState  `json:"current"`
		Previous   *ServiceState `json:"previous"`
		ReleaseDoc struct {
			ScorecardScore *float64 `json:"openssf_scorecard_score"`
			ProjectType    string   `json:"project_type"`
		} `json:"release_doc"`
	}

	type InventoryResult struct {
		Endpoint struct {
			Name         string `json:"name"`
			EndpointType string `json:"endpoint_type"`
			Environment  string `json:"environment"`
		} `json:"endpoint"`
		Services []ServiceInventory `json:"services"`
	}

	cursor, err := db.Database.Query(ctx, inventoryQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"endpointName": decodedName,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	if !cursor.HasMore() {
		return nil, nil
	}

	var inventory InventoryResult
	_, err = cursor.ReadDocument(ctx, &inventory)
	if err != nil {
		return nil, err
	}

	// ========================================================================
	// STEP 2: Collect unique release keys
	// ========================================================================
	uniqueReleases := make(map[string]bool)
	var releasesToFetch []ReleaseKey

	for _, svc := range inventory.Services {
		currKey := svc.Name + ":" + svc.Current.Version
		if !uniqueReleases[currKey] {
			uniqueReleases[currKey] = true
			releasesToFetch = append(releasesToFetch, ReleaseKey{svc.Name, svc.Current.Version})
		}
		if svc.Previous != nil {
			prevKey := svc.Name + ":" + svc.Previous.Version
			if !uniqueReleases[prevKey] {
				uniqueReleases[prevKey] = true
				releasesToFetch = append(releasesToFetch, ReleaseKey{svc.Name, svc.Previous.Version})
			}
		}
	}

	// ========================================================================
	// STEP 3: Fetch vulnerabilities (shared logic)
	// ========================================================================
	releaseVulnMap, err := fetchReleaseVulnerabilities(db, releasesToFetch)
	if err != nil {
		return nil, err
	}

	// ========================================================================
	// STEP 4: Assembly and Delta Calculation
	// ========================================================================
	var finalReleases []map[string]interface{}

	endpointCurrCounts := map[string]int{"critical": 0, "high": 0, "medium": 0, "low": 0}
	endpointPrevCounts := map[string]int{"critical": 0, "high": 0, "medium": 0, "low": 0}

	for _, svc := range inventory.Services {
		// --- Current State ---
		currKey := svc.Name + ":" + svc.Current.Version
		currVulns := releaseVulnMap[currKey]

		currServiceCounts := countSeverities(currVulns)
		dependencyCount := 0
		if len(currVulns) > 0 {
			if dc, ok := currVulns[0]["dependency_count"].(int); ok {
				dependencyCount = dc
			}
		}

		// --- Previous State ---
		prevServiceCounts := map[string]int{"critical": 0, "high": 0, "medium": 0, "low": 0}
		if svc.Previous != nil {
			prevKey := svc.Name + ":" + svc.Previous.Version
			prevVulns := releaseVulnMap[prevKey]
			prevServiceCounts = countSeverities(prevVulns)
		}

		// --- Aggregation ---
		totalCurr := 0
		for k, v := range currServiceCounts {
			endpointCurrCounts[k] += v
			totalCurr += v
		}

		totalPrev := 0
		for k, v := range prevServiceCounts {
			endpointPrevCounts[k] += v
			totalPrev += v
		}

		svcDelta := totalCurr - totalPrev

		finalReleases = append(finalReleases, map[string]interface{}{
			"release_name":              svc.Name,
			"release_version":           svc.Current.Version,
			"openssf_scorecard_score":   svc.ReleaseDoc.ScorecardScore,
			"dependency_count":          dependencyCount,
			"last_sync":                 svc.Current.SyncedAt,
			"vulnerability_count":       totalCurr,
			"vulnerability_count_delta": svcDelta,
			"vulnerabilities":           currVulns,
		})
	}

	deltaCritical := endpointCurrCounts["critical"] - endpointPrevCounts["critical"]
	deltaHigh := endpointCurrCounts["high"] - endpointPrevCounts["high"]
	deltaMedium := endpointCurrCounts["medium"] - endpointPrevCounts["medium"]
	deltaLow := endpointCurrCounts["low"] - endpointPrevCounts["low"]
	totalDelta := deltaCritical + deltaHigh + deltaMedium + deltaLow

	lastSync := ""
	for _, svc := range inventory.Services {
		if lastSync == "" || svc.Current.SyncedAt > lastSync {
			lastSync = svc.Current.SyncedAt
		}
	}

	return map[string]interface{}{
		"endpoint_name":             inventory.Endpoint.Name,
		"endpoint_url":              inventory.Endpoint.Name,
		"endpoint_type":             inventory.Endpoint.EndpointType,
		"environment":               inventory.Endpoint.Environment,
		"status":                    "active",
		"last_sync":                 lastSync,
		"total_vulnerabilities":     endpointCurrCounts,
		"vulnerability_count_delta": totalDelta,
		"releases":                  finalReleases,
	}, nil
}

// ============================================================================
// ResolveSyncedEndpoints
// ============================================================================

// ResolveSyncedEndpoints fetches a list of endpoints that have been synced,
// filtered to only include releases belonging to the requested org.
// When org is provided:
//   - Only endpoints running at least one release from that org are returned.
//   - Each endpoint's services list only contains releases from that org,
//     so release_count and vulnerability counts reflect the org slice only.
//
// Uses the same fetchReleaseVulnerabilities + countSeverities logic as
// ResolveEndpointDetails to guarantee identical vulnerability counts.
func ResolveSyncedEndpoints(db database.DBConnection, limit int, org string) ([]map[string]interface{}, error) {
	ctx := context.Background()

	// ========================================================================
	// STEP 1: Inventory Query
	// ========================================================================
	inventoryQuery := `
		FOR endpoint IN endpoint
			// Gate endpoint inclusion on having at least one release from the
			// requested org. COLLECT first to deduplicate, then check org.
			LET orgReleases = (
				FOR sync IN sync
					FILTER sync.endpoint_name == endpoint.name
					COLLECT releaseName = sync.release_name INTO groups = sync
					LET version = (
						FOR s IN groups SORT s.synced_at DESC LIMIT 1 RETURN s.release_version
					)[0]
					LET r = (
						FOR r IN release
							FILTER r.name == releaseName AND r.version == version
							LIMIT 1
							RETURN r
					)[0]
					FILTER r != null AND (@org == "" OR LOWER(r.org) == LOWER(@org))
					LIMIT 1
					RETURN 1
			)
			FILTER @org == "" OR LENGTH(orgReleases) > 0
			LIMIT @limit

			// Services list filtered to only the org's releases.
			// Org filter applied AFTER COLLECT to avoid duplicates.
			LET services = (
				FOR sync IN sync
					FILTER sync.endpoint_name == endpoint.name
					COLLECT releaseName = sync.release_name INTO groups = sync

					LET sortedSyncs = (
						FOR s IN groups
							SORT s.synced_at DESC
							RETURN {
								version: s.release_version,
								synced_at: s.synced_at
							}
					)

					LET current = sortedSyncs[0]
					LET previous = LENGTH(sortedSyncs) > 1 ? sortedSyncs[1] : null

					LET latestReleaseDoc = (
						FOR r IN release
							FILTER r.name == releaseName AND r.version == current.version
							LIMIT 1
							RETURN r
					)[0]

					// Org filter applied here, after COLLECT, against the resolved doc
					FILTER latestReleaseDoc != null
					FILTER @org == "" OR LOWER(latestReleaseDoc.org) == LOWER(@org)

					RETURN {
						name: releaseName,
						current: current,
						previous: previous
					}
			)

			FILTER LENGTH(services) > 0

			LET lastSync = MAX(services[*].current.synced_at)

			RETURN {
				endpoint_name: endpoint.name,
				endpoint_url: endpoint.name,
				endpoint_type: endpoint.endpoint_type,
				environment: endpoint.environment,
				status: "active",
				last_sync: lastSync,
				release_count: LENGTH(services),
				services: services
			}
	`

	type ServiceState struct {
		Version  string `json:"version"`
		SyncedAt string `json:"synced_at"`
	}

	type ServiceInventory struct {
		Name     string        `json:"name"`
		Current  ServiceState  `json:"current"`
		Previous *ServiceState `json:"previous"`
	}

	type EndpointInventory struct {
		EndpointName string             `json:"endpoint_name"`
		EndpointURL  string             `json:"endpoint_url"`
		EndpointType string             `json:"endpoint_type"`
		Environment  string             `json:"environment"`
		Status       string             `json:"status"`
		LastSync     string             `json:"last_sync"`
		ReleaseCount int                `json:"release_count"`
		Services     []ServiceInventory `json:"services"`
	}

	cursor, err := db.Database.Query(ctx, inventoryQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"limit": limit,
			"org":   org,
		},
	})
	if err != nil {
		return []map[string]interface{}{}, nil
	}
	defer cursor.Close()

	var endpointsInv []EndpointInventory

	// ========================================================================
	// STEP 2: Collect unique release keys
	// ========================================================================
	uniqueReleases := make(map[string]bool)
	var releasesToFetch []ReleaseKey

	for cursor.HasMore() {
		var ep EndpointInventory
		_, err := cursor.ReadDocument(ctx, &ep)
		if err != nil {
			continue
		}
		endpointsInv = append(endpointsInv, ep)

		for _, svc := range ep.Services {
			currKey := svc.Name + ":" + svc.Current.Version
			if !uniqueReleases[currKey] {
				uniqueReleases[currKey] = true
				releasesToFetch = append(releasesToFetch, ReleaseKey{svc.Name, svc.Current.Version})
			}
			if svc.Previous != nil {
				prevKey := svc.Name + ":" + svc.Previous.Version
				if !uniqueReleases[prevKey] {
					uniqueReleases[prevKey] = true
					releasesToFetch = append(releasesToFetch, ReleaseKey{svc.Name, svc.Previous.Version})
				}
			}
		}
	}

	if len(endpointsInv) == 0 {
		return []map[string]interface{}{}, nil
	}

	// ========================================================================
	// STEP 3: Fetch vulnerabilities (shared logic — same query as EndpointDetails)
	// ========================================================================
	releaseVulnMap, err := fetchReleaseVulnerabilities(db, releasesToFetch)
	if err != nil {
		return nil, err
	}

	// ========================================================================
	// STEP 4: Assembly (shared counting logic via countSeverities)
	// ========================================================================
	var finalEndpoints []map[string]interface{}

	for _, ep := range endpointsInv {
		var releasesList []map[string]interface{}

		currCounts := map[string]int{"critical": 0, "high": 0, "medium": 0, "low": 0}
		prevCounts := map[string]int{"critical": 0, "high": 0, "medium": 0, "low": 0}

		for _, svc := range ep.Services {
			// 1. Current Vulnerabilities
			currKey := svc.Name + ":" + svc.Current.Version
			currVulns := releaseVulnMap[currKey]
			svcCurrCounts := countSeverities(currVulns)

			for k, v := range svcCurrCounts {
				currCounts[k] += v
			}

			// 2. Previous Vulnerabilities (if exists)
			if svc.Previous != nil {
				prevKey := svc.Name + ":" + svc.Previous.Version
				prevVulns := releaseVulnMap[prevKey]
				svcPrevCounts := countSeverities(prevVulns)

				for k, v := range svcPrevCounts {
					prevCounts[k] += v
				}
			}

			releasesList = append(releasesList, map[string]interface{}{
				"release_name":    svc.Name,
				"release_version": svc.Current.Version,
			})
		}

		totalVulnerabilities := map[string]interface{}{
			"critical": currCounts["critical"],
			"high":     currCounts["high"],
			"medium":   currCounts["medium"],
			"low":      currCounts["low"],
		}

		finalEndpoints = append(finalEndpoints, map[string]interface{}{
			"endpoint_name":         ep.EndpointName,
			"endpoint_url":          ep.EndpointURL,
			"endpoint_type":         ep.EndpointType,
			"environment":           ep.Environment,
			"status":                ep.Status,
			"last_sync":             ep.LastSync,
			"release_count":         ep.ReleaseCount,
			"total_vulnerabilities": totalVulnerabilities,
			"releases":              releasesList,
		})
	}

	return finalEndpoints, nil
}

// ============================================================================
// Package-level helpers
// ============================================================================

// convertToModelsAffected converts generic map structure to models.Affected for util functions.
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
