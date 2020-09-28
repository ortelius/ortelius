FROM fedora:32
MAINTAINER ortelius.io
ARG GIT_TAG
ARG COMMIT_SHA
ARG REPO_NAME
ARG BLDDATE
EXPOSE 8080
RUN useradd -ms /bin/bash omreleng; 

RUN yum -y update; \
    yum -y install procps-ng dos2unix jq git sudo python-pip make redhat-rpm-config gcc python3-devel libffi-devel python-wheel openssl-devel unzip glibc* python-setuptools which iputils compat-openssl10 openssh-clients libnsl samba-client python-winrm python3-PyYAML python3-winrm python-requests-kerberos krb5-devel krb5-libs krb5-workstation ansible; \
    pip install requests-credssp pywinrm[credssp]; \
    pip3 install requests-credssp pywinrm[credssp] deployhub; \
    curl -sL https://raw.githubusercontent.com/DeployHubProject/win_zip/master/ansible/win_zip.py -o /usr/lib/python3.8/site-packages/ansible/modules/windows/win_zip.py; \
    curl -sL https://raw.githubusercontent.com/DeployHubProject/win_zip/master/ansible/win_zip.ps1 -o /usr/lib/python3.8/site-packages/ansible/modules/windows/win_zip.ps1; 

RUN pip3 install awscli --upgrade --user;
RUN curl -sL -o /tmp/install.sh curl -O https://dl.google.com/dl/cloudsdk/channels/rapid/downloads/google-cloud-sdk-311.0.0-linux-x86_64.tar.gz; \
    tar xzf google-cloud-sdk-311.0.0-linux-x86_64.tar.gz -C tmp/; \
    chmod 777 /tmp/google-cloud-sdk/install.sh; \
    /tmp/google-cloud-sdk/install.sh --quiet --path-update=/usr/local;

RUN curl -sL -o /tmp/helm_install.sh https://raw.githubusercontent.com/helm/helm/master/scripts/get; \
    chmod 777 /tmp/helm_install.sh; \
    /tmp/helm_install.sh; \
    mv /usr/local/bin/helm /usr/local/bin/helm2

RUN curl -fsSL -o /tmp/get_helm.sh https://raw.githubusercontent.com/helm/helm/master/scripts/get-helm-3; \
    chmod 777 /tmp/get_helm.sh; \
    /tmp/get_helm.sh

RUN curl -skL -X GET https://releases.hashicorp.com/terraform/0.12.17/terraform_0.12.17_linux_amd64.zip -o /tmp/terraform_0.12.17_linux_amd64.zip; \
    cd /tmp; \
    unzip -qq terraform_0.12.17_linux_amd64.zip; \
    chmod +x terraform; \
    mv terraform /usr/local/bin/

RUN pip install pep517; \
    pip install py-make; \
    pip install PyNaCl; \
    pip install cryptography --no-binary :all:; \
    pip install azure-cli --no-binary :all:;

WORKDIR /rpms
COPY . /deployhub-engine.rpm /tmp/
COPY . /deployhub-webadmin.rpm /tmp/
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
