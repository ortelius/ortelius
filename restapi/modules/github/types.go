// Package github provides GitHub integration types for the REST API.
package github

import "time"

// GitHubRepo represents a GitHub repository.
//
//revive:disable-next-line:exported
type GitHubRepo struct {
	ID          int    `json:"id"`
	Name        string `json:"name"`
	FullName    string `json:"full_name"`
	Description string `json:"description"`
	HTMLURL     string `json:"html_url"`
	Private     bool   `json:"private"`
}

// GitHubRelease represents a GitHub release.
//
//revive:disable-next-line:exported
type GitHubRelease struct {
	Name        string    `json:"name"`
	TagName     string    `json:"tag_name"`
	PublishedAt time.Time `json:"published_at"`
	Body        string    `json:"body"`
}

// GitHubWorkflowRun represents a GitHub Actions workflow run.
//
//revive:disable-next-line:exported
type GitHubWorkflowRun struct {
	ID         int       `json:"id"`
	Name       string    `json:"name"`
	Status     string    `json:"status"`
	Conclusion string    `json:"conclusion"`
	UpdatedAt  time.Time `json:"updated_at"`
	HeadBranch string    `json:"head_branch"`
	HeadSha    string    `json:"head_sha"`
}

// OnboardRequest represents a request to onboard GitHub repositories.
type OnboardRequest struct {
	Repos []string `json:"repos"` // List of full_names (e.g. "owner/repo")
}
