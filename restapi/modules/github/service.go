// Package github provides GitHub integration services for the REST API.
package github

import (
	"crypto/x509"
	"encoding/json"
	"encoding/pem"
	"fmt"
	"net/http"
	"os"
	"time"

	"github.com/golang-jwt/jwt/v5"
)

const githubAPI = "https://api.github.com"

// GetInstallationToken generates a short-lived installation access token
func GetInstallationToken(installationID string) (string, error) {
	appID := os.Getenv("GITHUB_APP_ID")
	privateKeyPEM := os.Getenv("GITHUB_PRIVATE_KEY")

	if appID == "" || privateKeyPEM == "" {
		return "", fmt.Errorf("GITHUB_APP_ID or GITHUB_PRIVATE_KEY not configured")
	}

	block, _ := pem.Decode([]byte(privateKeyPEM))
	if block == nil {
		return "", fmt.Errorf("failed to parse GITHUB_PRIVATE_KEY PEM")
	}
	privateKey, err := x509.ParsePKCS1PrivateKey(block.Bytes)
	if err != nil {
		return "", fmt.Errorf("failed to parse private key: %w", err)
	}

	claims := jwt.RegisteredClaims{
		Issuer:    appID,
		IssuedAt:  jwt.NewNumericDate(time.Now().Add(-60 * time.Second)),
		ExpiresAt: jwt.NewNumericDate(time.Now().Add(10 * time.Minute)),
	}

	token := jwt.NewWithClaims(jwt.SigningMethodRS256, claims)
	signedJWT, err := token.SignedString(privateKey)
	if err != nil {
		return "", fmt.Errorf("failed to sign JWT: %w", err)
	}

	client := &http.Client{}
	url := fmt.Sprintf("%s/app/installations/%s/access_tokens", githubAPI, installationID)
	req, _ := http.NewRequest("POST", url, nil)
	req.Header.Set("Authorization", "Bearer "+signedJWT)
	req.Header.Set("Accept", "application/vnd.github.v3+json")

	resp, err := client.Do(req)
	if err != nil {
		return "", fmt.Errorf("failed to request installation token: %w", err)
	}
	defer resp.Body.Close()

	if resp.StatusCode != 201 {
		return "", fmt.Errorf("github api error: %s", resp.Status)
	}

	var result struct {
		Token string `json:"token"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		return "", fmt.Errorf("failed to decode token response: %w", err)
	}

	return result.Token, nil
}

// FetchRepos retrieves repositories accessible to the installation
func FetchRepos(installationToken string) ([]GitHubRepo, error) {
	client := &http.Client{}

	url := fmt.Sprintf("%s/installation/repositories?per_page=100", githubAPI)

	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Set("Authorization", "Bearer "+installationToken)
	req.Header.Set("Accept", "application/vnd.github.v3+json")

	resp, err := client.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("failed to fetch repos: %s", resp.Status)
	}

	var result struct {
		Repositories []GitHubRepo `json:"repositories"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		return nil, err
	}
	return result.Repositories, nil
}

// FetchReleases retrieves releases for a GitHub repository.
func FetchReleases(token, owner, repo string) ([]GitHubRelease, error) {
	client := &http.Client{}
	url := fmt.Sprintf("%s/repos/%s/%s/releases", githubAPI, owner, repo)
	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Set("Authorization", "Bearer "+token)
	req.Header.Set("Accept", "application/vnd.github.v3+json")

	resp, err := client.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("failed to fetch releases: %s", resp.Status)
	}

	var releases []GitHubRelease
	if err := json.NewDecoder(resp.Body).Decode(&releases); err != nil {
		return nil, err
	}
	return releases, nil
}

// FetchWorkflowRuns retrieves workflow runs for a GitHub repository.
func FetchWorkflowRuns(token, owner, repo string) ([]GitHubWorkflowRun, error) {
	client := &http.Client{}
	url := fmt.Sprintf("%s/repos/%s/%s/actions/runs?status=success&per_page=10", githubAPI, owner, repo)
	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Set("Authorization", "Bearer "+token)
	req.Header.Set("Accept", "application/vnd.github.v3+json")

	resp, err := client.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("failed to fetch workflow runs: %s", resp.Status)
	}

	var result struct {
		WorkflowRuns []GitHubWorkflowRun `json:"workflow_runs"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		return nil, err
	}
	return result.WorkflowRuns, nil
}
