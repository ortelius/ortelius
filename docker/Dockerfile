FROM quay.io/openmake/meister:latest

WORKDIR /build/

COPY . /workspace
RUN /workspace/tgt/build.sh

FROM quay.io/ortelius/ortelius-base:latest

WORKDIR /opt/deployhub/
COPY --from=0 /workspace/installers/linux/webadmin/webapp-runner.jar webadmin/webapp-runner.jar
COPY --from=0 /workspace/installers/linux/engine/scripts engine/scripts
COPY --from=0 /workspace/installers/linux/engine/lib engine/lib
COPY --from=0 /workspace/installers/linux/engine/demorep engine/demorep
COPY --from=0 /workspace/installers/linux/engine/demo engine/demo
COPY --from=0 /workspace/installers/linux/engine/dm.* engine/
COPY --from=0 /workspace/installers/linux/engine/odbc.ini /etc/odbc.ini
COPY --from=0 /workspace/installers/linux/engine/trilogy.* engine/
COPY --from=0 /build/deployhub-webadmin.war webadmin/deployhub-webadmin.war
COPY --from=0 /build/DEBUG/bin engine/bin
COPY --from=0 /build/DEBUG/setodbc engine/
COPY --from=0 /workspace/docker/entrypoint.sh .

USER omreleng
ENV PATH="${PATH}:/usr/pgsql-10/bin:/usr/local/google-cloud-sdk/bin"
ENTRYPOINT /opt/deployhub/entrypoint.sh
