# Linux
POSTGRES_DIR=/opt/PostgreSQL/9.1
TRILOGYHOME=/home/harvest/Trilogy2.2.1
CC=gcc
CXX=g++
CXXFLAGS=-I/usr/local/include -I../dmapi -I/mnt/hgfs/philgibbs/sqlheaders -DHAVE_UNIXODBC -DDEV_VERSION -DLDAP_DEPRECATED -fPIC -fpermissive -m64 -Wno-enum-compare -Wno-write-strings
DEBUG=-g
OUTDIR=debug
#DEBUG=
#OUTDIR=release
MAKEFLAGS=POSTGRES_DIR=$(POSTGRES_DIR) TRILOGYHOME=$(TRILOGYHOME) \
	CC=$(CC) CXX=$(CXX) DEBUG=$(DEBUG)


all: $(OUTDIR) $(OUTDIR)/dm $(OUTDIR)/harvest.so $(OUTDIR)/perforce.so $(OUTDIR)/restful.so $(OUTDIR)/setodbc $(OUTDIR)/libhttp.so

$(OUTDIR):
	mkdir $(OUTDIR)

$(OUTDIR)/dmapi:
	mkdir $(OUTDIR)/dmapi

$(OUTDIR)/libdmapi.so: $(OUTDIR)/dmapi force_look
	cd $(OUTDIR)/dmapi; $(MAKE) -f ../../Makefile_dmapi

$(OUTDIR)/harvest:
	mkdir $(OUTDIR)/harvest

$(OUTDIR)/harvest.so: $(OUTDIR)/harvest force_look
	cd $(OUTDIR)/harvest; $(MAKE) -f ../../Makefile_harvest

$(OUTDIR)/perforce:
	mkdir $(OUTDIR)/perforce

$(OUTDIR)/perforce.so: $(OUTDIR)/perforce force_look
	cd $(OUTDIR)/perforce; $(MAKE) -f ../../Makefile_perforce

$(OUTDIR)/http:
	mkdir $(OUTDIR)/http

$(OUTDIR)/libhttp.so: $(OUTDIR)/http force_look
	cd $(OUTDIR)/http; $(MAKE) -f ../../Makefile_http

$(OUTDIR)/restful:
	mkdir $(OUTDIR)/restful

$(OUTDIR)/restful.so: $(OUTDIR)/restful force_look
	cd $(OUTDIR)/restful; $(MAKE) -f ../../Makefile_restful

$(OUTDIR)/dm: $(OUTDIR)/main.o $(OUTDIR)/libdmapi.so
	$(CXX) -o $@ $(OUTDIR)/main.o -L$(OUTDIR) -ldmapi -lldap -lz -lzip

$(OUTDIR)/setodbc: $(OUTDIR)/setodbc.o $(OUTDIR)/crypto.o $(OUTDIR)/dmapi/options.o $(OUTDIR)/triodbc.o
	$(CXX) -m64 -o $@ $(OUTDIR)/crypto.o $(OUTDIR)/setodbc.o $(OUTDIR)/dmapi/options.o $(OUTDIR)/triodbc.o -L$(ODBC_LIB_DIR) -L$(SSL_LIB_DIR) -L$(OUTDIR) -lssl -lcrypto -lodbc

$(OUTDIR)/crypto.o:     ../dmapi/crypto.cpp
	$(CXX) -DSETODBC $(CXXFLAGS) -m64 -o $@ -c ../dmapi/crypto.cpp

$(OUTDIR)/triodbc.o:    ../dmapi/triodbc.cpp
	$(CXX) -DSETODBC $(CXXFLAGS) -m64 -o $@ -c ../dmapi/triodbc.cpp

clean:
	rm -f $(OUTDIR)/*.o $(OUTDIR)/*.so $(OUTDIR)/dm $(OUTDIR)/dmapi/*.o \
		$(OUTDIR)/http/*.o $(OUTDIR)/perforce/*.o \
		$(OUTDIR)/restful/*.o



force_look:
	true

###

#
# Object files: Generic Rules
#

$(OUTDIR)/%.o: ../%.cpp
	$(CC) $(CXXFLAGS) $(DEBUG) -c -o $@ $<


###

$(OUTDIR)/main.o:			../main.cpp
