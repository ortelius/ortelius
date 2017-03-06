#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <fcntl.h>
#ifndef WIN32
#include <unistd.h>
#include <strings.h>
#include <ctype.h>
#define FALSE 0
#define TRUE 1
#endif
#include <errno.h>
#include <signal.h>

#include <assert.h>
#include <stdio.h>

// #include "triipc.h"
#include "ftpcomm.h"
#include <stdarg.h>

// #include "logging.h"
// #include "expandvars.h"

#ifdef WIN32
#include <io.h>
#include <process.h>
#define STRCASECMP _stricmp
#define strdup _strdup
#else
#define STRCASECMP strcasecmp
#endif

// #include "platform.h"

char *GlobalErrorPtr;

char *ReturnErrorPtr()
{
	return GlobalErrorPtr;
}

void SetGlobalErrorPtr(char *ErrStr, ...)
{
	char buf[2048];
	va_list ap;
	va_start(ap,ErrStr);
	vsnprintf(buf,sizeof(buf),ErrStr,ap);
	va_end(ap);
	if (GlobalErrorPtr) free(GlobalErrorPtr);
	GlobalErrorPtr = strdup(buf);
}


// #undef ENABLE_FTPS

#ifdef ENABLE_FTPS
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"
#endif /*ENABLE_SFTP*/

#define Free(x) free(x)
#define Malloc(x) malloc(x)
#define Strdup(x) strdup(x)
#define Realloc(x,y) realloc(x,y)

typedef struct ftp_system_type_strings{
char			*SystemTypeString;
FTP_SYSTEM_TYPE		SystemType;
char			DirSepChar;
} SYSTEM_TYPE_STRINGS;

static SYSTEM_TYPE_STRINGS *FTP_System_Strings = NULL;
static int FTP_System_Strings_Count = 0;

static SYSTEM_TYPE_STRINGS systemStrings[]={
{"UNIX",		FTP_SYSTEM_UNIX,	'/'	},
{"Windows_NT",	FTP_SYSTEM_WIN32,	'\\'},
{"OS/400",		FTP_SYSTEM_AS400,	'/'	},
{"VMS",			FTP_SYSTEM_VMS,		'.'	},
{"Nonstop", FTP_SYSTEM_TANDEM_GUARDIAN,	'.'	},
{"Tandem Guardian", FTP_SYSTEM_TANDEM_GUARDIAN,	'.'	},	// Dummy string - use to record dirSepChar
{"Tandem OSS",	FTP_SYSTEM_TANDEM_OSS,	'/'	},			// Dummy string - use to record dirSepChar
{"MVS",			FTP_SYSTEM_MVS, '.' }	// RHT 18/01/2011
};

typedef struct monthnames_tag {
char	*MonthName;
int	MonthNumber;
} MONTHNAMES;

static MONTHNAMES *FTP_Month_Names = NULL;
static int FTP_Month_Names_Count = 0;

static MONTHNAMES defaultMonthNames[] = {
	{"Jan",	1},
	{"Feb",	2},
	{"Mar",	3},
	{"Apr",	4},
	{"May",	5},
	{"Jun",	6},
	{"Jul",	7},
	{"Aug",	8},
	{"Sep",	9},
	{"Oct",	10},
	{"Nov",	11},
	{"Dec",	12}
};

int FTP_Read(FTPCONNECTION *c, char *buffer, int len);
int FTP_Write(FTPCONNECTION *c, char *buffer, int len);
#ifdef ENABLE_FTPS
static char* get_ssl_error();
#endif /*ENABLE_FTPS*/

// from triserv.cpp (UGH!) - RHT 15/08/2011
char *strstrnocase(char *str, const char *pat)
{
	int m = -1;
	while(pat[++m])
	{
		if(str[m] == 0) {
			return 0;
		}
		if(tolower(str[m]) != tolower(pat[m])) {
			str++;
			m = -1;
		}
	}
	return str;
}


static char* FtpSystemString_Strdup( const char * str )
{
	char *ret = (char*) malloc(strlen(str)+1);
	strcpy(ret, str);
	return ret;
}

static void BuildFtpSystemStrings()
{
	FTP_System_Strings_Count = sizeof(systemStrings)/sizeof(SYSTEM_TYPE_STRINGS);
	FTP_System_Strings = (SYSTEM_TYPE_STRINGS*) malloc((FTP_System_Strings_Count) * sizeof(SYSTEM_TYPE_STRINGS));	// ### RHT 30/04/2008 - don't use our memory manager

	for (int k=0; k<FTP_System_Strings_Count; k++)
	{
		FTP_System_Strings[k].SystemTypeString = FtpSystemString_Strdup(systemStrings[k].SystemTypeString);	// ### RHT 30/04/2008 - don't use our memory manager
		FTP_System_Strings[k].SystemType       = systemStrings[k].SystemType;
		FTP_System_Strings[k].DirSepChar       = systemStrings[k].DirSepChar;
	}

	// RHT 07/09/2010 - Populate the month names table as well
	FTP_Month_Names_Count = sizeof(defaultMonthNames)/sizeof(MONTHNAMES);
	FTP_Month_Names = (MONTHNAMES*) malloc((FTP_Month_Names_Count) * sizeof(MONTHNAMES));

	for (int j=0; j<FTP_Month_Names_Count; j++)
	{
		FTP_Month_Names[j].MonthName = FtpSystemString_Strdup(defaultMonthNames[j].MonthName);
		FTP_Month_Names[j].MonthNumber = defaultMonthNames[j].MonthNumber;
	}
}

void ResetSystemTypes()
{
	for (int k=sizeof(systemStrings)/sizeof(SYSTEM_TYPE_STRINGS); k<FTP_System_Strings_Count; k++)
	{
		free(FTP_System_Strings[k].SystemTypeString);	// ### RHT 30/04/2008 - don't use our memory manager
		FTP_System_Strings[k].SystemTypeString = NULL;
	}
	FTP_System_Strings_Count = sizeof(systemStrings)/sizeof(SYSTEM_TYPE_STRINGS);

	// RHT 07/09/2010 - Reset the month names table as well
	for (int j=sizeof(defaultMonthNames)/sizeof(MONTHNAMES); j<FTP_Month_Names_Count; j++)
	{
		free(FTP_Month_Names[j].MonthName);
		FTP_Month_Names[j].MonthName = NULL;
	}
	FTP_Month_Names_Count = sizeof(defaultMonthNames)/sizeof(MONTHNAMES);
}

void AddFtpSystemType( const char *str, const char *typestr )
{
	if (!FTP_System_Strings) { BuildFtpSystemStrings(); }
	//
	// Go through the list of known system types to match typestr
	//
	for (int k=0; k<(sizeof(systemStrings)/sizeof(SYSTEM_TYPE_STRINGS)); k++)
	{
		if (strcmp(	typestr, systemStrings[k].SystemTypeString)==0)
		{
			// Match
			FTP_System_Strings = (SYSTEM_TYPE_STRINGS*) realloc(FTP_System_Strings, (FTP_System_Strings_Count+1) * sizeof(SYSTEM_TYPE_STRINGS));	// ### RHT 30/04/2008 - don't use our memory manager
			FTP_System_Strings[FTP_System_Strings_Count].SystemTypeString = FtpSystemString_Strdup(str);	// ### RHT 30/04/2008 - don't use our memory manager
			FTP_System_Strings[FTP_System_Strings_Count].SystemType = systemStrings[k].SystemType;
			FTP_System_Strings[FTP_System_Strings_Count].DirSepChar=systemStrings[k].DirSepChar;
			FTP_System_Strings_Count++;
			break;
		}
	}
}

// RHT 07/09/2010 - Add a month name to the table
void AddMonthName( const char *str, int num )
{
	if (!FTP_Month_Names) { BuildFtpSystemStrings(); }

	FTP_Month_Names = (MONTHNAMES*) realloc(FTP_Month_Names, (FTP_Month_Names_Count+1) * sizeof(MONTHNAMES));
	FTP_Month_Names[FTP_Month_Names_Count].MonthName = FtpSystemString_Strdup(str);
	FTP_Month_Names[FTP_Month_Names_Count].MonthNumber = num;
	FTP_Month_Names_Count++;
}

void LogFTPTraffic( const char* fmt, ... ) 
{
	// char *Logging=getenv("TRIFTPLOGGING");
	char *Logging="Y";

	if (Logging && Logging[0]=='Y')
	{
		va_list args;

		va_start(args,fmt);
		char *lf = /* ExpandDateVars */(getenv("TRIFTPLOGFILE"));
		FILE *out=lf?fopen(lf,"a"):(FILE *)0;
		if (out)
		{
			struct tm *CurrTime;
			time_t	LocalTimeSinceEpoch;
			char TimeStamp[25];
			LocalTimeSinceEpoch=time((time_t *)0);

			CurrTime=localtime(&LocalTimeSinceEpoch);
			sprintf(TimeStamp,"%04d-%02d-%02d %02d:%02d:%02d ",
						CurrTime->tm_year+1900,
						CurrTime->tm_mon+1,
						CurrTime->tm_mday,
						CurrTime->tm_hour,
						CurrTime->tm_min,
						CurrTime->tm_sec);
			fprintf(out,TimeStamp);
			vfprintf(out,fmt,args);
			fclose(out);
		}
		va_end(args);
		//Free(lf);
	}
}


// RHT 17/11/2011 - Resusable version
void InternalLogFTPTraffic( FTPSOCKET *s, const char* fmt, va_list args ) 
{
	char *Logging=getenv("TRIFTPLOGGING");

	if (Logging && Logging[0]=='Y')
	{
		char *lf = /* ExpandDateVars */(getenv("TRIFTPLOGFILE"));
		FILE *out=fopen(lf,"a");
		if (out)
		{
			if (true)
			{
				struct tm *CurrTime;
				time_t	LocalTimeSinceEpoch;
				char TimeStamp[25];
				LocalTimeSinceEpoch=time((time_t *)0);

				CurrTime=localtime(&LocalTimeSinceEpoch);
				sprintf(TimeStamp,"%04d-%02d-%02d %02d:%02d:%02d ",
							CurrTime->tm_year+1900,
							CurrTime->tm_mon+1,
							CurrTime->tm_mday,
							CurrTime->tm_hour,
							CurrTime->tm_min,
							CurrTime->tm_sec);
				fprintf(out,TimeStamp);
			}
			if (true)
			{
				if( s && s->HostName )
				{
					fprintf(out,"%s[%08x] ", s->HostName, (long) s);
				}
			}
			vfprintf(out,fmt,args);
			fclose(out);
		}
	}
}

// RHT 17/11/2011 - split into two functions so we can resuse
void LogFTPTraffic( FTPSOCKET *s, const char* fmt, ... ) 
{
	char *Logging=getenv("TRIFTPLOGGING");

	if (Logging && Logging[0]=='Y')
	{
		va_list args;

		va_start(args,fmt);
		InternalLogFTPTraffic(s, fmt, args);
		va_end(args);
	}
}


// RHT 17/11/2011 - new fn to print messages to the log on a level
void LogFTPTrace(int level, FTPSOCKET *s, const char* fmt, ...)
{
	int loglevel = 0;
	char *loglevelstr = getenv("TRIFTPLOGLEVEL");
	if(loglevelstr && loglevelstr[0]) {
		loglevel = atoi(loglevelstr);
	}

	if ((loglevel & level) != 0)
	{
		va_list args;

		va_start(args,fmt);
		InternalLogFTPTraffic(s, fmt, args);
		va_end(args);
	}
}


// RHT 29/07/2011 - new fn to hex dump the transfer buffer
void LogFTPBuffer(char *buf, int len, int level)
{
	int loglevel = 0;
	char *loglevelstr = getenv("TRIFTPLOGLEVEL");
	if(loglevelstr && loglevelstr[0]) {
		loglevel = atoi(loglevelstr);
	}

	if ((loglevel & level) != 0)
	{
		char *lf = /* ExpandDateVars */(getenv("TRIFTPLOGFILE"));
		FILE *out=fopen(lf,"a");
		if (out)
		{
			fprintf(out, "------ --------\n");
			for (int y = 0; y < len; y = y + 16)
			{
				int BytesToDump = ((len - y) < 16) ? (len - y) : 16;
				char hexline[80];
				char charline[80];
				char *hpos = hexline;
				char *cpos = charline;
				for (int y1 = y; y1 < (y+BytesToDump); y1++) {
					unsigned char ch = (unsigned char) buf[y1];
					hpos += sprintf(hpos, "%02x ", ch);
					cpos += sprintf(cpos, "%0c", ((ch >= 32) && (ch < 127)) ? buf[y1] : '.');
				}
				if(BytesToDump < 16) {
					for (int y2 = BytesToDump; y2 < 16; y2++) {
						hpos += sprintf(hpos, "   ");
					}
				}
				fprintf(out, "%08xh: %s ; %s\n", y, hexline, charline);
			}
			fprintf(out, "------ --------\n");
			fclose(out);
		}
		Free(lf);
	}
}


#ifdef ENABLE_FTPS
FTP_REPLY FTPS_DataConnect(FTPSOCKET *s, FTPCONNECTION *c);
FTP_REPLY FTPS_Connect(FTPSOCKET *s, FTPCONNECTION *c);
int FTPS_Read(FTPCONNECTION *c, char *buffer, int len);
int FTPS_Write(FTPCONNECTION *c, char *buffer, int len);
static int sslInitialised = 0;
#endif /*ENABLE_FTPS*/

static int FTP_SendDataToServer(FTPCONNECTION *c,caddr_t Data,long Len)
{
	// RHT 21/07/2011 - fix to make sure we send the entire buffer
	long bytesRemaining = Len;

	while(bytesRemaining > 0)
	{
        int bytesSent = (c->write_fn)(c,Data,bytesRemaining);
        if (bytesSent == 0)
        {
			return bytesSent;
        }
        else if (bytesSent == -1)
        {
            perror("Failed to send");
			return bytesSent;
        }

		Data += bytesSent;
		bytesRemaining -= bytesSent;
	}

	return Len;
}

// void FTP_GetWelcomeText(FTPSOCKET *s); // forward reference

char *FTP_ReadLineFromServer(FTPCONNECTION *c)
{
	// RHT 10/08/2011 - say no to statics
	/*static*/ char Reply[1024];
	int n;

	// RHT 10/08/2011 - just to make we have a null-terminator
	Reply[0] = '\0';

	// Read up to CR-LF
	char ch;
	int t=0;
	do
	{
		n=(c->read_fn)(c,&ch,1);

		if (n==0)
		{
			// Connection has been closed
			LogFTPTraffic("Connection Closed -setting reply to 421 TIMED OUT (CONNECTION CLOSED)\n");
			ch='\0';
			//
			// Return a "TIMED OUT" indication for those servers that
			// don't handle it properly....
			//
			strcpy(Reply,"421 TIMED OUT (CONNECTION CLOSED)");
		}
		else
		if (n==1)
		{
			if (ch=='\n' || t>=(sizeof(Reply)-1)) ch='\0';
			if (ch!='\r') Reply[t++]=ch;
		}
		else
		{
#if defined(WIN32) && defined(_DEBUG)
			// RHT 15/07/2008 - try and figure out why we landed here!
			// Have to do this first as LogFTPTraffic corrupts the error number.
#ifdef ENABLE_FTPS
			if (c->read_fn == FTP_Read)
			{
#endif /*ENABLE_FTPS*/
				if (n == SOCKET_ERROR)
				{
					LPVOID lpMsgBuf;
					FormatMessage( 
						FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf, 0, NULL  );
					LogFTPTraffic("WSAGetLastError reports %s", (LPCTSTR)lpMsgBuf);
					LocalFree( lpMsgBuf );
				}
				else
				{
					LogFTPTraffic("n = %d\n", n);
				}
#ifdef ENABLE_FTPS
			}
			else
			{
				const char *error = get_ssl_error();
				LogFTPTraffic("get_ssl_error reports %s\n", error ? error : "(null)");
			}
#endif /*ENABLE_FTPS*/
#endif /*WIN32 && _DEBUG*/

			LogFTPTraffic("recv failed - setting Reply to 421 TIMED OUT (RECV failed)\n");
			strcpy(Reply,"421 TIMED OUT (RECV failed)");
			break;	// recv failed
		}
	} while ((int)ch);

	// RHT 10/08/2011 - duplicate the buffer - all callers must now call Free()
	return Strdup(Reply);
}

void FTP_GetWelcomeText(FTPSOCKET *s)
{
	bool MoreToRead=false;
	do
	{
		char *res = FTP_ReadLineFromServer(&s->control);

		LogFTPTraffic(s,"<--- %s\n",res ? res : "(null)");	// RHT 15/07/2008 - Log hostname

		// RHT - 12/09/2007 - record the welcome text if not already set
		if( !s->WelcomeText )
			s->WelcomeText = Strdup( res );

		//
		// Welcome text always starts with 220. The character after the 220 is either a
		// space or a dash. A dash means there's more lines to follow....
		//
		char *x=res;
		while (*x==' ' || *x=='\t') x++; // Find first significant digit
		if (*x>='0' && *x<='9')
		{
			// Digits...
			while (*x && *x!=' ' && *x!='-') x++; // Find end of digits
			if (*x=='-')
			{
				MoreToRead=true;
			}
			else
			{
				MoreToRead=false;
			}
		}
		else
		{
			// No digits starting this line, probably a continuation line
			MoreToRead=true;
		}

		Free(res);	// RHT 10/08/2011
	} while (MoreToRead);

	//###
	if (s->WelcomeText)
	{ 
		int ReplyCode = (int)atol(s->WelcomeText);
		if (ReplyCode == 421)
		{
			// Looks like number of connections has been exceeded
			s->Result = FTP_NO_CONNECTION;
			s->LastMessage = Strdup(s->WelcomeText);
		}
	}
	//###
}

TRISOCKET *EstablishOutgoingConnection(int destination_port,char *HostName)
{
	int					sock;
	int					res;
	struct sockaddr_in	name;
	struct hostent		*hp;
	TRISOCKET			*reply;

	hp = gethostbyname(HostName);
	if (hp == (struct hostent *)0)
	{
		// gethostbyname fails	
		errno=h_errno;
		reply=(TRISOCKET *)-1;
	}
	else
	{
		// Create a socket on which to send.
		sock = socket(AF_INET, SOCK_STREAM, 0);
		name.sin_family = AF_INET;
		name.sin_addr.s_addr = *((int *)hp->h_addr);
		name.sin_port = htons(destination_port);
		res=connect(sock,(struct sockaddr *)&name,sizeof(name));
		if (res == 0)
		{
			reply=(TRISOCKET *)Malloc(sizeof(TRISOCKET));
			reply->sock = sock;
			reply->name = name;
			reply->port = destination_port;
		}
		else
		{
			// Connection failure
#ifdef WIN32
			errno=WSAGetLastError();
#endif
			reply=(TRISOCKET *)0;
		}
	}
	return reply;
}

/**
 * This function establishes a connection to an FTP server.  If we require a
 * secure connection for FTPS, then it is at this point that we request it.
 * We also ask for data connections to be encrypted during the FTPS setup.
 * Note that this function is only ever used to open CONTROL connections.
 * There is a simpler setup procedure for both encrypted and cleartext data
 * connections.
 */
static void SetupConnection(FTPSOCKET *ftpserver, bool secure, bool encryptData)
{
	ftpserver->control.trisocket=EstablishOutgoingConnection(21,ftpserver->HostName);
	if ((long)ftpserver->control.trisocket == -1)
	{
		ftpserver->Result = FTP_NO_SUCH_HOST;
		SetGlobalErrorPtr("No Such Host");
		ftpserver->control.trisocket=(TRISOCKET *)0;
	}
	else
	if (ftpserver->control.trisocket)
	{
		// Connection was established to FTP server
		FTP_GetWelcomeText(ftpserver);
		//###
		if (ftpserver->Result == FTP_NO_CONNECTION)
		{
			LogFTPTraffic("Welcome text bailed out\n");
			ftpserver->control.trisocket=(TRISOCKET *)0;
			return;
		}
		//###
	}
	else
	{
		SetGlobalErrorPtr("Connection Refused");
		ftpserver->Result = FTP_NO_CONNECTION;
		ftpserver->control.trisocket=(TRISOCKET *)0;
		return;
	}

	if( secure )
	{
#ifdef ENABLE_FTPS
		SERVERREPLY	t = FTP_SendCommandToServer(ftpserver,"AUTH","TLS");	// was SSL
		Free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
		if ((t.ReplyCode == 234) || (t.ReplyCode == 334))	// Some older servers may return 334
		{
			ftpserver->Result = FTPS_Connect(ftpserver, &ftpserver->control);
			if (ftpserver->Result != FTP_OKAY)
			{
				LogFTPTraffic("SSL connection didn't work - bailing out\n");
				return;
			}

			// Setup the control channel ftpconnection struct to use the SSL read/write fns
			ftpserver->control.read_fn  = FTPS_Read;
			ftpserver->control.write_fn = FTPS_Write;

			// Set protection buffer size to 0 - as required by RFC2228 before PROT.
			SERVERREPLY	t = FTP_SendCommandToServer(ftpserver,"PBSZ","0");
			Free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
			if (t.ReplyCode != 200)
			{
				LogFTPTraffic("FTPS: protection buffer size NOT set\n");
				// TODO: Bail out if this turns out to be fatal
			}

			// Set data channel protection - P = Private; C = Clear
			t = FTP_SendCommandToServer(ftpserver, "PROT", (char*)(encryptData?"P":"C"));
			Free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
			if (t.ReplyCode != 200)
			{
				LogFTPTraffic("FTPS: unable to set data channel protection mode\n");
				FTP_CloseDataConnection(&ftpserver->control);
				ftpserver->Result = FTP_NO_CONNECTION;
				return;
			}

			LogFTPTraffic("FTPS: data channel will be %s\n", (encryptData?"encrypted":"cleartext"));
			if (encryptData)
			{
				// Setup the data channel ftpconnection struct to use the SSL read/write fns
				ftpserver->data.read_fn  = FTPS_Read;
				ftpserver->data.write_fn = FTPS_Write;
			}
		}
		else
		{
			// Don't proceed any further if the user asked for FTPS and it isn't supported
			ftpserver->Result = FTP_NO_CONNECTION;
			ftpserver->control.trisocket=(TRISOCKET *)0;
		}
#else
		// Bail out - FTPS is not compiled in
		ftpserver->Result = FTP_NO_CONNECTION;
		ftpserver->LastMessage = Strdup("FTPS is not enabled on this build");
#endif /*ENABLE_FTPS*/
	}

}

void FTP_EnterASCIIMode(FTPSOCKET *s)
{
	SERVERREPLY	t;
	t=FTP_SendCommandToServer(s,"TYPE","A");
	Free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
	if (t.ReplyCode == 200)
	{
		// Switched okay
		s->LastMode=0;
	}
}

void FTP_EnterBinaryMode(FTPSOCKET *s)
{
	SERVERREPLY	t;
	t=FTP_SendCommandToServer(s,"TYPE","I");
	Free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
	if (t.ReplyCode == 200)
	{
		// Switched okay
		s->LastMode=1;
	}
}


FTP_REPLY FTP_LoginToServer(FTPSOCKET *s,char *Username,char *Password)
{
	SERVERREPLY	t;

	t=FTP_SendCommandToServer(s,"USER",Username);
	Free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
	if (t.ReplyCode == 331 || t.ReplyCode == 332 || t.ReplyCode == 220)
	{
		t=FTP_SendCommandToServer(s,"PASS",Password);
		// printf("replytext=[%s]\n",t.ReplyText);
		SetGlobalErrorPtr(t.ReplyText);
		Free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
	}
	if (t.ReplyCode != 230)
	{
		LogFTPTraffic("Failed to login to FTP Server\n");
		return FTP_INVALID_LOGIN;
	}
	return FTP_OKAY;
}


SERVERREPLY FTP_SendCommandToServer(FTPSOCKET *s,const char *cmd,const char *param)
{
	/* Sends the specified "cmd" to the server and returns the
	 * result code.
	 */
	char		*x;
	char *Reply = NULL;	// RHT 10/08/2011 - was static char Reply[1024];
	SERVERREPLY res;

	if (param[0])
	{
		x=(char *)Malloc(strlen(cmd)+strlen(param)+4);
		sprintf(x,"%s %s\r\n",cmd,param);
		if (strcmp(cmd,"PASS")==0)
		{
			LogFTPTraffic(s,"---> PASS ********\n");	// RHT 15/07/2008 - Log hostname
		}
		else
		{
			LogFTPTraffic(s,"---> %s %s\n",cmd,param);	// RHT 15/07/2008 - Log hostname
		}
	}
	else
	{
		x=(char *)Malloc(strlen(cmd)+3);
		strcpy(x,cmd);
		strcat(x,"\r\n");
		LogFTPTraffic(s,"---> %s\n",cmd);	// RHT 15/07/2008 - Log hostname
	}

	// RHT 17/07/2008 - will use LastMessage to store complete reply text
	if (s->LastMessage)
	{
		Free(s->LastMessage);
		s->LastMessage = NULL;
	}

	int a = FTP_SendDataToServer(&s->control,x,strlen(x));
	Free(x);
	if (a == -1)
	{
		// Send has failed - probably since other end has closed connection
		// due to a timeout or somesuch....
		//
		LogFTPTraffic("Connection failed - setting reply to 421 TIMED OUT (CONNECTION FAILED)\n");
		Reply = Strdup("421 TIMED OUT (CONNECTION FAILED)"); // RHT 10/08/2011 was strcpy(Reply,"421 TIMED OUT (CONNECTION FAILED)");
	}
	else
	{
		bool AnotherLineComing=false;
		do
		{
			if(Reply) { Free(Reply); }	// RHT 10/08/2011 - previous line already copied into LastMessage

			Reply = FTP_ReadLineFromServer(&s->control); // RHT 10/08/2011 - was strcpy(Reply,FTP_ReadLineFromServer(&s->control));
			LogFTPTraffic(s,"<--- %s\n",Reply);	// RHT 15/07/2008 - Log hostname
			// The reply should start with a number. If the first character
			// after the digits is a "-" then there's another line coming...
			char *t=Reply;
			if (*t>='0' && *t<='9')
			{
				while (*t>='0' && *t<='9') t++; // get to first non-numeric character
				if (*t=='-')
				{
					AnotherLineComing=true;
				}
				else
				{
					AnotherLineComing=false;
				}
			}
			else
			{
				AnotherLineComing=true;
			}

			// RHT 17/07/2008 - will use LastMessage to store complete reply text
			if( s->LastMessage )
			{
				// RHT 10/08/2011 - newline is always 1 char - stdio translates it on output on Windows
				int len = strlen(s->LastMessage);
				s->LastMessage = (char*) Realloc(s->LastMessage, len + strlen(Reply) + 2);
				strcpy(&(s->LastMessage)[len], "\n");
				strcpy(&(s->LastMessage)[len + 1], Reply);
			}
			else
			{
				s->LastMessage = Strdup(Reply);
			}
		}
		while (AnotherLineComing);
	}
	
	res.ReplyText = Strdup(Reply);
	res.ReplyCode = (int)atol(Reply);
	if (res.ReplyCode == 421 || res.ReplyCode == 530)
	{
		if ((res.ReplyCode == 421)										// Timed Out
			 && (strcmp(cmd,"USER") != 0) && (strcmp(cmd,"PASS") != 0)  // Ignore Login Failures
			 && (strcmp(cmd,"PASV") != 0) 								// or PASV attempts
			 && (strcmp(cmd,"AUTH") != 0)								// or FTPS AUTH attempts
			 && (strcmp(cmd,"QUIT") != 0))								// or QUIT attempts - RHT 17/07/2008
		{
			// Timed out or not logged in
			LogFTPTraffic("ERROR - Attempting to relogin\n");
#ifdef ENABLE_FTPS
																								//
			SetupConnection(s,(s->control.read_fn==FTPS_Read),(s->data.read_fn==FTPS_Read));	// re-establish connection
																								//
#else
			SetupConnection(s,false,false);
#endif
			if (s->control.trisocket)
			{
				FTP_REPLY x = FTP_LoginToServer(s,s->UserName,s->Password);
				if (x==FTP_OKAY)
				{
					// Logged in okay
					LogFTPTraffic("Logged back in okay\n");
					//
					// Switch back to last mode (ASCII or BINARY)
					//
					if (s->LastMode)
					{
						FTP_EnterBinaryMode(s);
					}
					else
					{
						FTP_EnterASCIIMode(s);
					}
					
					if (param && param[0])
					{
						LogFTPTraffic("reissuing last command (%s %s)\n",cmd,param);
					}
					else
					{
						LogFTPTraffic("reissuing last command (%s)\n",cmd);
					}

					Free(res.ReplyText);	// RHT 10/08/2011 - free previous reply
					res = FTP_SendCommandToServer(s,cmd,param);
				}
				s->Result = x;
			}
		}
	}

	Free(Reply);	// RHT 10/08/2011

	///if (s->LastMessage) Free(s->LastMessage);
	///s->LastMessage = Strdup(res.ReplyText);	// RHT 10/04/2008 - always save last message
	return res;
}

char *FTP_GetSystemType(FTPSOCKET *s)
{
	SERVERREPLY res;
	char *dir=(char *)0;

	res=FTP_SendCommandToServer(s,"SYST","");
	// VMS returns 200 - changed to accept and 2XX code as okay
	if (res.ReplyCode >= 200 && res.ReplyCode <= 299)
	{
		// Valid response - chop out the system type name
		// format is:
		// 215 UNIX Type: L8 Version: SunOS
		// or
		// 215 Windows_NT
		// or
		// 215  OS/400 is the remote....
		// or
		// 200 VMS ...
		//
		char *x = res.ReplyText;
		while (*x && *x!=' ') x++;
		while (*x==' ') x++;
		char *p=x;
		while (*p && *p!=' ' && *p!='\n' && *p!='\r') p++;
		*p='\0';
		dir=Strdup(x);
	}
	Free(res.ReplyText);	// RHT 10/08/2011
	return dir;
}

char *FTP_GetCurrentDirectory(FTPSOCKET *s)
{
	SERVERREPLY res;
	char *dir=(char *)0;

	if (s->SystemType == FTP_SYSTEM_AS400)
	{
		SERVERREPLY ignore = FTP_SendCommandToServer(s,"SITE NAMEFMT","1");
		Free(ignore.ReplyText);	// RHT 10/08/2011 - free immediately as not used
	}
	res=FTP_SendCommandToServer(s,"PWD","");
	if (res.ReplyCode == 257)
	{
		//res.ReplyText = "257 \"$2$DKA200:[FERDU01]\" is current directory.";    /// ### REMOVE ME
		//
		// Valid Response - chop out the name
		// format is 257 "dir" is current directory
		//
		char *x,*y,*sos;
		x = res.ReplyText; // RHT 10/08/2011 - already Strdup'ed was char *p; p=Strdup(res.ReplyText); x=p;
		sos=(char *)0;
		while (*x)
		{
			if (*x=='"')
			{
				if (!sos) sos=++x;	// start of string
				else
				{
					// end of string
					*x='\0';
					break;
				}
			}
			x++;
		}
		// Strip multiple separators
		x=sos;
		y=sos;

		bool inSep=false;
		while (*x)
		{
			if (*x=='/' || *x=='\\')
			{
				// Separator
				if (inSep)
				{
					x++;	// Already done a separator - skip it
				}
				else
				{
					inSep=true;
				}
			}
			else
			{
				inSep=false;
			}
			*y=*x;
			y++;
			x++;
		}
		//
		// Remove any trailing separators put on by some FTP servers...
		//
		*y='\0';
		int c=strlen(sos)-1;
		y--;
		while (c && (*y=='\\' || *y=='/'))
		{
			y--;
			c--;
		}
		*(y+1)='\0';

		// RHT 18/01/2011 - Remove any quotes put on by MVS
		if(s->SystemType == FTP_SYSTEM_MVS) {
			if(sos[0] == '\'') sos++;
			if(sos[strlen(sos)-1] == '\'') sos[strlen(sos)-1] = '\0';
		}

		dir=Strdup(sos);
		// RHT 10/08/2011 - don't need to Free(p);
	}
	Free(res.ReplyText);	// RHT 10/08/2011 - always free at end
	return dir;
}


// RHT 10/08/2011 - Absolutely not!
//void FTP_ExitWithError(char *ErrText)
//{
//	perror(ErrText);
//	exit(1);
//}




void FTP_EnterPassiveMode(FTPSOCKET *s)
{
	SERVERREPLY	t;
	char		*x;
	int			c;
	short		HighOrderByte;
	short		LowOrderByte;
	int			DataPort;
	short		n=0;
	t=FTP_SendCommandToServer(s,"PASV","");
	while (t.ReplyCode != 227 && n<2)
	{
		// Some servers throw this if they're not ready to
		// enter PASV mode. Let's delay for a second and try
		// again. If it still doesn't work, forget the whole
		// thing.
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		Free(t.ReplyText);	// RHT 10/08/2011 - free before overwrite
		t=FTP_SendCommandToServer(s,"PASV","");
		n++;
	}
	if (t.ReplyCode == 227)
	{
		/* Get port number from server's reply */
		/* FORMAT is 227 Entering Passive Mode (10,201,150,106,169,216) */

		/* Find first digit after initial space */
		x=t.ReplyText;
		while (*x!=' ') x++;
		while (*x<'0' || *x>'9') x++;
		/* Now find the fourth comma */
		for (c=1;c<=4;c++)
		{
			while (*x!=',') x++;
			x++;
		}
		HighOrderByte = (atol(x) & 255);
		while (*x!=',') x++;
		x++;
		LowOrderByte = (atol(x) & 255);
		DataPort=(HighOrderByte * 256) + LowOrderByte;
		s->DataPort=DataPort;
	}
	Free(t.ReplyText);	// RHT 10/08/2011
}

unsigned char *FTP_ReadDataFromServer(FTPCONNECTION *c,unsigned long *ReturnedLength)
{
	unsigned long CurrMaxSize=1024;
	unsigned long CurrSize=0;
	unsigned long dp=0;
	unsigned char *Reply=(unsigned char *)Malloc(CurrMaxSize);
	int n;
	char buffer[1024];
	do
	{
		n=(c->read_fn)(c,buffer,sizeof(buffer));
		CurrSize=CurrSize+n;
		if (CurrSize > CurrMaxSize)
		{
			CurrMaxSize = CurrSize;
			Reply=(unsigned char *)Realloc(Reply,CurrSize);
		}
		if (n>0)
		{
			memmove(&(Reply[dp]),buffer,n);
		}
		dp=dp+n;
	} while (n>0);
	*ReturnedLength=CurrSize;
	return Reply;
}

void FTP_CloseDataConnection(FTPCONNECTION *c)
{
	//LogFTPTraffic("Closing Data Connection\n");

#ifdef ENABLE_FTPS
	if (c->read_fn == FTPS_Read)
	{
		SSL_free((SSL*)c->pSsl);
		c->pSsl = NULL;
	}
#endif /*ENABLE_FTPS*/

	if (c->trisocket)
	{
		if (c->trisocket->sock)
		{
#ifdef WIN32
			closesocket(c->trisocket->sock);
#else
			close(c->trisocket->sock);
#endif
		}

		Free(c->trisocket);		// RHT 30/09/2011 - missing free - *should* be safe
		c->trisocket = NULL;
	}
}


bool CheckForTandemGuardian(FTPSOCKET *s)
{
	// Tandem platforms have two types of filesystems. The Guardian
	// filesystem which is peculiar to Tandem and the "Open Software
	// System" (OSS) which is UNIX-like. In order to determine which
	// filesystem type we're in, we'll do a PWD and see if the reply
	// does not begin with a '/'.  We default to true if we are unsure.
	//
	SERVERREPLY res;

	res=FTP_SendCommandToServer(s,"PWD","");
	if (res.ReplyCode == 257)
	{

		// Valid reply
		char *x=res.ReplyText;

		// Eat the '257 "' off the front
		while (*x && (isdigit(*x) || isspace(*x) || (*x == '\"'))) x++;

		// RHT 10/08/2011 - store result temporarily then free reply
		bool ret = (*x && (*x != '/')) ? true : false;
		Free(res.ReplyText);
		return ret;
	}

	return true;
}


bool CheckForTandemOSS(FTPSOCKET *s)
{
	// Tandem platforms have two types of filesystems. The Guardian
	// filesystem which is peculiar to Tandem and the "Open Software
	// System" (OSS) which is UNIX-like. In order to determine which
	// filesystem type we're in, we'll do a PWD and see if the reply
	// begins with a '/'
	//
	SERVERREPLY res;

	res=FTP_SendCommandToServer(s,"PWD","");
	if (res.ReplyCode == 257)
	{

		// Valid reply
		char *x=res.ReplyText;

		// Eat the '257 "' off the front
		while (*x && (isdigit(*x) || isspace(*x) || (*x == '\"'))) x++;

		// RHT 10/08/2011 - store result temporarily then free reply
		bool ret = (*x && (*x == '/')) ? true : false;
		Free(res.ReplyText);
		return ret;
	}

	return false;
}


static void LookupDirSepChar( FTPSOCKET *ftpserver )
{
	if (!FTP_System_Strings) { BuildFtpSystemStrings(); }

	// In order to be consistent we use the system type setting and look it up
	// in order to find the correct DirSepChar.  This means that the
	// DirSepChar's are only ever listed in one place.
	//
	for (int k=0; k<FTP_System_Strings_Count; k++)
	{
		if (FTP_System_Strings[k].SystemType == ftpserver->SystemType)
		{
			ftpserver->DirSepChar=FTP_System_Strings[k].DirSepChar;
		}
	}
}


// RHT 15/08/2011 - new fn to extract AS400 IASP names
// IASP names are of the form:
//
// /IASPNAME/QSYS.LIB/LIBRARY.LIB/FILE.FILE
//
char *getAS400IASP(char *szND)
{
	if(!szND || (szND[0] != '/'))
		return NULL;

	char *dup = Strdup(szND + 1);

	char *libbybit =  strstrnocase(dup, ".LIB");

	char *ret = NULL;

	if(libbybit) {
		// Start after the leading slash
		char *slash = NULL;
		for(char *p = dup; *p && (p < libbybit); p++) {
			if(*p == '/') {
				// If this is second slash, we've gone too far!
				if(slash) {
					slash = NULL;
					break;
				}
				slash = p;
			}
		}

		// If found, truncate string and return copy
		if(slash) {
			*slash = '\0';
			ret = Strdup(dup);
		}
	}

	Free(dup);

	return ret;
}


SERVERREPLY FTP_SwitchDirectory(FTPSOCKET *s,char *newdir)
{
	SERVERREPLY	t;
	// Remove any multiple dir separators as some FTP servers
	// are picky about this sort of thing...
	//
	char *nd=Strdup(newdir);
	int from=0;
	int to=0;
	int origlen=strlen(nd);
	bool SeenDirSep=false;
	while (from<origlen)
	{
		while (SeenDirSep && nd[from]==s->DirSepChar)
		{
			from++;
		}
		SeenDirSep=(nd[from]==s->DirSepChar);
		if (from > to)
		{
			nd[to]=nd[from];
		}
		to++;
		from++;
	}
	nd[to]='\0';
	char *szND=nd;
	//
	// Make sure all the slashes go forward: /
	//
	char *p=szND;
	while (*p) {
		if (*p=='\\') *p='/';
		p++;
	}
	//
	// VMS Servers don't like leading / characters. The code
	// *should* handle this properly but if we do it in here
	// then we're safe
	// RHT 18/01/2011 - also for MVS
	//
	if (s->SystemType == FTP_SYSTEM_VMS)
	{
		while (*szND=='/' || *szND=='\\') szND++;
	}
	else if(s->SystemType == FTP_SYSTEM_MVS)
	{
		char buf[1024];
		if(((szND[0] == '/') || (szND[0]=='\\')) && (szND[1]) && (szND[1]!='\'')) {
			sprintf(buf, "'%s'", &szND[1]);
			szND=Strdup(buf);
		} else if(szND[0]!='\'') {
			sprintf(buf, "'%s'", szND);
			szND=Strdup(buf);
		}
	}

	//
	// On Tandem if we are in Guardian mode and we get a request to switch to a
	// path beginning with '/' then we should try and switch to OSS mode first.
	// We must then verify that we have changed mode, and reset the system type
	// accordingly before issuing the CWD.
	//
	else if ((s->SystemType == FTP_SYSTEM_TANDEM_GUARDIAN) && (*szND == '/'))
	{
		// Try and select OSS as we know we are currently in Guardian mode
		t=FTP_SendCommandToServer(s,"OSS","");
		Free(t.ReplyText);	// RHT 10/08/2011 - free immediately
		if ((t.ReplyCode == 257) && CheckForTandemOSS(s))
		{
			// Actually the Tandem box supports OSS, so revert chnage system type.
			LogFTPTraffic("SYSTEM TYPE reset to TANDEM OSS\n");
			s->SystemType=FTP_SYSTEM_TANDEM_OSS;
			LookupDirSepChar( s );
		}
	}
	//
	// On Tandem if we are in OSS mode and we get a request to switch to a path
	// beginning with '\' or '$' then we should try and switch to Guardian mode
	// first. We must then verify that we have changed mode, and reset the
	// system type accordingly before issuing the CWD.
	//
	else if ((s->SystemType == FTP_SYSTEM_TANDEM_OSS) && ((*szND == '\\') || (*szND == '$')))
	{
		// Try and select OSS as we know we are currently in Guardian mode
		t=FTP_SendCommandToServer(s,"Guardian","");
		Free(t.ReplyText);	// RHT 10/08/2011 - free immediately
		if ((t.ReplyCode == 257) && CheckForTandemGuardian(s))
		{
			// Actually the Tandem box supports Guardian, so change system type.
			LogFTPTraffic("SYSTEM TYPE reset to TANDEM GUARDIAN\n");
			s->SystemType=FTP_SYSTEM_TANDEM_GUARDIAN;
			LookupDirSepChar( s );
		}
	}

	//
	// On AS400 we had stupid IASPs which look a bit like IFS filenames, but have
	// .LIBs on them and are actually native filesystems.  On cd set the flag
	// accordingly.
	//
	else if (s->SystemType == FTP_SYSTEM_AS400)
	{
		char *AS400IASP = getAS400IASP(szND);
		if(AS400IASP) {
			char *temp = Strdup(szND);

			// We want to cd to the bit after the IASP name
			szND += strlen(AS400IASP) + 1;

			// We do not want to cd into .FILEs in IASP mode
			char *dotfile = strstrnocase(szND, ".FILE");
			if(dotfile) {
				// Sanity check - we should never have more than one .FILE
				if(strstrnocase(dotfile+5, ".FILE")) {
					t.ReplyCode = 500;
					t.ReplyText = Strdup("recursive cd attempt thwarted");
					Free(nd);
					return t;
				}
				// Truncate at the / immediately preceding the .FILE
				for(char *p = dotfile; (p > szND); p--) {
					if (*p == '/') {
						*p = '\0';
						break;
					}
				}
			}

			LogFTPTraffic("IASP mode: ASP=[%s] Dir=[%s] Req=[%s]\n", AS400IASP, szND, temp);
			Free(temp);
		}
		// Has the ASP changed?
		if ((s->AS400IASP && !AS400IASP)
			|| (!s->AS400IASP && AS400IASP)
			|| (s->AS400IASP && AS400IASP && (strcmp(s->AS400IASP, AS400IASP) != 0)))
		{

			char buf[1024];
			sprintf(buf, "SETASPGRP ASPGRP(%s)", (AS400IASP ? AS400IASP: "*NONE"));
			t = FTP_SendCommandToServer(s ,"RCMD", buf);
			if ((t.ReplyCode < 200) || (t.ReplyCode > 299)) {
				Free(nd);
				return t;
			}
			s->AS400IASP = AS400IASP ? Strdup(AS400IASP) : NULL;
		}
		if(AS400IASP) Free(AS400IASP);
	}

	t=FTP_SendCommandToServer(s,"CWD",szND);	// RHT 10/08/2011 - don't free reply
	if (t.ReplyCode==250)
	{
		// CWD command okay
	}
	Free(nd);
	return t;
}



char *CopyField(char *StartPos)
{
	// Copies a string up until the first whitespace
	int n=0;
	char *x=StartPos;
	//
	// Find out how many characters we're talking about...
	//
	while (*x && *x!=' ' && *x!='\t')
	{
		x++;
		n++;
	}
	//
	// Grab the space...
	//
	char *res=(char *)Malloc(n+1);
	//
	// Copy the string...
	//
	for (int a=0;a<n;a++)
	{
		res[a]=StartPos[a];
	}
	//
	// NULL Terminate the string
	//
	res[n]='\0';
	//
	// And return it.
	//
	return res;
}


DATETIME DecodeFTPDate(char *DateString)
{
	DATETIME res = { 1973, 1, 1, -1, -1, -1 };
	time_t SecsSinceEpoch=time(NULL);
	struct tm *TimeNow = localtime(&SecsSinceEpoch);

	// Mar 28 14:40
	// Mar  1  2005
	// Mar 28 14:37
	// Mar  3  2005
	// Jun 23 10:59
	// 0123456789012
	// First 3 chars are the month abbreviation. Note, this will vary
	// dependent on local language settings.
	//
	char dc[13];

	// ### RHT - 12/09/2007
	if( !DateString )
		return res;

	strcpy(dc,DateString);
	dc[3]='\0';
	dc[6]='\0';
	char *szMonth = dc;
	int day = atol(&(dc[4]));
	if ((day >= 1) && (day <= 31)) { res.Day = day; }
	if (dc[9]==':')
	{
		// It's a time
		dc[9]='\0';
		res.Hours   = (short)atol(&(dc[7]));
		res.Minutes = (short)atol(&(dc[10]));
		res.Seconds = -1;
		res.Year    = (short)TimeNow->tm_year+1900;
	}
	else
	{
		// It's a year
		res.Hours   = -1;
		res.Minutes = -1;
		res.Seconds = -1;
		int year = atol(&(dc[7]));
		if (year != 0) { res.Year = year; }
	}
	//res.Month=-1;
	// RHT 07/09/2010 - change to use static table which we can populate from config
	for (int k=0;k<FTP_Month_Names_Count;k++)
	{
		// TODO: Should perhaps make case sensitivity turn off-onable.
		if (STRCASECMP(FTP_Month_Names[k].MonthName,szMonth)==0)
		{
			res.Month=FTP_Month_Names[k].MonthNumber;
			break;
		}
	}
	return res;
}

DATETIME DecodeVMSDate(char *DateString)
{
	DATETIME res;
	time_t SecsSinceEpoch=time(NULL);
	struct tm *TimeNow = localtime(&SecsSinceEpoch);

	// 01234567890123456789
	//  9-FEB-2007 10:26:39
	// 14-FEB-2007 17:06:52
	// First 3 chars are the month abbreviation. Note, this will vary
	// dependent on local language settings.
	//
	char dc[21];        // Was 13 - RHT - 22/03/2007
	strcpy(dc,DateString);

    int off = (dc[1] == '-') ? 0 : 1;
	dc[off+1]='\0';
	dc[off+5]='\0';
	dc[off+10]='\0';
	dc[off+13]='\0';
	dc[off+16]='\0';
	res.Day = (short)atol(dc);
	char *szMonth = &dc[off+2];     // Was off+3 - RHT - 22/03/2007
    res.Year=(short)atol(&(dc[off+6]));
	res.Hours=(short)atol(&(dc[off+11]));
	res.Minutes=(short)atol(&(dc[off+14]));
	res.Seconds=(short)atol(&(dc[off+17]));    // Was off+16 - RHT - 22/03/2007

	res.Month=-1;
	// RHT 07/09/2010 - change to use static table which we can populate from config
	for (int k=0;k<FTP_Month_Names_Count;k++)
	{
		if (STRCASECMP(FTP_Month_Names[k].MonthName,szMonth)==0)
		{
			res.Month=FTP_Month_Names[k].MonthNumber;
			break;
		}
	}
	return res;
}


// Added 10/03/08 RHT - Fix for broken Tandem Guardian dates
DATETIME DecodeTandemDate(char *DateString)
{
	DATETIME res;
	time_t SecsSinceEpoch=time(NULL);
	struct tm *TimeNow = localtime(&SecsSinceEpoch);

	// 01234567890123456789
	//  9-Feb-07 10:26:39
	// 14-Feb-07 17:06:52
	// First 3 chars are the month abbreviation. Note, this will vary
	// dependent on local language settings.
	//
	char dc[19];
	strcpy(dc,DateString);

    int off = (dc[1] == '-') ? 0 : 1;
	dc[off+1]='\0';
	dc[off+5]='\0';
	dc[off+8]='\0';
	dc[off+11]='\0';
	dc[off+14]='\0';
	res.Day = (short)atol(dc);
	char *szMonth = &dc[off+2];
    res.Year=(short)atol(&(dc[off+6]));
	res.Hours=(short)atol(&(dc[off+9]));
	res.Minutes=(short)atol(&(dc[off+12]));
	res.Seconds=(short)atol(&(dc[off+15]));
	res.Year += ((res.Year >= 73) ? 1900 : 2000);
	res.Month=-1;
	// RHT 07/09/2010 - change to use static table which we can populate from config
	for (int k=0;k<FTP_Month_Names_Count;k++)
	{
		if (STRCASECMP(FTP_Month_Names[k].MonthName,szMonth)==0)
		{
			res.Month=FTP_Month_Names[k].MonthNumber;
			break;
		}
	}
	return res;
}


// Added 18/01/11 RHT - MVS dates
DATETIME DecodeMVSDate(char *DateString)
{
	DATETIME res;
	//time_t SecsSinceEpoch=time(NULL);
	//struct tm *TimeNow = localtime(&SecsSinceEpoch);

	// 01234567890123456789
	// 2010/05/25 14:32
	//
	char dc[19];
	strcpy(dc,DateString);

	dc[4]='\0';
	dc[7]='\0';
	dc[10]='\0';
	dc[13]='\0';
    res.Year=(short)atol(dc);
	res.Month=(short)atol(&(dc[5]));
	res.Day = (short)atol(&(dc[8]));
	res.Hours=(short)atol(&(dc[11]));
	res.Minutes=(short)atol(&(dc[14]));
	res.Seconds=0;
	return res;
}


FTP_FILE_LIST *DecodeUnixListOutput(void *d,unsigned long NumBytes)
{
	FTP_FILE_LIST	*res;
	FTP_FILE_LIST	*curr;
	char		*a;
	char		*dirdata;
	char		*p;
	bool		InSpace;
	short		fieldnum;

	dirdata=(char *)Malloc(NumBytes+1);
	memmove(dirdata,d,NumBytes);
	dirdata[NumBytes]='\0';

	res=(FTP_FILE_LIST *)0;
	
	char *dp=dirdata;
	while (a=strtok(dp,"\r\n"))
	{
		dp=NULL;
		while (a && a[0]=='t' && a[1]=='o' && a[2]=='t') a=strtok(dp,"\r\n"); // skip past any "total" lines
		if (!a) break;	// exit if we've run out of lines....

		p=a;
		long	SizeInBytes=0;
		char	*ModificationDate=(char *)0;
		char	*Owner=(char *)0;
		char	*Group=(char *)0;
		char	*Attributes=(char *)Malloc(11);
		for (int z=0;z<=9;z++)
		{
			Attributes[z]=p[z];
		}
		Attributes[10]='\0';
		fieldnum=1;
		InSpace=false;
		while (*p)
		{
			if (*p==' ' && !InSpace)
			{
				InSpace=true;
			}
			if (*p!=' ' && InSpace)
			{
				fieldnum++;
				InSpace=false;
				if (fieldnum==3)
				{
					// Owner
					Owner=CopyField(p);
				}
				else
				if (fieldnum==4)
				{
					// Group
					Group=CopyField(p);
				}
				else
				if (fieldnum==5)
				{
					SizeInBytes=atol(p);
				}
				else
				if (fieldnum==6)
				{
					ModificationDate=(char *)Malloc(13);
					for (int i=0;i<=11;i++)
					{
						ModificationDate[i]=p[i];
					}
					ModificationDate[12]='\0';
				}
				if (fieldnum==9)
				{
					break;
				}
			}
			p++;
		}
		if (a[0]=='d')
		{
			// This is a directory - if the last char is a /
			// then it's a "dir" indication on the filename...
			// strip it off.
			//
			if (p[strlen(p)-1]=='/')
			{
				// In case filenames have trailing chars
				// to identify their types...
				//
				p[strlen(p)-1]='\0';
			}
		}
		else
		{
			// This is a file. If the last char is a * then it's
			// executable. Remove the *
			//
			if (p[strlen(p)-1]=='*')
			{
				p[strlen(p)-1]='\0';
			}
		}
		char *SymLink=strstr(p," ->");
		if (SymLink)
		{
			// Symbolic Link detected, remove the "-> target" text
			*SymLink='\0';
		}
		if (strcmp(p,".") && strcmp(p,".."))
		{
			if (res==(FTP_FILE_LIST *)0)
			{
				res=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
				curr=res;
			}
			else
			{
				curr->next=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
				curr=curr->next;
			}
			curr->next=(FTP_FILE_LIST *)0;
			curr->FileName=Strdup(p);
			curr->Directory=(a[0]=='d');
			curr->SizeInBytes=SizeInBytes;
			curr->szModifiedDate=ModificationDate;
			curr->ModifiedDate=DecodeFTPDate(ModificationDate);
			curr->Attributes=Attributes;
			curr->Owner=Owner;
			curr->Group=Group;
		}
	}
	Free(dirdata);
	return res;
}


// RHT 23/08/2011 - AS400 IASPs sometimes don't report any attributes - we need
// .FILEs set as directories - this is a specific routine to decode this output.
FTP_FILE_LIST *DecodeAS400IASPListOutput(void *d,unsigned long NumBytes)
{
	FTP_FILE_LIST	*res;
	FTP_FILE_LIST	*curr;
	char		*a;
	char		*dirdata;
	char		*p;
	bool		InSpace;
	short		fieldnum;

	dirdata=(char *)Malloc(NumBytes+1);
	memmove(dirdata,d,NumBytes);
	dirdata[NumBytes]='\0';

	res=(FTP_FILE_LIST *)0;
	
	char *dp=dirdata;
	while (a=strtok(dp,"\r\n"))
	{
		dp=NULL;
		while (a && a[0]=='t' && a[1]=='o' && a[2]=='t') a=strtok(dp,"\r\n"); // skip past any "total" lines
		if (!a) break;	// exit if we've run out of lines....

		p=a;
		long	SizeInBytes=0;
		char	*ModificationDate=(char *)0;
		char	*Owner=(char *)0;
		char	*Group=(char *)0;
		char	*Attributes=(char *)Malloc(11);
		for (int z=0;z<=9;z++)
		{
			Attributes[z]=p[z];
		}
		Attributes[10]='\0';
		fieldnum=1;
		InSpace=false;
		while (*p)
		{
			if (*p==' ' && !InSpace) {
				InSpace=true;
			}
			if (*p!=' ' && InSpace)
			{
				fieldnum++;
				InSpace=false;
				if (fieldnum==3) {
					// Owner
					Owner=CopyField(p);
				} else if (fieldnum==4) {
					// Group
					Group=CopyField(p);
				} else if (fieldnum==5) {
					SizeInBytes=atol(p);
				} else if (fieldnum==6) {
					ModificationDate=(char *)Malloc(13);
					for (int i=0;i<=11;i++) {
						ModificationDate[i]=p[i];
					}
					ModificationDate[12]='\0';
				}
				if (fieldnum==9) {
					break;
				}
			}
			p++;
		}
		if (a[0]=='d')
		{
			// This is a directory - if the last char is a /
			// then it's a "dir" indication on the filename...
			// strip it off.
			//
			if (p[strlen(p)-1]=='/') {
				// In case filenames have trailing chars
				// to identify their types...
				//
				p[strlen(p)-1]='\0';
			}
		}
		else
		{
			// This is a file. If the last char is a * then it's
			// executable. Remove the *
			//
			if (p[strlen(p)-1]=='*') {
				p[strlen(p)-1]='\0';
			}
		}
		char *SymLink=strstr(p," ->");
		if (SymLink) {
			// Symbolic Link detected, remove the "-> target" text
			*SymLink='\0';
		}
		if (strcmp(p,".") && strcmp(p,".."))
		{
			if (res==(FTP_FILE_LIST *)0) {
				res=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
				curr=res;
			} else {
				curr->next=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
				curr=curr->next;
			}
			curr->next=(FTP_FILE_LIST *)0;
			curr->FileName=Strdup(p);
			curr->Directory=(a[0]=='d');
			curr->SizeInBytes=SizeInBytes;
			curr->szModifiedDate=ModificationDate;
			curr->ModifiedDate=DecodeFTPDate(ModificationDate);
			curr->Attributes=Attributes;
			curr->Owner=Owner;
			curr->Group=Group;

			if((a[0]=='-') && (strcmp(Attributes, "----------") == 0) && strstrnocase(p, ".FILE")) {
				curr->Directory = true;;
			}
		}
	}
	Free(dirdata);
	return res;
}


// RHT 07/09/2010 - Function to check if a windows directory listing is in the
// optional unix format.  If it is, it will start with the permissions rather
// than the date, so we can look for the first character of this which will
// either be 'd' for a directory or '-' for a file.
bool IsUnixFormatListOutput(void *d,unsigned long NumBytes)
{
	if(NumBytes > 0)
	{
		const char *p = (const char*) d;
		if((p[0] == 'd') || (p[0] == '-')) {
			return true;
		}
	}

	return false;
}


FTP_FILE_LIST *DecodeWinListOutput(void *d,unsigned long NumBytes)
{
	FTP_FILE_LIST	*res;
	FTP_FILE_LIST	*curr;
	char		*a;
	char		*dirdata;
	char		*p;
	bool		InSpace;
	short		fieldnum;
	char		*tokPtr;

	dirdata=(char *)Malloc(NumBytes+1);
	memmove(dirdata,d,NumBytes);
	dirdata[NumBytes]='\0';

	res=(FTP_FILE_LIST *)0;
	tokPtr = dirdata;

	while (a=strtok(tokPtr,"\r\n"))
	{
		tokPtr=(char *)0;

		if (res==(FTP_FILE_LIST *)0)
		{
			res=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=res;
		}
		else
		{
			curr->next=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=curr->next;
		}
		
		curr->next=(FTP_FILE_LIST *)0;

		curr->Directory=(strstr(a,"<DIR>"))?true:false;

		// RHT 06/09/2010 - Must initialise in case this is a directory
		curr->SizeInBytes=0;

// 02-22-05  02:23PM       <DIR>          Copy of Folder2
// 02-22-05  02:19PM       <DIR>          Folder1
// 02-22-05  02:23PM       <DIR>          Folder2
// 02-22-05  02:24PM                    0 test.txt
// 07-05-06  11:03PM       <DIR>          abc
// 07-09-06  11:08PM                   28 file1.txt
// 012345678901234567

// RHT 07/09/2010 - IIS also has a long year format:
// 07-09-2006  11:08PM                 28 file1.txt
// 01234567890123456789

		p=a;
		//
		// Sort out date/time first
		//
		char *szDT=Strdup(p);
		// RHT 06/09/2010 - safety check
		if(strlen(szDT) > 17)
		{
			// RHT 07/09/2010 - offset for long year format
			int offset = ((szDT[8] != ' ') && (strlen(szDT) > 17)) ? 2 : 0;
			bool PM = (szDT[15+offset]=='P') ? true : false;
			szDT[17+offset]='\0';
			curr->szModifiedDate=Strdup(szDT);
			szDT[2]='\0';
			szDT[5]='\0';
			szDT[8+offset]='\0';
			szDT[12+offset]='\0';
			// RHT 07/09/2010 - swap day and month as IIS seems to be fixed in US format
			curr->ModifiedDate.Month=(short)atol(&(szDT[0]));
			curr->ModifiedDate.Day=(short)atol(&(szDT[3]));
			curr->ModifiedDate.Year=(short)atol(&(szDT[6]));
			if (offset == 0) {
				curr->ModifiedDate.Year += (curr->ModifiedDate.Year>50) ? 1900 : 2000;
			}
			// RHT 06/09/2010 - For some reason adding 12 to the result of atol() fails!
			// This cludge ensures it is properly converted before we do the addition.
			long temp = atol(&szDT[10+offset]);
			short temp2 = (short) temp;
			short temp3 = temp2 + ((PM && (temp2 != 12)) ? 12 : 0);
			curr->ModifiedDate.Hours = temp3;
			curr->ModifiedDate.Minutes=(short)atol(&szDT[13+offset]);
			Free(szDT);
		} else {
			curr->szModifiedDate="";
			curr->ModifiedDate.Day = 1;
			curr->ModifiedDate.Month = 1;
			curr->ModifiedDate.Year = 1970;
			curr->ModifiedDate.Hours = 0;
			curr->ModifiedDate.Minutes = 0;
		}
		curr->ModifiedDate.Seconds=0;
		fieldnum=1;
		InSpace=false;
		while (*p)
		{
			if (*p==' ' && !InSpace)
			{
				InSpace=true;
			}
			if (*p!=' ' && InSpace)
			{
				fieldnum++;
				InSpace=false;
				if (fieldnum==3 && !curr->Directory)
				{
					curr->SizeInBytes=atol(p);
				}
				else
				if (fieldnum==4)
				{
					// Found Name
					break;
				}
			}
			p++;
		}
		curr->Attributes=Strdup("");
		curr->Group=Strdup("");
		curr->Owner=Strdup("");
		curr->FileName=Strdup(p);
	}
	Free(dirdata);
	return res;
}

FTP_FILE_LIST *DecodeVMSListOutput(void *d, unsigned long NumBytes, bool showAllVersions)
{
	FTP_FILE_LIST	*res;
	FTP_FILE_LIST	*curr;
	char		*a;
	char		*dirdata;
	char		*p;
	bool		InSpace;
	short		fieldnum;
	char		szLastFileName[512];

	szLastFileName[0]='\0';

	dirdata=(char *)Malloc(NumBytes+1);
	memmove(dirdata,d,NumBytes);
	dirdata[NumBytes]='\0';

	res=(FTP_FILE_LIST *)0;
	
    char bigbuffer[1024];
	char *dp=dirdata;
	while (a=strtok(dp,"\r\n"))
	{
		dp=NULL;

        if( (strstr(a,"Directory ") == a)
            || (strstr(a,"Total of ") == a) )
        {
            // Skip these lines
            continue;
        }

        while( a[strlen(a)-1] != ')' )
        {
            char *b = strtok(NULL,"\r\n");
            a = Strdup(a);
            sprintf( bigbuffer, "%s%s", a, b );
            Free(a);
            a = bigbuffer;
        }

		p=a;

		long	SizeInBytes=0;
		char	*Filename=(char *)0;
        bool    Directory = false;
		char	*Date=(char *)0;
		char	*Time=(char *)0;
		char	*Owner=(char *)0;
		char	*Group=(char *)0;
		char	*Attributes=(char *)Malloc(11);

        fieldnum=0;
		InSpace=true;
		while (*p)
		{
			if (*p==' ' && !InSpace)
			{
				InSpace=true;
			}
			if (*p!=' ' && InSpace)
			{
				fieldnum++;
				InSpace=false;
			    if (fieldnum==1)
			    {
				    // Filename
                    Filename = CopyField( p );

                    if( (Filename[0] == '\0')
                        || (strcmp(Filename,"Directory") == 0)
                        || (strcmp(Filename,"Total") == 0) )
                    {
                        Free( Filename );
                        Filename = (char *)0;
                        break;
                    }

                    if( strstr(Filename,".DIR") )
                        Directory = true;

					if (Filename && !Directory)
					{
						//
						// Strip the version number off
						//
						char *szFileName = Strdup(Filename);
						char *szVN = szFileName;
						while (*szVN && *szVN != ';') szVN++;
						if (*szVN==';')
						{
							// Found version number
							*szVN='\0';
						}
						if (!showAllVersions && strcmp(szFileName,szLastFileName)==0)
						{
							// This is the same filename as last time - must be
							// just an earlier version. Skip this by breaking out
							// of the line-processing loop. This will leave "p"
							// set to a non-zero value so the outer loop will
							// then continue to read the next line.
							//
							break;
						}
						else
						{
							strncpy(szLastFileName,szFileName,sizeof(szLastFileName));
							szLastFileName[sizeof(szLastFileName)-1]='\0';
						}
						Free(szFileName);
					}
                }
                else
				if (fieldnum==2)
				{
                    // Size
                    char *blocks = CopyField(p);
                    /// ### LogFTPTraffic( "blocks = \"%s\"\n", blocks ? blocks : "(null)" );
                    long BLOCK_SIZE = 512;
					SizeInBytes=atol(p)*BLOCK_SIZE;
				}
				else
				if (fieldnum==3)
				{
                    // Modification Date
                    Date = CopyField(p);
                    /// ### LogFTPTraffic( "Date = \"%s\"\n", Date ? Date : "(null)" );
                }
				else
				if (fieldnum==4)
				{
                    // Modification Time
                    Time = CopyField(p);
                    /// ### LogFTPTraffic( "Time = \"%s\"\n", Time ? Time : "(null)" );
				}
				else
				if (fieldnum==5)
				{
					// Owner
                    if( *p == '[' ) {
                        p++;
					    Owner=CopyField(p);
                        if( Owner[0] != '\0' ) {
                            Owner[strlen(Owner)-1]='\0';
                        }
                    } else {
					    Owner=CopyField(p);
                    }
                    /// ### LogFTPTraffic( "Owner = \"%s\"\n", Owner ? Owner : "(null)" );
				}
				else
				if (fieldnum==6)
				{
					// Attributes
                    if( *p == '(' ) {
                        p++;
					    Attributes=CopyField(p);
                        if( Attributes[0] != '\0' ) {
                            Attributes[strlen(Attributes)-1]='\0';
                        }
                   } else {
					    Attributes=CopyField(p);
                   }
                   /// ### LogFTPTraffic( "Attributes = \"%s\"\n", Attributes ? Attributes : "(null)" );
				}
				else
				if (fieldnum==7)
				{
					break;
				}
			}
			p++;
		}

		if (*p) continue;	// must have broken out of loop due to duplicate filename


		if (res==(FTP_FILE_LIST *)0)
		{
			res=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=res;
		}
		else
		{
			curr->next=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=curr->next;
		}
		curr->next=(FTP_FILE_LIST *)0;
		curr->FileName=Filename;
		curr->Directory=Directory;
		curr->SizeInBytes=SizeInBytes;
        char *ModificationDate = (char*) Malloc( strlen(Date) + strlen(Time) + 2 );
        sprintf( ModificationDate, "%s %s", Date, Time );
        Free( Date );
        Free( Time );
		curr->szModifiedDate=ModificationDate;
		curr->ModifiedDate=DecodeVMSDate(ModificationDate);
		curr->Attributes=Attributes;
		curr->Owner=Owner;
		curr->Group=Group;

	}
	Free(dirdata);
	return res;
}


FTP_FILE_LIST *DecodeTandemListOutput(void *d,unsigned long NumBytes)
{
	FTP_FILE_LIST	*res;
	FTP_FILE_LIST	*curr;
	char		*a;
	char		*dirdata;
	char		*p;
	bool		InSpace;
	short		fieldnum;

	dirdata=(char *)Malloc(NumBytes+1);
	memmove(dirdata,d,NumBytes);
	dirdata[NumBytes]='\0';

	res=(FTP_FILE_LIST *)0;
	
	char *dp=dirdata;
	while (a=strtok(dp,"\r\n"))
	{
		dp=NULL;

        if(strstr(a,"File ") == a)
        {
            // Skip these lines
            continue;
        }

		while (a && a[0]=='t' && a[1]=='o' && a[2]=='t') a=strtok(dp,"\r\n"); // skip past any "total" lines
		if (!a) break;	// exit if we've run out of lines....

		// Paths have the form $diskdrive.subvolume.filename
		// e.g. \EXTRA.$SYSTEM.NOSUBVOL
		//
		// Directory listings have the form:
		//
		// CDQHREAD 101 2662 19-Apr-01 12:00:17 210, 43 "ncnc"
		//
		// filename filecode size date time group,user atrributes
		//
		// group 255 is super group; user 255 is group manager; 255,255 is super id
		//
		// attributes appear to read, write, execute and progig, where:
		//  - = super id; u = user; c = group; n = any user; o = local owner; a = local any user
		//

		p=a;
		char	*FileName=(char *)0;
		// ### RHT 12/11/2007 - long	FileCode=0;
		long	SizeInBytes=0;
		char	*Date=(char *)0;
		char	*Time=(char *)0;
		char	*Owner=(char *)0;
		char	*Group=(char *)0;
		char	*Attributes=(char *)0;
		fieldnum=1;
		InSpace=false;

		// FileName
		FileName=CopyField(p);
        // LogFTPTraffic( "FileName = \"%s\"\n", FileName ? FileName : "(null)" );

		while (*p)
		{
			if (*p==' ' && !InSpace)
			{
				InSpace=true;
			}
			if (*p!=' ' && InSpace)
			{
				fieldnum++;
				InSpace=false;
				if (fieldnum==2)
				{
					// File Code
					// ### RHT - 12/11/2007 - FileCode=atol(p);
                    // LogFTPTraffic( "FileCode = %d\n",FileCode );
					Group = CopyField(p);
				}
				else if (fieldnum==3)
				{
					SizeInBytes=atol(p);
                    // LogFTPTraffic( "SizeInBytes = %d\n",SizeInBytes );
				}
				else if (fieldnum==4)
				{
                    // Modification Date
                    Date = CopyField(p);
                    // LogFTPTraffic( "Date = \"%s\"\n", Date ? Date : "(null)" );
                }
				else if (fieldnum==5)
				{
                    // Modification Time
                    Time = CopyField(p);
                    // LogFTPTraffic( "Time = \"%s\"\n", Time ? Time : "(null)" );
				}
				else if (fieldnum==6)
				{
					// Group, Owner "Attributes"
					char *temp = Strdup(p);
					char *x;
					for (x = temp; *x && isdigit(*x); x++) { /*empty loop - find end of digits*/ }
					// ### RHT - 12/11/2007 - leave Group and Owner combined and store in owner
					///if (*x == ',') { *x++ = '\0'; Group = Strdup(temp); temp = x; }
					///for ( ; *temp && (*temp == ' '); temp++ ) { /*empty loop - skip spaces*/ }
					///for (x = temp; *x && isdigit(*x); x++) { /*empty loop - find end of digits*/ }
					if (*x == ',') { x++; }
					for ( ; *x && (*x == ' '); x++ ) { /*empty loop - skip spaces*/ }
					for ( ; *x && isdigit(*x); x++) { /*empty loop - find end of digits*/ }
					// ### RHT - rest of code okay
					if (*x == ' ') { *x++ = '\0'; Owner = Strdup(temp); temp = x; }
					for ( ; *temp && (*temp == ' '); temp++ ) { /*empty loop - skip spaces*/ }
					if (*temp == '\"')
					{
						temp++;
						Attributes = Strdup(temp);
						if( Attributes[strlen(Attributes)-1] == '\"' )
							Attributes[strlen(Attributes)-1] = '\0'; 
					}
					else
						Attributes = Strdup(temp);

                    // LogFTPTraffic( "Group = \"%s\"\n", Group ? Group : "(null)" );
                    // LogFTPTraffic( "Owner = \"%s\"\n", Owner ? Owner : "(null)" );
                    // LogFTPTraffic( "Attributes = \"%s\"\n", Attributes ? Attributes : "(null)" );

					//Free(temp); temp does not point to the start of the string any more!
				}
				else if (fieldnum==9)
				{
					break;
				}
			}
			p++;
		}

		if (res==(FTP_FILE_LIST *)0)
		{
			res=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=res;
		}
		else
		{
			curr->next=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=curr->next;
		}
		curr->next=(FTP_FILE_LIST *)0;
		curr->FileName=FileName;
		curr->Directory=false; //(a[0]=='d');
		curr->SizeInBytes=SizeInBytes;
        char *ModificationDate = (char*) Malloc( strlen(Date) + strlen(Time) + 2 );
        sprintf( ModificationDate, "%s %s", Date, Time );
        Free( Date );
        Free( Time );
		curr->szModifiedDate=ModificationDate;
		curr->ModifiedDate=DecodeTandemDate(ModificationDate);
		curr->Attributes=Attributes;
		curr->Owner=Owner;
		curr->Group=Group;
	}
	Free(dirdata);
	return res;
}


FTP_FILE_LIST *DecodeMVSListOutput(void *d,unsigned long NumBytes)
{
	FTP_FILE_LIST	*res;
	FTP_FILE_LIST	*curr;
	char		*a;
	char		*dirdata;
	char		*p;
	bool		InSpace;
	short		fieldnum;
	bool		isDir = false;


	dirdata=(char *)Malloc(NumBytes+1);
	memmove(dirdata,d,NumBytes);
	dirdata[NumBytes]='\0';

	res=(FTP_FILE_LIST *)0;

	char *dp=dirdata;
	while (a=strtok(dp,"\r\n"))
	{
		dp=NULL;

		while (a && a[0]=='V' && a[1]=='o' && a[2]=='l') { isDir = true; a=strtok(dp,"\r\n"); } // skip past any "Volume" lines
		while (a && a[0]=='N' && a[1]=='a' && a[2]=='m') a=strtok(dp,"\r\n"); // skip past any "Name" lines
		while (a && a[0]==' ' && a[1]=='N' && a[2]=='a' && a[3]=='m') a=strtok(dp,"\r\n"); // skip past any "Name" lines
		if (!a) break;	// exit if we've run out of lines....

		// LogFTPTraffic( "dir: %s\n", a ? a : "(null)" );

		p=a;
		char	*FileName=(char *)0;
		long	SizeInBytes=0;
		char	*Date=(char *)0;
		char	*Time=(char *)0;
		char	*Owner="user"; //(char *)0;
		char	*Group="group"; //(char *)0;
		char	*Attributes="rwx"; //(char *)0;
		fieldnum=1;
		InSpace=false;

		// FileName
		if(isDir) {
			FileName=CopyField((p+56));
			// LogFTPTraffic( "FileName = \"%s\"\n", FileName ? FileName : "(null)" );
		} else {
			FileName=CopyField(p);
			// LogFTPTraffic( "FileName = \"%s\"\n", FileName ? FileName : "(null)" );

			while (*p)
			{
				if (*p==' ' && !InSpace)
				{
					InSpace=true;
				}
				if (*p!=' ' && InSpace)
				{
					fieldnum++;
					InSpace=false;
					if (fieldnum == 2)
					{
						// VV.MM
					}
					else if (fieldnum == 3)
					{
						// Created
					}
					else if (fieldnum == 4)
					{
						// Changed Date
						Date = CopyField(p);
					}
					else if (fieldnum == 5)
					{
						// Changed Time
						Time = CopyField(p);
					}
					else if (fieldnum == 6)
					{
						// Size
						SizeInBytes=atol(p);
					}
				}
				p++;
			}
		}

		if (res==(FTP_FILE_LIST *)0)
		{
			res=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=res;
		}
		else
		{
			curr->next=(FTP_FILE_LIST *)Malloc(sizeof(FTP_FILE_LIST));
			curr=curr->next;
		}
		curr->next=(FTP_FILE_LIST *)0;
		curr->FileName=FileName;
		curr->Directory=isDir;
		curr->SizeInBytes=SizeInBytes;
		if(Date && Time) {
			char *ModificationDate = (char*) Malloc( strlen(Date) + strlen(Time) + 2 );
			sprintf( ModificationDate, "%s %s", Date, Time );
			Free( Date );
			Free( Time );
			curr->szModifiedDate=ModificationDate;
			curr->ModifiedDate=DecodeMVSDate(ModificationDate);
		} else {
			curr->szModifiedDate="";
			curr->ModifiedDate=DecodeFTPDate("Jan 01 12:34");
		}
		curr->Attributes=Attributes;
		curr->Owner=Owner;
		curr->Group=Group;
	}
	Free(dirdata);

	return res;
}


FTP_FILE_LIST *FTP_GetDirectoryContents(FTPSOCKET *s,char *Pattern /* = (char *)0 */)
{
	SERVERREPLY	t;
	void		*dirdata;
	unsigned long	dirdatalen;
	FTP_FILE_LIST	*res;

	res = (FTP_FILE_LIST *)0;

	FTP_EnterPassiveMode(s);
	s->data.trisocket=EstablishOutgoingConnection(s->DataPort,s->HostName);
	if (!s->data.trisocket)
	{
		LogFTPTraffic(s,"Failed to connect to data connection");	// RHT 10/08/2011 - was FTP_ExitWithError
		return res;
	}
	else
	{
		// printf("Data connection established:\n");
		// printf("DataPort=[%d] HostName=[%s]\n",s->DataPort,s->HostName);
	}

	if (s->SystemType == FTP_SYSTEM_AS400)
	{
		// Switch iSeries boxes into List Format 1 (Unix Format)
		t=FTP_SendCommandToServer(s,"SITE LISTFMT","1");
		Free(t.ReplyText);	// RHT 10/08/2011 - free immediately
		// printf("Setting iSeries ListFormat 1 returned code %d\n",t.ReplyCode);
	}

	t=FTP_SendCommandToServer(s,"LIST",Pattern?Pattern:"");
	Free(t.ReplyText);	// RHT 10/08/2011 - free immediately

	// RHT 07/09/2010 - Change from 150 and 125 to all 1xx codes
	if ((t.ReplyCode >= 100) && (t.ReplyCode <= 199))
	{
#ifdef ENABLE_FTPS
		FTPS_DataConnect(s, &s->data);
#endif /*ENABLE_FTPS*/

		// printf("About to call FTP_ReadDataFromServer()\n");
		dirdata = FTP_ReadDataFromServer(&s->data,&dirdatalen);
		LogFTPBuffer((char*) dirdata, dirdatalen, FTP_DUMP_DIRECTORY_LISTINGS);	// RHT 05/08/2011 - log directory listing
		// write(1,dirdata,dirdatalen);
		// printf("DONE\n");
		switch (s->SystemType)
		{
			case FTP_SYSTEM_UNIX:
			case FTP_SYSTEM_TANDEM_OSS:
				res = DecodeUnixListOutput(dirdata,dirdatalen);
				break;
			case FTP_SYSTEM_AS400:	// RHT 23/08/2011 - use alternate decoder on AS400 IASPs
				if(s->AS400IASP) {
					res = DecodeAS400IASPListOutput(dirdata,dirdatalen);
				} else {
					res = DecodeUnixListOutput(dirdata,dirdatalen);
				}
				break;
			case FTP_SYSTEM_WIN32:
				// RHT 07/09/2010 - Allow us to handle optional unix format IIS listings
				if(IsUnixFormatListOutput(dirdata,dirdatalen)) {
					res = DecodeUnixListOutput(dirdata,dirdatalen);
				} else {
					res = DecodeWinListOutput(dirdata,dirdatalen);
				}
				break;
			case FTP_SYSTEM_VMS:
				res = DecodeVMSListOutput(dirdata,dirdatalen,false);
				break;
			case FTP_SYSTEM_TANDEM_GUARDIAN:
				res = DecodeTandemListOutput(dirdata,dirdatalen);
				break;
			case FTP_SYSTEM_MVS:
				res = DecodeMVSListOutput(dirdata,dirdatalen);
				break;
			default:
				LogFTPTraffic(s,"WARNING: Directory listing not understood\n");
				break;
		}
		FTP_CloseDataConnection(&s->data);
		char *post = FTP_ReadLineFromServer(&s->control);	// RHT 29/08/2009 - log post reply
		LogFTPTraffic(s,"<--- %s\n",post ? post : "(null)");
		Free(post);		// RHT 10/08/2011
	}
	// FTP_CloseDataConnection(d);
	// printf("\n\nRETURNING:\n");
	// FTP_FILE_LIST *x=res;
	// while (x)
	// {
	//	printf("%s (dir=%s)\n",x->FileName,x->Directory?"true":"false");
	//	x=x->next;
	// }
	return res;
}

bool CheckForIFS(FTPSOCKET *s)
{
	// iSeries platforms have two types of filesystems. A "native"
	// filesystem consisting of libraries/files/members and an
	// "Integrated File System" (IFS) which is UNIX-like. In order
	// to determine which file system type we're in, we'll do a PWD
	// and see if the reply ends in "library" or "directory"
	//
	bool IFS=false;
	SERVERREPLY res;

	res=FTP_SendCommandToServer(s,"PWD","");
	if (res.ReplyCode == 257)
	{
		// Valid reply
		int qc=0;	// Quote Count
		char *x=res.ReplyText;
		while (*x)
		{
			if (*x=='"')
			{
				qc++;
				if (qc>1)
				{
					// 2nd quote - check rest of string
					if (strstr(x,"library"))
					{
						IFS=false;
					}
					else
					{
						//
						// PAG mod for #307 - a .FILE in a .LIB will show as "directory"
						//
						if (strstr(res.ReplyText,".LIB/"))
						{
							IFS=false;
						}
						else
						{
							IFS=true;
						}
					}
					break;
				}
			}
			x++;
		}
	}
	Free(res.ReplyText);	// RHT 10/08/2011
	return IFS;
}


SERVERREPLY FTP_MakeDirectory(FTPSOCKET *s,char *DirName)
{
	SERVERREPLY	t;
	char		*ParentDir;
	t.ReplyText = NULL;	// RHT 10/08/2011 - initialise to null

	if (s->SystemType == FTP_SYSTEM_AS400)
	{
		bool IFS=CheckForIFS(s);
		bool CreatingLibrary;
		int n=strlen(DirName);
		if (n>4)
		{
			if (	DirName[n-4]=='.' &&
				DirName[n-3]=='L' &&
				DirName[n-2]=='I' &&
				DirName[n-1]=='B')
			{
				CreatingLibrary=TRUE;
			}
			else
			{
				CreatingLibrary=FALSE;
			}
		}
		else
		{
			CreatingLibrary=FALSE;
		}
		if (IFS || CreatingLibrary)
		{
			t=FTP_SendCommandToServer(s,"MKD",DirName);
		}
		else
		{
			//
			// iSeries "FILE" command needs to be in the
			// library/file where the new file needs to be
			//
			char *CurrentDirectory=FTP_GetCurrentDirectory(s);
			ParentDir=Strdup(DirName);
			int x=strlen(ParentDir);
			while (x)
			{
				if (ParentDir[x]=='/')
				{
					ParentDir[x]='\0';
					break;
				}
				x--;
			}
			t=FTP_SwitchDirectory(s,ParentDir);
			Free(ParentDir);
			if (t.ReplyCode==250)
			{
				// Switch Directory okay
				char *iSeriesFileName=(char *)Malloc(strlen(DirName)+20);
				int n=strlen(DirName);
				char *szName;
				while (n)
				{
					if (DirName[n]=='/') break;
					szName=&(DirName[n]);
					n--;
				}
				sprintf(iSeriesFileName,"FILE(%s",szName);
				//
				// Get rid of the file extension (Probably .FILE)
				//
				int y=strlen(iSeriesFileName);
				while (y)
				{
					if (iSeriesFileName[y]=='.')
					{
						iSeriesFileName[y]='\0';
						break;
					}
					y--;
				}
				strcat(iSeriesFileName,")");
				Free(t.ReplyText);	// RHT 10/08/2011 - free previous reply
				t=FTP_SendCommandToServer(s,"CRTS",iSeriesFileName);
				Free(iSeriesFileName);
				if (t.ReplyCode==250)
				{
					// Successful FILE creation - set reply code to 257
					// to indicate success
					t.ReplyCode=257;
				}
			}
			else
			{
				// We couldn't switch directory - pretend it's a 550 reply
				// to force the logic to go up a level
				//
				t.ReplyCode=550;
			}
		}
	}
	else
	{
		t=FTP_SendCommandToServer(s,"MKD",DirName);
	}
	//
	// If an element of the path doesn't exist, the reply
	// should be 550 (PATH DOES NOT EXIST, FILE NOT FOUND)
	// However, some servers insist on returning a generic
	// 521 error (usually "Access Denied" although there
	// appears to be no official documentation for this
	// code). The problem here is that Sun servers return
	// 521 meaning "Directory Already Exists". So we cannot
	// actually tell if we're getting an error because we're
	// already at a directory level that exists or because
	// the path doesn't exist. To work around this deeply
	// irritating problem we'll try and switch to the
	// directory above this one and only recurse if that
	// doesn't exist.
	//

	if (t.ReplyCode == 550 || t.ReplyCode == 521)
	{
		// Need to go back a level
		char *CurrentDir=Strdup(DirName);

		long k=strlen(CurrentDir)-1;
		while (k>=0)
		{
			if (CurrentDir[k]==s->DirSepChar)
			{
				if (s->SystemType == FTP_SYSTEM_VMS)
				{
					//
					// Terminate the directory list properly for VMS
					//
					CurrentDir[k]=']';
					CurrentDir[k+1]='\0';
				}
				else
				{
					CurrentDir[k]='\0';
				}
				break;
			}
			k--;
		}
		if (k>0)
		{
			SERVERREPLY c = FTP_SwitchDirectory(s,CurrentDir);
			Free(c.ReplyText);	// RHT 10/08/2011 - free immediately
			if (c.ReplyCode == 250)
			{
				//
				// Parent directory exists. Do nothing. We will
				// then drop out at this point, returning the
				// original reply code from the MKD command...
				//
			}
			else
			{
				//
				// Recurse, creating the directory above this one
				//
				Free(t.ReplyText);	// RHT 10/08/2011 - free previous reply
				t = FTP_MakeDirectory(s,CurrentDir);
			}
		}

		if (s->SystemType == FTP_SYSTEM_AS400)
		{
			bool IFS=CheckForIFS(s);
			bool CreatingLibrary;
			int n=strlen(DirName);
			if (n>4)
			{
				if (	DirName[n-4]=='.' &&
					DirName[n-3]=='L' &&
					DirName[n-2]=='I' &&
					DirName[n-1]=='B')
				{
					CreatingLibrary=TRUE;
				}
				else
				{
					CreatingLibrary=FALSE;
				}
			}
			else
			{
				CreatingLibrary=FALSE;
			}
			LogFTPTraffic("IFS=%s\n",IFS?"true":"false");
			char *CurrentDirectory=FTP_GetCurrentDirectory(s);
			if (IFS || CreatingLibrary)
			{
				Free(t.ReplyText);	// RHT 10/08/2011 - free before overwrite
				t=FTP_SendCommandToServer(s,"MKD",DirName);
			}
			else
			{
				//
				// iSeries "FILE" command needs to be in the library/file
				// where the new file needs to be
				//
				ParentDir=Strdup(DirName);
				int x=strlen(ParentDir);
				while (x)
				{
					if (ParentDir[x]=='/')
					{
						ParentDir[x]='\0';
						break;
					}
					x--;
				}
				Free(t.ReplyText);	// RHT 10/08/2011 - free before overwrite
				t=FTP_SwitchDirectory(s,ParentDir);
				Free(ParentDir);
				if (t.ReplyCode==250)
				{
					// Switch Directory okay
					char *iSeriesFileName=(char *)Malloc(strlen(DirName)+20);
					int n=strlen(DirName);
					char *szName;
					while (n)
					{
						if (DirName[n]=='/') break;
						szName=&(DirName[n]);
						n--;
					}
					sprintf(iSeriesFileName,"FILE(%s",DirName);
					//
					// Get rid of the file extension (Probably .FILE)
					//
					int y=strlen(iSeriesFileName);
					while (y)
					{
						if (iSeriesFileName[y]=='.')
						{
							iSeriesFileName[y]='\0';
							break;
						}
						y--;
					}
					strcat(iSeriesFileName,")");
					Free(t.ReplyText);	// RHT 10/08/2011 - free before overwrite
					t=FTP_SendCommandToServer(s,"CRTS",iSeriesFileName);
					Free(iSeriesFileName);
					if (t.ReplyCode==250)
					{
						// Successful FILE creation - set reply code to 257
						// to indicate success
						t.ReplyCode=257;
					}
				}
				else
				{
					// We couldn't switch directory - pretend it's a 550 reply
					// to force the logic to go up a level
					//
					t.ReplyCode=550;
				}
			}
		}
		else
		{
			Free(t.ReplyText);	// RHT 10/08/2011 - free before overwrite
			t=FTP_SendCommandToServer(s,"MKD",DirName);
		}
		Free(CurrentDir);
	}
	return t;
}

SERVERREPLY FTP_GetFileData(FTPSOCKET *s,bool TextFile,char *FileName,unsigned char **FileData,unsigned long *DataLen)
{
	SERVERREPLY	t;

	if (TextFile)
	{
		FTP_EnterASCIIMode(s);
	}
	else
	{
		FTP_EnterBinaryMode(s);
	}

	FTP_EnterPassiveMode(s);

	s->data.trisocket=EstablishOutgoingConnection(s->DataPort,s->HostName);
	if (!s->data.trisocket) LogFTPTraffic("Failed to connect to data connection");

	t=FTP_SendCommandToServer(s,"RETR",FileName);
	
	// RHT 06/09/2010 - #396 - Changed to accept 1xx as IIS returns 125 rather than 150
	if ((t.ReplyCode >= 100) && (t.ReplyCode <= 199))
	{
#ifdef ENABLE_FTPS
		FTPS_DataConnect(s, &s->data);
#endif /*ENABLE_FTPS*/

		*FileData = FTP_ReadDataFromServer(&s->data,DataLen);
		// LogFTPTraffic("Read %ld bytes of file data\n",*DataLen);

		///TRIDEBUG(("FileData=[%s]",*FileData));
		///for (int p=0;p<*DataLen;p++)
		///{
		///	TRIDEBUG(("%d) %02x ",p,(*FileData)[p]));
		///}
		//
		// Now close the connection
		//
		FTP_CloseDataConnection(&s->data);
		bool MoreToRead=false;
		do
		{
			char *res=FTP_ReadLineFromServer(&s->control);
			LogFTPTraffic(s,"<--- %s\n",res);	// RHT 15/07/2008 - Log hostname
			int rcode=atol(res);
			if (rcode)
			{
				//
				// Line begins with a numeric code
				//
				char *x=res;
				while (*x && (*x<'0' || *x>'9')) x++; // Find first digit
				while (*x && *x!=' ' && *x!='-') x++; // Find end of digits
				if (*x=='-')
				{
					MoreToRead=true;
				}
				else
				{
					MoreToRead=false;
				}
			}
			else
			{
				//
				// Line does NOT begin with a numeric code - assume continuation line and carry on
				//
				MoreToRead=true;
			}
			Free(res);	// RHT 10/08/2011
		} while (MoreToRead);
		//
		// Now read the "transfer complete" message (which is 226)
		//
		// printf("Awaiting transfer complete message from server...\n");
		// char *ServerLine = FTP_ReadLineFromServer(s->control);	// Cerberus FTP server seems to take a long time to send this message....
		// printf("...done\n");
		// LogFTPTraffic("Transfer complete - %s\n",ServerLine);
		//
		// Now make the file read-only (ignore response 'cos it may
		// not work if the FTP server at the other end doesn't handle
		// chmod)
		//
		/// ### RHT - 13/11/2007 - Temporarily don't attempt chmod on Tandem
		/// ### PAG - 18/04/2008 - Or AS400 - doesn't work anyway.
		/// RHT 24/01/2011 - Temporarily don't attempt chmod on MVS either
		///
		/// FOR AS/400 should we issue a CHGAUT command?
		///
		if((s->SystemType != FTP_SYSTEM_TANDEM_GUARDIAN)
			&& (s->SystemType != FTP_SYSTEM_AS400)
			&& (s->SystemType != FTP_SYSTEM_MVS)) {
			// RHT 10/08/2011 - free reply
			SERVERREPLY ignore = FTP_SendCommandToServer(s,"SITE CHMOD 444",FileName);
			Free(ignore.ReplyText);
		}
	}
	else
	{
		*FileData=(unsigned char *)0;
		*DataLen=0;
	}
	return t;
}


SERVERREPLY FTP_CreateFile(FTPSOCKET *s,char *pFileName,char *FileData,long DataLen,bool ThisMode,bool BrowseMode,bool ReplaceReadOnlyFiles,bool Translate)
{
	SERVERREPLY	t;
	char		*FileName=pFileName;
	t.ReplyText = NULL;	// RHT 10/08/2011 - initialise to null

	FTP_EnterPassiveMode(s);
	if (s->LastMode != ThisMode)
	{
		// Switching mode
		s->LastMode = ThisMode;
		if (ThisMode==0)
		{
			FTP_EnterASCIIMode(s);
		}
		else
		{
			FTP_EnterBinaryMode(s);
		}
	}
	while (*FileName && (*FileName=='\\' || *FileName=='/')) FileName++;	// skip past any leading dirsep chars...

	s->data.trisocket=EstablishOutgoingConnection(s->DataPort,s->HostName);
	// if (!d) printf("Failed to connect to data connection");

	if (ReplaceReadOnlyFiles && (s->SystemType != FTP_SYSTEM_VMS))
	{
		// If we're set to replace read-only files then delete the file first
		if( s->SystemType == FTP_SYSTEM_TANDEM_GUARDIAN )
		{
			// ### RHT - 13/11/2007 - On Tandem we need to remove the filecode for the DELE
			char *temp = Strdup(FileName);
			int n;
			for( n = strlen(temp) - 1; (n > 0) && isdigit(temp[n]); n-- ) { /* empty */ }
			if( temp[n] == ',' ) temp[n] = '\0';
			t=FTP_SendCommandToServer(s,"DELE",temp);
			Free(temp);
		}
		else
		{
			t=FTP_SendCommandToServer(s,"DELE",FileName);
		}
		//
		// we should really check the return code but if we can't remove it
		// the chances are we won't be able to STOR it either....
		//
	}

	//
	// ### RHT - 17/10/2006 - If this is a "check-out for browse" then attempt
	// to set the permissions to "read-only".  On Tandem the chmod takes effect
	// on subsequently STOREed files - ### THIS DOESN'T SEEM TO HAVE ANY EFEFCT
	//
	///if (BrowseMode && (s->SystemType == FTP_SYSTEM_TANDEM_GUARDIAN))
	///{
	///	FTP_SendCommandToServer(s,"SITE CHMOD 776","");
	///}

	if(t.ReplyText) { Free(t.ReplyText); }	// RHT 10/08/2011 - free if set

	t=FTP_SendCommandToServer(s,"STOR",FileName);
	// RHT 07/09/2010 - Changed from 150 and 125 to all 1xx codes
	if ((t.ReplyCode >= 100) && (t.ReplyCode <= 199))
	{
#ifdef ENABLE_FTPS
		FTPS_DataConnect(s, &s->data);
#endif /*ENABLE_FTPS*/

		if (ThisMode==0 && Translate)
		{
			// RHT 29/07/2011 - dump the pre-conversion buffer to the ftp log in very verbose mode
			LogFTPBuffer(FileData, DataLen, FTP_DUMP_FILE_CONTENT_BEFORE_XLATE);

			// ASCII transfer mode - ensure all LF are CR-LF
			// All ASCII files extracted from Harvest will be
			// in UNIX mode (LF only)
			//
			// translate LF to CR-LF
			//
			long LineCount=0;
			long k;
			for (k=0;k<DataLen;k++)
			{
				if (FileData[k]=='\n') LineCount++;
			}
			char *ConvFileData;
			ConvFileData=(char *)Malloc(DataLen+LineCount);
			long p=0;
			for (k=0;k<DataLen;k++)
			{
				if (FileData[k]=='\n')
				{
					ConvFileData[p++]='\r';
				}
				ConvFileData[p++]=FileData[k];
			}

			// RHT 28/07/2011 - log number of bytes we are sending; 12/08/2011 tidy
			LogFTPTraffic(s, "Sending %ld bytes in ASCII mode\n", p);

			// RHT 29/07/2011 - dump the post-conversion buffer to the ftp log in verbose mode
			LogFTPBuffer(ConvFileData, p, FTP_DUMP_FILE_CONTENT);

			FTP_SendDataToServer(&s->data,ConvFileData,p);
			Free(ConvFileData);
		}
		else
		{
			//
			// Binary data - send verbatim
			//
			// RHT 12/08/2011 - also log number of bytes we are sending here
			LogFTPTraffic(s, "Sending %ld bytes in %s mode\n", DataLen, (ThisMode == 0) ? "ASCII" : "Binary");

			// RHT 29/07/2011 - dump the buffer to the ftp log in verbose mode
			LogFTPBuffer(FileData, DataLen, FTP_DUMP_FILE_CONTENT);

			FTP_SendDataToServer(&s->data,FileData,DataLen);
		}

		FTP_CloseDataConnection(&s->data);
		
		bool MoreToRead=false;
		do
		{
			char *res=FTP_ReadLineFromServer(&s->control);
			LogFTPTraffic(s,"<--- %s\n",res);	// RHT 15/07/2008 - Log hostname
			int rcode=atol(res);
			if (rcode)
			{
				// RHT 17/11/2011 - look for 426 truncated messages on AS400 and report back
				if((s->SystemType == FTP_SYSTEM_AS400) && (rcode == 426) && (t.ReplyCode != 426))
				{
					t.ReplyCode = rcode;
					if(t.ReplyText) Free(t.ReplyText);
					t.ReplyText = Strdup(res);
				}

				//
				// Line begins with a numeric code
				//
				char *x=res;
				while (*x && (*x<'0' || *x>'9')) x++; // Find first digit
				while (*x && *x!=' ' && *x!='-') x++; // Find end of digits
				if (*x=='-')
				{
					MoreToRead=true;
				}
				else
				{
					MoreToRead=false;
				}
			}
			else
			{
				//
				// Line does NOT begin with a numeric code - assume continuation line and carry on
				//
				MoreToRead=true;
			}
			Free(res);	// RHT 10/08/2011
		} while (MoreToRead);
		
		//
		// If this is a "check-out for browse" then attempt to
		// set the permissions to "read-only"
		//
		if (BrowseMode)
		{
			// ### RHT - 17/10/2006 - On Tandem the permissions are set prior
			// to transfer, so we actually need to reset the permissions to 666
			// (the default) ready for subsequent transfers.  For anything else
			// we issue a chmod against the file we just transferred.
			///if (s->SystemType == FTP_SYSTEM_TANDEM_GUARDIAN)
			///	FTP_SendCommandToServer(s,"SITE CHMOD 666","");
			///else
			///	FTP_SendCommandToServer(s,"SITE CHMOD 444",FileName);

			/// Temporarily don't attempt chmod on Tandem
			if (s->SystemType != FTP_SYSTEM_TANDEM_GUARDIAN)
			{
				// RHT 10/08/2011 - free reply
				//SERVERREPLY ignore = FTP_SendCommandToServer(s,"SITE CHMOD 444",FileName);
				//Free(ignore.ReplyText);
			}
		}
	}
	else
	{
		FTP_CloseDataConnection(&s->data);
	}
	return t;
}

FTP_FILE_LIST *FTP_GetFiles(FTPSOCKET *s,char *Dir,char *FilePattern,bool showAllVersions)
{
	SERVERREPLY	t;
	void		*dirdata;
	unsigned long	dirdatalen;
	FTP_FILE_LIST	*FileList;


	FileList = (FTP_FILE_LIST *)0;

	FTP_EnterPassiveMode(s);
	s->data.trisocket=EstablishOutgoingConnection(s->DataPort,s->HostName);
	if (!s->data.trisocket)
	{
		// printf("Failed to establish outgoing data connection\n");
		// printf("DataPort=[%d] HostName=[%s]\n",s->DataPort,s->HostName);
		LogFTPTraffic(s, "Failed to connect to data connection");	// RHT 10/08/2011 - was FTP_ExitWithError
		return FileList;
	}
	else
	{
		// printf("Data connection established:\n");
		// printf("DataPort=[%d] HostName=[%s]\n",s->DataPort,s->HostName);
	}
	if (s->SystemType == FTP_SYSTEM_AS400)
	{
		// Switch iSeries boxes into List Format 1 (Unix Format)
		t=FTP_SendCommandToServer(s,"SITE LISTFMT","1");
		Free(t.ReplyText);	// RHT 10/08/2011 - free immediately
		// printf("Setting iSeries ListFormat 1 returned code %d\n",t.ReplyCode);

		// RHT 16/08/2011 - in IASP mode, we cannot cd into .FILEs
		char *dotfilename = NULL;
		if(s->AS400IASP) {
			char *dotfile = strstrnocase(Dir, ".FILE");
			if(dotfile) {
				// Pull out the .FILE part
				for(char *p = dotfile; (p > Dir); p--) {
					if (*p == '/') {
						dotfilename = Strdup(p + 1);
						break;
					}
				}
				if(!dotfilename) {
					dotfilename = Strdup(Dir);
				}

				LogFTPTraffic("IASP mode: Dir=[%s]\n", dotfilename);
			}
		}

		t = FTP_SendCommandToServer(s,"LIST", dotfilename ? dotfilename : "");

		if(dotfilename) Free(dotfilename);
	}
	// RHT 16/08/2011 - changed to handle AS400 list separately
	else
	{
		t=FTP_SendCommandToServer(s,"LIST","");
	}
	Free(t.ReplyText);	// RHT 10/08/2011 - free immediately

	// RHT 07/09/2010 - Changed from 150 and 125 to all 1xx codes
	if ((t.ReplyCode >= 100) && (t.ReplyCode <= 199))
	{
#ifdef ENABLE_FTPS
		FTPS_DataConnect(s, &s->data);
#endif /*ENABLE_FTPS*/

		dirdata = FTP_ReadDataFromServer(&s->data,&dirdatalen);
		LogFTPBuffer((char*) dirdata, dirdatalen, FTP_DUMP_DIRECTORY_LISTINGS);	// RHT 05/08/2011 - log directory listing
		// write(1,dirdata,dirdatalen);
		switch (s->SystemType)
		{
			case FTP_SYSTEM_UNIX:
			case FTP_SYSTEM_TANDEM_OSS:
				// FolderList = DecodeUnixListOutput(dirdata,dirdatalen);
				FileList = DecodeUnixListOutput(dirdata,dirdatalen);
				break;
			case FTP_SYSTEM_AS400:	// RHT 23/08/2011 - use alternate decoder on AS400 IASPs
				if(s->AS400IASP) {
					FileList = DecodeAS400IASPListOutput(dirdata,dirdatalen);
				} else {
					FileList = DecodeUnixListOutput(dirdata,dirdatalen);
				}
				break;
			case FTP_SYSTEM_WIN32:
				// FolderList = DecodeWinListOutput(dirdata,dirdatalen);
				// RHT 07/09/2010 - Allow us to handle optional unix format IIS listings
				if(IsUnixFormatListOutput(dirdata,dirdatalen)) {
					FileList = DecodeUnixListOutput(dirdata,dirdatalen);
				} else {
					FileList = DecodeWinListOutput(dirdata,dirdatalen);
				}
				break;
			case FTP_SYSTEM_VMS:
				FileList = DecodeVMSListOutput(dirdata,dirdatalen,showAllVersions);
                break;
			case FTP_SYSTEM_TANDEM_GUARDIAN:
				FileList = DecodeTandemListOutput(dirdata,dirdatalen);
				break;
			case FTP_SYSTEM_MVS:
				FileList = DecodeMVSListOutput(dirdata,dirdatalen);
                break;
			default:
				LogFTPTraffic(s,"WARNING: Directory listing not understood\n");
				break;
		}
		FTP_CloseDataConnection(&s->data);
		Free(dirdata);		// RHT 02/10/2011 - don't need the raw data any more!
		char *post = FTP_ReadLineFromServer(&s->control);	// RHT 29/08/2009 - log post reply
		LogFTPTraffic(s,"<--- %s\n",post ? post : "(null)");
		Free(post);	// RHT 10/08/2011
	}
	// FTP_CloseDataConnection(d);
	// printf("\n\nRETURNING:\n");
	// FTP_FILE_LIST *x=FileList;
	// while (x)
	// {
	//	printf("%s (Directory=%s)\n",x->FileName,x->Directory?"true":"false");
	//	x=x->next;
	// }
	return FileList;
}


int FTP_Read(FTPCONNECTION *c, char *buffer, int len)
{
	// RHT 29/09/2011 - check we haven't already closed the connection
	if(c->trisocket) {
		return recv(c->trisocket->sock,buffer,len,0);
	}
	return 0;
}

int FTP_Write(FTPCONNECTION *c, char *buffer, int len)
{
	// RHT 29/09/2011 - check we haven't already closed the connection
	if(c->trisocket) {
		return send(c->trisocket->sock,buffer,len,0);
	}
	return -1;
}


#ifdef ENABLE_FTPS
static char* get_ssl_error()
{
  SSL_load_error_strings();
  return ERR_error_string(ERR_get_error(), NULL);
}

int FTPS_Read(FTPCONNECTION *c, char *buffer, int len)
{
	return SSL_read((SSL*)c->pSsl,buffer,len);
}

int FTPS_Write(FTPCONNECTION *c, char *buffer, int len)
{
	return SSL_write((SSL*)c->pSsl,buffer,len);
}

FTP_REPLY FTPS_DataConnect(FTPSOCKET *s, FTPCONNECTION *c)
{
	if (c->read_fn == FTPS_Read)
	{
		return FTPS_Connect(s, c);
	}

	return FTP_OKAY;
}

#if 0
void FTPS_InfoCallback(const SSL *ssl,int type,int val)
{
	switch(type)
	{
	case SSL_CB_HANDSHAKE_START: LogFTPTraffic( "Info: Handshake Start %d (%08x)\n", val, ssl->state ); break;
	case SSL_CB_CONNECT_LOOP:    LogFTPTraffic( "Info: Connect Loop %d (%08x)\n", val, ssl->state ); break;
	case SSL_CB_CONNECT_EXIT:    LogFTPTraffic( "Info: Connect Exit %d (%08x)\n", val, ssl->state ); break;
	case SSL_CB_READ_ALERT:      LogFTPTraffic( "Info: Read Alert %d (%08x)\n", val, ssl->state ); break;
	default:					 LogFTPTraffic( "Info: %d %d (%08x)\n", type, val, ssl->state ); break;
	}
}

void FTPS_MsgCallback(int write_p, int version, int content_type, const void *buf, size_t len, SSL *ssl, void *arg)
{
	LogFTPTraffic( "Info: Msg %d %d %d %p %d %p (%08x)\n", write_p, version, content_type, buf, len, arg, ssl->state );
}
#endif

FTP_REPLY FTPS_Connect(FTPSOCKET *s, FTPCONNECTION *c)
{
	//LogFTPTraffic("FTPS: FTPS_Connect on fd %d\n", c->trisocket->sock);

	if (!sslInitialised)
	{
		sslInitialised = 1;

		SSL_library_init();
	}

	if (!s->ssl_ctx)
	{
		SSL_CTX* p_ctx = SSL_CTX_new(SSLv23_method());
		if (!p_ctx)
		{
			LogFTPTraffic("SSL_CTX_new failed\n");
			return FTP_NO_CONNECTION;
		}

		// SSL_OP_NO_SSLv2, SSL_OP_NO_SSLv3, SSL_OP_NO_TLSv1
		SSL_CTX_set_options(p_ctx, SSL_OP_ALL);

		if (SSL_CTX_set_cipher_list (p_ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH") != 1)
		{
			LogFTPTraffic("SSL_CTX_set_cipher_list failed\n");
			return FTP_NO_CONNECTION;
		}

		if (RAND_status() != 1)
		{
			LogFTPTraffic("RAND_status is not 1\n");
			return FTP_NO_CONNECTION;
		}

		s->ssl_ctx = p_ctx;
	}

	SSL_CTX* p_ctx = (SSL_CTX*) s->ssl_ctx;

	SSL* p_ssl = SSL_new(p_ctx);
	if (!p_ssl)
	{
		LogFTPTraffic("FTPS: SSL_new failed\n");
		return FTP_NO_CONNECTION;
	}

#if 0
	// FTPS Handshake debug
	SSL_set_info_callback(p_ssl, FTPS_InfoCallback);
	SSL_set_msg_callback(p_ssl, FTPS_MsgCallback);
#endif

	if (!SSL_set_fd(p_ssl, c->trisocket->sock))
	{
		LogFTPTraffic("FTPS: SSL_set_fd failed\n");
		SSL_free(p_ssl);
		return FTP_NO_CONNECTION;
	}

	if (s->control.pSsl)
	{
		SSL_copy_session_id(p_ssl,(const SSL*)s->control.pSsl);
	}

	if (SSL_connect(p_ssl) <= 0)
	{
		s->LastMessage = Strdup(get_ssl_error());
		LogFTPTraffic("FTPS: SSL_connect failed: %s\n", s->LastMessage);
		SSL_free(p_ssl);
		return FTP_NO_CONNECTION;
	}

	c->pSsl = p_ssl;

	LogFTPTraffic("FTPS: encrypted connection establised\n");
	return FTP_OKAY;
}
#endif /*ENABLE_FTPS*/


FTPSOCKET *FTP_Connect(char *ServerName,char *UserName,char *Password,bool secure,bool encryptData)
{
	FTPSOCKET	*ftpserver;

	ftpserver=(FTPSOCKET *)Malloc(sizeof(FTPSOCKET));
		/*###*/ftpserver->UserName = Strdup(UserName);
		/*###*/ftpserver->Password = Strdup(Password);
	ftpserver->HostName = Strdup(ServerName);
	ftpserver->WelcomeText		= NULL;
	ftpserver->Result			= FTP_OKAY;
	ftpserver->LastMessage   	= NULL;
	ftpserver->control.read_fn  = FTP_Read;
	ftpserver->control.write_fn = FTP_Write;
	ftpserver->control.pSsl     = NULL;
	ftpserver->data.read_fn     = FTP_Read;
	ftpserver->data.write_fn    = FTP_Write;
	ftpserver->data.pSsl        = NULL;
	ftpserver->ssl_ctx			= NULL;
	ftpserver->AS400IASP		= NULL;

	SetupConnection(ftpserver,secure,encryptData);

	if (ftpserver->control.trisocket
#ifdef ENABLE_FTPS
		&& (ftpserver->Result == FTP_OKAY)
#endif /*ENABLE_FTPS*/
		)
	{
		ftpserver->Result = FTP_LoginToServer(ftpserver,UserName,Password);
		//###ftpserver->UserName = Strdup(UserName);
		//###ftpserver->Password = Strdup(Password);
		if (ftpserver->Result == FTP_OKAY)
		{
			char *SystemType = FTP_GetSystemType(ftpserver);
			ftpserver->SystemType=FTP_SYSTEM_UNKNOWN;
			ftpserver->DirSepChar='/';	// default
			ftpserver->LastDir=(char *)0;

			// RHT - 11/09/2007 - Tandem does not return a system type - we
			// just get null - this fix just prevents a null pointer deref.
			if( SystemType )
			{
				if (!FTP_System_Strings) { BuildFtpSystemStrings(); }
				//
				// Go through the list of known system types
				//
				for (int k=0; k<FTP_System_Strings_Count; k++)
				{
					if (strcmp(	SystemType,
							FTP_System_Strings[k].SystemTypeString
						)==0)
					{
						// Match
						ftpserver->SystemType=FTP_System_Strings[k].SystemType;
						ftpserver->DirSepChar=FTP_System_Strings[k].DirSepChar;
					}
				}
			}

			// RHT - 11/01/2008 - If the system type is not recognised then
			// look at the welcome text - in particular this accounts for
			// Tandem systems running later FTP servers that return an unknown
			// system type.
			if (ftpserver->SystemType == FTP_SYSTEM_UNKNOWN)
			{
				//
				// Couldn't determine system type normally - use the WelcomeText as a hint
				// It would be preferable to move the "TANDEM" string into another column
				// in the system types table.
				// RHT - 12/09/2007
				//
				LogFTPTraffic("SYSTEM TYPE UNKNOWN: Hint = [%s]\n",ftpserver->WelcomeText);

				if( strstr( ftpserver->WelcomeText, "TANDEM" ) )
				{
					LogFTPTraffic("SYSTEM TYPE is actually TANDEM\n");
					ftpserver->SystemType=FTP_SYSTEM_TANDEM_GUARDIAN;
					LookupDirSepChar( ftpserver );
				}
			}

			// RHT - 11/01/2008 - The above code always selects Tandem Guardian
			// for any Tandem system.  We now need to check to see if it is
			// already in OSS mode and reset the system type accordingly.
			if ((ftpserver->SystemType == FTP_SYSTEM_TANDEM_GUARDIAN)
				&& CheckForTandemOSS(ftpserver))
			{
				// Actually the Tandem box supports OSS, so reset the system type.
				LogFTPTraffic("SYSTEM TYPE reset to TANDEM OSS\n");
				ftpserver->SystemType=FTP_SYSTEM_TANDEM_OSS;
				LookupDirSepChar( ftpserver );
			}
			// TODO: If it turns out an AS400 user's home dir can be on an IASP, we need to do a check here! - RHT 15/08/2011

			ftpserver->DefaultDir=FTP_GetCurrentDirectory(ftpserver);
			FTP_EnterBinaryMode(ftpserver);	// default
			ftpserver->LastMode=0;	// Start in ASCII Mode
		}
	}

	return ftpserver;
}

SERVERREPLY FTP_DeleteFile(FTPSOCKET *ftpserver,char *RemoteFileName)
{
	return FTP_SendCommandToServer(ftpserver,"DELE",RemoteFileName);
}

char *GetRecord(unsigned char *FileData,int FileLength,int recno)
{
	#define RECLEN 905
	int NumRecordsInFile = FileLength/RECLEN;
	char *reply;
	if (recno<NumRecordsInFile)
	{
		int offset = (recno*RECLEN);
		reply = (char *)malloc(RECLEN);
		for (int i=0;i<RECLEN;i++)
		{
			reply[i]=FileData[offset+i];
		}
	}
	else
	{
		// No More Records
		reply=(char *)0;
	}
	return reply;
}

char *GetFieldInRecord(char *p,int start, int len)
{
	char *res=(char *)malloc(len+1);
	int i;
	res[len]='\0';
	for (i=0;i<len;i++)
	{
		res[i]=p[start+i];
	}
	// rtrim
	i=len-1;
	while((i>=0) && (res[i]==' ')) i--;
	res[i+1]='\0';
	return res;
}

void SetExtendedAttributes_iSeries(FTPSOCKET *ftpserver,char *folderin,char *filename,char *mbrtype,char *mbrtext)
{
	if (ftpserver->SystemType == FTP_SYSTEM_AS400)
	{
		// printf("SetExtendedAttributes_iSeries folderin=[%s] filename=[%s] mbrtype=[%s] mbrtext=[%s]\n",folderin,filename,mbrtype,mbrtext);
		//
		// INFO: SetExtendedAttributes_iSeries folder=[/QSYS.LIB/PGIBBS1.LIB/PAGTEST2.FILE]
		// filename=[ATMA111B.MBR] mbrtype=[RPGLE] mbrtext=[This is ATMA111B and is a test file]
		//
		// folder string looks like:
		// /QSYS.LIB/mylib.lib/myfile.file
		//
		char *folder = strdup(folderin);
		char *libptr2 = (char *)0;
		char *libptr1 = strstrnocase(folder,".LIB");
		if (libptr1) {
			libptr2 = strstrnocase(libptr1+4,".LIB");
		}
		char *libptr = (libptr2)?libptr2:libptr1;
		if (!libptr) return;
		*(libptr+4)='\0';
		char *libfolder=strdup(folder);
		*libptr='\0';	// terminate at the .LIB
		
		char *fp=libptr+5;
		char *lp=libptr-1;
		while (*lp!='/' && *lp!='\\') lp--;	// walk backwards to find start of library
		lp++;	// move on to first character of library
		char *dp=strstrnocase(fp,".FILE");
		if (dp) *dp='\0';	// terminate at the .FILE
		char *fn = strdup(filename);
		char *tx = strstrnocase(fn,".MBR");
		if (tx) *tx='\0';

		// printf("lp=[%s] fp=[%s] fn=[%s]\n",lp,fp,fn);
		char *CmdString = (char *)malloc(strlen(lp)+strlen(fp)+strlen(fn)+strlen(mbrtype)+strlen(mbrtext)+128);
		// lp=[PGIBBS1] fp=[PAGTEST2] libfolder=[/QSYS.LIB/PGIBBS1.LIB]

		sprintf(CmdString, "CHGPFM FILE(%s/%s) MBR(%s) TEXT('%s') SRCTYPE(%s)",lp,fp,fn,mbrtext,mbrtype);
		// printf("%s\n",CmdString);
		SERVERREPLY t = FTP_SendCommandToServer(ftpserver,"RCMD",CmdString);
		// printf("t.ReplyText[%s]\n",t.ReplyText);
		free(t.ReplyText);
		free(libfolder);
		free(fn);
		free(CmdString);
		free(folder);
	}
}

char **GetExtendedAttributes_iSeries(FTPSOCKET *ftpserver,char *folderin)
{
	char **res = (char **)malloc(sizeof(char **));
	res[0]=(char *)0;	// end of list
	if (ftpserver->SystemType == FTP_SYSTEM_AS400)
	{
		//
		// folder string looks like:
		// /QSYS.LIB/mylib.lib/myfile.file
		//
		char *folder = strdup(folderin);
		char *libptr2 = (char *)0;
		char *libptr1 = strstrnocase(folder,".LIB");
		if (libptr1) {
			libptr2 = strstrnocase(libptr1+4,".LIB");
		}
		char *libptr = (libptr2)?libptr2:libptr1;
		if (!libptr) return res;
		*(libptr+4)='\0';
		char *libfolder=strdup(folder);
		*libptr='\0';	// terminate at the .LIB
		
		char *fp=libptr+5;
		char *lp=libptr-1;
		while (*lp!='/' && *lp!='\\') lp--;	// walk backwards to find start of library
		lp++;	// move on to first character of library
		char *dp=strstrnocase(fp,".FILE");
		if (dp) *dp='\0';	// terminate at the .FILE

		// printf("lp=[%s] fp=[%s] libfolder=[%s]\n",lp,fp,libfolder);

		SERVERREPLY t = FTP_SwitchDirectory(ftpserver,libfolder);
		free(libfolder);
		free(t.ReplyText);
		char *CmdString=(char *)malloc(strlen(lp)+strlen(fp)+200);
		sprintf(CmdString,"DSPFD FILE(%s/%s) TYPE(*MBR) OUTPUT(*OUTFILE) OUTFILE(QTEMP/RTI%05d)",lp,fp,getpid());
		// printf("%s\n",CmdString);
		t = FTP_SendCommandToServer(ftpserver,"RCMD",CmdString);
		// printf("t.ReplyText[%s]\n",t.ReplyText);
		free(t.ReplyText);
		free(CmdString);
		free(folder);
		
		if (t.ReplyCode >= 200 && t.ReplyCode <=299)
		{
			//
			// Now retrieve the file
			//
			char attrfile[30];
			sprintf(attrfile,"QTEMP/RTI%05d",getpid());
			unsigned char *FileData=(unsigned char *)0;
			unsigned long FileLength;
			t = FTP_SendCommandToServer(ftpserver,"SITE NAMEFMT","0");
			free(t.ReplyText);
			t = FTP_SendCommandToServer(ftpserver,"SITE TRIM","0");
			free(t.ReplyText);
			// printf("getting file data for %s\n",attrfile);
			t = FTP_GetFileData(ftpserver,true,attrfile,&FileData,&FileLength);
			// printf("t.ReplyText=[%s] FileLength=%d\n",t.ReplyText,FileLength);
			free(t.ReplyText);
			t = FTP_SendCommandToServer(ftpserver,"SITE TRIM","1");
			free(t.ReplyText);
			t = FTP_SendCommandToServer(ftpserver,"SITE NAMEFMT","1");
			free(t.ReplyText);
			if (((FileLength / RECLEN) * RECLEN) != FileLength) return res;
			int NumRecords = FileLength / RECLEN;
			// printf("NumRecords=%d\n",NumRecords);
			res = (char **)realloc(res,sizeof(char *)*(NumRecords+1));
			res[NumRecords]=(char *)0;	// end of list
			for (int n=0;n<NumRecords;n++) {
				char *p = GetRecord(FileData,FileLength,n);
				char *MemberName=GetFieldInRecord(p,163,10);
				char *MemberText=GetFieldInRecord(p,193,50);
				char *MemberType=GetFieldInRecord(p,586,10);
				printf("MemberName=[%s] MemberText=[%s] MemberType=[%s]\n",MemberName,MemberText,MemberType);
				res[n]=(char *)malloc(strlen(MemberName)+strlen(MemberText)+strlen(MemberType)+10);
				char *ptr=res[n];
				strcpy(ptr,MemberName);
				strcpy(ptr+strlen(MemberName)+1,MemberText);
				strcpy(ptr+strlen(MemberName)+strlen(MemberText)+2,MemberType);
				// res[n] is now MemberName\0MemberText\0MemberType\0
				free(MemberName);
				free(MemberText);
				free(MemberType);
				free(p);
			}
			free(FileData);
		}
	}
	return res;
}

