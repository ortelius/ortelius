// Package util provides utility functions for the backend.
//
//revive:disable-next-line:var-naming
package util

import (
	"github.com/google/osv-scanner/pkg/models"
)

// IsVersionAffectedAny checks if a version is affected by any of the provided affected ranges
// This is a convenience wrapper around IsVersionAffected for multiple affected entries
func IsVersionAffectedAny(version string, allAffected []models.Affected) bool {
	for _, affected := range allAffected {
		if IsVersionAffected(version, affected) {
			return true
		}
	}
	return false
}
