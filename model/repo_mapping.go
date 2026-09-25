// Package model - RepoMapping defines optional artifact/gitops mapping info
// a user can attach to a tracked or GitHub-App-installed repo.
package model

// RepoMapping holds the optional per-repo mapping a user enters when
// onboarding/tracking a repo (see the welcome page and TrackedRepo):
//   - ArtifactNamespace maps the repo to the Docker/artifact namespace it
//     publishes under (e.g. "deployhub"), used to build DockerRepo for
//     vulnerability/purl matching.
//   - GitopsEndpoint maps the repo to the runtime endpoint it deploys to,
//     formatted as "<endpoint name>/<namespace>" (e.g.
//     "us-central-1_deployhub/deployhub"), for repos that hold gitops
//     manifests/charts.
//
// Persisted (not pointers) so it can be stored directly on User/TrackedRepo
// and read by relscanner-job on every scan cycle, not just applied once at
// onboarding time.
type RepoMapping struct {
	ArtifactNamespace string `json:"artifact_namespace,omitempty" yaml:"artifact_namespace,omitempty"`
	GitopsEndpoint    string `json:"gitops_endpoint,omitempty" yaml:"gitops_endpoint,omitempty"`
}

// IsEmpty reports whether neither field of the mapping has been set.
func (m RepoMapping) IsEmpty() bool {
	return m.ArtifactNamespace == "" && m.GitopsEndpoint == ""
}
