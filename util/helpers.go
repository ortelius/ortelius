// Package util provides utility functions for working with Package URLs (PURLs),
// version comparisons for vulnerability checking, and extracting metadata from the environment.
//
//revive:disable-next-line:var-naming
package util

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"sort"
	"strconv"
	"strings"

	"github.com/Masterminds/semver/v3"
	npm "github.com/aquasecurity/go-npm-version/pkg"
	pep440 "github.com/aquasecurity/go-pep440-version"
	"github.com/google/osv-scanner/pkg/models"
	"github.com/package-url/packageurl-go"
	"gopkg.in/yaml.v2"
)

// ParsedVersion holds parsed semantic version components
type ParsedVersion struct {
	Major *int
	Minor *int
	Patch *int
}

// ParseSemanticVersion parses a version string into numeric components
// Returns nil values for components that cannot be parsed
func ParseSemanticVersion(version string) *ParsedVersion {
	if version == "" {
		return &ParsedVersion{}
	}

	// Special case for "0" (used in OSV for "from beginning")
	if version == "0" {
		zero := 0
		return &ParsedVersion{Major: &zero, Minor: &zero, Patch: &zero}
	}

	// FIX: Strip "go" prefix for Go stdlib versions (e.g., "go1.22.2")
	// This must be done before semver parsing since Masterminds/semver doesn't handle it
	cleanVersion := strings.TrimPrefix(version, "go")

	// Try semver parsing first
	v, err := semver.NewVersion(cleanVersion)
	if err == nil {
		major := int(v.Major())
		minor := int(v.Minor())
		patch := int(v.Patch())

		return &ParsedVersion{
			Major: &major,
			Minor: &minor,
			Patch: &patch,
		}
	}

	// Fallback: try to parse manually for versions like "1.2" or "2"
	parts := strings.Split(cleanVersion, ".")
	result := &ParsedVersion{}

	if len(parts) >= 1 {
		if major, err := strconv.Atoi(strings.TrimSpace(parts[0])); err == nil {
			result.Major = &major
		}
	}
	if len(parts) >= 2 {
		if minor, err := strconv.Atoi(strings.TrimSpace(parts[1])); err == nil {
			result.Minor = &minor
		}
	}
	if len(parts) >= 3 {
		// Remove any pre-release or build metadata
		patchStr := strings.FieldsFunc(parts[2], func(r rune) bool {
			return r == '-' || r == '+'
		})[0]
		if patch, err := strconv.Atoi(strings.TrimSpace(patchStr)); err == nil {
			result.Patch = &patch
		}
	}

	return result
}

// GetEnvDefault is a convenience function for handling env vars
func GetEnvDefault(key, defVal string) string {
	val, ex := os.LookupEnv(key) // get the env var
	if !ex {                     // not found return default
		return defVal
	}
	return val // return value for env var
}

// IsEmpty checks if a string is empty or contains only whitespace
func IsEmpty(s string) bool {
	return len(strings.TrimSpace(s)) == 0
}

// IsNotEmpty checks if a string is not empty
func IsNotEmpty(s string) bool {
	return !IsEmpty(s)
}

// FileExists checks if a file exists
func FileExists(filename string) bool {
	_, err := os.Stat(filename)
	return err == nil
}

// CleanName removes periods and dashes from the name, replacing with underscores
func CleanName(name string) string {
	if name == "" {
		return name
	}
	name = strings.ReplaceAll(name, ".", "_")
	name = strings.ReplaceAll(name, "-", "_")
	return name
}

// GetEnvOrDefault returns environment variable value or default
func GetEnvOrDefault(key, defaultValue string) string {
	if value := os.Getenv(key); value != "" {
		return value
	}
	return defaultValue
}

// Contains checks if a string slice contains an item
func Contains(slice []string, item string) bool {
	for _, s := range slice {
		if s == item {
			return true
		}
	}
	return false
}

// FindChartPath finds a Helm chart file
func FindChartPath(shortname string) string {
	paths := []string{
		filepath.Join("helm", shortname, "Chart.yaml"),
		filepath.Join("helm", shortname, "Chart.yml"),
		filepath.Join("chart", shortname, "Chart.yaml"),
		filepath.Join("chart", shortname, "Chart.yml"),
		filepath.Join("charts", shortname, "Chart.yaml"),
		filepath.Join("charts", shortname, "Chart.yml"),
	}

	for _, path := range paths {
		if FileExists(path) {
			return path
		}
	}
	return ""
}

// ExtractChartVersion extracts version from a Helm chart file
func ExtractChartVersion(chartPath string) string {
	content, err := os.ReadFile(chartPath)
	if err != nil {
		return ""
	}

	var chart map[string]any
	if err := yaml.Unmarshal(content, &chart); err != nil {
		return ""
	}

	if version, ok := chart["version"].(string); ok {
		return version
	}
	return ""
}

// FindFile finds the first existing file from a list of candidates
func FindFile(candidates []string) string {
	for _, candidate := range candidates {
		if FileExists(candidate) {
			return candidate
		}
	}
	return ""
}

// RunCmd executes a shell command and returns the trimmed output
func RunCmd(cmd string) string {
	if strings.Contains(cmd, "git") {
		if _, err := os.Stat(".git"); os.IsNotExist(err) {
			return ""
		}
	}

	parts := strings.Fields(cmd)
	if len(parts) == 0 {
		return ""
	}

	var cmdExec *exec.Cmd
	if len(parts) == 1 {
		cmdExec = exec.Command(parts[0])
	} else {
		// For complex shell commands, use sh -c
		if strings.Contains(cmd, "|") || strings.Contains(cmd, "&&") || strings.Contains(cmd, "||") || strings.Contains(cmd, ";") {
			cmdExec = exec.Command("sh", "-c", cmd)
		} else {
			cmdExec = exec.Command(parts[0], parts[1:]...)
		}
	}

	output, err := cmdExec.Output()
	if err != nil {
		return ""
	}

	return strings.TrimSpace(string(output))
}

// GetStringOrDefault returns value or default if empty
func GetStringOrDefault(value, defaultValue string) string {
	if value == "" {
		return defaultValue
	}
	return value
}

// CleanPURL removes qualifiers (after ?) but preserves the subpath (after #)
// to maintain module identity (e.g. #v2)
func CleanPURL(purlStr string) (string, error) {
	parsed, err := packageurl.FromString(purlStr)
	if err != nil {
		return "", err
	}

	// Create new PURL without qualifiers
	cleaned := packageurl.PackageURL{
		Type:      parsed.Type,
		Namespace: parsed.Namespace,
		Name:      parsed.Name,
		Version:   parsed.Version,
		Subpath:   parsed.Subpath, // <--- UPDATE: This must be preserved
		// Qualifiers are intentionally omitted to clean the string
	}

	// Julia purls require a uuid qualifier per spec — the uuid, not the
	// name, is the durable package identity (names can be renamed/reused
	// in the Julia registry). Dropping it produces an invalid purl that
	// fails re-validation the next time it's parsed (e.g. in
	// GetStandardBasePURL), so it must survive qualifier-stripping.
	if strings.EqualFold(parsed.Type, "julia") {
		if uuid, ok := parsed.Qualifiers.Map()["uuid"]; ok {
			cleaned.Qualifiers = packageurl.Qualifiers{{Key: "uuid", Value: uuid}}
		}
	}

	return strings.ToLower(cleaned.ToString()), nil
}

// GetBasePURL removes the version component from a PURL to create a base package identifier.
// Used for matching CVE PURLs with SBOM component PURLs.
// Example: pkg:apk/wolfi/glibc@2.42-r4 -> pkg:apk/wolfi/glibc
func GetBasePURL(purlStr string) (string, error) {
	return GetStandardBasePURL(purlStr)
}

// ============================================================================
// CENTRALIZED PURL STANDARDIZATION - SINGLE SOURCE OF TRUTH
// ============================================================================

// EcosystemToPurlType converts OSV ecosystem to PURL type
// CRITICAL: This mapping ensures Wolfi/Chainguard CVEs link to apk SBOMs
func EcosystemToPurlType(ecosystem string) string {
	mapping := map[string]string{
		"npm":        "npm",
		"PyPI":       "pypi",
		"Maven":      "maven",
		"Go":         "golang",
		"NuGet":      "nuget",
		"RubyGems":   "gem",
		"crates.io":  "cargo",
		"Packagist":  "composer",
		"Pub":        "pub",
		"CocoaPods":  "cocoapods",
		"Hex":        "hex",
		"Alpine":     "apk",
		"Wolfi":      "apk", // CRITICAL: Map Wolfi to apk
		"Chainguard": "apk", // CRITICAL: Map Chainguard to apk
		"Debian":     "deb",
		"Ubuntu":     "deb",
		"Julia":      "julia", // Registered purl type (purl-spec, Oct 2025)
		"Github":     "github",
	}

	// Try exact match first
	if purlType, exists := mapping[ecosystem]; exists {
		return purlType
	}

	// Fallback: try case-insensitive
	for key, value := range mapping {
		if strings.EqualFold(key, ecosystem) {
			return value
		}
	}

	// Last resort: return lowercase ecosystem
	return strings.ToLower(ecosystem)
}

// GetBasePURLFromComponents constructs a standardized base PURL from ecosystem and package name
// This is the SINGLE SOURCE OF TRUTH for Hub PURL generation from OSV data
// Example: ("Wolfi", "wolfi", "glibc") -> "pkg:apk/wolfi/glibc"
func GetBasePURLFromComponents(ecosystem, namespace, name string) string {
	purlType := EcosystemToPurlType(ecosystem)

	// Build base PURL
	var basePurl string
	if namespace != "" {
		basePurl = fmt.Sprintf("pkg:%s/%s/%s", purlType, namespace, name)
	} else {
		basePurl = fmt.Sprintf("pkg:%s/%s", purlType, name)
	}

	return strings.ToLower(basePurl)
}

// GetStandardBasePURL extracts a standardized base PURL (no version/qualifiers)
// This is used to ensure consistent Hub keys across all collections
// Example: "pkg:apk/wolfi/glibc@2.42-r4" -> "pkg:apk/wolfi/glibc"
func GetStandardBasePURL(purlStr string) (string, error) {
	parsed, err := packageurl.FromString(purlStr)
	if err != nil {
		return "", err
	}

	// Normalize the ecosystem using our mapping
	normalizedType := EcosystemToPurlType(parsed.Type)

	base := packageurl.PackageURL{
		Type:      normalizedType,
		Namespace: parsed.Namespace,
		Name:      parsed.Name,
		// Version, Qualifiers, Subpath intentionally omitted
	}

	// See CleanPURL: Julia purls require a uuid qualifier per spec.
	if strings.EqualFold(normalizedType, "julia") {
		if uuid, ok := parsed.Qualifiers.Map()["uuid"]; ok {
			base.Qualifiers = packageurl.Qualifiers{{Key: "uuid", Value: uuid}}
		}
	}

	return strings.ToLower(base.ToString()), nil
}

// ParsePURL parses a PURL string and returns the parsed PackageURL
func ParsePURL(purlStr string) (*packageurl.PackageURL, error) {
	parsed, err := packageurl.FromString(purlStr)
	if err != nil {
		return nil, err
	}
	return &parsed, nil
}

// IsVersionAffected checks if a version is affected by OSV ranges
// Uses ecosystem-specific version parsers for accurate comparison
func IsVersionAffected(version string, affected models.Affected) bool {
	// Check specific versions list
	if len(affected.Versions) > 0 {
		for _, v := range affected.Versions {
			if version == v {
				return true
			}
		}
	}

	// Check version ranges
	if len(affected.Ranges) > 0 {
		for _, vrange := range affected.Ranges {
			// Only handle SEMVER and ECOSYSTEM types
			if vrange.Type != models.RangeEcosystem && vrange.Type != models.RangeSemVer {
				continue
			}

			// Determine ecosystem from package information if available
			ecosystem := ""
			if affected.Package.Ecosystem != "" {
				ecosystem = string(affected.Package.Ecosystem)
			}

			if isVersionInRange(version, vrange, ecosystem) {
				return true
			}
		}
	}

	return false
}

// isVersionInRange checks if a version falls within an OSV range.
//
// OSV range events form a timeline of vulnerability state changes. An
// introduced event does not require a matching fixed/last_affected event;
// when no closing event exists, the range is intentionally open-ended and
// remains vulnerable indefinitely.
//
// The evaluator below also handles multiple introduced/fixed intervals and
// explicit limit events.
func isVersionInRange(version string, vrange models.Range, ecosystem string) bool {
	switch strings.ToLower(ecosystem) {
	case "npm":
		return isVersionInRangeNPM(version, vrange)
	case "pypi":
		return isVersionInRangePython(version, vrange)
	}

	// Use semantic-version ordering when both the target and all non-limit
	// event boundaries can be parsed as semantic versions. ECOSYSTEM ranges
	// are allowed to contain arbitrary version strings, so fall back to the
	// existing string ordering when semantic parsing is not possible.
	if canEvaluateSemverRange(version, vrange) {
		return evaluateOSVRange(version, vrange, compareSemverVersions, "version")
	}

	return isVersionInRangeString(version, vrange)
}

type versionCompareFunc func(left, right string) (int, error)

type osvRangeEvent struct {
	kind    string
	version string
}

func evaluateOSVRange(
	version string,
	vrange models.Range,
	compare versionCompareFunc,
	label string,
) bool {
	if version == "" {
		return false
	}

	hasIntroduced := false
	hasExplicitLimit := false
	beforeExplicitLimit := false
	events := make([]osvRangeEvent, 0, len(vrange.Events))

	// OSV limit events are evaluated independently from the affected-state
	// timeline. If no limit is present, an implicit infinite limit applies.
	for _, event := range vrange.Events {
		if event.Introduced != "" {
			hasIntroduced = true
			events = append(events, osvRangeEvent{
				kind:    "introduced",
				version: event.Introduced,
			})
		}
		if event.Fixed != "" {
			events = append(events, osvRangeEvent{
				kind:    "fixed",
				version: event.Fixed,
			})
		}
		if event.LastAffected != "" {
			events = append(events, osvRangeEvent{
				kind:    "last_affected",
				version: event.LastAffected,
			})
		}
		if event.Limit != "" {
			hasExplicitLimit = true

			// "*" is OSV's explicit infinity marker.
			if strings.Contains(event.Limit, "*") {
				beforeExplicitLimit = true
				continue
			}

			cmp, err := compare(version, event.Limit)
			if err != nil {
				log.Printf(
					"WARNING: Failed to compare %s %q with OSV limit %q: %v",
					label, version, event.Limit, err,
				)
				return false
			}
			if cmp < 0 {
				beforeExplicitLimit = true
			}
		}
	}

	// The OSV schema requires at least one introduced event. Unlike the old
	// implementation, an upper boundary is NOT required: introduced-only is
	// a valid open-ended vulnerability range.
	if !hasIntroduced {
		log.Printf("WARNING: OSV range for %s %q has no introduced event", label, version)
		return false
	}

	if hasExplicitLimit && !beforeExplicitLimit {
		return false
	}

	// OSV evaluation is defined over the events sorted by the range's version
	// ordering. Stable sorting also makes multiple affected intervals work
	// even when the source did not emit events in order.
	var sortErr error
	sort.SliceStable(events, func(i, j int) bool {
		if sortErr != nil {
			return false
		}

		cmp, err := compareRangeBoundary(events[i].version, events[j].version, compare)
		if err != nil {
			sortErr = err
			return false
		}
		if cmp != 0 {
			return cmp < 0
		}

		// At the same boundary, process introduced before closing events so
		// fixed can still win at that exact version.
		return osvRangeEventPriority(events[i].kind) < osvRangeEventPriority(events[j].kind)
	})

	if sortErr != nil {
		log.Printf("WARNING: Failed to order OSV range events for %s %q: %v", label, version, sortErr)
		return false
	}

	vulnerable := false

	for _, event := range events {
		cmp, err := compareVersionToBoundary(version, event.version, compare)
		if err != nil {
			log.Printf(
				"WARNING: Failed to compare %s %q with OSV %s boundary %q: %v",
				label, version, event.kind, event.version, err,
			)
			return false
		}

		switch event.kind {
		case "introduced":
			if cmp >= 0 {
				vulnerable = true
			}

		case "fixed":
			// fixed is exclusive: the fixed version and anything newer are
			// no longer affected until another introduced event is reached.
			if cmp >= 0 {
				vulnerable = false
			}

		case "last_affected":
			// last_affected is inclusive.
			if cmp > 0 {
				vulnerable = false
			}
		}
	}

	return vulnerable
}

func osvRangeEventPriority(kind string) int {
	switch kind {
	case "introduced":
		return 0
	case "last_affected":
		return 1
	case "fixed":
		return 2
	default:
		return 3
	}
}

func compareVersionToBoundary(version string, boundary string, compare versionCompareFunc) (int, error) {
	// introduced:"0" is a special OSV value that sorts before every real
	// version. Treat it as already reached without asking an ecosystem parser
	// to interpret it.
	if boundary == "0" {
		return 1, nil
	}
	return compare(version, boundary)
}

func compareRangeBoundary(left string, right string, compare versionCompareFunc) (int, error) {
	if left == right {
		return 0, nil
	}
	if left == "0" {
		return -1, nil
	}
	if right == "0" {
		return 1, nil
	}
	return compare(left, right)
}

func canEvaluateSemverRange(version string, vrange models.Range) bool {
	if _, err := semver.NewVersion(strings.TrimPrefix(version, "go")); err != nil {
		return false
	}

	for _, event := range vrange.Events {
		for _, boundary := range []string{event.Introduced, event.Fixed, event.LastAffected} {
			if boundary == "" || boundary == "0" {
				continue
			}
			if _, err := semver.NewVersion(strings.TrimPrefix(boundary, "go")); err != nil {
				return false
			}
		}

		if event.Limit != "" && !strings.Contains(event.Limit, "*") {
			if _, err := semver.NewVersion(strings.TrimPrefix(event.Limit, "go")); err != nil {
				return false
			}
		}
	}

	return true
}

func compareSemverVersions(left, right string) (int, error) {
	l, err := semver.NewVersion(strings.TrimPrefix(left, "go"))
	if err != nil {
		return 0, err
	}

	r, err := semver.NewVersion(strings.TrimPrefix(right, "go"))
	if err != nil {
		return 0, err
	}

	return l.Compare(r), nil
}

// isVersionInRangeNPM uses npm-specific version comparison.
func isVersionInRangeNPM(version string, vrange models.Range) bool {
	if _, err := npm.NewVersion(version); err != nil {
		return isVersionInRangeString(version, vrange)
	}

	return evaluateOSVRange(version, vrange, compareNPMVersions, "npm version")
}

func compareNPMVersions(left, right string) (int, error) {
	l, err := npm.NewVersion(left)
	if err != nil {
		return 0, err
	}

	r, err := npm.NewVersion(right)
	if err != nil {
		return 0, err
	}

	if l.LessThan(r) {
		return -1, nil
	}
	if r.LessThan(l) {
		return 1, nil
	}
	return 0, nil
}

// isVersionInRangePython uses PEP 440 version comparison for Python packages.
func isVersionInRangePython(version string, vrange models.Range) bool {
	if _, err := pep440.Parse(version); err != nil {
		return isVersionInRangeString(version, vrange)
	}

	return evaluateOSVRange(version, vrange, comparePythonVersions, "python version")
}

func comparePythonVersions(left, right string) (int, error) {
	l, err := pep440.Parse(left)
	if err != nil {
		return 0, err
	}

	r, err := pep440.Parse(right)
	if err != nil {
		return 0, err
	}

	if l.LessThan(r) {
		return -1, nil
	}
	if r.LessThan(l) {
		return 1, nil
	}
	return 0, nil
}

// isVersionInRangeString performs the existing string-based version ordering
// for ECOSYSTEM ranges that cannot be parsed by one of the supported version
// parsers. Open-ended ranges are valid here as well.
func isVersionInRangeString(version string, vrange models.Range) bool {
	return evaluateOSVRange(
		version,
		vrange,
		func(left, right string) (int, error) {
			return strings.Compare(left, right), nil
		},
		"string version",
	)
}

// GetSeverityScore returns the lowest CVSS base score threshold for a given severity rating.
func GetSeverityScore(severity string) float64 {
	switch strings.ToUpper(strings.TrimSpace(severity)) {
	case "NONE":
		return 0.0
	case "LOW":
		return 0.1
	case "MEDIUM":
		return 4.0
	case "HIGH":
		return 7.0
	case "CRITICAL":
		return 9.0
	default:
		return 0.0 // unknown severity defaults to 0.0
	}
}

// Add these functions to util/helpers.go

// ExtractApplicableFixedVersion checks all affected entries and returns the fix version for the matching range
func ExtractApplicableFixedVersion(currentVersion string, allAffected []models.Affected) []string {
	// Try each affected entry until we find one where the version is in range
	for _, affected := range allAffected {
		result := extractFixedFromSingleAffected(currentVersion, affected)
		if len(result) > 0 {
			return result
		}
	}
	// No match found, return all fixed versions as fallback
	return extractAllFixedFromMultiple(allAffected)
}

func extractFixedFromSingleAffected(currentVersion string, affected models.Affected) []string {
	currentV, err := semver.NewVersion(currentVersion)
	if err != nil {
		npmV, npmErr := npm.NewVersion(currentVersion)
		if npmErr == nil {
			return extractApplicableFixedVersionNPM(npmV, affected)
		}

		pepV, pepErr := pep440.Parse(currentVersion)
		if pepErr == nil {
			return extractApplicableFixedVersionPython(pepV, affected)
		}

		return []string{}
	}

	for _, vrange := range affected.Ranges {
		if vrange.Type != models.RangeEcosystem && vrange.Type != models.RangeSemVer {
			continue
		}

		var introduced, fixed, lastAffected string

		for _, event := range vrange.Events {
			if event.Introduced != "" {
				introduced = event.Introduced
			}
			if event.Fixed != "" {
				fixed = event.Fixed
			}
			if event.LastAffected != "" {
				lastAffected = event.LastAffected
			}
		}

		inRange := true

		if introduced != "" && introduced != "0" {
			introV, err := semver.NewVersion(introduced)
			if err == nil && currentV.LessThan(introV) {
				inRange = false
			}
		}

		if inRange {
			if fixed != "" {
				fixV, err := semver.NewVersion(fixed)
				if err == nil {
					if currentV.GreaterThan(fixV) || currentV.Equal(fixV) {
						inRange = false
					}
				}
			} else if lastAffected != "" {
				lastV, err := semver.NewVersion(lastAffected)
				if err == nil {
					if currentV.GreaterThan(lastV) {
						inRange = false
					}
				}
			}
		}

		if inRange {
			if fixed != "" {
				return []string{fixed}
			}
			if lastAffected != "" {
				return []string{}
			}
		}
	}

	return []string{}
}

func extractApplicableFixedVersionNPM(currentV npm.Version, affected models.Affected) []string {
	for _, vrange := range affected.Ranges {
		if vrange.Type != models.RangeEcosystem && vrange.Type != models.RangeSemVer {
			continue
		}

		var introduced, fixed, lastAffected string
		for _, event := range vrange.Events {
			if event.Introduced != "" {
				introduced = event.Introduced
			}
			if event.Fixed != "" {
				fixed = event.Fixed
			}
			if event.LastAffected != "" {
				lastAffected = event.LastAffected
			}
		}

		inRange := true

		if introduced != "" && introduced != "0" {
			introV, err := npm.NewVersion(introduced)
			if err == nil && currentV.LessThan(introV) {
				inRange = false
			}
		}

		if inRange {
			if fixed != "" {
				fixV, err := npm.NewVersion(fixed)
				if err == nil {
					if currentV.GreaterThan(fixV) || currentV.Equal(fixV) {
						inRange = false
					}
				}
			} else if lastAffected != "" {
				lastV, err := npm.NewVersion(lastAffected)
				if err == nil && currentV.GreaterThan(lastV) {
					inRange = false
				}
			}
		}

		if inRange && fixed != "" {
			return []string{fixed}
		}
	}

	return []string{}
}

func extractApplicableFixedVersionPython(currentV pep440.Version, affected models.Affected) []string {
	for _, vrange := range affected.Ranges {
		if vrange.Type != models.RangeEcosystem && vrange.Type != models.RangeSemVer {
			continue
		}

		var introduced, fixed, lastAffected string
		for _, event := range vrange.Events {
			if event.Introduced != "" {
				introduced = event.Introduced
			}
			if event.Fixed != "" {
				fixed = event.Fixed
			}
			if event.LastAffected != "" {
				lastAffected = event.LastAffected
			}
		}

		inRange := true

		if introduced != "" && introduced != "0" {
			introV, err := pep440.Parse(introduced)
			if err == nil && currentV.LessThan(introV) {
				inRange = false
			}
		}

		if inRange {
			if fixed != "" {
				fixV, err := pep440.Parse(fixed)
				if err == nil {
					if currentV.GreaterThan(fixV) || currentV.Equal(fixV) {
						inRange = false
					}
				}
			} else if lastAffected != "" {
				lastV, err := pep440.Parse(lastAffected)
				if err == nil && currentV.GreaterThan(lastV) {
					inRange = false
				}
			}
		}

		if inRange && fixed != "" {
			return []string{fixed}
		}
	}

	return []string{}
}

func extractAllFixedFromMultiple(allAffected []models.Affected) []string {
	var fixedVersions []string
	seen := make(map[string]bool)
	for _, affected := range allAffected {
		for _, vrange := range affected.Ranges {
			for _, event := range vrange.Events {
				if event.Fixed != "" && !seen[event.Fixed] {
					fixedVersions = append(fixedVersions, event.Fixed)
					seen[event.Fixed] = true
				}
			}
		}
	}
	return fixedVersions
}

// ============================================================================
// Version Parsing Functions
// ============================================================================

var versionPrefixPattern = regexp.MustCompile(`^.*?-v(\d+)`)
var semverPattern = regexp.MustCompile(`^(\d+)\.(\d+)\.(\d+)(?:-([0-9A-Za-z\-\.]+))?(?:\+([0-9A-Za-z\-\.]+))?$`)

// CleanVersion removes branch prefixes from version strings
// Examples:
//   - "main-v12.0.1376-g7ac6f3" -> "12.0.1376-g7ac6f3"
//   - "develop-v2.3.4" -> "2.3.4"
//   - "v1.2.3" -> "v1.2.3" (unchanged)
func CleanVersion(version string) string {
	if version == "" {
		return version
	}
	if versionPrefixPattern.MatchString(version) {
		matches := versionPrefixPattern.FindStringSubmatch(version)
		if len(matches) > 1 {
			cleaned := versionPrefixPattern.ReplaceAllString(version, matches[1])
			return cleaned
		}
	}
	return version
}

// ParsedSemver holds all components of a semantic version
type ParsedSemver struct {
	Major         *int
	Minor         *int
	Patch         *int
	Prerelease    string
	BuildMetadata string
}

// ParseSemver parses a semantic version string into all its components
// Returns nil if the version cannot be parsed
func ParseSemver(version string) *ParsedSemver {
	if version == "" {
		return nil
	}

	result := &ParsedSemver{}

	// Try full semver regex first
	matches := semverPattern.FindStringSubmatch(version)
	if len(matches) > 0 {
		// Parse major.minor.patch using existing ParseSemanticVersion
		parsed := ParseSemanticVersion(version)
		if parsed != nil {
			result.Major = parsed.Major
			result.Minor = parsed.Minor
			result.Patch = parsed.Patch
		}

		// Extract prerelease (group 4)
		if len(matches) > 4 && matches[4] != "" {
			result.Prerelease = matches[4]
		}

		// Extract build metadata (group 5)
		if len(matches) > 5 && matches[5] != "" {
			result.BuildMetadata = matches[5]
		}

		return result
	}

	// Fallback: try to parse what we can
	parsed := ParseSemanticVersion(version)
	if parsed != nil {
		result.Major = parsed.Major
		result.Minor = parsed.Minor
		result.Patch = parsed.Patch
	}

	// Try to extract prerelease and build metadata manually
	// Format: major.minor.patch-prerelease+buildmetadata
	parts := strings.SplitN(version, "-", 2)
	if len(parts) == 2 {
		remainder := parts[1]
		plusParts := strings.SplitN(remainder, "+", 2)
		if len(plusParts) == 2 {
			result.Prerelease = plusParts[0]
			result.BuildMetadata = plusParts[1]
		} else {
			result.Prerelease = remainder
		}
	}

	// Check for build metadata without prerelease
	if result.Prerelease == "" && strings.Contains(version, "+") {
		plusParts := strings.SplitN(version, "+", 2)
		if len(plusParts) == 2 {
			result.BuildMetadata = plusParts[1]
		}
	}

	return result
}
