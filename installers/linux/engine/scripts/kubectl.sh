#!/bin/sh

. /etc/profile

set -x
export HOME=/tmp
/usr/local/google-cloud-sdk/bin/kubectl "$@" 2>&1 | grep -v Error
