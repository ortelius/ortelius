/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 * Code to connect to a remote host, and to perform the client side of the
 * login (authentication) dialog.
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 */

#include "includes.h"
RCSID("$OpenBSD: sshconnect.c,v 1.171 2005/12/06 22:38:27 reyk Exp $");

#include <openssl/bn.h>

#include "ssh.h"
#include "xmalloc.h"
#include "rsa.h"
#include "buffer.h"
#include "bufaux.h" /*RHT*/
#include "packet.h"
#include "uidswap.h"
#include "compat.h"
#include "key.h"
#include "sshconnect.h"
#include "hostfile.h"
#include "log.h"
#include "readconf.h"
#include "atomicio.h"
#include "misc.h"
#include "dns.h"

char *client_version_string = NULL;
char *server_version_string = NULL;

static int matching_host_key_dns = 0;

/* import */
extern Options options;
extern char *__progname;
extern uid_t original_real_uid;
extern uid_t original_effective_uid;
extern pid_t proxy_command_pid;

#ifndef INET6_ADDRSTRLEN		/* for non IPv6 machines */
#define INET6_ADDRSTRLEN 46
#endif

static int show_other_keys(const char *, Key *);
static void warn_changed_key(Key *);

/*
 * Connect to the given ssh server using a proxy command.
 */
static int
ssh_proxy_connect(const char *host, u_short port, const char *proxy_command)
{
	char *command_string, *tmp;
	int pin[2], pout[2];
	pid_t pid;
	char strport[NI_MAXSERV];
	size_t len;

	/* Convert the port number into a string. */
	snprintf(strport, sizeof strport, "%hu", port);

	/*
	 * Build the final command string in the buffer by making the
	 * appropriate substitutions to the given proxy command.
	 *
	 * Use "exec" to avoid "sh -c" processes on some platforms
	 * (e.g. Solaris)
	 */
	len = strlen(proxy_command) + 6;
	tmp = (char *)xmalloc(len);
	strlcpy(tmp, "exec ", len);
	strlcat(tmp, proxy_command, len);
	command_string = percent_expand(tmp, "h", host,
	    "p", strport, (char *)NULL);
	xfree(tmp);

	/* Create pipes for communicating with the proxy. */
	if (pipe(pin) < 0 || pipe(pout) < 0)
		fatal("Could not create pipes to communicate with the proxy: %.100s",
		    strerror(errno));

	debug("Executing proxy command: %.500s", command_string);

	/* Fork and execute the proxy command. */
	if ((pid = fork()) == 0) {
		char *argv[10];

		/* Child.  Permanently give up superuser privileges. */
		seteuid(original_real_uid);
		setuid(original_real_uid);

		/* Redirect stdin and stdout. */
		close(pin[1]);
		if (pin[0] != 0) {
			if (dup2(pin[0], 0) < 0)
				perror("dup2 stdin");
			close(pin[0]);
		}
		close(pout[0]);
		if (dup2(pout[1], 1) < 0)
			perror("dup2 stdout");
		/* Cannot be 1 because pin allocated two descriptors. */
		close(pout[1]);

		/* Stderr is left as it is so that error messages get
		   printed on the user's terminal. */
		argv[0] = _PATH_BSHELL;
		argv[1] = "-c";
		argv[2] = command_string;
		argv[3] = NULL;

		/* Execute the proxy command.  Note that we gave up any
		   extra privileges above. */
		execv(argv[0], argv);
		perror(argv[0]);
		exit(1);
	}
	/* Parent. */
	if (pid < 0)
		fatal("fork failed: %.100s", strerror(errno));
	else
		proxy_command_pid = pid; /* save pid to clean up later */

	/* Close child side of the descriptors. */
	close(pin[0]);
	close(pout[1]);

	/* Free the command name. */
	xfree(command_string);

	/* Set the connection file descriptors. */
	packet_set_connection(pout[0], pin[1]);

	/* Indicate OK return */
	return 0;
}

/*
 * Creates a (possibly privileged) socket for use as the ssh connection.
 */
static int
ssh_create_socket(int privileged, struct addrinfo *ai)
{
	int sock, gaierr;
	struct addrinfo hints, *res;
	struct sockaddr_in *t;

	/*
	 * If we are running as root and want to connect to a privileged
	 * port, bind our own socket to a privileged port.
	 */
#ifndef WIN32
	if (privileged) {
		int p = IPPORT_RESERVED - 1;
		PRIV_START;
		sock = rresvport_af(&p, ai->ai_family);
		PRIV_END;
		if (sock < 0)
			error("rresvport: af=%d %.100s", ai->ai_family,
			    strerror(errno));
		else
			debug("Allocated local port %d.", p);
		return sock;
	}
#endif /*WIN32*/
	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (sock < 0)
		error("socket: %.100s", strerror(errno));

	/* Bind the socket to an alternative local IP address */
	if (options.bind_address == NULL)
		return sock;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = ai->ai_family;
	hints.ai_socktype = ai->ai_socktype;
	hints.ai_protocol = ai->ai_protocol;
	hints.ai_flags = AI_PASSIVE;

	gaierr = getaddrinfo(options.bind_address, "0", &hints, &res);
	if (gaierr) {
		error("getaddrinfo: %s: %s", options.bind_address,
		    gai_strerror(gaierr));
		close(sock);
		return -1;
	}

	if (bind(sock, res->ai_addr, res->ai_addrlen) < 0) {
		error("bind: %s: %s", options.bind_address, strerror(errno));
		close(sock);
		freeaddrinfo(res);
		return -1;
	}
	freeaddrinfo(res);
	return sock;
}

static int
timeout_connect(int sockfd, const struct sockaddr *serv_addr,
    socklen_t addrlen, int timeout)
{
	fd_set *fdset;
	struct timeval tv;
	socklen_t optlen;
	int fdsetsz, optval, rc, result = -1;

	if (timeout <= 0)
		return (connect(sockfd, serv_addr, addrlen));

	set_nonblock(sockfd);
	rc = connect(sockfd, serv_addr, addrlen);
	if (rc == 0) {
		unset_nonblock(sockfd);
		return (0);
	}
	if (errno != EINPROGRESS)
		return (-1);

	fdsetsz = howmany(sockfd + 1, NFDBITS) * sizeof(fd_mask);
	fdset = (fd_set *)xmalloc(fdsetsz);

	memset(fdset, 0, fdsetsz);
	FD_SET(sockfd, fdset);
	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	for (;;) {
		rc = select(sockfd + 1, NULL, fdset, NULL, &tv);
		if (rc != -1 || errno != EINTR)
			break;
	}

	switch (rc) {
	case 0:
		/* Timed out */
		errno = ETIMEDOUT;
		break;
	case -1:
		/* Select error */
		debug("select: %s", strerror(errno));
		break;
	case 1:
		/* Completed or failed */
		optval = 0;
		optlen = sizeof(optval);
		if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval,
		    &optlen) == -1) {
			debug("getsockopt: %s", strerror(errno));
			break;
		}
		if (optval != 0) {
			errno = optval;
			break;
		}
		result = 0;
		unset_nonblock(sockfd);
		break;
	default:
		/* Should not occur */
		fatal("Bogus return (%d) from select()", rc);
	}

	xfree(fdset);
	return (result);
}

/*
 * Opens a TCP/IP connection to the remote server on the given host.
 * The address of the remote host will be returned in hostaddr.
 * If port is 0, the default port will be used.  If needpriv is true,
 * a privileged port will be allocated to make the connection.
 * This requires super-user privileges if needpriv is true.
 * Connection_attempts specifies the maximum number of tries (one per
 * second).  If proxy_command is non-NULL, it specifies the command (with %h
 * and %p substituted for host and port, respectively) to use to contact
 * the daemon.
 */
int
ssh_connect(const char *host, struct sockaddr_storage * hostaddr,
    u_short port, int family, int connection_attempts,
    int needpriv, const char *proxy_command)
{
	struct sockaddr_in *t;
	int gaierr;
	int on = 1;
	int sock = -1, attempt;
	char ntop[NI_MAXHOST], strport[NI_MAXSERV];
	struct addrinfo hints, *ai, *aitop;

	debug2("ssh_connect: needpriv %d", needpriv);

	/* If a proxy command is given, connect using it. */
	if (proxy_command != NULL)
		return ssh_proxy_connect(host, port, proxy_command);

	/* No proxy command. */

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = family;
	hints.ai_socktype = SOCK_STREAM;
	snprintf(strport, sizeof strport, "%u", port);
	if ((gaierr = getaddrinfo(host, strport, &hints, &aitop)) != 0)
		fatal("%s: %.100s: %s", __progname, host,
		    gai_strerror(gaierr));
	//
	// PAG MOD 03/01/2016 - nice new year problem. getaddrinfo is a Windows Port function and
	// just sets port in aitop to 22, ignoring the passed port number
	//
	t = (struct sockaddr_in *)aitop->ai_addr;
	t->sin_port = ntohs(port);

	/*
	 * Try to connect several times.  On some machines, the first time
	 * will sometimes fail.  In general socket code appears to behave
	 * quite magically on many machines.
		 */
	for (attempt = 0; ;) {
		if (attempt > 0)
			debug("Trying again...");
		/* Loop through addresses for this host, and try each one in
		   sequence until the connection succeeds. */
		for (ai = aitop; ai; ai = ai->ai_next) {
			if (ai->ai_family != AF_INET && ai->ai_family != AF_INET6)
				continue;
			if (getnameinfo(ai->ai_addr, ai->ai_addrlen,
			    ntop, sizeof(ntop), strport, sizeof(strport),
			    NI_NUMERICHOST|NI_NUMERICSERV) != 0) {
				error("ssh_connect: getnameinfo failed");
				continue;
			}
			debug("Connecting to %.200s [%.100s] port %d.",
				host, ntop, port);

			/* Create a socket for connecting. */
			sock = ssh_create_socket(needpriv, ai);
			if (sock < 0)
				/* Any error is already output */
				continue;

			if (timeout_connect(sock, ai->ai_addr, ai->ai_addrlen,
			    options.connection_timeout) >= 0) {
				/* Successful connection. */
				memcpy(hostaddr, ai->ai_addr, ai->ai_addrlen);
				break;
			} else {
#ifdef WIN32
				debug("connect to address %s port %s: %s",
				    ntop, strport, wsa_strerror(errno));
#else
				debug("connect to address %s port %s: %s",
				    ntop, strport, strerror(errno));
#endif /*WIN32*/
				/*
				 * Close the failed socket; there appear to
				 * be some problems when reusing a socket for
				 * which connect() has already returned an
				 * error.
				 */
				close(sock);
			}
		}
		if (ai)
			break;	/* Successful connection. */

		attempt++;
		if (attempt >= connection_attempts)
			break;
		/* Sleep a moment before retrying. */
		sleep(1);
	}

	freeaddrinfo(aitop);

	/* Return failure if we didn't get a successful connection. */
	if (attempt >= connection_attempts) {
#ifdef WIN32
		error("ssh: connect to host %s port %s: %s",
		    host, strport, wsa_strerror(errno));
#else
		error("ssh: connect to host %s port %s: %s",
		    host, strport, strerror(errno));
#endif /*WIN32*/
		return (-1);
	}

	debug("Connection established.");

	/* Set SO_KEEPALIVE if requested. */
	if (options.tcp_keep_alive &&
	    setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&on,
	    sizeof(on)) < 0)
		error("setsockopt SO_KEEPALIVE: %.100s", strerror(errno));

	/* Set the connection. */
	packet_set_connection(sock, sock);

	return 0;
}

/*
 * Waits for the server identification string, and sends our own
 * identification string.
 */
static void
ssh_exchange_identification(void)
{
	char buf[256], remote_version[256];	/* must be same size! */
	int remote_major, remote_minor, mismatch;
	int connection_in = packet_get_connection_in();
	int connection_out = packet_get_connection_out();
	int minor1 = PROTOCOL_MINOR_1;
	u_int i;

	/* Read other side's version identification. */
	for (;;) {
		for (i = 0; i < sizeof(buf) - 1; i++) {
			size_t len = atomicio(read, connection_in, &buf[i], 1);

			if (len != 1 && errno == EPIPE)
				fatal("ssh_exchange_identification: Connection closed by remote host");
			else if (len != 1)
				fatal("ssh_exchange_identification: read: %.100s", strerror(errno));
			if (buf[i] == '\r') {
				buf[i] = '\n';
				buf[i + 1] = 0;
				continue;		/**XXX wait for \n */
			}
			if (buf[i] == '\n') {
				buf[i + 1] = 0;
				break;
			}
		}
		buf[sizeof(buf) - 1] = 0;
		if (strncmp(buf, "SSH-", 4) == 0)
			break;
		debug("ssh_exchange_identification: %s", buf);
	}
	server_version_string = xstrdup(buf);

	/*
	 * Check that the versions match.  In future this might accept
	 * several versions and set appropriate flags to handle them.
	 */
	if (sscanf(server_version_string, "SSH-%d.%d-%[^\n]\n",
	    &remote_major, &remote_minor, remote_version) != 3)
		fatal("Bad remote protocol version identification: '%.100s'", buf);
	debug("Remote protocol version %d.%d, remote software version %.100s",
	    remote_major, remote_minor, remote_version);

	compat_datafellows(remote_version);
	mismatch = 0;

	switch (remote_major) {
	case 1:
		if (remote_minor == 99 &&
		    (options.protocol & SSH_PROTO_2) &&
		    !(options.protocol & SSH_PROTO_1_PREFERRED)) {
			enable_compat20();
			break;
		}
		if (!(options.protocol & SSH_PROTO_1)) {
			mismatch = 1;
			break;
		}
		if (remote_minor < 3) {
			fatal("Remote machine has too old SSH software version.");
		} else if (remote_minor == 3 || remote_minor == 4) {
			/* We speak 1.3, too. */
			enable_compat13();
			minor1 = 3;
			if (options.forward_agent) {
				logit("Agent forwarding disabled for protocol 1.3");
				options.forward_agent = 0;
			}
		}
		break;
	case 2:
		if (options.protocol & SSH_PROTO_2) {
			enable_compat20();
			break;
		}
		/* FALLTHROUGH */
	default:
		mismatch = 1;
		break;
	}
	if (mismatch)
		fatal("Protocol major versions differ: %d vs. %d",
		    (options.protocol & SSH_PROTO_2) ? PROTOCOL_MAJOR_2 : PROTOCOL_MAJOR_1,
		    remote_major);
	/* Send our own protocol version identification. */
	snprintf(buf, sizeof buf, "SSH-%d.%d-%.100s\n",
	    compat20 ? PROTOCOL_MAJOR_2 : PROTOCOL_MAJOR_1,
	    compat20 ? PROTOCOL_MINOR_2 : minor1,
	    SSH_VERSION);
	if (atomicio(vwrite, connection_out, buf, strlen(buf)) != strlen(buf))
		fatal("write: %.100s", strerror(errno));
	client_version_string = xstrdup(buf);
	chop(client_version_string);
	chop(server_version_string);
	debug("Local version string %.100s", client_version_string);
}

/* defaults to 'no' */
static int
confirm(const char *prompt)
{
	const char *msg, *again = "Please type 'yes' or 'no': ";
	char *p;
	int ret = -1;

	if (options.batch_mode)
		return 0;

	for (msg = prompt;;msg = again) {
		p = read_passphrase(msg, RP_ECHO);
		if (p == NULL ||
		    (p[0] == '\0') || (p[0] == '\n') ||
		    strncasecmp(p, "no", 2) == 0)
			ret = 0;
		if (p && strncasecmp(p, "yes", 3) == 0)
			ret = 1;
		if (p)
			xfree(p);
		if (ret != -1)
			return ret;
	}
}

/* PAG TEMP - dump key */

#include "openssl/bn.h"

#define BUFFER_MAX_CHUNK        0x100000
#define BUFFER_MAX_LEN          0xa00000

#ifdef THIS_IS_NOT_DEFINED
typedef struct {
	u_char	*buf;		/* Buffer for data. */
	u_int	 alloc;		/* Number of bytes allocated for data. */
	u_int	 offset;	/* Offset of first byte containing data. */
	u_int	 end;		/* Offset of last byte containing data. */
}       Buffer;
#endif /*THIS_IS_NOT_DEFINED*/

#define PUT_32BIT(cp, value) do { \
  (cp)[0] = (value) >> 24; \
  (cp)[1] = (value) >> 16; \
  (cp)[2] = (value) >> 8; \
  (cp)[3] = (value); } while (0)

#define GET_32BIT(cp) (((u_long)(u_char)(cp)[0] << 24) | \
                       ((u_long)(u_char)(cp)[1] << 16) | \
                       ((u_long)(u_char)(cp)[2] << 8) | \
                       ((u_long)(u_char)(cp)[3]))

#define snewn(n, type) ((type *)malloc(n*sizeof(type)))
#ifdef THIS_IS_NOT_DEFINED
#define MIN(a,b) (((a)<(b))?(a):(b))


static void buffer_init(Buffer *buffer)
{
	const u_int len = 4096;

	buffer->alloc = 0;
	buffer->buf = (u_char *)xmalloc(len);
	buffer->alloc = len;
	buffer->offset = 0;
	buffer->end = 0;
}

/* Frees any memory used for the buffer. */

static void buffer_free(Buffer *buffer)
{
	if (buffer->alloc > 0) {
		memset(buffer->buf, 0, buffer->alloc);
		buffer->alloc = 0;
		xfree(buffer->buf);
	}
}

static void *buffer_append_space(Buffer *buffer, u_int len)
{
	u_int newlen;
	void *p;

	if (len > BUFFER_MAX_CHUNK)
	{
		fprintf(stderr,"buffer_append_space: len %u not supported", len);
		exit(1);
	}

	/* If the buffer is empty, start using it from the beginning. */
	if (buffer->offset == buffer->end) {
		buffer->offset = 0;
		buffer->end = 0;
	}
restart:
	/* If there is enough space to store all data, store it now. */
	if (buffer->end + len < buffer->alloc) {
		p = buffer->buf + buffer->end;
		buffer->end += len;
		return p;
	}
	/*
	 * If the buffer is quite empty, but all data is at the end, move the
	 * data to the beginning and retry.
	 */
	if (buffer->offset > MIN(buffer->alloc, BUFFER_MAX_CHUNK)) {
		memmove(buffer->buf, buffer->buf + buffer->offset,
			buffer->end - buffer->offset);
		buffer->end -= buffer->offset;
		buffer->offset = 0;
		goto restart;
	}
	/* Increase the size of the buffer and retry. */

	newlen = buffer->alloc + len + 32768;
	if (newlen > BUFFER_MAX_LEN)
	{
		fprintf(stderr,"buffer_append_space: alloc %u not supported",newlen);
		exit(1);
	}
	buffer->buf = (u_char *)xrealloc(buffer->buf, newlen);
	buffer->alloc = newlen;
	goto restart;
	/* NOTREACHED */
}

static void buffer_append(Buffer *buffer, const void *data, u_int len)
{
	void *p;
	p = buffer_append_space(buffer, len);
	memcpy(p, data, len);
}

static int buffer_get_ret(Buffer *buffer, void *buf, u_int len)
{
        if (len > buffer->end - buffer->offset) {
                fprintf(stderr,
		"buffer_get_ret: trying to get more bytes %d than in buffer %d",
                    len, buffer->end - buffer->offset);
                return (-1);
        }
        memcpy(buf, buffer->buf + buffer->offset, len);
        buffer->offset += len;
        return (0);
}

static int buffer_get_int_ret(u_int *ret, Buffer *buffer)
{
        u_char buf[4];

        if (buffer_get_ret(buffer, (char *) buf, 4) == -1)
                return (-1);
        *ret = GET_32BIT(buf);
        return (0);
}


static void buffer_put_int(Buffer *buffer, u_int value)
{
	char buf[4];

	PUT_32BIT(buf, value);
	buffer_append(buffer, buf, 4);
}

static void buffer_put_string(Buffer *buffer, const void *buf, u_int len)
{
	buffer_put_int(buffer, len);
	buffer_append(buffer, buf, len);
}


static int buffer_put_bignum2_ret(Buffer *buffer, const BIGNUM *value)
{
	u_int bytes;
	u_char *buf;
	int oi;
	u_int hasnohigh = 0;

	if (BN_is_zero(value)) {
		buffer_put_int(buffer, 0);
		return 0;
	}
	if (value->neg) {
		fprintf(stderr,"buffer_put_bignum2_ret: negative numbers not supported\n");
		return (-1);
	}
	bytes = BN_num_bytes(value) + 1; /* extra padding byte */
	if (bytes < 2) {
		fprintf(stderr,"buffer_put_bignum2_ret: BN too small\n");
		return (-1);
	}
	buf = (u_char *)xmalloc(bytes);
	buf[0] = 0x00;
	/* Get the value of in binary */
	oi = BN_bn2bin(value, buf+1);
	if (oi < 0 || (u_int)oi != bytes - 1) {
		fprintf(stderr,"buffer_put_bignum2_ret: BN_bn2bin() failed: "
		    "oi %d != bin_size %d\n", oi, bytes);
		xfree(buf);
		return (-1);
	}
	hasnohigh = (buf[1] & 0x80) ? 0 : 1;
	buffer_put_string(buffer, buf+hasnohigh, bytes-hasnohigh);
	memset(buf, 0, bytes);
	xfree(buf);
	return (0);
}

static char *DumpBufferInHex(Buffer *buffer)
{
	u_int len,stringlen;
	u_char *value;
	int k;
	int lz=1;
	char *s /*RHT unused: ,*fmtstr*/;
	int n;
	char *hex="0123456789abcdef";
	short n1,n2;

	buffer_get_int_ret(&len, buffer);
	/*RHT -: printf("len=%d\n",len);*/
	stringlen = (len*2)+10;	/* each byte is 2 chars long + "0x" + space for nulls and whatnot */
	s = (char *)xmalloc(stringlen);
	s[0]='0';
	s[1]='x';
	s[2]='\0';
	n=2;
	value = (u_char *)xmalloc(len + 1);
	buffer_get_ret(buffer,value,len);
	for (k=0;k<len;k++)
	{
		n1=(value[k] & 0xf0) >> 4;
		n2=(value[k] & 0x0f);
		if (lz)
		{
			if (n1) s[n++]=hex[n1];
			if (n2) s[n++]=hex[n2];
			if (n1 || n2) lz=0;
		}
		else
		{
			s[n++]=hex[n1];
			s[n++]=hex[n2];
		}
		s[n]='\0';
	}
	free(value);
	return s;
}

static char *dss_fmtkey(const Key *key)
{
	/*RHT unused: char *p;*/
	char *ps,*pq,*pg,*ppub,*retstr;
	int len /*RHT unused: , i, pos, nibbles*/;
	static const char hex[] = "0123456789abcdef";
	Buffer bufferp,bufferq,bufferg,bufferpub;

	if (!key->dsa->p)
	    return NULL;
	len = 8 + 4 + 1;		       /* 4 x "0x", punctuation, \0 */

	/*RHT -: Buffer bufferp,bufferq,bufferg,bufferpub;*/

	buffer_init(&bufferp);
	buffer_init(&bufferq);
	buffer_init(&bufferg);
	buffer_init(&bufferpub);

	buffer_put_bignum2_ret(&bufferp,key->dsa->p);
	buffer_put_bignum2_ret(&bufferq,key->dsa->q);
	buffer_put_bignum2_ret(&bufferg,key->dsa->g);
	buffer_put_bignum2_ret(&bufferpub,key->dsa->pub_key);

	ps=DumpBufferInHex(&bufferp);
	pq=DumpBufferInHex(&bufferq);
	pg=DumpBufferInHex(&bufferg);
	ppub=DumpBufferInHex(&bufferpub);

	retstr=(char *)malloc(strlen(ps)+strlen(pq)+strlen(pg)+strlen(ppub)+5);

	sprintf(retstr,"%s,%s,%s,%s",ps,pq,pg,ppub);
	free(ps);
	free(pq);
	free(pg);
	free(ppub);

	buffer_free(&bufferp);
	buffer_free(&bufferq);
	buffer_free(&bufferg);
	buffer_free(&bufferpub);
	return retstr;
}

static char *rsa_fmtkey(const Key *key)
{
	/* size of modulus 'n' */
	int bits;
	Buffer buffere,buffern;
	char *pe,*pn,*retstr;

	bits = BN_num_bits(key->rsa->n);

	/*RHT -: Buffer buffere,buffern;*/
	/*RHT -: char *pe,*pn,*retstr;*/

	buffer_init(&buffere);
	buffer_init(&buffern);

	buffer_put_bignum2_ret(&buffere,key->rsa->e);
	buffer_put_bignum2_ret(&buffern,key->rsa->n);

	pe=DumpBufferInHex(&buffere);
	pn=DumpBufferInHex(&buffern);

	retstr=(char *)malloc(strlen(pe)+strlen(pn)+10);

	sprintf(retstr,"%s,%s",pe,pn);
	free(pe);
	free(pn);

	buffer_free(&buffere);
	buffer_free(&buffern);
	return retstr;
}

void DumpHostKey(char *text,Key *key)
{
        unsigned int bits = 0;
        /*RHT unused: char *buf;*/

        if (key->type == KEY_RSA && key->rsa != NULL)
        {
                char *fmtkey = rsa_fmtkey(key);
		printf("rsa%s%s\n",text,fmtkey);
                free(fmtkey);

        }
        else if (key->type == KEY_DSA && key->dsa != NULL)
        {
                char *fmtkey = dss_fmtkey(key);
		printf("dsa%s%s\n",text,fmtkey);
                free(fmtkey);
        }
        else
        {
                printf("%s\n",text);
        }
}
#endif /*THIS_IS_NOT_DEFINED*/

/*
 * check whether the supplied host key is valid, return -1 if the key
 * is not valid. the user_hostfile will not be updated if 'readonly' is true.
 */
static int
check_host_key(char *host, struct sockaddr *hostaddr, Key *host_key,
    int readonly, const char *user_hostfile, const char *system_hostfile)
{
	Key *file_key;
	const char *type = key_type(host_key);
	char *ip = NULL;
	char hostline[1000], *hostp, *fp;
	HostStatus host_status;
	HostStatus ip_status;
	int r, local = 0, host_ip_differ = 0;
	int salen;
	char ntop[NI_MAXHOST];
	char msg[1024];
	int len, host_line, ip_line;
	const char *host_file = NULL, *ip_file = NULL;
	short port; /*RHT*/

	/*
	 * Force accepting of the host key for loopback/localhost. The
	 * problem is that if the home directory is NFS-mounted to multiple
	 * machines, localhost will refer to a different machine in each of
	 * them, and the user will get bogus HOST_CHANGED warnings.  This
	 * essentially disables host authentication for localhost; however,
	 * this is probably not a real problem.
	 */
	/**  hostaddr == 0! */
	switch (hostaddr->sa_family) {
	case AF_INET:
		local = (ntohl(((struct sockaddr_in *)hostaddr)->
		    sin_addr.s_addr) >> 24) == IN_LOOPBACKNET;
		port = ntohs(((struct sockaddr_in *)hostaddr)->sin_port); /*RHT*/
		salen = sizeof(struct sockaddr_in);
		break;
	case AF_INET6:
		local = IN6_IS_ADDR_LOOPBACK(
		    &(((struct sockaddr_in6 *)hostaddr)->sin6_addr));
		port = ((struct sockaddr_in6 *)hostaddr)->sin6_port; /*RHT*/
		salen = sizeof(struct sockaddr_in6);
		break;
	default:
		local = 0;
		port = 0; /*RHT*/
		salen = sizeof(struct sockaddr_storage);
		break;
	}

	/*options.check_host_ip = 1;*/ /*RHT*/
	/*options.strict_host_key_checking = 0;*/ /*RHT*/

	if (options.no_host_authentication_for_localhost == 1 && local &&
	    options.host_key_alias == NULL) {
		debug("Forcing accepting of host key for "
		    "loopback/localhost.");
		return 0;
	}

	/*
	 * We don't have the remote ip-address for connections
	 * using a proxy command
	 */
	if (options.proxy_command == NULL) {
		if (getnameinfo(hostaddr, salen, ntop, sizeof(ntop),
		    NULL, 0, NI_NUMERICHOST) != 0)
			fatal("check_host_key: getnameinfo failed");
		ip = xstrdup(ntop);
	} else {
		ip = xstrdup("<no hostip for proxy command>");
	}
	/*
	 * Turn off check_host_ip if the connection is to localhost, via proxy
	 * command or if we don't have a hostname to compare with
	 */
	if (options.check_host_ip &&
	    (local || strcmp(host, ip) == 0 || options.proxy_command != NULL))
		options.check_host_ip = 0;

	/*
	 * Allow the user to record the key under a different name. This is
	 * useful for ssh tunneling over forwarded connections or if you run
	 * multiple sshd's on different ports on the same machine.
	 */
	if (options.host_key_alias != NULL) {
		host = options.host_key_alias;
		debug("using hostkeyalias: %s", host);
	}

	/*
	 * Store the host key from the known host file in here so that we can
	 * compare it with the key for the IP address.
	 */
	file_key = key_new(host_key->type);

	/*
	 * Check if the host key is present in the user's list of known
	 * hosts or in the systemwide list.
	 */
	host_file = user_hostfile;
	host_status = check_host_in_hostfile(host_file, host, host_key,
	    file_key, &host_line);
#if 0
	if (host_status == HOST_NEW) {
		host_file = system_hostfile;
		host_status = check_host_in_hostfile(host_file, host, host_key,
		    file_key, &host_line);
	}
#endif /*0*/
	/*
	 * Also perform check for the ip address, skip the check if we are
	 * localhost or the hostname was an ip address to begin with
	 */
	if (options.check_host_ip) {
		Key *ip_key = key_new(host_key->type);

		ip_file = user_hostfile;
		ip_status = check_host_in_hostfile(ip_file, ip, host_key,
		    ip_key, &ip_line);
#if 0
		if (ip_status == HOST_NEW) {
			ip_file = system_hostfile;
			ip_status = check_host_in_hostfile(ip_file, ip,
			    host_key, ip_key, &ip_line);
		}
#endif /*0*/
		if (host_status == HOST_CHANGED &&
		    (ip_status != HOST_CHANGED || !key_equal(ip_key, file_key)))
			host_ip_differ = 1;

		key_free(ip_key);
	} else
		ip_status = host_status;

	key_free(file_key);

	// HUMUNGOUS BIG FRIG - BEGIN
//###	host_status = HOST_OK;
	// HUMUNGOUS BIG FRIG - END

	switch (host_status) {
	case HOST_OK:
		/* The host is known and the key matches. */
		debug("Host '%.200s' is known and matches the %s host key.",
		    host, type);
		debug("Found key in %s:%d", host_file, host_line);
		if (options.check_host_ip && ip_status == HOST_NEW) {
			if (readonly)
				logit("%s host key for IP address "
				    "'%.128s' not in list of known hosts.",
				    type, ip);
			else if (!add_host_to_hostfile(user_hostfile, ip,
			    host_key, options.hash_known_hosts))
				logit("Failed to add the %s host key for IP "
				    "address '%.128s' to the list of known "
				    "hosts (%.30s).", type, ip, user_hostfile);
//			else
//				logit("Warning: Permanently added the %s host "
//				    "key for IP address '%.128s' to the list "
//				    "of known hosts.", type, ip);
		}
		break;
	case HOST_NEW:
		if (readonly)
			goto fail;
		/* The host is new. */
		if (options.strict_host_key_checking == 1) {
			/*
			 * User has requested strict host key checking.  We
			 * will not add the host key automatically.  The only
			 * alternative left is to abort.
			 */
			error("No %s host key is known for %.200s and you "
			    "have requested strict checking.", type, host);
			goto fail;
		} else if (options.strict_host_key_checking == 2) {
			char msg1[1024], msg2[1024];

			if (show_other_keys(host, host_key))
				snprintf(msg1, sizeof(msg1),
				    "\nbut keys of different type are already"
				    " known for this host.");
			else
				snprintf(msg1, sizeof(msg1), ".");
			/* The default */
			fp = key_fingerprint(host_key, SSH_FP_MD5, SSH_FP_HEX);
			msg2[0] = '\0';
			if (options.verify_host_key_dns) {
				if (matching_host_key_dns)
					snprintf(msg2, sizeof(msg2),
					    "Matching host key fingerprint"
					    " found in DNS.\n");
				else
					snprintf(msg2, sizeof(msg2),
					    "No matching host key fingerprint"
					    " found in DNS.\n");
			}
#ifdef THIS_IS_NOT_DEFINED
			snprintf(msg, sizeof(msg),
			    "The authenticity of host '%.200s (%s)' can't be "
			    "established%s\n"
			    "%s key fingerprint is %s.\n%s"
			    "Are you sure you want to continue connecting "
			    "(yes/no)? ",
			    host, ip, msg1, type, fp, msg2);
#else
			snprintf(msg, sizeof(msg), "C%s %s\n", type, fp);
#endif /*THIS_IS_NOT_DEFINED*/
			xfree(fp);
			if (!confirm(msg))
				goto fail;
		}
		/*
		 * If not in strict mode, add the key automatically to the
		 * local known_hosts file.
		 */
		if (options.check_host_ip && ip_status == HOST_NEW) {
			snprintf(hostline, sizeof(hostline), "%s,%s",
			    host, ip);
			hostp = hostline;
			if (options.hash_known_hosts) {
				/* Add hash of host and IP separately */
				r = add_host_to_hostfile(user_hostfile, host,
				    host_key, options.hash_known_hosts) &&
				    add_host_to_hostfile(user_hostfile, ip,
				    host_key, options.hash_known_hosts);
			} else {
				/* Add unhashed "host,ip" */
				r = add_host_to_hostfile(user_hostfile,
				    hostline, host_key,
				    options.hash_known_hosts);
			}
		} else {
			r = add_host_to_hostfile(user_hostfile, host, host_key,
			    options.hash_known_hosts);
			hostp = host;
		}

		if (!r)
			logit("Failed to add the host to the list of known "
			    "hosts (%.500s).", user_hostfile);
		else
			logit("Warning: Permanently added '%.200s' (%s) to the "
			    "list of known hosts.", hostp, type);
		break;
	case HOST_CHANGED:
		if (options.check_host_ip && host_ip_differ) {
			char *key_msg;
			if (ip_status == HOST_NEW)
				key_msg = "is unknown";
			else if (ip_status == HOST_OK)
				key_msg = "is unchanged";
			else
				key_msg = "has a different value";
			error("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
			error("@       WARNING: POSSIBLE DNS SPOOFING DETECTED!          @");
			error("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
			error("The %s host key for %s has changed,", type, host);
			error("and the key for the according IP address %s", ip);
			error("%s. This could either mean that", key_msg);
			error("DNS SPOOFING is happening or the IP address for the host");
			error("and its host key have changed at the same time.");
			if (ip_status != HOST_NEW)
				error("Offending key for IP in %s:%d", ip_file, ip_line);
		}
		/* The host key has changed. */
		warn_changed_key(host_key);
		error("Add correct host key in %.100s to get rid of this message.",
		    user_hostfile);
		error("Offending key in %s:%d", host_file, host_line);

		/*
		 * If strict host key checking is in use, the user will have
		 * to edit the key manually and we can only abort.
		 */
		if (options.strict_host_key_checking) {
			error("%s host key for %.200s has changed and you have "
			    "requested strict checking.", type, host);
			goto fail;
		}

		/*
		 * If strict host key checking has not been requested, allow
		 * the connection but without MITM-able authentication or
		 * agent forwarding.
		 */
		if (options.password_authentication) {
			error("Password authentication is disabled to avoid "
			    "man-in-the-middle attacks.");
			options.password_authentication = 0;
		}
		if (options.kbd_interactive_authentication) {
			error("Keyboard-interactive authentication is disabled"
			    " to avoid man-in-the-middle attacks.");
			options.kbd_interactive_authentication = 0;
			options.challenge_response_authentication = 0;
		}
		if (options.challenge_response_authentication) {
			error("Challenge/response authentication is disabled"
			    " to avoid man-in-the-middle attacks.");
			options.challenge_response_authentication = 0;
		}
		if (options.forward_agent) {
			error("Agent forwarding is disabled to avoid "
			    "man-in-the-middle attacks.");
			options.forward_agent = 0;
		}
		if (options.forward_x11) {
			error("X11 forwarding is disabled to avoid "
			    "man-in-the-middle attacks.");
			options.forward_x11 = 0;
		}
		if (options.num_local_forwards > 0 ||
		    options.num_remote_forwards > 0) {
			error("Port forwarding is disabled to avoid "
			    "man-in-the-middle attacks.");
			options.num_local_forwards =
			    options.num_remote_forwards = 0;
		}
		/*
		 * XXX Should permit the user to change to use the new id.
		 * This could be done by converting the host key to an
		 * identifying sentence, tell that the host identifies itself
		 * by that sentence, and ask the user if he/she whishes to
		 * accept the authentication.
		 */
		break;
	case HOST_FOUND:
		fatal("internal error");
		break;
	}

	if (options.check_host_ip && host_status != HOST_CHANGED &&
	    ip_status == HOST_CHANGED) {
		snprintf(msg, sizeof(msg),
		    "Warning: the %s host key for '%.200s' "
		    "differs from the key for the IP address '%.128s'"
		    "\nOffending key for IP in %s:%d",
		    type, host, ip, ip_file, ip_line);
		if (host_status == HOST_OK) {
			len = strlen(msg);
			snprintf(msg + len, sizeof(msg) - len,
			    "\nMatching host key in %s:%d",
			    host_file, host_line);
		}
		if (options.strict_host_key_checking == 1) {
			logit("%s", msg);
			error("Exiting, you have requested strict checking.");
			goto fail;
		} else if (options.strict_host_key_checking == 2) {
#ifdef THIS_IS_NOT_DEFINED
			strlcat(msg, "\nAre you sure you want "
			    "to continue connecting (yes/no)? ", sizeof(msg));
#else
			fp = key_fingerprint(host_key, SSH_FP_MD5, SSH_FP_HEX);
			snprintf(msg, sizeof(msg), "D%s %s\n", type, fp);
			xfree(fp);
#endif /*THIS_IS_NOT_DEFINED*/
			if (!confirm(msg))
				goto fail;
		} else {
			logit("%s", msg);
		}
	}

	xfree(ip);
	return 0;

fail:
	xfree(ip);
	return -1;
}

/* returns 0 if key verifies or -1 if key does NOT verify */
int
verify_host_key(char *host, struct sockaddr *hostaddr, Key *host_key)
{
	struct stat st;
	int flags = 0;

	if (options.verify_host_key_dns &&
	    verify_host_key_dns(host, hostaddr, host_key, &flags) == 0) {

		if (flags & DNS_VERIFY_FOUND) {

			if (options.verify_host_key_dns == 1 &&
			    flags & DNS_VERIFY_MATCH &&
			    flags & DNS_VERIFY_SECURE)
				return 0;

			if (flags & DNS_VERIFY_MATCH) {
				matching_host_key_dns = 1;
			} else {
				warn_changed_key(host_key);
				error("Update the SSHFP RR in DNS with the new "
				    "host key to get rid of this message.");
			}
		}
	}

	/* return ok if the key can be found in an old keyfile */
	if (stat(options.system_hostfile2, &st) == 0 ||
	    stat(options.user_hostfile2, &st) == 0) {
		if (check_host_key(host, hostaddr, host_key, /*readonly*/ 1,
		    options.user_hostfile2, options.system_hostfile2) == 0)
			return 0;
	}
	return check_host_key(host, hostaddr, host_key, /*readonly*/ 0,
	    options.user_hostfile, options.system_hostfile);
}

/*
 * Starts a dialog with the server, and authenticates the current user on the
 * server.  This does not need any extra privileges.  The basic connection
 * to the server must already have been established before this is called.
 * If login fails, this function prints an error and never returns.
 * This function does not require super-user privileges.
 */
void
ssh_login(Sensitive *sensitive, const char *orighost,
    struct sockaddr *hostaddr, struct passwd *pw)
{
	char *host, *cp;
	char *server_user, *local_user;

	local_user = xstrdup(pw->pw_name);
	server_user = options.user ? options.user : local_user;

	/* Convert the user-supplied hostname into all lowercase. */
	host = xstrdup(orighost);
	for (cp = host; *cp; cp++)
		if (isupper(*cp))
			*cp = tolower(*cp);

	/* Exchange protocol version identification strings with the server. */
	ssh_exchange_identification();

	/* Put the connection into non-blocking mode. */
	packet_set_nonblocking();

	/* key exchange */
	/* authenticate user */
	if (compat20) {
		ssh_kex2(host, hostaddr);
		ssh_userauth2(local_user, server_user, host, sensitive);
	} else {
		ssh_kex(host, hostaddr);
		ssh_userauth1(local_user, server_user, host, sensitive);
	}
}

void
ssh_put_password(char *password)
{
	int size;
	char *padded;

	if (datafellows & SSH_BUG_PASSWORDPAD) {
		packet_put_cstring(password);
		return;
	}
	size = roundup(strlen(password) + 1, 32);
	padded = (char *)xmalloc(size);
	memset(padded, 0, size);
	strlcpy(padded, password, size);
	packet_put_string(padded, size);
	memset(padded, 0, size);
	xfree(padded);
}

static int
show_key_from_file(const char *file, const char *host, int keytype)
{
	Key *found;
	char *fp;
	int line, ret;

	found = key_new(keytype);
	if ((ret = lookup_key_in_hostfile_by_type(file, host,
	    keytype, found, &line))) {
		fp = key_fingerprint(found, SSH_FP_MD5, SSH_FP_HEX);
#ifdef THIS_IS_NOT_DEFINED
		logit("WARNING: %s key found for host %s\n"
		    "in %s:%d\n"
		    "%s key fingerprint %s.",
		    key_type(found), host, file, line,
		    key_type(found), fp);
#else
		logit("WARNING: %s key found for host %s\n"
		    "%s key fingerprint %s.",
		    key_type(found), host,
		    key_type(found), fp);
#endif /*THIS_IS_NOT_DEFINED*/
		xfree(fp);
	}
	key_free(found);
	return (ret);
}

/* print all known host keys for a given host, but skip keys of given type */
static int
show_other_keys(const char *host, Key *key)
{
	int type[] = { KEY_RSA1, KEY_RSA, KEY_DSA, -1};
	int i, found = 0;

	for (i = 0; type[i] != -1; i++) {
		if (type[i] == key->type)
			continue;
		if (type[i] != KEY_RSA1 &&
		    show_key_from_file(options.user_hostfile2, host, type[i])) {
			found = 1;
			continue;
		}
#if 0
		if (type[i] != KEY_RSA1 &&
		    show_key_from_file(options.system_hostfile2, host, type[i])) {
			found = 1;
			continue;
		}
#endif /*0*/
		if (show_key_from_file(options.user_hostfile, host, type[i])) {
			found = 1;
			continue;
		}
#if 0
		if (show_key_from_file(options.system_hostfile, host, type[i])) {
			found = 1;
			continue;
		}
#endif /*0*/
		debug2("no key of type %d for host %s", type[i], host);
	}
	return (found);
}

static void
warn_changed_key(Key *host_key)
{
	char *fp;
	const char *type = key_type(host_key);

	fp = key_fingerprint(host_key, SSH_FP_MD5, SSH_FP_HEX);

	error("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	error("@    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     @");
	error("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	error("IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!");
	error("Someone could be eavesdropping on you right now (man-in-the-middle attack)!");
	error("It is also possible that the %s host key has just been changed.", type);
	error("The fingerprint for the %s key sent by the remote host is\n%s.",
	    type, fp);
	error("Please contact your system administrator.");

	xfree(fp);
}

/*
 * Execute a local command
 */
int
ssh_local_cmd(const char *args)
{
	char *shell;
	pid_t pid;
	int status;

	if (!options.permit_local_command ||
	    args == NULL || !*args)
		return (1);

	if ((shell = getenv("SHELL")) == NULL)
		shell = _PATH_BSHELL;

	pid = fork();
	if (pid == 0) {
		debug3("Executing %s -c \"%s\"", shell, args);
		execl(shell, shell, "-c", args, (char *)NULL);
		error("Couldn't execute %s -c \"%s\": %s",
		    shell, args, strerror(errno));
		_exit(1);
	} else if (pid == -1)
		fatal("fork failed: %.100s", strerror(errno));
	while (waitpid(pid, &status, 0) == -1)
		if (errno != EINTR)
			fatal("Couldn't wait for child: %s", strerror(errno));

	if (!WIFEXITED(status))
		return (1);

	return (WEXITSTATUS(status));
}
