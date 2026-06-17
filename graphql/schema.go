// Package graphql provides the main schema construction and database initialization for the GraphQL API.
package graphql

import (
	"github.com/graphql-go/graphql"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/graphql/modules/dashboard"
	"github.com/ortelius/ortelius/v12/graphql/modules/endpoints"
	"github.com/ortelius/ortelius/v12/graphql/modules/releases"
	"github.com/ortelius/ortelius/v12/graphql/modules/scorecard"
	"github.com/ortelius/ortelius/v12/graphql/modules/users"
	"github.com/ortelius/ortelius/v12/graphql/modules/vulnerabilities"
)

var db database.DBConnection

// InitDB initializes the database connection for GraphQL resolvers
func InitDB(dbConn database.DBConnection) {
	db = dbConn
}

// CreateSchema assembles the complete GraphQL schema from all modules
func CreateSchema() (graphql.Schema, error) {
	// Step 1: Initialize base types that don't have circular dependencies
	vulnerabilityCountType := vulnerabilities.VulnerabilityCountType
	vulnerabilityType := vulnerabilities.VulnerabilityType
	severityType := vulnerabilities.SeverityType

	// Step 2: Initialize scorecard types
	scorecardResultType := scorecard.ScorecardResultType

	// Step 3: Initialize endpoint types (with dependency injection for circular refs)
	syncedEndpointTypeWithVulns, endpointDetailsType := endpoints.GetEndpointTypes(
		vulnerabilityCountType,
		vulnerabilityType,
	)
	affectedEndpointType := endpoints.AffectedEndpointType

	// Step 4: Initialize release types (with dependency injection for circular refs)
	releaseType := releases.GetReleaseType(
		db,
		vulnerabilityType,
		affectedEndpointType,
		scorecardResultType,
	)
	affectedReleaseType := releases.AffectedReleaseType

	// Step 5: Build unified query fields from all modules
	queryFields := graphql.Fields{}

	// Add release queries (release, affectedReleases)
	for k, v := range releases.GetQueryFields(db, releaseType, affectedReleaseType, severityType) {
		queryFields[k] = v
	}

	// Add vulnerability queries (vulnerabilities)
	for k, v := range vulnerabilities.GetQueryFields(db) {
		queryFields[k] = v
	}

	// Add Dashboard queries
	for k, v := range dashboard.GetQueryFields(db) {
		queryFields[k] = v
	}

	// Add endpoint queries (syncedEndpoints, endpointDetails)
	for k, v := range endpoints.GetQueryFields(db, syncedEndpointTypeWithVulns, endpointDetailsType) {
		queryFields[k] = v
	}

	// Add user queries (myFavoriteOrgs)
	for k, v := range users.GetQueryFields(db) {
		queryFields[k] = v
	}

	// Note: Scorecard data is accessed via release.scorecard_result field
	// No dedicated scorecard queries needed

	// Step 6: Create root query object
	rootQuery := graphql.NewObject(graphql.ObjectConfig{
		Name:   "Query",
		Fields: queryFields,
	})

	// Step 7: Build unified mutation fields from all modules.
	// This is currently the only module contributing mutations; the schema
	// previously had no mutation root at all.
	mutationFields := graphql.Fields{}

	for k, v := range users.GetMutationFields(db) {
		mutationFields[k] = v
	}

	rootMutation := graphql.NewObject(graphql.ObjectConfig{
		Name:   "Mutation",
		Fields: mutationFields,
	})

	// Step 8: Build and return final schema
	return graphql.NewSchema(graphql.SchemaConfig{
		Query:    rootQuery,
		Mutation: rootMutation,
	})
}
