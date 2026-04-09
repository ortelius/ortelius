// Package util provides utility functions for the application.
package util

import "strings"

// NormalizeOrgName ensures org names are always lowercase and trimmed
// Use this function whenever accepting org names from external sources
func NormalizeOrgName(org string) string {
	return strings.ToLower(strings.TrimSpace(org))
}

// NormalizeOrgNames normalizes a slice of org names
func NormalizeOrgNames(orgs []string) []string {
	if orgs == nil {
		return []string{}
	}

	normalized := make([]string, len(orgs))
	for i, org := range orgs {
		normalized[i] = NormalizeOrgName(org)
	}
	return normalized
}
