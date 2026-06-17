// Package users defines the GraphQL types and resolvers for the authenticated
// user's own account data (profile fields, favorites, etc).
//
// This is distinct from org membership (model.User.Orgs, set via RBAC) and
// from repo-level watch/scan state (model.Org.TrackedRepos). FavoriteOrgs is
// a lightweight per-user bookmark list with no effect on scanning or access.
package users

import (
	"github.com/graphql-go/graphql"
	"github.com/ortelius/ortelius/v12/model"
)

// UserType exposes the subset of model.User fields relevant to the
// authenticated user's own profile via GraphQL. Sensitive fields
// (PasswordHash, GitHubToken, ExternalID) are intentionally omitted.
var UserType = graphql.NewObject(graphql.ObjectConfig{
	Name: "User",
	Fields: graphql.Fields{
		"username": &graphql.Field{
			Type: graphql.String,
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				if u, ok := p.Source.(*model.User); ok {
					return u.Username, nil
				}
				return nil, nil
			},
		},
		"email": &graphql.Field{
			Type: graphql.String,
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				if u, ok := p.Source.(*model.User); ok {
					return u.Email, nil
				}
				return nil, nil
			},
		},
		"role": &graphql.Field{
			Type: graphql.String,
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				if u, ok := p.Source.(*model.User); ok {
					return u.Role, nil
				}
				return nil, nil
			},
		},
		"orgs": &graphql.Field{
			Type: graphql.NewList(graphql.String),
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				if u, ok := p.Source.(*model.User); ok {
					return u.Orgs, nil
				}
				return []string{}, nil
			},
		},
		"favorite_orgs": &graphql.Field{
			Type: graphql.NewList(graphql.String),
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				if u, ok := p.Source.(*model.User); ok {
					return u.FavoriteOrgs, nil
				}
				return []string{}, nil
			},
		},
	},
})
