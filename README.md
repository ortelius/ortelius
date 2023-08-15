# Ortelius

Welcome to Ortelius. Ortelius is a unified 'evidence store' of supply chain data designed to simplify a decoupled microservices environment. Ortelius builds relationships between service (package) and consuming 'logical' applications. With Ortelius, open-source and API developers can register their services and track their consumers. Enterprise developers register their 'logical' application's base version and are notified when a dependency change causes the creation of a new application version, new SBOM, CVE, and dependency map. By centralizing and tracking detailed supply chain data, Ortelius provides the enterprise open-source consumers and open-source developers a coordinated view of who is using a service, its version, and inventory across all end-points. We are creating a central store for development teams to define provenance, apply zero-trust policies, and eventually score risk factors of both internal common code and open-source packages. In other words, we answer the question, "who is using Log4j." For more information, see the comprehensive documentation at [Ortelius' documentation](http://docs.ortelius.io).

![Release](https://img.shields.io/github/v/release/ortelius/ortelius?sort=semver)
![license](https://img.shields.io/github/license/ortelius/ortelius)

![Build](https://img.shields.io/github/actions/workflow/status/ortelius/ortelius/build-push-chart.yml)
[![MegaLinter](https://github.com/ortelius/ortelius/workflows/MegaLinter/badge.svg?branch=main)](https://github.com/ortelius/ortelius/actions?query=workflow%3AMegaLinter+branch%3Amain)
![CodeQL](https://github.com/ortelius/ortelius/workflows/CodeQL/badge.svg)
[![OpenSSF
-Scorecard](https://api.securityscorecards.dev/projects/github.com/ortelius/ortelius/badge)](https://api.securityscorecards.dev/projects/github.com/ortelius/ortelius)


![Discord](https://img.shields.io/discord/722468819091849316)

## Ortelius Mission

Our mission is to simplify the adoption of modern architecture through a world-class microservice governance catalog driven by a supportive and diverse global open source community.

## Ortelius Value Statements

- We want Members to learn and understand solutions around microservice use and their challenges through a diverse contributor base.
- We want our members to develop their careers, skills and gain community recognition for their work and expertise.
- We want our members to have the opportunity to share and reuse open source microservices to further the adoption of a cloud native development.
- We want members to learn how to contribute to an open source community and become part of the broader conversation around cloud native architecture.

## Ortelius Benefits

The benefits of the Ortelius Open Source Project are:

1) Adds automated supply chain management of microservices and other shared components across application teams
2) Tracks the 'blast radius' of a single shared component across all consuming applications that are using it. 
3) Tracks the logical applications and aggregates component SBOM and CVE data to the logical applicaiton level. 
4) Simplifies the path to adopting cloud native development practices without a major redesign of their CD pipeline.
5) Performs container versioning and applicaiton pakcage versions. 
6) Serve as a central market place to share reusable components with their SBOM and CVE information. 

## Ortelius Development Goals for 2023

Goals for Q1 and Q2 of 2023

1) Implementation of XRP Ledger to create immutable SBOMs.
2) Improve data graphs and maps.
3) Increase adoption of the catalog.

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

### Project Management

Track progress, define process, work with Steve and Marky managing pull requests and releases dates.

Contributors:

- Tracy Ragan
- Neetu Jain

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


