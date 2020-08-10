# Ortelius

Otelius on-prem runs as docker container.  In order to install it you need to have docker up and running.

## Docker Installation
### [Docker for Windows](https://docs.docker.com/docker-for-windows/install/)
Requires Microsoft Windows 10 Professional or Enterprise 64-bit

### [Docker for CentOS](https://docs.docker.com/install/linux/docker-ce/centos/)
Requires CentOS 64-bit 7.1 and higher on x86_64

### [Docker for RedHat](https://docs.docker.com/install/linux/docker-ee/rhel/)
Require RHEL 64-bit 7.1 and higher on x86_64, s390x, or ppc64le (not ppc64).

### [Docker for Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
Requires 64-bit version of one of these Ubuntu versions:
* Bionic 18.04 (LTS)
* Artful 17.10
* Xenial 16.04 (LTS)
* Trusty 14.04 (LTS)

### [Docker for OS/X](https://docs.docker.com/docker-for-mac/install/)
Requires macOS El Capitan 10.11 and newer macOS releases are supported. We recommend upgrading to the latest version of macOS.

### [Test your Docker Install](https://docs.docker.com/get-started/#test-docker-installation)

### Ortelius Image Pull
```
docker pull quay.io/ortelius/ortelius
```
### Ortelius Quick Start Guide
```
docker pull quay.io/ortelius/ortelius:latest
mkdir -p ~/ortelius/data
chmod 777 ~/ortelius/data
docker run -v ~/ortelius/data:/var/lib/pgsql/data:Z -p 7171:8080 -d --hostname docker_dh -v ~/.ssh:/keys:Z ${IMAGE_ID_OF_ortelius}
```
e.g.
```
docker run -v ~/ortelius/data:/var/lib/pgsql/data:Z -p 7171:8080 -d --hostname docker_dh -v ~/.ssh:/keys:Z IMAGE_ID
```
### Accessing Ortelius Homepage:
Homepage URL : http://localhost:7171/dmadminweb/Home

Login Credentials:

Admin Username: admin

Admin Password: admin

## Support

https://github.com/ortelius/ortelius/issues

