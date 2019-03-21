#!/bin/sh

. /etc/profile

set -x
export HOME=/tmp
/usr/local/google-cloud-sdk/bin/gcloud "$@"
