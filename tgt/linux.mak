APPL=DEPLOY-PLUS
STAGE=ENGINE-LINUX-64
MAKEFILEDIR=

VPATH=.;$(REF_DIR)/tgt/linux-x64;$(REF_DIR);$(REF_DIR)/dmengine/dmapi;$(REF_DIR)/dmengine/includes;$(REF_DIR)/ftplib/includes;$(REF_DIR)/psftplib;$(REF_DIR)/TrilogyClient2.2;$(REF_DIR)/TrilogyClient2.2/Linux;$(REF_DIR)/TrilogyServer2.2;/usr/local/include;/usr/local/lib;$(COMPILER)/include;$(REF_DIR)/TrilogyClient2.2/tcl8.5.7/generic;$(REF_DIR)/TrilogyClient2.2/tk8.5.7/generic;$(COMPILER)/lib64;
PROJECTVPATH=.;$(REF_DIR);


$(CFG)/dmapi/alter.o : dmengine/extended/alter/alter.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/alter.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/extended/alter;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/appverloop.o : dmengine/extended/appverloop/appverloop.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/appverloop.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/extended/appverloop;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/audit.o : dmengine/dmapi/audit.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/audit.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/charptr.o : dmengine/dmapi/charptr.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/charptr.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/cmdline.o : dmengine/dmapi/cmdline.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/cmdline.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/context.o : dmengine/dmapi/context.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/context.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/credentials.o : dmengine/dmapi/credentials.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/credentials.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/crypto.o : dmengine/dmapi/crypto.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/crypto.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/datasource.o : dmengine/dmapi/datasource.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/datasource.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/datetime.o : dmengine/dmapi/datetime.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/datetime.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/deploy.o : dmengine/extended/deploy/deploy.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/deploy.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/extended/deploy;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/dm.o : dmengine/dmapi/dm.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/dm.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/dm.tab.o : dmengine/dmapi/dm.tab.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/dm.tab.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/dm_main.o : dmengine/dmapi/dm_main.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/dm_main.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/dropzone.o : dmengine/dmapi/dropzone.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/dropzone.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/engineconfig.o : dmengine/dmapi/engineconfig.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/engineconfig.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/envp.o : dmengine/dmapi/envp.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/envp.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/exceptions.o : dmengine/dmapi/exceptions.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/exceptions.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/execute-unix.o : dmengine/dmapi/execute-unix.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/execute-unix.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/execute.o : dmengine/dmapi/execute.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/execute.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/expr.o : dmengine/dmapi/expr.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/expr.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/extended.o : dmengine/dmapi/extended.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/extended.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/filefind.o : dmengine/dmapi/filefind.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/filefind.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/filenamecache.o : dmengine/dmapi/filenamecache.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/filenamecache.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/filesystem.o : dmengine/repositories/filesystem/filesystem.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/filesystem.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/repositories/filesystem;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/function.o : dmengine/dmapi/function.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/function.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/hashtable.o : dmengine/dmapi/hashtable.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/hashtable.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/hipchat.o : dmengine/notifiers/hipchat/hipchat.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/hipchat.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/hipchat;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/https.o : dmengine/notifiers/txtlocal/https.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/https.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/txtlocal;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/lexer.o : dmengine/dmapi/lexer.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/lexer.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/license.o : dmengine/dmapi/license.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/license.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/list.o : dmengine/dmapi/list.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/list.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/logprocess.o : dmengine/dmapi/logprocess.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/logprocess.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/model.o : dmengine/dmapi/model.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/model.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/modify.o : dmengine/dmapi/modify.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/modify.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/node.o : dmengine/dmapi/node.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/node.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/notify.o : dmengine/dmapi/notify.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/notify.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/notifystmt.o : dmengine/extended/notify/notifystmt.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/notifystmt.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/extended/notify;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/notifytemplate.o : dmengine/dmapi/notifytemplate.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/notifytemplate.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/odbcdatasource.o : dmengine/datasources/odbc/odbcdatasource.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/odbcdatasource.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/datasources/odbc;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/options.o : dmengine/dmapi/options.c \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ gcc -fPIC $(BLD_OPTIONS_A) -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ gcc -fPIC $(BLD_OPTIONS_A) -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/options.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ gcc -fPIC $(BLD_OPTIONS_A) -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ gcc -fPIC $(BLD_OPTIONS_A) -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/outputstream.o : dmengine/dmapi/outputstream.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/outputstream.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/pathname.o : dmengine/dmapi/pathname.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/pathname.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/ping.o : dmengine/dmapi/ping.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/ping.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/platform.o : dmengine/dmapi/platform.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/platform.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/properties.o : dmengine/dmapi/properties.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/properties.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/regexp.o : dmengine/dmapi/regexp.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/regexp.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/regsub.o : dmengine/dmapi/regsub.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/regsub.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/remotescript.o : dmengine/extended/remotescript/remotescript.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/remotescript.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/extended/remotescript;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/repository.o : dmengine/dmapi/repository.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/repository.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/rewrap.o : dmengine/dmapi/rewrap.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/rewrap.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/scopestack.o : dmengine/dmapi/scopestack.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/scopestack.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/scriptgen.o : dmengine/dmapi/scriptgen.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/scriptgen.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/slack.o : dmengine/notifiers/slack/slack.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/slack.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/slack;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/csmtp.o : dmengine/notifiers/smtpemail/csmtp.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/csmtp.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/smtpemail;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/md5.o : dmengine/notifiers/smtpemail/md5.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/md5.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/smtpemail;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/base64.o : dmengine/notifiers/smtpemail/base64.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/base64.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/smtpemail;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/smtpemail.o : dmengine/notifiers/smtpemail/smtpemail.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/smtpemail.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/smtpemail;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/svn.o : dmengine/repositories/svn/svn.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/svn.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/repositories/svn;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/task.o : dmengine/dmapi/task.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/task.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/text.o : dmengine/modifiers/text/text.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/text.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/modifiers/text;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/thread.o : dmengine/dmapi/thread.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/thread.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/timedjob.o : dmengine/dmapi/timedjob.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/timedjob.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/tinystr.o : dmengine/dmapi/tinystr.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/tinystr.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/tinyxml.o : dmengine/dmapi/tinyxml.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/tinyxml.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/tinyxmlerror.o : dmengine/dmapi/tinyxmlerror.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/tinyxmlerror.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/tinyxmlparser.o : dmengine/dmapi/tinyxmlparser.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/tinyxmlparser.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/transfer.o : dmengine/dmapi/transfer.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/transfer.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/triodbc.o : dmengine/dmapi/triodbc.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/triodbc.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/txtlocal.o : dmengine/notifiers/txtlocal/txtlocal.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/txtlocal.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/notifiers/txtlocal;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/dmapi/xml.o : dmengine/modifiers/xml/xml.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/dmapi/xml.o
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/dmapi;dmengine/modifiers/xml;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DDEV_VERSION -DDMAPI_EXPORTS -DLDAP_DEPRECATED -fpermissive}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/http/http.o : dmengine/repositories/http/http.cpp \
		VERSIONINFO  \
		RELOPTIONS RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libhttp.so
	TARGET $(CFG)/http/http.o
	TGTFILENAME http.so.tgt
	PROJECTDIRS $(CFG)/http;dmengine/repositories/http;
	INTDIR $(CFG)\http
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/http/httppattern.o : dmengine/repositories/http/httppattern.cpp \
		VERSIONINFO  \
		RELOPTIONS RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libhttp.so
	TARGET $(CFG)/http/httppattern.o
	TGTFILENAME http.so.tgt
	PROJECTDIRS $(CFG)/http;dmengine/repositories/http;
	INTDIR $(CFG)\http
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/iisplugin/iisplugin.o : dmengine/plugins/iisplugin/iisplugin.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DIISPLUGIN_EXPORTS}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DIISPLUGIN_EXPORTS}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/libiisplugin.so
	TARGET $(CFG)/iisplugin/iisplugin.o
	TGTFILENAME iisplugin.so.tgt
	PROJECTDIRS $(CFG)/iisplugin;dmengine/plugins/iisplugin;
	INTDIR $(CFG)/iisplugin
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DIISPLUGIN_EXPORTS}  RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DIISPLUGIN_EXPORTS}  RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/restful/restful.o : dmengine/plugins/restful/restful.cpp \
		VERSIONINFO  \
		RELOPTIONS RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/librestful.so
	TARGET $(CFG)/restful/restful.o
	TGTFILENAME restful.so.tgt
	PROJECTDIRS $(CFG)/restful;dmengine/plugins/restful;
	INTDIR $(CFG)\restful
	BUILDTYPE DM Shared Object
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Compile)[17]{ -g} RUL(Compile)[25]{ g++ -fPIC $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -DDEV_VERSION -D__LINUX__ -DHAVE_VA_LIST_STRUCT -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/bin/libdmapi.so : libldap.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[37] \
	$(CFG)/dmapi/odbcdatasource.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/audit.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/hipchat.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/slack.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/charptr.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/cmdline.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/ping.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/context.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/credentials.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/crypto.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/datasource.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/datetime.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/dm.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/dm.tab.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/dm_main.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/dropzone.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/engineconfig.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/envp.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/exceptions.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/execute-unix.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/execute.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/expr.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/extended.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/filefind.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/filenamecache.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/function.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/hashtable.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/lexer.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/license.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/list.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/logprocess.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/model.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/modify.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/node.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/notify.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/notifytemplate.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/options.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/outputstream.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/pathname.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/platform.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/properties.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/regexp.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/regsub.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/repository.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/rewrap.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/scopestack.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/scriptgen.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/task.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/thread.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/timedjob.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/tinystr.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/tinyxml.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/tinyxmlerror.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/tinyxmlparser.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/transfer.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/triodbc.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/alter.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/appverloop.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/deploy.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/notifystmt.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/remotescript.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/text.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/xml.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/csmtp.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/md5.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/base64.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \				
	$(CFG)/dmapi/smtpemail.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/https.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/txtlocal.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/filesystem.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/dmapi/svn.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libdmapi.so
	TARGET $(CFG)/bin/libdmapi.so
	TGTFILENAME dmapi.so.tgt
	PROJECTDIRS $(CFG)/bin;$(CFG)/dmapi;
	INTDIR $(CFG)/dmapi
	BUILDTYPE DM Shared Object
	BUILDTASK Link
	SCRIPT GNU gcc Linker.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}

$(CFG)/bin/libhttp.so : $(CFG)/bin/libdmapi.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/http/http.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/http/httppattern.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libhttp.so
	TARGET $(CFG)/bin/libhttp.so
	TGTFILENAME http.so.tgt
	PROJECTDIRS $(CFG)/bin;$(CFG)/http;
	INTDIR $(CFG)\http
	BUILDTYPE DM Shared Object
	BUILDTASK Link
	SCRIPT GNU gcc Linker.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}

$(CFG)/bin/libiisplugin.so : $(CFG)/bin/libdmapi.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[37] \
	$(CFG)/iisplugin/iisplugin.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/libiisplugin.so
	TARGET $(CFG)/bin/libiisplugin.so
	TGTFILENAME iisplugin.so.tgt
	PROJECTDIRS $(CFG)/bin;$(CFG)/iisplugin;
	INTDIR $(CFG)/iisplugin
	BUILDTYPE DM Shared Object
	BUILDTASK Link
	SCRIPT GNU gcc Linker.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}

$(CFG)/bin/librestful.so : $(CFG)/bin/libdmapi.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[37] \
	$(CFG)/restful/restful.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/librestful.so
	TARGET $(CFG)/bin/librestful.so
	TGTFILENAME restful.so.tgt
	PROJECTDIRS $(CFG)/bin;$(CFG)/restful;
	INTDIR $(CFG)\restful
	BUILDTYPE DM Shared Object
	BUILDTASK Link
	SCRIPT GNU gcc Linker.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Link)[17]{ -g} RUL(Link)[25]{ gcc -shared -L/usr/lib $(BLD_OPTIONS_C) -lm -lrt} BTOG(Link|Build Task Options)[0]{}

$(CFG)/dm_w/main.o : dmengine/main.cpp \
		VERSIONINFO  \
		RELOPTIONS RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/bin/dm
	TARGET $(CFG)/dm_w/main.o
	TGTFILENAME dm.noext.tgt
	PROJECTDIRS $(CFG)/dm_w;dmengine;
	INTDIR $(CFG)\dm_w
	BUILDTYPE Executable
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/setodbc_w/crypto.o : dmengine/dmapi/crypto.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/setodbc
	TARGET $(CFG)/setodbc_w/crypto.o
	TGTFILENAME setodbc.noext.tgt
	PROJECTDIRS $(CFG)/setodbc_w;dmengine/dmapi;
	INTDIR $(CFG)/setodbc_w
	BUILDTYPE Executable
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/setodbc_w/options.o : dmengine/dmapi/options.c \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ gcc $(BLD_OPTIONS_A) -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ gcc $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/setodbc
	TARGET $(CFG)/setodbc_w/options.o
	TGTFILENAME setodbc.noext.tgt
	PROJECTDIRS $(CFG)/setodbc_w;dmengine/dmapi;
	INTDIR $(CFG)/setodbc_w
	BUILDTYPE Executable
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ gcc $(BLD_OPTIONS_A) -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ gcc $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/setodbc_w/setodbc.o : dmengine/setodbc.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[37]
	FINALTARGET $(CFG)/setodbc
	TARGET $(CFG)/setodbc_w/setodbc.o
	TGTFILENAME setodbc.noext.tgt
	PROJECTDIRS $(CFG)/setodbc_w;dmengine;
	INTDIR $(CFG)/setodbc_w
	BUILDTYPE Executable
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/setodbc_w/triodbc.o : dmengine/dmapi/triodbc.cpp \
		VERSIONINFO  \
		RELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/setodbc
	TARGET $(CFG)/setodbc_w/triodbc.o
	TGTFILENAME setodbc.noext.tgt
	PROJECTDIRS $(CFG)/setodbc_w;dmengine/dmapi;
	INTDIR $(CFG)/setodbc_w
	BUILDTYPE Executable
	BUILDTASK Compile
	SCRIPT GNU gcc Compiler.sc
	OSNAME Linux
	ALLRELOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A) -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}
	ALLDEBOPTIONS TBTOG(Compile|Build Task Options)[378]{ -DSETODBC}  RUL(Compile)[25]{ g++ $(BLD_OPTIONS_A)} RUL(Compile)[17]{ -g} RUL(Compile)[25]{ -DHAVE_UNIXODBC -D__LINUX__ -w -Wno-write-strings} BTOG(Compile|Build Task Options)[0]{}

$(CFG)/bin/dm : libcrypto.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[37] \
	libodbc.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[37] \
	libssl.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[37] \
	libzip.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[37] \
	libz.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[37] \
	$(CFG)/bin/libdmapi.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[37] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[37] \
	$(CFG)/dm_w/main.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/bin/dm
	TARGET $(CFG)/bin/dm
	TGTFILENAME dm.noext.tgt
	PROJECTDIRS $(CFG)/bin;$(CFG)/dm_w;
	INTDIR $(CFG)\dm_w
	BUILDTYPE Executable
	BUILDTASK Link
	SCRIPT GNU gcc Linker.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{}

$(CFG)/setodbc : libodbc.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[5] \
	libcrypto.so \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/setodbc_w/setodbc.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/setodbc_w/options.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/setodbc_w/crypto.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[5] \
	$(CFG)/setodbc_w/triodbc.o \
		VERSIONINFO  \
		RELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{} DT[5] \
		DEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{} DT[5]
	FINALTARGET $(CFG)/setodbc
	TARGET $(CFG)/setodbc
	TGTFILENAME setodbc.noext.tgt
	PROJECTDIRS $(CFG);$(CFG)/setodbc_w;
	INTDIR $(CFG)/setodbc_w
	BUILDTYPE Executable
	BUILDTASK Link
	SCRIPT GNU gcc Linker.sc
	OSNAME Linux
	ALLRELOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} BTOG(Link|Build Task Options)[0]{}
	ALLDEBOPTIONS RUL(Link)[25]{ g++ $(BLD_OPTIONS_C)} RUL(Link)[17]{ -g} BTOG(Link|Build Task Options)[0]{}

all : $(CFG)/bin/libdmapi.so \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	$(CFG)/bin/libhttp.so \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	$(CFG)/bin/libiisplugin.so \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	$(CFG)/bin/librestful.so \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	$(CFG)/bin/dm \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0] \
	$(CFG)/setodbc \
		VERSIONINFO  \
		RELOPTIONS  DT[0] \
		DEBOPTIONS  DT[0]

--Start:GNU gcc Compiler.sc
$ScriptVersion = '$Header: /CVS/openmake64/tomcat/webapps/openmake.ear/openmake.war/scripts/GNU\040gcc\040Compiler.sc,v 1.3 2005/06/06 16:18:05 jim Exp $';
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
#  @PerlData = $TargetDeps->load("TargetDeps", @PerlData );
#  @PerlData = $TargetRelDeps->load("TargetRelDeps", @PerlData );
@PerlData = $TargetSrcDeps->load("TargetSrcDeps", @PerlData );

####### Setting Compiler Choice and Search Behavior #######  

@CompilersPassedInFlags = ("gcc", "g++");
$DefaultCompiler  = "gcc";

($Compiler,$Flags) = get_compiler($DebugFlags,$ReleaseFlags,$DefaultCompiler,@CompilersPassedInFlags);

#-- JAG - 03.03.04 - case 4354 - for certain compilers, can force the "-I-" option
my $ForceIOption = ""; #-- both aCC and cc should support -I-
my $ForceIMsg = '';

#-- print warning
$ForceIMsg = "Using $ForceIOption to override 'include \" \"' headers in local source directories\n\n" 
  if ( $Quiet =~ /no/i );

#-- add this to $IncludeNL so that it's printed in the logging
$IncludeNL = "$ForceIOption\n" . $IncludeNL if ( $ForceIOption);

$TargetFile = $Target->get;
@SourceList = $TargetSrcDeps->getExtQuoted(qw(.C .CPP .CXX .CC));

$Source = "";

foreach $line (@SourceList)
{
 $Source .= $line . " ";
}

if ($Target->getExt() eq ".cl")
{
 $newdir = $Target->getDP();
 $newdir = "." if ($newdir eq "");

 $olddir = getcwd();
 chdir($newdir);
 $CompilerArguments = "$Flags $Defines -c $Source";

 $StepDescription   = "Performing GNU C/C++ Compile for $TargetFile";
 #-- Due to formatting in 'omlogger', add $ForceIMsg in front of $Compiler
 omlogger("Begin",$StepDescription,"ERROR:","$StepDescription succeeded.", $ForceIMsg . $Compiler,$CompilerArguments,$IncludeNL,$RC,@CompilerOut);

 @CompilerOut = `$Compiler $ForceIOption $Include $CompilerArguments 2>&1`;
 $RC = $?;
 chdir($olddir);
 if ($RC == 0)
 {
  open(FP, ">" . $Target->get());
  
  foreach $src ($TargetSrcDeps->getList())
  {
   $f = Openmake::File->new($src);
   print FP $newdir . "/" . $f->getF() . ".o\n";
  }
  close(FP);
 }
}
else
{
 $CompilerArguments = "$Flags $Defines -c $Source -o $TargetFile";

 $StepDescription   = "Performing GNU C/C++ Compile for $TargetFile";
 #-- Due to formatting in 'omlogger', add $ForceIMsg in front of $Compiler
 omlogger("Begin",$StepDescription,"ERROR:","$StepDescription succeeded.", $ForceIMsg . $Compiler,$CompilerArguments,$IncludeNL,$RC,@CompilerOut);

 @CompilerOut = `$Compiler $ForceIOption $Include $CompilerArguments 2>&1`;
 $RC = $?;
}


omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,$IncludeNL,$RC,@CompilerOut), $RC = 1 if ($RC != 0);
omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArguments,$IncludeNL,$RC,@CompilerOut) if ($RC == 0);



--End:GNU gcc Compiler.sc
--Start:GNU gcc Linker.sc
$ScriptVersion = '$Header: /CVS/openmake64/tomcat/webapps/openmake.ear/openmake.war/scripts/GNU\040gcc\040Linker.sc,v 1.3 2005/06/06 16:18:05 jim Exp $';
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

####### Setting Compiler Choice and Search Behavior #######  

@CompilersPassedInFlags = ("gcc", "g++", "ld", "c++");
$DefaultCompiler  = "gcc";

( $Compiler, $Flags ) =
  &get_compiler( $DebugFlags, $ReleaseFlags, $DefaultCompiler,
 @CompilersPassedInFlags );

$TargetFile = $Target->get;

$OracleHome = $ENV{"ORACLE_HOME"};
$OracleHome = "/u/oracle/product/8.0.6" if ( $OracleHome eq "" );

@found = grep ( /^\d+\./, split ( /\//, $OracleHome ) );

$OracleVersion = pop ( @found );

&GenerateBillofMat( $BillOfMaterialFile->get, $BillOfMaterialRpt, $TargetFile ) if ( defined $BillOfMaterialFile );

if ( defined $FootPrintFile )
{
 ( $tmpfhs, $FPSource ) = tempfile( 'omXXXXX', SUFFIX => '.c', UNLINK => 0 );
 close $tmpfhs;

 $FPObject = $FPSource;
 $FPObject =~ s/\.c/$\.o/;
 $FPObjKeep = "-bkeepfile:$FPObject" if ($^O !~ /linux/i);

 #push(@DeleteFileList,$FPSource); # don't push source file in yet.
 push ( @DeleteFileList, $FPObject );
 
 $CompilerArguments = "-c -o $FPObject $FPSource";
 $CompilerFound     = "cc";
 $CompilerFound     = "gcc", $FPObjKeep = "-Xlinker -bkeepfile:$FPObject" if ( $BuildType eq "GNU Executable" );
 $CompilerFound = "insure", $CompilerArguments = "-g " . $CompilerArguments, $FPObjKeep = "-bkeepfile:$FPObject"  if ( $BuildType eq "Executable Insure" );

 #-- JAG - Case 3753 - some fixes for gcc 3.2 and linux
 if ( $^O =~ /linux/i )
 {
  $CompilerFound = "gcc" if ( $BuildType eq "Executable" );
 }

 #-- get the version of gcc
 if ( $CompilerFound eq "gcc" )
 {
  my $gccout = `gcc -dumpversion`;
  $FPObjKeep = "" if ( $gccout =~ /^3/ );
 }

 &GenerateFootPrint( $FootPrintFile->get, $TargetFile, $FPSource, $FPObject, $CompilerFound, $CompilerArguments );
} #-- end if ( defined $FootPrintFile...

$FETarget = Openmake::File->new($TargetFile);
$FETargetFile = $FETarget->getFE;

$Objs = "";
$SOLibs = "";
$ALibs = "";
$IncorrectFMTLibs = "";
$CorrectFMTLibs = "";
@CorrectFMTLibList = ();
$NoExtLibs = "";

@RawObjectList = $TargetDeps->getExt(qw(.O .OBJ .SO .NOEXT .A .SL));
my @cl = $TargetDeps->getExt(qw(.CL));

if (scalar @cl > 0)
{
 my $clfile = shift @cl;
 open(FP,"< $clfile");
 my @lines = <FP>;
 close(FP);

 my $line = "";
 foreach $line (@lines)
 {
  $line =~ s/\n//g;
  $Objs .= ' "' . $line . '"';
 }
}

$AORFile = $TargetDeps->getExt(qw(.AOR));

foreach $RawObject (@RawObjectList)
{
 next if ($RawObject =~ /$FETargetFile$/);

 #-- shorten object path to relative if in fact it is relative
 $RawObjectObject = Openmake::File->new($RawObject);
 $Object = $RawObjectObject->getRelative();
  #-- JAG - 10.13.04 - case 4499 - UNIX spaces. Quote objects
 
 if ($Object =~ m/lib([^\/\.]+)\.a$/)
 {
  $Object = " -l$1";
  push(@CorrectFMTLibList, $Object);
  next;
 }

 if ($Object =~ m/lib([^\/\.]+)\.sl$/)
 {
  $Object = " -l$1";
  push(@CorrectFMTLibList, $Object);
  next;
 }

 if( $Object =~ m/lib([^\/\.]+)\.so$/) 
 {

  $CorrectFMTSOLibs .= " -l$1";
  next;
 }

 if ($Object =~ /\.a$/)
 {
  $IncorrectFMTLibs .= " \"$Object\"";
  next;
 } 

 if( $Object =~ /\.so$/) 
 {
  $SOLibs .= " \"$Object\"";
  next;
 }
 if ( $Object =~ /\.o$/ )
 {
  $Objs .= " \"$Object\"";
  next;
 }
 if ( $Object =~ /\.obj$/ )
 {
  $Objs .= " \"$Object\"";
  next;
 }
 $NoExtLibs .= " \"$Object\"";
}

$Objs             =~ s/^ //;
$SOLibs           =~ s/^ //;
$ALibs            =~ s/^ //;
$IncorrectFMTLibs =~ s/^ //;
$NoExtLibs        =~ s/^ //;

@CorrectFMTLibList = &OrderLibs( $AORFile, @CorrectFMTLibList ) if ( $AORFile ne "" && -e $AORFile );
$CorrectFMTLibs = join ( " ", @CorrectFMTLibList );

$CompilerArguments = "$Flags $Defines $PerlLibs -o $TargetFile $Objs $FPObject $NoExtLibs $SOLibs $CorrectFMTSOLibs $CorrectFMTLibs $IncorrectFMTLibs "; 

$ENV{LD_LIBRARY_PATH} = $VPath->getString( '', ':' ) . $ENV{LD_LIBRARY_PATH};

#############
# Link It
#############

unlink $TargetFile if ( -e $TargetFile );

$CompilerArguments = $FPObjKeep . " " . $CompilerArguments;

#-- If there is nothing that needs searching through the -L's
#   then don't put them there (need to shorten command line)
if ( @CorrectFMTLibList == () and $CorrectFMTSOLibs eq '' ) {
 $LibPathNL = '';
 $LibPath = '';
}

$StepDescription = "Performing Link for $TargetFile";
&omlogger(  "Begin", $StepDescription, "ERROR:", "$StepDescription succeeded.", $Compiler,  $CompilerArguments, $LibPathNL, $RC, @CompilerOut );

@CompilerOut = `$Compiler $LibPath $CompilerArguments 2>&1`;

$RC = $?;

omlogger("Final",$StepDescription,"ERROR:","ERROR: $StepDescription failed!",$Compiler,$CompilerArguments,$LibPathNL,$RC,@CompilerOut), $RC = 1 if ($RC != 0);
omlogger("Final",$StepDescription,"ERROR:","$StepDescription succeeded.",$Compiler,$CompilerArguments,$LibPathNL,$RC,@CompilerOut) if ($RC == 0);
 


--End:GNU gcc Linker.sc
