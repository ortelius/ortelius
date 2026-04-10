// Package releases defines the REST API types for release operations.
package releases

// ReleaseWithSBOMResponse returns the result of POST operations
//
//revive:disable-next-line:exported
type ReleaseWithSBOMResponse struct {
	Success bool   `json:"success"`
	Message string `json:"message"`
}

// ReleaseListItem represents a simplified release for list view
//
//revive:disable-next-line:exported
type ReleaseListItem struct {
	Key     string `json:"_key"`
	Name    string `json:"name"`
	Version string `json:"version"`
}
