#!/bin/sh
DMHOME=`dirname $0`/debug
if [[ ${DMHOME:0:1} = "." ]] ; then
  DMHOME=`pwd`/${DMHOME}
fi
#echo "DMHOME=$DMHOME"

export JAVA_HOME=/usr/lib/jvm/java

POSTGRES_DIR=/opt/PostgreSQL/9.1
UNIXODBC_DIR=/usr/local
ORACLE_HOME=/usr/lib/oracle/xe/app/oracle/product/10.2.0/server
TRILOGYHOME=/home/harvest/Trilogy2.2.1
export LD_LIBRARY_PATH=${DMHOME}:${TRILOGYHOME}/etc:${POSTGRES_DIR}/lib:${ORACLE_HOME}/lib:${UNIXODBC_DIR}/lib:${LD_LIBRARY_PATH}

exec ${DMHOME}/dm "$@"
