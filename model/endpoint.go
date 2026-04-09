// Package model defines the data structures used by the pdvd-backend/v12,
// including releases, SBOMs, and endpoints.
package model

import "strings"

// EndpointType represents the type of deployment target
type EndpointType string

const (
	// EndpointTypeCluster represents a general compute cluster or Kubernetes environment.
	EndpointTypeCluster EndpointType = "cluster"
	// EndpointTypeEC2 represents an Amazon Elastic Compute Cloud instance.
	EndpointTypeEC2 EndpointType = "ec2"
	// EndpointTypeLambda represents an AWS Lambda function or equivalent serverless compute.
	EndpointTypeLambda EndpointType = "lambda"
	// EndpointTypeECS represents an Amazon Elastic Container Service environment.
	EndpointTypeECS EndpointType = "ecs"
	// EndpointTypeEKS represents an Amazon Elastic Kubernetes Service cluster.
	EndpointTypeEKS EndpointType = "eks"
	// EndpointTypeGKE represents a Google Kubernetes Engine cluster.
	EndpointTypeGKE EndpointType = "gke"
	// EndpointTypeAKS represents an Azure Kubernetes Service cluster.
	EndpointTypeAKS EndpointType = "aks"
	// EndpointTypeFargate represents an AWS Fargate serverless container service.
	EndpointTypeFargate EndpointType = "fargate"

	// Edge and IoT Devices

	// EndpointTypeEdge represents a deployment target at the edge of a network (e.g., local gateways).
	EndpointTypeEdge EndpointType = "edge"
	// EndpointTypeIoT represents an Internet of Things device.
	EndpointTypeIoT EndpointType = "iot"

	// Mission Assets - Military and Defense

	// EndpointTypeMissionAsset represents a critical deployment target in defense or security contexts.
	EndpointTypeMissionAsset EndpointType = "mission_asset"
)

// Endpoint represents a deployment target for syncing releases
type Endpoint struct {
	Key          string       `json:"_key,omitempty"`    // Unique identifier of the endpoint in the database.
	Name         string       `json:"name"`              // Human-readable name of the endpoint (e.g., "production-us-east-1").
	EndpointType EndpointType `json:"endpoint_type"`     // The specific type of infrastructure (e.g., "eks", "lambda").
	Environment  string       `json:"environment"`       // The environment designation (e.g., "staging", "production").
	ObjType      string       `json:"objtype,omitempty"` // The object type for database indexing (should be "Endpoint").

	// Parsed name components
	Org       string   `json:"org,omitempty"`       // Organization name parsed from name field
	Path      []string `json:"path,omitempty"`      // Path components parsed from name field
	Shortname string   `json:"shortname,omitempty"` // Short name parsed from name field
	IsPublic  bool     `json:"is_public"`           // Whether the endpoint is public (default: true)
}

// NewEndpoint creates a new Endpoint instance with default values
func NewEndpoint() *Endpoint {
	return &Endpoint{
		ObjType:  "Endpoint",
		IsPublic: true,
		Path:     []string{},
	}
}

// ParseAndSetNameComponents parses the Name field and populates org, path, shortname, and is_public.
// Org is only derived from the name if the caller has not already set it explicitly. This allows
// callers such as the GKE sync function to supply a meaningful org (e.g. a namespace or mapped
// org value) that is independent of the structural segments in the name (e.g. "cluster-2/kube-system"
// has cluster name as parts[0], not the org).
func (e *Endpoint) ParseAndSetNameComponents() {
	parts := strings.Split(e.Name, "/")

	if len(parts) > 1 {
		if e.Org == "" {
			e.Org = parts[0]
		}
		e.Shortname = parts[1]
	} else {
		if e.Org == "" {
			e.Org = "library"
		}
		e.Shortname = e.Name
	}

	e.Path = []string{}
	e.IsPublic = true
}
