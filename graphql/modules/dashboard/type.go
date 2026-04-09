// Package dashboard defines the GraphQL types for the application dashboard.
package dashboard

import (
	"github.com/graphql-go/graphql"
)

// DashboardOverviewType represents the high-level metrics for the top cards
var DashboardOverviewType = graphql.NewObject(graphql.ObjectConfig{
	Name: "DashboardOverview",
	Fields: graphql.Fields{
		"total_releases":  &graphql.Field{Type: graphql.Int},
		"total_endpoints": &graphql.Field{Type: graphql.Int},
		"total_cves":      &graphql.Field{Type: graphql.Int},
	},
})

// SeverityDistributionType represents the data for the pie/bar charts
var SeverityDistributionType = graphql.NewObject(graphql.ObjectConfig{
	Name: "SeverityDistribution",
	Fields: graphql.Fields{
		"critical": &graphql.Field{Type: graphql.Int},
		"high":     &graphql.Field{Type: graphql.Int},
		"medium":   &graphql.Field{Type: graphql.Int},
		"low":      &graphql.Field{Type: graphql.Int},
	},
})

// RiskyAssetType represents rows for the "Top Risky" tables
var RiskyAssetType = graphql.NewObject(graphql.ObjectConfig{
	Name: "RiskyAsset",
	Fields: graphql.Fields{
		"name":           &graphql.Field{Type: graphql.String},
		"version":        &graphql.Field{Type: graphql.String},
		"critical_count": &graphql.Field{Type: graphql.Int},
		"high_count":     &graphql.Field{Type: graphql.Int},
		"total_vulns":    &graphql.Field{Type: graphql.Int},
	},
})

// VulnerabilityTrendType represents the daily count of vulnerabilities from sync events
var VulnerabilityTrendType = graphql.NewObject(graphql.ObjectConfig{
	Name: "VulnerabilityTrend",
	Fields: graphql.Fields{
		"date":     &graphql.Field{Type: graphql.String},
		"critical": &graphql.Field{Type: graphql.Int},
		"high":     &graphql.Field{Type: graphql.Int},
		"medium":   &graphql.Field{Type: graphql.Int},
		"low":      &graphql.Field{Type: graphql.Int},
	},
})

// SeverityMetricType represents a count and its delta for a specific severity
var SeverityMetricType = graphql.NewObject(graphql.ObjectConfig{
	Name: "SeverityMetric",
	Fields: graphql.Fields{
		"count": &graphql.Field{Type: graphql.Int},
		"delta": &graphql.Field{Type: graphql.Int},
	},
})

// DashboardGlobalStatusType represents the aggregated vulnerability status across all endpoints
var DashboardGlobalStatusType = graphql.NewObject(graphql.ObjectConfig{
	Name: "DashboardGlobalStatus",
	Fields: graphql.Fields{
		"critical":          &graphql.Field{Type: SeverityMetricType},
		"high":              &graphql.Field{Type: SeverityMetricType},
		"medium":            &graphql.Field{Type: SeverityMetricType},
		"low":               &graphql.Field{Type: SeverityMetricType},
		"total_count":       &graphql.Field{Type: graphql.Int},
		"total_delta":       &graphql.Field{Type: graphql.Int},
		"high_risk_backlog": &graphql.Field{Type: graphql.Int},
		"high_risk_delta":   &graphql.Field{Type: graphql.Int},
	},
})

// ============================================================================
// MTTR & Advanced Metrics Types
// ============================================================================

// ExecutiveSummaryType implements Section G: Executive Summary Block
var ExecutiveSummaryType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ExecutiveSummary",
	Fields: graphql.Fields{
		"total_new_cves":            &graphql.Field{Type: graphql.Int},   // Section D.1
		"total_fixed_cves":          &graphql.Field{Type: graphql.Int},   // Section D.2
		"post_deployment_cves":      &graphql.Field{Type: graphql.Int},   // Section E.2 (Total)
		"mttr_all":                  &graphql.Field{Type: graphql.Float}, // Section A.1
		"mttr_post_deployment":      &graphql.Field{Type: graphql.Float}, // Section A.3
		"mean_open_age_all":         &graphql.Field{Type: graphql.Float}, // Section B.1
		"mean_open_age_post_deploy": &graphql.Field{Type: graphql.Float}, // Section B.3
		"open_cves_beyond_sla_pct":  &graphql.Field{Type: graphql.Float}, // Section C.1
		"oldest_open_critical_days": &graphql.Field{Type: graphql.Float}, // Section B.4
		"backlog_delta":             &graphql.Field{Type: graphql.Int},   // Section D.3
		"fixed_within_sla_pct":      &graphql.Field{Type: graphql.Float}, // Section C.2 - Added for frontend
	},
})

// DetailedSeverityMetricsType implements Sections A, B, C, D broken down by severity
var DetailedSeverityMetricsType = graphql.NewObject(graphql.ObjectConfig{
	Name: "DetailedSeverityMetrics",
	Fields: graphql.Fields{
		"severity": &graphql.Field{Type: graphql.String},

		// A. Remediation Effectiveness
		"mttr":                 &graphql.Field{Type: graphql.Float}, // A.2
		"mttr_post_deployment": &graphql.Field{Type: graphql.Float}, // A.3 (by severity)
		"fixed_within_sla_pct": &graphql.Field{Type: graphql.Float}, // C.2

		// B. Active Risk Exposure
		"mean_open_age":             &graphql.Field{Type: graphql.Float}, // B.2
		"mean_open_age_post_deploy": &graphql.Field{Type: graphql.Float}, // B.3 (by severity)
		"oldest_open_days":          &graphql.Field{Type: graphql.Float}, // B.4

		// C. SLA Compliance
		"open_beyond_sla_pct":   &graphql.Field{Type: graphql.Float}, // C.1 percentage
		"open_beyond_sla_count": &graphql.Field{Type: graphql.Int},   // C.1 count - NEW FIELD

		// D. Volume & Flow
		"new_detected":  &graphql.Field{Type: graphql.Int}, // D.1
		"remediated":    &graphql.Field{Type: graphql.Int}, // D.2
		"backlog_count": &graphql.Field{Type: graphql.Int}, // Current Open Count (legacy name)
		"open_count":    &graphql.Field{Type: graphql.Int}, // Current Open Count - NEW FIELD (same as backlog_count)
	},
})

// EndpointImpactCountType helps implement Section E.2
var EndpointImpactCountType = graphql.NewObject(graphql.ObjectConfig{
	Name: "EndpointImpactCount",
	Fields: graphql.Fields{
		"type":  &graphql.Field{Type: graphql.String}, // e.g., "mission_asset", "cluster"
		"count": &graphql.Field{Type: graphql.Int},
	},
})

// EndpointImpactMetricsType implements Section E: Endpoint Impact Metrics
var EndpointImpactMetricsType = graphql.NewObject(graphql.ObjectConfig{
	Name: "EndpointImpactMetrics",
	Fields: graphql.Fields{
		"affected_endpoints_count":     &graphql.Field{Type: graphql.Int},                              // E.1
		"post_deployment_cves_by_type": &graphql.Field{Type: graphql.NewList(EndpointImpactCountType)}, // E.2
	},
})

// MTTRAnalysisType is the root container for the Dashboard Metrics response
var MTTRAnalysisType = graphql.NewObject(graphql.ObjectConfig{
	Name: "MTTRAnalysis",
	Fields: graphql.Fields{
		"executive_summary": &graphql.Field{Type: ExecutiveSummaryType},
		"by_severity":       &graphql.Field{Type: graphql.NewList(DetailedSeverityMetricsType)},
		"endpoint_impact":   &graphql.Field{Type: EndpointImpactMetricsType},
	},
})
