// Package auth provides authentication and authorization types for the REST API.
package auth

// LoginRequest defines the body for Basic Auth login
type LoginRequest struct {
	Username string `json:"username"`
	Password string `json:"password"`
}

// UserResponse defines the session info returned to the frontend
type UserResponse struct {
	Username string `json:"username"`
	Role     string `json:"role"`
}

// ForgotPasswordRequest handles the invite/reset flow
type ForgotPasswordRequest struct {
	Email string `json:"email"`
}
