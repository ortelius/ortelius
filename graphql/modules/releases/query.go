// Package releases defines the GraphQL queries for release management.
package releases

import (
	"context"
	"net/url"
	"strings"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/graphql-go/graphql"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
	"github.com/ortelius/pdvd-backend/v12/restapi/modules/auth" // Imported for Context Keys
)

// GetQueryFields returns the release queries to be mounted in the root schema.
func GetQueryFields(db database.DBConnection, releaseType *graphql.Object, affectedReleaseType *graphql.Object, severityType *graphql.Enum) graphql.Fields {
	return graphql.Fields{
		"release": &graphql.Field{
			Type: releaseType,
			Args: graphql.FieldConfigArgument{
				"name":    &graphql.ArgumentConfig{Type: graphql.NewNonNull(graphql.String)},
				"version": &graphql.ArgumentConfig{Type: graphql.NewNonNull(graphql.String)},
			},
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				name := p.Args["name"].(string)
				version := p.Args["version"].(string)

				decodedName, err := url.QueryUnescape(name)
				if err != nil {
					decodedName = name
				}

				ctx := context.Background()
				query := `
					FOR r IN release
						FILTER r.name == @name && r.version == @version
						LIMIT 1
						RETURN r
				`
				cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
					BindVars: map[string]interface{}{
						"name":    decodedName,
						"version": version,
					},
				})
				if err != nil {
					return nil, err
				}
				defer cursor.Close()
				var release model.ProjectRelease
				if !cursor.HasMore() {
					return nil, nil
				}
				_, err = cursor.ReadDocument(ctx, &release)
				if err != nil {
					return nil, err
				}
				return release, nil
			},
		},
		"affectedReleases": &graphql.Field{
			Type: graphql.NewList(affectedReleaseType),
			Args: graphql.FieldConfigArgument{
				"severity": &graphql.ArgumentConfig{Type: graphql.NewNonNull(severityType)},
				"limit":    &graphql.ArgumentConfig{Type: graphql.Int, DefaultValue: 100},
				"org":      &graphql.ArgumentConfig{Type: graphql.String, DefaultValue: ""},
			},
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				severity := p.Args["severity"].(string)
				org := p.Args["org"].(string)
				return ResolveAffectedReleases(db, strings.ToLower(severity), org)
			},
		},
		"orgAggregatedReleases": &graphql.Field{
			Type: graphql.NewList(OrgAggregatedReleaseType),
			Args: graphql.FieldConfigArgument{
				"severity": &graphql.ArgumentConfig{Type: graphql.NewNonNull(severityType)},
			},
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				severity := p.Args["severity"].(string)

				// Extract username from GraphQL context using the typed key
				username := ""
				if p.Context != nil && p.Context.Value(auth.UserKey) != nil {
					username = p.Context.Value(auth.UserKey).(string)
				}

				return ResolveOrgAggregatedReleases(db, strings.ToLower(severity), username)
			},
		},
	}
}
