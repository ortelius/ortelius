// Package util provides utility functions for the backend.
//
//revive:disable-next-line:var-naming
package util

import (
	"context"
	"encoding/json"
	"fmt"
	"strings"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/google/osv-scanner/pkg/models"
	"github.com/ortelius/ortelius/v12/database"
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

// FetchCVEAffectedData retrieves the 'affected' arrays for a batch of CVE IDs (OSV IDs).
// This prevents high-fanout memory bloat in ArangoDB by doing a fast O(1) fetch in Go.
func FetchCVEAffectedData(ctx context.Context, db arangodb.Database, cveIDs []string) (map[string][]models.Affected, error) {
	if len(cveIDs) == 0 {
		return make(map[string][]models.Affected), nil
	}

	query := `
		FOR cve IN cve
			FILTER cve.id IN @cve_ids
			RETURN { id: cve.id, affected: cve.affected }
	`
	cursor, err := db.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"cve_ids": cveIDs},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	cveMap := make(map[string][]models.Affected)

	for cursor.HasMore() {
		var doc struct {
			ID       string                   `json:"id"`
			Affected []map[string]interface{} `json:"affected"`
		}
		if _, err := cursor.ReadDocument(ctx, &doc); err == nil {
			var modelsAffected []models.Affected
			for _, aMap := range doc.Affected {
				bytes, err := json.Marshal(aMap)
				if err != nil {
					continue
				}
				var affected models.Affected
				if err := json.Unmarshal(bytes, &affected); err == nil {
					modelsAffected = append(modelsAffected, affected)
				}
			}
			cveMap[doc.ID] = modelsAffected
		}
	}

	return cveMap, nil
}
