// Package util provides utility functions for the backend.
//
//revive:disable-next-line:var-naming
package util

import "strings"

// NameComponents holds the parsed components of a name field
type NameComponents struct {
	Org       string
	Path      []string
	Shortname string
	IsPublic  bool
}

// ParseName parses a name string into its components
// Format: <org>/<shortname> with no path
// If no "/" present, org defaults to "library"
// All records are set to public by default
func ParseName(name string) NameComponents {
	if name == "" {
		return NameComponents{
			Org:      "library",
			Path:     []string{},
			IsPublic: true,
		}
	}

	parts := strings.Split(name, "/")

	if len(parts) > 1 {
		// Has org prefix
		return NameComponents{
			Org:       parts[0],
			Shortname: parts[1],
			Path:      []string{},
			IsPublic:  true,
		}
	}

	// No org prefix, use "library" as default
	return NameComponents{
		Org:       "library",
		Shortname: name,
		Path:      []string{},
		IsPublic:  true,
	}
}
