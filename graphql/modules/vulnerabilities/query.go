// Package vulnerabilities defines the GraphQL queries for vulnerability management.
package vulnerabilities

import (
	"github.com/graphql-go/graphql"
	"github.com/ortelius/pdvd-backend/v12/database"
)

// GetQueryFields returns the vulnerability queries to be mounted in the root schema.
func GetQueryFields(db database.DBConnection) graphql.Fields {
	return graphql.Fields{
		"vulnerabilities": &graphql.Field{
			Type: graphql.NewList(MitigationType),
			Args: graphql.FieldConfigArgument{
				"limit": &graphql.ArgumentConfig{Type: graphql.Int, DefaultValue: 1000},
				"org":   &graphql.ArgumentConfig{Type: graphql.String, DefaultValue: ""},
			},
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				limit := p.Args["limit"].(int)
				org := p.Args["org"].(string)
				return ResolveVulnerabilities(db, limit, org)
			},
		},
	}
}
