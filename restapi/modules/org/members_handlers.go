// Package org — org member management handlers.
//
// These endpoints let an org owner invite, remove, and change the role of
// members from the UI while keeping the RBAC git repo as the source of
// truth. Every mutation follows the same GitOps sequence as the Signup and
// TrackRepo flows:
//
//  1. gitops.*        — commit the change to rbac.yaml and push
//  2. rbac.ApplyRBAC  — apply the updated YAML locally
//  3. (invite only)   — set the email on the DB record and send the
//     invitation. Email is never written to rbac.yaml (PII stays out of
//     git history); it travels only through this request and the database.
//
// Because ApplyRBAC deactivates users missing from the YAML, a DB-only
// invite would be reverted on the next webhook sync — the YAML commit is
// not optional, it is the mechanism.
package org

import (
	"context"
	"errors"
	"fmt"
	"regexp"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
	"github.com/ortelius/ortelius/v12/restapi/modules/auth"
	"github.com/ortelius/ortelius/v12/restapi/modules/gitops"
	"github.com/ortelius/ortelius/v12/restapi/modules/rbac"
)

var (
	usernameRe = regexp.MustCompile(`^[a-zA-Z0-9][a-zA-Z0-9._-]{1,62}[a-zA-Z0-9]$`)
	emailRe    = regexp.MustCompile(`^[^\s@]+@[^\s@]+\.[^\s@]+$`)
)

// memberView is the wire representation of an org member.
type memberView struct {
	Username  string    `json:"username"`
	Email     string    `json:"email"`
	Role      string    `json:"role"`
	Status    string    `json:"status"`
	IsActive  bool      `json:"is_active"`
	CreatedAt time.Time `json:"created_at"`
}

// invitationView is the wire representation of a pending invitation.
// Token is included only for owners/admins so the UI can offer
// "copy invite link" and "resend".
type invitationView struct {
	Username    string    `json:"username"`
	Email       string    `json:"email"`
	Role        string    `json:"role"`
	ExpiresAt   time.Time `json:"expires_at"`
	ResendCount int       `json:"resend_count"`
	Token       string    `json:"token,omitempty"`
}

// requesterContext pulls the authenticated user's identity out of the
// request locals set by auth.RequireAuth and validates org membership.
func requesterContext(c fiber.Ctx, orgName string) (username, role string, ok bool) {
	username, _ = c.Locals("username").(string)
	role, _ = c.Locals("role").(string)
	userOrgs, orgsOk := c.Locals("orgs").([]string)
	if username == "" || !orgsOk || !containsOrg(userOrgs, orgName) {
		return username, role, false
	}
	return username, role, true
}

func isOwnerRole(role string) bool {
	return role == "owner" || role == "admin"
}

// ListOrgMembers handles GET /api/v1/orgs/:org/members
// Visible to any member of the org. Returns active/pending members and,
// for owners/admins, pending invitations with their tokens.
func ListOrgMembers(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		if orgName == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org name is required"})
		}

		_, requesterRole, ok := requesterContext(c, orgName)
		if !ok {
			return c.Status(fiber.StatusForbidden).JSON(fiber.Map{"error": "access denied"})
		}

		ctx := c.Context()
		if _, err := rbac.GetOrgByName(ctx, db, orgName); err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found"})
		}

		members, err := listMembersForOrg(ctx, db, orgName)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to load members"})
		}

		usernames := make([]string, 0, len(members))
		for _, m := range members {
			usernames = append(usernames, m.Username)
		}

		invitations, err := listPendingInvitations(ctx, db, usernames, isOwnerRole(requesterRole))
		if err != nil {
			// Non-fatal — members list is still useful.
			invitations = []invitationView{}
		}

		return c.JSON(fiber.Map{
			"org":         orgName,
			"members":     members,
			"invitations": invitations,
		})
	}
}

// InviteOrgMemberRequest is the body for POST /orgs/:org/members.
type InviteOrgMemberRequest struct {
	Username string `json:"username"`
	Email    string `json:"email"`
	Role     string `json:"role"` // owner | admin | editor | viewer (default viewer)
}

// InviteOrgMember handles POST /api/v1/orgs/:org/members (owner/admin only).
//
// For a new user it commits to rbac.yaml, lets ApplyRBAC create the pending
// account, sets the email on the DB record, and sends the invitation email.
// For an existing user it commits to rbac.yaml and ApplyRBAC grants org
// membership — no invitation is needed because the account already exists.
func InviteOrgMember(db database.DBConnection, emailConfig *auth.EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		if orgName == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org name is required"})
		}

		requester, _, ok := requesterContext(c, orgName)
		if !ok {
			return c.Status(fiber.StatusForbidden).JSON(fiber.Map{"error": "access denied"})
		}

		var req InviteOrgMemberRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "invalid request body"})
		}

		req.Username = strings.TrimSpace(req.Username)
		req.Email = strings.TrimSpace(strings.ToLower(req.Email))
		req.Role = strings.ToLower(strings.TrimSpace(req.Role))
		if req.Role == "" {
			req.Role = "viewer"
		}

		if !usernameRe.MatchString(req.Username) {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "username must be 3-64 chars: letters, digits, dot, dash, underscore",
			})
		}
		if !gitops.IsValidMemberRole(req.Role) {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "role must be one of: owner, admin, editor, viewer",
			})
		}

		ctx := c.Context()
		if _, err := rbac.GetOrgByName(ctx, db, orgName); err != nil {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found"})
		}

		existingUser, lookupErr := auth.LoadUserByUsername(ctx, db, req.Username)
		userExists := lookupErr == nil && existingUser != nil

		if userExists {
			if containsOrg(existingUser.Orgs, orgName) {
				return c.Status(fiber.StatusConflict).JSON(fiber.Map{
					"error": fmt.Sprintf("%s is already a member of %s", req.Username, orgName),
				})
			}
		} else {
			// A brand-new account needs an email for the invitation.
			if !emailRe.MatchString(req.Email) {
				return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "a valid email is required to invite a new user"})
			}
			inUse, err := emailInUse(ctx, db, req.Email)
			if err != nil {
				return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to validate email"})
			}
			if inUse {
				return c.Status(fiber.StatusConflict).JSON(fiber.Map{"error": "email is already in use by another account"})
			}
		}

		// 1. GitOps: commit membership to rbac.yaml. Email is intentionally
		//    NOT passed — the YAML never carries PII.
		updatedYaml, err := gitops.AddOrgMember(orgName, req.Username, req.Role, requester)
		if err != nil {
			return gitopsErrorResponse(c, err)
		}

		// 2. Apply the updated config locally (idempotent — the RBAC webhook
		//    triggered by our own push will apply the same state again).
		config, err := rbac.LoadRBACConfig(updatedYaml)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "internal configuration error"})
		}
		if _, err := rbac.ApplyRBAC(db, config, auth.MakeUserCreator(db, emailConfig)); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to apply membership change"})
		}

		if userExists {
			return c.Status(fiber.StatusOK).JSON(fiber.Map{
				"success":  true,
				"invited":  false,
				"message":  fmt.Sprintf("%s added to %s as %s", req.Username, orgName, req.Role),
				"username": req.Username,
				"role":     req.Role,
			})
		}

		// 3. New user: set the email directly on the DB record (never via the
		//    YAML) and send the real invitation — same as the Signup handler.
		newUser, err := auth.LoadUserByUsername(ctx, db, req.Username)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "user was created but could not be loaded"})
		}
		if err := setUserEmail(ctx, db, req.Username, req.Email); err != nil {
			fmt.Printf("WARNING: failed to set email for invited user %s: %v\n", req.Username, err)
		}

		invitation, err := auth.CreateInvitation(ctx, db, emailConfig, req.Username, req.Email, newUser.Role)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": "user was created but the invitation could not be sent — use resend from the members list",
			})
		}

		return c.Status(fiber.StatusCreated).JSON(fiber.Map{
			"success":  true,
			"invited":  true,
			"message":  fmt.Sprintf("Invitation sent to %s", req.Email),
			"username": req.Username,
			"role":     req.Role,
			"invitation": invitationView{
				Username:    invitation.Username,
				Email:       invitation.Email,
				Role:        invitation.Role,
				ExpiresAt:   invitation.ExpiresAt,
				ResendCount: invitation.ResendCount,
				Token:       invitation.Token,
			},
		})
	}
}

// UpdateOrgMemberRoleRequest is the body for PATCH /orgs/:org/members/:username.
type UpdateOrgMemberRoleRequest struct {
	Role string `json:"role"`
}

// UpdateOrgMemberRole handles PATCH /api/v1/orgs/:org/members/:username
// (owner/admin only). Commits the role change to rbac.yaml and applies it.
func UpdateOrgMemberRole(db database.DBConnection, emailConfig *auth.EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		target := strings.TrimSpace(c.Params("username"))
		if orgName == "" || target == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org and username are required"})
		}

		requester, _, ok := requesterContext(c, orgName)
		if !ok {
			return c.Status(fiber.StatusForbidden).JSON(fiber.Map{"error": "access denied"})
		}

		var req UpdateOrgMemberRoleRequest
		if err := c.Bind().Body(&req); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "invalid request body"})
		}
		role := strings.ToLower(strings.TrimSpace(req.Role))
		if !gitops.IsValidMemberRole(role) {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "role must be one of: owner, admin, editor, viewer",
			})
		}

		updatedYaml, err := gitops.SetOrgMemberRole(orgName, target, role, requester)
		if err != nil {
			return gitopsErrorResponse(c, err)
		}

		config, err := rbac.LoadRBACConfig(updatedYaml)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "internal configuration error"})
		}
		if _, err := rbac.ApplyRBAC(db, config, auth.MakeUserCreator(db, emailConfig)); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to apply role change"})
		}

		return c.JSON(fiber.Map{
			"success":  true,
			"message":  fmt.Sprintf("%s is now %s in %s", target, role, orgName),
			"username": target,
			"role":     role,
		})
	}
}

// RemoveOrgMember handles DELETE /api/v1/orgs/:org/members/:username
// (owner/admin only). Commits the removal to rbac.yaml and applies it.
// If the user belongs to no other org, ApplyRBAC deactivates the account
// and any pending invitations for it are cleaned up.
func RemoveOrgMember(db database.DBConnection, emailConfig *auth.EmailConfig) fiber.Handler {
	return func(c fiber.Ctx) error {
		orgName := strings.ToLower(strings.TrimSpace(c.Params("org")))
		target := strings.TrimSpace(c.Params("username"))
		if orgName == "" || target == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "org and username are required"})
		}

		requester, _, ok := requesterContext(c, orgName)
		if !ok {
			return c.Status(fiber.StatusForbidden).JSON(fiber.Map{"error": "access denied"})
		}

		updatedYaml, err := gitops.RemoveOrgMember(orgName, target, requester)
		if err != nil {
			return gitopsErrorResponse(c, err)
		}

		config, err := rbac.LoadRBACConfig(updatedYaml)
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "internal configuration error"})
		}
		if _, err := rbac.ApplyRBAC(db, config, auth.MakeUserCreator(db, emailConfig)); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to apply membership change"})
		}

		// If the user no longer exists in the YAML at all, their pending
		// invitations are dead links — clean them up. Best-effort.
		stillInConfig := false
		for _, u := range config.Users {
			if u.Username == target {
				stillInConfig = true
				break
			}
		}
		if !stillInConfig {
			if err := deletePendingInvitations(c.Context(), db, target); err != nil {
				fmt.Printf("WARNING: failed to clean up invitations for %s: %v\n", target, err)
			}
		}

		return c.JSON(fiber.Map{
			"success":  true,
			"message":  fmt.Sprintf("%s removed from %s", target, orgName),
			"username": target,
		})
	}
}

// gitopsErrorResponse maps gitops sentinel errors to HTTP statuses.
func gitopsErrorResponse(c fiber.Ctx, err error) error {
	switch {
	case errors.Is(err, gitops.ErrOrgNotFound):
		return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "org not found in RBAC configuration"})
	case errors.Is(err, gitops.ErrAlreadyMember):
		return c.Status(fiber.StatusConflict).JSON(fiber.Map{"error": "user is already a member of this org"})
	case errors.Is(err, gitops.ErrNotAMember):
		return c.Status(fiber.StatusNotFound).JSON(fiber.Map{"error": "user is not a member of this org"})
	case errors.Is(err, gitops.ErrLastOwner):
		return c.Status(fiber.StatusConflict).JSON(fiber.Map{"error": "cannot remove or demote the last owner of the org"})
	case errors.Is(err, gitops.ErrInvalidRole):
		return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{"error": "invalid role"})
	default:
		fmt.Printf("GitOps member operation error: %v\n", err)
		return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{"error": "failed to update RBAC repository — please try again"})
	}
}

// ── database helpers ─────────────────────────────────────────────────────────

func listMembersForOrg(ctx context.Context, db database.DBConnection, orgName string) ([]memberView, error) {
	query := `
		FOR u IN users
			FILTER @org IN u.orgs
			SORT u.username ASC
			RETURN u
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"org": orgName},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	members := []memberView{}
	for cursor.HasMore() {
		var u model.User
		if _, err := cursor.ReadDocument(ctx, &u); err != nil {
			continue
		}
		members = append(members, memberView{
			Username:  u.Username,
			Email:     u.Email,
			Role:      u.Role,
			Status:    u.Status,
			IsActive:  u.IsActive,
			CreatedAt: u.CreatedAt,
		})
	}
	return members, nil
}

func listPendingInvitations(ctx context.Context, db database.DBConnection, usernames []string, includeToken bool) ([]invitationView, error) {
	if len(usernames) == 0 {
		return []invitationView{}, nil
	}
	query := `
		FOR i IN invitations
			FILTER i.username IN @usernames AND i.accepted_at == null
			SORT i.created_at DESC
			RETURN i
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"usernames": usernames},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	invitations := []invitationView{}
	seen := map[string]bool{}
	for cursor.HasMore() {
		var inv model.Invitation
		if _, err := cursor.ReadDocument(ctx, &inv); err != nil {
			continue
		}
		// Only the most recent pending invitation per user.
		if seen[inv.Username] {
			continue
		}
		seen[inv.Username] = true

		view := invitationView{
			Username:    inv.Username,
			Email:       inv.Email,
			Role:        inv.Role,
			ExpiresAt:   inv.ExpiresAt,
			ResendCount: inv.ResendCount,
		}
		if includeToken {
			view.Token = inv.Token
		}
		invitations = append(invitations, view)
	}
	return invitations, nil
}

// setUserEmail writes only the email field on a user record. Deliberately a
// narrow update (same reasoning as auth.SaveFavoriteOrgs) so nothing else on
// the record is disturbed.
func setUserEmail(ctx context.Context, db database.DBConnection, username, email string) error {
	query := `
		FOR u IN users
			FILTER u.username == @username
			UPDATE u WITH { email: @email, updated_at: @now } IN users
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"username": username,
			"email":    email,
			"now":      time.Now(),
		},
	})
	return err
}

func emailInUse(ctx context.Context, db database.DBConnection, email string) (bool, error) {
	query := `
		FOR u IN users
			FILTER LOWER(u.email) == LOWER(@email)
			LIMIT 1
			RETURN u.username
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"email": email},
	})
	if err != nil {
		return false, err
	}
	defer cursor.Close()
	return cursor.HasMore(), nil
}

func deletePendingInvitations(ctx context.Context, db database.DBConnection, username string) error {
	query := `
		FOR i IN invitations
			FILTER i.username == @username AND i.accepted_at == null
			REMOVE i IN invitations
	`
	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"username": username},
	})
	return err
}
