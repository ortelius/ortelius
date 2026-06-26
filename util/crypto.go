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
	fmt.Println("[DEBUG] getEncryptionKey: called")
	key := os.Getenv("TOKEN_ENCRYPTION_KEY")
	fmt.Println("[DEBUG] getEncryptionKey: TOKEN_ENCRYPTION_KEY raw length =", len(key))
	if key == "" {
		fmt.Println("[DEBUG] getEncryptionKey: TOKEN_ENCRYPTION_KEY is not set")
		return nil, fmt.Errorf("TOKEN_ENCRYPTION_KEY environment variable is not set")
	}
	keyBytes, err := base64.StdEncoding.DecodeString(key)
	if err != nil {
		fmt.Println("[DEBUG] getEncryptionKey: TOKEN_ENCRYPTION_KEY is not valid base64:", err)
		return nil, fmt.Errorf("TOKEN_ENCRYPTION_KEY must be a base64-encoded 32-byte key: %w", err)
	}
	fmt.Println("[DEBUG] getEncryptionKey: decoded key length =", len(keyBytes), "bytes")
	if len(keyBytes) != 32 {
		fmt.Println("[DEBUG] getEncryptionKey: TOKEN_ENCRYPTION_KEY decoded to", len(keyBytes), "bytes, expected 32")
		return nil, fmt.Errorf("TOKEN_ENCRYPTION_KEY must decode to exactly 32 bytes, got %d", len(keyBytes))
	}
	fmt.Println("[DEBUG] getEncryptionKey: success, returning valid 32-byte key")
	return keyBytes, nil
}

// EncryptToken encrypts a plaintext PAT using AES-256-GCM.
// The result is prefixed with "enc:v1:" and base64-encoded for safe storage.
// The encryption key is read from the TOKEN_ENCRYPTION_KEY environment variable,
// which must be a base64-encoded 32-byte key.
func EncryptToken(plaintext string) (string, error) {
	fmt.Println("[DEBUG] EncryptToken: called, plaintext length =", len(plaintext))
	if plaintext == "" {
		fmt.Println("[DEBUG] EncryptToken: plaintext is empty, returning empty string with no error")
		return "", nil
	}

	keyBytes, err := getEncryptionKey()
	if err != nil {
		fmt.Println("[DEBUG] EncryptToken: getEncryptionKey returned error:", err)
		return "", err
	}

	block, err := aes.NewCipher(keyBytes)
	if err != nil {
		fmt.Println("[DEBUG] EncryptToken: aes.NewCipher failed:", err)
		return "", fmt.Errorf("failed to create cipher: %w", err)
	}
	fmt.Println("[DEBUG] EncryptToken: aes.NewCipher succeeded")

	aesGCM, err := cipher.NewGCM(block)
	if err != nil {
		fmt.Println("[DEBUG] EncryptToken: cipher.NewGCM failed:", err)
		return "", fmt.Errorf("failed to create GCM: %w", err)
	}
	fmt.Println("[DEBUG] EncryptToken: cipher.NewGCM succeeded")

	nonce := make([]byte, aesGCM.NonceSize())
	if _, err := io.ReadFull(rand.Reader, nonce); err != nil {
		fmt.Println("[DEBUG] EncryptToken: failed to generate nonce:", err)
		return "", fmt.Errorf("failed to generate nonce: %w", err)
	}
	fmt.Println("[DEBUG] EncryptToken: nonce generated, size =", len(nonce))

	ciphertext := aesGCM.Seal(nonce, nonce, []byte(plaintext), nil)
	encoded := encPrefix + base64.StdEncoding.EncodeToString(ciphertext)
	fmt.Println("[DEBUG] EncryptToken: success, encoded token length =", len(encoded))
	return encoded, nil
}

// DecryptToken decrypts a token previously encrypted with EncryptToken.
// Returns an error if the token is not in the expected format or decryption fails.
func DecryptToken(encrypted string) (string, error) {
	fmt.Println("[DEBUG] DecryptToken: called, input length =", len(encrypted))
	if encrypted == "" {
		fmt.Println("[DEBUG] DecryptToken: input is empty, returning empty string with no error")
		return "", nil
	}

	if len(encrypted) <= len(encPrefix) || encrypted[:len(encPrefix)] != encPrefix {
		fmt.Println("[DEBUG] DecryptToken: missing or invalid version prefix")
		return "", fmt.Errorf("invalid encrypted token format: missing version prefix")
	}

	keyBytes, err := getEncryptionKey()
	if err != nil {
		fmt.Println("[DEBUG] DecryptToken: getEncryptionKey returned error:", err)
		return "", err
	}

	ciphertext, err := base64.StdEncoding.DecodeString(encrypted[len(encPrefix):])
	if err != nil {
		fmt.Println("[DEBUG] DecryptToken: failed to decode base64 ciphertext:", err)
		return "", fmt.Errorf("failed to decode encrypted token: %w", err)
	}
	fmt.Println("[DEBUG] DecryptToken: decoded ciphertext length =", len(ciphertext))

	block, err := aes.NewCipher(keyBytes)
	if err != nil {
		fmt.Println("[DEBUG] DecryptToken: aes.NewCipher failed:", err)
		return "", fmt.Errorf("failed to create cipher: %w", err)
	}
	fmt.Println("[DEBUG] DecryptToken: aes.NewCipher succeeded")

	aesGCM, err := cipher.NewGCM(block)
	if err != nil {
		fmt.Println("[DEBUG] DecryptToken: cipher.NewGCM failed:", err)
		return "", fmt.Errorf("failed to create GCM: %w", err)
	}
	fmt.Println("[DEBUG] DecryptToken: cipher.NewGCM succeeded")

	nonceSize := aesGCM.NonceSize()
	if len(ciphertext) < nonceSize {
		fmt.Println("[DEBUG] DecryptToken: ciphertext too short, len=", len(ciphertext), "nonceSize=", nonceSize)
		return "", fmt.Errorf("ciphertext too short")
	}

	nonce, ciphertext := ciphertext[:nonceSize], ciphertext[nonceSize:]
	plaintext, err := aesGCM.Open(nil, nonce, ciphertext, nil)
	if err != nil {
		fmt.Println("[DEBUG] DecryptToken: aesGCM.Open failed (wrong key or corrupted data):", err)
		return "", fmt.Errorf("failed to decrypt token: %w", err)
	}
	fmt.Println("[DEBUG] DecryptToken: success, plaintext length =", len(plaintext))

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
