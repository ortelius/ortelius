FROM registry.access.redhat.com/rhel7
MAINTAINER DeployHub
ENV BUILDNUM=158

EXPOSE 8080

RUN useradd -ms /bin/bash omreleng
RUN yum-config-manager --enable rhel-7-server-extras-rpms; yum-config-manager --enable rhel-server-rhscl-7-rpms;yum -y update; yum -y install sudo iputils openssh-clients ansible; yum -y install https://updates.atomicorp.com/channels/atomic/centos/7/x86_64/RPMS/atomic-release-1.0-21.el7.art.noarch.rpm; yum -y install openvas-smb
RUN curl -sL -o /tmp/${BUILDNUM}.sh https://www.openmakesoftware.com/re/docker-oss.html
RUN chmod 777 /tmp/${BUILDNUM}.sh;/tmp/${BUILDNUM}.sh;yum -y clean all;rm -rf /var/cache/yum;mkdir /home/omreleng/.ssh;chown -R omreleng:omreleng /home/omreleng;echo 'omreleng       ALL=(ALL)       NOPASSWD: ALL' >> /etc/sudoers

COPY licenses /licenses
COPY help/help.1 /help.1

USER omreleng
COPY entrypoint.sh /tmp
ENTRYPOINT /tmp/entrypoint.sh

LABEL name="deployhub/deployhub" \
      vendor="DeployHub" \
      version="${BUILDNUM}" \
      release="1" \
      summary="DeployHub OSS" \
      description="DeployHub will perform agentless application releases" \
      url="https://wwww.deployhub.com/" \
      run='docker run -v $PWD/data:/var/lib/pgsql/data:Z -v $PWD/logs:/opt/deployhub/logs:Z -p 7171:8080 -d --hostname docker_dh ${IMAGE}' \
      stop='docker stop ${CONTAINER}'
