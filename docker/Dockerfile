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

FROM quay.io/openmake/meister:v7_5_1 AS builder

WORKDIR /build/

COPY . /workspace
RUN /workspace/tgt/build.sh

FROM quay.io/ortelius/ortelius-base:fedora-34-v1_0_0

WORKDIR /opt/deployhub/
COPY --from=builder /workspace/installers/linux/webadmin/webapp-runner.jar webadmin/webapp-runner.jar
COPY --from=builder /workspace/installers/linux/engine/scripts engine/scripts
COPY --from=builder /workspace/installers/linux/engine/lib engine/lib
COPY --from=builder /workspace/installers/linux/engine/demorep engine/demorep
COPY --from=builder /workspace/installers/linux/engine/demo engine/demo
COPY --from=builder /workspace/installers/linux/engine/dm.* engine/
COPY --from=builder /workspace/installers/linux/engine/odbc.ini /etc/odbc.ini
COPY --from=builder /workspace/installers/linux/engine/trilogy.* engine/
COPY --from=builder /build/deployhub-webadmin.war webadmin/deployhub-webadmin.war
COPY --from=builder /build/DEBUG/bin engine/bin
COPY --from=builder /build/DEBUG/setodbc engine/
COPY --from=builder /workspace/docker/entrypoint.sh .
RUN sudo pip install --upgrade deployhub

WORKDIR /etc/nginx/

RUN sudo yum -y install nginx; \
    sudo mkdir /etc/nginx/location; \
    sudo mkdir /var/www;
COPY dmadminweb/microservice/nginx-onprem.conf  /etc/nginx/nginx.conf
COPY dmadminweb/microservice/location/  /etc/nginx/location/

WORKDIR /opt/deployhub/
USER omreleng
ENV PATH="${PATH}:/usr/pgsql-13/bin:/usr/local/google-cloud-sdk/bin"
ENTRYPOINT /opt/deployhub/entrypoint.sh

