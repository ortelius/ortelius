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

#!/bin/sh
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
    sudo -u postgres /usr/pgsql-10/bin/pg_ctl initdb --pgdata=/tmp/data
    sudo cp -rp /tmp/data /var/lib/pgsql
    sudo chown -R postgres:postgres /var/lib/pgsql
  fi

  sudo chmod 700 /var/lib/pgsql/data

  sudo grep -qxF 'host all all 0.0.0.0/0 trust' /var/lib/pgsql/data/pg_hba.conf || sudo sed -i '$ a\'"host all all 0.0.0.0/0 trust" /var/lib/pgsql/data/pg_hba.conf
  sudo grep -qxF "listen_addresses = '*'" /var/lib/pgsql/data/postgresql.conf || sudo sed -i '$ a\'"listen_addresses = '*'" /var/lib/pgsql/data/postgresql.conf

  sudo -u postgres /usr/pgsql-10/bin/pg_ctl start --pgdata=/var/lib/pgsql/data
  sleep 10
  sudo -u postgres /usr/pgsql-10/bin/pg_ctl status --pgdata=/var/lib/pgsql/data
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