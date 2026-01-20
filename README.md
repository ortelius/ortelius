# Ortelius

Welcome to Ortelius. The goal of the Ortelius community is to expose weak links and security compliance in the software supply chain by continuously gathering and analyzing software supply chain intelligence introduced across the DevOps pipeline. Generating security insights like SBOMs is not enough to harden your software supply chain. Consumption and analysis of the data is needed to rapidly respond to supply chain threats. IT teams struggle to respond to threats when critical security intelligence is fragmented across tools and managed at the container level. Ortelius gathers and aggregates security and DevOps intelligence, tracking open-source inventory and vulnerabilities at the higher organizational levels, including logical applications, runtime environments, and organizational domains. With Ortelius, you can easily answer the question, “Where is Log4J running?” For more information, see the comprehensive documentation at [Ortelius' documentation](http://docs.ortelius.io).


![Release](https://img.shields.io/github/v/release/ortelius/ortelius?sort=semver)
![license](https://img.shields.io/github/license/ortelius/.github)

![Build](https://img.shields.io/github/actions/workflow/status/ortelius/ortelius/build-push-chart.yml)
![CodeQL](https://github.com/ortelius/ortelius/workflows/CodeQL/badge.svg)
[![OpenSSF
-Scorecard](https://api.securityscorecards.dev/projects/github.com/ortelius/ortelius/badge)](https://api.securityscorecards.dev/projects/github.com/ortelius/ortelius)


![Discord](https://img.shields.io/discord/722468819091849316)

## Ortelius Mission

Our mission is to build a federated DevOps and Supply Chain Security evidence store designed to provide real-time insights and feedback on the security aspects of software development, deployment, and operations within a DevSecOps environment.

## Ortelius Value Statements

- We want Members to learn and understand solutions around software security, DevOps, and cloud-native environments through a diverse contributor base.
- We want our members to develop their careers, skills and gain community recognition for their work and expertise.
- We want members to learn how to contribute to an open source community and become part of the broader conversation around cloud native architecture.

## Ortelius Benefits

The benefits of the Ortelius Open Source Project are:

1) A Digital Twin that federates supply chain and DevOps intelligence across organizational silos.
2) Maps component versions > Open-source packages > deployed endpoint, synchronizing with OSV.dev to identify new CVEs exposing live systems and the attack surface.
3) Decreases MTTR for newly reported critical and high-risk CVEs. 
4) Integrates open-source security tooling into the development workflows.
5) Performs software supply chain versioning and builds historical trends, the basis for threat modeling and remediation.

## Ortelius Development Goals for 2026

Q1 - release the new version of Ortelius
Q2 - focus on MCP 
Q3 - Auto remediation of critical / high risk vulnerabilties running on endpoints

List of v11 Repos:
* [scec-app-tag](https://github.com/ortelius/scec-app-tag.git)
* [scec-appver](https://github.com/ortelius/scec-appver.git)
* [scec-comp-tag](https://github.com/ortelius/scec-comp-tag.git)
* [scec-compver](https://github.com/ortelius/scec-compver.git)
* [scec-vulnerability](https://github.com/ortelius/scec-vulnerability.git)
* [scec-deployment](https://github.com/ortelius/scec-deployment.git)
* [scec-deppkg](https://github.com/ortelius/scec-deppkg.git)
* [scec-environment](https://github.com/ortelius/scec-environment.git)
* [scec-group](https://github.com/ortelius/scec-group.git)
* [scec-scorecard](https://github.com/ortelius/scec-scorecard.git)
* [scec-textfile](https://github.com/ortelius/scec-textfile.git)
* [scec-user](https://github.com/ortelius/scec-user.git)
* [scec-usergroup](https://github.com/ortelius/scec-usergroup.git)
* [scec-validate-provenance](https://github.com/ortelius/scec-validate-provenance.git)
* [scec-validate-signing](https://github.com/ortelius/scec-validate-signing.git)
* [scec-validate-user](https://github.com/ortelius/scec-validate-user.git)


## Code of Conduct

[Contributor Covenant Code of Conduct](./CODE_OF_CONDUCT.md)

## Become a contributor

1) Review the [Ortelius Contributor Guide](https://docs.ortelius.io/guides/contributorguide/)
2) Add yourself to the [Ortelius Google Group](https://groups.google.com/g/ortelius-dev)
3) Join the [Discord community channel](https://discord.gg/ZtXU74x)

## Open Source Sub-Committees

[Calendar of meetings with times and zoom info.](https://ortelius.io/events/)

## Ortelius Governing Board
- [Govering Board Guidelines and Elections](https://ortelius.io/guidelines/)
- [GB Google Group](https://groups.google.com/g/ortelius-governing-board)
- [Current Governing Board Members](https://ortelius.io/blog/2022/12/13/ortelius-2023-governing-board/)


### CD Environment - Development Infrastructure and Productivity

Create a CD process for managing pull requests, builds, tests and releases.

Contributors:

- Anand Bhagwat
- Steve Taylor
- Sanjay Sheel
- Sacha Wharton
- Sagar Utekar
- Nael Fridhi
- Sanchit Khurana
- Natch Khongpasuk
- Brad McCoy
- Zach Jones
- Aditi Agarwal
- Jesse Gonzalez
- Jimmy Malhan
- Arvind Singharpuria
- Interas LLC - Corporate Contributer
- Ujwal Yelmareddy
- Lakshmi Viswanath
- Hamid Gholami
- Kingsathurthi
- Bassem Riahi
- Arnab Maity
- Steven Carrato
- Ragha Vema
- Priya Kashyap
- Siddharth Pareek


### CD Integrations

Create integrations with documentation and videos for the following CI/CD Solutions:

- Jenkins: 90% work completed
- Jenkins X
- Screwdriver
- Tekton (Tekton Catalog)
- Spinnaker
- Argo

Contributors:

- Steve Taylor
- Sacha Wharton
- Sagar Utekar
- Nael Fridhi
- Sanchit Khurana
- Karamjot Singh
- Sergio Canales
- Zach Jones
- Aditi Agarwal
- Jesse Gonzalez
- Lakshmi Viswanath
- Kingsathurthi
- Bassem Riahi
- Arnab Maity


### UX and Testing

Review User Interface and make recommendations for improving with a focus on ease of use. Define test cases with automation.

Contributors:
- David Edelhart
- Tracy Ragan
- Parijat Kalita
- Ashutosh Srivastava
- Poovaraj Thangamariappan
- Yasaman Khazaie
- Nik Poputko
- Anirudh Sharma
- Ragha Vema
- Manoj Singhal

### Documentation

Review documentation and re-write or clarify complexities.

Contributors:

- Tracy Ragan
- Divya Mohan
- Mark Peters
- Arijeet Majumdar
- Pawel Kulecki
- Jayesh Srivastava

### Architecture

Digtial Twin development, MCP and the use of AI for Auto-remediation of dependency files. 

Contributors:

- Christopher Hicks
- Steve Taylor
- Ayesha Khaliq
- Drishti Dhamejani
- Rahul Agrawal
- Sacha Wharton
- Sagar Utekar
- Nael Fridhi
- Sanchit Khurana
- Karamjot Singh
- Zach Jones
- Jesse Gonzalez
- Neil Chen
- Devendran Nehru
- Arvind Singharpuria
- Turker Aslan
- Leniuska Alvarado
- Ankur Kumar
- Lakshmi Viswanaths
- Bassem Riahi
- Paul Li
- Joseph Akayesi
- Christian De Leon
- Ian Anderson
- Priya Kashyap

### Development

Work on existing enhancements and bug fixes. Add them to the core Ortelius repository unless a doc change.

Contributors:

- Steve Taylor
- Drishti Dhamejani
- Melissa Albarella
- Sagar Utekar
- Nael Fridhi
- Sanchit Khurana
- Zach Jones
- Jesse Gonzalez
- Temitope Bimbo Babatola
- Munirat Sulaimon
- Neil Chen
- Atul Tiwari
- Devendran Nehru
- Utkarsh Kumar Sharma
- Avikam Jha
- Jayesh Srivastava
- Arvind Singharpuria
- Aman Saxena
- Ashutosh Srivastava
- Leniuska Alvarado
- Bassem Riahi
- Paul Li
- Joseph Akayesi
- Christian De Leon
- Akshat Jain
- Ragha Vema
- Kumar A. Anurag

### Product Management

- Website, branding, outreach
- Review messaging, update logo, submit blogs.
- Personas, Journey Maps, service maps, roadmaps, Value Canvas, Go-to-Market strategies, product metrics.
- What problem or opportunity is being explored?
- How is the solution being framed to tackle this?
- What is being measured to determine if this is successful?
- Who are the people that this solution serves?
- How are they being informed about it?
- How are they learning to actually use or benefit from it?
- How are they involved in collaborating on the solution with us?
- What is the experience like for new collaborators getting started?
- How does the solution fit with both the immediate and wider ecosystem?
- Are there any roadblocks that can be removed in how we operate?
- What additional resources could be made available? Where would those resources help most?
- Where is the documentation being maintained on the project?
- Do we understand accessibility requirements? Are we meeting them?

Contributors:

- Tracy Ragan
- Neetu Jain
- Divya Mohan
- Mark Peters
- Alok Tanna
- Arijeet Majumdar
- Tatiana Lazebnyk
- Turker Aslan
- Priya Kashyap

### Project Management

Track progress, define process, work with Steve and Marky managing pull requests and releases dates.

Contributors:

- Tracy Ragan
- Neetu Jain
- Priya Kashyap

## GitOps

Research, define and Automate GitOps with Ortelius

Contributors,

- Brad McCoy
- Arvind Singharpuria
- Amit Dsouza
- Ayesha Khaliq
- Saif Ul Islam
- Kingsathurthi
- Hamid Gholami
- Anuja Kumari
- Vrukshali Torawane
- Joseph Akayesi
- Rakesh Arumalla
- Arnab Maity

## Installation

Browse through the [Installation and Support Guide](http://docs.ortelius.io/guides/userguide/installation-and-support/) for detailed guidance on how to sign up for & set up Ortelius.

## Support

- [Issues](https://github.com/ortelius/ortelius/issues)
