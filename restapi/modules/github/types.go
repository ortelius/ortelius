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

// RepoMapping carries the optional per-repo mapping entered by the user at
// onboarding time. Both fields are pointers so an absent key (repo untouched
// by the user) is distinguishable from an explicitly empty value.
type RepoMapping struct {
	// ArtifactNamespace maps the repo to the Docker/artifact namespace it
	// publishes under (e.g. "deployhub"). Combined with the repo's short
	// name to populate ProjectRelease.DockerRepo for vulnerability matching.
	ArtifactNamespace *string `json:"artifactNamespace,omitempty"`
	// GitopsEndpoint maps the repo to the runtime endpoint it deploys to,
	// formatted as "<endpoint name>/<namespace>" (e.g.
	// "us-central-1_deployhub/deployhub"). Only meaningful for repos that
	// hold gitops manifests/charts.
	GitopsEndpoint *string `json:"gitopsEndpoint,omitempty"`
}

// OnboardRequest represents a request to onboard GitHub repositories.
type OnboardRequest struct {
	Repos []string `json:"repos"` // List of full_names (e.g. "owner/repo")
	// RepoMappings is keyed by full_name (e.g. "owner/repo"). A repo with no
	// entry here is onboarded with no artifact/gitops mapping, same as before.
	RepoMappings map[string]RepoMapping `json:"repoMappings,omitempty"`
}
