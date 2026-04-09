// Package auth provides email services for user invitations.
package auth

import (
	"bytes"
	"fmt"
	"html/template"
	"net/smtp"
	"os"
)

// EmailConfig holds email service configuration
type EmailConfig struct {
	SMTPHost     string
	SMTPPort     string
	SMTPUsername string
	SMTPPassword string
	FromEmail    string
	FromName     string
	BaseURL      string // Base URL for invitation links
}

// LoadEmailConfig loads email configuration from environment
func LoadEmailConfig() *EmailConfig {
	return &EmailConfig{
		SMTPHost:     getEnv("SMTP_HOST", "smtp.gmail.com"),
		SMTPPort:     getEnv("SMTP_PORT", "587"),
		SMTPUsername: getEnv("SMTP_USERNAME", ""),
		SMTPPassword: getEnv("SMTP_PASSWORD", ""),
		FromEmail:    getEnv("SMTP_FROM_EMAIL", "noreply@pdvd.com"),
		FromName:     getEnv("SMTP_FROM_NAME", "PDVD System"),
		BaseURL:      getEnv("BASE_URL", "http://localhost:3000"),
	}
}

func getEnv(key, defaultValue string) string {
	if value := os.Getenv(key); value != "" {
		return value
	}
	return defaultValue
}

// InvitationEmailData holds data for invitation email template
type InvitationEmailData struct {
	Username       string
	Email          string
	Role           string
	InvitationLink string
	ExpiresIn      string
	SupportEmail   string
}

// SendInvitationEmail sends an invitation email to a new user
func SendInvitationEmail(config *EmailConfig, username, email, token, role string) error {
	if config.SMTPUsername == "" || config.SMTPPassword == "" {
		// Email not configured - log the invitation link instead
		invitationLink := fmt.Sprintf("%s/invitation/%s", config.BaseURL, token)
		fmt.Printf(`
════════════════════════════════════════════════════════════════
📧 EMAIL NOT CONFIGURED - INVITATION LINK
════════════════════════════════════════════════════════════════

SMTP is not configured. Send this invitation link to the user:

User:     %s
Email:    %s
Role:     %s
Link:     %s

Valid for: 48 hours

════════════════════════════════════════════════════════════════
`, username, email, role, invitationLink)
		return nil
	}

	data := InvitationEmailData{
		Username:       username,
		Email:          email,
		Role:           role,
		InvitationLink: fmt.Sprintf("%s/invitation/%s", config.BaseURL, token),
		ExpiresIn:      "48 hours",
		SupportEmail:   config.FromEmail,
	}

	subject := "You've been invited to PDVD"
	body, err := renderInvitationEmail(data)
	if err != nil {
		return fmt.Errorf("failed to render email template: %w", err)
	}

	return sendEmail(config, email, subject, body)
}

// SendPasswordResetEmail sends a password reset email
func SendPasswordResetEmail(config *EmailConfig, email, token string) error {
	if config.SMTPUsername == "" || config.SMTPPassword == "" {
		resetLink := fmt.Sprintf("%s/reset-password/%s", config.BaseURL, token)
		fmt.Printf(`
════════════════════════════════════════════════════════════════
📧 PASSWORD RESET LINK
════════════════════════════════════════════════════════════════

Email:    %s
Link:     %s

Valid for: 1 hour

════════════════════════════════════════════════════════════════
`, email, resetLink)
		return nil
	}

	resetLink := fmt.Sprintf("%s/reset-password/%s", config.BaseURL, token)

	subject := "Reset your PDVD password"
	body := fmt.Sprintf(`
<html>
<body style="font-family: Arial, sans-serif; max-width: 600px; margin: 0 auto;">
	<h2>Password Reset Request</h2>
	<p>You requested to reset your PDVD password.</p>
	<p>Click the link below to reset your password:</p>
	<p><a href="%s" style="background-color: #4CAF50; color: white; padding: 10px 20px; text-decoration: none; border-radius: 4px; display: inline-block;">Reset Password</a></p>
	<p>This link will expire in 1 hour.</p>
	<p>If you didn't request this, please ignore this email.</p>
	<hr>
	<p style="color: #666; font-size: 12px;">PDVD System</p>
</body>
</html>
`, resetLink)

	return sendEmail(config, email, subject, body)
}

// renderInvitationEmail renders the invitation email template
func renderInvitationEmail(data InvitationEmailData) (string, error) {
	tmpl := `
<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<style>
		body { font-family: Arial, sans-serif; line-height: 1.6; color: #333; }
		.container { max-width: 600px; margin: 0 auto; padding: 20px; }
		.header { background-color: #4CAF50; color: white; padding: 20px; text-align: center; }
		.content { padding: 30px; background-color: #f9f9f9; }
		.button { 
			display: inline-block;
			background-color: #4CAF50;
			color: white;
			padding: 12px 30px;
			text-decoration: none;
			border-radius: 4px;
			margin: 20px 0;
		}
		.info-box { 
			background-color: #e8f5e9;
			border-left: 4px solid #4CAF50;
			padding: 15px;
			margin: 20px 0;
		}
		.footer { 
			padding: 20px;
			text-align: center;
			color: #666;
			font-size: 12px;
		}
	</style>
</head>
<body>
	<div class="container">
		<div class="header">
			<h1>Welcome to PDVD!</h1>
		</div>
		
		<div class="content">
			<p>Hi <strong>{{.Username}}</strong>,</p>
			
			<p>You've been invited to join the PDVD system with the following role:</p>
			
			<div class="info-box">
				<strong>Your Role:</strong> {{.Role}}<br>
				<strong>Email:</strong> {{.Email}}
			</div>
			
			<p>To complete your account setup and set your password, please click the button below:</p>
			
			<center>
				<a href="{{.InvitationLink}}" class="button">Accept Invitation & Set Password</a>
			</center>
			
			<p><strong>Important:</strong> This invitation link will expire in {{.ExpiresIn}}.</p>
			
			<p>If you didn't expect this invitation, please ignore this email or contact your administrator.</p>
		</div>
		
		<div class="footer">
			<p>PDVD System<br>
			Questions? Contact <a href="mailto:{{.SupportEmail}}">{{.SupportEmail}}</a></p>
		</div>
	</div>
</body>
</html>
`

	t, err := template.New("invitation").Parse(tmpl)
	if err != nil {
		return "", err
	}

	var buf bytes.Buffer
	if err := t.Execute(&buf, data); err != nil {
		return "", err
	}

	return buf.String(), nil
}

// sendEmail sends an email using SMTP
func sendEmail(config *EmailConfig, to, subject, htmlBody string) error {
	auth := smtp.PlainAuth("", config.SMTPUsername, config.SMTPPassword, config.SMTPHost)

	msg := []byte(fmt.Sprintf(
		"From: %s <%s>\r\n"+
			"To: %s\r\n"+
			"Subject: %s\r\n"+
			"MIME-Version: 1.0\r\n"+
			"Content-Type: text/html; charset=UTF-8\r\n"+
			"\r\n"+
			"%s",
		config.FromName, config.FromEmail, to, subject, htmlBody,
	))

	addr := fmt.Sprintf("%s:%s", config.SMTPHost, config.SMTPPort)
	return smtp.SendMail(addr, auth, config.FromEmail, []string{to}, msg)
}
