#CC = CC
CC=gcc
CPP=g++
ARCH=Linux

#CFLAGS = -D__sun__ -D__SUNOS__ -DHAVE_GETOPT_OPTRESET -g -I.. -I../RTOEXECP -features=no%conststrings -D__func__="\"function\""

CFLAGS = $(DEBUGFLAG) -I.. -D__LINUX__
#-DDEBUG_PK -DPACKET_DEBUG

CPPFLAGS = $(DEBUGFLAG) -I.. -D__LINUX__ -DUSE_TRIODBC -DRHEL_AS3

ULDFLAGS =
DEBUGFLAG=-g

all: rtoexecp2

release:
	make DEBUGFLAG=-O2 ULDFLAGS=-O2

#		config.o ssh.o readconf.o clientloop.o sshtty.o \
#		sshconnect.o sshconnect1.o sshconnect2.o tricmdline.o \
#		options.o triipc.o trilib.o logging.o

#		config.o ssh.o readconf.o clientloop.o sshtty.o \
#		sshconnect.o sshconnect1.o sshconnect2.o tricmdline.o \
#		options.o triipc.o trilib.o logging.o libcrypto.a libssl.a \

rtoexecp2:	acss.o authfd.o authfile.o base64.o bufaux.o buffer.o \
		canohost.o channels.o cipher.o cipher-acss.o cipher-aes.o \
		cipher-bf1.o cipher-ctr.o cipher-3des1.o cleanup.o \
		compat.o compress.o crc32.o deattack.o fatal.o hostfile.o \
		log.o match.o moduli.o nchan.o packet.o \
		readpass.o rsa.o ttymodes.o xmalloc.o \
		atomicio.o key.o dispatch.o kex.o mac.o uidswap.o uuencode.o \
		misc.o monitor_fdpass.o rijndael.o ssh-dss.o ssh-rsa.o dh.o \
		kexdh.o kexgex.o kexdhc.o kexgexc.o scard.o msg.o \
		progressmeter.o dns.o entropy.o scard-opensc.o gss-genr.o \
		ssh.o readconf.o clientloop.o sshtty.o \
		sshconnect.o sshconnect1.o sshconnect2.o \
		ssh.o readconf.o clientloop.o sshtty.o \
		sshconnect.o sshconnect1.o sshconnect2.o uxstore.o \
		\
		vis.o bsd-asprintf.o bsd-getpeereid.o getrrsetbyname.o \
		mktemp.o bsd-misc.o daemon.o strsep.o strtonum.o \
		bsd-arc4random.o readpassphrase.o openbsd-compat/port-linux.o \
		../../RTO_Server/$(ARCH)/release/librto.a
	$(CPP) $(ULDFLAGS) -o $@ acss.o authfd.o authfile.o base64.o bufaux.o buffer.o \
		canohost.o channels.o cipher.o cipher-acss.o cipher-aes.o \
		cipher-bf1.o cipher-ctr.o cipher-3des1.o cleanup.o \
		compat.o compress.o crc32.o deattack.o fatal.o hostfile.o \
		log.o match.o moduli.o nchan.o packet.o \
		readpass.o rsa.o ttymodes.o xmalloc.o \
		atomicio.o key.o dispatch.o kex.o mac.o uidswap.o uuencode.o \
		misc.o monitor_fdpass.o rijndael.o ssh-dss.o ssh-rsa.o dh.o \
		kexdh.o kexgex.o kexdhc.o kexgexc.o scard.o msg.o \
		progressmeter.o dns.o entropy.o scard-opensc.o gss-genr.o \
		ssh.o readconf.o clientloop.o sshtty.o \
		sshconnect.o sshconnect1.o sshconnect2.o uxstore.o \
		../../RTO_Server/$(ARCH)/release/librto.a libcrypto.a libssl.a \
		vis.o bsd-asprintf.o bsd-getpeereid.o getrrsetbyname.o \
		mktemp.o bsd-misc.o daemon.o strsep.o strtonum.o \
		bsd-arc4random.o readpassphrase.o openbsd-compat/port-linux.o \
		-lnsl -ldl -lz -lresolv 

#		-lsocket -lnsl -ldl -lz -lresolv 


clean:
	rm -f acss.o authfd.o authfile.o base64.o bufaux.o buffer.o \
		canohost.o channels.o cipher.o cipher-acss.o cipher-aes.o \
		cipher-bf1.o cipher-ctr.o cipher-3des1.o cleanup.o \
		compat.o compress.o crc32.o deattack.o fatal.o hostfile.o \
		log.o match.o moduli.o nchan.o packet.o \
		readpass.o rsa.o ttymodes.o xmalloc.o \
		atomicio.o key.o dispatch.o kex.o mac.o uidswap.o uuencode.o \
		misc.o monitor_fdpass.o rijndael.o ssh-dss.o ssh-rsa.o dh.o \
		kexdh.o kexgex.o kexdhc.o kexgexc.o scard.o msg.o \
		progressmeter.o dns.o entropy.o scard-opensc.o gss-genr.o \
		ssh.o readconf.o clientloop.o sshtty.o \
	 	sshconnect.o sshconnect1.o sshconnect2.o uxstore.o \
		bsd-arc4random.o bsd-misc.o strsep.o bsd-asprintf.o \
	 	daemon.o mktemp.o strtonum.o bsd-getpeereid.o \
		getrrsetbyname.o readpassphrase.o vis.o openbsd-compat/port-linux.o \
	       	rtoexecp2

vis.o:	../vis.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../vis.c

bsd-asprintf.o:	../bsd-asprintf.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../bsd-asprintf.c

bsd-getpeereid.o:	../bsd-getpeereid.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../bsd-getpeereid.c

getrrsetbyname.o:	../getrrsetbyname.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../getrrsetbyname.c

mktemp.o:	../mktemp.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../mktemp.c

bsd-misc.o:	../bsd-misc.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../bsd-misc.c

daemon.o:	../daemon.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../daemon.c

strsep.o:	../strsep.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../strsep.c

strtonum.o:	../strtonum.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../strtonum.c

bsd-arc4random.o:	../bsd-arc4random.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../bsd-arc4random.c

readpassphrase.o:	../readpassphrase.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../readpassphrase.c


acss.o: ../acss.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../acss.c

authfd.o: ../authfd.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../authfd.c

authfile.o: ../authfile.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../authfile.c

base64.o: ../base64.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../base64.c

bufaux.o: ../bufaux.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../bufaux.c

buffer.o: ../buffer.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../buffer.c

canohost.o: ../canohost.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../canohost.c

channels.o: ../channels.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../channels.c

cipher.o: ../cipher.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../cipher.c

cipher-acss.o: ../cipher-acss.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../cipher-acss.c

cipher-aes.o: ../cipher-aes.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../cipher-aes.c

cipher-bf1.o: ../cipher-bf1.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../cipher-bf1.c

cipher-ctr.o: ../cipher-ctr.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../cipher-ctr.c

cipher-3des1.o: ../cipher-3des1.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../cipher-3des1.c

cleanup.o: ../cleanup.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../cleanup.c

compat.o: ../compat.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../compat.c

compress.o: ../compress.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../compress.c

#config.o: ../config.cpp
#	$(CPP) $(COMPAT) $(XFLAGS) $(CPPFLAGS) -c ../config.cpp

crc32.o: ../crc32.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../crc32.c

deattack.o: ../deattack.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../deattack.c

fatal.o: ../fatal.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../fatal.c

hostfile.o: ../hostfile.cpp
	$(CPP) $(COMPAT) $(XFLAGS) $(CPPFLAGS) -c ../hostfile.cpp

log.o: ../log.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../log.c

match.o: ../match.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../match.c

moduli.o: ../moduli.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../moduli.c

nchan.o: ../nchan.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../nchan.c

packet.o: ../packet.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../packet.c

readpass.o: ../readpass.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../readpass.c

rsa.o: ../rsa.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../rsa.c

ttymodes.o: ../ttymodes.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../ttymodes.c

xmalloc.o: ../xmalloc.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../xmalloc.c

atomicio.o: ../atomicio.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../atomicio.c

key.o: ../key.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../key.c

dispatch.o: ../dispatch.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../dispatch.c

kex.o: ../kex.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../kex.c

mac.o: ../mac.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../mac.c

uidswap.o: ../uidswap.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../uidswap.c

uuencode.o: ../uuencode.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../uuencode.c

misc.o: ../misc.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../misc.c

monitor_fdpass.o: ../monitor_fdpass.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../monitor_fdpass.c

rijndael.o: ../rijndael.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../rijndael.c

ssh-dss.o: ../ssh-dss.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../ssh-dss.c

ssh-rsa.o: ../ssh-rsa.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../ssh-rsa.c

dh.o: ../dh.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../dh.c

kexdh.o: ../kexdh.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../kexdh.c

kexgex.o: ../kexgex.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../kexgex.c

kexdhc.o: ../kexdhc.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../kexdhc.c

kexgexc.o: ../kexgexc.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../kexgexc.c

scard.o: ../scard.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../scard.c

msg.o: ../msg.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../msg.c

progressmeter.o: ../progressmeter.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../progressmeter.c

dns.o: ../dns.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../dns.c

entropy.o: ../entropy.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../entropy.c

scard-opensc.o: ../scard-opensc.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../scard-opensc.c

gss-genr.o: ../gss-genr.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../gss-genr.c

ssh.o: ../ssh.cpp
	$(CPP) $(COMPAT) $(XFLAGS) $(CPPFLAGS) -c ../ssh.cpp

readconf.o: ../readconf.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../readconf.c

clientloop.o: ../clientloop.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../clientloop.c

sshtty.o: ../sshtty.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../sshtty.c

sshconnect.o: ../sshconnect.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../sshconnect.c

sshconnect1.o: ../sshconnect1.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../sshconnect1.c

sshconnect2.o: ../sshconnect2.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../sshconnect2.c

openbsd-compat/port-linux.o: ../openbsd-compat/port-linux.c
	$(CC) $(COMPAT) $(XFLAGS) $(CFLAGS) -c ../openbsd-compat/port-linux.c -o openbsd-compat/port-linux.o

uxstore.o: ../uxstore.cpp
	$(CPP) $(COMPAT) $(XFLAGS) $(CPPFLAGS) -c ../uxstore.cpp
