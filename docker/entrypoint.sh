#!/bin/sh
set -x
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

sudo -u postgres /usr/pgsql-10/bin/pg_ctl start --pgdata=/var/lib/pgsql/data
sleep 10
sudo -u postgres /usr/pgsql-10/bin/pg_ctl status --pgdata=/var/lib/pgsql/data

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

echo Running DeployHub Processes

# /opt/deployhub/engine/trilogyd 1>/opt/deployhub/logs/engine.out 2>/opt/deployhub/logs/engine.err &
java -jar /opt/deployhub/webadmin/webapp-runner.jar --path /dmadminweb /opt/deployhub/webadmin/deployhub-webadmin.war  2>&1 | perl -pe '$_ = localtime.": $_"' > /opt/deployhub/logs/deployhub.log
