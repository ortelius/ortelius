// Package vulnerabilities defines the GraphQL types for vulnerability management.
package vulnerabilities

import (
	"github.com/graphql-go/graphql"
)

// SeverityType is the GraphQL Enum for vulnerability severity levels.
var SeverityType = graphql.NewEnum(graphql.EnumConfig{
	Name: "Severity",
	Values: graphql.EnumValueConfigMap{
		"CRITICAL": &graphql.EnumValueConfig{Value: "critical"},
		"HIGH":     &graphql.EnumValueConfig{Value: "high"},
		"MEDIUM":   &graphql.EnumValueConfig{Value: "medium"},
		"LOW":      &graphql.EnumValueConfig{Value: "low"},
		"NONE":     &graphql.EnumValueConfig{Value: "none"},
	},
})

// VulnerabilityCountType is the GraphQL Object for aggregated vulnerability counts.
var VulnerabilityCountType = graphql.NewObject(graphql.ObjectConfig{
	Name: "VulnerabilityCount",
	Fields: graphql.Fields{
		"critical": &graphql.Field{Type: graphql.Int},
		"high":     &graphql.Field{Type: graphql.Int},
		"medium":   &graphql.Field{Type: graphql.Int},
		"low":      &graphql.Field{Type: graphql.Int},
	},
})

// VulnerabilityType is the GraphQL Object for individual vulnerability details.
var VulnerabilityType = graphql.NewObject(graphql.ObjectConfig{
	Name: "Vulnerability",
	Fields: graphql.Fields{
		"cve_id":           &graphql.Field{Type: graphql.String},
		"summary":          &graphql.Field{Type: graphql.String},
		"details":          &graphql.Field{Type: graphql.String},
		"severity_score":   &graphql.Field{Type: graphql.Float},
		"severity_rating":  &graphql.Field{Type: graphql.String},
		"cvss_v3_score":    &graphql.Field{Type: graphql.String},
		"published":        &graphql.Field{Type: graphql.String},
		"modified":         &graphql.Field{Type: graphql.String},
		"aliases":          &graphql.Field{Type: graphql.NewList(graphql.String)},
		"package":          &graphql.Field{Type: graphql.String},
		"affected_version": &graphql.Field{Type: graphql.String},
		"full_purl":        &graphql.Field{Type: graphql.String},
		"fixed_in":         &graphql.Field{Type: graphql.NewList(graphql.String)},
	},
})

// MitigationType is the GraphQL Object for vulnerability mitigation details.
var MitigationType = graphql.NewObject(graphql.ObjectConfig{
	Name: "Mitigation",
	Fields: graphql.Fields{
		"cve_id":             &graphql.Field{Type: graphql.String},
		"summary":            &graphql.Field{Type: graphql.String},
		"severity_score":     &graphql.Field{Type: graphql.Float},
		"severity_rating":    &graphql.Field{Type: graphql.String},
		"package":            &graphql.Field{Type: graphql.String},
		"affected_version":   &graphql.Field{Type: graphql.String},
		"full_purl":          &graphql.Field{Type: graphql.String},
		"fixed_in":           &graphql.Field{Type: graphql.NewList(graphql.String)},
		"affected_releases":  &graphql.Field{Type: graphql.Int},
		"affected_endpoints": &graphql.Field{Type: graphql.Int},
	},
})
