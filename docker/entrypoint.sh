#!/bin/sh

# Copyright (c) 2021 Linux Foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -x

DBLOCAL="$DBConnectionString$DBUserName$DBPassword"

if [[ "$DBLOCAL" == "" ]]; then
  sudo chown -R postgres:postgres /var/lib/pgsql
  sudo chmod -R 777 /opt/deployhub

  pkill postgres
  if sudo test -f "/var/lib/pgsql/data/pg_hba.conf"; then
    echo "Database already initialized"
  else
    mkdir /tmp/data
    sudo chown postgres:postgres /tmp/data
    sudo -u postgres $(find / -name pg_ctl 2>/dev/null) initdb --pgdata=/tmp/data
    sudo cp -rp /tmp/data /var/lib/pgsql
    sudo chown -R postgres:postgres /var/lib/pgsql
  fi

  sudo chmod 700 /var/lib/pgsql/data

  sudo grep -qxF 'host all all 0.0.0.0/0 trust' /var/lib/pgsql/data/pg_hba.conf || sudo sed -i '$ a\'"host all all 0.0.0.0/0 trust" /var/lib/pgsql/data/pg_hba.conf
  sudo grep -qxF "listen_addresses = '*'" /var/lib/pgsql/data/postgresql.conf || sudo sed -i '$ a\'"listen_addresses = '*'" /var/lib/pgsql/data/postgresql.conf

  sudo -u postgres $(find / -name pg_ctl 2>/dev/null) start --pgdata=/var/lib/pgsql/data
  sleep 10
  sudo -u postgres $(find / -name pg_ctl 2>/dev/null) status --pgdata=/var/lib/pgsql/data
else
  DBHost=`echo $DBConnectionString | cut -d "/" -f3 | cut -d ":" -f1`
  DBPort=`echo $DBConnectionString | cut -d "/" -f3 | cut -d ":" -f2`
  DBName=`echo $DBConnectionString | cut -d "/" -f4 | cut -f1 -d "?"`

  sudo sed -i.bak -e "s|Servername=localhost|Servername=$DBHost|" /etc/odbc.ini
  sudo sed -i.bak -e "s|Database=postgres|Database=$DBName|" /etc/odbc.ini
  sudo sed -i.bak -e "s|Port=5432|Port=$DBPort|" /etc/odbc.ini
  if [[ $DBConnectionString =~ "require" ]]; then
    sudo sed -i.bak -e "s|CommLog=0|CommLog=0\nSSLmode=require\n|" /etc/odbc.ini
  fi
fi

if [ ! -e /opt/deployhub/logs ]; then
 sudo mkdir /opt/deployhub/logs
fi

if [ ! -e /opt/deployhub/engine ]; then
 sudo mkdir /opt/deployhub/engine
fi

sudo chmod -R 777 /opt/deployhub
sudo chown -R omreleng /opt/deployhub

cd /opt/deployhub/engine || exit
export LD_LIBRARY_PATH=$PWD/lib:$PWD/bin
export PATH=$PWD/lib:$PWD/bin:$PATH
HOME=$(getent passwd "$(whoami)" | cut -d: -f6)
export HOME

sudo cp -r /keys/* /root/.ssh
sudo chown -R root /root/.ssh
sudo chmod 755 /root/.ssh

if [ -e /root/.ssh/known_hosts ]; then 
  sudo chmod 600 /root/.ssh/known_hosts
fi

cp -r /keys/* "$HOME/.ssh"
chown -R omreleng "$HOME/.ssh"
chmod 755 "$HOME/.ssh"
chmod 600 "$HOME/.ssh/known_hosts"

if [ ! -e /opt/deployhub/keys/id_rsa ]; then
  mkdir -p /opt/deployhub/keys
  openssl genpkey -out /opt/deployhub/keys/id_rsa -algorithm RSA -pkeyopt rsa_keygen_bits:2048
  openssl pkey -in /opt/deployhub/keys/id_rsa -pubout -out /opt/deployhub/keys/id_rsa.pub
fi

if [[ "$DBLOCAL" == "" ]]; then
  if [ "$NGINX" == "OFF" ]; then
    echo Running DeployHub Processes
    java -jar /opt/deployhub/webadmin/webapp-runner.jar --path /dmadminweb /opt/deployhub/webadmin/deployhub-webadmin.war  2>&1 | perl -pe '$_ = localtime.": $_"' > /opt/deployhub/logs/deployhub.log
  else
    sudo nginx
    java -jar /opt/deployhub/webadmin/webapp-runner.jar --port 9191 --path /dmadminweb /opt/deployhub/webadmin/deployhub-webadmin.war  2>&1 | perl -pe '$_ = localtime.": $_"' > /opt/deployhub/logs/deployhub.log
  fi
else
  if [ "$NGINX" == "OFF" ]; then
    echo Running DeployHub Processes
    java -jar /opt/deployhub/webadmin/webapp-runner.jar --path /dmadminweb /opt/deployhub/webadmin/deployhub-webadmin.war  2>&1 | perl -pe '$_ = localtime.": $_"' 
  else
    sudo nginx
    java -jar /opt/deployhub/webadmin/webapp-runner.jar --port 9191 --path /dmadminweb /opt/deployhub/webadmin/deployhub-webadmin.war  2>&1 | perl -pe '$_ = localtime.": $_"' 
  fi
fi
