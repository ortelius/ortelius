// Package metadata provides simple key-value storage for operational metadata.
package metadata

import (
	"context"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/gofiber/fiber/v3"
	"github.com/ortelius/ortelius/v12/database"
)

// MetadataRecord represents a key-value record in the metadata collection.
type MetadataRecord struct {
	Key       string `json:"_key"`
	Value     string `json:"value"`
	UpdatedAt string `json:"updated_at"`
}

// GetMetadata retrieves a value by key from the metadata collection.
func GetMetadata(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		key := c.Params("key")
		if key == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "key is required",
			})
		}

		ctx := context.Background()
		query := `
			FOR m IN metadata
				FILTER m._key == @key
				LIMIT 1
				RETURN { key: m._key, value: m.value, updated_at: m.updated_at }
		`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{"key": key},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": "Query failed",
			})
		}
		defer cursor.Close()

		if !cursor.HasMore() {
			return c.Status(fiber.StatusNotFound).JSON(fiber.Map{
				"error": "Key not found",
			})
		}

		var record MetadataRecord
		if _, err := cursor.ReadDocument(ctx, &record); err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": "Failed to read record",
			})
		}

		return c.JSON(fiber.Map{
			"key":        record.Key,
			"value":      record.Value,
			"updated_at": record.UpdatedAt,
		})
	}
}

// SetMetadata upserts a value by key into the metadata collection.
func SetMetadata(db database.DBConnection) fiber.Handler {
	return func(c fiber.Ctx) error {
		key := c.Params("key")
		if key == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "key is required",
			})
		}

		var body struct {
			Value string `json:"value"`
		}
		if err := c.Bind().Body(&body); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "Invalid request body",
			})
		}

		if body.Value == "" {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "value is required",
			})
		}

		now := time.Now().UTC().Format(time.RFC3339)
		ctx := context.Background()

		query := `
			UPSERT { _key: @key }
			INSERT { _key: @key, value: @value, updated_at: @updated_at }
			UPDATE { value: @value, updated_at: @updated_at }
			IN metadata
			RETURN NEW
		`
		cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
			BindVars: map[string]interface{}{
				"key":        key,
				"value":      body.Value,
				"updated_at": now,
			},
		})
		if err != nil {
			return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
				"error": "Upsert failed",
			})
		}
		defer cursor.Close()

		return c.JSON(fiber.Map{
			"key":        key,
			"value":      body.Value,
			"updated_at": now,
		})
	}
}
