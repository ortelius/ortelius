package util

import (
	"fmt"
	"regexp"
	"strings"

	"github.com/google/osv-scanner/pkg/models"
)

// apk (Alpine, Wolfi, Chainguard, Alpaquita) version ordering.
//
// An apk version is
//
//	<number>{.<number>}*[<letter>]{_<suffix>[<number>]}*[-r<number>]
//
// e.g. 3.3.12-r10, 2.42-r12, 1.2.3_rc1-r0, 20240101_git20240102-r3.
//
// The part that semver gets wrong is the package revision: -r9 is lower than
// -r10 because the revision is a number, but semver reads "-r9" as a
// pre-release tag and compares it as text, which puts r9 above r10 and makes a
// still-vulnerable r9 look already fixed.
//
// Ordering, most significant first:
//  1. the dot-separated numbers, numerically (a missing component counts as 0)
//  2. the optional single letter (none < a < b < ...)
//  3. the suffixes, left to right: _alpha < _beta < _pre < _rc < (none) < _cvs <
//     _svn < _git < _hg < _p, then their numbers (missing counts as 0)
//  4. the -r revision, numerically (missing counts as 0)
//
// Limitation: apk-tools compares a component with a leading zero (1.02) as a
// decimal fraction. That case is not reproduced; the component is compared as
// a plain number. Versions outside the grammar above are reported as invalid
// and callers fall back to the generic comparison.

var apkVersionPattern = regexp.MustCompile(`^([0-9]+(?:\.[0-9]+)*)([a-z])?((?:_(?:alpha|beta|pre|rc|cvs|svn|git|hg|p)[0-9]*)*)(?:-r([0-9]+))?$`)

var apkSuffixPattern = regexp.MustCompile(`_(alpha|beta|pre|rc|cvs|svn|git|hg|p)([0-9]*)`)

// apkSuffixRank orders the version suffixes; a version without a suffix ranks 0.
var apkSuffixRank = map[string]int{
	"alpha": -4, "beta": -3, "pre": -2, "rc": -1,
	"cvs": 1, "svn": 2, "git": 3, "hg": 4, "p": 5,
}

type apkSuffix struct {
	rank int
	num  string
}

type apkVersion struct {
	numbers  []string
	letter   string
	suffixes []apkSuffix
	revision string
}

func parseAPKVersion(version string) (apkVersion, error) {
	m := apkVersionPattern.FindStringSubmatch(version)
	if m == nil {
		return apkVersion{}, fmt.Errorf("not an apk version: %q", version)
	}

	v := apkVersion{letter: m[2], revision: trimLeadingZeros(m[4])}
	for _, n := range strings.Split(m[1], ".") {
		v.numbers = append(v.numbers, trimLeadingZeros(n))
	}
	for _, s := range apkSuffixPattern.FindAllStringSubmatch(m[3], -1) {
		v.suffixes = append(v.suffixes, apkSuffix{rank: apkSuffixRank[s[1]], num: trimLeadingZeros(s[2])})
	}
	return v, nil
}

// trimLeadingZeros normalizes a digit string; an empty or all-zero string is "0".
func trimLeadingZeros(s string) string {
	s = strings.TrimLeft(s, "0")
	if s == "" {
		return "0"
	}
	return s
}

// compareDigitStrings compares two normalized digit strings numerically
// without converting them, so arbitrarily long numbers cannot overflow.
func compareDigitStrings(a, b string) int {
	if len(a) != len(b) {
		if len(a) < len(b) {
			return -1
		}
		return 1
	}
	return strings.Compare(a, b)
}

// compareAPKVersions orders two apk versions: -1, 0 or 1.
func compareAPKVersions(left, right string) (int, error) {
	l, err := parseAPKVersion(left)
	if err != nil {
		return 0, err
	}
	r, err := parseAPKVersion(right)
	if err != nil {
		return 0, err
	}

	for i := 0; i < len(l.numbers) || i < len(r.numbers); i++ {
		ln, rn := "0", "0"
		if i < len(l.numbers) {
			ln = l.numbers[i]
		}
		if i < len(r.numbers) {
			rn = r.numbers[i]
		}
		if c := compareDigitStrings(ln, rn); c != 0 {
			return c, nil
		}
	}

	if c := strings.Compare(l.letter, r.letter); c != 0 {
		return c, nil
	}

	for i := 0; i < len(l.suffixes) || i < len(r.suffixes); i++ {
		ls, rs := apkSuffix{rank: 0, num: "0"}, apkSuffix{rank: 0, num: "0"}
		if i < len(l.suffixes) {
			ls = l.suffixes[i]
		}
		if i < len(r.suffixes) {
			rs = r.suffixes[i]
		}
		if ls.rank != rs.rank {
			if ls.rank < rs.rank {
				return -1, nil
			}
			return 1, nil
		}
		if c := compareDigitStrings(ls.num, rs.num); c != 0 {
			return c, nil
		}
	}

	return compareDigitStrings(l.revision, r.revision), nil
}

// isAPKEcosystem reports whether an OSV ecosystem name uses apk versions. OS
// ecosystems carry the release after a colon ("Alpine:v3.19"); only the name
// before it is considered.
func isAPKEcosystem(ecosystem string) bool {
	name := strings.ToLower(ecosystem)
	if i := strings.Index(name, ":"); i >= 0 {
		name = name[:i]
	}
	switch name {
	case "chainguard", "wolfi", "alpine", "alpaquita":
		return true
	}
	return false
}

// canEvaluateAPKRange reports whether the version and every boundary of the
// range are valid apk versions, so the apk ordering can be trusted. "0" is
// OSV's "from the beginning" marker and is handled by the range evaluator.
func canEvaluateAPKRange(version string, vrange models.Range) bool {
	if _, err := parseAPKVersion(version); err != nil {
		return false
	}

	for _, event := range vrange.Events {
		for _, boundary := range []string{event.Introduced, event.Fixed, event.LastAffected} {
			if boundary == "" || boundary == "0" {
				continue
			}
			if _, err := parseAPKVersion(boundary); err != nil {
				return false
			}
		}

		if event.Limit != "" && !strings.Contains(event.Limit, "*") {
			if _, err := parseAPKVersion(event.Limit); err != nil {
				return false
			}
		}
	}

	return true
}
