// Package auth - status.go exposes a small public endpoint reporting which
// SSO providers are currently configured, so the frontend can conditionally
// render "Sign in with X" buttons instead of showing one that 503s.
package auth

import "github.com/gofiber/fiber/v3"

// AuthStatusResponse is intentionally minimal - booleans only, never secrets
// or client IDs (those aren't needed by the frontend; it only redirects to
// a backend route, it never talks to the provider directly).
type AuthStatusResponse struct {
	Google bool `json:"google"`
	GitHub bool `json:"github"`
	// Authentik/Okta/GitLab slot in here the same way once registered -
	// just check RegisteredOIDCProviders() for their name below.
}

// AuthStatus handles GET /api/v1/auth/status - public, no auth required,
// since the whole point is letting a logged-out visitor know which login
// buttons to show.
func AuthStatus(c fiber.Ctx) error {
	googleEnabled := false
	for _, name := range RegisteredOIDCProviders() {
		if name == "google" {
			googleEnabled = true
			break
		}
	}

	return c.JSON(AuthStatusResponse{
		Google: googleEnabled,
		GitHub: GitHubSigninEnabled(),
	})
}
