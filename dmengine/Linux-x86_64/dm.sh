#!/bin/sh
DMHOME=`dirname $0`
POSTGRES_DIR=/opt/PostgreSQL/9.1
UNIXODBC_DIR=/usr/local
ORACLE_HOME=/usr/lib/oracle/xe/app/oracle/product/10.2.0/server
export LD_LIBRARY_PATH=${POSTGRES_DIR}/lib:${ORACLE_HOME}/lib:${UNIXODBC_DIR}/lib:${LD_LIBRARY_PATH}

runner="exec"

if [[ "$1" != "" ]] ; then
  case $1 in
  "-gdb") runner="gdb"; shift ;;
  "-vg") runner="valkyrie -f vg/DM2.cfg"; shift ;;
  "-testmode") ;;
  *) echo "unknown option $1"; exit 1 ;;
  esac
fi

if [[ 0 -eq 1 ]] ; then
  export TRIREASON=POPULATE
  export TRICLIENTUSERNAME=jamie
  export TRICURRENTFIELD=1
else
  export TRIREASON=SCRIPT
  export TRICLIENTUSERNAME=robert
  export TRIFIELD1=envA
  export TRIFIELD2=Robert
  export TRIFIELD3=password
  export TRIFIELD4=app2

  vars="cmdln_var=testvalue"
fi

#-pw "harvest"
${runner} ${DMHOME}/parser $* ${vars}
