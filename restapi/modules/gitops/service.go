// Package gitops provides functionality to manage RBAC configuration via a Git repository.
package gitops

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"time"

	"github.com/go-git/go-git/v5"
	"github.com/go-git/go-git/v5/plumbing/object"
	"github.com/go-git/go-git/v5/plumbing/transport/http"
	"gopkg.in/yaml.v2"
)

// Config represents the local structure for RBAC YAML parsing.
type Config struct {
	Orgs  []Org  `yaml:"orgs,omitempty"`
	Users []User `yaml:"users"`
	Roles []Role `yaml:"roles,omitempty"`
}

// Org represents an organization structure in the YAML config.
type Org struct {
	Name         string            `yaml:"name"`
	DisplayName  string            `yaml:"display_name,omitempty"`
	Description  string            `yaml:"description,omitempty"`
	Metadata     map[string]string `yaml:"metadata,omitempty"`
	Members      []Member          `yaml:"members,omitempty"`
	TrackedRepos []TrackedRepo     `yaml:"tracked_repos,omitempty"`
}

// TrackedRepo represents a repository being tracked within an org.
type TrackedRepo struct {
	Provider string `yaml:"provider"` // "github" | "gitlab"
	Owner    string `yaml:"owner"`    // github org/user or gitlab group
	Name     string `yaml:"name"`
	Private  bool   `yaml:"private,omitempty"`
}

// Member represents a user member of an organization.
type Member struct {
	Username string `yaml:"username"`
	Role     string `yaml:"role"`
}

// User represents a user identity structure.
// Deliberately has NO email field - this struct is marshaled into rbac.yaml,
// which is committed and pushed to a git repository (RBAC_REPO). Storing
// real email addresses in a git-tracked config file means they persist in
// history forever and can leak to anyone with read access to that repo -
// email is sourced/stored only in the application database, never here.
type User struct {
	Username     string `yaml:"username"`
	AuthProvider string `yaml:"auth_provider,omitempty"`
}

// Role represents a role definition structure.
type Role struct {
	Name        string   `yaml:"name"`
	Description string   `yaml:"description,omitempty"`
	Permissions []string `yaml:"permissions,omitempty"`
}

// UpdateRBACRepo clones the remote repo, adds the new user/org, and pushes changes.
func UpdateRBACRepo(username, firstName, lastName, orgName string) (string, error) {
	repoURL := os.Getenv("RBAC_REPO")
	token := os.Getenv("RBAC_REPO_TOKEN")

	if repoURL == "" || token == "" {
		return "", fmt.Errorf("RBAC_REPO and RBAC_REPO_TOKEN environment variables must be set")
	}

	authMethod := &http.BasicAuth{
		Username: "oauth2",
		Password: token,
	}

	maxRetries := 3
	var updatedYaml string
	var err error

	for i := 0; i < maxRetries; i++ {
		updatedYaml, err = tryUpdateRepo(repoURL, authMethod, username, firstName, lastName, orgName)
		if err == nil {
			return updatedYaml, nil
		}
		time.Sleep(time.Duration(500*(i+1)) * time.Millisecond)
	}

	return "", fmt.Errorf("failed to update RBAC repo after %d attempts: %w", maxRetries, err)
}

// AddTrackedRepo adds a repo to the org's tracked_repos in the YAML and pushes the change.
// Called by the TrackRepo handler when a user adds a public or private repo via the UI.
// Removing a repo from tracked_repos is done by editing the YAML directly (gitops flow).
func AddTrackedRepo(orgName, provider, owner, name string, private bool, addedBy string) (string, error) {
	repoURL := os.Getenv("RBAC_REPO")
	token := os.Getenv("RBAC_REPO_TOKEN")

	if repoURL == "" || token == "" {
		return "", fmt.Errorf("RBAC_REPO and RBAC_REPO_TOKEN environment variables must be set")
	}

	authMethod := &http.BasicAuth{
		Username: "oauth2",
		Password: token,
	}

	maxRetries := 3
	var updatedYaml string
	var err error

	for i := 0; i < maxRetries; i++ {
		updatedYaml, err = tryAddTrackedRepo(repoURL, authMethod, orgName, provider, owner, name, private, addedBy)
		if err == nil {
			return updatedYaml, nil
		}
		time.Sleep(time.Duration(500*(i+1)) * time.Millisecond)
	}

	return "", fmt.Errorf("failed to add tracked repo after %d attempts: %w", maxRetries, err)
}

// tryAddTrackedRepo performs the actual clone, update, and push for adding a tracked repo.
func tryAddTrackedRepo(repoURL string, authMethod *http.BasicAuth, orgName, provider, owner, name string, private bool, addedBy string) (string, error) {
	normalizedOrgName := strings.ToLower(strings.TrimSpace(orgName))

	tempDir, err := os.MkdirTemp("", "ortelius-rbac-*")
	if err != nil {
		return "", fmt.Errorf("failed to create temp dir: %w", err)
	}
	defer os.RemoveAll(tempDir)

	repo, err := git.PlainClone(tempDir, false, &git.CloneOptions{
		URL:      repoURL,
		Auth:     authMethod,
		Progress: nil,
		Depth:    1,
	})
	if err != nil {
		return "", fmt.Errorf("failed to clone repo: %w", err)
	}

	worktree, err := repo.Worktree()
	if err != nil {
		return "", fmt.Errorf("failed to get worktree: %w", err)
	}

	rbacPath := filepath.Join(tempDir, "rbac.yaml")
	yamlBytes, err := os.ReadFile(rbacPath)
	if err != nil {
		if os.IsNotExist(err) {
			yamlBytes = []byte("users: []\norgs: []\nroles: []")
		} else {
			return "", fmt.Errorf("failed to read rbac.yaml: %w", err)
		}
	}

	var config Config
	if err := yaml.Unmarshal(yamlBytes, &config); err != nil {
		return "", fmt.Errorf("failed to parse rbac.yaml: %w", err)
	}

	// Find the target org
	var targetOrg *Org
	for i := range config.Orgs {
		if strings.ToLower(config.Orgs[i].Name) == normalizedOrgName {
			targetOrg = &config.Orgs[i]
			break
		}
	}

	if targetOrg == nil {
		return "", fmt.Errorf("org %s not found in rbac.yaml", orgName)
	}

	// Check if repo is already tracked
	for _, r := range targetOrg.TrackedRepos {
		if r.Provider == provider && r.Owner == owner && r.Name == name {
			// Already tracked — return current yaml without committing
			newYamlBytes, _ := yaml.Marshal(config)
			return string(newYamlBytes), nil
		}
	}

	// Add the new tracked repo
	targetOrg.TrackedRepos = append(targetOrg.TrackedRepos, TrackedRepo{
		Provider: provider,
		Owner:    owner,
		Name:     name,
		Private:  private,
	})

	newYamlBytes, err := yaml.Marshal(config)
	if err != nil {
		return "", fmt.Errorf("failed to marshal config: %w", err)
	}

	if err := os.WriteFile(rbacPath, newYamlBytes, 0644); err != nil {
		return "", fmt.Errorf("failed to write rbac.yaml: %w", err)
	}

	if _, err := worktree.Add("rbac.yaml"); err != nil {
		return "", fmt.Errorf("failed to stage changes: %w", err)
	}

	commitMsg := fmt.Sprintf("feat: track %s/%s/%s in org %s (added by %s)", provider, owner, name, normalizedOrgName, addedBy)
	if _, err := worktree.Commit(commitMsg, &git.CommitOptions{
		Author: &object.Signature{
			Name:  "Ortelius Backend",
			Email: "noreply@ortelius.com",
			When:  time.Now(),
		},
	}); err != nil {
		return "", fmt.Errorf("failed to commit changes: %w", err)
	}

	if err := repo.Push(&git.PushOptions{Auth: authMethod}); err != nil {
		return "", fmt.Errorf("failed to push changes: %w", err)
	}

	return string(newYamlBytes), nil
}

func tryUpdateRepo(repoURL string, authMethod *http.BasicAuth, username, firstName, lastName, orgName string) (string, error) {
	normalizedOrgName := strings.ToLower(strings.TrimSpace(orgName))
	displayName := orgName

	tempDir, err := os.MkdirTemp("", "ortelius-rbac-*")
	if err != nil {
		return "", fmt.Errorf("failed to create temp dir: %w", err)
	}
	defer os.RemoveAll(tempDir)

	repo, err := git.PlainClone(tempDir, false, &git.CloneOptions{
		URL:      repoURL,
		Auth:     authMethod,
		Progress: nil,
		Depth:    1,
	})
	if err != nil {
		return "", fmt.Errorf("failed to clone repo: %w", err)
	}

	worktree, err := repo.Worktree()
	if err != nil {
		return "", fmt.Errorf("failed to get worktree: %w", err)
	}

	rbacPath := filepath.Join(tempDir, "rbac.yaml")
	yamlBytes, err := os.ReadFile(rbacPath)
	if err != nil {
		if os.IsNotExist(err) {
			yamlBytes = []byte("users: []\norgs: []\nroles: []")
		} else {
			return "", fmt.Errorf("failed to read rbac.yaml: %w", err)
		}
	}

	var config Config
	if err := yaml.Unmarshal(yamlBytes, &config); err != nil {
		return "", fmt.Errorf("failed to parse rbac.yaml: %w", err)
	}

	configUpdated := false

	var targetOrg *Org
	for i := range config.Orgs {
		if strings.ToLower(config.Orgs[i].Name) == normalizedOrgName {
			targetOrg = &config.Orgs[i]
			break
		}
	}

	if targetOrg == nil {
		newOrg := Org{
			Name:        normalizedOrgName,
			DisplayName: displayName,
			Description: fmt.Sprintf("Created for %s %s", firstName, lastName),
			Members: []Member{
				{Username: username, Role: "owner"},
			},
		}
		config.Orgs = append(config.Orgs, newOrg)
		configUpdated = true
	} else {
		if targetOrg.Name != normalizedOrgName {
			targetOrg.Name = normalizedOrgName
			configUpdated = true
		}
		if targetOrg.DisplayName == "" {
			targetOrg.DisplayName = displayName
			configUpdated = true
		}

		isMember := false
		for _, m := range targetOrg.Members {
			if m.Username == username {
				isMember = true
				break
			}
		}
		if !isMember {
			targetOrg.Members = append(targetOrg.Members, Member{
				Username: username,
				Role:     "viewer",
			})
			configUpdated = true
		}
	}

	userExists := false
	for _, user := range config.Users {
		if user.Username == username {
			userExists = true
			break
		}
	}
	if !userExists {
		config.Users = append(config.Users, User{
			Username:     username,
			AuthProvider: "local",
		})
		configUpdated = true
	}

	newYamlBytes, err := yaml.Marshal(config)
	if err != nil {
		return "", fmt.Errorf("failed to marshal config: %w", err)
	}

	if !configUpdated {
		return string(newYamlBytes), nil
	}

	if err := os.WriteFile(rbacPath, newYamlBytes, 0644); err != nil {
		return "", fmt.Errorf("failed to write rbac.yaml: %w", err)
	}

	if _, err := worktree.Add("rbac.yaml"); err != nil {
		return "", fmt.Errorf("failed to stage changes: %w", err)
	}

	commitMsg := fmt.Sprintf("feat: onboarding user %s to org %s", username, normalizedOrgName)
	if _, err := worktree.Commit(commitMsg, &git.CommitOptions{
		Author: &object.Signature{
			Name:  "Ortelius Backend",
			Email: "noreply@ortelius.com",
			When:  time.Now(),
		},
	}); err != nil {
		return "", fmt.Errorf("failed to commit changes: %w", err)
	}

	if err := repo.Push(&git.PushOptions{Auth: authMethod}); err != nil {
		return "", fmt.Errorf("failed to push changes: %w", err)
	}

	return string(newYamlBytes), nil
}
