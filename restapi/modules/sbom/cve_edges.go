package sbom

import (
	"context"
	"fmt"
	"log"
	"strings"
	"sync"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/ortelius/v12/database"
	"github.com/ortelius/ortelius/v12/util"
)

// Lazy CVE edges.
//
// The OSV loader used to write a cve2purl edge for every package of every
// advisory, millions of them for packages no release contains. With lazy edges
// the backend writes a package's edges the first time an SBOM references it:
// ProcessSBOMComponents calls EnsureCVEEdgesForPurls once the SBOM's package
// nodes and sbom2purl edges exist, and LinkReleaseToExistingCVEs then matches
// as before.
//
// It is switched off by default (LAZY_CVE_EDGES=true turns it on) and, even
// when on, does nothing until the CVE documents are ready: the lookup depends on
// affected[].base_purl, which the loader and the repair tool write, and without
// that field it finds nothing -- after which the package would be marked as
// built with no edges. Readiness is the document metadata/lazy_cve_edges with
// ready=true, set once base_purl has been backfilled (see docs/implementation.md).
// Until then ingest behaves as it did before lazy edges existed.
//
// Each package node records cve_edges_version once its edges are built, so a
// package is processed once. Raising CVEEdgesVersion makes every package
// rebuild its edges the next time an SBOM references it, which is how a fix to
// the edge logic reaches existing data without a full backfill.
//
// Ordering guarantee against the loader: here the package node and its sbom2purl
// edges are written first, and only then are the CVEs looked up; the loader
// writes the CVE document first and only then looks for packages with an
// sbom2purl edge. Whichever side goes first, the edge gets written by one of
// them, and edge keys are deterministic, so doing it twice is harmless.

// CVEEdgesVersion is stored on a package node (cve_edges_version) when its
// cve2purl edges have been built from the CVE documents.
const CVEEdgesVersion = 1

const lazyEdgeUpsertBatch = 500

// LazyCVEEdgesEnabled reports whether lazy CVE edge materialization is on.
func LazyCVEEdgesEnabled() bool {
	return strings.EqualFold(util.GetEnvOrDefault("LAZY_CVE_EDGES", "false"), "true")
}

var notReadyLogOnce sync.Once

// lazyEdgesReady reports whether the CVE documents have been prepared for lazy
// edge lookup, i.e. metadata/lazy_cve_edges exists with ready=true.
func lazyEdgesReady(ctx context.Context, db database.DBConnection) (bool, error) {
	cursor, err := db.Database.Query(ctx, `RETURN { ready: DOCUMENT("metadata/lazy_cve_edges").ready == true }`, &arangodb.QueryOptions{})
	if err != nil {
		return false, err
	}
	defer cursor.Close()

	var res struct {
		Ready bool `json:"ready"`
	}
	if cursor.HasMore() {
		if _, err := cursor.ReadDocument(ctx, &res); err != nil {
			return false, err
		}
	}
	return res.Ready, nil
}

type pendingHub struct {
	ID   string `json:"id"`
	Purl string `json:"purl"`
}

type cveHit struct {
	Key     string             `json:"key"`
	Entries []cveAffectedEntry `json:"entries"`
}

// EnsureCVEEdgesForPurls builds the cve2purl edges of every package node in
// purlIDMap (base purl -> "purl/<key>") that has not been built at the current
// CVEEdgesVersion. It does nothing unless LAZY_CVE_EDGES is enabled.
func EnsureCVEEdgesForPurls(ctx context.Context, db database.DBConnection, purlIDMap map[string]string) error {
	if !LazyCVEEdgesEnabled() || len(purlIDMap) == 0 {
		return nil
	}

	ready, err := lazyEdgesReady(ctx, db)
	if err != nil {
		return fmt.Errorf("checking lazy CVE edge readiness: %w", err)
	}
	if !ready {
		notReadyLogOnce.Do(func() {
			log.Printf("lazy CVE edges: LAZY_CVE_EDGES is on but metadata/lazy_cve_edges is not ready; skipping until the CVE documents are backfilled")
		})
		return nil
	}

	ids := make([]string, 0, len(purlIDMap))
	for _, id := range purlIDMap {
		ids = append(ids, id)
	}

	pending, err := findPendingHubs(ctx, db, ids)
	if err != nil {
		return fmt.Errorf("finding packages without CVE edges: %w", err)
	}

	written := 0
	for _, hub := range pending {
		n, err := ensureHubEdges(ctx, db, hub)
		if err != nil {
			return fmt.Errorf("building CVE edges for %s: %w", hub.Purl, err)
		}
		written += n
	}

	if len(pending) > 0 {
		log.Printf("lazy CVE edges: built %d edge(s) for %d package(s)", written, len(pending))
	}
	return nil
}

func findPendingHubs(ctx context.Context, db database.DBConnection, ids []string) ([]pendingHub, error) {
	const query = `
		FOR id IN @ids
			LET h = DOCUMENT(id)
			FILTER h != null AND h.purl != null AND h.cve_edges_version != @version
			RETURN { id: h._id, purl: h.purl }
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"ids": ids, "version": CVEEdgesVersion},
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	var hubs []pendingHub
	for cursor.HasMore() {
		var hub pendingHub
		if _, err := cursor.ReadDocument(ctx, &hub); err != nil {
			return nil, err
		}
		hubs = append(hubs, hub)
	}
	return hubs, nil
}

// ensureHubEdges builds and writes the edges of one package node, then marks it
// as built. It returns how many edges it wrote.
func ensureHubEdges(ctx context.Context, db database.DBConnection, hub pendingHub) (int, error) {
	// FILTER ... IN c.affected[*].base_purl uses the cve_affected_base_purl index.
	const query = `
		FOR c IN cve
			FILTER @purl IN c.affected[*].base_purl
			RETURN {
				key: c._key,
				entries: (FOR a IN c.affected FILTER a.base_purl == @purl RETURN { ranges: a.ranges })
			}
	`
	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{"purl": hub.Purl},
	})
	if err != nil {
		return 0, err
	}
	defer cursor.Close()

	purlKey := util.SanitizeKey(hub.Purl)
	var edges []map[string]interface{}
	for cursor.HasMore() {
		var hit cveHit
		if _, err := cursor.ReadDocument(ctx, &hit); err != nil {
			return 0, err
		}
		if edge, ok := buildCVEPurlEdge(hit.Key, hub.ID, purlKey, hit.Entries); ok {
			edges = append(edges, edge)
		}
	}

	if err := upsertCVEEdges(ctx, db, edges); err != nil {
		return 0, err
	}

	// Mark last: a crash before this point leaves the package unmarked, so the
	// next SBOM that references it redoes the (idempotent) work.
	const markQuery = `UPDATE @key WITH { cve_edges_version: @version } IN purl`
	if _, err := db.Database.Query(ctx, markQuery, &arangodb.QueryOptions{
		BindVars: map[string]interface{}{
			"key":     strings.TrimPrefix(hub.ID, "purl/"),
			"version": CVEEdgesVersion,
		},
	}); err != nil {
		return 0, fmt.Errorf("marking package: %w", err)
	}

	return len(edges), nil
}

// upsertCVEEdges writes edges in batches. The edge _key is deterministic, so
// this updates an edge the loader already wrote instead of adding a second one.
func upsertCVEEdges(ctx context.Context, db database.DBConnection, edges []map[string]interface{}) error {
	const query = `
		FOR doc IN @docs
			UPSERT { _key: doc._key }
			INSERT doc
			UPDATE doc
			IN cve2purl
	`
	const maxAttempts = 3

	for start := 0; start < len(edges); start += lazyEdgeUpsertBatch {
		end := start + lazyEdgeUpsertBatch
		if end > len(edges) {
			end = len(edges)
		}

		backoff := 200 * time.Millisecond
		for attempt := 1; ; attempt++ {
			_, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
				BindVars: map[string]interface{}{"docs": edges[start:end]},
			})
			if err == nil {
				break
			}
			// Concurrent ingests of SBOMs sharing a package can contend for
			// the same edge documents; retry that, fail on anything else.
			if attempt >= maxAttempts || !strings.Contains(err.Error(), "timeout waiting to lock") {
				return fmt.Errorf("edge batch %d-%d: %w", start, end, err)
			}
			select {
			case <-time.After(backoff):
			case <-ctx.Done():
				return ctx.Err()
			}
			backoff *= 2
		}
	}
	return nil
}
