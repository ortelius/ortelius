package sbom

import (
	"github.com/ortelius/ortelius/v12/util"
)

// cveEdgeVersionKeys are the numeric version parts a cve2purl edge may carry.
var cveEdgeVersionKeys = []string{
	"introduced_major", "introduced_minor", "introduced_patch",
	"fixed_major", "fixed_minor", "fixed_patch",
	"last_affected_major", "last_affected_minor", "last_affected_patch",
}

// cveRange is one OSV range of a CVE's affected entry.
type cveRange struct {
	Type   string                   `json:"type"`
	Events []map[string]interface{} `json:"events"`
}

// cveAffectedEntry is the part of one CVE affected[] entry the edge builder needs.
type cveAffectedEntry struct {
	Ranges []cveRange `json:"ranges"`
}

// buildCVEPurlEdge builds the cve2purl edge from a CVE to the package node an
// SBOM uses, from the CVE's affected entries for that package. It is the same
// edge the OSV loader writes (same _key, fields and range handling), so lazy and
// eager edges for one CVE and package are one document, and writing it again is
// harmless.
//
//   - SEMVER and ECOSYSTEM ranges are used; GIT ranges are skipped, because
//     commit SHAs cannot be compared as versions.
//   - SEMVER edges carry numeric version parts so the link query can compare
//     them in AQL.
//   - An entry with any ECOSYSTEM range gets explicit nulls instead (not
//     omitted: the upsert merges into an existing edge and old numbers would
//     survive), so every candidate goes through util.IsVersionAffected.
//   - When several ranges or entries map to the same package the last one wins,
//     as in the loader (there is one edge per CVE and package).
//
// ok is false when no usable range exists.
func buildCVEPurlEdge(cveKey, hubID, purlKey string, entries []cveAffectedEntry) (edge map[string]interface{}, ok bool) {
	for _, entry := range entries {
		hasEcosystemRange := false
		for _, r := range entry.Ranges {
			if r.Type == "ECOSYSTEM" {
				hasEcosystemRange = true
				break
			}
		}

		for _, r := range entry.Ranges {
			if r.Type != "SEMVER" && r.Type != "ECOSYSTEM" {
				continue
			}

			var introduced, fixed, lastAffected string
			for _, event := range r.Events {
				if v, isString := event["introduced"].(string); isString {
					introduced = v
				}
				if v, isString := event["fixed"].(string); isString {
					fixed = v
				}
				if v, isString := event["last_affected"].(string); isString {
					lastAffected = v
				}
			}
			if introduced == "" {
				introduced = "0"
			}

			introducedParsed := util.ParseSemanticVersion(introduced)
			fixedParsed := util.ParseSemanticVersion(fixed)
			lastAffectedParsed := util.ParseSemanticVersion(lastAffected)

			e := map[string]interface{}{
				"_key":          cveKey + "_" + purlKey,
				"_from":         "cve/" + cveKey,
				"_to":           hubID,
				"type":          r.Type,
				"introduced":    introduced,
				"fixed":         fixed,
				"last_affected": lastAffected,
			}

			setParts := func(prefix string, p *util.ParsedVersion) {
				if p.Major != nil {
					e[prefix+"_major"] = *p.Major
				}
				if p.Minor != nil {
					e[prefix+"_minor"] = *p.Minor
				}
				if p.Patch != nil {
					e[prefix+"_patch"] = *p.Patch
				}
			}
			setParts("introduced", introducedParsed)
			setParts("fixed", fixedParsed)
			setParts("last_affected", lastAffectedParsed)

			if hasEcosystemRange {
				for _, k := range cveEdgeVersionKeys {
					e[k] = nil
				}
			}

			edge, ok = e, true
		}
	}
	return edge, ok
}
