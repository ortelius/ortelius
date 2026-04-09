// Package auth provides authentication and authorization utilities.
//
//revive:disable-next-line:var-naming
package auth

import (
	"encoding/json"
	"fmt"
	"strings"

	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/pdvd-backend/v12/database"
	"gopkg.in/yaml.v2"
)

// RBACApplyRequest represents the request body for applying RBAC config
type RBACApplyRequest struct {
	Config string `json:"config"` // YAML config as string
	DryRun bool   `json:"dry_run,omitempty"`
}

// RBACApplyResponse represents the response from applying RBAC config
type RBACApplyResponse struct {
	Success      bool     `json:"success"`
	Message      string   `json:"message"`
	UsersCreated int      `json:"users_created,omitempty"`
	UsersUpdated int      `json:"users_updated,omitempty"`
	RolesCreated int      `json:"roles_created,omitempty"`
	Errors       []string `json:"errors,omitempty"`
}

// HandleRBACApply handles POST /api/v1/rbac/apply
func HandleRBACApply(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		// Note: Auth checks are handled by middleware (RequireAuth, RequireRole)

		var req RBACApplyRequest
		contentType := c.Get("Content-Type")

		switch {
		case strings.Contains(contentType, "application/json"):
			if err := c.Bind().Body(&req); err != nil {
				return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
					"success": false,
					"error":   "Invalid JSON: " + err.Error(),
				})
			}
		case strings.Contains(contentType, "application/x-yaml") || strings.Contains(contentType, "text/yaml"):
			req.Config = string(c.Body())
		default:
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"error":   "Content-Type must be application/json or application/x-yaml",
			})
		}

		if req.Config == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"error":   "RBAC config cannot be empty",
			})
		}

		var config RBACConfig
		if err := yaml.Unmarshal([]byte(req.Config), &config); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"error":   "Invalid YAML: " + err.Error(),
			})
		}

		if err := ValidateRBACConfig(&config); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"error":   "Invalid RBAC config: " + err.Error(),
			})
		}

		if req.DryRun {
			return c.JSON(RBACApplyResponse{
				Success: true,
				Message: fmt.Sprintf("Dry run successful. Config is valid. Would create/update %d users and %d roles.",
					len(config.Users), len(config.Roles)),
			})
		}

		usersCreated, usersUpdated, rolesCreated, errs := ApplyRBACConfig(db, &config)

		response := RBACApplyResponse{
			Success:      len(errs) == 0,
			UsersCreated: usersCreated,
			UsersUpdated: usersUpdated,
			RolesCreated: rolesCreated,
		}

		if len(errs) > 0 {
			response.Message = "RBAC config applied with errors"
			response.Errors = make([]string, len(errs))
			for i, err := range errs {
				response.Errors[i] = err.Error()
			}
			return c.Status(fiber.StatusPartialContent).JSON(response)
		}

		response.Message = fmt.Sprintf("RBAC config applied successfully. Created %d users, updated %d users, created %d roles.",
			usersCreated, usersUpdated, rolesCreated)
		return c.Status(fiber.StatusOK).JSON(response)
	}
}

// HandleRBACValidate handles POST /api/v1/rbac/validate
func HandleRBACValidate(_ database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		// Note: Auth checks are handled by middleware (RequireAuth, RequireRole)

		body := c.Body()
		if len(body) == 0 {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"error":   "Failed to read request body or empty",
			})
		}

		// Try to parse as JSON wrapper first if content type matches, otherwise treat as raw YAML
		contentType := c.Get("Content-Type")
		var configBytes []byte

		if strings.Contains(contentType, "application/json") {
			var req map[string]string
			if err := json.Unmarshal(body, &req); err == nil && req["config"] != "" {
				configBytes = []byte(req["config"])
			} else {
				// Fallback or error
				configBytes = body
			}
		} else {
			configBytes = body
		}

		var config RBACConfig
		if err := yaml.Unmarshal(configBytes, &config); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"error":   "Invalid YAML: " + err.Error(),
			})
		}

		if err := ValidateRBACConfig(&config); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"success": false,
				"error":   "Invalid RBAC config: " + err.Error(),
			})
		}

		return c.JSON(RBACApplyResponse{
			Success: true,
			Message: fmt.Sprintf("Config is valid. Contains %d users and %d roles.",
				len(config.Users), len(config.Roles)),
		})
	}
}
