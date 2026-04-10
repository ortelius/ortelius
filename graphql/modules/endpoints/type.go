// Package endpoints defines the GraphQL types for endpoint management.
package endpoints

import (
	"github.com/graphql-go/graphql"
)

// ReleaseInfoType represents basic release information.
var ReleaseInfoType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ReleaseInfo",
	Fields: graphql.Fields{
		"release_name":    &graphql.Field{Type: graphql.String},
		"release_version": &graphql.Field{Type: graphql.String},
	},
})

// SyncedEndpointType represents an endpoint that has been synced.
var SyncedEndpointType = graphql.NewObject(graphql.ObjectConfig{
	Name: "SyncedEndpoint",
	Fields: graphql.Fields{
		"endpoint_name": &graphql.Field{Type: graphql.String},
		"endpoint_url":  &graphql.Field{Type: graphql.String},
		"endpoint_type": &graphql.Field{Type: graphql.String},
		"environment":   &graphql.Field{Type: graphql.String},
		"status":        &graphql.Field{Type: graphql.String},
		"last_sync":     &graphql.Field{Type: graphql.String},
		"release_count": &graphql.Field{Type: graphql.Int},
	},
})

// AffectedEndpointType represents an endpoint affected by vulnerabilities.
var AffectedEndpointType = graphql.NewObject(graphql.ObjectConfig{
	Name: "AffectedEndpoint",
	Fields: graphql.Fields{
		"endpoint_name": &graphql.Field{Type: graphql.String},
		"endpoint_url":  &graphql.Field{Type: graphql.String},
		"endpoint_type": &graphql.Field{Type: graphql.String},
		"environment":   &graphql.Field{Type: graphql.String},
		"last_sync":     &graphql.Field{Type: graphql.String},
		"status":        &graphql.Field{Type: graphql.String},
	},
})

// GetEndpointTypes returns endpoint types with proper circular dependency handling
func GetEndpointTypes(vulnerabilityCountType *graphql.Object, vulnerabilityType *graphql.Object) (*graphql.Object, *graphql.Object) {
	endpointReleaseType := graphql.NewObject(graphql.ObjectConfig{
		Name: "EndpointRelease",
		Fields: graphql.Fields{
			"release_name":              &graphql.Field{Type: graphql.String},
			"release_version":           &graphql.Field{Type: graphql.String},
			"openssf_scorecard_score":   &graphql.Field{Type: graphql.Float},
			"dependency_count":          &graphql.Field{Type: graphql.Int},
			"last_sync":                 &graphql.Field{Type: graphql.String},
			"vulnerability_count":       &graphql.Field{Type: graphql.Int},
			"vulnerability_count_delta": &graphql.Field{Type: graphql.Int},
			"vulnerabilities":           &graphql.Field{Type: graphql.NewList(vulnerabilityType)},
		},
	})

	syncedEndpointTypeWithVulns := graphql.NewObject(graphql.ObjectConfig{
		Name: "SyncedEndpointWithVulns",
		Fields: graphql.Fields{
			"endpoint_name":         &graphql.Field{Type: graphql.String},
			"endpoint_url":          &graphql.Field{Type: graphql.String},
			"endpoint_type":         &graphql.Field{Type: graphql.String},
			"environment":           &graphql.Field{Type: graphql.String},
			"status":                &graphql.Field{Type: graphql.String},
			"last_sync":             &graphql.Field{Type: graphql.String},
			"release_count":         &graphql.Field{Type: graphql.Int},
			"total_vulnerabilities": &graphql.Field{Type: vulnerabilityCountType},
			"releases":              &graphql.Field{Type: graphql.NewList(ReleaseInfoType)},
		},
	})

	endpointDetailsType := graphql.NewObject(graphql.ObjectConfig{
		Name: "EndpointDetails",
		Fields: graphql.Fields{
			"endpoint_name":             &graphql.Field{Type: graphql.String},
			"endpoint_url":              &graphql.Field{Type: graphql.String},
			"endpoint_type":             &graphql.Field{Type: graphql.String},
			"environment":               &graphql.Field{Type: graphql.String},
			"status":                    &graphql.Field{Type: graphql.String},
			"last_sync":                 &graphql.Field{Type: graphql.String},
			"total_vulnerabilities":     &graphql.Field{Type: vulnerabilityCountType},
			"vulnerability_count_delta": &graphql.Field{Type: graphql.Int},
			"releases":                  &graphql.Field{Type: graphql.NewList(endpointReleaseType)},
		},
	})

	return syncedEndpointTypeWithVulns, endpointDetailsType
}
