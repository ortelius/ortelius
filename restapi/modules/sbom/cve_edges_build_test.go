package sbom

import "testing"

func rng(typ string, events ...map[string]interface{}) cveRange {
	return cveRange{Type: typ, Events: events}
}

func ev(kind, version string) map[string]interface{} {
	return map[string]interface{}{kind: version}
}

func TestBuildCVEPurlEdgeSemver(t *testing.T) {
	entries := []cveAffectedEntry{{Ranges: []cveRange{rng("SEMVER", ev("introduced", "1.2.0"), ev("fixed", "1.2.5"))}}}
	edge, ok := buildCVEPurlEdge("GHSA-x", "purl/abc", "pkg-npm-foo", entries)
	if !ok {
		t.Fatal("expected an edge")
	}
	want := map[string]interface{}{
		"_key": "GHSA-x_pkg-npm-foo", "_from": "cve/GHSA-x", "_to": "purl/abc", "type": "SEMVER",
		"introduced": "1.2.0", "fixed": "1.2.5", "last_affected": "",
		"introduced_major": 1, "introduced_minor": 2, "introduced_patch": 0,
		"fixed_major": 1, "fixed_minor": 2, "fixed_patch": 5,
	}
	for k, v := range want {
		if edge[k] != v {
			t.Errorf("edge[%q] = %v, want %v", k, edge[k], v)
		}
	}
	if _, present := edge["last_affected_major"]; present {
		t.Error("last_affected_major should be omitted when last_affected is empty")
	}
}

func TestBuildCVEPurlEdgeEcosystemHasNullParts(t *testing.T) {
	entries := []cveAffectedEntry{{Ranges: []cveRange{rng("ECOSYSTEM", ev("introduced", "0"), ev("fixed", "3.3.12-r10"))}}}
	edge, ok := buildCVEPurlEdge("CGA-1", "purl/x", "pkg-apk-wolfi-ruby", entries)
	if !ok {
		t.Fatal("expected an edge")
	}
	for _, k := range cveEdgeVersionKeys {
		v, present := edge[k]
		if !present || v != nil {
			t.Errorf("edge[%q] = %v (present=%v), want an explicit nil", k, v, present)
		}
	}
	if edge["type"] != "ECOSYSTEM" || edge["fixed"] != "3.3.12-r10" || edge["introduced"] != "0" {
		t.Errorf("unexpected edge: %v", edge)
	}
}

func TestBuildCVEPurlEdgeEcosystemEntryNullsSemverRangeToo(t *testing.T) {
	entries := []cveAffectedEntry{{Ranges: []cveRange{
		rng("SEMVER", ev("introduced", "1.0.0"), ev("fixed", "1.0.9")),
		rng("ECOSYSTEM", ev("introduced", "1.0.0"), ev("fixed", "1.0.9")),
	}}}
	edge, _ := buildCVEPurlEdge("C", "purl/x", "k", entries)
	for _, k := range cveEdgeVersionKeys {
		if v, present := edge[k]; !present || v != nil {
			t.Errorf("edge[%q] = %v, want nil when the entry has an ECOSYSTEM range", k, v)
		}
	}
}

func TestBuildCVEPurlEdgeSkipsGitAndEmpty(t *testing.T) {
	if _, ok := buildCVEPurlEdge("C", "purl/x", "k", []cveAffectedEntry{{Ranges: []cveRange{rng("GIT", ev("introduced", "abc123"))}}}); ok {
		t.Error("GIT ranges must not produce an edge")
	}
	if _, ok := buildCVEPurlEdge("C", "purl/x", "k", nil); ok {
		t.Error("no entries must not produce an edge")
	}
	if _, ok := buildCVEPurlEdge("C", "purl/x", "k", []cveAffectedEntry{{}}); ok {
		t.Error("an entry without ranges must not produce an edge")
	}
}

func TestBuildCVEPurlEdgeIntroducedDefaultsToZero(t *testing.T) {
	entries := []cveAffectedEntry{{Ranges: []cveRange{rng("SEMVER", ev("fixed", "2.0.0"))}}}
	edge, ok := buildCVEPurlEdge("C", "purl/x", "k", entries)
	if !ok || edge["introduced"] != "0" {
		t.Errorf("introduced = %v, want \"0\"", edge["introduced"])
	}
}

func TestBuildCVEPurlEdgeLastRangeWins(t *testing.T) {
	entries := []cveAffectedEntry{{Ranges: []cveRange{
		rng("SEMVER", ev("introduced", "1.0.0"), ev("fixed", "1.1.0")),
		rng("SEMVER", ev("introduced", "2.0.0"), ev("fixed", "2.1.0")),
	}}}
	edge, _ := buildCVEPurlEdge("C", "purl/x", "k", entries)
	if edge["introduced"] != "2.0.0" || edge["fixed"] != "2.1.0" {
		t.Errorf("expected the last range to win, got %v", edge)
	}
}
