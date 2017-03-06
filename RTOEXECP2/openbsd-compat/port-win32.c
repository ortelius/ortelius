/**
 * Win32 specific stuff - mainly functions and structures which are missing
 * under Windows.
 */

#ifdef WIN32

#include "includes.h"
#include "xmalloc.h"
#include "log.h"
#include <lmaccess.h>
#include <lmapibuf.h>
#include <lmerr.h>

/**
 * Substitute for getaddrinfo().  This implementation uses gethostbyname() and
 * only works with IPV4 addresses.  It only attempts to return the first
 * address, though it could be modified to add addition addresses to the
 * integral linked-list of the addrinfo structure.
 */
int /*WSAAPI*/ getaddrinfo(
  const char* nodename,
  const char* servname,
  const struct addrinfo* hints,
  struct addrinfo** res
  )
{
    char buffer[256];
    struct sockaddr_in *addr;
    struct hostent *host;

    debug2( "getaddrinfo( \"%s\", \"%s\", 0x%p, 0x%p )", nodename, servname, hints, res );

    if( (host = gethostbyname( nodename )) == NULL )
    {
        error( "gethostbyname failed to resolve \"%s\": %s", nodename, wsa_strerror(errno) );
        return -1;
    }

    if( (host->h_addrtype != AF_INET) || (host->h_length != 4)
        || !host->h_addr_list || !*host->h_addr_list )
    {
        error( "gethostbyname failed to resolve \"%s\": address is not an IPV4 address" );
        return -1;
    }

    *res = xmalloc( sizeof(struct addrinfo) );
    (*res)->ai_flags = 0;
    (*res)->ai_family = host->h_addrtype;
    (*res)->ai_socktype = SOCK_STREAM;
    (*res)->ai_protocol = IPPROTO_TCP;
    (*res)->ai_canonname = xstrdup( nodename );
    addr = xmalloc( sizeof(struct sockaddr_in) );
    addr->sin_family = AF_INET;
    addr->sin_port = htons(22);
    addr->sin_addr.S_un.S_un_b.s_b1 = (*host->h_addr_list)[0];
    addr->sin_addr.S_un.S_un_b.s_b2 = (*host->h_addr_list)[1];
    addr->sin_addr.S_un.S_un_b.s_b3 = (*host->h_addr_list)[2];
    addr->sin_addr.S_un.S_un_b.s_b4 = (*host->h_addr_list)[3];
    memset( addr->sin_zero,0,sizeof(addr->sin_zero) );
    (*res)->ai_addr = (struct sockaddr*) addr;
    (*res)->ai_addrlen = sizeof(struct sockaddr_in);
    (*res)->ai_next = NULL;

    debug2( "getaddrinfo returning %s", inet_ntop( (*res)->ai_family, (*res)->ai_addr->sa_data, buffer, sizeof(buffer) ) );

    return 0;
}


/**
 * flags:
 * NI_NOFQDN      If set, return only the hostname part of the FQDN for local
 *                hosts.  Not on WIN32.
 * NI_NUMERICHOST If set, then the numeric form of the hostname is returned.
 *                (When not set, this will still happen in case the node's name
 *                cannot be looked up.)
 * NI_NAMEREQD    If set, then a error is returned if the hostname cannot be
 *                looked up.
 * NI_NUMERICSERV If set, then the service address is returned in numeric form,
 *                for example by its port number.
 * NI_DGRAM       If set, then the service is datagram (UDP) based rather than
 *                stream (TCP) based. This is required for the few ports
 *                (512-514) that have different services for UDP and TCP.  Not
 *                on WIN32.
 */
int /*WSAAPI*/ getnameinfo(
  const struct sockaddr FAR* sa,
  socklen_t salen,
  char FAR* hostname,
  DWORD hostlen,
  char FAR* servname,
  DWORD servlen,
  int flags
  )
{
    struct hostent *host;

    debug2( "getnameinfo( 0x%p, %d, 0x%p, %d, 0x%p, %d, %d )",
            sa, salen, hostname, hostlen, servname, servlen, flags );

    debug3( "flags: %s%s%s",
            (flags & NI_NUMERICHOST) ? "NI_NUMERICHOST " : "",
            (flags & NI_NAMEREQD)    ? "NI_NAMEREQD " : "",
            (flags & NI_NUMERICSERV) ? "NI_NUMERICSERV " : "" );

    if( sa->sa_family != AF_INET )
    {
        error( "getnameinfo: address is not an IPV4 address" );
        return -1;
    }

    if( hostname )
    {
        if( flags & NI_NUMERICHOST )
        {
            inet_ntop( sa->sa_family, sa->sa_data, hostname, hostlen );
        }
        else
        {
            if( (host = gethostbyaddr( (const char*) &(((struct sockaddr_in*) sa)->sin_addr.S_un.S_addr),
                                       4, sa->sa_family )) == NULL )
            {
                char buffer[256];
                error( "gethostbyname failed to resolve \"%s\": %s",
                       inet_ntop( sa->sa_family, sa->sa_data, buffer, sizeof(buffer) ),
                       wsa_strerror(errno) );
                return -1;
            }

            snprintf( hostname, hostlen, "%s", host->h_name );
        }

        debug2( "getnameinfo returning hostname \"%s\"", hostname );
    }

    if( servname )
    {
        if( flags & NI_NUMERICSERV )
        {
            snprintf( servname, servlen, "%d", ntohs( ((struct sockaddr_in*) sa)->sin_port ) );
            debug2( "getnameinfo returning service \"%s\"", servname );
        }
    }

    return 0;
}

void freeaddrinfo(
  struct addrinfo* ai
  )
{
    debug2( "freeaddrinfo( 0x%p )", ai );
    xfree( ai );
}

/* getaddrinfo() error message */
char* gai_strerror(
  int ecode
  )
{
    return "ERROR from gai_strerror()";
}

const char *inet_ntop( int af, const void *src, char *dst, socklen_t len )
{
    debug2( "inet_ntop( %d, 0x%p, 0x%p, %d )", af, src, dst, len );

    if( af == AF_INET )
    {
        const u_char *uc_src = (const u_char*) src;
        struct in_addr in;
        in.S_un.S_un_b.s_b1 = uc_src[2];
        in.S_un.S_un_b.s_b2 = uc_src[3];
        in.S_un.S_un_b.s_b3 = uc_src[4];
        in.S_un.S_un_b.s_b4 = uc_src[5];
        sprintf( dst, "%s", inet_ntoa(in) );
        debug( "inet_ntop returning \"%s\"", dst );
        return dst;
    }
    else if ( af == AF_INET6 )
    {
        // fall thru...
    }

    debug( "inet_ntop returning NULL" );
    return NULL;
}

/* Report error from WSAGetLastError() when a socket operation fails */
const char *wsa_strerror( int errno_value )
{
    static char buffer[256];
    static const char *messages[] = {
        /*WSAEWOULDBLOCK     10035*/ "Resource temporarily unavailable",
        /*WSAEINPROGRESS     10036*/ "Operation now in progress",
        /*WSAEALREADY        10037*/ "Operation already in progress",
        /*WSAENOTSOCK        10038*/ "Socket operation on non-socket",
        /*WSAEDESTADDRREQ    10039*/ "Destination address required",
        /*WSAEMSGSIZE        10040*/ "Message too long",
        /*WSAEPROTOTYPE      10041*/ "Protocol wrong type for socket",
        /*WSAENOPROTOOPT     10042*/ "Bad protocol option",
        /*WSAEPROTONOSUPPORT 10043*/ "Protocol not supported",
        /*WSAESOCKTNOSUPPORT 10044*/ "Socket type not supported",
        /*WSAEOPNOTSUPP      10045*/ "Operation not supported",
        /*WSAEPFNOSUPPORT    10046*/ "Protocol family not supported",
        /*WSAEAFNOSUPPORT    10047*/ "Address family not supported by protocol family",
        /*WSAEADDRINUSE      10048*/ "Address already in use",
        /*WSAEADDRNOTAVAIL   10049*/ "Cannot assign requested address",
        /*WSAENETDOWN        10050*/ "Network is down",
        /*WSAENETUNREACH     10051*/ "Network is unreachable",
        /*WSAENETRESET       10052*/ "Network dropped connection on reset",
        /*WSAECONNABORTED    10053*/ "Software caused connection abort",
        /*WSAECONNRESET      10054*/ "Connection reset by peer",
        /*WSAENOBUFS         10055*/ "No buffer space available.",
        /*WSAEISCONN         10056*/ "Socket is already connected",
        /*WSAENOTCONN        10057*/ "Socket is not connected",
        /*WSAESHUTDOWN       10058*/ "Cannot send after socket shutdown",
        NULL,
        /*WSAETIMEDOUT       10060*/ "Connetion timed out",
        /*WSAECONNREFUSED    10061*/ "Connection refused",
        NULL,
        NULL,
        /*WSAEHOSTDOWN       10064*/ "Host is down",
        /*WSAEHOSTUNREACH    10065*/ "No route to host",
        NULL,
        /*WSAEPROCLIM        10067*/ "Too many processes"
    };
    const char *message = NULL;
    int errnum = WSAGetLastError();

    if( (errnum >= 10035) && (errnum <= 10067) )
        message = messages[errnum - 10035];

    if( message )
        return message;

    /* Non-contiguous or host dependent message numbers */
    switch( errnum )
    {
    case WSAEINTR           /*10004*/: return "Interrupted function call";
    case WSAEACCES          /*10013*/: return "Permision denied";
    case WSAEFAULT          /*10014*/: return "Bad address";
    case WSAEINVAL          /*10022*/: return "Invalid argument";
    case WSAEMFILE          /*10024*/: return "Too many open files";
    case WSASYSNOTREADY     /*10091*/: return "Network subsystem is unavailable";
    case WSAVERNOTSUPPORTED /*10092*/: return "WINSOCK.DLL version out of range";
    case WSANOTINITIALISED  /*10093*/: return "Successful WSAStartup not yet performed";
    case WSAEDISCON         /*10094*/: return "Graceful shutdown in progress";
    /*case WSATYPE            10109: return "Class type not found";*/
    case WSAHOST_NOT_FOUND  /*11001*/: return "Host not found";
    case WSATRY_AGAIN       /*11002*/: return "Non-authoritative host not found";
    case WSANO_RECOVERY     /*11003*/: return "This is a non-recoverable error";
    case WSANO_DATA         /*11004*/: return "Valid name, no data record of requested type";
    case WSA_INVALID_HANDLE:    return "Specified event object handle is invalid";
    case WSA_INVALID_PARAMETER: return "One or more parameters are invalid";
    /*case WSAINVALIDPROCTABLE:   return "Invalid procedure table from service provider";*/
    /*case WSAINVALIDPROVIDER:    return "Invalid service provider version number";*/
    case WSA_IO_INCOMPLETE:     return "Overlapped I/O event object not in signaled state";
    case WSA_IO_PENDING:        return "Overlapped operations will complete later";
    case WSA_NOT_ENOUGH_MEMORY: return "Insufficient memory available";
    /*case WSAPROVIDERFAILEDINIT: return "Unable to initialize a service provider";*/
    case WSASYSCALLFAILURE:     return "System call failure";
    case WSA_OPERATION_ABORTED: return "Overlapped operation aborted";
    }

    sprintf( buffer, "(%d)", errnum );
    return buffer;
}


void reportWindowsError( const char *fmt )
{
    LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
    );
    debug( fmt, (LPCTSTR) lpMsgBuf );
    LocalFree( lpMsgBuf );
}


/**
 * This function checks to see if there is input on the given file descriptor.
 * The file descriptor may represent the console, a pipe from another process
 * or a redirect file.  In most cases the file descriptor is simply a dup()ed
 * copy of the stdin file descriptor, or the stdin descriptor itself.
 * RHT 19/06/2008 - modified to peek two events ahead and to only look for
 *   key-up events.  Previously we assumed that the number of events was
 *   actually the number of charcters, which it isn't and as there are mouse
 *   events mixed in there we could end up in a hopeless mess.
 */
int check_console_for_input(
      int fd,
      fd_set * readfds,               
      fd_set * writefds
    )
{
	
    HANDLE h;
    INPUT_RECORD irec[2];
    DWORD avail = 0;
    int res;

    /* Check for characters available on stdin */
    h = GetStdHandle(STD_INPUT_HANDLE);
    res = PeekConsoleInput( h, irec, 2, &avail );

    if( res != 0 )
    {
        SetConsoleMode(h, ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
    }
    else if( GetLastError() == ERROR_INVALID_HANDLE )
    {
        /* Not a console, so is it a pipe from another process? */
        res = PeekNamedPipe( h, NULL, 0, NULL, &avail, NULL );
        if( res == 0 )
        {
            /* Not a pipe either, so must be a redirect file */
            res = WaitForSingleObject( h, 0 );
            if( res == WAIT_TIMEOUT )
                avail = 1;
            else if( res == WAIT_OBJECT_0 )
                debug( "terminated" );
            else if( res == WAIT_FAILED )
                reportWindowsError( "WaitForSingleObject failed: %s" );
        }
    }
    else
        reportWindowsError( "PeekConsoleInput failed: %s" );


    if( avail ) {
		/*
		if((irec[0].EventType == KEY_EVENT) && irec[0].Event.KeyEvent.bKeyDown)
		{
			FD_ZERO(readfds);
			FD_ZERO(writefds);
			FD_SET(fd, readfds);
			debug("check_console_for_input() event 0 is key down - returning 1");
			return 1;
		}

		if((irec[1].EventType == KEY_EVENT) && irec[1].Event.KeyEvent.bKeyDown)
		{
			FD_ZERO(readfds);
			FD_ZERO(writefds);
			FD_SET(fd, readfds);
			debug("check_console_for_input() event 1 is key-down - returning 1");
			return 1;
		}
		*/
		FD_ZERO(readfds);
		FD_ZERO(writefds);
		FD_SET(fd, readfds);	
		return 1;
    }

    return 0;
}

int unix_select (
      int nfds,                           
      fd_set * readfds,               
      fd_set * writefds,              
      fd_set * exceptfds,             
      const struct timeval * timeout,
      int session_ident,
      int connection_in,
      int connection_out,
      int control_fd
    )
{
	struct timeval tv;
    fd_set readset, writeset;
    int ret, done, socks;

    done = 0;

    while( !done )
    {
        if( FD_ISSET(fileno(stdin), readfds)
            && check_console_for_input(fileno(stdin), readfds, writefds) )
            return 1;

        if( channel_still_open() )
        {
            Channel *c = channel_by_id(session_ident);

            /* Check for characters available in decoded output channel buffer */
            if( c && FD_ISSET(c->wfd, writefds)
                && (buffer_len(&c->output) > 0) )
            {
                /*debug( "%d bytes waiting on output", buffer_len(&c->output) );*/
                FD_ZERO(readfds);
                FD_ZERO(writefds);
                FD_SET(c->wfd, writefds);
                return 1;
            }

            /* Check for console input */
            
			if( c && FD_ISSET(c->rfd, readfds)
                && check_console_for_input(c->rfd, readfds, writefds) )
                return 1;

            /* Check for characters available in decoded extended channel buffer */
			/* RHT 14/01/2011 - check write fds as well - fixes hang on stderr output */
            if( c && (FD_ISSET(c->efd, readfds) || FD_ISSET(c->efd, writefds)) )
            {
                if( (c->extended_usage == CHAN_EXTENDED_READ)
                        && (buffer_len(&c->extended) < c->remote_window) )
                {
                    debug( "%d bytes waiting on extended (read)", buffer_len(&c->extended) );
                    FD_ZERO(readfds);
                    FD_ZERO(writefds);
                    FD_SET(c->efd, readfds);
                    return 1;
                }
                else if( (c->extended_usage == CHAN_EXTENDED_WRITE)
                        && (buffer_len(&c->extended) > 0) )
                {
                    debug( "%d bytes waiting on extended (write)", buffer_len(&c->extended) );
                    FD_ZERO(readfds);
                    FD_ZERO(writefds);
                    FD_SET(c->efd, writefds);
                    return 1;
               }
            }
        }

        /* WIN32 does not support ordinary file descriptors in a select */
        FD_ZERO(&readset);
        FD_ZERO(&writeset);

        socks = 0;
        if( FD_ISSET(connection_in, readfds) ) { socks++; FD_SET(connection_in, &readset); }
        if( (control_fd != -1) && FD_ISSET(control_fd, readfds) )  { socks++; FD_SET(control_fd, &readset); }
        if( FD_ISSET(connection_out, writefds) ) { socks++; FD_SET(connection_out, &writeset); }

        /* Only do a select if there is at least on socket to check */

        if( socks > 0 )
        {
	        tv.tv_sec = 1;
	        tv.tv_usec = 0;
	        ret = select(nfds, &readset, &writeset, exceptfds, &tv);

            if( ret < 0 ) {
                debug( "ret = %d (%s)", ret, wsa_strerror(errno) );
                return ret;
            }

            if( ret > 0 ) {
                done = 1;
            }
        }
    }

    /* Setup the fd_set's with the active file descriptors */
    FD_ZERO(readfds);
    FD_ZERO(writefds);
    if( FD_ISSET(connection_in, &readset) )  FD_SET(connection_in, readfds);
    if( (control_fd != -1) && FD_ISSET(control_fd, &readset) )    FD_SET(control_fd, readfds);
    if( FD_ISSET(connection_out, &writeset) )  FD_SET(connection_out, writefds);

    return ret;
}


/* GRP and PWD */

gid_t getegid()  { return 501; }
int setegid( gid_t egid )  { return 1; }

uid_t geteuid()  { return 500; }
int seteuid( uid_t euid )  { return 1; }

gid_t getgid()  { return 501; }
int setgid( gid_t gid )  { return 1; }

uid_t getuid() { return 500; }
int setuid( uid_t uid ) { return 1; }

/**
 * Figure out the user's home directory.
 * 1. %USERPROFILE%
 * 2. %HOME"
 * 3. %HOMEDRIVE%%HOMEPATH%
 * 4. %SYSTEMDRIVE%
 * 5. C:\
 */
static const char *getUserHomeDir()
{
    const char *home = getenv("USERPROFILE");
    if( !home )
    {
        home = getenv("HOME");
        if( !home )
        {
            const char *drive = getenv("HOMEDRIVE");
            const char *path = getenv("HOMEPATH");
            if( drive && path )
            {
                char *home2 = xmalloc( strlen(drive) + strlen(path) + 1 );
                sprintf( home2, "%s%s", drive, path );
                home = home2;
            }
            else
            {
                home = getenv("SYSTEMDRIVE");
            }
        }
    }
    if( home && (home[strlen(home)-1] != '\\') )
    {
        char *home2 = xmalloc( strlen(home) + 2 );
        sprintf( home2, "%s\\", home );
        home = home2;
    }
    if( !home )
        home = "C:\\";

    return home;
}

struct passwd *getpwuid( uid_t uid )
{
    struct passwd *pw = xmalloc( sizeof(struct passwd) );
    const char *home = getUserHomeDir();

    debug2( "getpwuid( %d )", uid );

    pw->pw_name = "Robert";
    pw->pw_passwd = "password";
    pw->pw_gid = 501;
    pw->pw_uid = 500;
    pw->pw_comment = "";
    pw->pw_gecos = "";
    pw->pw_dir = home ? xstrdup( home ) : "~";
    pw->pw_shell = "cmd.exe";
    return pw;
}

struct passwd *getpwnam( const char * user )
{
    debug2( "getpwunam( %s )", user );
    return getpwuid( 500 );
}

int initgroups( const char *name, gid_t pw_gid )
{
    debug2( "initgroups( \"%s\", %d )", name, pw_gid );
    return 0;
}

int getgroups( int len, gid_t *groups )
{
   LPGROUP_USERS_INFO_0 pBuf = NULL;
   DWORD dwEntriesRead = 0;
   DWORD dwTotalEntries = 0;
   NET_API_STATUS nStatus;
   wchar_t server[7] = { 'P', 'o', 'o', 'k', 'y', '\0' };
   wchar_t user[8] = { 'R', 'o', 'b', 'e', 'r', 't', '\0' };

   
   debug2( "getgroups( %d, 0x%p )", len, groups );

   // Call the NetUserGetGroups function, specifying level 0.
   nStatus = NetUserGetGroups( server, user, 0,
                              (LPBYTE*)&pBuf, -1,
                              &dwEntriesRead, &dwTotalEntries);
   // If the call succeeds,
   if (nStatus == NERR_Success)
   {
      LPGROUP_USERS_INFO_0 pTmpBuf;
      DWORD i;
      DWORD dwTotalCount = 0;

      if ((pTmpBuf = pBuf) != NULL)
      {
         fprintf(stderr, "\nGlobal group(s):\n");
         //
         // Loop through the entries;
         //  print the name of the global groups
         //  to which the user belongs.
         //
         for (i = 0; i < dwEntriesRead; i++)
         {
            //assert(pTmpBuf != NULL);

            if (pTmpBuf == NULL)
            {
               fprintf(stderr, "An access violation has occurred\n");
               break;
            }

            wprintf(L"\t-- %s\n", pTmpBuf->grui0_name);

            pTmpBuf++;
            dwTotalCount++;
         }
      }
      //
      // If all available entries were
      //  not enumerated, print the number actually
      //  enumerated and the total number available.
      //
      if (dwEntriesRead < dwTotalEntries)
         fprintf(stderr, "\nTotal entries: %d", dwTotalEntries);
      //
      // Otherwise, just print the total.
      //
      printf("\nEntries enumerated: %d\n", dwTotalCount);
   }
   else
      fprintf(stderr, "A system error has occurred: %d\n", nStatus);

   //
   // Free the allocated buffer.
   //
   if (pBuf != NULL)
      NetApiBufferFree(pBuf);

   if( !groups )
        return dwTotalEntries;
   
   

    return dwTotalEntries;
}

int setgroups( int len, gid_t *groups )
{
    debug2( "initgroups( %d, 0x%p )", len, groups );
    return 0;
}


pid_t getpid()  { return 123; }
pid_t setsid(void)  { return 123; }
pid_t getpgrp(void)  { return 123; }
pid_t tcgetpgrp(int __fildes)  { return 123; }


int tcgetattr( int fd, struct termios *tio )
{
    debug2( "tcgetattr( %d, 0x%p )", fd, tio );
	if( fd == STDIN_FILENO )
	{
		HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
		DWORD flags = 0;
		GetConsoleMode(h, &flags);
		tio->c_lflag = (flags & ENABLE_ECHO_INPUT) ? ECHO : 0;
	}
    return 0;
}

int tcsetattr( int fd, int attr, struct termios *tio )
{
    debug2( "tcsetattr( %d, %d, 0x%p )", fd, attr, tio );

	if( fd == STDIN_FILENO )
	{
		HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
		DWORD flags = ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT;
		if(tio->c_lflag == ECHO) flags |= ENABLE_ECHO_INPUT;
		SetConsoleMode(h, flags);
	}
    return 0;
}

int cfsetispeed (struct termios *tio, speed_t speed)
{
    debug2( "cfsetispeed( 0x%p, %d )", tio, speed );
    tio->c_ispeed = speed;
    return 0;
}

int cfsetospeed (struct termios *tio, speed_t speed)
{
    debug2( "cfsetospeed( 0x%p, %d )", tio, speed );
    tio->c_ospeed = speed;
    return 0;
}


/* LOGGING */

void    closelog (void) {}
void    openlog (const char *prog, int flags, int facility) {}

void    syslog (int priority, const char *format, ...)
{
	va_list args;

	va_start( args, format );
	vprintf( format, args );
	va_end( args );
}


/* STRINGS */

size_t strlcpy( char *s1, const char *s2, size_t len )
{
    /*debug2( "strlcpy( 0x%p, \"%s\", %d )", s1, s2, len );*/
    return snprintf( s1, len, "%s", s2 );
}

size_t strlcat( char *s1, const char *s2, size_t len )
{
    char *s3 = xstrdup(s1);
    int ret = snprintf( s1, len, "%s%s", s3, s2 );
    /*debug2( "strlcat( \"%s\", \"%s\", %d ) returning \"%s\"", s3, s2, len, s1 );*/
    xfree(s3);
    return ret;
}

int64_t strtoll( const char *s, char **ep, int base )
{
    return (int64_t) strtol(s,ep,base);
}

int fork()
{
    debug( "fork" );
    return 0;
}

int waitpid()
{
    debug( "waitpid" );
    return 0;
}

int pipe()
{
    debug( "pipe" );
    return 0;
}


#endif /*WIN32*/
