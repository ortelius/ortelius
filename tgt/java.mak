APPL=DEPLOY-PLUS
STAGE=ADMIN
MAKEFILEDIR=

VPATH=.;$(WORKSPACE)/dmadminweb/microservice;$(WORKSPACE);$(WORKSPACE)/tgt/java;$(WORKSPACE)/docs/doctohelp;$(TOMCAT_LIB);$(JAVA_HOME)/lib;$(JAVA_HOME)/jre/lib;$(JAVA_HOME)/jre/lib/ext;
PROJECTVPATH=


nginx.tar : nginx.conf \
		VERSIONINFO  \
		RELOPTIONS BTOG(Ant Tar|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Ant Tar|Build Task Options)[0]{} DT[37] \
	location/*.conf \
		VERSIONINFO  \
		RELOPTIONS BTOG(Ant Tar|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Ant Tar|Build Task Options)[0]{} DT[37]
	FINALTARGET nginx.tar
	TARGET nginx.tar
	TGTFILENAME nginx.tar.tgt
	PROJECTDIRS location;
	INTDIR
	BUILDTYPE Java Tar
	BUILDTASK Ant Tar
	SCRIPT Ant Tar Task.sc
	OSNAME Java
	ALLRELOPTIONS BTOG(Ant Tar|Build Task Options)[0]{}
	ALLDEBOPTIONS BTOG(Ant Tar|Build Task Options)[0]{}

deployhub-webadmin.javac : dmadminweb/src/dmadmin/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/pro/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/json/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/jsp/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/util/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/model/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/com/openmakesoftware/trilogy/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/com/openmakesoftware/XDR/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	deployhub-webadmin.classpath \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5]
	FINALTARGET deployhub-webadmin.war
	TARGET deployhub-webadmin.javac
	TGTFILENAME deployhub-webadmin.war.tgt
	PROJECTDIRS dmadminweb/src/dmadmin;dmadminweb/src/dmadmin/pro;dmadminweb/src/dmadmin/json;dmadminweb/src/dmadmin/jsp;dmadminweb/src/dmadmin/util;dmadminweb/src/dmadmin/model;dmadminweb/src/com/openmakesoftware/trilogy;dmadminweb/src/com/openmakesoftware/XDR;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant Javac
	SCRIPT Ant Javac Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}

ms-general.javac : dmadminweb/src/dmadmin/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/pro/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/json/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/jsp/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/util/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/model/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/com/openmakesoftware/trilogy/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/com/openmakesoftware/XDR/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	ms-general.classpath \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5]
	FINALTARGET ms-general.war
	TARGET ms-general.javac
	TGTFILENAME ms-general.war.tgt
	PROJECTDIRS dmadminweb/src/dmadmin;dmadminweb/src/dmadmin/pro;dmadminweb/src/dmadmin/json;dmadminweb/src/dmadmin/jsp;dmadminweb/src/dmadmin/util;dmadminweb/src/dmadmin/model;dmadminweb/src/com/openmakesoftware/trilogy;dmadminweb/src/com/openmakesoftware/XDR;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant Javac
	SCRIPT Ant Javac Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}

ms-init.javac : dmadminweb/src/dmadmin/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/pro/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/json/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/jsp/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/util/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/model/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/com/openmakesoftware/trilogy/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/com/openmakesoftware/XDR/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	ms-init.classpath \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5]
	FINALTARGET ms-init.war
	TARGET ms-init.javac
	TGTFILENAME ms-init.war.tgt
	PROJECTDIRS dmadminweb/src/dmadmin;dmadminweb/src/dmadmin/pro;dmadminweb/src/dmadmin/json;dmadminweb/src/dmadmin/jsp;dmadminweb/src/dmadmin/util;dmadminweb/src/dmadmin/model;dmadminweb/src/com/openmakesoftware/trilogy;dmadminweb/src/com/openmakesoftware/XDR;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant Javac
	SCRIPT Ant Javac Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}

ms-ui.javac : dmadminweb/src/dmadmin/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/pro/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/json/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/jsp/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/util/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/model/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/com/openmakesoftware/trilogy/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/com/openmakesoftware/XDR/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	ms-ui.classpath \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5]
	FINALTARGET ms-ui.war
	TARGET ms-ui.javac
	TGTFILENAME ms-ui.war.tgt
	PROJECTDIRS dmadminweb/src/dmadmin;dmadminweb/src/dmadmin/pro;dmadminweb/src/dmadmin/json;dmadminweb/src/dmadmin/jsp;dmadminweb/src/dmadmin/util;dmadminweb/src/dmadmin/model;dmadminweb/src/com/openmakesoftware/trilogy;dmadminweb/src/com/openmakesoftware/XDR;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant Javac
	SCRIPT Ant Javac Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}

dmadminweb.javac : dmadminweb/src/dmadmin/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/pro/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/json/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/jsp/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/dmadmin/util/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/dmadmin/model/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb/src/com/openmakesoftware/trilogy/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[37] \
	dmadminweb/src/com/openmakesoftware/XDR/*.java \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
	dmadminweb.classpath \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{} DT[5]
	FINALTARGET dmadminweb.war
	TARGET dmadminweb.javac
	TGTFILENAME dmadminweb.war.tgt
	PROJECTDIRS dmadminweb/src/dmadmin;dmadminweb/src/dmadmin/pro;dmadminweb/src/dmadmin/json;dmadminweb/src/dmadmin/jsp;dmadminweb/src/dmadmin/util;dmadminweb/src/dmadmin/model;dmadminweb/src/com/openmakesoftware/trilogy;dmadminweb/src/com/openmakesoftware/XDR;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant Javac
	SCRIPT Ant Javac Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant Javac|Build Task Options)[338]{ debug=true} TBTOG(Ant Javac|Build Task Options)[347]{ compiler=modern} TBTOG(Ant Javac|Build Task Options)[378]{ destdir="dmadminweb/src"}  BTOG(Ant Javac|Build Task Options)[0]{}

deployhub-webadmin.war : dmadminweb/WebContent/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/img/network/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/img/timeline/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/cupertino/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/*.svg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Rest/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/filetypes/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Home/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.xml \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/syntax/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_lib/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/edit_area/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/cultures/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/minified/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/minified/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/ao/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/plugins/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/userimages/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/jqwidgets/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/reports/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/META-INF/*.MF \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.jks \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/web.xml \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/schema/*.sql \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/schema/*.json \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.tld \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
	NetHelp/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
	deployhub-webadmin.javac \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5]
	FINALTARGET deployhub-webadmin.war
	TARGET deployhub-webadmin.war
	TGTFILENAME deployhub-webadmin.war.tgt
	PROJECTDIRS dmadminweb/WebContent;dmadminweb/WebContent/css/img/network;dmadminweb/WebContent/css/img/timeline;dmadminweb/WebContent/css;dmadminweb/WebContent/css/cupertino/images;dmadminweb/WebContent/css/cupertino;dmadminweb/WebContent/css/images;dmadminweb/WebContent/css/images/images;dmadminweb/WebContent/Rest/**;dmadminweb/WebContent/hot;dmadminweb/WebContent/images;dmadminweb/WebContent/images/filetypes;dmadminweb/WebContent/js/_demo;dmadminweb/WebContent/Home;dmadminweb/WebContent/js/_docs;dmadminweb/WebContent/js/_docs/syntax;dmadminweb/WebContent/js/_lib;dmadminweb/WebContent/js;dmadminweb/WebContent/js/edit_area/**;dmadminweb/WebContent/js/cultures;dmadminweb/WebContent/js/i18n;dmadminweb/WebContent/js/minified/i18n;dmadminweb/WebContent/js/minified;dmadminweb/WebContent/js/ao;dmadminweb/WebContent/js/plugins;dmadminweb/WebContent/js/themes/apple;dmadminweb/WebContent/js/themes/classic;dmadminweb/WebContent/js/themes/default;dmadminweb/WebContent/js/themes/default-rtl;dmadminweb/WebContent/themes/apple;dmadminweb/WebContent/themes/classic;dmadminweb/WebContent/themes/default;dmadminweb/WebContent/themes/default-rtl;dmadminweb/WebContent/userimages;dmadminweb/WebContent/js/jqwidgets/**;dmadminweb/WebContent/reports;dmadminweb/WebContent/META-INF;dmadminweb/WebContent/WEB-INF;dmadminweb/WebContent/WEB-INF/schema;dmadminweb/WebContent/WEB-INF/lib;NetHelp/**;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant War
	SCRIPT Ant War Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}

ms-general.war : dmadminweb/WebContent/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/img/network/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/img/timeline/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/cupertino/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/*.svg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Rest/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/filetypes/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Home/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.xml \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/syntax/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_lib/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/edit_area/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/cultures/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/minified/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/minified/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/ao/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/plugins/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/userimages/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/jqwidgets/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/reports/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/META-INF/*.MF \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.jks \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/microservice/ms-general/web.xml \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/schema/*.sql \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/schema/*.json \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.tld \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
	NetHelp/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
	ms-general.javac \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5]
	FINALTARGET ms-general.war
	TARGET ms-general.war
	TGTFILENAME ms-general.war.tgt
	PROJECTDIRS dmadminweb/WebContent;dmadminweb/WebContent/css/img/network;dmadminweb/WebContent/css/img/timeline;dmadminweb/WebContent/css;dmadminweb/WebContent/css/cupertino/images;dmadminweb/WebContent/css/cupertino;dmadminweb/WebContent/css/images;dmadminweb/WebContent/css/images/images;dmadminweb/WebContent/Rest/**;dmadminweb/WebContent/hot;dmadminweb/WebContent/images;dmadminweb/WebContent/images/filetypes;dmadminweb/WebContent/js/_demo;dmadminweb/WebContent/Home;dmadminweb/WebContent/js/_docs;dmadminweb/WebContent/js/_docs/syntax;dmadminweb/WebContent/js/_lib;dmadminweb/WebContent/js;dmadminweb/WebContent/js/edit_area/**;dmadminweb/WebContent/js/cultures;dmadminweb/WebContent/js/i18n;dmadminweb/WebContent/js/minified/i18n;dmadminweb/WebContent/js/minified;dmadminweb/WebContent/js/ao;dmadminweb/WebContent/js/plugins;dmadminweb/WebContent/js/themes/apple;dmadminweb/WebContent/js/themes/classic;dmadminweb/WebContent/js/themes/default;dmadminweb/WebContent/js/themes/default-rtl;dmadminweb/WebContent/themes/apple;dmadminweb/WebContent/themes/classic;dmadminweb/WebContent/themes/default;dmadminweb/WebContent/themes/default-rtl;dmadminweb/WebContent/userimages;dmadminweb/WebContent/js/jqwidgets/**;dmadminweb/WebContent/reports;dmadminweb/WebContent/META-INF;dmadminweb/WebContent/WEB-INF;dmadminweb/microservice/ms-general;dmadminweb/WebContent/WEB-INF/schema;dmadminweb/WebContent/WEB-INF/lib;NetHelp/**;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant War
	SCRIPT Ant War Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-general/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}

ms-init.war : dmadminweb/WebContent/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/img/network/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/img/timeline/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/cupertino/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/*.svg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Rest/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/filetypes/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.xml \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/syntax/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_lib/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/edit_area/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/cultures/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/minified/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/minified/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/ao/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/plugins/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/userimages/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/jqwidgets/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/reports/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/META-INF/*.MF \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.jks \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/microservice/ms-init/web.xml \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/schema/*.sql \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.tld \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
	NetHelp/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
	ms-init.javac \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5]
	FINALTARGET ms-init.war
	TARGET ms-init.war
	TGTFILENAME ms-init.war.tgt
	PROJECTDIRS dmadminweb/WebContent;dmadminweb/WebContent/css/img/network;dmadminweb/WebContent/css/img/timeline;dmadminweb/WebContent/css;dmadminweb/WebContent/css/cupertino/images;dmadminweb/WebContent/css/cupertino;dmadminweb/WebContent/css/images;dmadminweb/WebContent/css/images/images;dmadminweb/WebContent/Rest/**;dmadminweb/WebContent/hot;dmadminweb/WebContent/images;dmadminweb/WebContent/images/filetypes;dmadminweb/WebContent/js/_demo;dmadminweb/WebContent/js/_docs;dmadminweb/WebContent/js/_docs/syntax;dmadminweb/WebContent/js/_lib;dmadminweb/WebContent/js;dmadminweb/WebContent/js/edit_area/**;dmadminweb/WebContent/js/cultures;dmadminweb/WebContent/js/i18n;dmadminweb/WebContent/js/minified/i18n;dmadminweb/WebContent/js/minified;dmadminweb/WebContent/js/ao;dmadminweb/WebContent/js/plugins;dmadminweb/WebContent/js/themes/apple;dmadminweb/WebContent/js/themes/classic;dmadminweb/WebContent/js/themes/default;dmadminweb/WebContent/js/themes/default-rtl;dmadminweb/WebContent/themes/apple;dmadminweb/WebContent/themes/classic;dmadminweb/WebContent/themes/default;dmadminweb/WebContent/themes/default-rtl;dmadminweb/WebContent/userimages;dmadminweb/WebContent/js/jqwidgets/**;dmadminweb/WebContent/reports;dmadminweb/WebContent/META-INF;dmadminweb/WebContent/WEB-INF;dmadminweb/microservice/ms-init;dmadminweb/WebContent/WEB-INF/schema;dmadminweb/WebContent/WEB-INF/lib;NetHelp/**;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant War
	SCRIPT Ant War Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-init/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}

ms-ui.war : dmadminweb/WebContent/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/img/network/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/img/timeline/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/cupertino/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/*.svg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Rest/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/filetypes/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Home/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.xml \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/syntax/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_lib/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/edit_area/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/cultures/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/minified/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/minified/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/ao/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/plugins/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/userimages/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/jqwidgets/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/reports/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/META-INF/*.MF \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.jks \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/microservice/ms-ui/web.xml \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/schema/*.sql \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.tld \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
	NetHelp/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
	ms-ui.javac \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5]
	FINALTARGET ms-ui.war
	TARGET ms-ui.war
	TGTFILENAME ms-ui.war.tgt
	PROJECTDIRS dmadminweb/WebContent;dmadminweb/WebContent/css/img/network;dmadminweb/WebContent/css/img/timeline;dmadminweb/WebContent/css;dmadminweb/WebContent/css/cupertino/images;dmadminweb/WebContent/css/cupertino;dmadminweb/WebContent/css/images;dmadminweb/WebContent/css/images/images;dmadminweb/WebContent/Rest/**;dmadminweb/WebContent/hot;dmadminweb/WebContent/images;dmadminweb/WebContent/images/filetypes;dmadminweb/WebContent/js/_demo;dmadminweb/WebContent/Home;dmadminweb/WebContent/js/_docs;dmadminweb/WebContent/js/_docs/syntax;dmadminweb/WebContent/js/_lib;dmadminweb/WebContent/js;dmadminweb/WebContent/js/edit_area/**;dmadminweb/WebContent/js/cultures;dmadminweb/WebContent/js/i18n;dmadminweb/WebContent/js/minified/i18n;dmadminweb/WebContent/js/minified;dmadminweb/WebContent/js/ao;dmadminweb/WebContent/js/plugins;dmadminweb/WebContent/js/themes/apple;dmadminweb/WebContent/js/themes/classic;dmadminweb/WebContent/js/themes/default;dmadminweb/WebContent/js/themes/default-rtl;dmadminweb/WebContent/themes/apple;dmadminweb/WebContent/themes/classic;dmadminweb/WebContent/themes/default;dmadminweb/WebContent/themes/default-rtl;dmadminweb/WebContent/userimages;dmadminweb/WebContent/js/jqwidgets/**;dmadminweb/WebContent/reports;dmadminweb/WebContent/META-INF;dmadminweb/WebContent/WEB-INF;dmadminweb/microservice/ms-ui;dmadminweb/WebContent/WEB-INF/schema;dmadminweb/WebContent/WEB-INF/lib;NetHelp/**;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant War
	SCRIPT Ant War Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/microservice/ms-ui/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}

dmadminweb.war : dmadminweb/WebContent/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/img/network/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/img/timeline/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/cupertino/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/cupertino/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/*.svg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/css/images/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/css/images/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Rest/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/hot/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/images/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/images/filetypes/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/Home/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_demo/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/*.xml \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_docs/syntax/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/_docs/syntax/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/_lib/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/edit_area/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/cultures/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/minified/i18n/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/minified/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/ao/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/plugins/*.js \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/js/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.jpg \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/apple/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/apple/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/classic/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/classic/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.gif \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/themes/default-rtl/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/themes/default-rtl/*.css \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[37] \
	dmadminweb/WebContent/userimages/*.png \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/js/jqwidgets/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/reports/*.html \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{} DT[5] \
	dmadminweb/WebContent/META-INF/*.MF \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Meta-Inf)[378]{ dir="dmadminweb/WebContent/META-INF"}  BTOG(Ant War|Meta-Inf)[95]{ prefix=META-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/*.jsp \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.jks \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.xml \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/schema/*.sql \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/schema/*.json \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[37] \
	dmadminweb/WebContent/WEB-INF/*.tld \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf)[314]{ dir="dmadminweb/WebContent/WEB-INF"}  BTOG(Ant War|Web-Inf)[95]{ prefix=WEB-INF} DT[5] \
	dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Lib)[378]{ dir="dmadminweb/WebContent/WEB-INF/lib"}  BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5] \
	NetHelp/**/*.* \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Help)[314]{ dir="NetHelp"}  BTOG(Ant War|Help)[31]{ prefix="Help"} DT[37] \
	dmadminweb.javac \
		VERSIONINFO  \
		RELOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5] \
		DEBOPTIONS TBRT(Ant War|Build Task Options)[346]{ compress=true} TBRT(Ant War|Build Task Options)[91]{ webxml=} TBRT(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBRT(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"} TBTOG(Ant War|Web-Inf Classes)[378]{ dir="dmadminweb/src"}  BTOG(Ant War|Web-Inf Classes)[95]{ prefix=WEB-INF/classes} DT[5]
	FINALTARGET dmadminweb.war
	TARGET dmadminweb.war
	TGTFILENAME dmadminweb.war.tgt
	PROJECTDIRS dmadminweb/WebContent;dmadminweb/WebContent/css/img/network;dmadminweb/WebContent/css/img/timeline;dmadminweb/WebContent/css;dmadminweb/WebContent/css/cupertino/images;dmadminweb/WebContent/css/cupertino;dmadminweb/WebContent/css/images;dmadminweb/WebContent/css/images/images;dmadminweb/WebContent/Rest/**;dmadminweb/WebContent/hot;dmadminweb/WebContent/images;dmadminweb/WebContent/images/filetypes;dmadminweb/WebContent/js/_demo;dmadminweb/WebContent/Home;dmadminweb/WebContent/js/_docs;dmadminweb/WebContent/js/_docs/syntax;dmadminweb/WebContent/js/_lib;dmadminweb/WebContent/js;dmadminweb/WebContent/js/edit_area/**;dmadminweb/WebContent/js/cultures;dmadminweb/WebContent/js/i18n;dmadminweb/WebContent/js/minified/i18n;dmadminweb/WebContent/js/minified;dmadminweb/WebContent/js/ao;dmadminweb/WebContent/js/plugins;dmadminweb/WebContent/js/themes/apple;dmadminweb/WebContent/js/themes/classic;dmadminweb/WebContent/js/themes/default;dmadminweb/WebContent/js/themes/default-rtl;dmadminweb/WebContent/themes/apple;dmadminweb/WebContent/themes/classic;dmadminweb/WebContent/themes/default;dmadminweb/WebContent/themes/default-rtl;dmadminweb/WebContent/userimages;dmadminweb/WebContent/js/jqwidgets/**;dmadminweb/WebContent/reports;dmadminweb/WebContent/META-INF;dmadminweb/WebContent/WEB-INF;dmadminweb/WebContent/WEB-INF/schema;dmadminweb/WebContent/WEB-INF/lib;NetHelp/**;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Ant War
	SCRIPT Ant War Task.sc
	OSNAME Java
	ALLRELOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Ant War|Build Task Options)[346]{ compress=true} TBTOG(Ant War|Build Task Options)[91]{ webxml=} TBTOG(Ant War|Build Task Options)[378]{ basedir="dmadminweb/WebContent" manifest="dmadminweb/WebContent/META-INF/MANIFEST.MF"} TBTOG(Ant War|Build Task Options)[379]{ webxml="dmadminweb/WebContent/WEB-INF/web.xml"}  BTOG(Ant War|Build Task Options)[0]{}

deployhub-webadmin.classpath : dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	annotations-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-ant.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina-ha.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-tribes.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	ecj-P20140317-1600.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	el-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jasper.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	jasper-el.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jsp-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	servlet-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-coyote.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-dbcp.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-es.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-i18n-fr.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-ja.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-jdbc.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-util.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat7-websocket.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	websocket-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37]
	FINALTARGET deployhub-webadmin.war
	TARGET deployhub-webadmin.classpath
	TGTFILENAME deployhub-webadmin.war.tgt
	PROJECTDIRS dmadminweb/WebContent/WEB-INF/lib;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Set Classpath
	SCRIPT Set Classpath.sc
	OSNAME Java
	ALLRELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}
	ALLDEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}

ms-general.classpath : dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	annotations-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-ant.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina-ha.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-tribes.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	ecj-P20140317-1600.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	el-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jasper.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	jasper-el.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jsp-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	servlet-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-coyote.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-dbcp.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-es.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-i18n-fr.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-ja.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-jdbc.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-util.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat7-websocket.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	websocket-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37]
	FINALTARGET ms-general.war
	TARGET ms-general.classpath
	TGTFILENAME ms-general.war.tgt
	PROJECTDIRS dmadminweb/WebContent/WEB-INF/lib;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Set Classpath
	SCRIPT Set Classpath.sc
	OSNAME Java
	ALLRELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}
	ALLDEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}

ms-init.classpath : dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	annotations-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-ant.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina-ha.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-tribes.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	ecj-P20140317-1600.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	el-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jasper.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	jasper-el.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jsp-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	servlet-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-coyote.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-dbcp.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-es.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-i18n-fr.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-ja.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-jdbc.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-util.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat7-websocket.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	websocket-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37]
	FINALTARGET ms-init.war
	TARGET ms-init.classpath
	TGTFILENAME ms-init.war.tgt
	PROJECTDIRS dmadminweb/WebContent/WEB-INF/lib;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Set Classpath
	SCRIPT Set Classpath.sc
	OSNAME Java
	ALLRELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}
	ALLDEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}

ms-ui.classpath : dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	annotations-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-ant.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina-ha.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-tribes.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	ecj-P20140317-1600.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	el-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jasper.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	jasper-el.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jsp-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	servlet-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-coyote.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-dbcp.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-es.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-i18n-fr.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-ja.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-jdbc.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-util.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat7-websocket.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	websocket-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37]
	FINALTARGET ms-ui.war
	TARGET ms-ui.classpath
	TGTFILENAME ms-ui.war.tgt
	PROJECTDIRS dmadminweb/WebContent/WEB-INF/lib;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Set Classpath
	SCRIPT Set Classpath.sc
	OSNAME Java
	ALLRELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}
	ALLDEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}

dmadminweb.classpath : dmadminweb/WebContent/WEB-INF/lib/*.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	annotations-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-ant.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	catalina-ha.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	catalina-tribes.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	ecj-P20140317-1600.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	el-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jasper.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	jasper-el.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	jsp-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	servlet-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-coyote.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-dbcp.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-es.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-i18n-fr.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-i18n-ja.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat-jdbc.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	tomcat-util.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
	tomcat7-websocket.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[5] \
	websocket-api.jar \
		VERSIONINFO  \
		RELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{} DT[37]
	FINALTARGET dmadminweb.war
	TARGET dmadminweb.classpath
	TGTFILENAME dmadminweb.war.tgt
	PROJECTDIRS dmadminweb/WebContent/WEB-INF/lib;
	INTDIR
	BUILDTYPE Java War
	BUILDTASK Set Classpath
	SCRIPT Set Classpath.sc
	OSNAME Java
	ALLRELOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}
	ALLDEBOPTIONS BTOG(Set Classpath|Build Task Options)[0]{}

all : nginx.tar \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	deployhub-webadmin.war \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	ms-general.war \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	ms-init.war \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	ms-ui.war \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	dmadminweb.war \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0]

--Start:Ant Tar Task.sc
#########################################
#-- Set script version number
#   Last updated:
#    JAG - 02.11.05 - fixs
#    JAG - 12.08.04 - eliminate copylocal step
#    JAG - 07.13.04 - make 'build.xml' more meaningful
my $ScriptVersion = '$Header: /CVS/openmake64/tomcat/webapps/openmake.ear/openmake.war/scripts/Ant\040Tar\040Task.sc,v 1.4 2009/04/20 17:49:19 steve Exp $';
 #-- Clean up $ScriptVersion so that it prints useful information
 if ($ScriptVersion =~ /^\s*\$Header:\s*(\S+),v\s+(\S+)\s+(\S+)\s+(\S+)/ )
 {
  my $path = $1;
  my $version = $2;
  my $date = $3;
  my $time = $4;

  #-- massage path
  $path =~ s/\\040/ /g;
  my @t = split /\//, $path;
  my $file = pop @t;
  my $module = $t[2];

 $ScriptVersion = "$module, v$version";
 }

#########################################
#-- Load Openmake Variables from Data File
#   Uncomment Openmake objects that need to be loaded

@PerlData = $AllDeps->load("AllDeps", @PerlData );
@PerlData = $RelDeps->load("RelDeps", @PerlData );
#  @PerlData = $NewerDeps->load("NewerDeps", @PerlData );
@PerlData = $TargetDeps->load("TargetDeps", @PerlData );
@PerlData = $TargetRelDeps->load("TargetRelDeps", @PerlData );
#  @PerlData = $TargetSrcDeps->load("TargetSrcDeps", @PerlData );

$StepDescription = "Jar File Creation for " . $Target->get;
$Verbose = $Quiet =~ /no/i;
$LogOutputNL = "";
my $cwd = &getcwd();
my @IncludesLines = ();
my $uses_zip = 1; #-- all tasks use zipfilesets

$ENV{PATH} = $ENV{JAVA_HOME} . $DL . "bin" . $PathDL . $ENV{PATH} if ($ENV{JAVA_HOME});

#########################################
#-- Define global variables
#
#-- get the name from the $BuildTask variable
my $Ant_Type = "tar";
my $Task = "";
if ( $BuildTask =~ /Ant.?\s+(\w+)$/ )
{
 $Ant_Type = $1;
 $Ant_Type = lc($Ant_Type);
}

#-- the following are files that might be specified
#   as an option, where the script will have to
#   parse out and substitute a file.
my %TaskOptionFiles = (
                       "manifest" => 1
                      );

#-- define an array of the "Results From" dependency extensions
#   that need to be parsed for sub task dependencies. Each
#   extension added to the array will be scanned.
my @sub_task_exts = qw(
		       .javac
		       .rmic
		       .jup
               .copy
		      );


#-- set the name of the build.xml file. This is used
#   for debug purposes to have a more meaningful name
#   when the -ks option is used.
my $Build_XML_File = 'build_' . $Target->get;
$Build_XML_File =~ s|\W+|_|g;
$Build_XML_File .= '.xml';
push @dellist, $Build_XML_File unless $KeepScript =~ /y/i;
my $CompilerArgs .= ' -buildfile ' . $Build_XML_File;

#########################################
#-- determine the configuration
#
my $optionref;
( $CFG eq "DEBUG" ) ? $optionref = $DebugFlags : $optionref = $ReleaseFlags;

#########################################
#-- create the option objects
#
use Openmake::BuildOption;
my $buildopt = Openmake::BuildOption->new($optionref);

#########################################
#-- determine how many Build tasks/Option Groups we have
my @build_tasks = $buildopt->getBuildTasks;
my $build_task = $build_tasks[0];

#-- find the build task that matches to the task
$build_task = $BuildTask if ( grep /$BuildTask/, @build_tasks );

#-- find the optiongroups;
my @option_groups = $buildopt->getOptionGroups($build_task);

#-- find the compiler
$Compiler = &GetAnt();

#########################################
#-- Generate Bill of Materials if Requested
#
&GenerateBillofMat($BillOfMaterialFile->get,$BillOfMaterialRpt,$Target->get) if( defined $BillOfMaterialFile );

#########################################
# Special Options section
#
#-- the following is a loop over possible keys in the task argument
#   that we might want to find in the Build Task general options
#   For example, in Ant Tar, we look for Manifest.mf
#   in the list of dependencies
#
#   We define a hash %TaskOptionFiles (at top of script)
#   to tell us what to look for
#   Note that the default type (eg. war) is defined from the
#   build type
#
foreach my $wantedkey ( keys %TaskOptionFiles)
{
 my $wantedfile = $buildopt->getBuildTaskOption( $wantedkey, $build_task );
 $wantedfile =~ s|\\|\/|g;
 $wantedfile =~ s|^"||; #"
 $wantedfile =~ s|"$||; #"
 my $wantedmatch = quotemeta($wantedfile);

 my @foundfiles = ();
 #-- find files that match to the extension, then match to the
 #   full file name
 my $wantedmatchext = "\\.NOEXT";
 if ( $wantedfile =~ /(\.[^\.]*)\s*$/ )
 {
  $wantedmatchext =  $1;
 }
 @foundfiles = $TargetDeps->getExt($wantedmatchext);

 foreach my $foundfile ( @foundfiles )
 {
  $foundfile =~ s|\\|\/|g;
  if ( $foundfile =~ /$wantedmatch$/ && -e $foundfile )
  {
   #-- take the first-found
   $foundfile =~ s|\$|\$\$|g;
   $TaskOptionFiles{$wantedkey} = $foundfile;
   $LogOutputNL .= "\nFound Build Task requested $wantedkey file:\n\n $foundfile\n\n";
   last;
  }
 }
}

##########################################
#
#-- Set the IntDir

$IntDirDL = $IntDir->get . "/";

##########################################
#
#-- Determine the overall basedir

my $BaseDir = $buildopt->getBuildTaskOption( "basedir", $build_task, $OPTIONGROUP_DEFAULT_NAME );
$BaseDir =~ s/^"//; #"
$BaseDir =~ s/"$//; #"

#-- Create the BaseDir with the Intermediate Directory appended as well
my $IntBaseDir = $BaseDir;
if($IntDirDL ne './' && $IntDirDL ne '/')
{
 $IntBaseDir = $IntDirDL . $BaseDir;
}

my $Target_Path = $Target->getDP;
$Target_Path =~ s/\\/\//g;
$Target_Path =~ s/\/$//;
$Target_Path = "" if ( $Target_Path eq "." );

##########################################
#
#-- each Option Group is a separate zipfile set
#
my $i = 0;
foreach my $option_group ( @option_groups )
{
 #-- the "dir" key is a special case that we always look for.
 #   In the future, we will replace this without needing a copy local
 #
 my $wanted_dir = $buildopt->getBuildTaskOption( "dir", $build_task, $option_group) || $BaseDir;
 $wanted_dir =~ s/^"//;
 $wanted_dir =~ s/"$//;

 my @wanted_dirs = ();
 my %wanted_file_hash = ();
 my @derived_deps = ();
 my @subtask_deps = ();

 #-- get the files in localres that correspond to this option group
 #-- JAG 04.14.04 - remove 1 as last option -- was returning all files (not
 #                  just files for this build)
 my ($file_ref, $opt_ref ) = $buildopt->getBuildTaskFiles( $build_task, $option_group );
 my @option_files = @{$file_ref};


 #-- remove files that are keyed in %TaskOptionFiles
 my $i = 0;
 my @t_option_files = @option_files;
 foreach my $file ( @option_files )
 {
  foreach my $val ( values %TaskOptionFiles )
  {
   #-- assume lowercase is okay here
   if ( lc $val eq (lc $file) )
   {
    splice( @t_option_files, $i, 1);
    $i--;
   }
  }
  $i++;
 }
 @option_files = @t_option_files;

 ####################################################################################################
 #-- Retrieve all the sub task deps for each matching extension from @sub_task_exts defined at top
 #   Use GetSubTaskDeps to resolve all of the derived dependencies
 #   Also return $dir_list - the list of relative dirs where the matching sup task deps are found ADG 3.15.05
 foreach $sub_task_ext (@sub_task_exts)
 {
  if (grep /$sub_task_ext$/, @option_files)
  {
   my ($rel_dir, @ret_subtask_deps) = &GetSubTaskDeps($TargetDeps,$TargetRelDeps,$wanted_dir,$sub_task_ext);
   push @subtask_deps, @ret_subtask_deps;
   #-- remove the sub task extension files from the @option_files
   @option_files = grep { $_ !~ /$sub_task_ext$/ } @option_files;
   $dir_list .= ",$rel_dir";
  }
 }

 if ($Verbose && @subtask_deps != "") #only log if subtask_deps exist
 {
  $LogOutputNL .= "\nIncluding from Sub-Task Dependencies:\n\n";
  foreach (@subtask_deps) { $LogOutputNL .= " $_\n" if $_ =~ /\w+/ ; } #Added to strip out empty strings from logging AG 081804
 }

 #-- JAG - add package classes to TargetRelDeps so that we can use TargetRelDeps later
 #         in AntFilesetOrg
 foreach ( @subtask_deps ) { $TargetRelDeps->push($_); }

 push @option_files, @subtask_deps; #join the found subtask deps with the exisiting option files
 foreach (@option_files) { $_ =~ s|\\|/|g; } #all slashes forward
 next unless ( @option_files );

 #-- we pass in the prefix option, in case we need to split on that
 #
 my $prefix =  $buildopt->getBuildTaskOption( "prefix", $build_task, $option_group );
 $prefix =~ s|\\|\/|g;
 $prefix =~ s|^"||; #"
 $prefix =~ s|"$||; #"

 #-- JAG - 02.11.05 - add intdir to list of possible dirs
 my $dir_list .= "$wanted_dir,$IntDirDL,$IntBaseDir";

 $int_dir = $IntDir->getAbsolute;

 my $option_target = Openmake::FileList->new(@option_files);
 %wanted_file_hash = &AntFilesetOrg($option_target,$TargetRelDeps,$dir_list,$int_dir,$prefix,qw( .MF .mf .javac .classpath ));

 push @wanted_dirs, (keys %wanted_file_hash);

 #-- strip off the leading ${dir} off the resources if necessary
 #   this replaces module dir.
 foreach my $temp_dir ( @wanted_dirs )
 {
  if ( $temp_dir)
  {
   $temp_dir =~ s|^"||; #"
   $temp_dir =~ s|"$||; #"

   $temp_dir =~ s|^\.[\\\/]||;

   my $etemp_dir = $temp_dir;
   $etemp_dir =~ s|\\|\/|g;
   $etemp_dir =~ s|\/|\\\/|g;
  }

  #-- write build.xml lines for resources
  if (@{$wanted_file_hash{$temp_dir}} )
  {
   #-- get the options at the Option Group. Ignore the "Default case", since
   #   this will be handled at the root level
   my @options = $buildopt->getBuildTaskOptions( $build_task, $option_group);

   #-- need to parse options for missing quotes
   foreach my $opt ( @options )
   {
    next unless $opt;

    #-- ignore the dir option as specified in the options if
    #   we've redefined it locally in this loop. Check for IntDir as
    #   well
    #
    if ( $opt =~ /(\w*dir)\s*=(.+)/ )
    {
     my $t_opt  = $1;
     my $t_end  = $2;
     $t_end =~ s/^"//;
     $t_end =~ s/"$//;
     $opt = $t_opt . "=\"$temp_dir\"";
     next;
    }

    #-- following gets rid of ill-defined options like manifest=
    if ( $opt =~ /=(")?$/ ) #"
    {
     $opt = "";
     next;
    }
    next if ( $opt =~ /="/ && $opt =~ /"$/); #-- if it's quoted, it's fine.
    $opt =~ s|=|="|; #"
    $opt .= "\"";
   }
   $options = join " ", @options;
   $options =~ s|\s+| |g;

   #-- reset the options if this is the root level. They get set
   #   below.
   if ( $option_group eq $Openmake::BuildOption::OPTIONGROUP_DEFAULT_NAME )
   {
    $options = "";
    if ( $temp_dir ne "." && $temp_dir ne $IntBaseDir)
    {
     $options = "dir=\"$temp_dir\"";
    }
   }

   #-- if $temp_dir is "." and there's a $Target_Path ...
   my $output_dir = $temp_dir;
   $output_dir = $Target_Path . "/" if ( $temp_dir eq "." );

   if ($Quiet =~ /no/i)
   {
    $LogOutputNL .= "\nAdding to $Ant_Type from Directory:";
    $LogOutputNL .= "\n$temp_dir\n\n";
    foreach (@{$wanted_file_hash{$temp_dir}}) { $LogOutputNL .= " $_\n"; }
   }
   #-- Determine if we created a separate zipfileset within the Build Option
   #   Group. If so, add "zipfileset"
   #   Otherwise, add the code under the root level (<tar>, etc)
   #
   if  ( $option_group ne $Openmake::BuildOption::OPTIONGROUP_DEFAULT_NAME
         || $output_dir ne $IntBaseDir || $uses_zip  )
   {
    if ( $options !~ /dir=/ )
    {
     if ( $output_dir && ( $output_dir ne $IntBaseDir || $uses_zip ))
     {
      $options .= " dir=\"$output_dir\" ";
     }
     else
     {
      $options .= " dir=\"\${$Ant_Type}\" ";
     }
    }

    push(@IncludesLines, "tarfileset $options\n");
    push(@IncludesLines, GetAntIncludeXML( @{$wanted_file_hash{$temp_dir}} ) );
    push(@IncludesLines, "/tarfileset\n");
    $uses_zip = 1;
   }
   else
   {
    push(@IncludesLines, GetAntIncludeXML( @{$wanted_file_hash{$temp_dir}} ) );
   }
  }
 } #-- end of loop on possible separate directories in the dir= parameter
} #-- end of zipfilesets.

##################################################
# Write Build.xml
#
my $xml =<<ENDXML;
project name = "$Project" default = "$Ant_Type" basedir= "."

 !-- Set properties --
 !--   ignore the classpath that Ant is running under --
 property name = "build.sysclasspath" value = "ignore" /

 property name = "$Ant_Type" value = "." /

 !-- Start $Ant_Type section --
  target name = "$Ant_Type"
ENDXML
$xml .= "  $Ant_Type destfile = \"$E{$Target->get}\" ";

#-- add in Task Level options, parsing out the pieces that we may have
#   replaced
my @options = $buildopt->getBuildTaskOptions($build_task);

#-- if there are no options, we may need to add in a basedir
if ( ( ! grep /basedir=/, @options ) && scalar @option_groups == 1 )
{
 my $dir = $IntBaseDir || ".";
 push @options, "basedir=\"$dir\"";
}
foreach my $opt ( @options )
{
 my ( $key, $val);
 if ( $opt =~ /(\w+)="?(.+)"?/ )
 {
  $key = $1;
  $val = $2;
  $val =~ s/"$//; #"
 }
 if ( $TaskOptionFiles{$key} )
 {
  $xml .= $key . "=\"" . $TaskOptionFiles{$key} . "\" " if ( $TaskOptionFiles{$key} != 1 );
 }
 elsif( $key eq "basedir")
 {
  if( $val =~ /,/)
  {
   #-- ignore basedir if it's comma separated. Do not set to "." otherwise
   #   everything in the basedir gets zipped
   next;
  }
  else
  {
   #-- this is the case where all the files live under the root <tar/war/ear>
   #   task. Here we have to set basedir.
   if ( $IntBaseDir )
   {
    $xml .= "$key=\"$IntBaseDir\" " unless ( $uses_zip );
   }
   elsif ( scalar @option_groups == 1 )
   {
    $xml .= "$key=\"$val\" " unless ($uses_zip) ;
   }
  }
 }
 elsif ( $key && $val )
 {
  $xml .= "$key=\"$val\" ";
 }
}
$xml .= "\n";

$xml .=<<ENDXML2;
   @IncludesLines

  /$Ant_Type
 /target

!-- End the project --
/project
ENDXML2

WriteAntXML($xml, $Build_XML_File );

######################################
# Execute Build
#
&omlogger("Begin",$StepDescription,"FAILED","$StepDescription succeeded.",$Compiler,$CompilerArgs,$LogOutputNL,$RC,@Output);

@CompilerOut = `$Compiler $CompilerArgs 2>&1`;

$RC = Check4Errors("FAILED",$?,@CompilerOut);

&omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,"",$RC,@CompilerOut), $RC = 1 if ($RC != 0);
&omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArguments,"",$RC,@CompilerOut) if ($RC == 0);

#########################################
#-- Generate Footprint if Requested
#
#   For Java, we create a BOM report with the
#   defined name "META-INF/openmake_footprint.log"
#   This gets jarred into the archive
#
#-- Also jar in BOM, but also
#   jar in omXXXXXX.fp file with -0 option (no compression)
#   test with omident.exe
#
if (defined $FootPrintFile)
{
 my $target = $Target->get;
 my $fp_file = $FootPrintFile->get;
 my $bom_fp_file = "META-INF/openmake/openmake_footprint.log";

 my $StepDescription = "Footprint for " . $target;
 my $Compiler = "";
 my @CompilerOut = ();

 mkfulldir( $bom_fp_file);
 #-- JAG - 04.20.06 - if the BOM already exists in text form, use it
 if ( -e $BillOfMaterialRpt )
 {
  my $file = Openmake::File->new($BillOfMaterialRpt)->getDPF();
  $file = $file . '.txt' if ( -e $file . '.txt' );
  $file = $file . '.txt' if ( -e $file . '.log' );
  copy( $file, $bom_fp_file);
 }
 else
 {
  GenerateBillofMat($fp_file, $bom_fp_file, $target);
 }

 #-- everything forward for Java
 $target      =~ s|\\|\/|g;
 $fp_file     =~ s|\\|\/|g;
 $bom_fp_file =~ s|\\|\/|g;

 #-- add the bom
 my @bom_jar = `jar -uf \"$target\" \"$bom_fp_file\" 2>&1`;
 my $RC = $?;
 if ( $RC != 0 )
 {
  push @CompilerOut, @bom_jar;
 }
 push @dellist, $bom_fp_file unless $KeepScript =~ /y/i;

 #-- add the Footprint file. Copy local
 my @t = split "/", $fp_file;
 my $rel_fp_file = pop @t;
 $rel_fp_file = "META-INF/openmake/". $rel_fp_file;

 #-- format the .fp file in the format expected by omident, with $OMBOM, etc
 GenerateFootPrint( {
                      'FootPrint' => $fp_file,
                      'FPSource'  => $rel_fp_file,
                      'FPType'    => 'Java',
                      'Compiler'  => 'tar',
                      'CompilerArguments' => "-u0f \"$target\" \"$rel_fp_file\" 2>&1"
                    }
                   );

 &omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,"",$RC,@CompilerOut), $RC = 1 if ($RC != 0);
 &omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArguments,"",$RC,@CompilerOut) if ($RC == 0);

}


--End:Ant Tar Task.sc
--Start:Ant Javac Task.sc
#########################################
#-- Set script version number
#   Last updated:
#    JAG - 07.13.04 - make 'build.xml' more meaningful
my $ScriptVersion = '$Header: /CVS/openmake64/tomcat/webapps/openmake.ear/openmake.war/scripts/Ant\040Javac\040Task.sc,v 1.21 2010/11/04 21:39:10 layne Exp $';
#-- Clean up $ScriptVersion so that it prints useful information
if ($ScriptVersion =~ /^\s*\$Header:\s*(\S+),v\s+(\S+)\s+(\S+)\s+(\S+)/ )
{
 my $path = $1;
 my $version = $2;
 my $date = $3;
 my $time = $4;

 #-- massage path
 $path =~ s/\\040/ /g;
 my @t = split /\//, $path;
 my $file = pop @t;
 my $module = $t[2];

 $ScriptVersion = "$module, v$version";
}

#########################################
#-- Load Openmake Variables from Data File
#   Uncomment Openmake objects that need to be loaded

#  @PerlData = $AllDeps->load("AllDeps", @PerlData );
#  @PerlData = $RelDeps->load("RelDeps", @PerlData );
@PerlData = $NewerDeps->load("NewerDeps", @PerlData );
@PerlData = $TargetDeps->load("TargetDeps", @PerlData );
@PerlData = $TargetRelDeps->load("TargetRelDeps", @PerlData );
#  @PerlData = $TargetSrcDeps->load("TargetSrcDeps", @PerlData );

$StepDescription = "Javac Task for " . $Target->get;
$TargetFile = $Target->get;
$Verbose = $Quiet =~ /no/i;
$LogOutputNL = "";
my $cwd = &getcwd();
my @IncludesLines = ();

$ENV{PATH} = $ENV{JAVA_HOME} . $DL . "bin" . $PathDL . $ENV{PATH} if ($ENV{JAVA_HOME});

#########################################
#-- Define global variables
#
#-- get the name from the $BuildTask variable
my $Ant_Type = "javac";
if ( $BuildTask =~ /Ant\s+.?\s+(\w+)$/ )
{
 $Ant_Type = $1;
 $Ant_Type = lc($Ant_Type);
}

#-- the following are files that might be specified
#   as an option, where the script will have to
#   parse out and substitute a file.
my %TaskOptionFiles = ();


#-- because we call javac.main directly, we have to translate Ant to javac attributes
#   that's what this hash does
#
#   The hash is set up to be keyed by Ant attribute. The value is an array.
#   The first array element is a bitmask indicating that either, both, or neither
#   of the Ant attribute or the javac command line takes a flag
#    1 = Ant takes option
#    2 = javac takes option
#   second element is the flag to pass to the javac compiler. if the compiler
#   takes an option, it gets added on directly behind the flag, so note the space.
#
#   In the case of Ant taking an option but javac not, (bitmask == 1 ), it's assumed
#   the Ant option is either "on|off"
#
#   Also, Destdir and compiler are special cases treated below.
#
#-- JAG - 04.29.04 - case 4610, added extra options
my %AntJavacOptions = ( "encoding" => [3, "-encoding "],
                        "nowarn" => [1, "-nowarn "],
                        #-- JAG - 05.09.05 - case 5707 - debug is now a special case
                        #"debug" => [1, "-g "],
                        #"debuglevel" => [ 3, "-g:"],
                        "optimize" => [1, "-O"],
                        "deprecation" => [ 1, "-deprecation"],
                        "verbose" => [1, "-verbose"],
                        "target"  => [3, "-target"],
                        "source"  => [3, "-source"],
                        "encoding" => [3, "-encoding"]
                     );

#-- set the name of the build.xml file. This is used
#   for debug purposes to have a more meaningful name
#   when the -ks option is used.
my $Build_XML_File = 'build_' . $Target->get;
$Build_XML_File =~ s|\W+|_|g;
$Build_XML_File .= '.xml';
push @dellist, $Build_XML_File unless $KeepScript =~ /y/i;
my $CompilerArgs .= ' -buildfile ' . $Build_XML_File;

#-- default options defined for Javac. These get
#   filled in below
my $ClassPath;
my $BootClassPath;
my $JikesClassPath;
my $SourcePath;
my $global_options;

#########################################
#-- determine the configuration
#
my $optionref = $ReleaseFlags;
$optionref    = $DebugFlags if ( $CFG eq "DEBUG" );

#########################################
#-- create the option objects
#
use Openmake::BuildOption;
my $buildopt = Openmake::BuildOption->new($optionref);

#########################################
#-- determine how many Build tasks/Option Groups we have
my @build_tasks = $buildopt->getBuildTasks;
my $build_task = $build_tasks[0];

#-- find the build task that matches to the task
$build_task = $BuildTask if ( grep /$BuildTask/, @build_tasks );

#-- find the optiongroups;
my @option_groups = $buildopt->getOptionGroups($build_task);
my $option_group ;

#-- javac: if more than one option group, error out
if ( scalar (@option_groups ) > 1 )
{
 &omlogger("Begin",$StepDescription,"FAILED","$StepDescription failed!",$Compiler,$CompilerArgs,$LogOutputNL,$RC,@Output);
 $RC = 1;
 push(@CompilerOut, "ERROR: More than one Option Group defined\n", "\n");
 push @dellist, $Target->get;

 &omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,"",$RC,@CompilerOut), $RC = 1 if ($RC != 0);

 &ExitScript($RC);
}
else
{
 $option_group = $option_groups[0];
}

#-- if more than one Destdir specified (through a comma-separated list), error out.
#   This could have occurred in the old-style 6.2 targets. Print error.
#
my $destdir = $buildopt->getBuildTaskOption( "destdir", $build_task, $option_group);
if ( $destdir =~ /,/ )
{
 &omlogger("Begin",$StepDescription,"FAILED","$StepDescription failed!",$Compiler,$CompilerArgs,$LogOutputNL,$RC,@Output);
 $RC = 1;
 push(@CompilerOut, "ERROR: Multiple Destination directories specified via comma-separated list in destdir= \n" );
 push(@CompilerOut, "Reconfigure the target definition file to specify only one destdir= \n" );
 push @dellist, $Target->get;

 &omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,"",$RC,@CompilerOut), $RC = 1 if ($RC != 0);

 &ExitScript($RC);
}

#-- find the Ant compiler
$Compiler = &GetAnt();

#-- determine how we expect to call the compiler
my $compiler = $buildopt->getBuildTaskOption( "compiler", $build_task, $option_group);

#########################################
#-- Get classpath from Classpath Task
#
$ClassPath = GetClasspath( $TargetDeps );

#########################################
#-- tools.jar is added to access the java compiler
#
#-- JAG - 04.23.07 - case FLS-4 Need to put int-dir + destdir on the classpath
$destdir =~ s{^"}{};
$destdir =~ s{"$}{};
my $intdir_dl = $IntDir->get . $DL;
$destdir = $intdir_dl . $destdir;
$destdir =~ s{\\}{/}g;

$ClassPath = $destdir . $PathDL . $ClassPath;
if ($^O =~ /darwin|osx/i)
{
$ClassPath .= $PathDL . $ENV{JAVA_HOME} . $DL . "../Classes" . $DL . "classes.jar";
}
else
{
$ClassPath .= $PathDL . $ENV{JAVA_HOME} . $DL . "lib" . $DL . "tools.jar";
}

# push(@CompilerArguments,"arg value=\"-bootclasspath\" /\n");
# push(@CompilerArguments,'arg value="' . $ClassPath . "\" /\n");

push(@CompilerArguments,"arg value=\"--add-exports\" /\n");
push(@CompilerArguments,'arg value="' . "java.naming/com.sun.jndi.ldap=ALL-UNNAMED" . "\" /\n");

if ($compiler =~ /jikes/i)
{
 delete $ENV{JIKESPATH};
 push(@CompilerArguments,"arg value=\"-Djikes.class.path\" /\n");
 push(@CompilerArguments,'arg value="' . $ClassPath . "\" /\n");
}

if ($Verbose)
{
 $LogOutputNL .= "\nClassPath Directories:\n";
 $ClassPathNL = $ClassPath;
 $ClassPathNL =~ s/$PathDL/\n/g;
 $LogOutputNL .= $ClassPathNL;
}

#########################################
#-- Get sourcepath from VPath
#
push(@CompilerArguments,"arg value=\"-sourcepath\" /\n");
push(@CompilerArguments,'arg value="' . $VPath->getString("",$PathDL) . "\" /\n");

if ($Verbose)
{
 $LogOutputNL .= "\n\nSourcePath Directories:\n";
 $LogOutputNL .= $VPath->getString("","\n");
}

#########################################
#-- Get the options
#

#-- set the options. Because we only allow one task, we can do this
#   here. May change in future.
my @options = $buildopt->getBuildTaskOptions( $build_task, $option_group);

#-- JAG - 05.09.05 - case 5707 - need special handling of debug/debuglevel
#    to match Ant

my $options_str = join " ", @options;
#if ( $options_str =~ /debug="?on"?/ )
if ( $options_str =~ /debug="?on"?/ || $options_str =~ /debug="?true"?/ ) #-- JAG - 08.24.06 - case 7356
{
 #-- allowable is word, space, comma
 if ( $options_str =~ /debuglevel="([\w\s,]+?)"/ )
 {
  push(@CompilerArguments, "arg value=\"-g:$1\" /\n");
 }
 else
 {
  push(@CompilerArguments, "arg value=\"-g\" /\n");
 }
}
else
{
 # Default must be no -g flag for RELEASE builds (no '-g' flag is different than '-g:none')
 #  this broke backward compatibility - SAB

 # The code above should handle  -g:none if someone wants it

 # push(@CompilerArguments, "arg value=\"-g:none\" /\n");
}

#-- need to parse options for missing quotes

#-- Look to see if any options passed lead with -J -- indicating arguments
#   to pass to the underlying JVM
#
my $jvmargs = "";
foreach my $opt ( @options )
{
 next unless $opt;
  if ( $opt =~ /(\-*\w+)=?"?(\S+)/ )  #" #match param on non-whitespace characters instead of word characters due to version numbers such as 1.4 AG 1.7.05 5353
 {
  my $key = $1;
  my $val = $2;
  $val =~ s|"$||g; #"

  #-- see if this is a guy we need
  #-- JAG - case 4744. If an Option is precedeed by -J, this
  #   should get passed to the JVM.
  if ( $key =~ s/^-J// )
  {
   		$key .= "m" if($key =~ m{Xm[sx]([0-9]+)}); # we accidentally hack off the trailing "m" in the regex above
 		$key = "-" . $key if ($key !~ m{^\-+});
 		$jvmargs .= $key . " ";
  }
  elsif ( $AntJavacOptions{ $key} )
  {
   my ( $bitmask, $joption) = @{$AntJavacOptions{$key}};
   if ( $bitmask & 1 )
   {
    #-- mismatch in options
    next unless $val;

    #-- now see if javac takes an option
    if ( $bitmask & 2 )
    {
     $global_options = "$joption $val"; #AG 1.7.05 5353
    }
    else
    {
     #-- javac doesn't take an option, so look for "on" or "off"
     if ( $val =~ /on/i )
     {
      $global_options = "$joption"; #AG 1.7.05 5353
     }
    }

    #-- add to args
    my @opts = split " ", $global_options;
    foreach ( @opts )
    {
     push(@CompilerArguments, "arg value=\"" . $_ . "\" /\n");
    }
   }
  }
 }
}
$jvmargs =~ s{\s+$}{};

#########################################
#-- set the destdir and the compiler class
$compiler = $buildopt->getBuildTaskOption( "compiler", $build_task, $option_group);

#-- handle the compiler class
#-- default for Modern,javac1.3 or greater
my $CompilerClass = "com.sun.tools.javac.Main";
$CompilerClass = "sun.tools.javac.Main"
  if ($compiler =~ /javac1\.[12]/ || $compiler =~ /classic/i );
#-- tentative kopi support?
$CompilerClass = "at.dms.kjc.Main"
  if ($compiler =~ /kcj/ || $compiler =~ /kopi/i );

#-- JAG - 04.23.07 - case FLS-4
#my $wanted_dir = $buildopt->getBuildTaskOption( "destdir", $build_task, $option_group);
#$wanted_dir =~ s/^"//;
#$wanted_dir =~ s/"$//;
#
#my $intdir_dl = $IntDir->get . $DL;
#-- this needs
#$destdir = $intdir_dl . $wanted_dir;
#$destdir =~ s|\\|\/|g;

push(@CompilerArguments, "arg value=\"-d\" /\n");
push(@CompilerArguments, "arg value=\"$destdir\" /\n");

#-- make sure Destdir exists. Note mkfulldir takes a file, not a path
&mkfulldir($destdir . "/temp" ) unless ( -d $destdir);

#########################################
#-- determine packages based on the java
#  files in the .wsdljava .omidl, .copypkg
#  files and the java dependencies
#
@Newer = &GetPackageDeps($NewerDeps);
push(@Newer, $NewerDeps->getExt(qw(.java)));

#-- case 5026/7010 - SAB
if ( @Newer == () ) { #-- handle case where only classpath has changed
  if ( $NewerDeps->getList ) { #-- if there is something newer that is not java, e.g. .classpath
   @PerlData = $TargetDeps->load("TargetDeps", @PerlData );
   @Newer = $TargetDeps->getExt(qw(.java));
  }
}

if ( @Newer == () ) {
  $RC = 1;
  push(@CompilerOut, "ERROR: Ant Javac Task.sc: No source code was found to compile.\n", "\n");
  push @dellist, $Target->get;

  omlogger("Final",$StepDescription,"ERROR:","$StepDescription failed.",$Compiler,$CompilerArgs,$ClassPathNL,$RC,@CompilerOut);

  ExitScript $RC;
}

#########################################
#-- Remove old classes in package dirs
#
#-- This makes little sense. $Class gets set to "./C:/l..."
$LogOutputNL .= "\n\nOut of Date Files:\n";

foreach $file (sort @Newer)
{
 #$Class = $file;
 #$Class =~ s/\.java/\*\.class/;
 #$Class   = $IntDir->get . $DL . $Class;

 next if ($file =~ /\*/);

 # escape $ in $file for ANT compilation
 if ($file =~ m{\$})
 {
  $escapefile = $file;
  $escapefile =~ s{\$}{\$\$}g;
  push(@FilesToCompile,'arg value="' . $escapefile . "\" /\n");
 }
 else
 {
 push(@FilesToCompile,'arg value="' . $file . "\" /\n");
 }

 $LogOutputNL .= "$file\n";

 #chmod 0777, "$Class";
 #unlink ($Class);
}

##################################################
# Determine if we have jvmargs
#
my  $fork = "fork=\"true\" \n";
if ( $jvmargs )
{
 $jvmargs = "jvmarg value=\"$jvmargs\" /\n";
}

##################################################
# Write Build.xml
#
$xml =<<ENDXML;
project name = "$Project" default = "compile" basedir = "."

!-- Set properties --
!--   ignore the classpath that Ant is running under --
property name = "build.sysclasspath" value = "ignore" /

property name = "src" value = "$E{$IntDir->get}" /
property name = "build" value = "$E{$IntDir->get}" /

!-- Start compile section --
target  name = "compile"
java failonerror="true" classname="$CompilerClass" classpath = "$ClassPath" $fork
 $jvmargs

 @CompilerArguments
 @FilesToCompile

/java
/target

!-- End the project --
/project
ENDXML

######################################
# Execute Build
#
&omlogger("Begin",$StepDescription,"FAILED","$StepDescription succeeded.",$Compiler,$CompilerArgs,$LogOutputNL,$RC,@Output);

#-- JAG - 08.02.05 - case 6145 : if there are no files to compile, don't run
#   Ant. This isn't needed if we were to use the 'Ant Javac' task directly.
if ( @FilesToCompile)
{
 &WriteAntXML($xml, $Build_XML_File );
 @CompilerOut = `$Compiler $CompilerArgs 2>&1`;
}
else
{
 @CompilerOut = ();
}

foreach $l (@CompilerOut)
{
print $l;
 $l =~ s/\[java\]/\[javac\]/;
 $l =~ m/\[javac\][\s.](\d+)[\s.]error/;
 if ($1 ne "")
 {
  $RC += $1;
 }

 $l =~ m/\[javac\] Java Result: (\d+)/;
 if ($1 ne "")
 {
  $RC += $1;
 }
}

$RC += &Check4Errors("FAILED",$?,@CompilerOut);
$RC += &Check4Errors("Could not find",$?,@CompilerOut);
$RC += &Check4Errors("cannot resolve symbol",$?,@CompilerOut);
$RC += &Check4Errors("cannot find symbol",$?,@CompilerOut);

#-- stupid Ant prints "BUILD SUCCESSFUL" when it fails
#   because of not finding the compiler

@CompilerOut = grep !/BUILD SUCCESSFUL/, @CompilerOut
 if grep /Could not find/, @CompilerOut;

#-- JAG - 02.11.05 - see if we have any properties files
#                    these are absolute paths.
my @properties_files = $TargetDeps->getExt(qw(.properties));

#-- this gets classes from .javac filesS
@Classes = &GetPackageDeps($TargetRelDeps);

#-- JAG 02.06.04
#
#   This has to be reworked based on destdir/dir and the Java package structure.
#
#   The following occurs.
#   1. We pass to Ant the full file name
#    - C:/work/Minibank/MinibankWeb/Java Source/com/minibank/foo.java
#
#      we don't know the package structure starts at "com" because the
#      relative path is ($TargetRelDeps)
#
#    - MinibankWeb/Java Source/com/minibank/foo.java
#
#   2. We pass a destdir="intdir/classes" to Ant
#   3. Ant places the files in
#      <build dir>/intdir/classes/com/minibank/foo.java
#
#   We have to parse the "expected" classes "MinibankWeb/Java Source/com/minibank/*.java"
#   to find the class files in the build directory.
#
push(@Classes, $TargetRelDeps->getExt(qw(.java)));

#-- Use a subroutine
@Packages = &GetBuildDirPackages($destdir, @Classes);

open(FP, ">$TargetFile");
my $target_path = $Target->getDP;
#-- JAG - 05.21.04 - Case  4689 - wrong slash
#$target_path =~ s/\//\//g;
$target_path =~ s/\\/\//g;
my $etarget_path = quotemeta($target_path);

#-- make a hash of defined package locations.
#   we strip off $eTarget_path here

#-- JAG 10.18.06 - case 7509 - 'map' modifies the array in place, so use
#      a temp array
my @tmp_packages = @Packages;
my %package_hash = map { $_ =~ s/^$etarget_path//;
                         $_ =~ s/^\///;
                         $_ =~  s/^\\//;
                         $_ =~ s/\/$//;
                         $_ = "." if ($_ eq "");
                         $_ => 1 } @tmp_packages;

#-- now look for properties that match to this
foreach my $properties_file ( @properties_files )
{
 $properties_file =~ s/\\/\//g;
 #-- split the path
 my @p = split /\//, $properties_file;
 my $file = pop @p;
 while ( @p)
 {
  my $path = join "/", @p;
  if ( $package_hash{$path})
  {
   #-- copy the file
   my $local_file = $path . "/$file";
   copy ( $properties_file, $local_file);

   #-- add to .javac
   if ( $path eq "." )
   {
    print FP $file, "\n";
   }
   else
   {
    print FP $path, "/", $file, "\n";
   }
   last;
  }
  shift @p;
 }
}

foreach (@Packages)
{
 #-- JAG - 03.05.04 - strip off leading directory if the target lives in that
 #                    directory as well
 s/^$etarget_path//;
 s/^\///;
 s/^\\//;
 s/\/$//; #AG 11.12.04 Added to strip off end slash case 5277
 $_ = "." if ( $_ eq "" );
 if ($_ eq ".")
 {
  print FP "*.class\n";
 }
 else
 {
  print FP "$_$DL*.class\n";
 }
}
close FP;

&omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,"",$RC,@CompilerOut), $RC = 1 if ($RC != 0);
&omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArguments,"",$RC,@CompilerOut) if ($RC == 0);



--End:Ant Javac Task.sc
--Start:Ant War Task.sc
#########################################
#-- Set script version number
#   Last updated:
#    JAG - 02.11.05 - fixs
#    JAG - 12.08.04 - eliminate copylocal step
#    JAG - 07.13.04 - make 'build.xml' more meaningful
my $ScriptVersion = '$Header: /CVS/openmake64/tomcat/webapps/openmake.ear/openmake.war/scripts/Ant\040War\040Task.sc,v 1.15 2010/04/13 00:19:54 steve Exp $';
#-- Clean up $ScriptVersion so that it prints useful information
if ($ScriptVersion =~ /^\s*\$Header:\s*(\S+),v\s+(\S+)\s+(\S+)\s+(\S+)/ )
{
 my $path = $1;
 my $version = $2;
 my $date = $3;
 my $time = $4;

 #-- massage path
 $path =~ s/\\040/ /g;
 my @t = split /\//, $path;
 my $file = pop @t;
 my $module = $t[2];

 $ScriptVersion = "$module, v$version";
}

#########################################
#-- Load Openmake Variables from Data File
#   Uncomment Openmake objects that need to be loaded

#  @PerlData = $AllDeps->load("AllDeps", @PerlData );
@PerlData = $RelDeps->load("RelDeps", @PerlData );
#  @PerlData = $NewerDeps->load("NewerDeps", @PerlData );
@PerlData = $TargetDeps->load("TargetDeps", @PerlData );
@PerlData = $TargetRelDeps->load("TargetRelDeps", @PerlData );
#  @PerlData = $TargetSrcDeps->load("TargetSrcDeps", @PerlData );

$StepDescription = "War File Creation for " . $Target->get;
# 5637 SAB - Verify that dependencies can be found
@DepSearchFound = ();
@DepSearchMissing = ();
@DepList = unique($TargetDeps->getList());

foreach $dep (@DepList){
 unless (-f $dep) {
  push(@DepSearchMissing, "  $dep\n");
 }
}

if (@DepSearchMissing) {
  $RC = 1;
  push(@CompilerOut, "ERROR: Dependencies could not be found\n", "\n");
  push(@CompilerOut, "MISSING DEPENDENCIES:\n", @DepSearchMissing, "\n");
  push @dellist, $Target->get;

  omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArgs,$ClassPathNL,$RC,@CompilerOut) if ($RC == 0);
  omlogger("Final",$StepDescription,"ERROR:","$StepDescription failed.",$Compiler,$CompilerArgs,$ClassPathNL,$RC,@CompilerOut) if ($RC == 1);

  ExitScript $RC;
}

$Verbose = $Quiet =~ /no/i;
$LogOutputNL = "";
my $cwd = &getcwd();
my @IncludesLines = ();
my $uses_zip = 1; #-- all tasks use zipfilesets

if ($ENV{'ARCHIVA_URL'} ne "")
{
 $RC = RuntimeDependencies($ENV{'ARCHIVA_URL'},$ENV{'ARCHIVA_USERID'},$ENV{'ARCHIVA_PASSWORD'},$ENV{'ARCHIVA_REPO'},$RelDeps->getExtList(qw(.jar .zip)));

 push(@CompilerOut,"\n");

  omlogger("Final",$StepDescription,"ERROR:","$StepDescription failed.",$Compiler,$CompilerArgs,$ClassPathNL,$RC,@CompilerOut), ExitScript $RC if ($RC == 1);
}


#########################################
#-- Define global variables
#
#-- get the name from the $BuildTask variable
my $Ant_Type = "jar";
my $Task = "";
if ( $BuildTask =~ /Ant.?\s+(\w+)$/ )
{
 $Ant_Type = $1;
 $Ant_Type = lc($Ant_Type);
}

#-- the following are files that might be specified
#   as an option, where the script will have to
#   parse out and substitute a file.
my %TaskOptionFiles = (
                       "manifest" => 1,
                       "webxml"   => 1
                      );

#-- define an array of the "Results From" dependency extensions
#   that need to be parsed for sub task dependencies. Each
#   extension added to the array will be scanned.
my @sub_task_exts = qw(
		       .javac
		       .rmic
		       .jup
		      );


#-- set the name of the build.xml file. This is used
#   for debug purposes to have a more meaningful name
#   when the -ks option is used.
my $Build_XML_File = 'build_' . $Target->get;
$Build_XML_File =~ s|\W+|_|g;
$Build_XML_File .= '.xml';
push @dellist, $Build_XML_File unless $KeepScript =~ /y/i;
my $CompilerArgs .= ' -buildfile ' . $Build_XML_File;

#########################################
#-- determine the configuration
#
my $optionref = $ReleaseFlags;
$optionref = $DebugFlags if ( $CFG eq "DEBUG" );

#########################################
#-- create the option objects
#
use Openmake::BuildOption;
my $buildopt = Openmake::BuildOption->new($optionref);

#########################################
#-- determine how many Build tasks/Option Groups we have
my @build_tasks = $buildopt->getBuildTasks;
my $build_task = $build_tasks[0];

#-- find the build task that matches to the task
$build_task = $BuildTask if ( grep /$BuildTask/, @build_tasks );

#-- find the optiongroups;
my @option_groups = $buildopt->getOptionGroups($build_task);

#-- find the compiler
$Compiler = &GetAnt();

#########################################
#-- Generate Bill of Materials if Requested
#
&GenerateBillofMat($BillOfMaterialFile->get,$BillOfMaterialRpt,$Target->get) if( defined $BillOfMaterialFile );

#########################################
# Special Options section
#
#-- the following is a loop over possible keys in the task argument
#   that we might want to find in the Build Task general options
#   For example, in Ant War, we look for Manifest.mf, web.xml
#   in the list of dependencies
#
#   We define a hash %TaskOptionFiles (at top of script)
#   to tell us what to look for
#   Note that the default type (eg. war) is defined from the
#   build type
#
foreach my $wantedkey ( keys %TaskOptionFiles)
{
 my $wantedfile = $buildopt->getBuildTaskOption( $wantedkey, $build_task );
 $wantedfile =~ s|\\|\/|g;
 $wantedfile =~ s|^"||; #"
 $wantedfile =~ s|"$||; #"
 my $wantedmatch = quotemeta($wantedfile);

 my @foundfiles = ();
 #-- find files that match to the extension, then match to the
 #   full file name
 my $wantedmatchext = "\\.NOEXT";
 if ( $wantedfile =~ /(\.[^\.]*)\s*$/ )
 {
  $wantedmatchext =  $1;
 }
 @foundfiles = $TargetDeps->getExt($wantedmatchext);

 foreach my $foundfile ( @foundfiles )
 {
  $foundfile =~ s|\\|\/|g;
  if ( $foundfile =~ /$wantedmatch$/ && -e $foundfile )
  {
   #-- take the first-found
   $foundfile =~ s|\$|\$\$|g;
   $TaskOptionFiles{$wantedkey} = $foundfile;
   $LogOutputNL .= "\nFound Build Task requested $wantedkey file:\n\n $foundfile\n\n";
   last;
  }
 }
}

##########################################
#
#-- Set the IntDir

$IntDirDL = $IntDir->get . "/";

##########################################
#
#-- Determine the overall basedir

my $BaseDir = $buildopt->getBuildTaskOption( "basedir", $build_task, $OPTIONGROUP_DEFAULT_NAME );
$BaseDir =~ s/^"//; #"
$BaseDir =~ s/"$//; #"

#-- Create the BaseDir with the Intermediate Directory appended as well
my $IntBaseDir = $BaseDir;
if($IntDirDL ne './' && $IntDirDL ne '/')
{
 $IntBaseDir = $IntDirDL . $BaseDir;
}

my $Target_Path = $Target->getDP;
$Target_Path =~ s/\\/\//g;
$Target_Path =~ s/\/$//;
$Target_Path = "" if ( $Target_Path eq "." );

##########################################
#
#-- each Option Group is a separate zipfile set
#
my $i = 0;
foreach my $option_group ( @option_groups )
{
 #-- the "dir" key is a special case that we always look for.
 #   In the future, we will replace this without needing a copy local
 #
 my $wanted_dir = $buildopt->getBuildTaskOption( "dir", $build_task, $option_group) || $BaseDir;
 $wanted_dir =~ s/^"//;
 $wanted_dir =~ s/"$//;
 my $dir_list = "";
 my @wanted_dirs = ();
 my %wanted_file_hash = ();
 my @derived_deps = ();
 my @subtask_deps = ();

 #-- get the files in localres that correspond to this option group
 #-- JAG 04.14.04 - remove 1 as last option -- was returning all files (not
 #                  just files for this build)
 my ($file_ref, $opt_ref ) = $buildopt->getBuildTaskFiles( $build_task, $option_group);
 my @option_files = @{$file_ref};


 #-- remove files that are keyed in %TaskOptionFiles
 my $i = 0;
 my @t_option_files = @option_files;
 foreach my $file ( @option_files )
 {
  foreach my $val ( values %TaskOptionFiles )
  {
   #-- assume lowercase is okay here
   if ( lc $val eq (lc $file) )
   {
    splice( @t_option_files, $i, 1);
    $i--;
   }
  }
  $i++;
 }
 @option_files = @t_option_files;

 ####################################################################################################
 #-- Retrieve all the sub task deps for each matching extension from @sub_task_exts defined at top
 #   Use GetSubTaskDeps to resolve all of the derived dependencies
 #   Also return $dir_list - the list of relative dirs where the matching sup task deps are found ADG 3.15.05
 foreach $sub_task_ext (@sub_task_exts)
 {
  if (grep /$sub_task_ext$/, @option_files)
  {
   my ($rel_dir, @ret_subtask_deps) = &GetSubTaskDeps($TargetDeps,$TargetRelDeps,$wanted_dir,$sub_task_ext);
   push @subtask_deps, @ret_subtask_deps;
   #-- remove the sub task extension files from the @option_files
   @option_files = grep { $_ !~ /$sub_task_ext$/ } @option_files;
   $dir_list .= "$rel_dir," if ($rel_dir);
  }
 }

 if ($Verbose && @subtask_deps != "") #only log if subtask_deps exist
 {
  $LogOutputNL .= "\nIncluding from Sub-Task Dependencies:\n\n";
  foreach (@subtask_deps) { $LogOutputNL .= " $_\n" if $_ =~ /\w+/ ; } #Added to strip out empty strings from logging AG 081804
 }

 #-- JAG - add package classes to TargetRelDeps so that we can use TargetRelDeps later
 #         in AntFilesetOrg
 foreach ( @subtask_deps ) { $TargetRelDeps->push($_); }

 push @option_files, @subtask_deps; #join the found subtask deps with the exisiting option files
 foreach (@option_files) { $_ =~ s|\\|/|g; } #all slashes forward
 next unless ( @option_files );

 #-- we pass in the prefix option, in case we need to split on that
 #
 my $prefix =  $buildopt->getBuildTaskOption( "prefix", $build_task, $option_group );
 $prefix =~ s|\\|\/|g;
 $prefix =~ s|^"||; #"
 $prefix =~ s|"$||; #"

 #-- JAG - 02.11.05 - add intdir to list of possible dirs
 $dir_list .= "$wanted_dir,$IntDirDL,$IntBaseDir";

 $int_dir = $IntDir->getAbsolute;

 my $option_target = Openmake::FileList->new(@option_files);
 %wanted_file_hash = &AntFilesetOrg($option_target,$TargetRelDeps,$dir_list,$int_dir,$prefix,qw( .MF .mf .javac .classpath ));

 push @wanted_dirs, (keys %wanted_file_hash);

 #-- strip off the leading ${dir} off the resources if necessary
 #   this replaces module dir.
 foreach my $temp_dir ( @wanted_dirs )
 {
  if ( $temp_dir)
  {
   $temp_dir =~ s|^"||; #"
   $temp_dir =~ s|"$||; #"

   $temp_dir =~ s|^\.[\\\/]||;

   my $etemp_dir = $temp_dir;
   $etemp_dir =~ s|\\|\/|g;
   $etemp_dir =~ s|\/|\\\/|g;
  }

  #-- write build.xml lines for resources
  if (@{$wanted_file_hash{$temp_dir}} )
  {
   #-- get the options at the Option Group. Ignore the "Default case", since
   #   this will be handled at the root level
   my @options = $buildopt->getBuildTaskOptions( $build_task, $option_group);

   #---Create Custom Manifest for Jar
   # The following section allows for custom lines to be added into the MANIFEST.MF file.
   # The lines to be written in the manifest must be defined in the options for the Ant Jar task
   # The option must be entered with "MFName=" prepended to the value for Option Name and "MFValue=" prepended to the value for Parameter

   #---grep out options that include MFName and assign them to an array separate from @options array.
   @MFOptions = (grep /MFName=/, @options);
   #---if @MFOptions defined, and we have not iterated through this if statement yet,
   # parse values for MFName and MFValue from @MFOptions and assign them
   if (@MFOptions && $UsesManifestParams !~ "1")
   {
    @lines = grep (/MFName=/, @MFOptions);
    foreach $line(@lines)
    {
     @values = split(MFValue,$line);
     $mfname = shift(@values);
     $mfvalue = shift(@values);
     $mfname  =~ s|^.*\=||;
     $mfvalue =~ s|\=||;
   #---format correctly for ANT
     $mfline = "attribute name=" . "\"$mfname\"" . " value=" . "\"$mfvalue\" \/";
     push (@mflines, $mfline);
    }
   #---create @ManifestLines array separate from @IncludesLines array
   # @ManifestLines included in Write build.XML below
    push(@ManifestLines, "manifest\n");
    foreach $manifestline(@mflines)
    {
     push(@ManifestLines, "$manifestline\n");
    }
    push(@ManifestLines, "/manifest\n");
    $UsesManifestParams = "1";
   }
   #---end of Create Custom Manifest section


   #-- need to parse options for missing quotes
   foreach my $opt ( @options )
   {
    next unless $opt;

    #-- ignore the dir option as specified in the options if
    #   we've redefined it locally in this loop. Check for IntDir as
    #   well
    #
    if ( $opt =~ /(\w*dir)\s*=(.+)/ )
    {
     my $t_opt  = $1;
     my $t_end  = $2;
     $t_end =~ s/^"//;
     $t_end =~ s/"$//;
     $opt = $t_opt . "=\"$temp_dir\"";
       next;
    }

    #-- following gets rid of ill-defined options like manifest=
    if ( $opt =~ /=(")?$/ ) #"
    {
     $opt = "";
     next;
    }
    next if ( $opt =~ /="/ && $opt =~ /"$/); #-- if it's quoted, it's fine.
    $opt =~ s|=|="|; #"
    $opt .= "\"";
   }
   $options = join " ", @options;
   $options =~ s|\s+| |g;

   #-- reset the options if this is the root level. They get set
   #   below.
   if ( $option_group eq $Openmake::BuildOption::OPTIONGROUP_DEFAULT_NAME )
   {
    $options = "";
    if ( $temp_dir ne "." && $temp_dir ne $IntBaseDir)
    {
     $options = "dir=\"$temp_dir\"";
    }
   }

   #-- if $temp_dir is "." and there's a $Target_Path ...
   my $output_dir = $temp_dir;
   $output_dir = $Target_Path . "/" if ( $temp_dir eq "." );

   if ($Quiet =~ /no/i)
   {
    $LogOutputNL .= "\nAdding to $Ant_Type from Directory:";
    $LogOutputNL .= "\n$temp_dir\n\n";
    foreach (@{$wanted_file_hash{$temp_dir}}) { $LogOutputNL .= " $_\n"; }
   }
   #-- Determine if we created a separate zipfileset within the Build Option
   #   Group. If so, add "zipfileset"
   #   Otherwise, add the code under the root level (<jar>, etc)
   #
   if  ( $option_group ne $Openmake::BuildOption::OPTIONGROUP_DEFAULT_NAME
         || $output_dir ne $IntBaseDir || $uses_zip )
   {
    if ( $options !~ /dir=/ )
    {
     if ( $output_dir && ( $output_dir ne $IntBaseDir || $uses_zip ))
     {
      $options .= " dir=\"$output_dir\" ";
     }
     else
     {
      $options .= " dir=\"\${$Ant_Type}\" ";
     }
    }

    push(@IncludesLines, "zipfileset $options\n");
    push(@IncludesLines, GetAntIncludeXML( @{$wanted_file_hash{$temp_dir}} ) );
    push(@IncludesLines, "/zipfileset\n");
    $uses_zip = 1;
   }
   else
   {
    push(@IncludesLines, GetAntIncludeXML( @{$wanted_file_hash{$temp_dir}} ) );
   }
  }
 } #-- end of loop on possible separate directories in the dir= parameter
} #-- end of zipfilesets.

##################################################
# Write Build.xml
#
my $xml =<<ENDXML;
project name = "$Project" default = "$Ant_Type" basedir= "."

 !-- Set properties --
 !--   ignore the classpath that Ant is running under --
 property name = "build.sysclasspath" value = "ignore" /

 property name = "$Ant_Type" value = "." /

 !-- Start $Ant_Type section --
  target name = "$Ant_Type"
ENDXML
$xml .= "  $Ant_Type destfile = \"$E{$Target->get}\" ";

#-- add in Task Level options, parsing out the pieces that we may have
#   replaced
my @options = $buildopt->getBuildTaskOptions($build_task);
@options = (grep {$_ !~ /MFName=/} @options); #want to remove manifest specific options from the general jar options

#-- if there are no options, we may need to add in a basedir
if ( ( ! grep /basedir=/, @options ) && scalar @option_groups == 1 )
{
 my $dir = $IntBaseDir || ".";
 push @options, "basedir=\"$dir\"";
}
foreach my $opt ( @options )
{
 my ( $key, $val);
 if ( $opt =~ /(\w+)="?(.+)"?/ )
 {
  $key = $1;
  $val = $2;
  $val =~ s/"$//; #"
 }
 if ( $TaskOptionFiles{$key} )
 {
  $xml .= $key . "=\"" . $TaskOptionFiles{$key} . "\" " if ( $TaskOptionFiles{$key} != 1 );
 }
 elsif( $key eq "basedir" )
 {
  if( $val =~ /,/)
  {
   #-- ignore basedir if it's comma separated. Do not set to "." otherwise
   #   everything in the basedir gets zipped
   next;
  }
  else
  {
   #-- this is the case where all the files live under the root <jar/war/ear>
   #   task. Here we have to set basedir.
   if ( $IntBaseDir )
   {
    $xml .= "$key=\"$IntBaseDir\" " unless ( $uses_zip );
   }
   elsif ( scalar @option_groups == 1 )
   {
    $xml .= "$key=\"$val\" " unless ($uses_zip) ;
   }
  }
 }
 elsif ( $key && $val )
 {
  $xml .= "$key=\"$val\" ";
 }
}
$xml .= "\n";

$xml .=<<ENDXML2;

   @ManifestLines
   @IncludesLines

  /$Ant_Type
 /target

!-- End the project --
/project
ENDXML2

WriteAntXML($xml, $Build_XML_File);

######################################
# Execute Build
#
&omlogger("Begin",$StepDescription,"FAILED","$StepDescription succeeded.",$Compiler,$CompilerArgs,$LogOutputNL,$RC,@Output);

push(@CompilerOut,`$Compiler $CompilerArgs 2>&1`);

$RC = Check4Errors("FAILED",$?,@CompilerOut);

if ( $RC != 0 ) {
 chmod 0777, $Target->get;
 push @dellist, $Target->get;

}

&omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,"",$RC,@CompilerOut), $RC = 1 if ($RC != 0);
&omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArguments,"",$RC,@CompilerOut) if ($RC == 0);

#########################################
#-- Generate Footprint if Requested
#
#   For Java, we create a BOM report with the
#   defined name "META-INF/openmake_footprint.log"
#   This gets jarred into the archive
#
#-- Also jar in BOM, but also
#   jar in omXXXXXX.fp file with -0 option (no compression)
#   test with omident.exe
#
if (defined $FootPrintFile)
{
 my $target = $Target->get;
 my $fp_file = $FootPrintFile->get;
 my $bom_fp_file = "META-INF/openmake/openmake_footprint.log";

 my $StepDescription = "Footprint for " . $target;
 my $Compiler = "";
 my @CompilerOut = ();

 mkfulldir( $bom_fp_file);
 #-- JAG - 04.20.06 - if the BOM already exists in text form, use it
 if ( -e $BillOfMaterialRpt )
 {
  my $file = Openmake::File->new($BillOfMaterialRpt)->getDPF();
  $file = $file . '.txt' if ( -e $file . '.txt' );
  $file = $file . '.txt' if ( -e $file . '.log' );
  copy( $file, $bom_fp_file);
 }
 else
 {
  GenerateBillofMat($fp_file, $bom_fp_file, $target);
 }

 #-- everything forward for Java
 $target      =~ s|\\|\/|g;
 $fp_file     =~ s|\\|\/|g;
 $bom_fp_file =~ s|\\|\/|g;

 #-- add the bom
 my @bom_jar = `jar -uf \"$target\" \"$bom_fp_file\" 2>&1`;
 my $RC = $?;
 if ( $RC != 0 )
 {
  push @CompilerOut, @bom_jar;
 }
 push @dellist, $bom_fp_file unless $KeepScript =~ /y/i;

 #-- add the Footprint file. Copy local
 my @t = split "/", $fp_file;
 my $rel_fp_file = pop @t;
 $rel_fp_file = "META-INF/openmake/". $rel_fp_file;

 #-- format the .fp file in the format expected by omident, with $OMBOM, etc
 GenerateFootPrint( {
                      'FootPrint' => $fp_file,
                      'FPSource'  => $rel_fp_file,
                      'FPType'    => 'Java',
                      'Compiler'  => 'jar',
                      'CompilerArguments' => "-u0f \"$target\" \"$rel_fp_file\" 2>&1"
                    }
                   );

 &omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,"",$RC,@CompilerOut), $RC = 1 if ($RC != 0);
 &omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArguments,"",$RC,@CompilerOut) if ($RC == 0);

}

sub RuntimeDependencies
{
	my ($server_url,$userid,$password,$repo,@jars) = @_;
    my $tarfile = $Target->get;
	my $use_readkey = eval { require Frontier::Client; };
	$use_readkey = eval { require Frontier::RPC2; };
	$use_readkey = eval { require LWP::Simple; };

	my $coder = Frontier::RPC2->new;

 	my $server = Frontier::Client->new('url' => $server_url . "/xmlrpc" );

	my $retcode = 0;

	$server->{rq}->authorization_basic($userid, $password);

	foreach my $jar (@jars)
	{
	 next if ($jar !~ /-/);

	 my $artifactId = substr($jar,0,rindex($jar,"-"));

	 my @args = ($artifactId);
	 my $result = $server->call('SearchService.quickSearch',@args);

	 my @res = @{$result};

	 for ($j=0;$j< scalar @res;$j++)
	 {
	  my %info = %{$res[$j]};

	  next if ($info{repositoryId} !~ /$repo/i);

	  my $foundjar = $info{artifactId} . "-" . $info{version} . "." . $info{type};

	  if ($jar =~ /$foundjar/i)
	  {
	   my @args = ( $info{groupId}, $info{artifactId}, $coder->string($info{version}));
	   my $result = $server->call('SearchService.getDependencies',@args);

	   @res = @{$result};

	   for ($i=0;$i < scalar @res;$i++)
	   {
	    %info = %{$res[$i]};
	    push @deptree, $res[$i] if ($info{scope} !~ /test|provide/i);
	   }
	  }
	 }
	}

	my @SP = $VPath->getList();
	push (@SP,$IntDir->get) if ($IntDir->get ne "." && $IntDir->get ne "");

	foreach my $artifact (@deptree)
	{
	 my @args = ($artifact->{artifactId});
	 my $artver = $artifact->{version};

	 my $propErr = 0;
	 if ($artver =~ /\$\{/)
	 {
	  $artver =~ s/\$\{//g;
	  $artver =~ s/\}//g;

	  $retcode = 1, $propErr=1, push(@CompilerOut,"ERROR 210: $artver property definition is not defined\n") if ($ENV{$artver} eq "");
	  $artver = $ENV{$artver};
	 }

	  my $reldepfile = $artifact->{groupId} . "/" . $artifact->{artifactId} . "/" . $artver . "/" . $artifact->{artifactId} . "-" . $artver . "." . $artifact->{type};

	  my $foundjaronFS = 0;

	 foreach my $dir (@SP)
	 {
	  my $lookfor = $dir;
	  $lookfor = $cwd if ($dir eq "." || $dir eq "./" || $dir eq ".\\");

	  $lookfor .= "/" . $reldepfile;
	  if ($^O =~ /win32/i)
	  {
	   $lookfor =~ s/\//\\/g;
	  }
	  else
	  {
	   $lookfor =~ s/\\/\//g;
	  }

	  if (-e $lookfor)
      {
	  	my $localjar = Openmake::File->new($lookfor);

       $ReleaseFlags->{$tarfile}->{'Ant War'}->{'Web-Inf Lib'}->{$localjar->get} = 'BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5]';
	   $DebugFlags->{$tarfile}->{'Ant War'}->{'Web-Inf Lib'}->{$localjar->get} = 'BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5]';

	   $TargetDeps->push($localjar->get);
	   $TargetRelDeps->push($localjar->getFE);
	   $foundjaronFS = 1;
	   last;
      }
	 }

	 if ($foundjaronFS == 0 && $propErr == 0)
	 {
	  my $localjar = Openmake::File->new(File::Spec->catfile($cwd, $IntDir->get, $reldepfile));

	  $localjar->mkdir();

	  push(@CompilerOut,"Caching $server_url/repository/$repo/$reldepfile -> " . $localjar->get . "\n");

	  getstore( "$server_url/repository/$repo/$reldepfile", $localjar->get );

       $ReleaseFlags->{$tarfile}->{'Ant War'}->{'Web-Inf Lib'}->{$localjar->get} = 'BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5]';
	   $DebugFlags->{$tarfile}->{'Ant War'}->{'Web-Inf Lib'}->{$localjar->get} = 'BTOG(Ant War|Web-Inf Lib)[95]{ prefix=WEB-INF/lib} DT[5]';

	   $TargetDeps->push($localjar->get);
	   $TargetRelDeps->push($localjar->getFE);
	   $TargetRelDeps->push($localjar->getFE);
	 }


	 my $result = $server->call('SearchService.quickSearch',@args);

	 my @res = @{$result};

	 my $found = 0;

	 for ($j=0;$j < scalar @res;$j++)
	 {
	  my %info = %{$res[$j]};

	  next if ($info{repositoryId} !~ /$repo/i);
	  my $iver = $info{version};

	  if ($iver =~ /\$\{/)
	  {
	   $iver =~ s/\$\{//g;
	   $iver =~ s/\}//g;
	   $RC = 1, print "ERROR 210: $iver property definition is not defined\n" if ($ENV{$iver} eq "");
	   $iver = $ENV{$iver};
	  }

	  $artifact->{type} = "jar" if ($artifact->{type} eq "pom");
	  $info{type} = "jar" if ($info{type} eq "pom");

	  my $foundjar = $info{artifactId} . "-" . $iver . "." . $info{type};
	  my $jar = $artifact->{artifactId} . "-" . $artver . "." . $artifact->{type};

	  if ($jar =~ /$foundjar/i)
	  {
	   my @args = ( $info{groupId}, $info{artifactId}, $coder->string($iver));
	   my $result = $server->call('SearchService.getDependencies',@args);
	   $found = 1;
	   @res = @{$result};

	   for ($i=0;$i < scalar @res;$i++)
	   {
	    %info = %{$res[$i]};
	    push @deptree, $res[$i] if ($info{scope} !~ /test|provide/i);
	   }
	  }
	 }

	 $retcode=1, push(@CompilerOut,"ERROR 211: GroupId=" . $artifact->{groupId} . " ArtifactId=" . $artifact->{artifactId} . " Version=" . $artver . " is not found in the Archiva Repository Index.  Please add the component to the Repository and Re-index.\n") if ($found eq 0 && $propErr eq 0);
	}
  return $retcode;
}

--End:Ant War Task.sc
--Start:Set Classpath.sc
# Openmake Perl
$Compiler  = 'Openmake ClassPath Generator';
#-- Set up HTML logging variables
$ScriptName = $Script->get;
$ScriptVersion = '$Header: /CVS/openmake64/tomcat/webapps/openmake.ear/openmake.war/scripts/Set\040Classpath.sc,v 1.3 2005/06/06 16:18:06 jim Exp $';
#-- Clean up $ScriptVersion so that it prints useful information
if ($ScriptVersion =~ /^\s*\$Header:\s*(\S+),v\s+(\S+)\s+(\S+)\s+(\S+)/ )
{
 my $path = $1;
 my $version = $2;
 my $date = $3;
 my $time = $4;

 #-- massage path
 $path =~ s/\\040/ /g;
 my @t = split /\//, $path;
 my $file = pop @t;
 my $module = $t[2];

 $ScriptVersion = "$module, v$version";
}

#########################################
#-- Load Openmake Variables from Data File
#   Uncomment Openmake objects that need to be loaded

#  @PerlData = $AllDeps->load("AllDeps", @PerlData );
#  @PerlData = $RelDeps->load("RelDeps", @PerlData );
#  @PerlData = $NewerDeps->load("NewerDeps", @PerlData );
@PerlData = $TargetDeps->load("TargetDeps", @PerlData );
#  @PerlData = $TargetRelDeps->load("TargetRelDeps", @PerlData );
#  @PerlData = $TargetSrcDeps->load("TargetSrcDeps", @PerlData );

$ScriptHeader = "Beginning Classpath Generation...";
$ScriptFooter = "Finished Classpath Generation";
$ScriptDefault = "Initial";
$ScriptDefault = "Initial";
$ScriptDefaultHTML = "Initial";
$ScriptDefaultTEXT = "Initial";
$HeaderBreak = "True";

$StepDescription = "Classpath Generation for $E{$FinalTarget->get}";

$Flags = $DebugFlags   if ($CFG eq 'DEBUG');
$Flags = $ReleaseFlags if ($CFG ne 'DEBUG');

$ClassPath = Openmake::SearchPath->new;
$ClassPath->push( '.' );
$ClassPath->push( $IntDir->get ) unless $IntDir->get eq '.';
$ClassPath->push( unique($TargetDeps->getExtList(qw(.jar .zip .properties .class ))) );

# Verify that dependencies can be found
@DepSearchFound = ();
@DepSearchMissing = ();
@DepList = unique($TargetDeps->getExtList(qw(.jar .zip .properties .class )));
foreach $dep (@DepList)
{
 if (-f $dep)
 {
  push(@DepSearchFound, "  $dep\n");
 }
 else
 {
  push(@DepSearchMissing, "  $dep\n");
 }
}

#-- JAG - 03.09.04 - Case 4450. This line doubles up all jars in the classpath
#                    (see line
#$ClassPath->push( @DepList );

#-- JAG - 03.09.04 - need to see if add.dirs was passed as an option
my $optionref = $ReleaseFlags;
$optionref = $DebugFlags if ( $CFG eq "DEBUG" );

#-- create the option objects
use Openmake::BuildOption;
my $build_option = Openmake::BuildOption->new($optionref);

#-- get the options
my $options_str = $build_option->getBuildTaskOptions( $BuildTask);
$ClassPath->push( $ProjectVPath->getList, $VPath->getList )
  if ( $options_str =~ /add.dirs/ );

# Construct formatted classpath for logging
$ClassPathNL = "Classpath:\n\n " . join("\n ", $ClassPath->getList );
$ClassPathNL .= "\n";

unless ( open(CP, ">$E{$Target->get}") ) {
 $StepError = "classpath.sc: 01: Couldn't open $E{$Target->get}.";
 omlogger('Final',$StepDescription,"FAILED","ERROR: $StepError","","","",1,"");
}

print CP $ClassPath->get . "\n";
close CP;

#-- Generate Bill of Materials if Requested
GenerateBillofMat($BillOfMaterialFile->get,$BillOfMaterialRpt,$Target->get)
 if( defined $BillOfMaterialFile );

$RC = 0;
if (@DepSearchMissing) {
  $RC = 1;
  push(@CompilerOut, "ERROR: Dependencies could not be found\n", "\n");
  push(@CompilerOut, "MISSING DEPENDENCIES:\n", @DepSearchMissing, "\n");
  push(@CompilerOut, "Found Dependencies:\n", @DepSearchFound, "\n");
  push @dellist, $Target->get;
}
else {
  push(@CompilerOut,$ClassPathNL);
}

omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArgs,$ClassPathNL,$RC,@CompilerOut) if ($RC == 0);
omlogger("Final",$StepDescription,"ERROR:","$StepDescription failed.",$Compiler,$CompilerArgs,$ClassPathNL,$RC,@CompilerOut) if ($RC == 1);

ExitScript $RC;

###################################################
sub OrderClasspath {
 #-- The following works for directories and .zip
 #-- files as well as .jar files

 my @unorderedJars = @_;
 my $jar, $mJar, @theJarOrder, @orderedJars;

 #-- Open up java-order file
 #unless( open(JOR,"<" . $JOR->get ) ) {
 # $StepError = "classpath.sc: 02:  Couldn't open Java Order file, " . $JOR->get . ".\n";
 # write_text_output( 'abort' );
 # omlogger();
 #}

 #@theJarOrder = <JOR>;
 #close JOR;
 chomp @theJarOrder;

 # make an exception for '.' and the intermediate directory
 @temp = grep(/^\.$/,@unorderedJars);
 $eIntdir = $IntDir->getEscaped;
 @temp2 = grep(/^$eIntdir$/,@unorderedJars);

 $dot = shift @temp;
 @unorderedJars = grep(!/^\.$/,@unorderedJars);
 @unorderedJars = grep(!/^$eIntdir$/,@unorderedJars);

 foreach $jar ( @theJarOrder ) {
  next if $jar =~ /^\#|^\s*$/;
  $mJar = $jar;
  $mJar =~ s|(\W)|\\$1|g;

  # match from the end since the jar from the
  # unordered list is stuck to a fully qualified path
  # determined by the vpath

  push(@orderedJars, grep( /$mJar$/,@unorderedJars ) );
  @unorderedJars = grep( !/$mJar$/,@unorderedJars );
 }

 # put the intermediate output directory back
 unshift(@orderedJars, $eIntdir) if $IntDir->get ne '';

 # put the dot back
 unshift @orderedJars, $dot;

 print "end sub OrderJars\n" if $debug =~ /sub/;
 return (@orderedJars, @unorderedJars);
}



--End:Set Classpath.sc
