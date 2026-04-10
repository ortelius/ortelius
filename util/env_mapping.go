// Package util provides utility functions for working with Package URLs (PURLs),
// version comparisons for vulnerability checking, and extracting metadata from the environment.
//
//revive:disable-next-line:var-naming
package util

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"time"
)

// GetDerivedEnvMapping builds environment mapping with git metadata
func GetDerivedEnvMapping(mapping map[string]string) map[string]string {
	// Helper to set mapping value if empty
	setIfEmpty := func(key, value string) {
		if mapping[key] == "" {
			mapping[key] = value
		}
	}

	// Helper to get first non-empty environment variable
	getFirstEnv := func(keys ...string) string {
		for _, key := range keys {
			if val := GetEnvOrDefault(key, ""); val != "" {
				return val
			}
		}
		return ""
	}

	// PHASE 1: Check all environment variables first
	assignMetaData(mapping, setIfEmpty, getFirstEnv)

	return mapping
}

func assignMetaData(mapping map[string]string, setIfEmpty func(string, string), getFirstEnv func(...string) string) {
	now := time.Now()

	// Git-related attributes from env vars
	gitEnvMappings := map[string][]string{
		"BaseName":                 {"BASENAME"},
		"BuildDate":                {"BLDDATE"},
		"BuildId":                  {"BUILD_ID", "BUILD_NUMBER", "BUILD_NUM", "BUILDNUM", "GITHUB_RUN_ID"},
		"BuildNumber":              {"BUILD_NUMBER", "BUILD_NUM", "BUILDNUM", "GITHUB_RUN_NUMBER"},
		"BuildUrl":                 {"BUILD_URL"},
		"CompName":                 {"COMPNAME"},
		"DockerRepo":               {"DOCKERREPO", "IMAGE_REPO"},
		"DockerSha":                {"DOCKERSHA", "DIGEST"},
		"DockerTag":                {"DOCKERTAG", "IMAGE_TAG"},
		"GitBranch":                {"GIT_BRANCH"},
		"GitBranchCreateCommit":    {"GIT_BRANCH_CREATE_COMMIT"},
		"GitBranchCreateTimestamp": {"GIT_BRANCH_CREATE_TIMESTAMP"},
		"GitBranchParent":          {"GIT_BRANCH_PARENT"},
		"GitCommit":                {"GIT_COMMIT", "SHORT_SHA"},
		"GitCommitAuthors":         {"GIT_COMMIT_AUTHORS"},
		"GitCommittersCnt":         {"GIT_COMMITTERS_CNT"},
		"GitCommitTimestamp":       {"GIT_COMMIT_TIMESTAMP"},
		"GitContribPercentage":     {"GIT_CONTRIB_PERCENTAGE"},
		"GitLinesAdded":            {"GIT_LINES_ADDED"},
		"GitLinesDeleted":          {"GIT_LINES_DELETED"},
		"GitLinesTotal":            {"GIT_LINES_TOTAL"},
		"GitOrg":                   {"GIT_ORG"},
		"GitRepo":                  {"GIT_REPO", "GITHUB_REPOSITORY"},
		"GitRepoProject":           {"GIT_REPO_PROJECT"},
		"GitServerUrl":             {"GITHUB_SERVER_URL"},
		"GitSignedOffBy":           {"GIT_SIGNED_OFF_BY"},
		"GitTotalCommittersCnt":    {"GIT_TOTAL_COMMITTERS_CNT"},
		"GitTrigger":               {"GIT_TRIGGER"},
		"GitUrl":                   {"GIT_URL"},
		"GitVerifyCommit":          {"GIT_VERIFY_COMMIT"},
	}

	for key, envKeys := range gitEnvMappings {
		setIfEmpty(key, getFirstEnv(envKeys...))
	}

	// order must be preserved since commands can depend upon previous ones values
	gitCommands := map[string]string{
		"GitCommit":                "git rev-parse HEAD",
		"BuildNumber":              "git log --oneline | wc -l | tr -d ' '",
		"GitRepo":                  "git config --get remote.origin.url | awk -F/ '{print $(NF-1)\"/\"$NF}'| sed 's/.git$//'",
		"GitOrg":                   "git config --get remote.origin.url | awk -F'[@:/]' '{print $(NF-1)}'",
		"GitRepoProject":           "git config --get remote.origin.url | awk -F/ '{print $NF}'| sed 's/.git$//'",
		"GitUrl":                   "git config --get remote.origin.url",
		"GitBranch":                "git rev-parse --abbrev-ref HEAD",
		"GitVerifyCommit":          fmt.Sprintf("git verify-commit %s 2>&1 | grep -i 'Signature made' | wc -l", mapping["GitCommit"]),
		"GitSignedOffBy":           fmt.Sprintf("git log -1 %s | grep 'Signed-off-by:' | cut -d: -f2 | sed 's/^[ \t]*//;s/[ \t]*$//' | sed 's/&/\\&amp;/g; s/</\\&lt;/g; s/>/\\&gt;/g;'", mapping["GitCommit"]),
		"GitCommitTimestamp":       fmt.Sprintf("git log --pretty='format:%%cd' %s | head -1", mapping["GitCommit"]),
		"GitBranchParent":          "git show-branch -a 2>/dev/null | sed \"s/].*//\" | grep \"\\*\" | grep -v \"$(git rev-parse --abbrev-ref HEAD)\" | head -n1 | sed \"s/^.*\\[//\"",
		"GitBranchCreateCommit":    fmt.Sprintf("git log --oneline --reverse %s..%s | head -1 | awk -F' ' '{print $1}'", GetStringOrDefault(mapping["GitBranchParent"], "main"), mapping["GitBranch"]),
		"GitBranchCreateTimestamp": fmt.Sprintf("git log --pretty='format:%%cd' %s | head -1", GetStringOrDefault(mapping["GitBranchCreateCommit"], "HEAD")),
		"GitCommitAuthors":         fmt.Sprintf("git rev-list --remotes --pretty --since='%s' --until='%s' | grep -i 'Author:' | grep -v dependabot | awk -F'[:<>]' '{print $3}' | sed 's/^ //' | sed 's/ $//' | sort -u | tr '\n' ',' | sed 's/,$//'", mapping["GitBranchCreateTimestamp"], mapping["GitCommitTimestamp"]),
		"GitLinesTotal":            "git ls-files | xargs wc -l 2>/dev/null | grep total | awk '{print $1}'",
	}

	RunCmd("git fetch --unshallow")

	// Run basic git commands
	for key, cmd := range gitCommands {
		setIfEmpty(key, RunCmd(cmd))
	}

	setIfEmpty("DockerBuildDate", now.Format(time.RFC3339))
	setIfEmpty("BuildDate", now.Format(time.RFC3339))

	mapping["BuildDate"] = strings.Trim(mapping["BuildDate"], `"`)

	githubURL := mapping["GitServerUrl"] + "/" + mapping["GitRepo"] + "/actions/runs/" + mapping["BuildId"]
	if strings.Contains(githubURL, "github") {
		setIfEmpty("BuildUrl", githubURL)
	}

	// Handle repo:tag format
	if strings.Contains(mapping["DockerRepo"], ":") {
		if parts := strings.Split(mapping["DockerRepo"], ":"); len(parts) >= 2 {
			mapping["DockerRepo"] = parts[0]
			if mapping["DockerTag"] == "" {
				mapping["DockerTag"] = parts[1]
			}
		}
	}

	// Generate PURL if we have docker info
	if mapping["DockerRepo"] != "" && mapping["DockerTag"] != "" {
		setIfEmpty("Purl", dockerTagToPurl(mapping["DockerRepo"]+":"+mapping["DockerTag"]))
	}

	// Special handling for GitVerifyCommit from env
	mapping["GitVerifyCommit"] = map[bool]string{true: "Y", false: "N"}[mapping["GitVerifyCommit"] == "1" || mapping["GitVerifyCommit"] == "Y"]

	// Parse Git URL for repo info if available from env
	if gitURL := mapping["GitUrl"]; gitURL != "" && strings.Contains(gitURL, "/") {
		if parts := strings.Split(gitURL, "/"); len(parts) >= 2 {
			setIfEmpty("GitRepo", strings.ReplaceAll(strings.Join(parts[len(parts)-2:], "/"), ".git", ""))
		}
	}

	// Set GitTag from GitBranch if not set
	if mapping["GitTag"] == "" && mapping["GitBranch"] != "" {
		mapping["GitTag"] = mapping["GitBranch"]
	}

	// Job trigger logic
	if trigger := mapping["GitTrigger"]; trigger != "" && trigger != "workflow_dispatch" {
		setIfEmpty("JobTriggeredBy", "SCM")
	}

	// BASENAME - derive from current directory if not set from env
	if mapping["BaseName"] == "" {
		if cwd, err := os.Getwd(); err == nil {
			mapping["BaseName"] = filepath.Base(cwd)
		}
	}

	// COMPNAME - derive from git repo project if not set from env
	if mapping["CompName"] == "" && mapping["GitRepoProject"] != "" {
		mapping["CompName"] = mapping["GitRepoProject"]
	}
}

func dockerTagToPurl(dockerTag string) string {
	if IsEmpty(dockerTag) || dockerTag == ":" {
		return ""
	}

	// Split the docker tag into registry, namespace/repository, and tag
	if !strings.Contains(dockerTag, ":") {
		dockerTag += ":latest"
	}

	parts := strings.Split(dockerTag, ":")
	if len(parts) != 2 {
		return ""
	}

	registryRepo := parts[0]
	tag := parts[1]

	if !strings.Contains(registryRepo, "/") {
		registryRepo = "library/" + registryRepo
	}

	registryParts := strings.SplitN(registryRepo, "/", 2)
	if len(registryParts) != 2 {
		return ""
	}

	registry := registryParts[0]
	repo := registryParts[1]

	return fmt.Sprintf("pkg:docker/%s/%s@%s", registry, repo, tag)
}
