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

// SanitizeKey ensures the database key is valid for ArangoDB.
//
// ArangoDB _key values may only contain letters, digits, and the symbols
// _ - : . @ ( ) + , = ; $ ! * ' %  -- anything else must be removed or
// replaced, or ArangoDB rejects the document outright. Unless every single
// caller checks that error (most don't, historically -- see
// osvdev-job's processEdges), an invalid key fails silently: the document
// is never written, but the caller proceeds as if it was, e.g. creating an
// edge that points at a purl document that doesn't exist and never will,
// on every single run, forever (ArangoDB doesn't enforce referential
// integrity on edge collections, so nothing else catches this either).
//
// This was surfaced by Julia purls, which now legitimately retain a
// "?uuid=..." qualifier (required by the purl-spec for that type) -- the
// "?" was never in this function's replacer and isn't in ArangoDB's
// allowed set, so every Julia purl's key generation failed the same way,
// every time.
func SanitizeKey(key string) string {
	// 1. Trim whitespace/newlines first.
	key = strings.TrimSpace(key)

	// 2. Preserve prior behavior exactly for these specific characters, so
	// every key that doesn't hit the new fallback below stays
	// byte-identical to what's already stored.
	replacer := strings.NewReplacer(
		" ", "-",
		"/", "-",
		"[", "",
		"]", "",
		"(", "",
		")", "",
	)
	key = replacer.Replace(key)

	// 3. Safety net: replace anything ArangoDB doesn't allow in a _key with
	// "-" instead of silently producing an invalid key. This only changes
	// output for inputs that were already broken (nothing was ever
	// successfully stored under an invalid key), so it can't orphan
	// anything that currently works.
	const allowedSymbols = "_-:.@+,=;$!*'%"
	var b strings.Builder
	b.Grow(len(key))
	for _, r := range key {
		switch {
		case r >= 'a' && r <= 'z', r >= 'A' && r <= 'Z', r >= '0' && r <= '9':
			b.WriteRune(r)
		case strings.ContainsRune(allowedSymbols, r):
			b.WriteRune(r)
		default:
			b.WriteRune('-')
		}
	}

	return b.String()
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
