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
)

// GitHubLogin initiates the GitHub App Installation flow.
// Accepts an optional ?return_to= query param (a relative app path) which is
// threaded through GitHub's installation flow via the `state` parameter and
// used by GitHubCallback to send the user back to where they started
// (e.g. the Welcome page) instead of always landing on /profile.
func GitHubLogin(c fiber.Ctx) error {
	appName := os.Getenv("GITHUB_APP_NAME")
	if appName == "" {
		return c.Status(fiber.StatusInternalServerError).SendString("GITHUB_APP_NAME not configured")
	}

	installURL := fmt.Sprintf("https://github.com/apps/%s/installations/new", appName)

	if returnTo := c.Query("return_to"); returnTo != "" && isSafeReturnPath(returnTo) {
		installURL += "?state=" + url.QueryEscape(returnTo)
	}

	return c.Redirect().To(installURL)
}

// isSafeReturnPath restricts redirect targets to same-app relative paths,
// preventing the state param from being used as an open redirect.
func isSafeReturnPath(path string) bool {
	return strings.HasPrefix(path, "/") && !strings.HasPrefix(path, "//") && !strings.Contains(path, "://")
}

// GitHubCallback handles the callback from GitHub
func GitHubCallback(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		code := c.Query("code")
		installationID := c.Query("installation_id") // Captured from App Install flow
		state := c.Query("state")                    // Optional return path set by GitHubLogin
		setupAction := c.Query("setup_action")       // Indicates an update redirect!

		// 1. Authenticate the User from Cookie FIRST
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

		// 2. Handle the "Update" Redirect Flow
		// If GitHub is just redirecting after an update, bypass OAuth exchange,
		// save the installation ID if it changed, and send the user straight back.
		if setupAction == "update" {
			if installationID != "" && user.GitHubInstallationID != installationID {
				user.GitHubInstallationID = installationID
				user.UpdatedAt = time.Now()
				_ = updateUser(ctx, db, user)
			}

			frontendURL := os.Getenv("BASE_URL")
			if frontendURL == "" {
				frontendURL = "http://localhost:4000"
			}
			returnPath := "/profile"
			if state != "" && isSafeReturnPath(state) {
				returnPath = state
			}
			return c.Redirect().To(fmt.Sprintf("%s%s?github_updated=true", frontendURL, returnPath))
		}

		// 3. Handle the Standard OAuth Flow
		if code == "" {
			return c.Status(fiber.StatusBadRequest).SendString("Missing code. Ensure the App requests OAuth during installation.")
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

		// FIXED: If the app was already installed, GitHub doesn't send the installation_id in the URL.
		// We must fetch the user's accessible installations directly using their new token.
		if installationID == "" {
			reqInst, _ := http.NewRequest("GET", "https://api.github.com/user/installations", nil)
			reqInst.Header.Set("Authorization", "Bearer "+accessToken)
			reqInst.Header.Set("Accept", "application/vnd.github.v3+json")

			respInst, err := client.Do(reqInst)
			if err == nil {
				defer respInst.Body.Close()
				var instResult struct {
					Installations []struct {
						ID int `json:"id"`
					} `json:"installations"`
				}
				if json.NewDecoder(respInst.Body).Decode(&instResult) == nil {
					if len(instResult.Installations) > 0 {
						installationID = fmt.Sprintf("%d", instResult.Installations[0].ID)
					}
				}
			}
		}

		// Store Token and Installation ID
		user.GitHubToken = accessToken
		if installationID != "" {
			user.GitHubInstallationID = installationID
		}
		user.UpdatedAt = time.Now()

		if err := updateUser(ctx, db, user); err != nil {
			return c.Status(fiber.StatusInternalServerError).SendString("Failed to save GitHub token")
		}

		// Redirect to Frontend
		frontendURL := os.Getenv("BASE_URL")
		if frontendURL == "" {
			frontendURL = "http://localhost:4000"
		}
		returnPath := "/profile"
		if state != "" && isSafeReturnPath(state) {
			returnPath = state
		}

		return c.Redirect().To(fmt.Sprintf("%s%s?github_connected=true", frontendURL, returnPath))
	}
}
