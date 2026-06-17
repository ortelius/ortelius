package users

import (
	"context"
	"fmt"

	"github.com/graphql-go/graphql"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/restapi/modules/auth"
)

// usernameFromContext extracts the authenticated username set by the
// auth.RequireAuth / auth.OptionalAuth middleware, mirroring the pattern
// already used in graphql/modules/releases/query.go.
func usernameFromContext(ctx context.Context) (string, error) {
	if ctx == nil || ctx.Value(auth.UserKey) == nil {
		return "", fmt.Errorf("authentication required")
	}
	username, ok := ctx.Value(auth.UserKey).(string)
	if !ok || username == "" {
		return "", fmt.Errorf("authentication required")
	}
	return username, nil
}

// ResolveMyFavoriteOrgs returns the authenticated user's favorite org names.
func ResolveMyFavoriteOrgs(db database.DBConnection, ctx context.Context) ([]string, error) {
	username, err := usernameFromContext(ctx)
	if err != nil {
		return nil, err
	}

	user, err := auth.LoadUserByUsername(ctx, db, username)
	if err != nil {
		return nil, fmt.Errorf("failed to load user: %w", err)
	}

	if user.FavoriteOrgs == nil {
		return []string{}, nil
	}
	return user.FavoriteOrgs, nil
}

// ResolveToggleFavoriteOrg adds orgName to the authenticated user's
// favorites if absent, or removes it if present, persists the change, and
// returns the updated full list.
func ResolveToggleFavoriteOrg(db database.DBConnection, ctx context.Context, orgName string) ([]string, error) {
	username, err := usernameFromContext(ctx)
	if err != nil {
		return nil, err
	}

	if orgName == "" {
		return nil, fmt.Errorf("orgName is required")
	}

	user, err := auth.LoadUserByUsername(ctx, db, username)
	if err != nil {
		return nil, fmt.Errorf("failed to load user: %w", err)
	}

	updated := user.ToggleFavoriteOrg(orgName)

	if err := auth.SaveFavoriteOrgs(ctx, db, username, updated); err != nil {
		return nil, fmt.Errorf("failed to save favorites: %w", err)
	}

	return updated, nil
}

// GetQueryFields returns the user queries to be mounted in the root schema.
func GetQueryFields(db database.DBConnection) graphql.Fields {
	return graphql.Fields{
		"myFavoriteOrgs": &graphql.Field{
			Type: graphql.NewList(graphql.String),
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				return ResolveMyFavoriteOrgs(db, p.Context)
			},
		},
	}
}

// GetMutationFields returns the user mutations to be mounted in the root schema.
func GetMutationFields(db database.DBConnection) graphql.Fields {
	return graphql.Fields{
		"toggleFavoriteOrg": &graphql.Field{
			Type: graphql.NewList(graphql.String),
			Args: graphql.FieldConfigArgument{
				"orgName": &graphql.ArgumentConfig{Type: graphql.NewNonNull(graphql.String)},
			},
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				orgName := p.Args["orgName"].(string)
				return ResolveToggleFavoriteOrg(db, p.Context, orgName)
			},
		},
	}
}
