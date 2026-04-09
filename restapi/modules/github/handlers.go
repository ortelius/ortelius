// Package github provides GitHub integration handlers for the REST API.
package github

import (
	"fmt"
	"strings"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
)

// ListRepos returns the list of repositories accessible to the user via App Installation
func ListRepos(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		username := c.Locals("username").(string)
		ctx := c.Context()

		query := `FOR u IN users FILTER u.username == @username LIMIT 1 RETURN u`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{"username": username},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Database error"})
		}
		defer cursor.Close()

		var user model.User
		if _, err := cursor.ReadDocument(ctx, &user); err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "User not found"})
		}

		if user.GitHubInstallationID == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "GitHub App not connected"})
		}

		// 1. Generate Fresh Installation Token (Server-to-Server)
		appToken, err := GetInstallationToken(user.GitHubInstallationID)
		if err != nil {
			return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "Failed to generate app token: " + err.Error()})
		}

		// 2. Fetch Repos using the App Token
		repos, err := FetchRepos(appToken)
		if err != nil {
			return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "Failed to fetch repos from GitHub: " + err.Error()})
		}

		return c.JSON(repos)
	}
}

// OnboardRepos processes the selected repositories using the App Installation Token
func OnboardRepos(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		username := c.Locals("username").(string)
		var req OnboardRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "Invalid body"})
		}

		ctx := c.Context()

		query := `FOR u IN users FILTER u.username == @username LIMIT 1 RETURN u`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{"username": username},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Database error"})
		}
		defer cursor.Close()

		var user model.User
		if _, err := cursor.ReadDocument(ctx, &user); err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "User not found"})
		}

		if user.GitHubInstallationID == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "GitHub App not connected"})
		}

		// 1. Generate Fresh Installation Token
		appToken, err := GetInstallationToken(user.GitHubInstallationID)
		if err != nil {
			return c.Status(fiber.StatusBadGateway).JSON(fiber.Map{"error": "Failed to generate app token: " + err.Error()})
		}

		processed := 0
		errors := []string{}

		for _, fullRepoName := range req.Repos {
			// fullRepoName = "owner/repo"
			parts := strings.Split(fullRepoName, "/")
			if len(parts) != 2 {
				continue
			}
			owner, repo := parts[0], parts[1]

			// 2. Fetch Releases using App Token
			releases, err := FetchReleases(appToken, owner, repo)
			if err == nil {
				for _, r := range releases {
					releaseModel := model.ProjectRelease{
						Name:           fullRepoName,
						Version:        r.TagName,
						ObjType:        "ProjectRelease",
						GitRepo:        fullRepoName,
						GitOrg:         owner,
						GitRepoProject: repo,
						BuildDate:      r.PublishedAt,
						IsPublic:       false,
						ContentSha:     r.TagName,
					}
					releaseModel.ParseAndSetNameComponents()
					releaseModel.ParseAndSetVersion()

					_, err := db.Collections["release"].CreateDocument(ctx, releaseModel)
					if err == nil {
						processed++
					}
				}
			} else {
				errors = append(errors, fmt.Sprintf("Failed to fetch releases for %s: %v", fullRepoName, err))
			}

			// 3. Fetch Workflow Runs using App Token
			runs, err := FetchWorkflowRuns(appToken, owner, repo)
			if err == nil {
				for _, run := range runs {
					if run.Conclusion == "success" {
						endpointName := "github-actions/" + fullRepoName

						endpoint := model.NewEndpoint()
						endpoint.Name = endpointName
						endpoint.EndpointType = "serverless"
						endpoint.Environment = "ci"
						endpoint.ParseAndSetNameComponents()

						db.Collections["endpoint"].CreateDocument(ctx, endpoint)

						sync := model.NewSync()
						sync.EndpointName = endpointName
						sync.ReleaseName = fullRepoName
						version := run.HeadSha
						if len(version) > 7 {
							version = version[:7]
						}
						sync.ReleaseVersion = version
						sync.SyncedAt = run.UpdatedAt

						db.Collections["sync"].CreateDocument(ctx, sync)
						processed++
					}
				}
			}
		}

		return c.JSON(fiber.Map{
			"message": fmt.Sprintf("Processed %d items via GitHub App", processed),
			"errors":  errors,
		})
	}
}
