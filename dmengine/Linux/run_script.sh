#!/bin/sh
BASEDIR=`dirname $0`
if [[ "$BASEDIR" = "." ]] ; then
  BASEDIR=$(pwd)
fi
DMHOME="$BASEDIR/debug"
if [[ ${DMHOME:0:1} = "." ]] ; then
  DMHOME=`pwd`/${DMHOME}
fi
echo "DMHOME=$DMHOME"

export JAVA_HOME=/usr/lib/jvm/java

POSTGRES_DIR=/opt/PostgreSQL/9.1
UNIXODBC_DIR=/usr/local
ORACLE_HOME=/usr/lib/oracle/xe/app/oracle/product/10.2.0/server
export TRILOGYHOME=/home/harvest/Trilogy2.3
export LD_LIBRARY_PATH=${DMHOME}:${TRILOGYHOME}/etc:${POSTGRES_DIR}/lib:${ORACLE_HOME}/lib:${UNIXODBC_DIR}/lib:${LD_LIBRARY_PATH}

runner="exec"

if [[ "$1" != "" ]] ; then
  case $1 in
  "-gdb") runner="gdb"; shift ;;
  "-ldd") runner="ldd"; shift ;;
  "-vg") runner="valgrind --gen-suppressions=yes --suppressions=/mnt/hgfs/Shared/DM2/DeploymentManager/Linux/vg/dm2.supp"; shift ;;	#  --suppressions vg/dm.supp
  "-vk") runner="valkyrie -f vg/DM2.cfg"; shift ;;
  "-testmode"|"-install"|"-forceunlink") ;;
  #*) echo "unknown option $1"; exit 1 ;;
  esac
fi

TEST=7

if [[ 0 -eq 1 ]] ; then
  export TRIREASON=POPULATE
  #export TRICLIENTUSERNAME=jamie
  export TRICLIENTUSERNAME=robert
  case $TEST in
  1) export TRICURRENTFIELD=1 ;;
  2) export TRIFIELD1=envC
     export TRICURRENTFIELD=2 ;;
  3) export TRIFIELD1=envC
     export TRICURRENTFIELD=3 ;;
  4) export TRICURRENTFIELD=4 ;;
  7) export TRICURRENTFIELD=7 ;;
  esac
elif [[ 0 -eq 1 ]] ; then
  export TRIREASON=VALIDATION
  #export TRICLIENTUSERNAME=jamie
  export TRICLIENTUSERNAME=robert
  export TRIFIELD1=envA
  export TRIFIELD2=harvest
  export TRIFIELD3=********
  export TRIFIELD4=app1
elif [[ 0 -eq 1 ]] ; then
  export TRIREASON=SCRIPT
  export TRICLIENTUSERNAME=demo
  #export TRIFIELD1="Demo Environment (SSH)"
  export TRIFIELD1="Demo Environment (FTP)"
  export TRIFIELD2=Robert
  export TRIFIELD3=password
  #export TRIFIELD4="DMDemo AppVer 2.0"
  export TRIFIELD4="DMDemo P4 Components"
  export TRILISTBOXSELECTIONS=1
  export TRI_PACKAGENAME_1=DMDemo-2.0
  export TRI_ASSIGNED_TO_1=demo
  export TRI_MODIFIED_ON_1="16:01 17/09/2012"
  export TRI_CREATED_ON_1="18:03 29/08/2012"
  #vars="cmdln_harstate=Test cmdln_harpackages='DMDemo-2.0'"
  #vars="cmdln_harstate=Test DMDemo-1.0 DMDemo-2.0"
  vars="cmdln_harproject=DMDemo cmdln_harstate=Test DMDemo-2.0"
else
  export TRIREASON=SCRIPT
  export TRICLIENTUSERNAME=robert
  export TRIFIELD1=envA
  export TRIFIELD2=Robert
  export TRIFIELD3=password
  export TRIFIELD4=app2
  #export TRIFIELD4=macys
  export TRILISTBOXSELECTIONS=2
  export TRI_PACKAGENAME_1=MyPackage1
  export TRI_CREATED_BY_1=fred
  export TRI_CREATED_ON_1="18:03 29/08/2012"
  export TRI_PACKAGENAME_2=MyPackage2
  export TRI_CREATED_BY_2=jim
  export TRI_CREATED_ON_2="18:04 29/08/2012"

  vars="cmdln_var=testvalue argv1 argv2"
fi

#-pw "harvest"
if [[ ${runner} = "gdb" ]] ; then
  echo "run -home "$BASEDIR/../distrib" -fields "environments,username,password,applications,combo1,combo2,combo3" $* cmdln_haruser=harvest cmdln_harpass=harvest ${vars}" > /tmp/gdb.cmd
  ${runner} ${DMHOME}/dm -x /tmp/gdb.cmd
  exit
fi

${runner} ${DMHOME}/dm -home "$BASEDIR/../distrib" -color \
  -fields "environments,username,password,applications,combo1,combo2,combo3" \
  $* cmdln_haruser=harvest cmdln_harpass=harvest ${vars}
