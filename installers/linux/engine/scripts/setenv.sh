#!/bin/sh
cd /opt/deployhub/engine
export DMHOME=$PWD
export LD_LIBRARY_PATH=$PWD/bin:$PWD/lib
export trinounlink=Y
export DM_TARGET_USER=root
