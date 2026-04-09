// Package model defines the data structures for organization management in the PDVD system.
package model

import "time"

// Org represents an organization in the system
type Org struct {
	Key         string            `json:"_key,omitempty"`
	ID          string            `json:"_id,omitempty"`
	Rev         string            `json:"_rev,omitempty"`
	Name        string            `json:"name"`
	DisplayName string            `json:"display_name,omitempty"`
	Description string            `json:"description,omitempty"`
	Metadata    map[string]string `json:"metadata,omitempty"`
	CreatedAt   time.Time         `json:"created_at"`
	UpdatedAt   time.Time         `json:"updated_at"`
}
