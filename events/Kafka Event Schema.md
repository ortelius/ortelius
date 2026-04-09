# Release Sync Kafka Event Schema Documentation

## Overview

This schema defines a unified event contract for the PDVD system. It combines software release metadata, storage references for SBOMs, and deployment endpoint information into a single message. This structure ensures that events processed via Kafka trigger the same validation, deduplication, and automated CVE linking logic as the REST API.

---

## Top-Level Properties

* **event_type** (string, Required): Unique identifier for the event logic (e.g., `release.sync.created`).
* **event_id** (string, Required): A unique UUID for message tracking and deduplication.
* **event_time** (string, Required): The ISO 8601 timestamp when the event was generated.
* **synced_at** (string, Optional): The ISO 8601 timestamp of the actual deployment. If omitted, the system defaults to the current processing time.

---

## 1. Release Object

Contains core metadata for the software component.

* **name** (string, Required): The full name of the release (e.g., "org/repo").
* **version** (string, Required): The version string, automatically cleaned and parsed into SemVer components during ingestion.
* **projecttype** (string, Optional): The category of project (e.g., `docker`, `container`, `git`).
* **gitcommit** (string, Optional): The Git SHA associated with the release.
* **dockersha** (string, Optional): The Docker Image Digest.
* **is_public** (boolean, Default: `true`): Visibility flag for the release.

---

## 2. SBOM Reference Object

Describes how to retrieve the SBOM content for security analysis.

* **cid** (string, Required): The IPFS Content Identifier where the JSON SBOM is stored.
* **storage_type** (string, Required): The backend storage provider. Allowed values: `["ipfs", "s3"]`.
* **content_sha** (string, Optional): A SHA256 hash of the SBOM content for integrity verification.
* **uploaded_at** (string, Optional): Timestamp when the SBOM was persisted to the storage backend.

---

## 3. Endpoint Object

Defines the deployment target for MTTR and lifecycle tracking.

* **name** (string, Required): Unique name of the environment or cluster (e.g., "prod-us-east-1").
* **endpoint_type** (string, Required): The infrastructure category. Supported types include `eks`, `lambda`, `gke`, `fargate`, `iot`, and `mission_asset`.
* **environment** (string, Required): The environment designation (e.g., `production`, `staging`).
* **is_public** (boolean, Default: `true`): Visibility flag for the endpoint.

---

## Example JSON Payload

```json
{
  "event_type": "release.sync.created",
  "event_id": "550e8400-e29b-41d4-a716-446655440000",
  "event_time": "2023-10-27T10:00:00Z",
  "synced_at": "2023-10-27T09:55:00Z",
  "release": {
    "name": "ortelius/reporting-service",
    "version": "v1.2.3",
    "projecttype": "docker",
    "gitcommit": "af32c1b",
    "dockersha": "sha256:45b34006...77a",
    "is_public": true
  },
  "sbom_ref": {
    "cid": "QmXoypizjW3WknFiJnKLwHCnL72vedxjQkDDP1mXWo6uco",
    "storage_type": "ipfs",
    "content_sha": "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
    "uploaded_at": "2023-10-27T09:50:00Z"
  },
  "endpoint": {
    "name": "production-cluster-01",
    "endpoint_type": "eks",
    "environment": "production",
    "is_public": true
  }
}
```
