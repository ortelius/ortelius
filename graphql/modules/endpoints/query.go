// Package endpoints defines the GraphQL queries for endpoint management.
package endpoints

import (
	"github.com/graphql-go/graphql"
	"github.com/ortelius/pdvd-backend/v12/database"
)

// GetQueryFields returns the endpoint queries to be mounted in the root schema.
func GetQueryFields(db database.DBConnection, syncedEndpointType *graphql.Object, endpointDetailsType *graphql.Object) graphql.Fields {
	return graphql.Fields{
		"syncedEndpoints": &graphql.Field{
			Type: graphql.NewList(syncedEndpointType),
			Args: graphql.FieldConfigArgument{
				"limit": &graphql.ArgumentConfig{Type: graphql.Int, DefaultValue: 1000},
				"org":   &graphql.ArgumentConfig{Type: graphql.String, DefaultValue: ""},
			},
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				limit := p.Args["limit"].(int)
				org := p.Args["org"].(string)
				return ResolveSyncedEndpoints(db, limit, org)
			},
		},
		"endpointDetails": &graphql.Field{
			Type: endpointDetailsType,
			Args: graphql.FieldConfigArgument{
				"name": &graphql.ArgumentConfig{Type: graphql.NewNonNull(graphql.String)},
			},
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				name := p.Args["name"].(string)
				return ResolveEndpointDetails(db, name)
			},
		},
	}
}
