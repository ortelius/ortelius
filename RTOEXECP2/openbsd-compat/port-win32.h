/**
 * Win32 specific stuff - mainly functions and structures which are missing
 * under Windows.
 */

#ifndef __PORT_WIN32_H
#define __PORT_WIN32_H

#ifdef WIN32


#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short u_short;

#define gid_t int
#define uid_t int
#define pid_t int
#define mode_t int

#  define       NBBY    8               /* number of bits in a byte */
typedef     long    fd_mask;
#  define       NFDBITS (sizeof (fd_mask) * NBBY)       /* bits per mask */
#  ifndef       howmany
#       define  howmany(x,y)    (((x)+((y)-1))/(y))
#  endif

#define socklen_t u_int
#define sockaddr_storage sockaddr
#define ss_family sa_family
#define setsockopt(sock,level,name,value,len) setsockopt(sock,level,name, (const char FAR*) value,len)
#define getsockopt(sock,level,name,value,len) getsockopt(sock,level,name, (char FAR*) value,len)

#define strcasecmp stricmp
#define strncasecmp strnicmp
#define snprintf _snprintf
#define vsnprintf _vsnprintf

#define mkdir(path,mode) _mkdir(path)
#define lstat(a,b) stat(a,b)

typedef struct addrinfo {
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  size_t ai_addrlen;
  char* ai_canonname;
  struct sockaddr* ai_addr;
  struct addrinfo* ai_next;
} ADDRINFOA, 
 *PADDRINFOA;


int /*WSAAPI*/ getaddrinfo(
  const char* nodename,
  const char* servname,
  const struct addrinfo* hints,
  struct addrinfo** res
);

int /*WSAAPI*/ getnameinfo(
  const struct sockaddr FAR* sa,
  socklen_t salen,
  char FAR* host,
  DWORD hostlen,
  char FAR* serv,
  DWORD servlen,
  int flags
);

void freeaddrinfo(
  struct addrinfo* ai
);

char* gai_strerror(
  int ecode
);

const char *wsa_strerror( int ecode );

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
);


/* GRP and PWD */

struct passwd {
    char *pw_name;
    char *pw_passwd;
    uid_t pw_uid;
    gid_t pw_gid;
    char *pw_comment;
    char *pw_gecos;
    char *pw_dir;
    char *pw_shell;
};

gid_t getegid();
int setegid( gid_t egid );
uid_t geteuid();
int seteuid( uid_t euid );
gid_t getgid();
int setgid( gid_t gid );
uid_t getuid();
int setuid( uid_t uid );

struct passwd *getpwuid( uid_t uid );
struct passwd *getpwnam( const char * user );

int getgroups( int len, gid_t *groups );
int setgroups( int len, gid_t *groups );
int initgroups( const char *name, gid_t pw_gid );

pid_t getpid();
pid_t setsid(void);
pid_t getpgrp(void);
pid_t tcgetpgrp(int __fildes);
#define sleep(secs) Sleep(secs*1000)


/* TERMIOS */

/* cflag */
#define B0      0x00000
#define B50     0x00001
#define B75     0x00002
#define B110    0x00003
#define B134    0x00004
#define B150    0x00005
#define B200    0x00006
#define B300    0x00007
#define B600    0x00008
#define B1200   0x00009
#define B1800   0x0000a
#define B2400   0x0000b
#define B4800   0x0000c
#define B9600   0x0000d
#define B19200  0x0000e
/* EXTA */
#define B38400  0x0000f
/* EXTB */
#define CS7     0x00020
#define CS8     0x00030
#define PARENB  0x00100
#define PARODD  0x00200
/* B7200 */
/* B14400 */
/* B28800 */
#define B57600  0x01001
/* B76800 */
#define B115200 0x01002
#define B230400 0x01004

/* iflag */
#define IGNPAR  0x00004
#define IMAXBEL 0x00008
#define INPCK   0x00010
#define ISTRIP  0x00020
#define INLCR   0x00040
#define IGNCR   0x00080
#define ICRNL   0x00100
#define IXON    0x00400
#define IXOFF   0x01000
#define IUCLC   0x04000
#define IXANY   0x08000
#define PARMRK  0x10000

/* lflag*/
#define ISIG    0x0001
#define ICANON  0x0002
/*XCASE*/
#define ECHO    0x0004
#define ECHOE   0x0008
#define ECHOK   0x0010
#define ECHONL  0x0020
#define NOFLSH  0x0040
#define TOSTOP  0x0080
#define IEXTEN  0x0100
#define ECHOKE  0x0400
#define ECHOCTL 0x0800
/*PENDIN*/

/* oflag */
#define OPOST   0x00001
#define OLCUC   0x00002
#define OCRNL   0x00004
#define ONLCR   0x00008
#define ONOCR   0x00010
#define ONLRET  0x00020

/* cc */
#define VINTR  1
#define VEOF   4
#define VERASE 5
#define VMIN   9
#define VQUIT  10
#define VSTART 12
#define VSTOP  13
#define VTIME  16

#define NCCS            18

typedef unsigned char cc_t;
typedef unsigned int  tcflag_t;
typedef unsigned int  speed_t;
typedef unsigned short otcflag_t;
typedef unsigned char ospeed_t;

struct termios
{
  tcflag_t      c_iflag;
  tcflag_t      c_oflag;
  tcflag_t      c_cflag;
  tcflag_t      c_lflag;
  char          c_line;
  cc_t          c_cc[NCCS];
  speed_t       c_ispeed;
  speed_t       c_ospeed;
};

#define TCSADRAIN 0x01
#define TCSANOW   0x02
#define TCSAFLUSH 0x04

int tcgetattr( int attr, struct termios *tio );
int tcsetattr( int fd, int attr, struct termios *tio );

#define cfgetospeed(tp)         ((tp)->c_ospeed)
#define cfgetispeed(tp)         ((tp)->c_ispeed)

int cfsetispeed (struct termios *tio, speed_t speed);
int cfsetospeed (struct termios *tio, speed_t speed);

/* Extra stuff to make porting stuff easier.  */
struct winsize
{
  unsigned short ws_row, ws_col;
  unsigned short ws_xpixel, ws_ypixel;
};

#define TIOCGWINSZ (('T' << 8) | 1)
#define TIOCSWINSZ (('T' << 8) | 2)

#define ioctl(a,b,c) ioctlsocket(a,b,(u_long*)c)


/* LOGGING */

#define LOG_EMERG       0
#define LOG_ALERT       1
#define LOG_CRIT        2
#define LOG_ERR         3
#define LOG_WARNING     4
#define LOG_NOTICE      5
#define LOG_INFO        6
#define LOG_DEBUG       7

#define LOG_KERN        (0<<3)
#define LOG_USER        (1<<3)
#define LOG_MAIL        (2<<3)
#define LOG_DAEMON      (3<<3)
#define LOG_AUTH        (4<<3)
#define LOG_SYSLOG      (5<<3)
#define LOG_LPR         (6<<3)
#define LOG_NEWS        (7<<3)
#define LOG_UUCP        (8<<3)
#define LOG_CRON        (9<<3)
#define LOG_AUTHPRIV    (10<<3)
#define LOG_FTP         (11<<3)
/* Codes through 15 are reserved for system use */
#define LOG_LOCAL0      (16<<3)
#define LOG_LOCAL1      (17<<3)
#define LOG_LOCAL2      (18<<3)
#define LOG_LOCAL3      (19<<3)
#define LOG_LOCAL4      (20<<3)
#define LOG_LOCAL5      (21<<3)
#define LOG_LOCAL6      (22<<3)
#define LOG_LOCAL7      (23<<3)

#define LOG_PID         0x01    /* log the pid with each message */
#define LOG_CONS        0x02    /* log on the console if errors in sending */
#define LOG_ODELAY      0x04    /* delay open until first syslog() (default) */
#define LOG_NDELAY      0x08    /* don't delay open */
#define LOG_NOWAIT      0x10    /* don't wait for console forks: DEPRECATED */
#define LOG_PERROR      0x20    /* log to stderr as well */

void    closelog (void);
void    openlog (const char *prog, int flags, int facility);
void    syslog (int priority, const char *format, ...);


#define EADDRINUSE 1
#define EINPROGRESS 1
#define ETIMEDOUT 1
#define EAFNOSUPPORT 1
#define ENOTCONN 1
#define IN_LOOPBACKNET 127
#define WIFEXITED(x) 0
#define WEXITSTATUS(x) 0
#define LLONG_MIN 0
#define LLONG_MAX 0

#ifdef __cplusplus
}
#endif


#endif /*WIN32*/

#endif /*__PORT_WIN32_H*/
