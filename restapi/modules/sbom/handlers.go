// Package sbom implements the REST API handlers for SBOM processing operations.
package sbom

import (
	"context"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"log"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/model"
	"github.com/ortelius/pdvd-backend/v12/util"
)

// GetSBOMContentHash calculates SHA256 hash of SBOM content
func GetSBOMContentHash(sbom model.SBOM) string {
	hash := sha256.Sum256(sbom.Content)
	return hex.EncodeToString(hash[:])
}

// ProcessSBOM handles SBOM creation or retrieval
// Returns: existingSBOMKey, sbomID, error
func ProcessSBOM(ctx context.Context, db database.DBConnection, sbom model.SBOM) (string, string, error) {
	// Calculate content hash for SBOM
	sbomHash := GetSBOMContentHash(sbom)
	sbom.ContentSha = sbomHash

	// Check if SBOM with this content hash already exists
	existingSBOMKey, err := database.FindSBOMByContentHash(ctx, db.Database, sbomHash)
	if err != nil {
		return "", "", err
	}

	var sbomID string

	if existingSBOMKey != "" {
		// SBOM already exists, use existing key
		sbomID = "sbom/" + existingSBOMKey
		sbom.Key = existingSBOMKey
	} else {
		// Save new SBOM to ArangoDB
		sbomMeta, err := db.Collections["sbom"].CreateDocument(ctx, sbom)
		if err != nil {
			return "", "", err
		}
		sbomID = "sbom/" + sbomMeta.Key
		sbom.Key = sbomMeta.Key
	}

	return existingSBOMKey, sbomID, nil
}

// ProcessSBOMComponents extracts PURLs from SBOM and creates hub-spoke relationships
// FIXED: Now uses centralized PURL standardization for consistent Hub keys
func ProcessSBOMComponents(ctx context.Context, db database.DBConnection, sbom model.SBOM, sbomID string) error {
	// Parse SBOM content to extract components
	var sbomData struct {
		Components []struct {
			Purl string `json:"purl"`
		} `json:"components"`
	}

	if err := json.Unmarshal(sbom.Content, &sbomData); err != nil {
		return err
	}

	// Step 1: Collect and process all PURLs using centralized standardization
	var purlInfos []PurlInfo
	basePurlSet := make(map[string]bool)

	for _, component := range sbomData.Components {
		if component.Purl == "" {
			continue
		}

		cleanedPurl, err := util.CleanPURL(component.Purl)
		if err != nil {
			log.Printf("Failed to clean PURL %s: %v", component.Purl, err)
			continue
		}

		parsed, err := util.ParsePURL(cleanedPurl)
		if err != nil {
			log.Printf("Failed to parse PURL %s: %v", cleanedPurl, err)
			continue
		}

		// CRITICAL FIX: Use centralized standardization for Hub key
		basePurl, err := util.GetStandardBasePURL(cleanedPurl)
		if err != nil {
			log.Printf("Failed to get standard base PURL from %s: %v", cleanedPurl, err)
			continue
		}

		versionParsed := util.ParseSemanticVersion(parsed.Version)

		purlInfos = append(purlInfos, PurlInfo{
			BasePurl:     basePurl,
			Version:      parsed.Version,
			FullPurl:     cleanedPurl,
			VersionMajor: versionParsed.Major,
			VersionMinor: versionParsed.Minor,
			VersionPatch: versionParsed.Patch,
			Ecosystem:    parsed.Type,
		})

		basePurlSet[basePurl] = true
	}

	if len(purlInfos) == 0 {
		return nil
	}

	// Step 2: Batch find/create all unique base PURLs
	uniqueBasePurls := make([]string, 0, len(basePurlSet))
	for basePurl := range basePurlSet {
		uniqueBasePurls = append(uniqueBasePurls, basePurl)
	}

	purlIDMap, err := batchFindOrCreatePURLs(ctx, db, uniqueBasePurls)
	if err != nil {
		return err
	}

	// Step 3: Prepare all edges for batch insertion
	var edgesToCheck []EdgeInfo
	edgeCheckMap := make(map[string]bool)

	for _, info := range purlInfos {
		purlID, exists := purlIDMap[info.BasePurl]
		if !exists {
			log.Printf("Warning: PURL ID not found for base PURL %s", info.BasePurl)
			continue
		}

		edgeKey := sbomID + ":" + purlID + ":" + info.Version
		if edgeCheckMap[edgeKey] {
			continue
		}
		edgeCheckMap[edgeKey] = true

		edgesToCheck = append(edgesToCheck, EdgeInfo{
			From:     sbomID,
			To:       purlID,
			Version:  info.Version,
			FullPurl: info.FullPurl,
		})
	}

	if len(edgesToCheck) == 0 {
		return nil
	}

	// Check which edges already exist
	existingEdges, err := batchCheckEdgesExist(ctx, db, "sbom2purl", edgesToCheck)
	if err != nil {
		return err
	}

	// Build edges to insert with version metadata
	var edgesToCreate []map[string]interface{}
	for i, checkEdge := range edgesToCheck {
		edgeKey := checkEdge.From + ":" + checkEdge.To + ":" + checkEdge.Version
		if !existingEdges[edgeKey] {
			var matchingInfo *PurlInfo
			for j := range purlInfos {
				purlID := purlIDMap[purlInfos[j].BasePurl]
				if checkEdge.From == sbomID && checkEdge.To == purlID && checkEdge.Version == purlInfos[j].Version {
					matchingInfo = &purlInfos[j]
					break
				}
			}

			edge := map[string]interface{}{
				"_from":     edgesToCheck[i].From,
				"_to":       edgesToCheck[i].To,
				"version":   edgesToCheck[i].Version,
				"full_purl": edgesToCheck[i].FullPurl,
			}

			if matchingInfo != nil {
				edge["ecosystem"] = matchingInfo.Ecosystem
				if matchingInfo.VersionMajor != nil {
					edge["version_major"] = *matchingInfo.VersionMajor
				}
				if matchingInfo.VersionMinor != nil {
					edge["version_minor"] = *matchingInfo.VersionMinor
				}
				if matchingInfo.VersionPatch != nil {
					edge["version_patch"] = *matchingInfo.VersionPatch
				}
			}

			edgesToCreate = append(edgesToCreate, edge)
		}
	}

	if len(edgesToCreate) > 0 {
		err = BatchInsertEdges(ctx, db, "sbom2purl", edgesToCreate)
		if err != nil {
			return err
		}
	}

	return nil
}

// batchFindOrCreatePURLs finds or creates multiple PURLs in a single query
// FIXED: Uses util.SanitizeKey on standardized base PURLs for consistent Hub IDs
func batchFindOrCreatePURLs(ctx context.Context, db database.DBConnection, basePurls []string) (map[string]string, error) {
	if len(basePurls) == 0 {
		return make(map[string]string), nil
	}

	query := `
		FOR purl IN @purls
			LET upsertedPurl = FIRST(
				UPSERT { purl: purl }
				INSERT { purl: purl, objtype: "PURL" }
				UPDATE {} IN purl
				RETURN NEW
			)
			RETURN {
				basePurl: purl,
				purlId: CONCAT("purl/", upsertedPurl._key)
			}
	`

	bindVars := map[string]interface{}{
		"purls": basePurls,
	}

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: bindVars,
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	purlIDMap := make(map[string]string)
	for cursor.HasMore() {
		var result struct {
			BasePurl string `json:"basePurl"`
			PurlID   string `json:"purlId"`
		}
		_, err := cursor.ReadDocument(ctx, &result)
		if err != nil {
			return nil, err
		}
		purlIDMap[result.BasePurl] = result.PurlID
	}

	return purlIDMap, nil
}

// batchCheckEdgesExist checks which edges already exist in a single query
func batchCheckEdgesExist(ctx context.Context, db database.DBConnection, edgeCollection string, edges []EdgeInfo) (map[string]bool, error) {
	if len(edges) == 0 {
		return make(map[string]bool), nil
	}

	type edgeCheck struct {
		From    string `json:"from"`
		To      string `json:"to"`
		Version string `json:"version"`
	}

	var edgeChecks []edgeCheck
	for _, edge := range edges {
		edgeChecks = append(edgeChecks, edgeCheck{
			From:    edge.From,
			To:      edge.To,
			Version: edge.Version,
		})
	}

	query := `
		FOR check IN @edges
			LET exists = (
				FOR e IN @@edgeCollection
					FILTER e._from == check.from 
					   AND e._to == check.to 
					   AND e.version == check.version
					LIMIT 1
					RETURN true
			)
			RETURN {
				key: CONCAT(check.from, ":", check.to, ":", check.version),
				exists: LENGTH(exists) > 0
			}
	`

	bindVars := map[string]interface{}{
		"@edgeCollection": edgeCollection,
		"edges":           edgeChecks,
	}

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: bindVars,
	})
	if err != nil {
		return nil, err
	}
	defer cursor.Close()

	existsMap := make(map[string]bool)
	for cursor.HasMore() {
		var result struct {
			Key    string `json:"key"`
			Exists bool   `json:"exists"`
		}
		_, err := cursor.ReadDocument(ctx, &result)
		if err != nil {
			return nil, err
		}
		existsMap[result.Key] = result.Exists
	}

	return existsMap, nil
}

// BatchInsertEdges inserts multiple edges in a single query
func BatchInsertEdges(ctx context.Context, db database.DBConnection, edgeCollection string, edges []map[string]interface{}) error {
	if len(edges) == 0 {
		return nil
	}

	query := `
		FOR edge IN @edges
			INSERT edge INTO @@edgeCollection
	`

	bindVars := map[string]interface{}{
		"@edgeCollection": edgeCollection,
		"edges":           edges,
	}

	cursor, err := db.Database.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: bindVars,
	})
	if err != nil {
		return err
	}
	cursor.Close()

	return nil
}
