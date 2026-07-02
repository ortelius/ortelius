# Getting Started

When a critical open source vulnerability is disclosed, most security teams face the same four questions in rapid succession — and struggle to answer any of them quickly:

- **What exactly is the threat?** The CVE ID, CVSS score, and enough detail to know whether it's exploitable in your environment.
- **Where in your codebase do you fix it?** Which repo, which package, and which version introduced the vulnerable dependency.
- **Where is that code actually running right now?** Not where it was deployed last quarter — where it is live in production today, whether that's a Kubernetes cluster, a cloud function, or an edge device in the field.
- **How do you actually fix it?** The upgraded version and a clear remediation path so nothing falls through the cracks.

Ortelius answers all four questions in a single platform. It ingests your Software Bill of Materials (SBOM) at build time, matches your deployed components against the OSV vulnerability database every 15 minutes, tracks every CVE from the moment it is introduced into a release through every environment it reaches until it is remediated, and measures how long each step takes against your SLA targets.

---

## TLDR; (Hosted)

The fastest way to get started is the hosted version at **[app.deployhub.com](https://app.deployhub.com)** — no infrastructure setup required.

1. Go to [app.deployhub.com](https://app.deployhub.com) and click **Sign Up**
2. Enter your username, email, first name, last name, and organization name
3. Check your email for an invitation link — click it to set your password and log in
4. You're in. Your organization is created and you are its owner.

> **Note:** If your organization already exists in the system, you'll see a conflict message with the org admin's email address. Contact them directly to be added.

---

## Connect Your GitHub Repositories

Once logged in, connect your GitHub account. This is the fastest way to get repository and release metadata into Ortelius, though — see the note at the end of this section — it does **not** by itself give you vulnerability data.

### Step 1 — Connect GitHub

1. In the top-right menu, go to **Profile → Connect GitHub**
2. You'll be redirected to GitHub to install the Ortelius GitHub App on your account or organization
3. Select which repositories to give Ortelius access to — you can start with one and add more later
4. After approving, you'll be redirected back to Ortelius with GitHub connected

### Step 2 — Onboard Repositories

From **Profile → GitHub Integration**, pick which of your installed repositories to onboard (`POST /api/v1/github/onboard`). For each selected `owner/repo`, Ortelius:

1. **Fetches GitHub Releases** for the repo via the App installation token and creates a `release` record per release tag (version = tag name; visibility is set from the repo's public/private status)
2. **Fetches recent GitHub Actions workflow runs** and, for each run that completed successfully, creates a `github-actions/<owner>/<repo>` endpoint and a `sync` record (release version = first 7 characters of the run's head commit SHA)

This gives you a release and deployment timeline for the repo. Repeated onboarding calls simply re-run this same import — this REST endpoint itself has no automatic SBOM discovery, container image inspection, or OpenSSF Scorecard lookup. (A separate, optionally-deployed component, [`relscanner-job`](https://github.com/ortelius/relscanner-job), *does* provide a persistent background scanner with exactly those capabilities — see [Why Am I Seeing Zero CVEs?](#why-am-i-seeing-zero-cves) below.)

### Step 3 — Verify Your Data

After onboarding, go to your dashboard. You should see:

- Your repositories listed under **Releases**, with a version per GitHub release
- `github-actions/<owner>/<repo>` listed as an endpoint under **Endpoints**, if any workflow runs succeeded

#### Why Am I Seeing Zero CVEs?

GitHub onboarding (above) imports release and deployment *metadata* — it does not attach an SBOM, so there is nothing yet for CVE matching to run against. To get vulnerability data for a release, an SBOM needs to reach it by one of these paths:

1. **Automatically, via a scanner component** — if your deployment runs the [`relscanner-job`](https://github.com/ortelius/relscanner-job) CronJob (discovers releases from GitHub/GitLab CI and auto-attaches an SBOM via OCI attestations, Cosign, GitHub Release assets, or Syft/cdxgen generation) or the [`deployment-gke`](https://github.com/ortelius/deployment-gke) Cloud Function (does the same for pods observed running in GKE), this happens for you with no CI changes required. Ask your platform team whether either is deployed alongside this backend.
2. **From your own CI pipeline** — upload an SBOM for the release via `POST /api/v1/releases` (see [Implementation Guide](docs/implementation.md#releases)), typically from a CI step using Syft, Trivy, or any CycloneDX-compatible tool, matched to the release by `name` + `version`
3. **Sync that release to an endpoint** via `POST /api/v1/sync`, if it isn't already covered by a GitHub Actions endpoint created during onboarding, or by an endpoint one of the scanner components created

CVE data is refreshed from OSV.dev every 15 minutes. Components with missing or malformed PURLs are silently skipped during CVE matching.

---

### Getting the Best Results

If neither scanner component above is deployed, have your CI pipeline call the REST API directly — this is the most direct way to get SBOM-based CVE matching without additional infrastructure. A typical pipeline step:

```yaml
# Example: generate an SBOM with Syft and post it to Ortelius
- name: Generate SBOM
  run: syft ${{ env.IMAGE_NAME }}:${{ env.IMAGE_TAG }} -o cyclonedx-json --file sbom.json

- name: Upload release + SBOM to Ortelius
  run: |
    curl -X POST "$ORTELIUS_URL/api/v1/releases" \
      -H "Content-Type: application/json" \
      -d @release-payload.json   # release fields + sbom.content = contents of sbom.json
```

**Supported ecosystems for CVE matching:** npm, PyPI, Maven, Go, NuGet, RubyGems, cargo (crates.io), Composer, apk (Alpine/Wolfi), deb (Debian/Ubuntu)

> **Using a different CI system or want direct API access?** See the [Implementation Guide](docs/implementation.md) for REST API reference.

---

## Read Your Dashboard

The dashboard is organized into five sections. All metrics use a **rolling 180-day window** and reflect the **NIST Recommended SLA Policy** (Critical 15d · High 30d · Medium 90d · Low 180d) unless you configure a different policy.

### Top Cards — Executive Summary

Five cards give you the headline numbers at a glance:

| Card                         | What it means                                                                     | Calculation                                                                                                                                                  |
|------------------------------|-----------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Total New CVEs**           | Total vulnerabilities detected within the rolling 180-day window                  | CVEs where Detected Date is within the last 180 days                                                                                                         |
| **Post-Deploy CVEs**         | Currently open CVEs that were disclosed *after* the software was already deployed | Open CVEs where Disclosure Date > Deployment date                                                                                                            |
| **MTTR (Pre + Post Deploy)** | Average days to remediate across all endpoint CVEs fixed in the period            | Σ(Fix Date − First Introduced Date) / Total Fixed CVEs                                                                                                       |
| **MTTR (Post-Deploy)**       | Average days to remediate for post-deployment CVEs only                           | Σ(Fix Date − First Introduced Date) / Total Fixed Post-Deploy CVEs. Clock starts at `root_introduced_at` — the first known version where the CVE was present |
| **% Open > SLA**             | Percentage of open CVEs exceeding their severity-based SLA                        | (Count of Open CVEs > SLA / Total Open CVEs) × 100                                                                                                           |

### Severity Breakdown & SLA Compliance

A table segmented by Critical, High, Medium, and Low with six columns per row:

| Column             | What it means                                                                           |
|--------------------|-----------------------------------------------------------------------------------------|
| **MTTR (Days)**    | Σ(Fix − Detect) / Fixed — average remediation time for closed CVEs at this severity     |
| **MTTR (Post)**    | Σ(Fix − Detect) / Post-deploy fixed — same but restricted to post-deployment CVEs       |
| **% Fixed in SLA** | (Fixed ≤ SLA / Total) × 100 — what fraction were resolved before the deadline           |
| **Mean Age**       | Σ(Now − Detect) / Open — average age of currently open CVEs at this severity            |
| **Oldest**         | Max open age — the single oldest unresolved CVE at this severity                        |
| **% > SLA**        | (Open > SLA / Total Open) × 100 — how many open CVEs have already breached the deadline |

### Volume & Flow

Shows the **Backlog Delta** — new CVEs detected minus CVEs fixed — broken out as a bar chart by severity (Critical, High, Medium, Low). Green bars are fixes; red bars are new CVEs. A rising red bar means risk is accumulating faster than it is being resolved. The aggregate delta number is shown top-right; positive means the backlog is growing.

### Vulnerability Trend (180 Days)

A stacked area chart showing open CVE counts per day for each severity band over the rolling window. Use this to spot when a new batch of vulnerabilities was introduced (a sudden step up) or when a remediation effort took effect (a step down). The x-axis is date; the y-axis is count of open CVEs. Each band is colour-coded: Critical (red), High (orange), Medium (yellow), Low (blue).

### Security Velocity & Impact Metrics

Four metric cards focused on operational effectiveness:

| Metric                 | What it means                                               | Calculation                             |
|------------------------|-------------------------------------------------------------|-----------------------------------------|
| **Fix Velocity**       | CVEs remediated per week over the last 180 days             | Fixed CVEs / 26 weeks                   |
| **High-Risk Backlog**  | Total open Critical + High CVEs right now                   | Count of open Critical + open High      |
| **Shift-Left Success** | Percentage of CVEs caught before they reached production    | Pre-Deploy / (Pre-Deploy + Total) × 100 |
| **SLA Burn Rate**      | How many SLA deadlines are being breached per 30-day period | Open CVEs where (SLA − Age) < 30d       |

Three summary numbers appear at the bottom of this section:

| Number                | What it means                                                                                                                                 |
|-----------------------|-----------------------------------------------------------------------------------------------------------------------------------------------|
| **CVE-Days Exposure** | Total accumulated exposure across all open CVEs — Σ(Mean Age) across all open CVEs. A high number means many CVEs have been open a long time. |
| **Weeks to Clear**    | Estimated weeks to resolve the current backlog at the current fix velocity — Open / Weekly Velocity                                           |
| **Risk Score**        | Weighted sum of open CVEs by severity — Weighted Open (C×8 H×3 M×1 L×1)                                                                       |

---

## Invite Your Team

As an org owner or admin, go to your organization settings to invite colleagues.

Each invited user receives an email with a link to set their password. Links expire after **48 hours**. If a link expires, an admin can resend it.

When choosing a role, use the minimum necessary:

| Role       | Can do                                                                                    |
|------------|-------------------------------------------------------------------------------------------|
| **Owner**  | Everything, including managing billing and deleting the org                               |
| **Admin**  | Everything except billing — invite users, manage roles, access all resources              |
| **Editor** | Upload releases, upload SBOMs, sync endpoints — the right role for CI/CD service accounts |
| **Viewer** | Read-only — dashboards, CVE details, endpoint status                                      |

---

## SLA Reference

SLA targets define how many days your team has to remediate a CVE before it counts as overdue. The clock starts when the CVE is first detected on a deployed endpoint.

| Severity | Standard Endpoint | Mission-Critical Endpoint |
|----------|-------------------|---------------------------|
| Critical | 15 days           | 7 days                    |
| High     | 30 days           | 15 days                   |
| Medium   | 90 days           | 90 days                   |
| Low      | 180 days          | 180 days                  |

Endpoints with `endpoint_type: mission_asset` use the tighter targets in the right column.

---

## Glossary

| Term                  | Definition                                                                                           |
|-----------------------|------------------------------------------------------------------------------------------------------|
| **CVE**               | Common Vulnerabilities and Exposures — a unique identifier for a known vulnerability                 |
| **CVSS**              | Common Vulnerability Scoring System — a 0–10 severity score (9.0+ = Critical)                        |
| **SBOM**              | Software Bill of Materials — a machine-readable inventory of every library your software depends on  |
| **PURL**              | Package URL — a standardized identifier for a software package, e.g. `pkg:npm/lodash@4.17.20`        |
| **MTTR**              | Mean Time To Remediate — average days from CVE detection to fix deployment                           |
| **SLA**               | Service Level Agreement — the target number of days within which a CVE should be remediated          |
| **Endpoint**          | A running environment where software is deployed (cluster, function, device)                         |
| **Sync**              | The act of telling Ortelius what versions are currently deployed to an endpoint                          |
| **OSV**               | Open Source Vulnerabilities — the vulnerability database Ortelius pulls from, refreshed every 15 minutes |
| **OCI Attestation**   | An SBOM or other artifact attached directly to a container image in the registry. This backend's own REST/GitHub-onboarding endpoints don't discover these, but the optional [`relscanner-job`](https://github.com/ortelius/relscanner-job) and [`deployment-gke`](https://github.com/ortelius/deployment-gke) scanner components do |
| **OpenSSF Scorecard** | An automated security health score (0–10) for open source repositories. Not fetched by this backend's own endpoints, but `relscanner-job` fetches it automatically when deployed; otherwise populate `scorecard_result` on a release yourself via the API |

---

## Next Steps

- **Running on-premises or self-hosted?** → [Architecture Guide](docs/architecture.md)
- **Integrating the API, writing queries, or contributing code?** → [Implementation Guide](docs/implementation.md)