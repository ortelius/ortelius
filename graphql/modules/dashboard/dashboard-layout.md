# Mission-Critical Endpoint Vulnerability Dashboard

**Scope:**  

- **Asset type:** Endpoint (mission-critical production systems)  
- **Vulnerability type:** CVEs impacting deployed endpoints  
- **SLA:** Severity-based, as defined below  
- **Rolling MTTR Window:** 180 days

---

## 1. SLA Reference Table (for calculations)

| Severity | Default SLA (days) | High-Risk Endpoint SLA (days) |
|----------|--------------------|-------------------------------|
| Critical | 15                 | 7                             |
| High     | 30                 | 15                            |
| Medium   | 90                 | 90                            |
| Low      | 180                | 180                           |

- **Clock starts at detection** (first confirmed impact on deployed endpoint)  
- Applies to **post-deployment CVEs** and pre-deployment CVEs that reach production.

---

## 2. Dashboard Metrics & Calculations

### A. Remediation Effectiveness (Closed CVEs)

**1. MTTR – All Endpoint CVEs (Rolling 180 Days)**  
\[
\text{MTTR}_{180d} = \frac{\sum_{i=1}^{N} (T_{\text{fix},i} - T_{\text{detect},i})}{N_{\text{fixed in last 180d}}}
\]  
**Rules:**  

- Include only CVEs **fixed within the rolling 180-day window**  
- Detection may predate the window  
- Exclude open CVEs  

**Mapping:**  

- NIST SP 800-53 SI-2, RA-5, CM-8  
- DoD Continuous ATO metric: “Mean remediation time for endpoint vulnerabilities”  
- **NIST SP 800-218:** Practice PP-3 (Establish and maintain software vulnerability remediation)  
- **NIST SP 800-190:** Section 3.2, Container security patching and vulnerability management

---

**2. MTTR by Severity**  
\[
\text{MTTR}^{severity}_{180d} = \frac{\sum_{i=1}^{N_{severity}} (T_{\text{fix},i} - T_{\text{detect},i})}{N_{severity, \text{fixed}}}
\]  

- Segment by Critical, High, Medium, Low  
- Maps to FedRAMP High/Moderate requirement for severity-based remediation  
- **NIST SP 800-218:** PP-3, PP-6 (Prioritize vulnerabilities by severity)  
- **NIST SP 800-190:** Section 3.2, vulnerability prioritization for container images and runtime

---

**3. MTTR – Post-Deployment Only**  
\[
\text{MTTR}_{post} = \frac{\sum_{i=1}^{M} (T_{\text{fix},i} - T_{\text{detect},i})}{M_{\text{post-deployment, fixed}}}
\]  

- Focuses on **new vulnerabilities affecting deployed endpoints**  
- Supports Continuous ATO reporting for **real-time exposure response**  
- **NIST SP 800-218:** PP-4 (Monitor deployed software for vulnerabilities)  
- **NIST SP 800-190:** Section 3.3, Runtime container monitoring and patching

---

### B. Active Risk Exposure (Open CVEs)

**1. Mean Open Vulnerability Age (All Open Endpoint CVEs)**  
\[
\text{Mean Open Age} = \frac{\sum_{i=1}^{O} (Now - T_{\text{detect},i})}{O}
\]  

- Measures ongoing exposure for all open endpoint CVEs  
- NIST mapping: SI-2, RA-5  
- **NIST SP 800-218:** PP-5 (Measure and track vulnerability exposure over time)  
- **NIST SP 800-190:** Section 3.2, track container CVE exposure

---

**2. Mean Open Age by Severity**  
\[
\text{Mean Open Age}^{severity} = \frac{\sum_{i=1}^{O_{severity}} (Now - T_{\text{detect},i})}{O_{severity}}
\]  

- **NIST SP 800-218:** PP-6 (Prioritize vulnerabilities)  
- **NIST SP 800-190:** Section 3.2, prioritize container CVEs by severity

---

**3. Mean Open Age – Post-Deployment Endpoint CVEs**  
\[
\text{Mean Post-Deploy Exposure} = \frac{\sum_{i=1}^{P} (Now - T_{\text{detect},i})}{P}
\]  

- Focuses on **live, mission-critical exposure**  
- **NIST SP 800-218:** PP-4, PP-5  
- **NIST SP 800-190:** Section 3.3, Runtime vulnerability tracking

---

**4. Oldest Open CVE (Overall & Post-Deployment)**  
\[
\text{Oldest Open CVE} = \max_{i}(Now - T_{\text{detect},i})
\]  

- Display separately by severity and post-deployment  
- NIST mapping: Continuous Monitoring, RMF control SI-2  
- **NIST SP 800-218:** PP-5  
- **NIST SP 800-190:** Section 3.3, highlight oldest unresolved container vulnerabilities

---

### C. SLA Compliance Metrics

**1. % Open Endpoint CVEs Beyond SLA**  
\[
\% \text{Beyond SLA} = \frac{\#\text{open CVEs where } (Now - T_{\text{detect}}) > SLA}{\#\text{open CVEs}} \times 100
\]  

- Calculated using **severity-specific SLA**  
- Segment by **post-deployment vs all CVEs**  
- **NIST SP 800-218:** PP-3, PP-5  
- **NIST SP 800-190:** Section 3.2, SLA adherence for container CVEs

---

**2. % Fixed Within SLA**  
\[
\% \text{Fixed Within SLA} = \frac{\#\text{CVEs where } (T_{\text{fix}} - T_{\text{detect}}) \le SLA}{\#\text{CVEs fixed}} \times 100
\]  

- Segment by severity and endpoint type (high-risk vs standard)  
- DoD Continuous ATO / FedRAMP compliance metric  
- **NIST SP 800-218:** PP-3, PP-4, PP-6  
- **NIST SP 800-190:** Section 3.2, timely patching

---

### D. Volume & Flow Metrics

**1. New CVEs Detected (Endpoint, Period)**  
\[
\text{New CVEs} = \#\text{CVEs where } T_{\text{detect}} \in \text{reporting period}
\]  

- **NIST SP 800-218:** PP-4 (Continuous detection)  
- **NIST SP 800-190:** Section 3.3, new CVE detection in containers

**2. CVEs Remediated (Endpoint, Period)**  
\[
\text{Fixed CVEs} = \#\text{CVEs where } T_{\text{fix}} \in \text{reporting period}
\]  

- **NIST SP 800-218:** PP-3  
- **NIST SP 800-190:** Section 3.3, remediation metrics

**3. Backlog Delta**  
\[
\text{Backlog Change} = \text{New CVEs} - \text{Fixed CVEs}
\]  

- Positive = risk accumulation  
- Negative = remediation outpacing new discoveries  
- **NIST SP 800-218:** PP-5 (Measure vulnerability backlog)  
- **NIST SP 800-190:** Section 3.3

---

### E. Endpoint Impact Metrics

**1. Affected Endpoints (Open CVEs)**  
\[
\text{Affected Endpoints} = \# \text{unique endpoints impacted by open CVEs}
\]  

- **NIST SP 800-218:** PP-5  
- **NIST SP 800-190:** Section 3.2, impact assessment for container workloads

**2. Post-Deployment CVEs by Endpoint Type**  

- Standard, Privileged, Safety-Critical  
- Supports **blast radius assessment**  
- **NIST SP 800-218:** PP-6  
- **NIST SP 800-190:** Section 3.2

---

### F. Trend Metrics (Optional but Recommended)

- MTTR rolling 180-day trend (overall, post-deployment)  
- Mean open exposure trend  
- New vs fixed CVEs by period  
- Backlog trend  

**NIST SP 800-137 / DoD Continuous ATO:** Continuous Monitoring metrics  

- **NIST SP 800-218:** PP-5  
- **NIST SP 800-190:** Section 3.3

---

### G. Executive Summary Block

| Metric                                   | Value  |
|------------------------------------------|--------|
| Total new CVEs detected (endpoint)       | X      |
| Post-deployment CVEs                     | Y      |
| MTTR – All endpoints                     | Z days |
| MTTR – Post-deployment                   | A days |
| Mean open age – all                      | B days |
| Mean open age – post-deployment          | C days |
| % open CVEs beyond SLA                   | D%     |
| Oldest open post-deployment critical CVE | E days |
| Backlog delta                            | F      |

---

## 3. Notes & Best Practices

1. **Rolling 180-day MTTR** ensures current operational relevance  
2. **Post-deployment CVEs tracked separately** to highlight live risk  
3. **Severity-weighted SLA compliance** drives prioritization and reporting  
4. **Endpoint-focused metrics** match mission-critical production requirements  
5. **NIST/DoD/FedRAMP/SSDF/Container alignment:**  
   - SI-2: Flaw Remediation  
   - RA-5: Vulnerability Scanning  
   - CM-8/CM-3: Configuration and monitoring  
   - **NIST SP 800-218:** PP-3, PP-4, PP-5, PP-6  
   - **NIST SP 800-190:** Section 3.2-3.3, container vulnerability management  
   - Continuous ATO: MTTR, SLA compliance, backlog trends

---

**Policy Language Example (for documentation):**  
> _Vulnerability remediation SLAs are severity-based and endpoint-focused, measured from the time a vulnerability is confirmed to impact a mission-critical production endpoint._
