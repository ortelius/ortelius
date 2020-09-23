FROM fedora:32
MAINTAINER ortelius.io
ARG GIT_TAG
ARG COMMIT_SHA
ARG REPO_NAME
EXPOSE 8080
RUN useradd -ms /bin/bash omreleng; 

RUN rpm --import https://dl.fedoraproject.org/pub/epel/RPM-GPG-KEY-EPEL-8; \ 
    yum -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm; \
    yum -y update; \
    yum -y install procps-ng dos2unix jq git sudo unzip which iputils compat-openssl10 openssh-clients libnsl.i686 samba-client python-winrm python3-PyYAML python3-winrm python-requests-kerberos krb5-devel krb5-libs krb5-workstation ansible; \
    pip install requests-credssp pywinrm[credssp]; \
    pip3 install requests-credssp pywinrm[credssp] deployhub;

RUN curl -sL https://raw.githubusercontent.com/DeployHubProject/win_zip/master/ansible/win_zip.py -o /usr/lib/python3/site-packages/ansible/modules/windows/win_zip.py; \
    curl -sL https://raw.githubusercontent.com/DeployHubProject/win_zip/master/ansible/win_zip.ps1 -o /usr/lib/python3/site-packages/ansible/modules/windows/win_zip.ps1; 

#RUN curl -sL https://raw.githubusercontent.com/DeployHubProject/win_zip/master/ansible/win_zip.py -o /usr/lib/python2.7/site-packages/ansible/modules/windows/win_zip.py; \
#    curl -sL https://raw.githubusercontent.com/DeployHubProject/win_zip/master/ansible/win_zip.ps1 -o /usr/lib/python2.7/site-packages/ansible/modules/windows/win_zip.ps1; 

RUN curl -O "https://awscli.amazonaws.com/awscli-exe-linux-aarch64.zip" -o "awscliv2.zip"; \
    yum -y install unzip; \
    unzip awscli-exe-linux-aarch64.zip; \
    ./aws/install -i /usr/local/aws -b /usr/local/bin;
RUN   aws --version; 

#RUN yum -y install which curl; \
#    curl -sL -o /tmp/gcloud_install.sh https://sdk.cloud.google.com; \
#    chmod 777 /tmp/gcloud_install.sh; \
#    /tmp/gcloud_install.sh --disable-prompts --install-dir=/usr/local; \
#    /usr/local/google-cloud-sdk/bin/gcloud components install kubectl docker-credential-gcr; 
RUN yum -y install which curl; \
    tee -a /etc/yum.repos.d/google-cloud-sdk.repo; \
    [google-cloud-sdk]; \
    name=Google Cloud SDK; \
    baseurl=https://packages.cloud.google.com/yum/repos/cloud-sdk-el7-x86_64; \
    enabled=1; \
    gpgcheck=1; \
    repo_gpgcheck=1; \
    gpgkey=https://packages.cloud.google.com/yum/doc/yum-key.gpg; \
    https://packages.cloud.google.com/yum/doc/rpm-package-key.gpg; \
    yum -y install kubectl docker-credential-gcr;

RUN yum -y install which curl; \ 
    curl -sL -o /tmp/helm_install.sh https://raw.githubusercontent.com/helm/helm/master/scripts/get; \
    chmod 777 /tmp/helm_install.sh; \
    /tmp/helm_install.sh; \
    mv /usr/local/bin/helm /usr/local/bin/helm2

RUN yum -y install which curl; \ 
    curl -fsSL -o /tmp/get_helm.sh https://raw.githubusercontent.com/helm/helm/master/scripts/get-helm-3; \
    chmod 777 /tmp/get_helm.sh; \
    /tmp/get_helm.sh

RUN yum -y install which curl; \ 
    curl -skL -X GET https://releases.hashicorp.com/terraform/0.12.17/terraform_0.12.17_linux_amd64.zip -o /tmp/terraform_0.12.17_linux_amd64.zip; \
    cd /tmp; \
    unzip -q terraform_0.12.17_linux_amd64.zip; \
    chmod +x terraform; \
    mv terraform /usr/local/bin/

RUN rpm --import https://packages.microsoft.com/keys/microsoft.asc;
RUN echo -e "\
[azure-cli] \n\
name=Azure CLI \n\
baseurl=https://packages.microsoft.com/yumrepos/azure-cli \n\
enabled=1 \n\
gpgcheck=1 \n\
gpgkey=https://packages.microsoft.com/keys/microsoft.asc \
" > /etc/yum.repos.d/azure-cli.repo
RUN yum -y install azure-cli

COPY rpms/deployhub-engine.rpm /tmp/
COPY rpms/deployhub-webadmin.rpm /tmp/

RUN yum -y install /tmp/deployhub-engine.rpm /tmp/deployhub-webadmin.rpm; \
    yum -y clean all; \
    rm -rf /var/cache/yum; \
    mkdir -p /winshares; \
    chmod -R 775 /winshares; \
    chmod -R 777  /var/lib/samba/private; \
    chmod -R 777  /var/lib/samba/lock; \   
    chown -R omreleng:omreleng /winshares; \
    mkdir -p /home/omreleng/.ssh; \
    mkdir -p /root/.ssh; \
    chown -R omreleng:omreleng /home/omreleng; \
    echo 'omreleng       ALL=(ALL)       NOPASSWD: ALL' >> /etc/sudoers; 

USER omreleng
COPY docker/entrypoint.sh /tmp/
ENV PATH "${PATH}:/usr/local/google-cloud-sdk/bin"
ENTRYPOINT /tmp/entrypoint.sh

LABEL name="ortelius" \
      vendor="Ortelius.io" \
      variant="$GIT_TAG" \
      version="$COMMIT_SHA" \
      gitrepo="$REPO_NAME" \
      blddate="$BLDDATE" \
      summary="Ortelius" \
      description="Microservice mapping and configuration management" \
      url="https://wwww.ortelius.io/" \
      run='docker run -v $PWD/data:/var/lib/pgsql/data:Z -v $PWD/logs:/opt/deployhub/logs:Z -p 7171:8080 -d --hostname docker_dh' \
      stop='docker stop'
