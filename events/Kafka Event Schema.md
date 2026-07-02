# Release SBOM Created — Kafka Event Schema Documentation

## Overview

This schema defines the event contract for asynchronous release ingestion in Ortelius. A producer publishes a `release.sbom.created` event referencing release metadata and a stored SBOM; the Kafka consumer (`internal/kafka.RunEventProcessor`) fetches the SBOM content and runs it through the **same** release-creation path (`ReleaseService.CreateRelease`) used by `POST /api/v1/releases` — including PURL extraction and CVE linking. There is no divergence between the REST and event-driven ingestion paths.

Topic: `release-events` · Consumer group: `ortelius-worker`.

> **Scope note:** This event carries release + SBOM data only. It does **not** carry endpoint or sync information — deployment state is recorded separately via `POST /api/v1/sync`. There is currently no Kafka event for sync/deployment events.

---

## Top-Level Properties

* **event_type** (string, Required): Identifies the event contract. Always `"release.sbom.created"` from the built-in producer (`ReleaseProducer.PublishReleaseSBOMCreated`).
* **event_id** (string, Required): A UUID generated per message for tracking.
* **event_time** (string, Required): ISO 8601 timestamp when the event was generated.
* **schema_version** (string, Required): Currently `"v1"`.

---

## 1. `release` Object

A `model.ProjectRelease` document, serialized directly. The consumer reads the following fields; any others present in the JSON are ignored by the event handler:

* **name** (string, Required): The full name of the release (e.g., `"org/repo"`).
* **version** (string, Required): The version string; re-parsed into SemVer components on ingest.
* **projecttype** (string, Optional): The category of project (e.g., `docker`, `container`, `git`).
* **gitcommit** (string, Optional): The Git SHA associated with the release.
* **dockersha** (string, Optional): The Docker image digest.
* **is_public** (boolean, Default: `true`): Visibility flag for the release — controls unauthenticated GraphQL access.

## 2. `sbom_ref` Object

Describes where the SBOM content is stored so the consumer can retrieve it before processing.

* **cid** (string, Required): Content identifier used to fetch the SBOM (e.g., an IPFS CID).
* **storage_type** (string, Required): Storage backend identifier. The built-in producer always sets this to `"ipfs"`.
* **bucket**, **object_key** (string, Optional): Present only when `storage_type` involves S3.
* **content_sha** (string, Optional): SHA256 hash of the SBOM content for integrity verification.
* **size_bytes** (integer, Optional): Size of the stored SBOM payload.
* **uploaded_at** (string, Required): Timestamp when the SBOM was persisted to the storage backend.

> **Implementation status:** The consumer's SBOM fetcher (`internal/services.CIDFetcher`) is currently a **placeholder** — it does not yet call out to IPFS or S3. It returns an empty `{"components":[]}` payload regardless of `cid`. Wire up real storage retrieval here before relying on this path in production; until then, releases ingested via Kafka will have no SBOM components.

---

## Validation

The consumer rejects a message (returns an error, does not retry) if any of `release.name`, `release.version`, or `sbom_ref.cid` is empty.

---

## Example JSON Payload

```json
{
  "event_type": "release.sbom.created",
  "event_id": "550e8400-e29b-41d4-a716-446655440000",
  "event_time": "2024-12-01T10:00:00Z",
  "schema_version": "v1",
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
    "uploaded_at": "2024-12-01T09:55:00Z"
  }
}
```

See also: [Architecture Guide — Kafka](../docs/architecture.md#kafka) for broker/topic configuration and deployment notes.
