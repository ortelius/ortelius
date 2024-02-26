# Ortelius

Welcome to Ortelius. The goal of the Ortelius community is to expose weak links and security compliance in the software supply chain by continuously gathering and analyzing software supply chain intelligence introduced across the DevOps pipeline. Generating security insights like SBOMs is not enough to harden your software supply chain. Consumption and analysis of the data is needed to rapidly respond to supply chain threats. IT teams struggle to respond to threats when critical security intelligence is fragmented across tools and managed at the container level. Ortelius gathers and aggregates security and DevOps intelligence, tracking open-source inventory and vulnerabilities at the higher organizational levels, including logical applications, runtime environments, and organizational domains. With Ortelius, you can easily answer the question, “Where is Log4J running?” For more information, see the comprehensive documentation at [Ortelius' documentation](http://docs.ortelius.io).


![Release](https://img.shields.io/github/v/release/ortelius/ortelius?sort=semver)
![license](https://img.shields.io/github/license/ortelius/ortelius)

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

1) Federates supply chain and DevOps intelligence across organizational silos.
2) Tracks component versions 'blast radius' and their vulnerabilities across all consuming applications for rapid response.
3) Tracks the logical applications and aggregates component SBOM and CVE data to the logical application level. 
4) Integrates open-source security tooling into the CI/CD pipeline.
5) Performs software supply chain versioning and builds historical trends, the basis for threat modeling and remediation.
6) Serve as a central market place to share reusable components with their SBOM and CVE information. 

## Ortelius Development Goals for 2023

Goals for Q1 and Q2 of 2024

Continue development of Ortelius v11.  See the [Ortelius v11 Project Boards](https://github.com/orgs/ortelius/projects/10) for a list of issues to work on.  Get paid for your work by completing an issue tagged with `Bounty`.

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

### Market Place and Domains

Enhance the current Domain structure to make it more like a Marketplace for sharing Microservices.  Think API marketplace.

Contributors:

- Christopher Hicks
- Steve Taylor
- Ayesha Khaliq
- Nael Fridhi
- Karamjot Singh

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

Work to begin breaking down the monolithic into services. Starting with logging as a good first step. Integration with Istio with Routing. Solving onboarding efforts (AWS scraping for existing microservice customers)

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
