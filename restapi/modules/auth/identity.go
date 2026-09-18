// Package auth - identity.go centralizes how an incoming SSO login (Google,
// GitHub, and later Authentik/Okta/GitLab) resolves to a local User record.
//
// This is the single place that decides "is this a returning SSO user, an
// existing local-password user signing in with SSO for the first time, or a
// brand new user" - every provider's callback handler calls into here so
// that logic doesn't get reimplemented (and drift) per provider.
package auth

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/model"
)

// ResolveSSOUser finds-or-creates-or-links the local user for an incoming
// SSO login. Resolution order:
//
//  1. Match by (provider, externalID) in linked_identities - this is a
//     returning SSO user. Nothing to do but load them.
//  2. If no match AND emailVerified is true, match by email against any
//     existing account (local password signup OR a different provider's SSO
//     account). If found, ATTACH this provider as a new linked identity on
//     that account rather than creating a second one. This is what makes
//     "signed up with password, later clicked Sign in with Google" land on
//     one account instead of two.
//  3. If still no match, provision a brand new account with this identity
//     pre-linked.
//
// emailVerified must reflect the PROVIDER's own claim (Google's
// email_verified, or GitHub's emails[].verified) - never assume true. Step 2
// is a trust decision: matching on an unverified email would let anyone take
// over an existing account by registering at an IdP that doesn't check
// ownership of the address they typed in.
func ResolveSSOUser(ctx context.Context, db database.DBConnection, provider, externalID, email string, emailVerified bool, defaultUsername string) (*model.User, error) {
	// 1. Returning SSO user - the common case after the first login.
	if user, err := getUserByLinkedIdentity(ctx, db, provider, externalID); err == nil {
		if err := activateSSOUser(ctx, db, user); err != nil {
			return nil, err
		}
		return user, nil
	}

	identity := model.LinkedIdentity{
		Provider:      provider,
		ExternalID:    externalID,
		Email:         email,
		EmailVerified: emailVerified,
		LinkedAt:      time.Now(),
	}

	// 2. Link to an existing account by verified email - covers both
	// "local password user adds Google" and "Google user adds GitHub".
	if emailVerified && email != "" {
		if existing, err := getUserByEmail(ctx, db, email); err == nil {
			if hasLinkedIdentity(existing, provider, externalID) {
				// Shouldn't happen given step 1, but guards against a race.
				if err := activateSSOUser(ctx, db, existing); err != nil {
					return nil, err
				}
				return existing, nil
			}
			existing.LinkedIdentities = append(existing.LinkedIdentities, identity)
			existing.UpdatedAt = time.Now()
			existing.IsActive = true
			existing.Status = "active"
			// A user who previously only had a password is now also an SSO
			// user; auth_provider becomes informational once linked - leave
			// it as "local" if they still have a password (PasswordHash != "")
			// so password login keeps working, and only mark "sso" if this
			// account never had one (e.g. it was itself created by a
			// different provider's first login).
			if existing.PasswordHash == "" {
				existing.AuthProvider = "sso"
			}
			if err := updateUser(ctx, db, existing); err != nil {
				return nil, fmt.Errorf("failed to link %s identity to existing account: %w", provider, err)
			}
			return existing, nil
		}
	}

	// 3. New user - provision with this identity pre-linked.
	username := uniqueUsernameFrom(ctx, db, defaultUsername)
	user := model.NewUser(username, "viewer")
	user.Email = email
	user.AuthProvider = "sso"
	user.Status = "active"
	user.IsActive = true
	user.LinkedIdentities = []model.LinkedIdentity{identity}

	if err := createUser(ctx, db, user); err != nil {
		return nil, fmt.Errorf("failed to provision user: %w", err)
	}
	return user, nil
}

// activateSSOUser marks a user active after a successful Google/GitHub/OIDC
// login (is_active=true, status="active"), persisting only if it changed.
func activateSSOUser(ctx context.Context, db database.DBConnection, user *model.User) error {
	if user.IsActive && user.Status == "active" {
		return nil
	}
	user.IsActive = true
	user.Status = "active"
	user.UpdatedAt = time.Now()
	if err := updateUser(ctx, db, user); err != nil {
		return fmt.Errorf("failed to activate user: %w", err)
	}
	return nil
}

func hasLinkedIdentity(user *model.User, provider, externalID string) bool {
	for _, li := range user.LinkedIdentities {
		if li.Provider == provider && li.ExternalID == externalID {
			return true
		}
	}
	return false
}

// uniqueUsernameFrom avoids colliding with an existing LOCAL username that
// happens to match the SSO-derived default (e.g. someone already signed up
// with username "jane" locally, unrelated to jane@gmail.com signing in via
// Google for the first time with no matching email). Falls back to
// appending the provider-qualified suffix until it's unique.
func uniqueUsernameFrom(ctx context.Context, db database.DBConnection, base string) string {
	candidate := base
	for i := 0; i < 5; i++ {
		if _, err := getUserByUsername(ctx, db, candidate); err != nil {
			return candidate // not found - free to use
		}
		candidate = fmt.Sprintf("%s-%d", strings.TrimSuffix(base, "-"), i+1)
	}
	return candidate
}
