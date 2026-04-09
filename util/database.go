// Package util provides utility functions for the backend.
//
//revive:disable-next-line:var-naming
package util

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/pdvd-backend/v12/database"
)

// SanitizeKey ensures the database key is valid for ArangoDB
// ArangoDB keys cannot contain spaces, slashes, or brackets
func SanitizeKey(key string) string {
	// 1. Trim whitespace/newlines first
	key = strings.TrimSpace(key)

	// 2. Use Replacer for cleaner, faster, multi-string replacement
	replacer := strings.NewReplacer(
		" ", "-",
		"/", "-",
		"[", "",
		"]", "",
		"(", "",
		")", "",
	)

	return replacer.Replace(key)
}

// EcosystemMetadata stores the high-water mark for imports
type EcosystemMetadata struct {
	Key          string `json:"_key"`          // e.g., "npm", "maven"
	LastModified string `json:"last_modified"` // RFC3339 Timestamp
	Type         string `json:"type"`          // "ecosystem_metadata"
}

// GetLastRun retrieves the timestamp of the last successful import for an ecosystem
func GetLastRun(db database.DBConnection, ecosystem string) (time.Time, error) {
	key := SanitizeKey(ecosystem)
	if key == "" {
		return time.Time{}, nil
	}

	ctx := context.Background()
	query := `RETURN DOCUMENT("metadata", @key)`
	bindVars := map[string]interface{}{"key": key}

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	if err != nil {
		return time.Time{}, nil
	}
	defer cursor.Close()

	var meta EcosystemMetadata
	if _, err := cursor.ReadDocument(ctx, &meta); err != nil {
		return time.Time{}, nil
	}

	return time.Parse(time.RFC3339, meta.LastModified)
}

// SaveLastRun updates the timestamp after a successful import
func SaveLastRun(db database.DBConnection, ecosystem string, lastModified time.Time) error {
	key := SanitizeKey(ecosystem)

	// Final safety check to prevent empty keys
	if key == "" {
		return fmt.Errorf("cannot save last run for empty ecosystem key (original: %s)", ecosystem)
	}

	ctx := context.Background()
	query := `
		UPSERT { _key: @key }
		INSERT { _key: @key, last_modified: @time, type: "ecosystem_metadata" }
		UPDATE { last_modified: @time }
		IN metadata
	`

	bindVars := map[string]interface{}{
		"key":  key,
		"time": lastModified.Format(time.RFC3339),
	}

	_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{BindVars: bindVars})
	return err
}
