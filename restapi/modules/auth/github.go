// Package auth provides authentication and authorization handlers for the Ortelius backend.
package auth

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
	"net/url"
	"os"
	"strings"
	"time"

	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/util"
)

// githubOAuthNonceCookie is the short-lived cookie that anchors the CSRF nonce
// to the browser session that *started* the GitHub flow. GitHubCallback checks
// the state param against this cookie before exchanging any code.
const githubOAuthNonceCookie = "github_oauth_nonce"

// isSafeReturnPath restricts return_to to a same-site relative path, blocking
// open-redirect style values like "https://evil.com" or "//evil.com".
func isSafeReturnPath(p string) bool {
	if p == "" {
		return false
	}
	if !strings.HasPrefix(p, "/") || strings.HasPrefix(p, "//") {
		return false
	}
	if strings.Contains(p, "://") {
		return false
	}
	return true
}

// GitHubLogin initiates the GitHub App Installation flow.
// UPDATED: Changed *fiber.Ctx to fiber.Ctx and used c.Redirect().To() for Fiber v3
// UPDATED: Generates a random CSRF nonce, stores it in a short-lived cookie on
// this browser, and carries it (plus an optional return_to) through GitHub via
// the `state` param. GitHubCallback must see this same nonce come back before
// it will exchange a code — that binding is what actually prevents login CSRF,
// since the callback URL itself is fixed and can't otherwise tell "this code
// belongs to the session that started the flow" from "an attacker's code,
// replayed against a victim's cookie."
func GitHubLogin(c fiber.Ctx) error {
	appName := os.Getenv("GITHUB_APP_NAME")
	if appName == "" {
		return c.Status(fiber.StatusInternalServerError).SendString("GITHUB_APP_NAME not configured")
	}

	nonce, err := GenerateSecureToken(16)
	if err != nil {
		return c.Status(fiber.StatusInternalServerError).SendString("Failed to start GitHub flow")
	}

	c.Cookie(&fiber.Cookie{
		Name:     githubOAuthNonceCookie,
		Value:    nonce,
		HTTPOnly: true,
		Secure:   false, // TODO: true once served over HTTPS in all environments
		SameSite: "Lax",
		MaxAge:   600, // 10 minutes - single flow, then expires
		Path:     "/",
	})

	returnTo := c.Query("return_to")
	if !isSafeReturnPath(returnTo) {
		returnTo = "/profile"
	}

	state := nonce + "|" + returnTo
	installURL := fmt.Sprintf("https://github.com/apps/%s/installations/new?state=%s",
		appName, url.QueryEscape(state))
	return c.Redirect().To(installURL)
}

// GitHubCallback handles the callback from GitHub
// UPDATED: Changed *fiber.Ctx to fiber.Ctx and used c.Redirect().To() for Fiber v3
func GitHubCallback(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		code := c.Query("code")
		installationID := c.Query("installation_id") // Captured from App Install flow
		stateParam := c.Query("state")

		if code == "" {
			return c.Status(fiber.StatusBadRequest).SendString("Missing code. Ensure the App requests OAuth during installation.")
		}

		// CSRF check: the nonce in `state` must match the cookie set by
		// GitHubLogin on *this* browser. Without this, anyone who captures or
		// generates a valid code+state pair (e.g. by running the flow against
		// their own GitHub account) could send the resulting callback URL to a
		// victim and have it bind the attacker's GitHub identity to the
		// victim's session, since the callback only trusts the auth_token
		// cookie to decide whose account to update.
		cookieNonce := c.Cookies(githubOAuthNonceCookie)
		statePart := strings.SplitN(stateParam, "|", 2)
		returnTo := "/profile"
		if len(statePart) == 2 {
			returnTo = statePart[1]
		}
		if !isSafeReturnPath(returnTo) {
			returnTo = "/profile"
		}

		// Clear the nonce cookie immediately so it can't be replayed even if
		// the state check below passes.
		c.Cookie(&fiber.Cookie{
			Name:     githubOAuthNonceCookie,
			Value:    "",
			Expires:  time.Now().Add(-1 * time.Hour),
			MaxAge:   -1,
			HTTPOnly: true,
			Secure:   false,
			SameSite: "Lax",
			Path:     "/",
		})

		if cookieNonce == "" || len(statePart) != 2 || statePart[0] != cookieNonce {
			return c.Redirect().To("/?error=invalid_oauth_state")
		}

		clientID := os.Getenv("GITHUB_CLIENT_ID")
		clientSecret := os.Getenv("GITHUB_CLIENT_SECRET")

		if clientID == "" || clientSecret == "" {
			return c.Status(fiber.StatusInternalServerError).SendString("Server misconfiguration")
		}

		// Exchange Code for User Token
		reqBody, _ := json.Marshal(map[string]string{
			"client_id":     clientID,
			"client_secret": clientSecret,
			"code":          code,
		})

		req, _ := http.NewRequest("POST", "https://github.com/login/oauth/access_token", bytes.NewBuffer(reqBody))
		req.Header.Set("Content-Type", "application/json")
		req.Header.Set("Accept", "application/json")

		client := &http.Client{Timeout: 10 * time.Second}
		resp, err := client.Do(req)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).SendString("Failed to exchange token")
		}
		defer resp.Body.Close()

		var result map[string]interface{}
		if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
			return c.Status(fiber.StatusInternalServerError).SendString("Failed to parse token response")
		}

		if errMsg, isError := result["error"]; isError {
			return c.Status(fiber.StatusUnauthorized).SendString(fmt.Sprintf("GitHub Error: %v", errMsg))
		}

		accessToken, ok := result["access_token"].(string)
		if !ok || accessToken == "" {
			return c.Status(fiber.StatusUnauthorized).SendString("Failed to get access token")
		}

		// Get Current User from Cookie
		token := c.Cookies("auth_token")
		if token == "" {
			return c.Redirect().To("/?error=session_expired")
		}

		claims, err := ValidateJWT(token)
		if err != nil {
			return c.Redirect().To("/?error=session_expired")
		}

		username := claims.Username

		ctx := c.Context()
		user, err := getUserByUsername(ctx, db, username)
		if err != nil {
			return c.Redirect().To("/?error=user_not_found")
		}

		// Store Token and Installation ID
		// Encrypted at rest, same as org-level PATs (see org.UpdateOrgCredentials) —
		// this is a live credential with repo access, not safe to keep in plaintext.
		encryptedToken, err := util.EncryptToken(accessToken)
		if err != nil {
			fmt.Println("[DEBUG] GitHubCallback: util.EncryptToken failed:", err)
			return c.Status(fiber.StatusInternalServerError).SendString("Failed to secure GitHub token")
		}
		user.GitHubToken = encryptedToken

		if installationID != "" {
			user.GitHubInstallationID = installationID
		}

		user.UpdatedAt = time.Now()

		if err := updateUser(ctx, db, user); err != nil {
			return c.Status(fiber.StatusInternalServerError).SendString("Failed to save GitHub token")
		}

		// Redirect to Frontend, honoring the return_to carried through `state`
		frontendURL := os.Getenv("BASE_URL")
		if frontendURL == "" {
			frontendURL = "http://localhost:4000"
		}
		sep := "?"
		if strings.Contains(returnTo, "?") {
			sep = "&"
		}
		return c.Redirect().To(fmt.Sprintf("%s%s%sgithub_connected=true", frontendURL, returnTo, sep))
	}
}
