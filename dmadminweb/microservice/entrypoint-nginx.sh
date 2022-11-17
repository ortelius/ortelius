#!/usr/bin/env sh

set -x

# use default nginx.conf unless SSL or NOSSL are set to 1

if [ "$SSLTYPE" = "ON" ]; then
  echo "Turning SSL On"
  cp /etc/nginx/nginx-ssl.conf /etc/nginx/nginx.conf
fi

if [ "$SSLTYPE" = "OFF" ]; then
  echo "Turn SSL Off"
  cp /etc/nginx/nginx-nossl.conf /etc/nginx/nginx.conf
fi

if [ "$POD_NAMESPACE" != "" ]; then
 sed -i.bak "s/default.svc.cluster.local/$POD_NAMESPACE.svc.cluster.local/" /etc/nginx/nginx.conf
fi

nginx -g 'daemon off;'
