package util

import (
	"testing"

	"github.com/google/osv-scanner/pkg/models"
)

func rangeAffected(ecosystem, introduced, fixed string) models.Affected {
	return models.Affected{
		Package: models.Package{Ecosystem: models.Ecosystem(ecosystem)},
		Ranges: []models.Range{{
			Type:   models.RangeEcosystem,
			Events: []models.Event{{Introduced: introduced}, {Fixed: fixed}},
		}},
	}
}

// The npm parser only accepts full major.minor.patch versions. A range that
// writes a boundary any other way ("16.3", "v16.3.0") used to make the whole
// range evaluate as "not affected"; it now falls back to the lenient semver
// comparison.
func TestIsVersionAffectedNPMLenientBoundaries(t *testing.T) {
	cases := []struct {
		name, introduced, fixed, installed string
		want                               bool
	}{
		{"two-part fixed, vulnerable", "0", "16.3", "16.2.1", true},
		{"two-part fixed, vulnerable 2", "0", "16.3", "15.9.9", true},
		{"two-part fixed, at fix", "0", "16.3", "16.3.0", false},
		{"two-part fixed, after fix", "0", "16.3", "16.4.3", false},
		{"v-prefixed fixed, vulnerable", "0", "v16.3.0", "16.2.1", true},
		{"v-prefixed fixed, after fix", "0", "v16.3.0", "16.3.1", false},
		{"two-part introduced", "16.2", "16.3.0", "16.2.1", true},
		{"two-part introduced, before it", "16.2", "16.3.0", "16.1.9", false},
		// ranges the npm parser accepts keep using it
		{"plain range, vulnerable", "1.0.0", "1.0.5", "1.0.4", true},
		{"plain range, fixed", "1.0.0", "1.0.5", "1.0.5", false},
		{"plain range, before introduced", "1.0.0", "1.0.5", "0.9.0", false},
	}
	for _, c := range cases {
		for _, eco := range []string{"npm", "NPM"} {
			if got := IsVersionAffected(c.installed, rangeAffected(eco, c.introduced, c.fixed)); got != c.want {
				t.Errorf("%s [%s]: IsVersionAffected(%q) with introduced %q fixed %q = %v, want %v",
					c.name, eco, c.installed, c.introduced, c.fixed, got, c.want)
			}
		}
	}
}

func TestCanEvaluateRange(t *testing.T) {
	strict := func(v string) error {
		if v == "bad" {
			return errBadVersion
		}
		return nil
	}
	rng := func(events ...models.Event) models.Range { return models.Range{Events: events} }

	cases := []struct {
		name    string
		version string
		r       models.Range
		want    bool
	}{
		{"all valid", "1.0.0", rng(models.Event{Introduced: "0"}, models.Event{Fixed: "2.0.0"}), true},
		{"invalid version", "bad", rng(models.Event{Introduced: "0"}), false},
		{"invalid fixed", "1.0.0", rng(models.Event{Introduced: "0"}, models.Event{Fixed: "bad"}), false},
		{"invalid introduced", "1.0.0", rng(models.Event{Introduced: "bad"}), false},
		{"invalid last_affected", "1.0.0", rng(models.Event{Introduced: "0"}, models.Event{LastAffected: "bad"}), false},
		{"invalid limit", "1.0.0", rng(models.Event{Introduced: "0"}, models.Event{Limit: "bad"}), false},
		{"wildcard limit is not parsed", "1.0.0", rng(models.Event{Introduced: "0"}, models.Event{Limit: "*"}), true},
		{"zero boundary is not parsed", "1.0.0", rng(models.Event{Introduced: "0"}), true},
	}
	for _, c := range cases {
		if got := canEvaluateRange(c.version, c.r, strict); got != c.want {
			t.Errorf("%s: canEvaluateRange = %v, want %v", c.name, got, c.want)
		}
	}
}

type badVersionError struct{}

func (badVersionError) Error() string { return "bad version" }

var errBadVersion error = badVersionError{}
