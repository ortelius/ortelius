# Ortelius

Welcome to Ortelius. Ortelius is an open source project that aims to simplify the implementation of microservices. By providing a central catalog of services with their deployment specs, application teams can easily consume and deploy services across cluster. Ortelius tracks application versions based on service updates and maps their service dependencies eliminating confusion and guess work.  Unique to Ortelius is the ability to track your microservice inventory across clusters mapping the differences.  Ortelius serves Site Reliability Engineers and Cloud Architects in their migration to and ongoing management of a microservice implementation. For more information, see the comprehensive documentation at http://docs.ortelius.io 

## Ortelius Mission

Our mission is to simplify the adoption of modern architecture through a world-class microservice management platform driven by a supportive and diverse global open source community.

## Ortelius Value Statements

- We want Members to learn and understand solutions around microservice use and their challenges through a diverse contributor base. 
- We want our members to develop their careers, skills and gain community recognition for their work and expertise. 
- We want our members to have the opportunity to share and reuse open source microservices to further the adoption of a cloud native development. 
- We want members to learn how to contribute to an open source community and become part of the broader conversation around cloud native architecture.

## Ortelius Benefits

The benefits of the Ortelius Open Source Project are:

1) Adds automated configuration management of microservices, tracking logical applications and their versions, into the continuous delivery process.
2) Simplifies the path to adopting cloud native development practices without a major redesign of their CD pipeline.
3) Give developers the power to control how their software is released across all environments by defining deployment configurations data that is separate from the definition.
4) Deliver transparency into the continuous delivery process by mapping component and microservice relationships with BOM and Difference Reports across deployed environments (clusters, cloud, physical).  
5) Serve as a central market place to share microservices with their deployment requirements and versions.

## Ortelius Development Goals for 2021

Goals for Q1 and Q2 of 2021

1) Website redesign to support easy contribution (pull request) (Jenkins as an example)
2) Improve data graphs and maps
3) Increase adoption by adding 'scraping' of AWS, GKE, Azure.

Goals for the Q3 and Q4 months of 2021

1) Publish our first global marketplace 
2) Convert DMScript to Python


## Code of Conduct

[Contributor Covenant Code of Conduct](./CODE_OF_CONDUCT.md)

## Become a contributor

1) Review the [Ortelius Contributor Guide](https://docs.ortelius.io/guides/contributorguide/)
2) Add yourself to the [Ortelius Google Group](https://groups.google.com/g/ortelius-dev)
3) Join the [Discord community channel](https://discord.gg/ZtXU74x)

## Open Source Sub-Committees

[Calendar of meetings with times and zoom info.](https://ortelius.io/events/)

[Discord community channel](https://discord.gg/ZtXU74x)

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
  
### Data Science and Visualization

Determine what reports and maps can currently be created and/or enhanced.  Begin looking at what data can be passed back to the CD pipelines for predictive reporting, risk assessment. (Think truth tables).

Contributors:

- Tracy Ragan
- Tatiana Lazebnyk

### Deployment Integrations

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

- Tracy Ragan
- Parijat Kalita
- Ashutosh Srivastava
- Poovaraj Thangamariappan

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

### Product Management

- Website, branding, outreach 
- Review messaging, update logo, work on blogs. 
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

Contributors:
- Brad McCoy
- Arvind Singharpuria
- Amit Dsouza
- Ayesha Khaliq

## Installation

Browse through the [Installation and Support Guide](http://docs.ortelius.io/guides/userguide/installation-and-support/) for detailed guidance on how to sign up for & set up Ortelius.

## Support

https://github.com/ortelius/ortelius/issues

