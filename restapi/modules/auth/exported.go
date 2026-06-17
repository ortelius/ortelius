// restapi/modules/auth/exported.go
package auth

import (
	"context"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
)

// LoadUserByUsername is an exported wrapper around the package-private
// getUserByUsername, for use by other packages (e.g. graphql/modules/users)
// that need to load a user record without duplicating the lookup logic.
//
// Named LoadUserByUsername (not GetUserByUsername) to avoid any future
// collision with handler-factory naming conventions used elsewhere in this
// package (e.g. GetUser(db) fiber.Handler).
func LoadUserByUsername(ctx context.Context, db database.DBConnection, username string) (*model.User, error) {
	return getUserByUsername(ctx, db, username)
}

// SaveFavoriteOrgs persists only the favorite_orgs field for a user.
//
// This is intentionally separate from the package-private updateUser,
// which writes a fixed set of profile fields (email, role, orgs, status,
// github token, etc.) and does NOT include favorite_orgs. Routing favorites
// through updateUser would silently drop the change, so this does a
// narrowly-scoped update instead.
func SaveFavoriteOrgs(ctx context.Context, db database.DBConnection, username string, favoriteOrgs []string) error {
	query := `
		FOR u IN users
		FILTER u.username == @username
		UPDATE u WITH {
			favorite_orgs: @favorite_orgs
		} IN users
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"username":      username,
			"favorite_orgs": favoriteOrgs,
		},
	})
	return err
}
