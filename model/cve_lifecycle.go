// Package model - CVE lifecycle tracking for MTTR analysis
package model

import "time"

// CVELifecycleEvent tracks the lifecycle of a CVE across endpoints
type CVELifecycleEvent struct {
	Key                      string     `json:"_key,omitempty"`
	CveID                    string     `json:"cve_id"`                     // e.g., "CVE-2024-1234"
	EndpointName             string     `json:"endpoint_name"`              // e.g., "production-us-east-1"
	ReleaseName              string     `json:"release_name"`               // e.g., "payment-service"
	Package                  string     `json:"package"`                    // e.g., "pkg:npm/lodash"
	SeverityRating           string     `json:"severity_rating"`            // e.g., "CRITICAL"
	SeverityScore            float64    `json:"severity_score"`             // e.g., 9.8
	IntroducedAt             time.Time  `json:"introduced_at"`              // When CVE first appeared
	IntroducedVersion        string     `json:"introduced_version"`         // Release version when introduced
	RemediatedAt             *time.Time `json:"remediated_at"`              // When CVE was fixed (null if still open)
	RemediatedVersion        *string    `json:"remediated_version"`         // Release version when fixed
	DaysToRemediate          *float64   `json:"days_to_remediate"`          // Calculated: RemediatedAt - IntroducedAt
	IsRemediated             bool       `json:"is_remediated"`              // Quick filter flag
	DisclosedAfterDeployment bool       `json:"disclosed_after_deployment"` // true = CVE disclosed after deployment
	ObjType                  string     `json:"objtype"`                    // "CVELifecycleEvent"
	CreatedAt                time.Time  `json:"created_at"`
	UpdatedAt                time.Time  `json:"updated_at"`
}

// NewCVELifecycleEvent creates a new CVE lifecycle event
func NewCVELifecycleEvent() *CVELifecycleEvent {
	now := time.Now()
	return &CVELifecycleEvent{
		ObjType:                  "CVELifecycleEvent",
		IsRemediated:             false,
		DisclosedAfterDeployment: false,
		CreatedAt:                now,
		UpdatedAt:                now,
	}
}
