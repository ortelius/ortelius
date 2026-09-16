# Ortelius Project Roadmap

Ortelius is focused on making post-deployment open source security easier to understand and act upon. The project maintains a digital representation of software releases, SBOMs, dependencies, repositories, and deployment endpoints so users can determine which vulnerabilities affect software they are actually running. The 2026 roadmap emphasizes simpler onboarding, stronger endpoint visibility, improved SBOM intelligence, actionable vulnerability prioritization, regulatory reporting, and broader project adoption.

## Near-Term Priorities — 2026

** Simplify Repository and Project Onboarding **

The first priority is making Ortelius extremely easy to adopt. Repository onboarding has already been substantially simplified, with GitHub repositories represented as projects beneath an organization and releases automatically enriched with package, SBOM, build, vulnerability, and OpenSSF Scorecard information. The next steps are to refine public and private repository onboarding, improve tutorials, and use real open source projects to test the workflow.

Key deliverables include:

- Complete streamlined public and private repository onboarding.
- Continue improving GitHub and GitLab connectors.
- Publish onboarding tutorials and examples.
- Gather usability feedback from community members and adopters.

** Make Endpoint Discovery as Easy as Repository Onboarding **

After repository onboarding, the next major workflow priority is simplified endpoint synchronization. Ortelius needs to reliably connect software releases to their operational environments so teams can understand where vulnerable components are running.

Current work includes Kubernetes audit-log monitoring, Helm and GitOps deployment detection, and improved support for Kubernetes and AWS Lambda deployment evidence. Earlier work established a release scanner capable of watching Kubernetes audit logs, associating SBOMs with deployments, and tracking endpoint information.

** Improve Vulnerability Prioritization **

Ortelius will continue moving beyond simple CVE severity reporting toward vulnerability intelligence that helps users determine what needs immediate attention.

Priorities include:

- Add a distinct Exploited vulnerability classification.
- Improve CVE deduplication across components within the same application.
- Investigate reachability as an additional prioritization signal.
- Continue improving organization-wide vulnerability trends and dashboards.

The working group specifically identified exploited vulnerabilities as separate from Critical, High, Medium, and Low severity because active exploitation materially changes remediation urgency. CVE deduplication and reachability have also been identified as areas for further development. In addition a new category "Exploited" should be added. 

** SBOM Intelligence **
Integrate SBOM Quality Analysis working with IBM and their existing tooling - John Bergland and Zadia Allen are leads. 

Ortelius will expand from SBOM ingestion to evaluating the quality and usefulness of the SBOM itself. Work with IBM's sbom-utilities project is intended to provide insight into component composition, direct versus transitive dependencies, components requiring attention, and whether an SBOM appears accurate and complete.

Planned work includes:

- Integrate IBM SBOM analysis data into Ortelius.
- Associate SBOM quality evaluations with individual software releases.
- Present SBOM health alongside OpenSSF Scorecard information.
- Maintain historical SBOM evaluations so changes can be compared over time.

The architecture group has specifically discussed storing these evaluations with release information and exposing an SBOM health score through the Ortelius interface.

** Regulatory and Compliance Support **

Ortelius will increasingly use its underlying software evidence model to help projects respond to emerging software-security requirements.

The initial areas identified by the working group are:

- European Union: support reporting associated with actively exploited vulnerabilities under the Cyber Resilience Act.
- India: investigate SBOM and cryptographic BOM requirements.
- Italy: support ACN requirements, including expanded component-origin information.
- Broader international requirements: generate jurisdiction-specific reports from a common underlying software data model.

The project will identify which required fields already exist, allow projects to supply missing information, distinguish asserted data from independently validated information, and develop reporting aligned with individual regulatory frameworks.

Ortelius will also evaluate the Open Group's emerging Cyber Risk Data Model as a possible mechanism for standardized cyber-risk data exchange and regulatory mappings when that work becomes publicly available.

** Platform and Architecture **

The architecture modernization effort will continue simplifying the Ortelius deployment model and reducing operational complexity. Work during 2026 consolidated the application from roughly twelve microservices to approximately six pods/services, introduced a clearer front-end/back-end architecture, and moved deployment management toward Terraform, Flux, and GitOps.

Continuing priorities include:

- Complete and stabilize the cloud-native deployment architecture.
- Improve Flux/Terraform deployment automation.
- Simplify secrets management.
- Expand GitHub application support for repository automation.
- Integrate Renovate for dependency version-drift management.
- Continue improving RBAC and organization-based access controls.
- Open Source Project and Dependency Visibility

Ortelius will expand its model beyond internally developed repositories to include the open source projects organizations consume.

Open source projects typically distribute releases through source repositories, package registries, container registries, or chart repositories rather than traditional operational endpoints. Ortelius intends to capture those distribution points and identify which versions were obtained and which vulnerabilities affect those versions. Users can also favorite public projects and dependencies so Ortelius can continuously track vulnerabilities affecting software their organizations consume.

This capability will support a consolidated view of risk across both internally developed applications and external open source dependencies.

** Community and Adoption **

Adoption remains a primary project objective, and ease of onboarding will be treated as a core product requirement rather than simply a documentation issue. Community members will be encouraged to onboard repositories, test workflows, identify areas of confusion, and provide direct feedback to the architecture team.

The community roadmap includes:

- Expand tutorials and practical onboarding examples.
- Continue the contributor badging program.
- Use Pathfinder badges to recognize contributors onboarding open source projects.
- Launch and grow the Ortelius adopter-badging initiative.
- Use real adopter environments and sandbox deployments to validate functionality.
- Continue outreach specifically aimed at growing a diverse contributor base.
- Combine outreach and architecture discussions to keep community priorities closely connected to technical development.

The working group has also moved toward twice-monthly combined outreach and architecture meetings, with the first portion dedicated to outreach and the remainder to technical architecture.

** OpenSSF Transition **

Ortelius will continue its transition into the OpenSSF ecosystem. As of the September 2026 Architecture Working Group meeting, the relevant OpenSSF working group had approved the project and advancement to the OpenSSF technical oversight process was the next step.

The transition is intended to strengthen collaboration around vulnerability disclosure, SBOM consumption, open source security, and post-deployment vulnerability management while providing a broader neutral home for the Ortelius community.

** Longer-Term Direction **

The longer-term goal is for Ortelius to provide an open, accessible platform that connects software creation, distribution, deployment, and vulnerability intelligence. This means moving from simply identifying CVEs to answering the operational questions that follow disclosure:

Is this vulnerable component part of software we use? Where is that version running? Is the vulnerability exploited or reachable? Which applications and endpoints are affected? What information do we need to provide for remediation or regulatory reporting?

The roadmap therefore continues moving Ortelius toward a comprehensive, deployment-aware view of open source risk while keeping the platform lightweight and accessible to organizations and open source projects that may not be able to deploy large, agent-based production security platforms. That positioning was a recurring theme in the architecture discussions.
