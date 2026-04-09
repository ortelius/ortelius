// Package auth provides invitation management logic.
package auth

import (
	"context"
	"fmt"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
)

// CreateInvitation creates a new user invitation and sends email
func CreateInvitation(ctx context.Context, db database.DBConnection, emailConfig *EmailConfig, username, email, role string) (*model.Invitation, error) {
	// Generate secure token
	token, err := GenerateRandomString(32)
	if err != nil {
		return nil, fmt.Errorf("failed to generate token: %w", err)
	}

	// Create invitation
	invitation := model.NewInvitation(username, email, token, role)

	// Save to database
	_, err = db.Collections["invitations"].CreateDocument(ctx, invitation)
	if err != nil {
		return nil, fmt.Errorf("failed to save invitation: %w", err)
	}

	// Send invitation email
	if err := SendInvitationEmail(emailConfig, username, email, token, role); err != nil {
		// Log error but don't fail - invitation still created
		fmt.Printf("⚠️  Failed to send invitation email to %s: %v\n", email, err)
		fmt.Printf("   Invitation link: %s/invitation/%s\n", emailConfig.BaseURL, token)
	}

	return invitation, nil
}

// GetInvitation retrieves an invitation by token
func GetInvitation(ctx context.Context, db database.DBConnection, token string) (*model.Invitation, error) {
	query := `FOR i IN invitations FILTER i.token == @token RETURN i`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"token": token},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	if !cursor.HasMore() {
		return nil, fmt.Errorf("invitation not found")
	}

	var invitation model.Invitation
	_, err = cursor.ReadDocument(ctx, &invitation)
	if err != nil {
		return nil, err
	}

	return &invitation, nil
}

// AcceptInvitation accepts an invitation and activates the user
func AcceptInvitation(ctx context.Context, db database.DBConnection, token, password string) (*model.User, error) {
	// Get invitation
	invitation, err := GetInvitation(ctx, db, token)
	if err != nil {
		return nil, fmt.Errorf("invalid invitation: %w", err)
	}

	// Validate invitation
	if invitation.IsExpired() {
		return nil, fmt.Errorf("invitation has expired")
	}

	if invitation.IsAccepted() {
		return nil, fmt.Errorf("invitation already accepted")
	}

	// Get user
	user, err := getUserByUsername(ctx, db, invitation.Username)
	if err != nil {
		return nil, fmt.Errorf("user not found: %w", err)
	}

	// Validate user status
	if user.Status != "pending" {
		return nil, fmt.Errorf("user is not in pending status")
	}

	// Hash password
	passwordHash, err := HashPassword(password)
	if err != nil {
		return nil, fmt.Errorf("failed to hash password: %w", err)
	}

	// Update user
	// FIXED: Added is_active: true to the update payload
	now := time.Now()
	query := `
		FOR u IN users 
		FILTER u.username == @username 
		UPDATE u WITH {
			password_hash: @password_hash,
			status: "active",
			is_active: true,
			updated_at: @updated_at
		} IN users
		RETURN NEW
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"username":      invitation.Username,
			"password_hash": passwordHash,
			"updated_at":    now,
		},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var updatedUser model.User
	if cursor.HasMore() {
		_, err = cursor.ReadDocument(ctx, &updatedUser)
		if err != nil {
			return nil, err
		}
	}

	// Mark invitation as accepted
	acceptedAt := now
	updateInvitationQuery := `
		FOR i IN invitations 
		FILTER i.token == @token 
		UPDATE i WITH {accepted_at: @accepted_at} IN invitations
	`
	_, err = db.Database.Query(ctx, updateInvitationQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"token":       token,
			"accepted_at": acceptedAt,
		},
	})
	if err != nil {
		fmt.Printf("⚠️  Failed to mark invitation as accepted: %v\n", err)
	}

	fmt.Printf("✅ User %s accepted invitation and activated account\n", invitation.Username)

	return &updatedUser, nil
}

// ResendInvitation resends an invitation email
func ResendInvitation(ctx context.Context, db database.DBConnection, emailConfig *EmailConfig, token string) error {
	invitation, err := GetInvitation(ctx, db, token)
	if err != nil {
		return fmt.Errorf("invalid invitation: %w", err)
	}

	if invitation.IsAccepted() {
		return fmt.Errorf("invitation already accepted")
	}

	// Update resend count and expiry
	now := time.Now()
	newExpiry := now.Add(48 * time.Hour)

	query := `
		FOR i IN invitations 
		FILTER i.token == @token 
		UPDATE i WITH {
			expires_at: @expires_at,
			resend_count: i.resend_count + 1
		} IN invitations
	`
	_, err = db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"token":      token,
			"expires_at": newExpiry,
		},
	})
	if err != nil {
		return err
	}

	// Resend email
	if err := SendInvitationEmail(emailConfig, invitation.Username, invitation.Email, token, invitation.Role); err != nil {
		return fmt.Errorf("failed to resend invitation email: %w", err)
	}

	fmt.Printf("✅ Resent invitation to %s (count: %d)\n", invitation.Email, invitation.ResendCount+1)

	return nil
}

// CleanupExpiredInvitations removes expired invitations
func CleanupExpiredInvitations(ctx context.Context, db database.DBConnection) (int, error) {
	query := `
		FOR i IN invitations 
		FILTER i.expires_at < @now AND i.accepted_at == null
		REMOVE i IN invitations
		RETURN OLD
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"now": time.Now()},
	})
	if err != nil {
		return 0, err
	}
	defer cursor.Close()

	count := 0
	for cursor.HasMore() {
		var inv model.Invitation
		if _, err := cursor.ReadDocument(ctx, &inv); err == nil {
			count++
		}
	}

	return count, nil
}
