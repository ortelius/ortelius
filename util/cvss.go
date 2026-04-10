// Package util provides utility functions for the backend.
//
//revive:disable-next-line:var-naming
package util

import (
	"strings"

	gocvss31 "github.com/pandatix/go-cvss/31"
	gocvss40 "github.com/pandatix/go-cvss/40"
)

// CalculateCVSSScore calculates the CVSS base score from a vector string
func CalculateCVSSScore(vectorStr string) float64 {
	if vectorStr == "" || !strings.HasPrefix(vectorStr, "CVSS:") {
		return 0
	}
	if strings.HasPrefix(vectorStr, "CVSS:3.1") || strings.HasPrefix(vectorStr, "CVSS:3.0") {
		if cvss31, err := gocvss31.ParseVector(vectorStr); err == nil {
			return cvss31.BaseScore()
		}
	}
	if strings.HasPrefix(vectorStr, "CVSS:4.0") {
		if cvss40, err := gocvss40.ParseVector(vectorStr); err == nil {
			return cvss40.Score()
		}
	}
	return 0
}

// AddCVSSScoresToContent adds CVSS scores and severity rating to CVE content
func AddCVSSScoresToContent(content map[string]interface{}) {
	severity, ok := content["severity"].([]interface{})
	var baseScores []float64
	var highestScore float64
	hasValidScore := false

	if ok && len(severity) > 0 {
		for _, sev := range severity {
			sevMap, ok := sev.(map[string]interface{})
			if !ok {
				continue
			}
			scoreStr, ok := sevMap["score"].(string)
			if !ok || scoreStr == "" {
				continue
			}
			sevType, _ := sevMap["type"].(string)
			if sevType == "CVSS_V3" || sevType == "CVSS_V4" {
				baseScore := CalculateCVSSScore(scoreStr)
				if baseScore > 0 {
					baseScores = append(baseScores, baseScore)
					hasValidScore = true
					if baseScore > highestScore {
						highestScore = baseScore
					}
				}
			}
		}
	}

	if !hasValidScore {
		highestScore = 0.1
		baseScores = []float64{0.1}
	}

	if content["database_specific"] == nil {
		content["database_specific"] = make(map[string]interface{})
	}
	dbSpecific := content["database_specific"].(map[string]interface{})
	dbSpecific["cvss_base_scores"] = baseScores
	dbSpecific["cvss_base_score"] = highestScore
	dbSpecific["severity_rating"] = GetSeverityRating(highestScore)
	content["database_specific"] = dbSpecific
}

// GetSeverityRating returns the severity rating for a given CVSS score
func GetSeverityRating(score float64) string {
	switch {
	case score == 0:
		return "NONE"
	case score < 4.0:
		return "LOW"
	case score < 7.0:
		return "MEDIUM"
	case score < 9.0:
		return "HIGH"
	default:
		return "CRITICAL"
	}
}
