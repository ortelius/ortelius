#!/bin/sh
. /etc/profile

id

#DOCKERFILE=`echo $1 | sed "s/Dockerfile.*/Dockerfile/"`
#mv $1 $DOCKERFILE
#cat $DOCKERFILE
echo ========== $1 ==============
cat $1
echo
echo ========== $1 ==============
docker build -f $1 -t $2:$3 $PWD | grep -v "Sending build context to Docker daemon"
