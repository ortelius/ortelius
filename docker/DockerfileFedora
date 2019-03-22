FROM registry.fedoraproject.org/fedora:28
MAINTAINER DeployHub

EXPOSE 8080

RUN useradd -ms /bin/bash omreleng; 

RUN rpm -Uvh https://download.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm; \
    yum -y update; \
    yum -y install git sudo unzip which iputils compat-openssl10 openssh-clients libnsl.i686 python-winrm python3-PyYAML python3-winrm python-requests-kerberos krb5-devel krb5-libs krb5-workstation ansible; \
    pip install pywinrm[credssp]; \
    pip3 install pywinrm[credssp] qtoml; 

RUN curl "https://s3.amazonaws.com/aws-cli/awscli-bundle.zip" -o "awscli-bundle.zip"; \
    unzip awscli-bundle.zip; \
    ./awscli-bundle/install -i /usr/local/aws -b /usr/local/bin/aws; 

RUN curl -sL -o /tmp/gcloud_install.sh https://sdk.cloud.google.com; \
    chmod 777 /tmp/gcloud_install.sh; \
    /tmp/gcloud_install.sh --disable-prompts --install-dir=/usr/local; \
    /usr/local/google-cloud-sdk/bin/gcloud components install kubectl docker-credential-gcr; 

RUN curl -sL -o /tmp/helm_install.sh https://raw.githubusercontent.com/helm/helm/master/scripts/get; \
    chmod 777 /tmp/helm_install.sh; \
    /tmp/helm_install.sh; 

RUN BLD=$(date|md5);curl -sL -o /tmp/deployhub-engine${BLD}.rpm https://github.com/ortelius/ortelius/blob/master/rpms/deployhub-engine.rpm?raw=true; \
    curl -sL -o /tmp/deployhub-webadmin${BLD}.rpm https://github.com/ortelius/ortelius/blob/master/rpms/deployhub-webadmin.rpm?raw=true; \
    yum -y install /tmp/deployhub-engine${BLD}.rpm /tmp/deployhub-webadmin${BLD}.rpm; \
    yum -y clean all; \
    rm -rf /var/cache/yum; \
    mkdir -p /home/omreleng/.ssh; \
    mkdir -p /root/.ssh; \
    chown -R omreleng:omreleng /home/omreleng; \
    echo 'omreleng       ALL=(ALL)       NOPASSWD: ALL' >> /etc/sudoers

USER omreleng
COPY entrypoint.sh /tmp
ENTRYPOINT /tmp/entrypoint.sh

LABEL name="ortelius/ortelius" \
      vendor="Ortelius" \
      version="8.0" \
      release="1" \
      summary="Ortelius" \
      description="Ortelius will perform agentless application releases" \
      url="https://ortelius.io/" \
      run='docker run -v $PWD/data:/var/lib/pgsql/data:Z -p 7171:8080 -d --hostname docker_dh' \
      stop='docker stop'