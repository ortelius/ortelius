// restapi/modules/auth/rbac_fiber_handlers.go
package auth

import (
	"context"
	"fmt"
	"io"
	"os"

	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
	"github.com/ortelius/ortelius/v12/restapi/modules/rbac"
	"gopkg.in/yaml.v2"
)

// MakeUserCreator returns a rbac.UserCreator function
func MakeUserCreator(db database.DBConnection, emailConfig *EmailConfig) rbac.UserCreator {
	return func(ctx context.Context, dbConn database.DBConnection, username, email, role string, orgs []string) (string, error) {
		user := model.NewUser(username, role)
		user.Email = email
		user.Orgs = orgs
		user.IsActive = false
		user.Status = "pending"
		user.AuthProvider = "local"

		randomPass, err := GenerateSecureToken(32)
		if err != nil {
			return "", fmt.Errorf("failed to generate password: %w", err)
		}
		hash, err := HashPassword(randomPass)
		if err != nil {
			return "", fmt.Errorf("failed to hash password: %w", err)
		}
		user.PasswordHash = hash

		if err := rbac.CreateUser(ctx, dbConn, user); err != nil {
			return "", fmt.Errorf("failed to create user: %w", err)
		}

		// No email known at this point (RBAC config never carries it - see
		// rbac.RBACUser). The caller is responsible for setting email and
		// sending the real invitation through a non-git-tracked path (e.g.
		// the Signup handler does this immediately after this returns, using
		// the email from the signup request directly).
		if email == "" {
			return "", nil
		}

		inv, err := CreateInvitation(ctx, dbConn, emailConfig, username, email, role)
		if err != nil {
			return "", nil
		}
		return inv.Token, nil
	}
}

// ApplyRBACFromBody applies RBAC config from request body
func ApplyRBACFromBody(db database.DBConnection, emailConfig *EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		yamlContent := string(c.Body())
		if yamlContent == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "RBAC config cannot be empty"})
		}

		config, err := rbac.LoadRBACConfig(yamlContent)
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": err.Error()})
		}

		result, err := rbac.ApplyRBAC(db, config, MakeUserCreator(db, emailConfig))
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": err.Error()})
		}

		return c.JSON(fiber.Map{"success": true, "result": result})
	}
}

// ValidateRBAC validates RBAC config without applying changes
func ValidateRBAC(_ database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		yamlContent := string(c.Body())
		if yamlContent == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"valid": false, "error": "RBAC config cannot be empty"})
		}

		config, err := rbac.LoadRBACConfig(yamlContent)
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"valid": false, "error": err.Error()})
		}

		return c.JSON(fiber.Map{
			"valid": true,
			"summary": fiber.Map{
				"orgs":  len(config.Orgs),
				"users": len(config.Users),
			},
		})
	}
}

// ApplyRBACFromUpload applies RBAC config from uploaded file
func ApplyRBACFromUpload(db database.DBConnection, emailConfig *EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		file, err := c.FormFile("file")
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "No file uploaded"})
		}
		openedFile, err := file.Open()
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to open file"})
		}
		defer openedFile.Close()

		yamlContent, err := io.ReadAll(openedFile)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to read file"})
		}

		config, err := rbac.LoadRBACConfig(string(yamlContent))
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": err.Error()})
		}

		result, err := rbac.ApplyRBAC(db, config, MakeUserCreator(db, emailConfig))
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": err.Error()})
		}

		return c.JSON(fiber.Map{"success": true, "result": result})
	}
}

// ApplyRBACFromFile applies RBAC config from filesystem path
func ApplyRBACFromFile(db database.DBConnection, emailConfig *EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		var req struct {
			FilePath string `json:"file_path"`
		}
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "Invalid request body"})
		}
		if req.FilePath == "" {
			req.FilePath = "/etc/ortelius/rbac.yaml"
		}

		yamlContent, err := os.ReadFile(req.FilePath)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to read file"})
		}

		config, err := rbac.LoadRBACConfig(string(yamlContent))
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": err.Error()})
		}

		result, err := rbac.ApplyRBAC(db, config, MakeUserCreator(db, emailConfig))
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": err.Error()})
		}

		return c.JSON(fiber.Map{"success": true, "result": result})
	}
}

// GetRBACConfig exports current RBAC configuration from DB to YAML
func GetRBACConfig(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		config, err := rbac.ExportRBACConfig(c.Context(), db)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to export config"})
		}
		yamlData, err := yaml.Marshal(config)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Failed to marshal config"})
		}
		c.Set("Content-Type", "application/x-yaml")
		return c.Send(yamlData)
	}
}
