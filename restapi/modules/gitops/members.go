// Package gitops — org member management via the RBAC git repository.
//
// These functions implement the same GitOps pattern as UpdateRBACRepo and
// AddTrackedRepo: clone RBAC_REPO, mutate rbac.yaml, commit, push, and
// return the updated YAML so the caller can apply it locally with
// rbac.ApplyRBAC. The git repo remains the single source of truth —
// any member change that skipped the YAML would be reverted (and the
// user deactivated) on the next webhook/startup sync.
//
// Like the rest of this package, no email addresses or other PII are
// ever written to rbac.yaml. Emails live only in the application
// database and are set by the calling handler after ApplyRBAC runs.
package gitops

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"

	"github.com/go-git/go-git/v5"
	"github.com/go-git/go-git/v5/plumbing/object"
	"github.com/go-git/go-git/v5/plumbing/transport/http"
	"gopkg.in/yaml.v2"
)

// Sentinel errors so handlers can map failures to proper HTTP statuses.
var (
	// ErrOrgNotFound is returned when the target org does not exist in rbac.yaml.
	ErrOrgNotFound = errors.New("org not found in rbac.yaml")
	// ErrAlreadyMember is returned when the user is already a member of the org.
	ErrAlreadyMember = errors.New("user is already a member of this org")
	// ErrNotAMember is returned when the user is not a member of the org.
	ErrNotAMember = errors.New("user is not a member of this org")
	// ErrLastOwner is returned when an operation would leave the org with no owner.
	ErrLastOwner = errors.New("operation would leave the org without an owner")
	// ErrInvalidRole is returned when the role is not one of the known roles.
	ErrInvalidRole = errors.New("invalid role")
)

// rbacRepoMu serializes RBAC repo mutations within this process. Cross-process
// races are still handled by the retry loop (push conflicts force a fresh
// clone + re-apply on the next attempt), but serializing locally avoids
// pointless conflict churn when several owners act at once.
var rbacRepoMu sync.Mutex

// validMemberRoles mirrors the role set understood by rbac.getRoleRank.
var validMemberRoles = map[string]bool{
	"owner":  true,
	"admin":  true,
	"editor": true,
	"viewer": true,
}

// IsValidMemberRole reports whether role is one of owner/admin/editor/viewer.
func IsValidMemberRole(role string) bool {
	return validMemberRoles[strings.ToLower(strings.TrimSpace(role))]
}

// AddOrgMember adds username to the org's members list (and to the top-level
// users list if absent), commits, and pushes. Returns the updated YAML.
//
// Fails with ErrOrgNotFound if the org is missing (invites never create
// orgs — that is the signup flow's job) and ErrAlreadyMember if the user
// is already a member.
func AddOrgMember(orgName, username, role, invitedBy string) (string, error) {
	role = strings.ToLower(strings.TrimSpace(role))
	if !validMemberRoles[role] {
		return "", ErrInvalidRole
	}
	username = strings.TrimSpace(username)
	normalizedOrgName := strings.ToLower(strings.TrimSpace(orgName))

	commitMsg := fmt.Sprintf("feat: add %s to org %s as %s (invited by %s)",
		username, normalizedOrgName, role, invitedBy)

	return mutateRBACRepo(commitMsg, func(config *Config) (bool, error) {
		targetOrg := findOrg(config, normalizedOrgName)
		if targetOrg == nil {
			return false, ErrOrgNotFound
		}

		for _, m := range targetOrg.Members {
			if m.Username == username {
				return false, ErrAlreadyMember
			}
		}

		targetOrg.Members = append(targetOrg.Members, Member{
			Username: username,
			Role:     role,
		})

		userExists := false
		for _, u := range config.Users {
			if u.Username == username {
				userExists = true
				break
			}
		}
		if !userExists {
			config.Users = append(config.Users, User{
				Username:     username,
				AuthProvider: "local",
			})
		}

		return true, nil
	})
}

// SetOrgMemberRole changes username's role within the org, commits, and
// pushes. Demoting the last remaining owner is rejected with ErrLastOwner.
func SetOrgMemberRole(orgName, username, role, changedBy string) (string, error) {
	role = strings.ToLower(strings.TrimSpace(role))
	if !validMemberRoles[role] {
		return "", ErrInvalidRole
	}
	username = strings.TrimSpace(username)
	normalizedOrgName := strings.ToLower(strings.TrimSpace(orgName))

	commitMsg := fmt.Sprintf("feat: set role of %s in org %s to %s (changed by %s)",
		username, normalizedOrgName, role, changedBy)

	return mutateRBACRepo(commitMsg, func(config *Config) (bool, error) {
		targetOrg := findOrg(config, normalizedOrgName)
		if targetOrg == nil {
			return false, ErrOrgNotFound
		}

		memberIdx := -1
		for i, m := range targetOrg.Members {
			if m.Username == username {
				memberIdx = i
				break
			}
		}
		if memberIdx == -1 {
			return false, ErrNotAMember
		}

		current := strings.ToLower(targetOrg.Members[memberIdx].Role)
		if current == role {
			return false, nil // no-op — nothing to commit
		}

		// Guard: demoting the only owner would orphan the org.
		if current == "owner" && role != "owner" && countOwners(targetOrg) <= 1 {
			return false, ErrLastOwner
		}

		targetOrg.Members[memberIdx].Role = role
		return true, nil
	})
}

// RemoveOrgMember removes username from the org's members list, commits, and
// pushes. If the user belongs to no other org afterward they are also removed
// from the top-level users list, which causes rbac.ApplyRBAC to deactivate
// the account (status "removed"). Removing the last owner is rejected.
func RemoveOrgMember(orgName, username, removedBy string) (string, error) {
	username = strings.TrimSpace(username)
	normalizedOrgName := strings.ToLower(strings.TrimSpace(orgName))

	commitMsg := fmt.Sprintf("feat: remove %s from org %s (removed by %s)",
		username, normalizedOrgName, removedBy)

	return mutateRBACRepo(commitMsg, func(config *Config) (bool, error) {
		targetOrg := findOrg(config, normalizedOrgName)
		if targetOrg == nil {
			return false, ErrOrgNotFound
		}

		memberIdx := -1
		for i, m := range targetOrg.Members {
			if m.Username == username {
				memberIdx = i
				break
			}
		}
		if memberIdx == -1 {
			return false, ErrNotAMember
		}

		if strings.ToLower(targetOrg.Members[memberIdx].Role) == "owner" && countOwners(targetOrg) <= 1 {
			return false, ErrLastOwner
		}

		targetOrg.Members = append(targetOrg.Members[:memberIdx], targetOrg.Members[memberIdx+1:]...)

		// Drop from the top-level users list only if they no longer belong
		// to ANY org. ApplyRBAC will then deactivate the account.
		if !userInAnyOrg(config, username) {
			for i, u := range config.Users {
				if u.Username == username {
					config.Users = append(config.Users[:i], config.Users[i+1:]...)
					break
				}
			}
		}

		return true, nil
	})
}

// findOrg returns a pointer to the org with the given (already normalized)
// name, or nil.
func findOrg(config *Config, normalizedOrgName string) *Org {
	for i := range config.Orgs {
		if strings.ToLower(strings.TrimSpace(config.Orgs[i].Name)) == normalizedOrgName {
			return &config.Orgs[i]
		}
	}
	return nil
}

// countOwners returns how many members of the org hold the owner role.
func countOwners(org *Org) int {
	count := 0
	for _, m := range org.Members {
		if strings.ToLower(m.Role) == "owner" {
			count++
		}
	}
	return count
}

// userInAnyOrg reports whether username appears in any org's members list.
func userInAnyOrg(config *Config, username string) bool {
	for _, o := range config.Orgs {
		for _, m := range o.Members {
			if m.Username == username {
				return true
			}
		}
	}
	return false
}

// mutateRBACRepo clones RBAC_REPO, applies mutate to the parsed rbac.yaml,
// and — if mutate reports a change — commits and pushes. Push conflicts and
// transient failures are retried with a fresh clone, matching the retry
// semantics of UpdateRBACRepo / AddTrackedRepo. Sentinel errors from mutate
// abort immediately (retrying a business-rule failure is pointless).
//
// Returns the resulting YAML (updated or unchanged).
func mutateRBACRepo(commitMsg string, mutate func(*Config) (bool, error)) (string, error) {
	repoURL := os.Getenv("RBAC_REPO")
	token := os.Getenv("RBAC_REPO_TOKEN")

	if repoURL == "" || token == "" {
		return "", fmt.Errorf("RBAC_REPO and RBAC_REPO_TOKEN environment variables must be set")
	}

	authMethod := &http.BasicAuth{
		Username: "oauth2",
		Password: token,
	}

	rbacRepoMu.Lock()
	defer rbacRepoMu.Unlock()

	maxRetries := 3
	var updatedYaml string
	var err error

	for i := 0; i < maxRetries; i++ {
		updatedYaml, err = tryMutateRBACRepo(repoURL, authMethod, commitMsg, mutate)
		if err == nil {
			return updatedYaml, nil
		}
		// Business-rule failures are final — don't retry them.
		if errors.Is(err, ErrOrgNotFound) || errors.Is(err, ErrAlreadyMember) ||
			errors.Is(err, ErrNotAMember) || errors.Is(err, ErrLastOwner) ||
			errors.Is(err, ErrInvalidRole) {
			return "", err
		}
		time.Sleep(time.Duration(500*(i+1)) * time.Millisecond)
	}

	return "", fmt.Errorf("failed to update RBAC repo after %d attempts: %w", maxRetries, err)
}

// tryMutateRBACRepo performs a single clone → mutate → commit → push cycle.
func tryMutateRBACRepo(repoURL string, authMethod *http.BasicAuth, commitMsg string, mutate func(*Config) (bool, error)) (string, error) {
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

	changed, err := mutate(&config)
	if err != nil {
		return "", err
	}

	newYamlBytes, err := yaml.Marshal(config)
	if err != nil {
		return "", fmt.Errorf("failed to marshal config: %w", err)
	}

	if !changed {
		return string(newYamlBytes), nil
	}

	if err := os.WriteFile(rbacPath, newYamlBytes, 0644); err != nil {
		return "", fmt.Errorf("failed to write rbac.yaml: %w", err)
	}

	if _, err := worktree.Add("rbac.yaml"); err != nil {
		return "", fmt.Errorf("failed to stage changes: %w", err)
	}

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
