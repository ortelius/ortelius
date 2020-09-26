#!/bin/sh

# Sets up a docker credential helper so docker login credentials are not stored encoded in base64 plain text.
# Uses the pass secret service as the credentials store.
# If previously logged in w/o cred helper, docker logout <registry> under each user or remove ~/.docker/config.json.

tee -a /etc/yum.repos.d/google-cloud-sdk.repo << EOM
[google-cloud-sdk]
name=Google Cloud SDK
baseurl=https://packages.cloud.google.com/yum/repos/cloud-sdk-el7-x86_64
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://packages.cloud.google.com/yum/doc/yum-key.gpg
       https://packages
EOM

echo "GCP repo added"