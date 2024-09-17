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

if [[ "$DBLOCAL" != "" ]]; then
  DBHost=`echo $DBConnectionString | cut -d "/" -f3 | cut -d ":" -f1`
  DBPort=`echo $DBConnectionString | cut -d "/" -f3 | cut -d ":" -f2`
  DBName=`echo $DBConnectionString | cut -d "/" -f4 | cut -f1 -d "?"`
fi

if [ ! -e /opt/deployhub/keys/id_rsa ]; then
  mkdir -p /opt/deployhub/keys
  openssl genpkey -out /opt/deployhub/keys/id_rsa -algorithm RSA -pkeyopt rsa_keygen_bits:2048
  openssl pkey -in /opt/deployhub/keys/id_rsa -pubout -out /opt/deployhub/keys/id_rsa.pub
fi

if [ "${SamlIdpMetadata}" != "" ]; then
  export SamlIdpMetadataPath=/tmp/idp_metadata.xml
  export SamlKeystorePath=/tmp/trust.keystore
  export SamlKeystorePassword=$(openssl rand -base64 12)
  export SamlPrivateKeyPassword=${SamlKeystorePassword}
  export SamlKeystoreAlias=saml
  echo "${SamlIdpMetadata}" > "${SamlIdpMetadataPath}"
  keytool -genkey -keyalg RSA -alias "${SamlKeystoreAlias}" -keypass "${SamlPrivateKeyPassword}" -keystore "${SamlKeystorePath}" -storepass "${SamlKeystorePassword}" -dname "cn=U
nknown, ou=Unknown, o=Unknown, c=Unknown" -validity 365
fi

echo Running DeployHub Processes
java -jar /opt/deployhub/webadmin/webapp-runner.jar --path /dmadminweb /opt/deployhub/webadmin/deployhub-webadmin.war  2>&1
