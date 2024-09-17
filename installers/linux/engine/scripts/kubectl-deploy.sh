#!/bin/sh

. /etc/profile
export HOME=/home/omreleng

cd $1

for f in *-ms*.yaml
do
   cat $f
   /usr/local/google-cloud-sdk/bin/kubectl apply -f $f
done

if ls *.delete > /dev/null 2>&1; then
 for f in *.delete
 do
   DEPLOYMENT=`echo $f | sed "s/\.delete//"`
   /usr/local/google-cloud-sdk/bin/kubectl delete deployment $DEPLOYMENT
 done
fi
