package util

import (
	"testing"

	"github.com/google/osv-scanner/pkg/models"
)

func TestCompareAPKVersions(t *testing.T) {
	cases := []struct {
		left, right string
		want        int
	}{
		// the revision is numeric: r9 < r10 (semver orders these as text)
		{"3.3.12-r9", "3.3.12-r10", -1},
		{"3.3.12-r2", "3.3.12-r10", -1},
		{"2.42-r9", "2.42-r12", -1},
		{"3.3.12-r10", "3.3.12-r10", 0},
		{"3.3.12-r11", "3.3.12-r10", 1},
		// a missing revision counts as r0
		{"1.2.3", "1.2.3-r0", 0},
		{"1.2.3-r1", "1.2.3", 1},
		// numbers compare numerically, not as text
		{"2.10.0-r0", "2.9.9-r0", 1},
		{"1.2", "1.2.0", 0},
		// letters
		{"1.2a-r0", "1.2-r0", 1},
		{"1.2a-r0", "1.2b-r0", -1},
		// suffixes: pre-release < release < post-release
		{"1.2.3_rc1-r0", "1.2.3-r0", -1},
		{"1.2.3_alpha1-r0", "1.2.3_beta1-r0", -1},
		{"1.2.3_beta1-r0", "1.2.3_rc1-r0", -1},
		{"1.2.3_rc1-r0", "1.2.3_rc2-r0", -1},
		{"1.2.3_p1-r0", "1.2.3-r0", 1},
		{"1.2.3_git20240102-r0", "1.2.3-r0", 1},
		{"20240101-r5", "20240101-r40", -1},
	}
	for _, c := range cases {
		got, err := compareAPKVersions(c.left, c.right)
		if err != nil {
			t.Errorf("compareAPKVersions(%q, %q): unexpected error: %v", c.left, c.right, err)
			continue
		}
		if got != c.want {
			t.Errorf("compareAPKVersions(%q, %q) = %d, want %d", c.left, c.right, got, c.want)
		}
		// ordering must be antisymmetric
		back, _ := compareAPKVersions(c.right, c.left)
		if back != -c.want {
			t.Errorf("compareAPKVersions(%q, %q) = %d, want %d", c.right, c.left, back, -c.want)
		}
	}
}

func TestCompareAPKVersionsInvalid(t *testing.T) {
	for _, v := range []string{"", "abc", "1.2.3-beta", "1.2.3~git", "v1.2.3", "1..2"} {
		if _, err := compareAPKVersions(v, "1.0-r0"); err == nil {
			t.Errorf("compareAPKVersions(%q, ...) expected an error", v)
		}
	}
}

func TestIsVersionAffectedAPK(t *testing.T) {
	affected := func(ecosystem, fixed string) models.Affected {
		return models.Affected{
			Package: models.Package{Ecosystem: models.Ecosystem(ecosystem)},
			Ranges: []models.Range{{
				Type:   models.RangeEcosystem,
				Events: []models.Event{{Introduced: "0"}, {Fixed: fixed}},
			}},
		}
	}

	cases := []struct {
		ecosystem, fixed, installed string
		want                        bool
	}{
		{"Wolfi", "3.3.12-r10", "3.3.12-r9", true},
		{"Wolfi", "3.3.12-r10", "3.3.12-r2", true},
		{"Wolfi", "3.3.12-r10", "3.3.12-r10", false},
		{"Wolfi", "3.3.12-r10", "3.3.12-r11", false},
		{"Wolfi", "3.3.12-r10", "3.3.11-r5", true},
		{"Wolfi", "3.3.12-r10", "3.3.13-r0", false},
		{"Chainguard", "0.64.1-r1", "0.64.1-r0", true},
		{"Chainguard", "0.64.1-r1", "0.64.0-r9", true},
		{"Chainguard", "0.64.1-r1", "0.65.0-r0", false},
		{"Wolfi", "2.42-r12", "2.42-r9", true},
		{"Wolfi", "2.42-r12", "2.42-r12", false},
		{"Alpine", "1.2.3-r5", "1.2.3-r4", true},
		{"Alpine:v3.19", "1.2.3-r5", "1.2.3-r4", true},
		{"Alpine:v3.19", "1.2.3-r5", "1.2.3-r5", false},
		{"Alpaquita", "8.0.1-r10", "8.0.1-r9", true},
	}
	for _, c := range cases {
		if got := IsVersionAffected(c.installed, affected(c.ecosystem, c.fixed)); got != c.want {
			t.Errorf("IsVersionAffected(%q) with %s fixed in %q = %v, want %v", c.installed, c.ecosystem, c.fixed, got, c.want)
		}
	}
}

func TestIsAPKEcosystem(t *testing.T) {
	for _, e := range []string{"Chainguard", "Wolfi", "Alpine", "Alpaquita", "alpine:v3.19", "Wolfi:foo"} {
		if !isAPKEcosystem(e) {
			t.Errorf("isAPKEcosystem(%q) = false, want true", e)
		}
	}
	for _, e := range []string{"", "Debian", "Ubuntu:22.04:LTS", "npm", "PyPI", "AlpineX"} {
		if isAPKEcosystem(e) {
			t.Errorf("isAPKEcosystem(%q) = true, want false", e)
		}
	}
}
