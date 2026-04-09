// Package auth provides authentication and authorization utilities.
//
//revive:disable-next-line:var-naming
package auth

import (
	"crypto/rand"
	"encoding/base64"
	"fmt"
	"time"

	"github.com/golang-jwt/jwt/v5"
	"golang.org/x/crypto/bcrypt"
)

// JWT secret key - should be loaded from environment variable in production
var jwtSecret = []byte("your-secret-key-change-this-in-production")

// ============================================================================
// PASSWORD HASHING
// ============================================================================

// HashPassword generates a bcrypt hash of the password
func HashPassword(password string) (string, error) {
	bytes, err := bcrypt.GenerateFromPassword([]byte(password), bcrypt.DefaultCost)
	return string(bytes), err
}

// CheckPasswordHash compares a password with a hash
func CheckPasswordHash(password, hash string) bool {
	err := bcrypt.CompareHashAndPassword([]byte(hash), []byte(password))
	return err == nil
}

// ============================================================================
// JWT TOKEN MANAGEMENT
// ============================================================================

// Claims represents JWT claims
// UPDATED: Kept only Username (Subject) to minimize payload size.
// Role and Orgs are now fetched from the database in the middleware.
type Claims struct {
	Username string `json:"username"`
	jwt.RegisteredClaims
}

// GenerateJWT generates a JWT token for a user
// UPDATED: Removed role and orgs arguments
func GenerateJWT(username string) (string, error) {
	expirationTime := time.Now().Add(24 * time.Hour) // 24 hour expiry

	claims := &Claims{
		Username: username,
		RegisteredClaims: jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(expirationTime),
			IssuedAt:  jwt.NewNumericDate(time.Now()),
			Issuer:    "pdvd-backend",
			Subject:   username,
		},
	}

	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	tokenString, err := token.SignedString(jwtSecret)
	if err != nil {
		return "", err
	}

	return tokenString, nil
}

// ValidateJWT validates a JWT token and returns the claims
func ValidateJWT(tokenString string) (*Claims, error) {
	claims := &Claims{}

	token, err := jwt.ParseWithClaims(tokenString, claims, func(token *jwt.Token) (interface{}, error) {
		// Verify signing method
		if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
			return nil, fmt.Errorf("unexpected signing method: %v", token.Header["alg"])
		}
		return jwtSecret, nil
	})

	if err != nil {
		return nil, err
	}

	if !token.Valid {
		return nil, fmt.Errorf("invalid token")
	}

	return claims, nil
}

// RefreshJWT generates a new token with extended expiration for an existing valid token
func RefreshJWT(oldTokenString string) (string, error) {
	claims, err := ValidateJWT(oldTokenString)
	if err != nil {
		return "", fmt.Errorf("cannot refresh invalid token: %w", err)
	}

	// Generate new token with same username but fresh expiration
	return GenerateJWT(claims.Username)
}

// ============================================================================
// TOKEN GENERATION
// ============================================================================

// GenerateSecureToken generates a cryptographically secure random token
// Used for invitation tokens, password reset tokens, etc.
func GenerateSecureToken(length int) (string, error) {
	if length <= 0 {
		length = 32 // Default to 32 bytes
	}

	bytes := make([]byte, length)
	if _, err := rand.Read(bytes); err != nil {
		return "", err
	}

	return base64.URLEncoding.EncodeToString(bytes), nil
}

// ============================================================================
// CONFIGURATION
// ============================================================================

// SetJWTSecret sets the JWT secret (call this on startup with env var)
func SetJWTSecret(secret string) {
	if secret == "" {
		panic("JWT secret cannot be empty")
	}
	jwtSecret = []byte(secret)
}

// GetJWTExpirationTime returns the configured JWT expiration duration
func GetJWTExpirationTime() time.Duration {
	return 24 * time.Hour
}

// ============================================================================
// VALIDATION HELPERS
// ============================================================================

// IsStrongPassword validates password strength
// Requires at least 8 characters
func IsStrongPassword(password string) bool {
	return len(password) >= 8
}

// ValidatePasswordStrength validates password meets security requirements
// Returns error with specific requirement that failed
func ValidatePasswordStrength(password string) error {
	if len(password) < 8 {
		return fmt.Errorf("password must be at least 8 characters long")
	}
	return nil
}
