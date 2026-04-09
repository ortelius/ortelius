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
	Name        string            `yaml:"name"`
	DisplayName string            `yaml:"display_name,omitempty"`
	Description string            `yaml:"description,omitempty"`
	Metadata    map[string]string `yaml:"metadata,omitempty"`
	Members     []Member          `yaml:"members,omitempty"`
}

// Member represents a user member of an organization.
type Member struct {
	Username string `yaml:"username"`
	Role     string `yaml:"role"`
}

// User represents a user identity structure.
type User struct {
	Username     string `yaml:"username"`
	Email        string `yaml:"email"`
	AuthProvider string `yaml:"auth_provider,omitempty"`
}

// Role represents a role definition structure.
type Role struct {
	Name        string   `yaml:"name"`
	Description string   `yaml:"description,omitempty"`
	Permissions []string `yaml:"permissions,omitempty"`
}

// UpdateRBACRepo clones the remote repo, adds the new user/org, and pushes changes.
func UpdateRBACRepo(username, email, firstName, lastName, orgName string) (string, error) {
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
		updatedYaml, err = tryUpdateRepo(repoURL, authMethod, username, email, firstName, lastName, orgName)
		if err == nil {
			return updatedYaml, nil
		}
		time.Sleep(time.Duration(500*(i+1)) * time.Millisecond)
	}

	return "", fmt.Errorf("failed to update RBAC repo after %d attempts: %w", maxRetries, err)
}

func tryUpdateRepo(repoURL string, authMethod *http.BasicAuth, username, email, firstName, lastName, orgName string) (string, error) {
	// Normalize org name to lowercase for internal use
	normalizedOrgName := strings.ToLower(strings.TrimSpace(orgName))
	displayName := orgName // Preserve original for display

	// 1. Clone
	tempDir, err := os.MkdirTemp("", "pdvd-rbac-*")
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

	// 2. Read YAML
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

	// 3. Update Configuration
	configUpdated := false

	// Find or Create Org (using normalized name)
	var targetOrg *Org
	for i := range config.Orgs {
		if strings.ToLower(config.Orgs[i].Name) == normalizedOrgName {
			targetOrg = &config.Orgs[i]
			break
		}
	}

	if targetOrg == nil {
		// Create new Org with User as Owner
		newOrg := Org{
			Name:        normalizedOrgName, // Lowercase for internal use
			DisplayName: displayName,       // Original case for display
			Description: fmt.Sprintf("Created for %s %s", firstName, lastName),
			Members: []Member{
				{Username: username, Role: "owner"},
			},
		}
		config.Orgs = append(config.Orgs, newOrg)
		configUpdated = true
	} else {
		// Ensure org name is normalized
		if targetOrg.Name != normalizedOrgName {
			targetOrg.Name = normalizedOrgName
			configUpdated = true
		}
		// Preserve display name if it exists, otherwise set it
		if targetOrg.DisplayName == "" {
			targetOrg.DisplayName = displayName
			configUpdated = true
		}

		// Check if user is already a member
		isMember := false
		for _, m := range targetOrg.Members {
			if m.Username == username {
				isMember = true
				break
			}
		}
		if !isMember {
			// Add as Viewer to existing Org
			targetOrg.Members = append(targetOrg.Members, Member{
				Username: username,
				Role:     "viewer",
			})
			configUpdated = true
		}
	}

	// Ensure User Identity Exists
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
			Email:        email,
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

	// 4. Write, Commit, Push
	if err := os.WriteFile(rbacPath, newYamlBytes, 0644); err != nil {
		return "", fmt.Errorf("failed to write rbac.yaml: %w", err)
	}

	_, err = worktree.Add("rbac.yaml")
	if err != nil {
		return "", fmt.Errorf("failed to stage changes: %w", err)
	}

	commitMsg := fmt.Sprintf("feat: onboarding user %s to org %s", username, normalizedOrgName)
	_, err = worktree.Commit(commitMsg, &git.CommitOptions{
		Author: &object.Signature{
			Name:  "PDVD Backend",
			Email: "noreply@pdvd.com",
			When:  time.Now(),
		},
	})
	if err != nil {
		return "", fmt.Errorf("failed to commit changes: %w", err)
	}

	err = repo.Push(&git.PushOptions{
		Auth: authMethod,
	})
	if err != nil {
		return "", fmt.Errorf("failed to push changes: %w", err)
	}

	return string(newYamlBytes), nil
}
