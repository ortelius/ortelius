// Package sbom defines the REST API types for SBOM operations.
package sbom

// EdgeInfo holds edge information for batch processing
type EdgeInfo struct {
	From     string
	To       string
	Version  string
	FullPurl string
}

// PurlInfo holds PURL information for batch processing
type PurlInfo struct {
	BasePurl     string
	Version      string
	FullPurl     string
	VersionMajor *int
	VersionMinor *int
	VersionPatch *int
	Ecosystem    string
}
