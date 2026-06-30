// Package auth - github_oauth.go implements GitHub *sign-in* via OAuth2.
//
// This is distinct from github.go's GitHubLogin/GitHubCallback, which link a
// GitHub App installation (repo access) to an ALREADY authenticated user.
// This file lets someone log in to Ortelius for the first time using their
// GitHub identity. GitHub has no /.well-known/openid-configuration endpoint,
// so it can't use oidc.go's generic path - but the handler shape (nonce
// cookie, state, exchange, provision-or-load user, SetAuthCookie) is kept
// identical on purpose.
package auth

import (
	"context"
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/gofiber/fiber/v3"
	"golang.org/x/oauth2"
	githuboauth "golang.org/x/oauth2/github"

	"github.com/ortelius/ortelius/v12/database"
)

const githubSigninNonceCookie = "github_signin_nonce"

var githubOAuthConfig *oauth2.Config

// GitHubSigninEnabled reports whether GITHUB_OAUTH_CLIENT_ID was set at
// startup. Used by AuthStatus to tell the frontend whether to show the
// "Sign in with GitHub" button.
func GitHubSigninEnabled() bool {
	return githubOAuthConfig != nil
}

// LoadGitHubOAuthFromEnv configures the GitHub sign-in OAuth2 app. Call once
// at startup alongside LoadOIDCProvidersFromEnv. Uses a SEPARATE client
// id/secret from GITHUB_APP_NAME's app (env: GITHUB_OAUTH_CLIENT_ID /
// GITHUB_OAUTH_CLIENT_SECRET / GITHUB_OAUTH_REDIRECT_URL) since a GitHub
// "OAuth App" (for login) and a "GitHub App" (for repo installs, used by
// github.go) are different registrations in GitHub's UI.
func LoadGitHubOAuthFromEnv() {
	clientID := os.Getenv("GITHUB_OAUTH_CLIENT_ID")
	if clientID == "" {
		return // not configured, skip silently - same convention as LoadOIDCProvidersFromEnv
	}

	githubOAuthConfig = &oauth2.Config{
		ClientID:     clientID,
		ClientSecret: os.Getenv("GITHUB_OAUTH_CLIENT_SECRET"),
		RedirectURL:  os.Getenv("GITHUB_OAUTH_REDIRECT_URL"),
		Endpoint:     githuboauth.Endpoint,
		Scopes:       []string{"read:user", "user:email"},
	}
	fmt.Println("Registered GitHub sign-in OAuth2 app")
}

// GitHubSigninLogin redirects to GitHub's OAuth2 authorize endpoint.
// Mounted as GET /api/v1/auth/github-signin/login (kept distinct from
// /auth/github/login, which is the App-install flow in github.go).
func GitHubSigninLogin(c fiber.Ctx) error {
	if githubOAuthConfig == nil {
		return c.Status(fiber.StatusServiceUnavailable).SendString("GitHub sign-in not configured")
	}

	nonce, err := GenerateSecureToken(16)
	if err != nil {
		return c.Status(fiber.StatusInternalServerError).SendString("Failed to start login")
	}

	c.Cookie(&fiber.Cookie{
		Name:     githubSigninNonceCookie,
		Value:    nonce,
		HTTPOnly: true,
		Secure:   false,
		SameSite: "Lax",
		MaxAge:   600,
		Path:     "/",
	})

	returnTo := c.Query("return_to")
	if !isSafeReturnPath(returnTo) {
		returnTo = "/"
	}
	state := nonce + "|" + returnTo

	return c.Redirect().To(githubOAuthConfig.AuthCodeURL(state))
}

type githubUserResp struct {
	ID    int64  `json:"id"`
	Login string `json:"login"`
	Name  string `json:"name"`
	Email string `json:"email"` // often null even on a verified account - see githubEmailResp fallback below
}

type githubEmailResp struct {
	Email    string `json:"email"`
	Primary  bool   `json:"primary"`
	Verified bool   `json:"verified"`
}

func fetchGitHubIdentity(ctx context.Context, token *oauth2.Token) (*githubUserResp, string, error) {
	client := githubOAuthConfig.Client(ctx, token)

	resp, err := client.Get("https://api.github.com/user")
	if err != nil {
		return nil, "", err
	}
	defer resp.Body.Close()
	if resp.StatusCode != http.StatusOK {
		return nil, "", fmt.Errorf("github /user returned %d", resp.StatusCode)
	}
	var user githubUserResp
	if err := json.NewDecoder(resp.Body).Decode(&user); err != nil {
		return nil, "", err
	}

	if user.Email != "" {
		return &user, user.Email, nil
	}

	// Public profile email can be empty/private even with user:email scope -
	// fall back to the dedicated emails endpoint and pick the verified primary.
	emailResp, err := client.Get("https://api.github.com/user/emails")
	if err != nil {
		return &user, "", err
	}
	defer emailResp.Body.Close()
	var emails []githubEmailResp
	if err := json.NewDecoder(emailResp.Body).Decode(&emails); err != nil {
		return &user, "", err
	}
	for _, e := range emails {
		if e.Primary && e.Verified {
			return &user, e.Email, nil
		}
	}
	return &user, "", fmt.Errorf("no verified primary email on GitHub account")
}

// GitHubSigninCallback exchanges the code, fetches the GitHub identity, and
// provisions/loads the local user - same JWT/session mechanics as Login()
// and OIDCCallback.
func GitHubSigninCallback(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		if githubOAuthConfig == nil {
			return c.Status(fiber.StatusServiceUnavailable).SendString("GitHub sign-in not configured")
		}

		cookieNonce := c.Cookies(githubSigninNonceCookie)
		c.Cookie(&fiber.Cookie{
			Name: githubSigninNonceCookie, Value: "", Expires: time.Now().Add(-time.Hour), MaxAge: -1,
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

		code := c.Query("code")
		if code == "" {
			return c.Status(fiber.StatusBadRequest).SendString("Missing code")
		}

		ctx := c.Context()
		token, err := githubOAuthConfig.Exchange(ctx, code)
		if err != nil {
			return c.Status(fiber.StatusUnauthorized).SendString("Token exchange failed: " + err.Error())
		}

		ghUser, email, err := fetchGitHubIdentity(ctx, token)
		if err != nil || email == "" {
			return c.Redirect().To("/?error=github_email_unavailable")
		}

		externalID := strconv.FormatInt(ghUser.ID, 10)
		user, err := ResolveSSOUser(ctx, db, "github", externalID, email, true, ghUser.Login)
		if err != nil {
			fmt.Printf("WARNING: ResolveSSOUser failed for provider=github id=%s: %v\n", externalID, err)
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
