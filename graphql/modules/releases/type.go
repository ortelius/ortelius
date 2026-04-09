// Package releases defines the GraphQL types for release management.
package releases

import (
	"context"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/graphql-go/graphql"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
)

// SBOMType represents the Software Bill of Materials.
var SBOMType = graphql.NewObject(graphql.ObjectConfig{
	Name: "SBOM",
	Fields: graphql.Fields{
		"key":        &graphql.Field{Type: graphql.String},
		"contentsha": &graphql.Field{Type: graphql.String},
		"objtype":    &graphql.Field{Type: graphql.String},
		"content": &graphql.Field{
			Type: graphql.String,
			Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				if sbom, ok := p.Source.(model.SBOM); ok {
					return string(sbom.Content), nil
				}
				return nil, nil
			},
		},
	},
})

// ReleaseInfoType represents summary information about a release.
var ReleaseInfoType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ReleaseInfo",
	Fields: graphql.Fields{
		"release_name":    &graphql.Field{Type: graphql.String},
		"release_version": &graphql.Field{Type: graphql.String},
	},
})

// AffectedReleaseType represents a release affected by a specific vulnerability.
var AffectedReleaseType = graphql.NewObject(graphql.ObjectConfig{
	Name: "AffectedRelease",
	Fields: graphql.Fields{
		"cve_id":                    &graphql.Field{Type: graphql.String},
		"summary":                   &graphql.Field{Type: graphql.String},
		"details":                   &graphql.Field{Type: graphql.String},
		"severity_score":            &graphql.Field{Type: graphql.Float},
		"severity_rating":           &graphql.Field{Type: graphql.String},
		"published":                 &graphql.Field{Type: graphql.String},
		"modified":                  &graphql.Field{Type: graphql.String},
		"aliases":                   &graphql.Field{Type: graphql.NewList(graphql.String)},
		"package":                   &graphql.Field{Type: graphql.String},
		"affected_version":          &graphql.Field{Type: graphql.String},
		"full_purl":                 &graphql.Field{Type: graphql.String},
		"fixed_in":                  &graphql.Field{Type: graphql.NewList(graphql.String)},
		"release_name":              &graphql.Field{Type: graphql.String},
		"release_version":           &graphql.Field{Type: graphql.String},
		"version_count":             &graphql.Field{Type: graphql.Int},
		"content_sha":               &graphql.Field{Type: graphql.String},
		"project_type":              &graphql.Field{Type: graphql.String},
		"openssf_scorecard_score":   &graphql.Field{Type: graphql.Float},
		"dependency_count":          &graphql.Field{Type: graphql.Int},
		"synced_endpoint_count":     &graphql.Field{Type: graphql.Int},
		"vulnerability_count":       &graphql.Field{Type: graphql.Int},
		"vulnerability_count_delta": &graphql.Field{Type: graphql.Int},
	},
})

// OrgAggregatedReleaseType represents releases aggregated by organization
var OrgAggregatedReleaseType = graphql.NewObject(graphql.ObjectConfig{
	Name: "OrgAggregatedRelease",
	Fields: graphql.Fields{
		"org_name":                  &graphql.Field{Type: graphql.String},
		"total_releases":            &graphql.Field{Type: graphql.Int},
		"total_versions":            &graphql.Field{Type: graphql.Int},
		"total_vulnerabilities":     &graphql.Field{Type: graphql.Int},
		"critical_count":            &graphql.Field{Type: graphql.Int},
		"high_count":                &graphql.Field{Type: graphql.Int},
		"medium_count":              &graphql.Field{Type: graphql.Int},
		"low_count":                 &graphql.Field{Type: graphql.Int},
		"max_severity_score":        &graphql.Field{Type: graphql.Float},
		"avg_scorecard_score":       &graphql.Field{Type: graphql.Float},
		"total_dependencies":        &graphql.Field{Type: graphql.Int},
		"synced_endpoint_count":     &graphql.Field{Type: graphql.Int},
		"vulnerability_count_delta": &graphql.Field{Type: graphql.Int},
	},
})

// GetReleaseType returns the ReleaseType with proper circular dependency handling
func GetReleaseType(db database.DBConnection, vulnerabilityType *graphql.Object, affectedEndpointType *graphql.Object, scorecardResultType *graphql.Object) *graphql.Object {
	return graphql.NewObject(graphql.ObjectConfig{
		Name: "Release",
		Fields: graphql.Fields{
			"key":          &graphql.Field{Type: graphql.String},
			"name":         &graphql.Field{Type: graphql.String},
			"version":      &graphql.Field{Type: graphql.String},
			"project_type": &graphql.Field{Type: graphql.String},

			"content_sha": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.ContentSha, nil
			}},
			"docker_repo": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.DockerRepo, nil
			}},
			"docker_tag": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.DockerTag, nil
			}},
			"docker_sha": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.DockerSha, nil
			}},
			"basename": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.Basename, nil
			}},

			"git_commit": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitCommit, nil
			}},
			"git_branch": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitBranch, nil
			}},
			"git_tag": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitTag, nil
			}},
			"git_repo": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitRepo, nil
			}},
			"git_org": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitOrg, nil
			}},
			"git_url": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitURL, nil
			}},
			"git_repo_project": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitRepoProject, nil
			}},
			"git_verify_commit": &graphql.Field{Type: graphql.Boolean, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitVerifyCommit, nil
			}},
			"git_signed_off_by": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitSignedOffBy, nil
			}},

			"git_commit_timestamp": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitCommitTimestamp, nil
			}},
			"git_commit_authors": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitCommitAuthors, nil
			}},
			"git_committerscnt": &graphql.Field{Type: graphql.Int, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitCommittersCnt, nil
			}},
			"git_total_committerscnt": &graphql.Field{Type: graphql.Int, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitTotalCommittersCnt, nil
			}},
			"git_contrib_percentage": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitContribPercentage, nil
			}},
			"git_lines_added": &graphql.Field{Type: graphql.Int, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitLinesAdded, nil
			}},
			"git_lines_deleted": &graphql.Field{Type: graphql.Int, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitLinesDeleted, nil
			}},
			"git_lines_total": &graphql.Field{Type: graphql.Int, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitLinesTotal, nil
			}},
			"git_prev_comp_commit": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.GitPrevCompCommit, nil
			}},

			"build_date": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.BuildDate, nil
			}},
			"build_id": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.BuildID, nil
			}},
			"build_num": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.BuildNum, nil
			}},
			"build_url": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
				release, _ := p.Source.(model.ProjectRelease)
				return release.BuildURL, nil
			}},

			"sbom": &graphql.Field{
				Type: SBOMType,
				Resolve: func(p graphql.ResolveParams) (interface{}, error) {
					release, ok := p.Source.(model.ProjectRelease)
					if !ok {
						return nil, nil
					}
					ctx := context.Background()
					query := `
						FOR r IN release
							FILTER r.name == @name && r.version == @version
							FOR v IN 1..1 OUTBOUND r release2sbom
								RETURN v
					`
					cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
						BindVars: map[string]interface{}{
							"name":    release.Name,
							"version": release.Version,
						},
					})
					if err != nil {
						return nil, err
					}
					defer cursor.Close()
					var sbom model.SBOM
					if cursor.HasMore() {
						_, err = cursor.ReadDocument(ctx, &sbom)
						if err != nil {
							return nil, err
						}
						return sbom, nil
					}
					return nil, nil
				},
			},
			"vulnerabilities": &graphql.Field{
				Type: graphql.NewList(vulnerabilityType),
				Resolve: func(p graphql.ResolveParams) (interface{}, error) {
					release, ok := p.Source.(model.ProjectRelease)
					if !ok {
						return []map[string]interface{}{}, nil
					}
					vulnerabilities, err := ResolveReleaseVulnerabilities(db, release.Name, release.Version)
					if err != nil {
						return []map[string]interface{}{}, nil
					}
					if vulnerabilities == nil {
						return []map[string]interface{}{}, nil
					}
					return vulnerabilities, nil
				},
			},

			"dependency_count": &graphql.Field{
				Type: graphql.Int,
				Resolve: func(p graphql.ResolveParams) (interface{}, error) {
					release, ok := p.Source.(model.ProjectRelease)
					if !ok {
						return 0, nil
					}

					ctx := context.Background()
					query := `
						FOR release IN release
							FILTER release.name == @name AND release.version == @version
							FOR sbom IN 1..1 OUTBOUND release release2sbom
								LIMIT 1
								LET dependencyCount = (
									FOR edge IN sbom2purl
										FILTER edge._from == sbom._id
										COLLECT fullPurl = edge.full_purl
										RETURN 1
								)
								RETURN LENGTH(dependencyCount)
					`

					cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
						BindVars: map[string]interface{}{
							"name":    release.Name,
							"version": release.Version,
						},
					})
					if err != nil {
						return 0, err
					}
					defer cursor.Close()

					if cursor.HasMore() {
						var count int
						_, err := cursor.ReadDocument(ctx, &count)
						if err != nil {
							return 0, err
						}
						return count, nil
					}

					return 0, nil
				},
			},

			"synced_endpoint_count": &graphql.Field{
				Type: graphql.Int,
				Resolve: func(p graphql.ResolveParams) (interface{}, error) {
					release, ok := p.Source.(model.ProjectRelease)
					if !ok {
						return 0, nil
					}

					endpoints, err := ResolveAffectedEndpoints(db, release.Name, release.Version)
					if err != nil {
						return 0, err
					}

					return len(endpoints), nil
				},
			},
			"synced_endpoints": &graphql.Field{
				Type: graphql.NewList(affectedEndpointType),
				Resolve: func(p graphql.ResolveParams) (interface{}, error) {
					release, ok := p.Source.(model.ProjectRelease)
					if !ok {
						return []map[string]interface{}{}, nil
					}
					endpoints, err := ResolveAffectedEndpoints(db, release.Name, release.Version)
					if err != nil {
						return []map[string]interface{}{}, nil
					}
					if endpoints == nil {
						return []map[string]interface{}{}, nil
					}
					return endpoints, nil
				},
			},

			"openssf_scorecard_score": &graphql.Field{
				Type: graphql.Float,
				Resolve: func(p graphql.ResolveParams) (interface{}, error) {
					release, _ := p.Source.(model.ProjectRelease)
					return release.OpenSSFScorecardScore, nil
				},
			},
			"scorecard_result": &graphql.Field{
				Type: scorecardResultType,
				Resolve: func(p graphql.ResolveParams) (interface{}, error) {
					release, _ := p.Source.(model.ProjectRelease)
					return release.ScorecardResult, nil
				},
			},
		},
	})
}
