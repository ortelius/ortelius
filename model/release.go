// Package model - ProjectRelease defines the struct and handles marshaling/unmarshaling the struct to/from NFT Storage.
package model

import (
	"strings"
	"time"

	"github.com/ortelius/pdvd-backend/v12/util"
)

// ProjectRelease represents a release object stored in the database.
type ProjectRelease struct {
	Key                      string                `json:"_key,omitempty"`
	Cid                      string                `json:"cid,omitempty"`
	ObjType                  string                `json:"objtype,omitempty"`
	Name                     string                `json:"name"`
	Version                  string                `json:"version"`
	VersionMajor             *int                  `json:"version_major,omitempty"`
	VersionMinor             *int                  `json:"version_minor,omitempty"`
	VersionPatch             *int                  `json:"version_patch,omitempty"`
	VersionPrerelease        string                `json:"version_prerelease,omitempty"`
	VersionBuildMetadata     string                `json:"version_build_metadata,omitempty"`
	ProjectType              string                `json:"projecttype,omitempty"`
	ContentSha               string                `json:"contentsha,omitempty"`
	Basename                 string                `json:"basename,omitempty"`
	BuildDate                time.Time             `json:"builddate,omitempty"`
	BuildID                  string                `json:"buildid,omitempty"`
	BuildNum                 string                `json:"buildnum,omitempty"`
	BuildURL                 string                `json:"buildurl,omitempty"`
	DockerRepo               string                `json:"dockerrepo,omitempty"`
	DockerSha                string                `json:"dockersha,omitempty"`
	DockerTag                string                `json:"dockertag,omitempty"`
	GitBranch                string                `json:"gitbranch,omitempty"`
	GitBranchCreateCommit    string                `json:"gitbranchcreatecommit,omitempty"`
	GitBranchCreateTimestamp time.Time             `json:"gitbranchcreatetimestamp,omitempty"`
	GitBranchParent          string                `json:"gitbranchparent,omitempty"`
	GitCommit                string                `json:"gitcommit,omitempty"`
	GitCommitAuthors         string                `json:"gitcommitauthors,omitempty"`
	GitCommittersCnt         string                `json:"gitcommittescnt,omitempty"`
	GitCommitTimestamp       time.Time             `json:"gitcommittimestamp,omitempty"`
	GitContribPercentage     string                `json:"gitcontribpercentage,omitempty"`
	GitLinesAdded            string                `json:"gitlinesadded,omitempty"`
	GitLinesDeleted          string                `json:"gitlinesdeleted,omitempty"`
	GitLinesTotal            string                `json:"gitlinestotal,omitempty"`
	GitOrg                   string                `json:"gitorg,omitempty"`
	GitPrevCompCommit        string                `json:"gitpreviouscomponentcommit,omitempty"`
	GitRepo                  string                `json:"gitrepo,omitempty"`
	GitRepoProject           string                `json:"gitrepoproject,omitempty"`
	GitSignedOffBy           string                `json:"gitsignedoffby,omitempty"`
	GitTag                   string                `json:"gittag,omitempty"`
	GitTotalCommittersCnt    string                `json:"gittotalcommittescnt,omitempty"`
	GitURL                   string                `json:"giturl,omitempty"`
	GitVerifyCommit          bool                  `json:"gitverifycommit,omitempty"`
	OpenSSFScorecardScore    float64               `json:"openssf_scorecard_score,omitempty"`
	ScorecardResult          *ScorecardAPIResponse `json:"scorecard_result,omitempty"`
	VulnerabilityCount       int                   `json:"vulnerability_count,omitempty"`
	PrevVersionVulnCount     int                   `json:"prev_version_vuln_count,omitempty"`

	// Parsed name components
	Org       string   `json:"org,omitempty"`       // Organization name parsed from name field
	Path      []string `json:"path,omitempty"`      // Path components parsed from name field
	Shortname string   `json:"shortname,omitempty"` // Short name parsed from name field
	IsPublic  bool     `json:"is_public"`           // Whether the release is public (default: true)
}

// ScorecardAPIResponse represents the JSON response from the OpenSSF Scorecard API.
type ScorecardAPIResponse struct {
	Date      string   `json:"date"`
	Repo      Repo     `json:"repo"`
	Scorecard Scores   `json:"scorecard"`
	Score     float64  `json:"score"`
	Checks    []Check  `json:"checks"`
	Metadata  []string `json:"metadata,omitempty"`
}

// Repo represents repository information in the Scorecard response.
type Repo struct {
	Name   string `json:"name"`
	Commit string `json:"commit"`
}

// Scores represents scoring version information.
type Scores struct {
	Version string `json:"version"`
	Commit  string `json:"commit"`
}

// Check represents a specific Scorecard check.
type Check struct {
	Name          string        `json:"name"`
	Score         int           `json:"score"`
	Reason        string        `json:"reason"`
	Details       []string      `json:"details,omitempty"`
	Documentation Documentation `json:"documentation"`
}

// Documentation represents links to check documentation.
type Documentation struct {
	Short string `json:"short"`
	URL   string `json:"url"`
}

// NewProjectRelease creates a new ProjectRelease with default values.
func NewProjectRelease() *ProjectRelease {
	return &ProjectRelease{
		ObjType:               "ProjectRelease",
		OpenSSFScorecardScore: -1,
		IsPublic:              true,
		Path:                  []string{},
	}
}

// ParseAndSetNameComponents parses the Name field and populates org, path, shortname, and is_public.
// Org is only derived from the name if the caller has not already set it explicitly. This mirrors
// the same policy as Endpoint.ParseAndSetNameComponents and prevents callers that supply a
// meaningful org from having it silently overwritten by the first path segment of the name.
func (r *ProjectRelease) ParseAndSetNameComponents() {
	parts := strings.Split(r.Name, "/")

	if len(parts) > 1 {
		if r.Org == "" {
			r.Org = parts[0]
		}
		r.Shortname = parts[1]
	} else {
		if r.Org == "" {
			r.Org = "library"
		}
		r.Shortname = r.Name
	}

	r.Path = []string{}
	r.IsPublic = true
}

// ParseAndSetVersion parses the version string into semver components.
func (r *ProjectRelease) ParseAndSetVersion() {
	if r.Version == "" {
		return
	}

	cleanedVersion := util.CleanVersion(r.Version)
	r.Version = cleanedVersion

	parsed := util.ParseSemver(cleanedVersion)
	if parsed != nil {
		r.VersionMajor = parsed.Major
		r.VersionMinor = parsed.Minor
		r.VersionPatch = parsed.Patch
		r.VersionPrerelease = parsed.Prerelease
		r.VersionBuildMetadata = parsed.BuildMetadata
	}
}
