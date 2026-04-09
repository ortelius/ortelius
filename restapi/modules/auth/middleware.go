// Package auth provides authentication and authorization middleware for Fiber applications.
package auth

import (
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/pdvd-backend/v12/database"
)

// CtxKey defines a type for context keys to ensure type safety and avoid collisions.
type CtxKey string

// Exported context keys for user authentication data
const (
	UserKey CtxKey = "username"
	RoleKey CtxKey = "role"
	OrgsKey CtxKey = "orgs"
)

// RequireAuth middleware validates JWT token from cookie and blocks guests.
// UPDATED: Now requires DB connection to look up user details (Role/Orgs) not present in JWT.
func RequireAuth(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		token := c.Cookies("auth_token")
		if token == "" {
			return c.Status(fiber.StatusUnauthorized).JSON(fiber.Map{
				"error": "Authentication required",
			})
		}

		claims, err := ValidateJWT(token)
		if err != nil {
			return c.Status(fiber.StatusUnauthorized).JSON(fiber.Map{
				"error": "Invalid or expired session",
			})
		}

		// Perform DB Lookup to get fresh user details (Role, Orgs)
		ctx := c.Context()
		user, err := getUserByUsername(ctx, db, claims.Username)
		if err != nil {
			return c.Status(fiber.StatusUnauthorized).JSON(fiber.Map{
				"error": "User not found or inactive",
			})
		}

		// Store user info in context
		c.Locals("is_authenticated", true)
		c.Locals("username", user.Username)
		c.Locals("role", user.Role) // Fetched from DB
		c.Locals("orgs", user.Orgs) // Fetched from DB

		return c.Next()
	}
}

// OptionalAuth identifies the user if a token is present but does not block guests.
// This allows a single endpoint to serve both public and private data based on status.
// UPDATED: Now requires DB connection to look up user details.
func OptionalAuth(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		token := c.Cookies("auth_token")
		if token == "" {
			c.Locals("is_authenticated", false)
			return c.Next()
		}

		claims, err := ValidateJWT(token)
		if err != nil {
			// Treat invalid/expired tokens as guest access
			c.Locals("is_authenticated", false)
			return c.Next()
		}

		// Perform DB Lookup
		ctx := c.Context()
		user, err := getUserByUsername(ctx, db, claims.Username)
		if err != nil {
			// If token is valid but user deleted/inactive, treat as guest
			c.Locals("is_authenticated", false)
			return c.Next()
		}

		// User is authenticated; set context for handlers
		c.Locals("is_authenticated", true)
		c.Locals("username", user.Username)
		c.Locals("role", user.Role) // Fetched from DB
		c.Locals("orgs", user.Orgs) // Fetched from DB

		return c.Next()
	}
}

// RequireRole middleware checks if user has one of the required roles
func RequireRole(allowedRoles ...string) fiber.Handler {
	return func(c fiber.Ctx) error {
		userRole, ok := c.Locals("role").(string)
		if !ok {
			return c.Status(fiber.StatusUnauthorized).JSON(fiber.Map{
				"error": "Authentication required",
			})
		}

		for _, role := range allowedRoles {
			if userRole == role {
				return c.Next()
			}
		}

		return c.Status(fiber.StatusForbidden).JSON(fiber.Map{
			"error": "Insufficient permissions",
		})
	}
}
