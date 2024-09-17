#!/bin/sh

. /etc/profile

export HOME=/tmp
export PATH=$PATH:/usr/local/google-cloud-sdk/bin
docker-credential-gcr configure-docker
docker "$@"
