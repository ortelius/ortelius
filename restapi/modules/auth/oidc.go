// Package auth - oidc.go provides a provider-agnostic OIDC login/callback pair.
//
// Any spec-compliant OIDC issuer (Google, Authentik, Okta, ...) plugs in here
// via config only - no new handler code is needed per provider. GitHub is
// NOT handled here because GitHub has no OIDC discovery document; see
// github_oauth.go for its OAuth2-only flow, which intentionally mirrors this
// file's structure (same nonce/state/cookie pattern) so the two stay easy to
// reason about side by side.
package auth

import (
	"context"
	"fmt"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/coreos/go-oidc/v3/oidc"
	"github.com/gofiber/fiber/v3"
	"golang.org/x/oauth2"

	"github.com/ortelius/ortelius/v12/database"
)

// OIDCProviderConfig describes one OIDC issuer Ortelius can log users in
// against. Add a new entry in registeredOIDCProviders (or load from env/DB)
// to support another provider - Authentik and Okta both fit this shape
// exactly, since both expose standard /.well-known/openid-configuration docs.
type OIDCProviderConfig struct {
	Name          string // url-safe identifier, e.g. "google", "authentik", "okta" - used in /auth/:name/login
	IssuerURL     string // e.g. "https://accounts.google.com"
	ClientID      string
	ClientSecret  string
	RedirectURL   string
	Scopes        []string // defaults to {"openid", "email", "profile"} if empty
	AllowedDomain string   // optional - if set, only ID tokens whose "hd" (or domain-derived-from-email) claim matches are accepted
}

// oidcRuntimeProvider bundles a config with its initialized oidc.Provider and
// oauth2.Config so we don't re-run discovery on every request.
type oidcRuntimeProvider struct {
	cfg      OIDCProviderConfig
	provider *oidc.Provider
	verifier *oidc.IDTokenVerifier
	oauth2   *oauth2.Config
}

var (
	oidcProvidersMu sync.RWMutex
	oidcProviders   = map[string]*oidcRuntimeProvider{}
)

// oidcNonceCookiePrefix - one cookie per provider so concurrent flows
// (e.g. a user trying Google then Authentik in two tabs) don't collide.
const oidcNonceCookiePrefix = "oidc_nonce_"

// LoadOIDCProvidersFromEnv registers the OIDC providers configured via
// environment variables. Call this once at startup (same place
// auth.BootstrapAdmin / auth.EnsureDefaultRoles are called from router.go).
//
// Env vars per provider, prefixed by its uppercased name, e.g. for "google":
//
//	GOOGLE_OIDC_ISSUER_URL   (defaults to https://accounts.google.com)
//	GOOGLE_OIDC_CLIENT_ID
//	GOOGLE_OIDC_CLIENT_SECRET
//	GOOGLE_OIDC_REDIRECT_URL
//	GOOGLE_OIDC_ALLOWED_DOMAIN   (optional - restrict to a Workspace domain)
//
// Extending to Authentik/Okta later is just adding their name to this slice
// and setting the matching env vars - no code change.
func LoadOIDCProvidersFromEnv(ctx context.Context) {
	for _, name := range []string{"google", "authentik", "okta"} {
		upper := strings.ToUpper(name)
		clientID := os.Getenv(upper + "_OIDC_CLIENT_ID")
		if clientID == "" {
			continue // provider not configured, skip silently
		}

		issuer := os.Getenv(upper + "_OIDC_ISSUER_URL")
		if issuer == "" && name == "google" {
			issuer = "https://accounts.google.com"
		}
		if issuer == "" {
			fmt.Printf("WARNING: %s_OIDC_CLIENT_ID set but %s_OIDC_ISSUER_URL missing - skipping provider %q\n", upper, upper, name)
			continue
		}

		cfg := OIDCProviderConfig{
			Name:          name,
			IssuerURL:     issuer,
			ClientID:      clientID,
			ClientSecret:  os.Getenv(upper + "_OIDC_CLIENT_SECRET"),
			RedirectURL:   os.Getenv(upper + "_OIDC_REDIRECT_URL"),
			AllowedDomain: os.Getenv(upper + "_OIDC_ALLOWED_DOMAIN"),
		}

		if err := RegisterOIDCProvider(ctx, cfg); err != nil {
			fmt.Printf("WARNING: failed to register OIDC provider %q: %v\n", name, err)
			continue
		}
		fmt.Printf("Registered OIDC provider: %s\n", name)
	}
}

// RegisterOIDCProvider runs discovery against cfg.IssuerURL and stores the
// resulting provider under cfg.Name for use by OIDCLogin/OIDCCallback.
func RegisterOIDCProvider(ctx context.Context, cfg OIDCProviderConfig) error {
	provider, err := oidc.NewProvider(ctx, cfg.IssuerURL)
	if err != nil {
		return fmt.Errorf("oidc discovery failed for %s (%s): %w", cfg.Name, cfg.IssuerURL, err)
	}

	scopes := cfg.Scopes
	if len(scopes) == 0 {
		scopes = []string{oidc.ScopeOpenID, "email", "profile"}
	}

	rp := &oidcRuntimeProvider{
		cfg:      cfg,
		provider: provider,
		verifier: provider.Verifier(&oidc.Config{ClientID: cfg.ClientID}),
		oauth2: &oauth2.Config{
			ClientID:     cfg.ClientID,
			ClientSecret: cfg.ClientSecret,
			RedirectURL:  cfg.RedirectURL,
			Endpoint:     provider.Endpoint(),
			Scopes:       scopes,
		},
	}

	oidcProvidersMu.Lock()
	oidcProviders[cfg.Name] = rp
	oidcProvidersMu.Unlock()
	return nil
}

func getOIDCProvider(name string) (*oidcRuntimeProvider, bool) {
	oidcProvidersMu.RLock()
	defer oidcProvidersMu.RUnlock()
	rp, ok := oidcProviders[name]
	return rp, ok
}

// RegisteredOIDCProviders returns the names of all currently configured OIDC
// providers (e.g. ["google"], or ["google", "authentik"] once more are
// added). Used by AuthStatus to tell the frontend which SSO buttons to show.
func RegisteredOIDCProviders() []string {
	oidcProvidersMu.RLock()
	defer oidcProvidersMu.RUnlock()
	names := make([]string, 0, len(oidcProviders))
	for name := range oidcProviders {
		names = append(names, name)
	}
	return names
}

// OIDCLogin redirects the browser to the named provider's authorize endpoint.
// Mounted as GET /api/v1/auth/:provider/login - :provider must be a name
// previously passed to RegisterOIDCProvider (e.g. "google").
func OIDCLogin(c fiber.Ctx) error {
	name := c.Params("provider")
	rp, ok := getOIDCProvider(name)
	if !ok {
		return c.Status(fiber.StatusNotFound).SendString(fmt.Sprintf("Unknown or unconfigured OIDC provider: %s", name))
	}

	nonce, err := GenerateSecureToken(16)
	if err != nil {
		return c.Status(fiber.StatusInternalServerError).SendString("Failed to start login")
	}

	c.Cookie(&fiber.Cookie{
		Name:     oidcNonceCookiePrefix + name,
		Value:    nonce,
		HTTPOnly: true,
		Secure:   false, // TODO: true once served over HTTPS everywhere
		SameSite: "Lax",
		MaxAge:   600,
		Path:     "/",
	})

	returnTo := c.Query("return_to")
	if !isSafeReturnPath(returnTo) {
		returnTo = "/"
	}
	state := nonce + "|" + returnTo

	authCodeOpts := []oauth2.AuthCodeOption{}
	// hd restricts Google's account chooser to a Workspace domain. It is
	// advisory only (the real enforcement is the ID-token claim check in
	// OIDCCallback below) but it gives users the right UX - they don't see
	// personal Gmail accounts in the picker at all.
	if name == "google" && rp.cfg.AllowedDomain != "" {
		authCodeOpts = append(authCodeOpts, oauth2.SetAuthURLParam("hd", rp.cfg.AllowedDomain))
	}

	return c.Redirect().To(rp.oauth2.AuthCodeURL(state, authCodeOpts...))
}

// OIDCCallback handles GET /api/v1/auth/:provider/callback for every
// registered OIDC provider. It validates CSRF state, exchanges the code,
// verifies the ID token's signature/issuer/audience (via go-oidc), enforces
// AllowedDomain if configured, then provisions/loads the local user exactly
// like Login() does for password auth - same JWT, same SetAuthCookie.
func OIDCCallback(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		name := c.Params("provider")
		rp, ok := getOIDCProvider(name)
		if !ok {
			return c.Status(fiber.StatusNotFound).SendString(fmt.Sprintf("Unknown or unconfigured OIDC provider: %s", name))
		}

		cookieName := oidcNonceCookiePrefix + name
		cookieNonce := c.Cookies(cookieName)
		c.Cookie(&fiber.Cookie{
			Name: cookieName, Value: "", Expires: time.Now().Add(-time.Hour), MaxAge: -1,
			HTTPOnly: true, Secure: false, SameSite: "Lax", Path: "/",
		})

		stateParam := c.Query("state")
		statePart := strings.SplitN(stateParam, "|", 2)
		returnTo := "/"
		if len(statePart) == 2 {
			returnTo = statePart[1]
		}
		if !isSafeReturnPath(returnTo) {
			returnTo = "/"
		}
		if cookieNonce == "" || len(statePart) != 2 || statePart[0] != cookieNonce {
			return c.Redirect().To("/?error=invalid_oauth_state")
		}

		if errParam := c.Query("error"); errParam != "" {
			return c.Redirect().To(fmt.Sprintf("/?error=%s", errParam))
		}

		code := c.Query("code")
		if code == "" {
			return c.Status(fiber.StatusBadRequest).SendString("Missing code")
		}

		ctx := c.Context()
		token, err := rp.oauth2.Exchange(ctx, code)
		if err != nil {
			return c.Status(fiber.StatusUnauthorized).SendString("Token exchange failed: " + err.Error())
		}

		rawIDToken, ok := token.Extra("id_token").(string)
		if !ok || rawIDToken == "" {
			return c.Status(fiber.StatusUnauthorized).SendString("Provider did not return an id_token")
		}

		idToken, err := rp.verifier.Verify(ctx, rawIDToken)
		if err != nil {
			return c.Status(fiber.StatusUnauthorized).SendString("ID token verification failed: " + err.Error())
		}

		var claims struct {
			Subject       string `json:"sub"`
			Email         string `json:"email"`
			EmailVerified bool   `json:"email_verified"`
			Name          string `json:"name"`
			HostedDomain  string `json:"hd"` // Google-specific Workspace domain claim
		}
		if err := idToken.Claims(&claims); err != nil {
			return c.Status(fiber.StatusInternalServerError).SendString("Failed to parse claims")
		}

		if claims.Email == "" || !claims.EmailVerified {
			return c.Redirect().To("/?error=email_not_verified")
		}

		// Domain enforcement - the actual access-control gate. Do NOT rely on
		// the advisory `hd` authorize-URL param alone; a user can still reach
		// the callback having authenticated with an out-of-domain account.
		if rp.cfg.AllowedDomain != "" {
			domain := claims.HostedDomain
			if domain == "" {
				if at := strings.LastIndex(claims.Email, "@"); at != -1 {
					domain = claims.Email[at+1:]
				}
			}
			if !strings.EqualFold(domain, rp.cfg.AllowedDomain) {
				return c.Redirect().To("/?error=domain_not_allowed")
			}
		}

		user, err := ResolveSSOUser(ctx, db, name, claims.Subject, claims.Email, claims.EmailVerified, claims.Email)
		if err != nil {
			fmt.Printf("WARNING: ResolveSSOUser failed for provider=%s sub=%s: %v\n", name, claims.Subject, err)
			return c.Redirect().To("/?error=account_resolution_failed")
		}

		if !user.IsActive {
			return c.Redirect().To("/?error=account_inactive")
		}

		jwtToken, err := GenerateJWT(user.Username)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).SendString("Failed to generate session")
		}
		SetAuthCookie(c, jwtToken)

		frontendURL := os.Getenv("BASE_URL")
		if frontendURL == "" {
			frontendURL = "http://localhost:4000"
		}
		sep := "?"
		if strings.Contains(returnTo, "?") {
			sep = "&"
		}
		return c.Redirect().To(fmt.Sprintf("%s%s%slogin=success", frontendURL, returnTo, sep))
	}
}
