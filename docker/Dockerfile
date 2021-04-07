FROM quay.io/openmake/meister:v7_5_1 AS builder

WORKDIR /build/

COPY . /workspace
RUN /workspace/tgt/build.sh

FROM quay.io/ortelius/ortelius-base:fedora-32-v1_0_4

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

USER omreleng
ENV PATH="${PATH}:/usr/pgsql-10/bin:/usr/local/google-cloud-sdk/bin"
ENTRYPOINT /opt/deployhub/entrypoint.sh
