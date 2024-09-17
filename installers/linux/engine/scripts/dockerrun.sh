#!/bin/sh

. /etc/profile

set -x
docker run $1 $3
