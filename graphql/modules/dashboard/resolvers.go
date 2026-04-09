// Package dashboard implements the resolvers for dashboard metrics.
// It provides GraphQL resolvers for vulnerability trend analysis, MTTR calculations,
// and dashboard overview statistics using a version-aware snapshot model.
package dashboard

import (
	"context"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/pdvd-backend/v12/database"
)

// ResolveOverview handles fetching the high-level dashboard metrics.
func ResolveOverview(db database.DBConnection) (interface{}, error) {
	ctx := context.Background()
	query := `
		RETURN {
			total_releases: LENGTH(release),
			total_endpoints: LENGTH(endpoint),
			total_cves: LENGTH(cve)
		}
	`
	cursor, err := db.Database.Query(ctx, query, nil)
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var result map[string]interface{}
	if cursor.HasMore() {
		_, err = cursor.ReadDocument(ctx, &result)
	}
	return result, err
}

// ResolveSeverityDistribution fetches current breakdown of issues using latest active version snapshots.
func ResolveSeverityDistribution(db database.DBConnection) (interface{}, error) {
	ctx := context.Background()
	query := `
		LET latest_syncs = (
			FOR sync IN sync
				COLLECT 
					endpoint = sync.endpoint_name,
					release = sync.release_name
				AGGREGATE 
					latest_sync_time = MAX(DATE_TIMESTAMP(sync.synced_at))
				
				LET version = (
					FOR s IN sync 
						FILTER s.endpoint_name == endpoint 
						AND s.release_name == release 
						AND DATE_TIMESTAMP(s.synced_at) == latest_sync_time 
						LIMIT 1 
						RETURN s.release_version
				)[0]
				
				RETURN { endpoint, release, version }
		)
		
		LET counts = (
			FOR latest IN latest_syncs
				FOR r IN cve_lifecycle
					FILTER r.endpoint_name == latest.endpoint
					FILTER r.release_name == latest.release
					FILTER r.introduced_version == latest.version
					FILTER r.is_remediated == false
					COLLECT severity = UPPER(r.severity_rating) WITH COUNT INTO count
					RETURN { [LOWER(severity)]: count }
		)
		RETURN MERGE(counts)
	`
	cursor, err := db.Database.Query(ctx, query, nil)
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var result map[string]int
	if cursor.HasMore() {
		_, err = cursor.ReadDocument(ctx, &result)
	}
	return result, err
}

// ResolveTopRisks fetches the top risky assets based on current active version snapshots.
func ResolveTopRisks(db database.DBConnection, assetType string, limit int, org string) (interface{}, error) {
	ctx := context.Background()
	baseSnapshotAQL := `
		LET latest_syncs = (
			FOR sync IN sync
				COLLECT 
					endpoint = sync.endpoint_name,
					release = sync.release_name
				AGGREGATE 
					latest_sync_time = MAX(DATE_TIMESTAMP(sync.synced_at))
				LET version = (FOR s IN sync FILTER s.endpoint_name == endpoint AND s.release_name == release AND DATE_TIMESTAMP(s.synced_at) == latest_sync_time LIMIT 1 RETURN s.release_version)[0]
				RETURN { endpoint, release, version }
		)`

	var query string
	if assetType == "releases" {
		query = baseSnapshotAQL + `
			FOR latest in latest_syncs
				// Filter by Org
				LET relDoc = (FOR r IN release FILTER r.name == latest.release AND r.version == latest.version LIMIT 1 RETURN r)[0]
				FILTER @org == "" OR relDoc.org == @org

				FOR r IN cve_lifecycle
					FILTER r.endpoint_name == latest.endpoint AND r.release_name == latest.release
					FILTER r.introduced_version == latest.version AND r.is_remediated == false
					COLLECT release = r.release_name, version = r.introduced_version AGGREGATE 
						critical = SUM(UPPER(r.severity_rating) == "CRITICAL" ? 1 : 0),
						high = SUM(UPPER(r.severity_rating) == "HIGH" ? 1 : 0),
						total = COUNT(r)
					SORT critical DESC, high DESC, total DESC
					LIMIT @limit
					RETURN { name: release, version: version, critical_count: critical, high_count: high, total_vulns: total }`
	} else {
		query = baseSnapshotAQL + `
			FOR latest in latest_syncs
				// Filter by Org
				LET relDoc = (FOR r IN release FILTER r.name == latest.release AND r.version == latest.version LIMIT 1 RETURN r)[0]
				FILTER @org == "" OR relDoc.org == @org

				FOR r IN cve_lifecycle
					FILTER r.endpoint_name == latest.endpoint AND r.release_name == latest.release
					FILTER r.introduced_version == latest.version AND r.is_remediated == false
					COLLECT endpoint = r.endpoint_name AGGREGATE 
						critical = SUM(UPPER(r.severity_rating) == "CRITICAL" ? 1 : 0),
						high = SUM(UPPER(r.severity_rating) == "HIGH" ? 1 : 0),
						total = COUNT(r)
					SORT critical DESC, high DESC, total DESC
					LIMIT @limit
					RETURN { name: endpoint, version: "-", critical_count: critical, high_count: high, total_vulns: total }`
	}

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

	var risks []map[string]interface{}
	for cursor.HasMore() {
		var risk map[string]interface{}
		if _, err := cursor.ReadDocument(ctx, &risk); err == nil {
			risks = append(risks, risk)
		}
	}
	return risks, nil
}

// ResolveVulnerabilityTrend returns daily counts using a two-phase approach:
//  1. Compute the latest active (endpoint, release, version) snapshot ONCE and pull all
//     relevant lifecycle events in a single pass — avoiding repeated full-collection scans.
//  2. Aggregate those pre-fetched events per day in AQL, which operates entirely on the
//     in-memory result set rather than hitting the collections again.
//
// This reduces database I/O from O(days × collections) to O(1 × collections).
func ResolveVulnerabilityTrend(db database.DBConnection, days int, org string) ([]map[string]interface{}, error) {
	ctx := context.Background()
	if days <= 0 {
		days = 180
	}
	now := time.Now().UTC()

	query := `
		// ── PHASE 1: resolve active snapshots and pull lifecycle events ONCE ──────

		LET latest_snapshots = (
			FOR s IN sync
				COLLECT endpoint = s.endpoint_name, releaseName = s.release_name
				AGGREGATE latest_ts = MAX(DATE_TIMESTAMP(s.synced_at))
				LET version = (
					FOR sv IN sync
						FILTER sv.endpoint_name == endpoint
						AND sv.release_name == releaseName
						AND DATE_TIMESTAMP(sv.synced_at) == latest_ts
						LIMIT 1
						RETURN sv.release_version
				)[0]

				// Org filter applied once here, not repeated per day
				LET relDoc = (
					FOR r IN release
						FILTER r.name == releaseName AND r.version == version
						LIMIT 1
						RETURN r.org
				)[0]
				FILTER @org == "" OR relDoc == @org

				RETURN { endpoint, release: releaseName, version }
		)

		// Pull every lifecycle event that could appear in any day of the window.
		// The composite index on (endpoint_name, release_name, introduced_version)
		// makes this lookup fast rather than a full collection scan.
		LET all_lifecycle = (
			FOR active IN latest_snapshots
				FOR r IN cve_lifecycle
					FILTER r.endpoint_name == active.endpoint
					AND r.release_name == active.release
					AND r.introduced_version == active.version
					RETURN {
						severity:       UPPER(r.severity_rating),
						introduced_ts:  DATE_TIMESTAMP(r.introduced_at),
						remediated_ts:  r.remediated_at != null ? DATE_TIMESTAMP(r.remediated_at) : null
					}
		)

		// ── PHASE 2: aggregate per day from the in-memory result set ─────────────

		LET now_ts = DATE_TIMESTAMP(DATE_TRUNC(@now, "day"))

		FOR day_offset IN 0..@days
			LET current_day_ts = DATE_SUBTRACT(now_ts, day_offset, "day")
			LET eod_ts = DATE_TIMESTAMP(DATE_ADD(current_day_ts, 1, "day"))

			LET day_counts = (
				FOR r IN all_lifecycle
					FILTER r.introduced_ts <= eod_ts
					FILTER r.remediated_ts == null OR r.remediated_ts > eod_ts
					COLLECT severity = r.severity WITH COUNT INTO count
					RETURN { severity, count }
			)

			RETURN {
				date:     DATE_FORMAT(current_day_ts, "%yyyy-%mm-%dd"),
				critical: FIRST(FOR d IN day_counts FILTER d.severity == "CRITICAL" RETURN d.count) || 0,
				high:     FIRST(FOR d IN day_counts FILTER d.severity == "HIGH"     RETURN d.count) || 0,
				medium:   FIRST(FOR d IN day_counts FILTER d.severity == "MEDIUM"   RETURN d.count) || 0,
				low:      FIRST(FOR d IN day_counts FILTER d.severity == "LOW"      RETURN d.count) || 0
			}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"now":  now.Format(time.RFC3339),
			"days": days,
			"org":  org,
		},
	})
	if err != nil {
		return []map[string]interface{}{}, err
	}
	defer cursor.Close()

	var trendData []map[string]interface{}
	for cursor.HasMore() {
		var point map[string]interface{}
		if _, err := cursor.ReadDocument(ctx, &point); err == nil {
			trendData = append(trendData, point)
		}
	}

	// Results come out newest-first (day_offset 0 = today); reverse to chronological order.
	for i, j := 0, len(trendData)-1; i < j; i, j = i+1, j-1 {
		trendData[i], trendData[j] = trendData[j], trendData[i]
	}
	return trendData, nil
}

// ResolveDashboardGlobalStatus calculates aggregated vulnerability counts and deltas.
func ResolveDashboardGlobalStatus(db database.DBConnection, _ int, org string) (map[string]interface{}, error) {
	ctx := context.Background()
	windowTime := time.Now().AddDate(0, 0, -30)

	query := `
		LET get_active_state = (FOR ts IN [@now, @window]
			RETURN (
				FOR sync IN sync
					FILTER DATE_TIMESTAMP(sync.synced_at) <= DATE_TIMESTAMP(ts)
					
					// Filter by Org
					LET relDoc = (FOR r IN release FILTER r.name == sync.release_name AND r.version == sync.release_version LIMIT 1 RETURN r)[0]
					FILTER @org == "" OR relDoc.org == @org

					COLLECT endpoint = sync.endpoint_name, release = sync.release_name INTO groups = sync
					LET latest = (FOR g IN groups SORT DATE_TIMESTAMP(g.synced_at) DESC LIMIT 1 RETURN g)[0]
					RETURN { endpoint, release, version: latest.release_version, snapshot_ts: DATE_TIMESTAMP(ts) }
			)
		)

		LET current_snapshot = get_active_state[0]
		LET previous_snapshot = get_active_state[1]

		LET current_cves = (
			FOR active IN current_snapshot
				FOR r IN cve_lifecycle
					FILTER r.endpoint_name == active.endpoint 
					AND r.release_name == active.release 
					AND r.introduced_version == active.version
					FILTER r.is_remediated == false
					RETURN r
		)

		LET previous_cves = (
			FOR active IN previous_snapshot
				FOR r IN cve_lifecycle
					FILTER r.endpoint_name == active.endpoint 
					AND r.release_name == active.release 
					AND r.introduced_version == active.version
					FILTER DATE_TIMESTAMP(r.introduced_at) <= active.snapshot_ts
					FILTER (r.remediated_at == null OR DATE_TIMESTAMP(r.remediated_at) > active.snapshot_ts)
					RETURN r
		)

		LET current_results = (FOR r IN current_cves COLLECT sev = LOWER(r.severity_rating) WITH COUNT INTO count RETURN { severity: sev, count })
		LET prev_results = (FOR r IN previous_cves COLLECT sev = LOWER(r.severity_rating) WITH COUNT INTO count RETURN { severity: sev, count })

		LET combined = (
			FOR sev_val IN ["critical", "high", "medium", "low"]
				LET curr = FIRST(FOR r IN current_results FILTER r.severity == sev_val RETURN r.count) || 0
				LET prev = FIRST(FOR r IN prev_results FILTER r.severity == sev_val RETURN r.count) || 0
				RETURN { severity: sev_val, count: curr, delta: curr - prev }
		)

		// Calculate High-Risk Backlog (Critical + High)
		LET critical_count = FIRST(FOR r IN combined FILTER r.severity == "critical" RETURN r.count) || 0
		LET high_count = FIRST(FOR r IN combined FILTER r.severity == "high" RETURN r.count) || 0
		LET high_risk_backlog = critical_count + high_count

		LET critical_delta = FIRST(FOR r IN combined FILTER r.severity == "critical" RETURN r.delta) || 0
		LET high_delta = FIRST(FOR r IN combined FILTER r.severity == "high" RETURN r.delta) || 0
		LET high_risk_delta = critical_delta + high_delta

		RETURN {
			critical: FIRST(FOR r IN combined FILTER r.severity == "critical" RETURN { count: r.count, delta: r.delta }) || { count: 0, delta: 0 },
			high: FIRST(FOR r IN combined FILTER r.severity == "high" RETURN { count: r.count, delta: r.delta }) || { count: 0, delta: 0 },
			medium: FIRST(FOR r IN combined FILTER r.severity == "medium" RETURN { count: r.count, delta: r.delta }) || { count: 0, delta: 0 },
			low: FIRST(FOR r IN combined FILTER r.severity == "low" RETURN { count: r.count, delta: r.delta }) || { count: 0, delta: 0 },
			total_count: SUM(combined[*].count),
			total_delta: SUM(combined[*].delta),
			high_risk_backlog: high_risk_backlog,
			high_risk_delta: high_risk_delta
		}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"now":    time.Now().Format(time.RFC3339),
			"window": windowTime.Format(time.RFC3339),
			"org":    org,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var result map[string]interface{}
	if cursor.HasMore() {
		_, err = cursor.ReadDocument(ctx, &result)
	}
	return result, err
}

// ResolveMTTR calculates comprehensive metrics using root_introduced_at for cross-version duration accuracy.
func ResolveMTTR(db database.DBConnection, days int, org string) (map[string]interface{}, error) {
	ctx := context.Background()
	if days <= 0 {
		days = 180
	}
	cutoffTimestamp := time.Now().AddDate(0, 0, -days).Unix() * 1000

	query := `
		LET sla_def = { 
			"CRITICAL": { "default": 15, "high_risk": 7 }, 
			"HIGH":     { "default": 30, "high_risk": 15 }, 
			"MEDIUM":   { "default": 90, "high_risk": 90 }, 
			"LOW":      { "default": 180, "high_risk": 180 },
			"NONE":     { "default": 365, "high_risk": 365 }
		}

		LET ep_map = MERGE(FOR e IN endpoint RETURN { [e.name]: e.endpoint_type })

		LET latest_snapshots = (
			FOR sync IN sync
				// Filter by Org
				LET relDoc = (FOR r IN release FILTER r.name == sync.release_name AND r.version == sync.release_version LIMIT 1 RETURN r)[0]
				FILTER @org == "" OR relDoc.org == @org

				COLLECT endpoint = sync.endpoint_name, release = sync.release_name
				AGGREGATE latest_ts = MAX(DATE_TIMESTAMP(sync.synced_at))
				LET version = (FOR s IN sync FILTER s.endpoint_name == endpoint AND s.release_name == release AND DATE_TIMESTAMP(s.synced_at) == latest_ts LIMIT 1 RETURN s.release_version)[0]
				RETURN { endpoint, release, version }
		)

		LET current_snapshot_cves = (
			FOR active IN latest_snapshots
				FOR lifecycle IN cve_lifecycle
					FILTER lifecycle.endpoint_name == active.endpoint 
					AND lifecycle.release_name == active.release 
					AND lifecycle.introduced_version == active.version
					FILTER lifecycle.is_remediated == false
					
					LET ep_type = HAS(ep_map, lifecycle.endpoint_name) ? ep_map[lifecycle.endpoint_name] : "unknown"
					LET is_high_risk = (ep_type == "mission_asset")
					LET sev_key = UPPER(lifecycle.severity_rating)
					LET sla_entry = HAS(sla_def, sev_key) ? sla_def[sev_key] : sla_def["NONE"]
					LET sla_days = is_high_risk ? sla_entry.high_risk : sla_entry.default
					LET discovery_ts = DATE_TIMESTAMP(lifecycle.root_introduced_at != null ? lifecycle.root_introduced_at : lifecycle.introduced_at)
					
					RETURN MERGE(lifecycle, {
						endpoint_type: ep_type,
						sla_days: sla_days,
						open_age: DATE_DIFF(discovery_ts, DATE_NOW(), "d"),
						is_post: (lifecycle.disclosed_after_deployment == true)
					})
		)

		LET all_events = (
			FOR r IN cve_lifecycle
				LET remediated_ts = r.remediated_at != null ? DATE_TIMESTAMP(r.remediated_at) : null
				FILTER remediated_ts != null
				
				// Filter by Org
				LET relDoc = (FOR rel IN release FILTER rel.name == r.release_name AND rel.version == r.introduced_version LIMIT 1 RETURN rel)[0]
				FILTER @org == "" OR relDoc.org == @org

				LET ep_type = HAS(ep_map, r.endpoint_name) ? ep_map[r.endpoint_name] : "unknown"
				LET is_high_risk = (ep_type == "mission_asset")
				
				LET sev_key = UPPER(r.severity_rating)
				LET sla_entry = HAS(sla_def, sev_key) ? sla_def[sev_key] : sla_def["NONE"]
				LET sla_days = is_high_risk ? sla_entry.high_risk : sla_entry.default
				
				LET discovery_ts = DATE_TIMESTAMP(r.root_introduced_at != null ? r.root_introduced_at : r.introduced_at)
				LET total_duration = DATE_DIFF(discovery_ts, remediated_ts, "d")

				RETURN MERGE(r, {
					endpoint_type: ep_type,
					sla_days: sla_days,
					days_to_remediate: total_duration,
					in_window_fix: (remediated_ts >= @cutoffTimestamp),
					is_post: (r.disclosed_after_deployment == true)
				})
		)

		LET severity_groups = (
			FOR severity_val IN ["CRITICAL", "HIGH", "MEDIUM", "LOW"]
				LET fixed_in_window = (FOR g IN all_events FILTER UPPER(g.severity_rating) == severity_val AND g.in_window_fix RETURN g)
				LET count_fixed = LENGTH(fixed_in_window)
				LET sum_mttr = SUM(fixed_in_window[*].days_to_remediate)

				LET fixed_post = (FOR g IN fixed_in_window FILTER g.is_post == true RETURN g)
				LET count_fixed_post = LENGTH(fixed_post)
				LET sum_mttr_post = SUM(fixed_post[*].days_to_remediate)

				LET open_items = (FOR g IN current_snapshot_cves FILTER UPPER(g.severity_rating) == severity_val RETURN g)
				LET count_open = LENGTH(open_items)
				
				LET open_post = (FOR g IN open_items FILTER g.is_post == true RETURN g)
				LET count_open_post = LENGTH(open_post)
				LET sum_open_age_post = SUM(open_post[*].open_age)

				LET open_beyond_sla = LENGTH(FOR g IN open_items FILTER g.open_age > g.sla_days RETURN 1)
				
				RETURN {
					severity: severity_val,
					mttr: count_fixed > 0 ? sum_mttr / count_fixed : 0,
					mttr_post_deployment: count_fixed_post > 0 ? sum_mttr_post / count_fixed_post : 0,
					fixed_within_sla_pct: count_fixed > 0 ? (SUM(FOR g IN fixed_in_window FILTER g.days_to_remediate <= g.sla_days RETURN 1) * 100.0 / count_fixed) : 0,
					open_count: count_open,
					backlog_count: count_open,
					mean_open_age: count_open > 0 ? AVG(open_items[*].open_age) : 0,
					mean_open_age_post_deploy: count_open_post > 0 ? sum_open_age_post / count_open_post : 0,
					oldest_open_days: count_open > 0 ? MAX(open_items[*].open_age) : 0,
					open_beyond_sla_pct: count_open > 0 ? (open_beyond_sla * 100.0 / count_open) : 0,
					open_beyond_sla_count: open_beyond_sla,
					new_detected: count_open,
					remediated: count_fixed,
					open_post_count: count_open_post,
					
					_sum_mttr: sum_mttr || 0,
					_sum_mttr_post: sum_mttr_post || 0,
					_count_fixed_post: count_fixed_post,
					_sum_open_age: SUM(open_items[*].open_age) || 0,
					_sum_open_age_post: sum_open_age_post || 0,
					_count_fixed_within_sla: SUM(FOR g IN fixed_in_window FILTER g.days_to_remediate <= g.sla_days RETURN 1)
				}
		)

		LET total_fixed = SUM(severity_groups[*].remediated)
		LET total_open = SUM(severity_groups[*].open_count)
		LET total_open_post = SUM(severity_groups[*].open_post_count)
		LET total_fixed_post = SUM(severity_groups[*]._count_fixed_post)

		LET exec_summary = {
			total_new_cves: SUM(severity_groups[*].new_detected),
			total_fixed_cves: total_fixed,
			post_deployment_cves: total_open_post,
			mttr_all: total_fixed > 0 ? SUM(severity_groups[*]._sum_mttr) / total_fixed : 0,
			mttr_post_deployment: total_fixed_post > 0 ? SUM(severity_groups[*]._sum_mttr_post) / total_fixed_post : 0,
			mean_open_age_all: total_open > 0 ? SUM(severity_groups[*]._sum_open_age) / total_open : 0,
			mean_open_age_post_deploy: total_open_post > 0 ? SUM(severity_groups[*]._sum_open_age_post) / total_open_post : 0,
			open_cves_beyond_sla_pct: total_open > 0 ? (SUM(severity_groups[*].open_beyond_sla_count) * 100.0 / total_open) : 0,
			fixed_within_sla_pct: total_fixed > 0 ? (SUM(severity_groups[*]._count_fixed_within_sla) * 100.0 / total_fixed) : 0,
			oldest_open_critical_days: MAX(FOR g IN severity_groups FILTER g.severity == "CRITICAL" RETURN g.oldest_open_days),
			backlog_delta: SUM(severity_groups[*].new_detected) - total_fixed
		}

		RETURN {
			by_severity: severity_groups,
			executive_summary: exec_summary,
			endpoint_impact: {
				affected_endpoints_count: LENGTH(UNIQUE(FOR e IN current_snapshot_cves RETURN e.endpoint_name)),
				post_deployment_cves_by_type: (FOR e IN current_snapshot_cves FILTER e.is_post == true COLLECT type = e.endpoint_type WITH COUNT INTO count RETURN { type, count })
			}
		}
	`

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"cutoffTimestamp": cutoffTimestamp, "org": org},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var data map[string]interface{}
	if cursor.HasMore() {
		_, err = cursor.ReadDocument(ctx, &data)
	}

	return data, err
}
