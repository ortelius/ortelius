// Package util provides cryptographic utilities for secure token storage.
package util

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/base64"
	"fmt"
	"io"
	"os"
)

const encPrefix = "enc:v1:"

// getEncryptionKey retrieves and validates the encryption key from the environment.
func getEncryptionKey() ([]byte, error) {
	key := os.Getenv("TOKEN_ENCRYPTION_KEY")
	if key == "" {
		return nil, fmt.Errorf("TOKEN_ENCRYPTION_KEY environment variable is not set")
	}
	keyBytes, err := base64.StdEncoding.DecodeString(key)
	if err != nil {
		return nil, fmt.Errorf("TOKEN_ENCRYPTION_KEY must be a base64-encoded 32-byte key: %w", err)
	}
	if len(keyBytes) != 32 {
		return nil, fmt.Errorf("TOKEN_ENCRYPTION_KEY must decode to exactly 32 bytes, got %d", len(keyBytes))
	}
	return keyBytes, nil
}

// EncryptToken encrypts a plaintext PAT using AES-256-GCM.
// The result is prefixed with "enc:v1:" and base64-encoded for safe storage.
// The encryption key is read from the TOKEN_ENCRYPTION_KEY environment variable,
// which must be a base64-encoded 32-byte key.
func EncryptToken(plaintext string) (string, error) {
	if plaintext == "" {
		return "", nil
	}

	keyBytes, err := getEncryptionKey()
	if err != nil {
		return "", err
	}

	block, err := aes.NewCipher(keyBytes)
	if err != nil {
		return "", fmt.Errorf("failed to create cipher: %w", err)
	}

	aesGCM, err := cipher.NewGCM(block)
	if err != nil {
		return "", fmt.Errorf("failed to create GCM: %w", err)
	}

	nonce := make([]byte, aesGCM.NonceSize())
	if _, err := io.ReadFull(rand.Reader, nonce); err != nil {
		return "", fmt.Errorf("failed to generate nonce: %w", err)
	}

	ciphertext := aesGCM.Seal(nonce, nonce, []byte(plaintext), nil)
	encoded := encPrefix + base64.StdEncoding.EncodeToString(ciphertext)
	return encoded, nil
}

// DecryptToken decrypts a token previously encrypted with EncryptToken.
// Returns an error if the token is not in the expected format or decryption fails.
func DecryptToken(encrypted string) (string, error) {
	if encrypted == "" {
		return "", nil
	}

	if len(encrypted) <= len(encPrefix) || encrypted[:len(encPrefix)] != encPrefix {
		return "", fmt.Errorf("invalid encrypted token format: missing version prefix")
	}

	keyBytes, err := getEncryptionKey()
	if err != nil {
		return "", err
	}

	ciphertext, err := base64.StdEncoding.DecodeString(encrypted[len(encPrefix):])
	if err != nil {
		return "", fmt.Errorf("failed to decode encrypted token: %w", err)
	}

	block, err := aes.NewCipher(keyBytes)
	if err != nil {
		return "", fmt.Errorf("failed to create cipher: %w", err)
	}

	aesGCM, err := cipher.NewGCM(block)
	if err != nil {
		return "", fmt.Errorf("failed to create GCM: %w", err)
	}

	nonceSize := aesGCM.NonceSize()
	if len(ciphertext) < nonceSize {
		return "", fmt.Errorf("ciphertext too short")
	}

	nonce, ciphertext := ciphertext[:nonceSize], ciphertext[nonceSize:]
	plaintext, err := aesGCM.Open(nil, nonce, ciphertext, nil)
	if err != nil {
		return "", fmt.Errorf("failed to decrypt token: %w", err)
	}

	return string(plaintext), nil
}

// MaskToken returns a masked version of a token for display in the UI.
// Shows only the last 4 characters, e.g. "ghp_****abcd".
func MaskToken(token string) string {
	if len(token) <= 4 {
		return "****"
	}
	return "****" + token[len(token)-4:]
}

// GenerateEncryptionKey generates a new random 32-byte key and returns it
// as a base64-encoded string. Used for initial setup / key rotation.
func GenerateEncryptionKey() (string, error) {
	key := make([]byte, 32)
	if _, err := rand.Read(key); err != nil {
		return "", fmt.Errorf("failed to generate key: %w", err)
	}
	return base64.StdEncoding.EncodeToString(key), nil
}
