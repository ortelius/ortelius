// Package scorecard defines the GraphQL types for OpenSSF Scorecard results.
package scorecard

import (
	"github.com/graphql-go/graphql"
	"github.com/ortelius/pdvd-backend/v12/model"
)

// ScorecardDocumentationType represents the documentation links for a check.
var ScorecardDocumentationType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ScorecardDocumentation",
	Fields: graphql.Fields{
		"Short": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			doc, _ := p.Source.(model.Documentation)
			return doc.Short, nil
		}},
		"URL": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			doc, _ := p.Source.(model.Documentation)
			return doc.URL, nil
		}},
	},
})

// ScorecardCheckType represents an individual scorecard check result.
var ScorecardCheckType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ScorecardCheck",
	Fields: graphql.Fields{
		"Name": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			check, _ := p.Source.(model.Check)
			return check.Name, nil
		}},
		"Score": &graphql.Field{Type: graphql.Int, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			check, _ := p.Source.(model.Check)
			return check.Score, nil
		}},
		"Reason": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			check, _ := p.Source.(model.Check)
			return check.Reason, nil
		}},
		"Details": &graphql.Field{Type: graphql.NewList(graphql.String), Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			check, _ := p.Source.(model.Check)
			return check.Details, nil
		}},
		"Documentation": &graphql.Field{Type: ScorecardDocumentationType, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			check, _ := p.Source.(model.Check)
			return check.Documentation, nil
		}},
	},
})

// ScorecardRepoType represents the repository metadata in a scorecard result.
var ScorecardRepoType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ScorecardRepo",
	Fields: graphql.Fields{
		"Name": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			repo, _ := p.Source.(model.Repo)
			return repo.Name, nil
		}},
		"Commit": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			repo, _ := p.Source.(model.Repo)
			return repo.Commit, nil
		}},
	},
})

// ScorecardScoresType represents the scoring metadata.
var ScorecardScoresType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ScorecardScores",
	Fields: graphql.Fields{
		"Version": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			scores, _ := p.Source.(model.Scores)
			return scores.Version, nil
		}},
		"Commit": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			scores, _ := p.Source.(model.Scores)
			return scores.Commit, nil
		}},
	},
})

// ScorecardResultType represents the full OpenSSF Scorecard analysis result.
var ScorecardResultType = graphql.NewObject(graphql.ObjectConfig{
	Name: "ScorecardResult",
	Fields: graphql.Fields{
		"Date": &graphql.Field{Type: graphql.String, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			res, _ := p.Source.(*model.ScorecardAPIResponse)
			return res.Date, nil
		}},
		"Repo": &graphql.Field{Type: ScorecardRepoType, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			res, _ := p.Source.(*model.ScorecardAPIResponse)
			return res.Repo, nil
		}},
		"Scorecard": &graphql.Field{Type: ScorecardScoresType, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			res, _ := p.Source.(*model.ScorecardAPIResponse)
			return res.Scorecard, nil
		}},
		"Score": &graphql.Field{Type: graphql.Float, Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			res, _ := p.Source.(*model.ScorecardAPIResponse)
			return res.Score, nil
		}},
		"Checks": &graphql.Field{Type: graphql.NewList(ScorecardCheckType), Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			res, _ := p.Source.(*model.ScorecardAPIResponse)
			return res.Checks, nil
		}},
		"Metadata": &graphql.Field{Type: graphql.NewList(graphql.String), Resolve: func(p graphql.ResolveParams) (interface{}, error) {
			res, _ := p.Source.(*model.ScorecardAPIResponse)
			return res.Metadata, nil
		}},
	},
})
