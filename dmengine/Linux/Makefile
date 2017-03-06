# Linux
POSTGRES_DIR=/opt/PostgreSQL/9.1
TRILOGYHOME=/home/harvest/Trilogy2.2.1
CC=gcc
CXX=g++
#CXXFLAGS=-fexceptions
#CXXFLAGS=-I$(POSTGRES_DIR)/include -I/usr/local/include -DHAVE_UNIXODBC -DDEV_VERSION
CXXFLAGS=-I/usr/local/include -DHAVE_UNIXODBC -DDEV_VERSION -DLDAP_DEPRECATED -fPIC -fpermissive -m64
DEBUG=-g
OUTDIR=debug
#DEBUG=
#OUTDIR=release
MAKEFLAGS=POSTGRES_DIR=$(POSTGRES_DIR) TRILOGYHOME=$(TRILOGYHOME) \
	CC=$(CC) CXX=$(CXX) DEBUG=$(DEBUG)


all: $(OUTDIR) $(OUTDIR)/dm $(OUTDIR)/harvest.so $(OUTDIR)/perforce.so $(OUTDIR)/restful.so

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

$(OUTDIR)/restful:
	mkdir $(OUTDIR)/restful

$(OUTDIR)/restful.so: $(OUTDIR)/restful force_look
	cd $(OUTDIR)/restful; $(MAKE) -f ../../Makefile_restful

$(OUTDIR)/dm: $(OUTDIR)/main.o $(OUTDIR)/libdmapi.so
	$(CXX) -o $@ $(OUTDIR)/main.o -L$(OUTDIR) -ldmapi -lldap
	#$(CXX) -o $@ $(OUTDIR)/main.o -Wl,--rpath-link=$(POSTGRES_DIR)/lib -L$(OUTDIR) -ldmapi 
	#	 ${TRILOGYHOME}/etc/trilogyapi.so -L${TRILOGYHOME}/etc -ltrilogy -ltcl8.5 -ltk8.5

clean:
	rm -f $(OUTDIR)/*.o $(OUTDIR)/dm $(OUTDIR)/libdmapi.so \
		$(OUTDIR)/harvest.so $(OUTDIR)/dmapi/*.o $(OUTDIR)/harvest/*.o

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
