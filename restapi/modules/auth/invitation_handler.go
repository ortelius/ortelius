// Package auth provides invitation management handlers for user onboarding.
package auth

import (
	"fmt"
	"net/url"

	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/pdvd-backend/v12/database"
)

// Helper: cleanToken aggressively decodes URL-encoded strings
func cleanToken(raw string) string {
	decoded, err := url.QueryUnescape(raw)
	if err == nil {
		return decoded
	}
	return raw
}

// GetInvitationHandler handles GET /api/v1/invitation/:token
func GetInvitationHandler(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		rawToken := c.Params("token")
		token := cleanToken(rawToken)

		ctx := c.Context()
		invitation, err := GetInvitation(ctx, db, token)
		if err != nil {
			// Fallback: Try without cleaning if first attempt fails
			if token != rawToken {
				invitation, err = GetInvitation(ctx, db, rawToken)
			}
			if err != nil {
				return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "Invalid or expired invitation"})
			}
		}

		if invitation.IsExpired() || invitation.IsAccepted() {
			return c.Status(fiber.StatusGone).JSON(fiber.Map{"error": "Invitation no longer valid"})
		}

		return c.JSON(fiber.Map{
			"username": invitation.Username,
			"email":    invitation.Email,
			"role":     invitation.Role,
		})
	}
}

// AcceptInvitationRequest defines the activation body
type AcceptInvitationRequest struct {
	Password        string `json:"password"`
	PasswordConfirm string `json:"password_confirm"`
	Token           string `json:"token"`
}

// AcceptInvitationHandler handles activation and immediate login
func AcceptInvitationHandler(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		var req AcceptInvitationRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "Invalid body"})
		}

		if req.Password != req.PasswordConfirm {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "Passwords mismatch"})
		}

		// Determine token: Body > URL
		token := req.Token
		if token == "" {
			token = c.Params("token")
		}

		// Clean the token (converts %3D to =, etc.)
		token = cleanToken(token)

		fmt.Printf("Processing invitation acceptance for token: %s\n", token)

		ctx := c.Context()
		user, err := AcceptInvitation(ctx, db, token, req.Password)
		if err != nil {
			fmt.Printf("AcceptInvitation error: %v\n", err)
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": err.Error()})
		}

		// Immediate login after successful activation
		// UPDATED: Only pass username to GenerateJWT
		jwtToken, err := GenerateJWT(user.Username)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "Login failed"})
		}

		SetAuthCookie(c, jwtToken)

		return c.JSON(fiber.Map{
			"message":  "Account activated. You are now logged in.",
			"username": user.Username,
		})
	}
}

// ResendInvitationHandler handles POST /api/v1/invitation/:token/resend
func ResendInvitationHandler(db database.DBConnection, emailConfig *EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		token := cleanToken(c.Params("token"))
		if token == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "Invitation token is required",
			})
		}

		ctx := c.Context()
		err := ResendInvitation(ctx, db, emailConfig, token)
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": err.Error(),
			})
		}

		return c.JSON(fiber.Map{
			"message": "Invitation email resent successfully",
		})
	}
}

// ListPendingInvitationsHandler handles GET /api/v1/admin/invitations (admin only)
func ListPendingInvitationsHandler(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		ctx := c.Context()

		query := `
			FOR i IN invitations 
			FILTER i.accepted_at == null
			SORT i.created_at DESC
			RETURN {
				username: i.username,
				email: i.email,
				role: i.role,
				created_at: i.created_at,
				expires_at: i.expires_at,
				is_expired: i.expires_at < DATE_NOW(),
				resend_count: i.resend_count
			}
		`
		cursor, err := db.Database.Query(ctx, query, nil)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": "Failed to fetch invitations",
			})
		}
		defer cursor.Close()

		invitations := []interface{}{}
		for cursor.HasMore() {
			var inv interface{}
			if _, err := cursor.ReadDocument(ctx, &inv); err == nil {
				invitations = append(invitations, inv)
			}
		}

		return c.JSON(fiber.Map{
			"invitations": invitations,
			"count":       len(invitations),
		})
	}
}
