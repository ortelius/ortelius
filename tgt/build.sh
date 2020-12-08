#!/bin/sh

export TOMCAT_LIB=/workspace/tomcat7/lib
cd /build
export JAVA_HOME=$(readlink -f /usr/bin/javac | sed "s:/bin/javac::")
export PATH=$PATH:$(readlink -f /usr/bin/javac | sed "s:/bin/javac::")/bin

om -l -f /workspace/tgt/java.mak CFG=DEBUG deployhub-webadmin.war | grep -v WARN
test $? -eq 0 || exit $?

om -l -f /workspace/tgt/linux.mak CFG=DEBUG | grep -v WARN
test $? -eq 0 || exit $?