/*
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef WIN32
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#define _O_WRONLY O_WRONLY 
#define _O_BINARY 0
#define _O_CREAT O_CREAT
#define _O_TRUNC O_TRUNCs
#endif /*WIN32*/

#include "dm.h"
#include "node.h"
#include "lexer.h"
#include "scopestack.h"
#include "exceptions.h"
#include "thread.h"
#include "model.h"
#include "dropzone.h"
#include "context.h"
#include "extended.h"
#include "function.h"
#include "expr.h"
#include "charptr.h"
#include "triodbc.h"
#include "audit.h"
#include "notifytemplate.h"

// For factories
#include "repository.h"
#include "transfer.h"
#include "notify.h"
#include "transfer.h"
#include "datasource.h"
#include "filefind.h"
#include "properties.h"
#include "modify.h"
#include "pathname.h"
#include "task.h"

// For base64 decoding
#include "crypto.h"

// For loading scripts
#include "tinyxml.h"

// For DoHttpRequest
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"

extern char *base64encode(unsigned char *data, unsigned long datalen);
extern int yyparse(void *buffer);

extern void InitialiseWinsock();

///////////////////////////////////////////////////////////////////////////////

bool getConnectionDetails(const char *fullurl,char **server,int *port,bool *secure,char **url)
{
	int sp;
	if (	tolower(fullurl[0])=='h' &&
			tolower(fullurl[1])=='t' &&
			tolower(fullurl[2])=='t' &&
			tolower(fullurl[3])=='p') {
		if (tolower(fullurl[4])=='s') {
			*secure=true;
			sp=5;
		} else {
			*secure=false;
			sp=4;
		}
		//printf("sp=%d\n",sp);
		//printf("fullurl[%d]='%c'\n",sp,fullurl[sp]);
		if (fullurl[sp]!=':')	return true;	
		if (fullurl[sp+1]!='/') return true;
		if (fullurl[sp+2]!='/') return true;
		// http://server[:port]/ or
		// http://server/
		char *serverport = (char *)strdup((const char *)&(fullurl[sp+3]));	// Point to server[:port]
		char *ep = (char *)strchr(serverport,'/');
		if (ep) {
			// Found a /
			*url=strdup(ep);
			*ep='\0';
		} else {
			// No / - presumably just server[:port]
			*url=strdup("");
		}
		// Is there a port ID?
		char *ps = (char *)strchr(serverport,':');
		if (ps) {
			// There's a port specification
			*ps='\0';
			*port = atoi(ps+1);
		} else {
			// No port specified, use default
			*port = *secure?443:80;
		}
		*server=strdup(serverport);
		free((void *)serverport);
	} else {
		// No http:// start string.
		char *ep = (char *)strchr(fullurl,'/');
		if (ep) {
			*url=strdup(ep);
			*ep='\0';
		} else {
			*url=strdup("");
		}
		char *serverport=strdup(fullurl);
		char *ps = strchr(serverport,':');
		if (ps) {
			// There's a port specification
			*ps='\0';
			*port = atoi(ps+1);
		} else {
			// No port specified, use default
			*port = 80;
		}
		*server=strdup(serverport);
		free(serverport);
	}
	//printf("server=[%s]\n",*server);
	//printf("port=[%d]\n",*port);
	//printf("url=[%s]\n",*url);
	return false;
}

void SendDataToSocket(int sock, const char *data, long len)
{
	long bytesRemaining = len;

	while(bytesRemaining > 0)
	{
        int bytesSent = send(sock, data, bytesRemaining, 0);
        if(bytesSent == 0)
        {
			return;
        }
        else if(bytesSent == -1)
        {
            perror("Failed to send");
			return;
        }

		data += bytesSent;
		bytesRemaining -= bytesSent;
	}
}

void SendDataToSocket(SSL *ssl, const char *data, long len)
{
	long bytesRemaining = len;

	while(bytesRemaining > 0)
	{
		int bytesSent = SSL_write(ssl, data, bytesRemaining);   /* encrypt & send message */
        if(bytesSent == 0)
        {
			return;
        }
        else if(bytesSent == -1)
        {
            perror("Failed to send");
			return;
        }

		data += bytesSent;
		bytesRemaining -= bytesSent;
	}
}

bool ReadDataFromSocket(int sock, char *dataPtr, long length)
{
	// Reads "length" bytes of data from the socket.
	// If less than "Length" bytes are read then loop.
	//
	long		BytesRead=0;
	char 		*xPtr;
	long		BytesToRead;
	bool		res=true;

	xPtr=(char *)dataPtr;
	BytesToRead=length;
	bool BytesRemaining=true;
	while (BytesRemaining)
	{
		BytesRead=recv(sock,xPtr,BytesToRead,0);
		if (BytesRead == -1)
		{
			// ShowErrorMessage("Failed to read from socket");
			BytesRemaining=false;
			res=false;
			break;
		}
		if (BytesRead == 0)
		{
			// Server has closed connection.
			// ShowErrorMessage("Server has closed connection");
			BytesRemaining=false;
			res=false;
			break;
		}
		if (BytesRead<BytesToRead)
		{
			BytesToRead=BytesToRead-BytesRead;
			xPtr=xPtr+BytesRead;
		}
		else
		{
			BytesRemaining=false;
		}
	}
	return res;
}

bool ReadDataFromSocket(SSL *ssl, char *dataPtr, long length)
{
	// Reads "length" bytes of data from the socket.
	// If less than "Length" bytes are read then loop.
	//
	long		BytesRead=0;
	char 		*xPtr;
	long		BytesToRead;
	bool		res=true;

	xPtr=(char *)dataPtr;
	BytesToRead=length;
	bool BytesRemaining=true;
	while (BytesRemaining)
	{
        BytesRead = SSL_read(ssl, xPtr, BytesToRead); /* get reply & decrypt */
		if (BytesRead == -1)
		{
			BytesRemaining=false;
			res=false;
			break;
		}
		if (BytesRead == 0)
		{
			BytesRemaining=false;
			res=false;
			break;
		}
		if (BytesRead<BytesToRead)
		{
			BytesToRead=BytesToRead-BytesRead;
			xPtr=xPtr+BytesRead;
		}
		else
		{
			BytesRemaining=false;
		}
	}
	return res;
}

char *ReadToEndOfStream(int sock)
{
	// Reads bytes of data from a socket until the end of the stream is reached.
	//
	int			currSize = 0;
	char		*ret = NULL;
	char		xBuf[1024];

	while(true) {
		long bytesRead = recv(sock, xBuf, sizeof(xBuf), 0);
		if(bytesRead == -1) {
			// ShowErrorMessage("Failed to read from socket");
			break;
		}
		if(bytesRead == 0) {
			// Server has closed connection.
			break;
		}

		ret = ret ? (char*) realloc(ret, currSize + bytesRead + 1) : (char*) malloc(bytesRead + 1);

		memcpy(&ret[currSize], xBuf, bytesRead);
		currSize += bytesRead;
		ret[currSize] = '\0';

	}
	return ret;
}

char *ReadToEndOfStream(SSL *ssl)
{
	// Reads bytes of data from a socket until the end of the stream is reached.
	//
	int			currSize = 0;
	char		*ret = NULL;
	char		xBuf[1024];

	while(true) {	
        long bytesRead = SSL_read(ssl, xBuf, sizeof(xBuf));
		if(bytesRead == -1) {
			break;
		}
		if(bytesRead == 0) {
			// Server has closed connection.
			break;
		}

		ret = ret ? (char*) realloc(ret, currSize + bytesRead + 1) : (char*) malloc(bytesRead + 1);

		memcpy(&ret[currSize], xBuf, bytesRead);
		currSize += bytesRead;
		ret[currSize] = '\0';
	}
	return ret;
}


char *ReadLineFromSocket(int sock)
{
	char Reply[1024];
	int n;

	Reply[0] = '\0';

	// Read up to CR-LF
	char ch;
	int t=0;
	do
	{
		n = recv(sock,&ch,1,0);

		if(n==0)
		{
			// Connection has been closed
			ch='\0';
		}
		else if(n==1)
		{
			if (ch=='\n' || t>=(sizeof(Reply)-1)) ch='\0';
			if (ch!='\r') Reply[t++]=ch;
		}
		else
		{
			break;	// recv failed
		}
	} while((int) ch);

	return strdup(Reply);
}

char *ReadLineFromSocket(SSL *ssl)
{
	char Reply[1024];
	int n;

	Reply[0] = '\0';

	// Read up to CR-LF
	char ch;
	int t=0;
	do
	{
		n = SSL_read(ssl, &ch, 1);

		if(n==0)
		{
			// Connection has been closed
			ch='\0';
		}
		else if(n==1)
		{
			if (ch=='\n' || t>=(sizeof(Reply)-1)) ch='\0';
			if (ch!='\r') Reply[t++]=ch;
		}
		else
		{
			break;	// recv failed
		}
	} while((int) ch);

	return strdup(Reply);
}

SSL_CTX* InitCTX(void)
{   
	SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    ctx = SSL_CTX_new(SSLv23_method());   /* Create new context */
    if ( ctx == NULL )
    {
       ERR_print_errors_fp(stderr);
       printf("Failed to InitCTX\n");
    }
	SSL_CTX_set_options(ctx, SSL_OP_ALL);
    return ctx;
}


int DoHttpRequest(const char *hostname, int port, const char *uri,	// where
			  const char *params, MESSAGE_TYPE mt, bool isSecure,const char *host, 
			  const char *soapaction, DMArray *cookieJar, DMArray *header,	// content
			  int *status, char **contentType, char **content,char *logfilename /*=NULL*/, int *datalen /*=NULL */)	// return
{
#ifdef WIN32
	InitialiseWinsock();
#endif
	FILE *logfile = (FILE *)0;
	if (logfilename) {
		logfile = fopen(logfilename,"a+");
	}
	int	sock;
	int	res;
	char *TransferEncoding = (char *)0;
	bool chunked=false;
	char *NewLocation = (char *)0;

	bool isPost = false;
	if (mt != MESSAGE_TYPE_GET) isPost=true;	// isPost is true for both SOAP and POST

	struct hostent *hp = gethostbyname(hostname);
	if(!hp)
	{
		// gethostbyname fails	
		errno = h_errno;
		return -1;
	}
	else
	{
		// Create a socket on which to send.
		sock = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in	name;
		name.sin_family = AF_INET;
		name.sin_addr.s_addr = *((int *)hp->h_addr);
		name.sin_port = htons(port);
		unsigned char *ap = (unsigned char *)&(name.sin_addr.s_addr);

		if (logfile) fprintf(logfile,"Connecting to %d.%d.%d.%d:%d...\n",
			(unsigned int)ap[0],(unsigned int)ap[1],(unsigned int)ap[2],(unsigned int)ap[3], port);

		res = connect(sock,(struct sockaddr*) &name, sizeof(name));
		if(res)
		{
			// Connection failure
			if (logfile) fprintf(logfile,"Failed to connect\n");
#ifdef WIN32
			errno = WSAGetLastError();
#endif /*WIN32*/
			return -2;
		}
		if (logfile) fprintf(logfile,"Connected\n");
	}

	SSL_CTX *ctx;
    int server;
    SSL *ssl;

	if (isSecure) {
		SSL_library_init();
 
		ctx = InitCTX();
		ssl = SSL_new(ctx);      /* create new SSL connection state */
		SSL_set_fd(ssl, sock);    /* attach the socket descriptor */
		if ( SSL_connect(ssl) == -1 ) {
			/* perform the connection */
			ERR_print_errors_fp(stderr);
			return -3;
		}
	}

	OutputStream request(false);	// auto-newline off
	if(!isPost && params) {
		// Params go on the uri for GET
		request.writeToStdOut("GET %s%s%s HTTP/1.1\r\n", uri, params[0]?"?":"",params);
	} else {
		// Either no params or using POST
		// temp
		request.writeToStdOut("%s %s HTTP/1.1\r\n", (isPost ? "POST" : "GET"), uri);
	}

	bool hostSeen=false;
	bool userAgentSeen=false;
	bool contentTypeSeen=false;
	if(header && header->count() > 0) {
		// Additional header lines
		AutoPtr<StringList> keys = header->keys();
		StringListIterator iter(*keys);
		for(const char *key = iter.first(); key; key = iter.next()) {
			Variable *var = header->get(key);
			ConstCharPtr val = (var ? var->toString() : NULL);
			if(val) {
				if (STRCASECMP(key,"host")==0) hostSeen=true;
				if (STRCASECMP(key,"user-agent")==0) userAgentSeen=true;
				if (STRCASECMP(key,"content-type")==0) contentTypeSeen=true;
				request.writeToStdOut("%s: %s\r\n",key,(const char*)val);
			}
		}
	}

	if(host && !hostSeen) {
		// Optional host header - user for sending requests to name-based virtual hosts
		request.writeToStdOut("Host: %s\r\n", host);
	}
	// Ensure User-Agent is set (github for example requires this)
	if (!userAgentSeen) {
		request.writeToStdOut("User-Agent: DeployHub\r\n");
	}

	if(cookieJar && (cookieJar->count() > 0)) {
		// Add cookies from the cookie jar hashtable
		char *cookies = (char*) malloc(1);
		*cookies = '\0';
		AutoPtr<StringList> keys = cookieJar->keys();
		StringListIterator iter(*keys);
		for(const char *key = iter.first(); key; key = iter.next()) {
			Variable *var = cookieJar->get(key);
			ConstCharPtr val = (var ? var->toString() : NULL);
			if(val) {
				debug1("Setting cookie: '%s' = '%s'", key, (const char *)val);
				cookies = (char*) realloc(cookies, strlen(cookies) + strlen(key) + strlen(val) + 4);
				if(*cookies) {
					sprintf(cookies, "%s; %s=%s", cookies, key, (const char *)val);
				} else {
					sprintf(cookies, "%s=%s", key, (const char *)val);
				}
			}
		}
		request.writeToStdOut("Cookie: %s\r\n", cookies);
	}

	if(isPost) {
		switch(mt) {
			case MESSAGE_TYPE_GET:
			case MESSAGE_TYPE_POST:
				if (!contentTypeSeen) {
					if (params && *params == '{') {
						// Looks like JSON encoded string (bit rough-and-ready)
						request.writeToStdOut("Content-Type: application/json\r\n");
					} else {
						request.writeToStdOut("Content-Type: application/x-www-form-urlencoded\r\n");
					}
				}
				break;
			case MESSAGE_TYPE_SOAP:
				if (!contentTypeSeen) {
					request.writeToStdOut("Content-Type: text/xml;charset=UTF-8\r\n");
				}
				if (soapaction && soapaction[0]) {
					request.writeToStdOut("SOAPAction: \"%s\"\r\n",soapaction);
				}
				break;
		}
		request.writeToStdOut("Content-Length: %d\r\n", (params ? strlen(params) : 0));
	}
	request.writeToStdOut("\r\n");		// End of header
	if(isPost && params) {
		request.writeToStdOut("%s", params);
	}

	if (logfile) fprintf(logfile,"Request: %s\n", request.buffer());

	char *line;
	if (isSecure) {
		SendDataToSocket(ssl, request.buffer(), request.size());
		line = ReadLineFromSocket(ssl);
	} else {
		SendDataToSocket(sock, request.buffer(), request.size());
		line = ReadLineFromSocket(sock);
	}

	if (logfile) fprintf(logfile,"line: [%s]\n",line);

	if(strncmp(line, "HTTP/1.", 7) != 0) {
		if (isSecure) {
			SSL_free(ssl);
		} else {
			CLOSESOCKET(sock);
		}
		return -3; // Not HTTP
	}

	*status = atoi(&line[9]);
	debug1("Status: %d", *status);
	if (logfile) fprintf(logfile,"status: %d\n",*status);

	bool noWait = (*status == 204);	// No Content


	long length = -1;

	while(true) {
		if (isSecure) {
			line = ReadLineFromSocket(ssl);
		} else {
			line = ReadLineFromSocket(sock);
		}
		
		if (logfile) fprintf(logfile,"line=%s\n",line);
		//debug1("line = %s", line);

		if(!*line) break;

		if(strncmp(line, "Content-Length: ", 16) == 0) {
			length = atol(&line[16]);
			if (datalen) *datalen = length;
		} else if(strncmp(line, "Content-Type: ", 14) == 0) {
			*contentType = strdup(&line[14]);
		} else if(strncmp(line, "Set-Cookie: ", 12) == 0) {
			if(cookieJar) {
				char *cookie = &line[12];
				if(*cookie == ' ') {
					cookie++;
				}
				char *eq = strchr(cookie, '=');
				if(eq) {
					char *sc = strchr(eq, ';');
					if(sc) {
						*sc = '\0';
					}
					*eq = '\0';
					debug1("Storing cookie: '%s' = '%s'", cookie, &eq[1]);
					cookieJar->put(cookie, new Variable(NULL, &eq[1]));
				}
			}
		} else if(strncmp(line,"Transfer-Encoding: ",19) == 0) {
			TransferEncoding = &(line[19]);
			// printf("TransferEncoding=[%s]\n",TransferEncoding);
			chunked=(strcmp(TransferEncoding,"chunked")==0);
		} else if(strncmp(line,"Location: ",10) == 0) {
			// If redirect, this is the new location of the file
			NewLocation = &(line[10]);
		}
	}
	if (logfile) fprintf(logfile,"Length: %d\n", length);
	if (*status >= 300 && *status <= 399) {
		// redirect in progress.
		if (NewLocation) {
			if (logfile) fprintf(logfile, "NewLocation=[%s]\n",NewLocation);
			char *newserver;
			int newport;
			bool newsecure;
			char *newurl;
			getConnectionDetails(NewLocation,&newserver,&newport,&newsecure,&newurl);
			// Recurse with new details
			// Keep original port because nexus 2 returns http://localhost rather than http://localhost:8081 
			if (logfile) fflush(logfile);
			DoHttpRequest(newserver,port,newurl,NULL,mt,newsecure,host,soapaction,cookieJar,header,status,contentType,content,logfilename);
		} else {
			if (logfile) fprintf(logfile, "Redirect status %d but no Location: returned\n",*status);
		}
	} else {
		if (chunked) {
			// No length, data is arriving in chunks
			int totalLength=0;
			char *data = (char *)malloc(1024);	// initial chunk
			do {
				char *hexsize;
				if (isSecure) {
					hexsize = ReadLineFromSocket(ssl);
				} else {
					hexsize = ReadLineFromSocket(sock);
				}
				length = strtol(hexsize,NULL,16);
				if (length>0) {
					data = (char *)realloc(data,totalLength+length+1);
					if (isSecure) {
						ReadDataFromSocket(ssl, &(data[totalLength]), length);
					} else {
						ReadDataFromSocket(sock, &(data[totalLength]), length);
					}
					totalLength=totalLength+length;
					// Now read trailing \r\n after the chunk of data
					if (isSecure) {
						ReadLineFromSocket(ssl);
					} else {
						ReadLineFromSocket(sock);
					}
				}
			} while (length);
			data[totalLength]='\0';
			if (datalen) *datalen = totalLength;
			*content = data;
		} else {
			if(length > 0) {
				char *data = (char*) malloc(length + 1);
				if (isSecure) {
					if(ReadDataFromSocket(ssl, data, length)) {
						//debug1("Data: %s", data);
						data[length] = '\0';
						*content = data;
					}
				} else {
					if(ReadDataFromSocket(sock, data, length)) {
						//debug1("Data: %s", data);
						data[length] = '\0';
						*content = data;
					}
				}
				// 
				
			} else if(length == -1 && !noWait) {
				if (isSecure) {
					*content = ReadToEndOfStream(ssl);
				} else {
					*content = ReadToEndOfStream(sock);
				}
				
			}
		}
		if (logfile && length>0) {
			fprintf(logfile,"Content is:\n");
			if (content) fprintf(logfile,*content);
			fprintf(logfile,"\nend of content\n");
			fclose(logfile);
		}
	}

	if (isSecure) {
		SSL_free(ssl);
	} else {
		CLOSESOCKET(sock);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////


int clearDirectory(const char *pathname)
{
	int res = 0;	// success
	FileFind finder(pathname, NULL);
	if(finder.find()) {
		do {
			if(finder.isDirectory()) {
				// Directory
				if(!finder.isDots()) {
					char *fp = finder.fullpath();
					debug3("recursing into '%s'", fp);
					if(clearDirectory(fp) != 0) {
						SAFE_FREE(fp);
						res = 1;
						break;
					}
					SAFE_FREE(fp);
				}
			} else {
				// File, so attempt to remove it
				char *fp = finder.fullpath();
				debug3("removing '%s'", fp);
				CHMOD(fp, FILE_WRITE_PERMISSIONS);
				if(unlink(fp) != 0) {
					debug1("clearDirectory failed to unlink '%s'", fp);
					res = 1;
					break;
				}
				SAFE_FREE(fp);
			}
		} while(finder.next());
	}
	finder.close();

	debug3("removing folder '%s'", pathname);
	CHMOD(pathname, FILE_WRITE_PERMISSIONS);
	if(RMDIR(pathname) != 0)
	{
//#if defined(WIN32) && defined(_DEBUG)
#ifdef WIN32
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		debug1("rmdir(\"%s\") failed: %s", pathname, lpMsgBuf);
		LocalFree(lpMsgBuf);
#endif /*WIN32 && _DEBUG*/
		res = 1;
	}
	return res;
}


///////////////////////////////////////////////////////////////////////////////
// TempFile
///////////////////////////////////////////////////////////////////////////////


TempFile::TempFile(int num)
{
	char fn[1024];
#ifdef WIN32
	sprintf(fn, "%s\\tdm.%d.%d.tmp", getenv("TEMP"), getpid(), num);
#else
	sprintf(fn, "/tmp/tdm.%d.%d.tmp", getpid(), num);
#endif
	m_filename = strdup(fn);
}


TempFile::~TempFile()
{
	SAFE_FREE(m_filename);
}


FILE *TempFile::open(bool forRead /*= false*/, bool append /*= false*/)
{
#ifdef WIN32
	return fopen(m_filename, (forRead ? "rb" : (append ? "ab+" : "wb")));
#else
	return fopen(m_filename, (forRead ? "r" : (append ? "a+" : "w")));
#endif /*WIN32*/
}


bool TempFile::unlink()
{
	// Make writable (dfo files are created read-only)
	CHMOD(m_filename, FILE_WRITE_PERMISSIONS);

	// Remove the file
	return (::unlink(m_filename) == 0) ? true : false;
}


///////////////////////////////////////////////////////////////////////////////
// TempFolder
///////////////////////////////////////////////////////////////////////////////


TempFolder::TempFolder(const char *deployId)
{
	char fn[1024];
#ifdef WIN32
	sprintf(fn, "%s\\tdm.%s", getenv("TEMP"), deployId);
#else
	sprintf(fn, "/tmp/tdm.%s", deployId);
#endif
	m_pathname = strdup(fn);
}


TempFolder::~TempFolder()
{
	SAFE_FREE(m_pathname);
}


bool TempFolder::mkdir(bool empty)
{
	struct stat sb;
	if(stat(m_pathname, &sb) == -1) {
		debug3("TempFolder::mkdir('%s') - stat returned -1", m_pathname);
		if(errno == ENOENT) {
			// Doesn't exist - so create it
			bool ret = (MKDIR(m_pathname) == 0) ? true : false;
			debug3("m_pathname = [%s] ret=%s",m_pathname,ret?"true":"false");
	
			if(!ret) {
				// Create failed - create the parent and then retry
				debug3("Create failed");
				TempFolder *tmp = parent();
				if(tmp) {
					debug1("TempFolder::mkdir('%s') - creating parent '%s'", m_pathname, tmp->pathname());
					if(tmp->mkdir(true)) {
						ret = (MKDIR(m_pathname) == 0) ? true : false;
					}
					SAFE_DELETE(tmp);
				}
			}

			return ret;
		} else {
			// Unable to stat
			return false;
		}
	}
	// Is it a directory?
	if(sb.st_mode & STAT_IS_DIRECTORY) {
		// Directory
		debug2("TempFolder::mkdir('%s') - dir exists", m_pathname);
		if(!empty) {
			// Empty check not required
			return true;
		}
		// Is it empty?
		FileFind finder(m_pathname, NULL);
		bool found = false;
		if(finder.find()) {
			do {
				if(!finder.isDots()) {
					// Found something, so not empty
					debug1("found file=[%s]",finder.filename());
					char *fp = (char *)malloc(strlen(m_pathname)+strlen(finder.filename())+128);
					sprintf(fp,"%s/%s",m_pathname,finder.filename());
					if (::unlink(fp)) found=true;
					free(fp);

					break;
				}
			} while(finder.next());
		}

		return found ? false : true;
	}

	// File with same name as the directory we want
	debug1("TempFolder::mkdir('%s') - file exists with same name", m_pathname);
	return false;
}


bool TempFolder::unlink()
{
	debug3("TempFolder::unlink('%s')", m_pathname);
	return (clearDirectory(m_pathname) == 0) ? true : false;
}


TempFolder *TempFolder::parent()
{
	const char *ls = strrchr(m_pathname, DIR_SEP_CHAR);
	if(!ls) {
		return NULL;
	}

	int len = ls - m_pathname;

	char *pf = (char*) malloc(len + 1);
	strncpy(pf, m_pathname, len);
	pf[len] = '\0';
	
	TempFolder *ret = new TempFolder("dummy");
	ret->m_pathname = pf;
	return ret;
}


///////////////////////////////////////////////////////////////////////////////
// DM
///////////////////////////////////////////////////////////////////////////////


DM::DM()
	: m_environment(NULL), m_application(NULL), m_application2(NULL), m_model(NULL),
	  /*m_deployId(0), m_depMajor(0),*/ m_tempFiles(true), m_tempFolders(true),
	  m_dropzones(true, true), m_dmhome(NULL), m_initializationDone(false),
	  m_stack(*this), m_eventid(0), m_BaseDir(0), m_argv(0), m_envp(0),
	  m_dialogCredentials(NULL)
{}


DM::~DM()
{
	// printf("*** IN DM DESTRUCTOR! ***\n");
	m_environment = NULL;			// storage owned by model
	m_application = NULL;			// storage owned by model
	m_application2 = NULL;			// storage owned by model
	m_model = NULL;					// created on stack - will destroy itself
	// m_tempFiles, m_tempFolders and m_dropzones are members so should be deleted automatically
}


void DM::setModel(Model *model)
{
	m_model = model;
}


Model *DM::getModel()
{
	return m_model;
}


class EngineConfig &DM::getEngineConfig()
{
	if(!m_model) {
		throw RuntimeError("Model not yet initialised - can't read engine config");
	}

	return m_model->getEngineConfig();
}


bool DM::setTargetEnvironment(const char *env)
{
	m_environment = m_model ? m_model->getEnvironment(env) : NULL;
	return m_environment ? true : false;
}


bool DM::setTargetEnvironment(int envid)
{
	m_environment = m_model ? m_model->getEnvironment(envid) : NULL;
	return m_environment ? true : false;
}


Environment *DM::getTargetEnvironment()
{
	return m_environment;
}


bool DM::setTargetApplication(const char *app)
{
	m_application = m_environment ? m_environment->application(app) : NULL;
	debug1("setTargetApplication, m_application=%s",m_application?m_application->name():"NULL");
	if (m_removeApp && m_application) {
		// If we're removing the app we need to "deploy" the base version regardless.
		Application *parent = m_application->getParent();
		if (parent) m_application = parent;
	}
	return m_application ? true : false;
}


bool DM::setTargetApplication(int appid)
{
	// TODO: This should be m_environment->application(appid)
	m_application = m_model ? m_model->getApplication(appid) : NULL;
	debug1("setTargetApplication, m_application=%s",m_application?m_application->name():"null");
	if (m_removeApp && m_application) {
		// If we're removing the app we need to "deploy" the base version regardless.
		Application *parent = m_application->getParent();
		if (parent) m_application = parent;
	}
	return m_application ? true : false;
}


class Application *DM::getTargetApplication()
{
	return m_application;
}


bool DM::setSecondApplication(int appid)
{
	bool failReturn = false;

	if(appid < 0) {
		// Special values:
		// -1 = latest (or parent application if no versions)
		// -2 = latest on branch (or parent application if no versions)
		//      new version will be created on the selected branch (i.e. the branch in inherited)
		Application *tgtapp = getTargetApplication();
		debug1("target app is %d",tgtapp->id());
		m_application2 = tgtapp ? tgtapp->getLatestVersion(/*branch*/) : NULL;
		if (!m_application2) {
			// No latest version found - probably only have a base version
			failReturn = true;	// Don't throw an error
		}
	} else {
		m_application2 = m_model ? m_model->getApplication(appid) : NULL;
	}
	return m_application2 ? true : failReturn;
}


class Application *DM::getSecondApplication()
{
	return m_application2;
}


void DM::setDialogCredentials(class Credentials *creds)
{
	m_dialogCredentials = creds;
}

void DM::setRemoveApp(bool removeApp)
{
	m_removeApp = removeApp;
}

bool DM::getRemoveApp()
{
	return m_removeApp;
}


class Credentials *DM::getDialogCredentials()
{
	return m_dialogCredentials;
}


bool DM::setCurrentUser(const char *username)
{
	if(!m_model) {
		return false;
	}

	User *currentUser = m_model->getUser(username);
	m_model->setCurrentUser(currentUser);
	if (currentUser) {
		m_stack.setGlobal("username",username);
		m_stack.setGlobal("user",new ObjectReference(currentUser));
		m_model->setDomainList();
	}
	return currentUser ? true : false;
}


class User *DM::getCurrentUser()
{
	return m_model ? m_model->getCurrentUser() : NULL;
}


void DM::updateUserLastLogin(User &user)
{
	if(m_model) {
		m_model->updateUserLastLogin(user);
	}
}


Server *DM::getPhysicalServer(const char *name)
{
	return m_environment ? m_environment->server(name) : NULL;
}


Component *DM::getComponent(const char *name,bool throwRTE /* = false */)
{
	return m_model ? m_model->getComponent(name,throwRTE) : NULL;
}


DateTime *DM::getDeployTime(Component *comp,Server *serv)
{
	return m_model ? m_model->getDeployTime(comp,serv) : NULL;
}

DateTime *DM::getDeployTime(Application *app,Environment *env)
{
	return m_model ? m_model->getDeployTime(app,env) : NULL;
}

Component *DM::getComponentById(int compid)
{
	return m_model ? m_model->getComponentById(compid) : NULL;
}

Application *DM::getApplication(const char *name)
{
	return m_model ? m_model->getApplication(name,true) : NULL;
}

Environment *DM::getEnvironment(const char *name)
{
	return m_model ? m_model->getEnvironment(name,true) : NULL;
}


Repository *DM::getRepository(const char *name)
{
	return m_model ? m_model->getRepository(name,true) : NULL;
}


class Notify *DM::getNotifier(const char *name)
{
	return m_model ? m_model->getNotifier(name) : NULL;
}


class Notify *DM::getNotifier(int id)
{
	return m_model ? m_model->getNotifier(id) : NULL;
}


class Datasource *DM::getDatasource(const char *name)
{
	return m_model ? m_model->getDatasource(name) : NULL;
}


class Action *DM::getAction(const char *name)
{
	return m_model ? m_model->getAction(name) : NULL;
}

class Action *DM::getActionTask(Task &task)
{
	return  m_model ? m_model->getActionTask(task) : NULL;
}


class Task *DM::getTask(const char *name)
{
	// allow the use of fully-qualified names
	int taskid = m_model ? m_model->GetIDFromQualifiedName(name,"dm_task",true) : 0;
	return taskid > 0 ? m_model->getTask(taskid) : NULL;
}

class User *DM::waitForTask(class Task &task, class Application &app,int deploymentid)
{
	return m_model ? m_model->waitForTask(task,app,deploymentid) : NULL;
}


class Task *DM::getTask(int taskid)
{
	return m_model ? m_model->getTask(taskid) : NULL;
}


// TODO: These will be removed after we add a Notification object
class User *DM::getUser(int id)
{
	return m_model ? m_model->getUserById(id) : NULL;
}

class NotifyTemplate *DM::getSuccessTemplate(Context &ctx,int taskid)
{
	return m_model ? m_model->getSuccessTemplate(ctx,taskid) : NULL;
}

class NotifyTemplate *DM::getFailureTemplate(Context &ctx,int taskid)
{
	return m_model ? m_model->getFailureTemplate(ctx,taskid) : NULL;
}


class UserGroup *DM::getUserGroup(int id)
{
	return m_model ? m_model->getUserGroup(id) : NULL;
}


char *DM::getNotifyText(int id)
{
	return m_model ? m_model->getNotifyText(id) : NULL;
}


ActionNode *DM::firstActionNode()
{
	return m_model ? m_model->firstActionNode() : NULL;
}


ActionNode *DM::getActionNode(const char *name)
{
	return m_model ? m_model->getActionNode(name) : NULL;
}


int DM::deployId()
{
	return getAudit().deploymentId();
}

void DM::addDeploymentAction(class Action *action)
{
	m_model->addDeploymentAction(action);
}


class Audit &DM::getAudit()
{
	if(!m_model) {
		throw RuntimeError("Model not yet initialised - can't deploy");
	}

	return m_model->getAudit();
}


class Audit &DM::getDummyAudit()
{
	if(!m_model) {
		throw RuntimeError("Model not yet initialised - can't deploy");
	}

	return m_model->getDummyAudit();
}


void DM::startAudit()
{
	Audit &audit = getAudit();
	audit.startAudit(m_environment, m_application, m_eventid);
	m_stack.setGlobal("DEPLOY_ID", audit.deploymentId());
}


void DM::finishAudit(int exitCode)
{
	getAudit().finishAudit(exitCode);
}

void DM::setEventId(int eventid)
{
	m_eventid = eventid;
}


FILE *DM::createTemporaryFile(char **filename)
{
	printf("in createTemporaryFile - filename=0x%lx\n",filename);
	TempFile *tmp = new TempFile(m_tempFiles.size() + 1);
	printf("tmp->filename=[%s]\n",tmp->filename());
	m_tempFiles.add(tmp);
	if(filename) {
		*filename = (char*) tmp->filename();
	}
	return tmp->open();
}


bool DM::createTemporaryFolder(const char *deployId, char **pathname, bool empty)
{
	TempFolder *tmp = createTemporaryFolder(deployId, empty);

	if(tmp && pathname) {
		*pathname = (char*) tmp->pathname();
	}

	return tmp ? true : false; 
}


TempFolder *DM::createTemporaryFolder(const char *deployId, bool empty)
{
	TempFolder *ret = new TempFolder(deployId);

	if(ret->mkdir(empty)) {
		m_tempFolders.add(ret);
		return ret;
	}

	// If -forceunlink specified have a go at cleaning up and retry
	if(getenv("triforceunlink")) {
		debug1("mkdir '%s' failed - trying to delete and recreate", ret->pathname());
		if(ret->unlink()) {
			if(ret->mkdir(true)) {
				m_tempFolders.add(ret);
				return ret;
			}
		}
	}

	return NULL;
}


TempFolder *DM::getTemporaryFolder(const char *deployId)
{
	TempFolder tmp(deployId);

	ListIterator<TempFolder> iter(m_tempFolders);
	for(TempFolder *f = iter.first(); f; f = iter.next()) {
		if(strcmp(f->pathname(), tmp.pathname()) == 0) {
			return f;
		}
	}
	return NULL;
}


void DM::removeTemporaryFolder(TempFolder *folder)
{
	m_tempFolders.remove(folder);
}


void DM::deleteTemporaryFilesAndFolders()
{
	if(!getenv("trinounlink")) {
		writeToLogFile("Removing temporary files");

		ListIterator<TempFile> fit(m_tempFiles);
		for(TempFile *f = fit.first(); f; f = fit.next()) {
			if(!f->unlink()) {
				writeToLogFile("Failed to remove temporary file \"%s\"", f->filename());
			}
		}

		ListIterator<TempFolder> dit(m_tempFolders);
		for(TempFolder *d = dit.first(); d; d = dit.next()) {
			if(!d->unlink()) {
				writeToLogFile("Failed to remove temporary folder \"%s\"", d->pathname());
			}
		}
	}
}


Dropzone *DM::getDropzone(const char *name)
{
	return m_dropzones.get(name);
}


Dropzone *DM::newDropzone(const char *name, bool autoUnlink)
{
	Dropzone *ret = new Dropzone(*this, name, autoUnlink);
	m_dropzones.put(ret->name(), ret);
	return ret;
}


void DM::removeDropzone(Dropzone *dropzone)
{
	dropzone->remove();
	m_dropzones.remove(dropzone->name());		// This will delete the dropzone object
}


char *DM::getHostname()
{
	char hostname[1024];
	gethostname(hostname, sizeof(hostname));
	return strdup(hostname);
}


typedef int (*PLUGIN_START_FN_PTR)(DM&); 


bool DM::loadPlugin(const char *plugin)
{
	PLUGIN_START_FN_PTR fn = NULL;

#ifdef WIN32
	HINSTANCE handle = LoadLibrary(plugin);

#else
	char buf[1024];
	sprintf(buf, "lib%s.so", plugin);
	void *handle = dlopen(buf, RTLD_LAZY);
#endif /*WIN32*/
	
	if(handle) {
#ifdef WIN32
		fn = (PLUGIN_START_FN_PTR) GetProcAddress(handle, "PluginStart");
#else
		fn = (PLUGIN_START_FN_PTR) dlsym(handle, "PluginStart");
#endif /*WIN32*/
		if(!fn) {
			writeToLogFile("Failed to find required functions in plugin \"%s\".", plugin);
#ifdef WIN32
			FreeLibrary(handle);
#else
			dlclose(handle);
#endif /*WIN32*/
		}
	} else {
		writeToLogFile("Failed to find plugin \"%s\".", plugin);
	}

	if(fn)
	{
		// Invoke the startup function.
		if((*fn)(*this) == 0) {
			writeToLogFile("Plugin \"%s\" loaded.", plugin);
			return true;
		} 

		writeToLogFile("Plugin \"%s\" failed to initialise.", plugin);
	}

	return false;
}


bool DM::installPlugin(const char *plugin)
{
	PLUGIN_START_FN_PTR fn = NULL;

#ifdef WIN32
	HINSTANCE handle = LoadLibrary(plugin);
#else
	char buf[1024];
	sprintf(buf, "%s.so", plugin);
	void *handle = dlopen(buf, RTLD_LAZY);
#endif /*WIN32*/
	
	if(handle) {
#ifdef WIN32
		fn = (PLUGIN_START_FN_PTR) GetProcAddress(handle, "PluginInstall");
#else
		fn = (PLUGIN_START_FN_PTR) dlsym(handle, "PluginInstall");
#endif /*WIN32*/
		if(!fn) {
			//writeToLogFile("Failed to find required functions in plugin \"%s\".", plugin);
			printf("Failed to find required functions in plugin \"%s\".", plugin);
#ifdef WIN32
			FreeLibrary(handle);
#else
			dlclose(handle);
#endif /*WIN32*/
		}
	} else {
		//writeToLogFile("Failed to find plugin \"%s\".", plugin);
	}

	if(fn)
	{
		// Invoke the install function.
		if((*fn)(*this) == 0) {
			//writeToLogFile("Plugin \"%s\" installed.", plugin);
			printf("Plugin \"%s\" installed.",plugin);
			return true;
		}

		printf("Plugin \"%s\" failed to install.", plugin);
	}

	return false;
}


bool DM::installProviderImpl(
	const char *name, const char *plugin,
	int kind, List<class PropertyDef> *propdefs)
{
	return m_model ? m_model->installProviderImpl(name, plugin, kind, propdefs) : false;
}


int DM::parse(const char *filename)
{
	char *input = (char *)0;
	bool psi=false;
	if (filename[0]=='^' && filename[1]=='\0') {
		// Parsing Standard Input
		int bytesread;
		int ts=0;
		char inbuf[10];
		while(bytesread = read(0,inbuf,sizeof(inbuf))) {
			input=(char *)((!input)?malloc(bytesread):realloc(input,ts+bytesread));
			memmove(&(input[ts]),inbuf,bytesread);
			ts=ts+bytesread;
		}
		input=(char *)realloc(input,ts+1);
		input[ts]='\0';
		psi=true;
	} else {
		// Conventional File
		struct stat sb;
		if(stat(filename, &sb) != 0) {
			fprintf(stderr, "Input file '%s' not found\n", (const char*) filename);
			return 1;
		}

		int buflen = sb.st_size;
		input = (char*) malloc(buflen + 1);
		FILE *f = fopen(filename, "r");
		if(!f) {
			fprintf(stderr, "Error opening '%s'\n", (const char*) filename);
			return 1;
		}

		int read = fread(input, 1, buflen, f);
		fclose(f);

		if(read == 0) {
			return 1;
		}

		input[read] = '\0';
	}
	LexerBuffer lb(action_lexer, input, psi?NULL:filename);

	yyparse_param param;
	param.buffer = &lb;
	int res = yyparse(&param);

	SAFE_FREE(input);			// we have finished with the input buffer now
	// TODO: if an exception is raised we still need to free the memory try catch(...) throw???

	if(res != 0) {
		if(param.ex) {
			param.ex->print(*this);
		}

		return 2;
	}

	if(!param.actnlist) {
		return 3;
	}

	if(!getenv("trinosemantic")) {
		param.actnlist->semanticCheck(*this);
	}

	return 0;
}

int DM::importScript()
{
	char buf[1024];
	int n;
	int tlen=0;
	char *inputdata = (char *)0;
	while (n=read(0,buf,sizeof(buf))) {
		inputdata = (char *)(inputdata?realloc(inputdata,tlen+n):malloc(n));
		memcpy(&(inputdata[tlen]),buf,n);
		tlen+=n;
	}

	TiXmlDocument doc;
	doc.Parse(inputdata);
	if(!doc.Error()) {
		TiXmlElement *root = doc.FirstChildElement("action");
		if (root) {
			root->SetCondenseWhiteSpace(false);
			TiXmlElement *scripts = root->FirstChildElement("scripts");
			if (scripts) {
				for(TiXmlElement *file = scripts->FirstChildElement("scriptbody"); scripts; scripts = scripts->NextSiblingElement("scriptbody")) {
					const char *b64 = file->GetText();
					const char *name = file->Attribute("filename");
					if (name) {
						// name attribute is present
						char *tgtfilename = (char *)malloc(strlen(m_dmhome)+strlen(name)+50);
						sprintf(tgtfilename,"%s/scripts/%s",m_dmhome,name);
						// tinyxml will replace newlines with spaces. Space is not a valid base64 character so
						// remove it here.
						char *nptr = (char *)malloc(strlen(b64));
						char *p=nptr;
						char *q=(char *)b64;
						while (*q) {
							if (*q != ' ') *p++ = *q;
							q++;
						}
						*p='\0';
						int retlen;
						unsigned char *scriptdata = decryptbase64(nptr,&retlen);
						free(nptr);
						int out = open(tgtfilename,_O_WRONLY | _O_CREAT);
						if (out != -1) {
							write(out,scriptdata,retlen);
							close(out);
							chmod(tgtfilename,0755);
						} else {
							exitWithError("Failed to open script file %s for writing",tgtfilename);
						}
						free(tgtfilename);
					} else {
						exitWithError("XML Parse Error: \"name\" attribute not set");
					}
				}
			} else {
				fprintf(stderr,"No <scripts> tag found\n");
				return 1;
			}
		} else {
			fprintf(stderr,"No <action> tag found\n");
			return 1;
		}
	} else {
		fprintf(stderr,"Error parsing XML: %s\n",doc.ErrorDesc());
		return 1;
	}
	return 0;
}

int DM::DumpScript(int actionid)
{
	char *FileExts[]={
	"vbs",
	"cmd",
	"bat",
	"exe"
	};
	int NumExts = sizeof(FileExts) / sizeof(FileExts[0]);

	printf("<scripts>\n");

	Action *a = m_model->getAction(actionid);
	bool extvar=(strstr(a->filepath(),"$EXT") || strstr(a->filepath(),"${EXT}"));
	if (!a) return 1;
	Script *s = a->getScript();
	List<Server> emptySet;
	Context ctx(*this, emptySet, m_stack);
	int i;
	unsigned char *res=(unsigned char *)0;
	char *stext1;
	if (extvar) {
		// Replace $EXT with the different permutations for Windows
		for (i=0;i<NumExts;i++) {
			struct stat t;
			m_stack.setGlobal("EXT",FileExts[i]);
			char *szFilepath = (char *)strdup(a->filepath());
			Node expandedText1(NODE_STR, szFilepath, true);	// will free szFilepath on delete
			ExprPtr etext1 = expandedText1.evaluate(ctx);
			stext1 = etext1->stringify();
			if (stat(stext1,&t)==0) {
				// found matching file
				res = (unsigned char *)s->getFileContent(stext1);
				break;
			} else {
				// no match
				SAFE_FREE(stext1);
			}
		}
	} else {
		// EXT not present. Just grab the file. Will still need to expand other variables (like DMHOME)
		char *szFilepath = (char *)strdup(a->filepath());
		Node expandedText1(NODE_STR, szFilepath, true);	// will free szFilepath on delete
		ExprPtr etext1 = expandedText1.evaluate(ctx);
		stext1 = etext1->stringify();
		struct stat t;
		if (stat(stext1,&t)==0) {
			// found it
			res = (unsigned char *)s->getFileContent(stext1);
		}
	}
	if (res) {
		char *tptr = stext1;
		char *tgtfile = stext1;
		while (*tptr) {
			if (*tptr=='/' || *tptr=='\\') tgtfile=tptr+1;
			tptr++;
		}
		printf("<scriptbody filename=\"%s\">\n",tgtfile);
		char *b64 = base64encode(res,strlen((const char *)res));
		printf(b64);
		printf("\n");
		printf("</scriptbody>\n");
	}
	//
	// Check if there's a Unix/Linux version of the same script
	//
	if (extvar) {
		struct stat t;
		m_stack.setGlobal("EXT","sh");
		char *szFilepath = (char *)strdup(a->filepath());
		Node expandedText1(NODE_STR, szFilepath, true);	// will free szFilepath on delete
		ExprPtr etext1 = expandedText1.evaluate(ctx);
		stext1 = etext1->stringify();
		if (stat(stext1,&t)==0) {
			// yes there is!
			res = (unsigned char *)s->getFileContent(stext1);
			char *tptr = stext1;
			char *tgtfile = stext1;
			while (*tptr) {
				if (*tptr=='/' || *tptr=='\\') tgtfile=tptr+1;
				tptr++;
			}
			printf("<scriptbody filename=\"%s\">\n",tgtfile);
			char *b64 = base64encode(res,strlen((const char *)res));
			printf(b64);
			printf("\n");
			printf("</scriptbody>\n");
		}
	}
	SAFE_FREE(res);
	SAFE_FREE(stext1);
	printf("</scripts>\n");
	return 0;
}


int DM::doNotify(int nfyid, const char *from, const char *subject, int templateid)
{
	if(!m_model) {
		return -1;
	}

	try {
		Notify *notify = m_model->getNotifier(nfyid);
		if(!notify) {
			writeToStdOut("Notify %d not found or not configured", nfyid);
			return 1;
		}

		Property *pfrom = notify->getProperty("from");
		if (pfrom) from = pfrom->value();	// ignore any passed "from" field

		DMArray *tos = NULL;
		Variable *var = m_stack.get("ARGV");
		if(var) {
			tos = var->getArray();
		}
		if(!tos || (tos->count() == 0)) {
			writeToStdOut("No recipients specified");
			return 1;
		}

		DMArray *recipients = m_stack.newGlobalArray("RECIPIENTS", true);

		AutoPtr<StringList> keys = tos->keys();
		StringListIterator iter(*keys);
		for(const char *key = iter.first(); key; key = iter.next()) {
			var = tos->get(key);
			if(var) {
				ConstCharPtr sotid = var->toString();
				if(sotid) {
					ObjectKindAndId otid(sotid);
					Object *obj = NULL;
					switch(otid.getObjectKind()) {
					case OBJ_KIND_USER:
						obj = m_model->getUserById(otid.getId());
						break;
					case OBJ_KIND_USERGROUP:
						obj = m_model->getUserGroup(otid.getId());
						break;
					default:
						writeToStdOut("Invalid recipient type: '%s'", (const char *)sotid);
						continue;
					}
					if(obj) {
						recipients->add(new Variable(NULL, obj));
					}
				}
			}
		}

		List<Server> emptySet;
		Context ctx(*this, emptySet, m_stack);

		bool envPushed = false;
		if(getTargetEnvironment()) {
			m_stack.push(getTargetEnvironment()->getVars());
			envPushed = true;
		}

		bool appPushed = false, parentPushed = false;
		Application *app = getTargetApplication();
		if(app) {
			// If the app is an app ver, then push the parent application first
			if(app->kind() == OBJ_KIND_APPVERSION) {
				Application *parent = app->getParent();
				if(!parent) {
					throw RuntimeError(m_stack, "Application version '%s' has no parent", app->name());
				}
				m_stack.push(parent->getVars());
				parentPushed = true;
			}
			m_stack.push(app->getVars());
			appPushed = true;
		}

		StmtList *args = new StmtList();
		// TODO: allow multiple recipients
		args->add(new Stmt(strdup("to"), new Node(NODE_ARRAY, new Node(NODE_NLVAR, new Node(NODE_IDOROBJ, strdup("RECIPIENTS"))), new Node(0))));
		args->add(new Stmt(strdup("from"), new Node(NODE_STR, strdup(from))));
		args->add(new Stmt(strdup("subject"), new Node(NODE_STR, strdup(subject))));
		ExtendedStmt stmt(strdup("notify"), args);
		Audit &audit = getDummyAudit();
		AuditEntry *ae = audit.newAuditEntry("notify");
		OutputStream body;
		if(templateid != 0) {
			char *text = m_model->getNotifyText(templateid);
			if(!text) {
				throw RuntimeError("Template %d is empty - message not sent", templateid);
			}
			Node expandedText(NODE_STR, text, true);	// will free text on delete
			ExprPtr etext = expandedText.evaluate(ctx);
			ConstCharPtr stext = etext->stringify();
			body.writeToStdOut("%s", (const char*) stext);
		} else {
			body.readFromStdIn();
		}

		notify->notify(1, stmt, ae, body, NULL, ctx);

		if(appPushed) {
			m_stack.pop(APPLICATION_SCOPE);
		}
		if(parentPushed) {
			m_stack.pop(APPLICATION_SCOPE);
		}
		if(envPushed) {
			m_stack.pop(ENVIRONMENT_SCOPE);
		}
		return 0;
	} catch(DMException &e) {
		fprintf(stderr, "Exception: %s\n", e.getMessage());
		return 1;
	} catch(...) {
		fprintf(stderr, "Unhandled exception\n");
	}

	return -1;
}


int DM::providerTest(OBJECT_KIND kind, int id, char *recipient)
{
	if(!m_model) {
		return -1;
	}

	try {
		switch(kind) {
		case OBJ_KIND_REPOSITORY: {
			Repository *repo = m_model->getRepository(id);
			if(!repo) {
				writeToStdOut("Repository %d not found or not configured", id);
				return 1;
			}

			List<Server> emptySet;
			Context ctx(*this, emptySet, m_stack);

			ExtendedStmt stmt(strdup("checkout"), new StmtList());
			if(!repo->testProvider(stmt, ctx)) {
				writeToStdOut("Test of repository \"%s\" failed!", repo->name());
				return 1;
			}
			}
			return 0;

		case OBJ_KIND_NOTIFY: {
			Notify *notify = m_model->getNotifier(id);
			if(!notify) {
				writeToStdOut("Notify %d not found or not configured", id);
				return 1;
			}

			List<Server> emptySet;
			Context ctx(*this, emptySet, m_stack);

			ExtendedStmt stmt(strdup("notify"), new StmtList());
			if(!notify->testProvider(stmt, ctx)) {
				writeToStdOut("Test of notify \"%s\" failed!", notify->name());
				return 1;
			}

			StmtList *args = new StmtList();
			if (recipient) {
				args->add(new Stmt(strdup("to"), new Node(NODE_STR, strdup(recipient))));
			} else {
				args->add(new Stmt(strdup("to"), new Node(getCurrentUser())));
			}
			args->add(new Stmt(strdup("subject"), new Node(NODE_STR, strdup("Test notification from Openmake DeployHub"))));
			args->add(new Stmt(strdup("logfile"), new Node(NODE_STR, strdup("$DMHOME/smtp.log"), true)));
			args->add(new Stmt(strdup("isTestMode"), new Node(true)));	// Turn on log to screen
			ExtendedStmt stmt2(strdup("notify"), args);
			Audit &audit = getDummyAudit();
			AuditEntry *ae = audit.newAuditEntry("notify");
			OutputStream body;
			body.writeToStdOut("Test notification from Openmake DeployHub");

			//body.writeToStdOut("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Typi non habent claritatem insitam; est usus legentis in iis qui facit eorum claritatem. Investigationes demonstraverunt lectores legere me lius quod ii legunt saepius. Claritas est etiam processus dynamicus, qui sequitur mutationem consuetudium lectorum. Mirum est notare quam littera gothica, quam nunc putamus parum claram, anteposuerit litterarum formas humanitatis per seacula quarta decima et quinta decima. Eodem modo typi, qui nunc nobis videntur parum clari, fiant sollemnes in futurum.");
			//body.writeToStdOut("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas pretium lacinia quam nec malesuada. In vehicula orci vitae lacus convallis accumsan. Etiam eleifend metus eu fringilla viverra. Etiam pretium eleifend tellus, at condimentum velit fermentum non. Phasellus dictum ultrices ullamcorper. Proin vestibulum aliquam mauris at fringilla. Aliquam erat volutpat. Nam id sapien a orci vulputate viverra in auctor elit. Nulla facilisi. In vitae tortor libero.\n"
			//	"Nulla viverra, libero et euismod fringilla, urna odio malesuada neque, at ornare urna libero id metus. Sed fermentum, tortor nec eleifend molestie, enim elit varius augue, nec luctus risus nisl a velit. Donec blandit blandit sapien in accumsan. Phasellus dignissim nec lacus et pulvinar. Donec semper urna vitae lacinia interdum. Morbi tellus purus, pharetra eget risus non, volutpat venenatis dolor. Pellentesque a lorem pulvinar, aliquam odio ut, elementum ligula. Proin et scelerisque turpis, vitae sodales diam. Proin ultricies sed ligula quis porttitor. Suspendisse tincidunt nibh nec feugiat eleifend. Curabitur rhoncus tincidunt varius.\n"
			//	"Integer tempor odio at tortor aliquet hendrerit. Nullam rhoncus gravida auctor. Pellentesque vitae massa vel mi congue bibendum. Mauris porttitor mattis massa eu vestibulum. Quisque auctor viverra condimentum. Duis felis sapien, porttitor et massa sed, sodales gravida massa. Nulla mollis arcu non consectetur vehicula.\n"
			//	"Maecenas interdum neque diam, iaculis tincidunt ligula adipiscing vel. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Fusce lobortis eros eget suscipit consectetur. Aenean suscipit sem in rhoncus mattis. Suspendisse placerat pretium nulla eget adipiscing. Aliquam et imperdiet lectus. Nulla at orci pharetra lorem sollicitudin malesuada. Suspendisse blandit convallis purus.\n"
			//	"Donec porttitor lobortis justo vel consectetur. Integer sit amet blandit tortor. Phasellus tortor arcu, gravida at erat vel, faucibus imperdiet odio. Quisque non accumsan metus, non porta tortor. Morbi in erat faucibus, consequat mi sit amet, imperdiet lorem. Phasellus varius, odio ut pulvinar lacinia, magna felis viverra sapien, eget porttitor odio quam in ligula. Vestibulum molestie, nulla ut aliquet blandit, massa sem luctus massa, ac dignissim elit velit a augue. Integer viverra sit amet orci ac commodo. Fusce dictum euismod urna vel porttitor.\n"
			//	"Cras blandit egestas mauris et gravida. Duis lorem diam, tempor in vestibulum sit amet, vestibulum ut massa. Vestibulum ipsum libero, ornare eu elementum accumsan, consectetur at enim. Aliquam dignissim velit sed metus rutrum, id feugiat orci tempus. Duis id hendrerit lorem, sit amet hendrerit sem. Pellentesque volutpat nisl sed erat fringilla, nec porttitor augue interdum. Praesent molestie in odio eu feugiat.\n"
			//	"Pellentesque vitae auctor dui, ut pretium dolor. Nullam aliquam semper ornare. Vivamus eu erat eget dui auctor vulputate. Donec ornare id libero id venenatis. Nulla facilisi. Pellentesque nec rhoncus augue. Donec faucibus egestas mattis. Suspendisse potenti. Integer nisl risus, elementum ac odio non, luctus pulvinar tortor. Pellentesque vestibulum purus vel odio rhoncus, vitae interdum lorem pharetra. Vivamus ultricies volutpat est, at vulputate magna egestas at. Integer ac blandit mauris, nec posuere elit.\n"
			//	"Suspendisse potenti. Sed et posuere lorem. Ut sagittis erat ut ante porta, vitae interdum dui lobortis. Cras posuere condimentum mauris. Morbi iaculis nibh a quam eleifend imperdiet. Proin in placerat arcu. Vestibulum pulvinar tortor enim. Integer imperdiet, elit at condimentum tempus, nulla lacus lacinia risus, eu porttitor velit libero id risus. Fusce pulvinar tristique posuere. Quisque porta lorem at interdum fringilla. Maecenas imperdiet tellus eu hendrerit ultrices. Donec fermentum volutpat consequat. Aliquam eu dolor feugiat, aliquet mauris non, cursus nisi. Phasellus eu dui ac dui consectetur tincidunt.\n"
			//	"Duis ultricies enim convallis ultrices consequat. Aenean ornare egestas ipsum eu eleifend. Mauris sed nisl ornare, sodales mauris nec, interdum massa nunc.");
			notify->notify(1, stmt2, ae, body, NULL, ctx);
			}
			return 0;
		case OBJ_KIND_DATASOURCE: {
			Datasource *ds = m_model->getDatasource(id);
			if(!ds) {
				writeToStdOut("Datasource %d not found or not configured", id);
				return 1;
			}

			List<Server> emptySet;
			Context ctx(*this, emptySet, m_stack);

			ExtendedStmt stmt(strdup("query"), new StmtList());
			Credentials *creds = ds->getCredentials();
			if (!creds) {
				// No credentials provided, use username/password attributes
				AutoPtr<Credentials> cap;
				// Create a credentials object from the username and password properties
				Property *username = ds->getProperty("username");
				Property *password = ds->getProperty("password");
				if (username && password) {
					Credentials *tcred = new Credentials(*m_model,username, password);
					User *me = getCurrentUser();
					tcred->setDomain(me->getDomain());	// user's base domain
					ds->setCredentials(tcred);
				}
			}
			if(!ds->testProvider(stmt, ctx)) {
				writeToStdErr("Test of datasource \"%s\" failed!", ds->name());
				return 1;
			}
			}
			return 0;
		}
	} catch(DMException &e) {
		fprintf(stderr, "Exception: %s\n", e.getMessage());
		return 1;
	} catch(...) {
		fprintf(stderr, "Unhandled exception\n");
	}
	return -1;
}


void DM::writeToLogFile(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if(/*m_Logging*/ true) {
		char *logFileName;
		if(m_dmhome) {
			logFileName = (char *) malloc(strlen(m_dmhome) + 15);
			sprintf(logFileName, "%s%s%s", m_dmhome, DIR_SEP_STR, "dm.log");	// RHT 19/02/2014 - log should go in home
		} else {
			logFileName = strdup("dm.log");
		}
		FILE *out = fopen(/*m_LogFileName*/ logFileName, "a+");
		if (out) {
			time_t timenow;
			struct tm *t;
			time(&timenow);
			t = localtime(&timenow);
			fprintf(out,"%04d/%02d/%02d %02d:%02d:%02d [%d] [0x%08lx] ",
				t->tm_year+1900, t->tm_mon+1, t->tm_mday,
				t->tm_hour, t->tm_min, t->tm_sec,
				getpid(), Thread::currentThreadId());
			vfprintf(out, fmt, args);
			fprintf(out, "\n");
			fclose(out);
		} else {
			throw RuntimeError("Could not write to logfile \"%s\"", /*m_LogFileName*/ logFileName);
		}
		SAFE_FREE(logFileName);
	}
	va_end(args);
}


void DM::writeToStdOut(const char* fmt, ...)
{
	char *temp=NULL;
	int len = 0;
	va_list args;
	va_start(args, fmt);
	vasprintf(&temp,fmt,args);
	va_end(args);
	
	writevToStdOut(0, temp);
	free(temp);
}


void DM::writevToStdOut(long threadId, const char* buffer)
{
	int len = 0;
	fprintf(stdout, buffer);
	fprintf(stdout, "\n");
	
	// Also record in db log
	if(m_model) {
		Thread::lock(__LINE__,__FILE__);
		len=strlen(buffer);
		m_model->getAudit().writeBufferToAuditLog(1, Thread::currentThreadId(), buffer, len);
		Thread::unlock(__LINE__,__FILE__);
	}
}


void DM::writeBufferToStdOut(long threadId, const char *buffer, int len)
{
	write(1, buffer, len);

	if (!getenv("triscanserver")) {
		// Also record in db log (if we're not running a server MD5 scan)
		if(m_model) {
			m_model->getAudit().writeBufferToAuditLog(1, threadId, buffer, len);
		}
	}
}


void DM::writeToStdErr(const char* fmt, ...)
{
	char *temp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&temp,fmt,args);
	va_end(args);
	writevToStdErr(0, temp);
	free(temp);
}


void DM::writevToStdErr(long threadId, const char *buffer)
{
	bool useColor = getenv("triusecolor") ? true : false;
	if(useColor) {
		fprintf(stderr, "\x1b[31m");
	}
	fprintf(stderr, buffer);
	fprintf(stderr, (useColor ? "\x1b[0m\n" :  "\n"));

	// Also record in db log
	if(m_model) {
		m_model->getAudit().writevToAuditLog(2, threadId, buffer);
	}
}


void DM::writeBufferToStdErr(long threadId, const char *buffer, int len)
{
	bool useColor = getenv("triusecolor") ? true : false;
	if(useColor) {
		fprintf(stderr, "\x1b[31m");
	}
	write(2, buffer, len);
	if(useColor) {
		fprintf(stderr, "\x1b[0m");
	}

	if (!getenv("triscanserver")) {
		// Also record in db log (if we're not running a server MD5 scan)
		if(m_model) {
			m_model->getAudit().writeBufferToAuditLog(2, threadId, buffer, len);
		}
	}
}


IOutputStream *DM::toOutputStream()
{
	return this;
}


void DM::exitWithError(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	fprintf(stderr,"\n");

	if(/*m_Logging*/ true) {
		FILE *out = fopen(/*m_LogFileName*/ "dm.log", "a+");
		if (out) {
			time_t timenow;
			struct tm *t;
			time(&timenow);
			t = localtime(&timenow);
			fprintf(out,"%04d/%02d/%02d %02d:%02d:%02d [%d] [0x%08lx] ",
				t->tm_year+1900, t->tm_mon+1, t->tm_mday,
				t->tm_hour, t->tm_min, t->tm_sec,
				getpid(), Thread::currentThreadId());
			vfprintf(out, fmt, args);
			fprintf(out, "\n");
			fclose(out);
		} else {
			fprintf(stderr, "Could not write to logfile \"%s\"", /*m_LogFileName*/ "dm.log");
		}

	}
	va_end(args);
	exit(1);
}


const char *DM::getDefaultFields()
{
	return "environments,username,password,applications";
}


const char *DM::getTriField(int field)
{
	char envvar[16];
	sprintf(envvar, "TRIFIELD%d", field);
	return getenv(envvar);
}


void DM::registerBuiltins()
{
	// Setup the built in statements (deploy etc.) and functions
	ExtendedStmtImplRegistry::instance().registerBuiltIns();
	FunctionNodeImplRegistry::instance().registerBuiltIns();

	// Setup the built in repositories (harvest etc.)
	RepositoryImplRegistry::instance().registerBuiltIns(*this);

	// Setup the built in transfer providers (ftp etc.)
	TransferProviderImplRegistry::instance().registerBuiltIns();

	// Setup the built in notify providers
	NotifyProviderImplRegistry::instance().registerBuiltIns(*this);

	// Setup the built in datasource providers
	DatasourceProviderImplRegistry::instance().registerBuiltIns(*this);

	// Setup the built in modifiers
	ModifyProviderImplRegistry::instance().registerBuiltIns(*this);

	// Setup the built in path formats
	PathNameImplRegistry::instance().registerBuiltIns();

	// Setup the built in tasks
	TaskImplRegistry::instance().registerBuiltIns();
}


void DM::setEnvironmentVariables(char **envp)
{
	while(envp && *envp) {
		char *dstr = strdup(*envp);
		char *eq = strchr(dstr,'=');
		if(eq) { *eq = '\0'; }
		char *name = dstr;
		char *value = (eq+1);
		m_stack.setGlobal(name, value);
		free(dstr);
		envp++;
	}
	//
	// Add session id (if set). This will allow API calls made from DMScript to
	// login automatically without another explicit login.
	//
	if (getenv("trisessionid")) {
		m_stack.setGlobal("JSESSIONID",getenv("trisessionid"));
	}
}


/**
 * Any remaining command line arguments that are not preceded by a switch come
 * here. Two kinds of argument are supported: name/value pairs and positional
 * arguments. Name/value pairs must appear first, followed by positional
 * arguments:
 *
 *   dm ... name1=value1 name2=value2 arg1 arg2
 *
 * Name/value pairs result in a global variable of the same name being set.
 * Positional arguments go into a global variable called "ARGV" and a variable
 * called "ARGC" is set to the count of these arguments.
 */
void DM::setCommandLineVariables(char **argv)
{
	// Handle name/value pairs first
	for(; *argv; argv++) {
		if(!strstr(*argv, "=")) {
			// End of name/value pairs
			break;
		}

		char *arg = strdup(*argv);
		char *eq = strstr(arg, "=");
		*eq = '\0'; eq++;
		m_stack.setGlobal(arg, eq);
		free(arg);
	}

	// Now deal with positional arguments
	DMArray *ht = m_stack.newGlobalArray("ARGV");
	int argc = 0;

	for(; *argv; argv++) {
		char key[32];
		sprintf(key, "%d", argc);
		ht->put(key, new Variable(NULL, *argv));
		argc++;
	}

	m_stack.setGlobal("ARGC", argc);
}

void DM::initialize(const char *baseDir, char **argv, char **envp)
{
	if(m_initializationDone) {
		return;
	}

	m_initializationDone = true;

	registerBuiltins();

	m_BaseDir = baseDir;
	m_argv = argv;
	m_envp = envp;

	// Set up the stack with env vars, trifield array and globals
	setEnvironmentVariables(envp);
	setCommandLineVariables(argv);

	m_stack.setGlobal("DMHOME", baseDir);
	m_stack.setGlobal("TRIDM_PLATFORM", DM_PLATFORM);
	debug2("Setting $$ = %d", getpid());
	m_stack.setGlobal("$", getpid());
}

const char *DM::getBaseDir()
{
	return m_BaseDir;
}

char **DM::getArgv()
{
	return m_argv;
}

char **DM::getEnvp()
{
	return m_envp;
}

void DM::removeRequest(int taskid)
{
	Environment *EnvOnStack = getTargetEnvironment();
	List<Server> emptyList;
	Context ctx(*this, EnvOnStack?*(EnvOnStack->getServers()):emptyList, m_stack);
	m_model->removeRequest(taskid,ctx);
}

int DM::runLinkedAction(Action &action)
{
	Environment *EnvOnStack = getTargetEnvironment();
	List<Server> emptyList;
	Context ctx(*this, EnvOnStack?*(EnvOnStack->getServers()):emptyList, m_stack);
	if (action.actionKind() == ACTION_KIND_REMOTE_EXTERNAL) {
		throw new RuntimeError("Cannot execute remote external as a pre/post link action");
	}
	if (action.actionKind() == ACTION_KIND_LOCAL_EXTERNAL) {
		int res = internalRunAction(action,&ctx);
		if (res == 0) {
			Variable *v = ctx.stack().get("?");
			if (v) return v->getInt();
			else throw new RuntimeError("$? not set following successful internalRunAction");
		}
		return res;
	} else {
		ActionNode *act = action.getActionNode(ctx);
		ACTION_KIND res;
		
		if(act) {
			const char *scopeName = act->isFunction() ? FUNCTION_SCOPE : ACTION_SCOPE;
			Scope *actscope = new Scope(scopeName, act, *act, true);
			ctx.stack().push(actscope);
			try {
				act->execute(ctx);
			} catch(ReturnException &e) {
				if(e.expr() && (e.expr()->toInt() != 0)) {
					ctx.dm().writeToStdOut("action returned %d", e.expr()->toInt());
					ctx.stack().pop(scopeName);
					return e.expr()->toInt();
				}
				ctx.stack().pop(scopeName);
				return 0;
			} catch(...) {
				ctx.stack().pop(scopeName);
				throw;
			}
			ctx.stack().pop(scopeName);
		}
	}
	return 0;
}

int DM::runAction(Action &action)
{
	if(!m_initializationDone) {
		writeToStdErr("FATAL ERROR: Initialization not done before running script");
		return 1;
	}

	debug2("Running action '%s'", action.name());

	bool envPushed = false, appPushed = false, parentPushed = false;

	if(getTargetEnvironment()) {
		m_stack.push(getTargetEnvironment()->getVars());
		envPushed = true;
	}

	Application *app = getTargetApplication();
	if(app) {
		// If the app is an app ver, then push the parent application first
		if(app->kind() == OBJ_KIND_APPVERSION) {
			Application *parent = app->getParent();
			if(!parent) {
				throw RuntimeError(m_stack, "Application version '%s' has no parent", app->name());
			}
			m_stack.push(parent->getVars());
			parentPushed = true;
		}
		m_stack.push(app->getVars());
		appPushed = true;
	}

	int ret = internalRunAction(action);

	try {
		if(appPushed) {
			m_stack.pop(APPLICATION_SCOPE);
		}
		if(parentPushed) {
			m_stack.pop(APPLICATION_SCOPE);
		}

		if(envPushed) {
			m_stack.pop(ENVIRONMENT_SCOPE);
		}
	} catch(DMException &e) {
		writeToLogFile("Caught exception (2)");
		e.print(*this);
		ret = 3;
	} catch(...) {
		writeToStdErr("FATAL ERROR: Unhandled exception");
		writeToLogFile("Unhandled exception (2)");
		ret = 3;
	}

	return ret;
}


int DM::runTask(Task &task)
{
	if(!m_initializationDone) {
		writeToStdErr("FATAL ERROR: Initialization not done before running task");
		return 1;
	}

	bool envPushed = false, appPushed = false, compPushed = false, parentPushed = false;

	if(getTargetEnvironment()) {
		m_stack.push(getTargetEnvironment()->getVars());
		envPushed = true;
	}

	char *szCompID = getenv("tricompid");
	if (szCompID) {
		// Component specified
		int compid = atol(szCompID);
		if (compid>0) {
			Component *comp = getComponentById(compid);
			if (comp) {
				// Component found - push it onto stack
				m_stack.push(comp->getVars());
				compPushed = true;
			} else {
				compid=0;	// failed to get component by id
			}
		}
		if (compid<=0) {
			writeToStdErr("FATAL ERROR: Component ID %s invalid or not found",szCompID);
			return 1;
		}
	}

	Application *app = getTargetApplication();
	if(app) {
		// If the app is an app ver, then push the parent application first
		if(app->kind() == OBJ_KIND_APPVERSION) {
			Application *parent = app->getParent();
			if(!parent) {
				//throw RuntimeError(m_stack, "Application version '%s' has no parent", app->name());
				writeToStdErr("FATAL ERROR: Application version '%s' has no parent", app->name());
				return 1;
			}
			m_stack.push(parent->getVars());
			parentPushed = true;
		}
		m_stack.push(app->getVars());
		appPushed = true;
	}

	StmtList *args = new StmtList();

	// Add the appropriate arguments for each task type
	if(strcmp(task.taskType(), "approve") == 0) { 
		OutputStream note;
		note.readFromStdIn();
		args->add(new Stmt(strdup("approve"), new Node(NODE_LOOKUP, strdup("approve_approved"))));
		args->add(new Stmt(strdup("note"), new Node(NODE_STR, DUP_NULL(note.buffer()))));
	} else if((strcmp(task.taskType(), "move") == 0)
			|| (strcmp(task.taskType(), "copy") == 0)
			|| (strcmp(task.taskType(), "request") == 0)) {
		OutputStream note;
		note.readFromStdIn();
		args->add(new Stmt(strdup("note"), new Node(NODE_STR, DUP_NULL(note.buffer()))));
	} else if(strcmp(task.taskType(), "createversion") == 0) {
		Application *predecessor = getSecondApplication();
		if(predecessor) {
			debug1("predecessor is %d '%s'", predecessor->id(), predecessor->name());
			args->add(new Stmt(strdup("predecessor"), new Node(predecessor)));
		}
	}

	ExtendedStmt stmt(strdup("runtask"), args);
	Audit &audit = getDummyAudit();
	AuditEntry *ae = audit.newAuditEntry("task");

	int ret = -1;
	try {
		if(getTargetEnvironment()) {
			Context ctx(*this, *(getTargetEnvironment()->getServers()), m_stack);
			Scope *scope = task.getVars(1, stmt, ae, ctx);
			//scope->setStatementObject(this);
			//Scope *scope = new Scope(TASK_SCOPE, task);
			ctx.stack().push(scope);
			try {
				ret = task.run(1, stmt, ae, ctx);
			} catch(...) {
				m_stack.pop(TASK_SCOPE);
				throw;
			}
			m_stack.pop(TASK_SCOPE);
		} else {
			List<Server> emptyList;
			Context ctx(*this, emptyList, m_stack);
			Scope *scope = task.getVars(1, stmt, ae, ctx);
			// scope->setStatementObject(this);
			// Scope *scope = new Scope(TASK_SCOPE, task);
			ctx.stack().push(scope);
			try {
				ret = task.run(1, stmt, ae, ctx);
			} catch(...) {
				m_stack.pop(TASK_SCOPE);
				throw;
			}
			m_stack.pop(TASK_SCOPE);
		}
		//getAudit().setStatus("Finished");
	} catch(ReturnException &/*e*/) {
		// Normal return via return statement
		//getAudit().setStatus("Finished");
		ret = 0;
	} catch(DMException &e) {
		writeToLogFile("Caught exception");
		e.print(*this);
		if(e.getStacktrace()) {
			writeToStdErr(e.getStacktrace());
		}
		//getAudit().setStatus(e.getMessage());
		ret = 2;
	} catch(...) {
		writeToLogFile("Unhandled exception");
		writeToStdErr("FATAL ERROR: Unhandled exception");
		//getAudit().setStatus("Unhandled exception");
		ret = 2;
	}

	try {
		if(appPushed) {
			m_stack.pop(APPLICATION_SCOPE);
		}
		if(parentPushed) {
			m_stack.pop(APPLICATION_SCOPE);
		}
		if (compPushed) {
			m_stack.pop(COMPONENT_SCOPE);
		}

		if(envPushed) {
			m_stack.pop(ENVIRONMENT_SCOPE);
		}
	} catch(DMException &e) {
		writeToLogFile("Caught exception (2)");
		e.print(*this);
		ret = 3;
	} catch(...) {
		writeToStdErr("FATAL ERROR: Unhandled exception");
		writeToLogFile("Unhandled exception (2)");
		ret = 3;
	}

	return ret;
}


int DM::internalRunAction(Action &action,Context *ctx /* = NULL */)
{
	// Use ExtendedStmt implementation mechanism to perform the action call.
	// This will handle parsing scripts in files or the db, mapping to external
	// scripts and even calling plugin actions directly.

	ScopeStack &stack = ctx?ctx->stack():m_stack;
	try {
		ExtendedStmt stmt(&action);
		if(getTargetEnvironment()) {
			Context ctx(*this, *(getTargetEnvironment()->getServers()), stack);
			stmt.execute(ctx);
		} else {
			List<Server> emptyList;
			Context ctx(*this, emptyList, stack);
			stmt.execute(ctx);
		}
		getAudit().setStatus("Finished");
	} catch(SyntaxError &e) {
		e.print(*this);
		getAudit().setStatus(e.getMessage());
		return 1;
	} catch(ReturnException &/*e*/) {
		// Normal return via return statement
		getAudit().setStatus("Finished");
	} catch(DMException &e) {
		getAudit().setStatus(e.getMessage());
		e.print(*this);
		return 1;
	} catch(...) {
		writeToLogFile("Unhandled exception");
		writeToStdErr("FATAL ERROR: Unhandled exception");
		getAudit().setStatus("Unhandled exception");
		return 2;
	}

	return 0;
}




//int DM::internalDeployComponent(class Component &comp)
//{
//	writeToStdOut("Deploying component '%s'", comp.name());
//
//	m_stack.push(comp.getVars());
//
//	int ret = 0;
//
//	//m_stack.dump();
//
//	// Build a pretend deploy statement - the component will come from the stack
//	yylloc_param loc;
//	loc.file_num     = loc.frag_num    = 0;
//	loc.first_column = loc.last_column = 0;
//	loc.first_line   = loc.last_line   = 0;
//
//	StmtList *args = new StmtList();
//	ExtendedStmt deployComp(loc, strdup("deploy"), args, NULL, NULL);
//
//	try {
//		writeToLogFile("Deploying component '%s'", comp.name());
//		if(getTargetEnvironment()) {
//			Context ctx(*this, *(getTargetEnvironment()->getServers()), m_stack);
//			AutoPtr<List<Server> > servers = comp.getServerSubset(ctx);
//			Context newctx(ctx, *servers);
//			deployComp.execute(newctx);
//		} else {
//			// This is probably pointless
//			List<Server> emptyList;
//			Context ctx(*this, emptyList, m_stack);
//			deployComp.execute(ctx);
//		}
//		//getAudit().setStatus("Finished");
//	} catch(ReturnException &/*e*/) {
//		// Normal return via return statement
//		//getAudit().setStatus("Finished");
//	} catch(DMException &e) {
//		writeToLogFile("Caught exception");
//		e.print(*this);
//		if(e.getStacktrace()) {
//			writeToStdErr(e.getStacktrace());
//		}
//		getAudit().setStatus(e.getMessage());
//		ret = 2;
//	} catch(...) {
//		writeToLogFile("Unhandled exception");
//		writeToStdErr("FATAL ERROR: Unhandled exception");
//		getAudit().setStatus("Unhandled exception");
//		ret = 2;
//	}
//
//	try {
//		m_stack.pop(COMPONENT_SCOPE);
//	} catch(DMException &e) {
//		writeToLogFile("Caught exception (2)");
//		e.print(*this);
//		ret = 3;
//	} catch(...) {
//		writeToStdErr("FATAL ERROR: Unhandled exception");
//		writeToLogFile("Unhandled exception (2)");
//		ret = 3;
//	}
//
//	return ret;
//}


void DM::recordDeployedToEnvironment(class Application &app, bool success)
{
	debug1("Updating appsinenv success=%s",success?"true":"false");

	Environment *env = getTargetEnvironment();
	if(env) {
		app.recordDeployedToEnv(*this, *env, success);
	}
}

int DM::internalNotify(Context &ctx,NotifyTemplate *t)
{
	writeToStdOut("INFO: Using template \"%s\" to notify users",t->getName());
	const char **tos = t->getRecipients();
	int nfyid=1;
	try {
		Notify *notify = t->getNotify();

		DMArray *recipients = ctx.stack().newGlobalArray("RECIPIENTS", true);
		// TODO: change to: AutoPtr<DMArray> recipients = new DMArray(true);
		// AutoPtr<DMArray> recipients = new DMArray(true);

		for(const char **sotid = tos; sotid && *sotid; sotid++) {
			ObjectKindAndId otid(*sotid);
			Object *obj = NULL;
			switch(otid.getObjectKind()) {
			case OBJ_KIND_USER:
				obj = getUser(otid.getId());
				break;
			case OBJ_KIND_USERGROUP:
				obj = getUserGroup(otid.getId());
				break;
			default:
				writeToStdOut("ERROR: Invalid recipient type: '%s'", sotid);
				continue;
			}
			if(obj) {
				recipients->add(new Variable(NULL, obj));
			}
		}

		if(recipients->count() == 0) {
			writeToStdOut("WARNING: No recipients specified");
			return 1;
		}

		StmtList *args = new StmtList();
		// TODO: allow multiple recipients - want to use: new Node((DMArray*) recipients)
		args->add(new Stmt(strdup("to"), new Node((DMArray*) recipients)));
		// args->add(new Stmt(strdup("to"), new Node(NODE_ARRAY, new Node(NODE_NLVAR, new Node(NODE_IDOROBJ, strdup("RECIPIENTS"))), new Node(0))));

		char *subject = strdup(t->getSubject());
		Node expandedText1(NODE_STR, subject, true);	// will free subject on delete
		ExprPtr etext1 = expandedText1.evaluate(ctx);
		ConstCharPtr stext1 = etext1->stringify();
		args->add(new Stmt(strdup("subject"), new Node(NODE_STR,strdup(stext1)))); //expsubject is freed when Node is deleted
		
		ExtendedStmt stmt(strdup("notify"), args);
		Audit &audit = getDummyAudit();
		AuditEntry *ae = audit.newAuditEntry("notify");
		OutputStream body;
		char *text = strdup(t->getBody());
		Node expandedText2(NODE_STR, text, true);	// will free text on delete
		ExprPtr etext2 = expandedText2.evaluate(ctx);
		ConstCharPtr stext2 = etext2->stringify();
		body.writeToStdOut("%s", (const char*) stext2);
		ae->start();
		notify->notify(nfyid, stmt, ae, body, NULL, ctx);
		ae->finish();
		return 0;
	} catch(DMException &e) {
		fprintf(stderr, "Exception: %s\n", e.getMessage());
		return 1;
	} catch(...) {
		fprintf(stderr, "Unhandled exception\n");
	}
}

int DM::internalDeployApplication(class Application &app,Context *origctx /* = NULL */)
{
	debug1("%s application '%s'", m_removeApp?"Removing":"Deploying",app.name());

	ScopeStack &stack = origctx?origctx->stack():m_stack;

	Action *action = app.getCustomAction();
	if(action) {
		// Custom action, this takes precedence over components
		List<Server> *serverList;
		if (getTargetEnvironment()) {
			serverList = getTargetEnvironment()->getServers();
		}
		Context ctx(*this, *serverList,stack);
		Audit &audit = ctx.dm().getDummyAudit();
		AuditEntry *ae = audit.newAuditEntry("ApplicationCustom");
		ae->start();
		NotifyTemplate *tmpl = (NotifyTemplate *)0;
		int res = internalRunAction(*action,&ctx);
		ae->finish();
		Action *postAction = app.getPostAction();

		printf("getting template, res=%d\n",res);
		tmpl = (res==0)?app.getSuccessTemplate(ctx):app.getFailureTemplate(ctx);
		if (tmpl) internalNotify(ctx,tmpl);
		/*
		if(getTargetEnvironment()) {
			Context ctx(*this, *(getTargetEnvironment()->getServers()),stack);
			tmpl = (res==0)?app.getSuccessTemplate(ctx):app.getFailureTemplate(ctx);
			if (tmpl) internalNotify(ctx,tmpl);
		} else {
			writeToStdOut("No target environment");
			List<Server> emptyList;
			Context ctx(*this, emptyList, stack);
			tmpl = (res==0)?app.getSuccessTemplate(ctx):app.getFailureTemplate(ctx);
			if (tmpl) internalNotify(ctx,tmpl);
		}
		*/
		recordDeployedToEnvironment(app, (res==0));	// success if res = 0, false otherwise
		// PAG MOD 30/05/2015 - don't cleanup here since this deletes the ExtendedStmtImplRegistry
		// and if we're deploying a release the subsequent version doesn't have access to the built-in
		// commands
		// cleanup();
		if(postAction) {
			Audit &audit = ctx.dm().getDummyAudit();
			AuditEntry *ae = audit.newAuditEntry("ApplicationPost");
			ae->start();
			res = internalRunAction(*postAction);
			ae->finish();
		}
		return res;
	}

	// No custom action, so assume that this is a component based deployment
	bool NeedRollup=false;
	List<Component> *comps = app.getComponents();
	ListIterator<Component> iter(*comps);
	for (Component *comp = iter.first(); comp; comp = iter.next()) {
		if (comp->getRollback() != OFF || comp->getRollup() != OFF) {
			// This component has rollup/rollback options set
			NeedRollup=true;
			break;
		}
		List<ComponentItem> *cis = comp->getItems();
		ListIterator<ComponentItem> iter2(*cis);
		for (ComponentItem *ci = iter2.first(); ci; ci = iter2.next()) {
			if (ci->getRollback() != OFF || ci->getRollup() != OFF) {
				NeedRollup=true;
				break;
			}
		}
	}

	Action *preAction = app.getPreAction();
	Action *postAction = app.getPostAction();
	//List<Component> *components = app.getComponents();
	ApplicationComponentGraph *compGraph = app.getApplicationComponentGraph();
	

	// Moved to here
	Context ctx(*this, *(getTargetEnvironment()->getServers()), stack);
	// compGraph->dump(ctx);

	int res = 0;
	
	if(compGraph) {
		// compGraph->dump();
		if(!getTargetEnvironment()) {
			throw RuntimeError("No environment specified - cannot deploy");
		}
		// Context ctx(*this, *(getTargetEnvironment()->getServers()), stack);
		//
		// Now do an avloop from the version of the app present in the environment
		// to the version being deployed
		//
		Application *parent = app.getParent();
		Application *tmp = getTargetEnvironment()->getDeployedApplication(parent?*parent:app);
		ApplicationVersion *avfrom = tmp?tmp->toApplicationVersion():NULL;
		ApplicationVersion *avto   = app.toApplicationVersion();
		List<ApplicationVersion> path;
		bool found = false;
		bool rollback = false;
		for(ApplicationVersion *av = avto;av;av = av->getPredecessor()) {
			path.prepend(av);
			if(av == avfrom) {
				found = true;
				break;
			}
			if (av->getParent()==0) break;	// end of loop (no more predecessors)
		}
		// At this point path leads from avfrom to avto. If avfrom was not set, path starts at BASE version.
		bool isFirst = true;
		bool isLast = false;
		bool isSame = false;

		// If there is no "from" then we can only be rolling forward from the first
		// version in the path, so skip looking for rollbacks, even though found is false
		if(avfrom) {
			// No path from "to" to "from", so try "from" to "to" for a rollback
			if(!found) {
				path.clear();
				for(ApplicationVersion *av = avfrom; av; av = av->getPredecessor()) {
					path.add(av);
					if(av == avto) {
						found = true;
						rollback = true;
						break;
					}
				}
			}
			if(!found) {
				throw RuntimeError(
					"There is no path between '%s' and '%s'",
					(avfrom ? avfrom->name() : "(null)"),
					(avto ? avto->name() : "(null)"));
			}
			isSame = (avfrom->id() == avto->id());
		} else {
			// This alters the comploop processing to always include the components from the first AV as well
			isFirst = false;
		}

		if (avfrom != avto && NeedRollup /* && !m_removeApp */) {
			ctx.dm().writeToStdOut("INFO: Rolling %s from %s to %s",rollback?"Back":"Forward",avfrom?avfrom->name():"NULL",avto->name());
		}
		/*
		if (m_removeApp) {
			ctx.dm().writeToStdOut("INFO: Rolling back from %s to remove",avfrom?avfrom->name():"NULL");
		}
		*/


		debug3("Create newctx from ctx, rollback = %s",rollback?"true":"false");

		Context newctx(ctx, rollback);

		Scope *avloop = new Scope(AVLOOP_SCOPE);

		newctx.stack().push(avloop);

		bool preActionRun=false;
		ListIterator<ApplicationVersion> iter(path);
		for(ApplicationVersion *av = iter.first(); av; av = iter.next()) {
			if(av == avto) {
				isLast = true;
			}
			ctx.stack().setGlobal("dep.same",isSame);
			ctx.stack().setGlobal("dep.first",isFirst);
			ctx.stack().setGlobal("dep.last",isLast);
			ctx.stack().setGlobal("dep.rollback",rollback);
			ctx.stack().setGlobal("dep.rollforward",!rollback);
			// Also set these values in avloop scope
			avloop->set("dep.same",isSame);
			avloop->set("dep.first",isFirst);
			avloop->set("dep.last",isLast);
			avloop->set("dep.rollback",rollback);
			avloop->set("dep.rollforward",!rollback);
				
			if(preAction && !preActionRun) {
				Audit &audit = ctx.dm().getDummyAudit();
				AuditEntry *ae = audit.newAuditEntry("ApplicationPre");
				ae->start();
				res = internalRunAction(*preAction);
				ae->finish();
				preActionRun=true;
			}
			if (res == 0) {
				// Pre Action ran okay (or there wasn't one)
				// Push the application version onto the Stack
				newctx.stack().push(av->getVars());
				try {
					//
					// Do the deploy
					//
					ApplicationComponentGraph *cg = av->getApplicationComponentGraph();
					debug3("Invoking ApplicationComponentGraph - newctx.isRollback()=%s",newctx.isRollback()?"true":"false");
					res = cg->deploy(newctx);
					// printf("After cg->deploy res=%d",res);
				} catch(...) {
					// If an exception is raised we still need to do the pop
					newctx.stack().pop(APPLICATION_SCOPE);
					newctx.stack().pop(AVLOOP_SCOPE);
					//if(m_appVerScope) {
					//	m_appVerScope->setOpen(true);
					//}
					throw;
				}
				newctx.stack().pop(APPLICATION_SCOPE);
				isFirst = false;
			}
		}

		newctx.stack().pop(AVLOOP_SCOPE);

		// If we got here and didn't throw an exception then the deployment was successful
		recordDeployedToEnvironment(app, res?false:true);
		NotifyTemplate *tmpl = res?app.getFailureTemplate(ctx):app.getSuccessTemplate(ctx);
		if (tmpl) internalNotify(ctx,tmpl);
	}
	if(postAction && res == 0) {
		Audit &audit = ctx.dm().getDummyAudit();
		AuditEntry *ae = audit.newAuditEntry("ApplicationPost");
		ae->start();
		res = internalRunAction(*postAction);
		ae->finish();
	}
	return res;
}


int DM::doDeployment(Application &app,Context *ctx /* = NULL */)
{
	if(!m_initializationDone) {
		writeToStdErr("FATAL ERROR: Initialization not done before running script");
		return 1;
	}

	ScopeStack &stack = ctx?ctx->stack():m_stack;

	// We have to have an application of some sort
	// Application *app = getTargetApplication();

	if (m_removeApp) {
		// Check if the application has a custom action - if it does, we're
		// going to struggle to remove it. Perhaps?
	}

	if (app.isRelease()) {
		// This is a release made up of multiple applications
		// writeToStdOut("INFO: Deploying Release");
		stack.push(app.getVars());
		try
		{
			ReleaseApplicationGraph *rag = app.getReleaseApplicationGraph();
			Context ctx(*this, *(getTargetEnvironment()->getServers()), stack);
			int ret = rag->deploy(ctx);
			stack.pop(APPLICATION_SCOPE);
			return ret;
		}
		catch(...)
		{
			stack.pop(APPLICATION_SCOPE);
			throw;
		}
	}

	AutoPtr<List<Application> > parents = app.getParentList();
	if(!parents) {
		writeToStdErr("ERROR: Unable to get application parents");
		return 1;
	}
	// Check that we have something to run or deploy
	bool somethingToDo = false;
	ListIterator<Application> ait(*parents);
	for(Application *a = ait.first(); a; a = ait.next()) {
		if(a->getCustomAction()
			|| (a->getComponents() && (a->getComponents()->size() > 0))) {
			somethingToDo = true;
			break;
		}
	}
	if(!somethingToDo) {
		writeToStdOut("WARNING: Application has no action or components to deploy (Nothing to do)");
		//writeToStdErr("ERROR: Application has no action or components to deploy");
		// return 1;
	}

	// Setup the stack:

	// Environment goes on first
	bool envPushed = false;
	if(getTargetEnvironment()) {
		stack.push(getTargetEnvironment()->getVars());
		envPushed = true;
	}

	// Then the application OR the parent application and its application version
	bool parentPushed = false;
	// If the app is an app ver, then push the parent application first
	if(app.kind() == OBJ_KIND_APPVERSION) {
		Application *parent = app.getParent();
		if(!parent) {
			throw RuntimeError(stack, "ERROR: Application version '%s' has no parent", app.name());
		}
		stack.push(parent->getVars());
		parentPushed = true;
	}
	stack.push(app.getVars());

	char *szTaskID = getenv("tritaskid");
	int deptaskid = 0;	
	Task *task = (Task *)0;
	Action *preAction = (Action *)0;
	Action *postAction = (Action *)0;
	if (szTaskID) {
		// Find the deployment task referenced
		deptaskid = (int)atol(szTaskID);
		task = getTask(deptaskid);
		if (task) {
			preAction = task->getPreAction();
			postAction = task->getPostAction();
		}
	}

	int ret = 0;

	if (preAction) {
		// Run any pre-action, stopping the deployment if it returns non-zero exit code
		ret = runLinkedAction(*preAction);
		writeToStdOut("INFO: Pre-Deploy action \"%s\" exits code %d",preAction->name(),ret);
	}

	if (ret == 0) {
		ret = internalDeployApplication(app,ctx);
		if (postAction) {
		// Run any post-action
			int postret = runLinkedAction(*postAction);
			writeToStdOut("INFO: Post-Deploy action \"%s\" exits code %d",postAction->name(),postret);
			if (ret == 0) ret = postret;
		}
	}

	//
	// Remove any pending request for this deployment
	//
	if (deptaskid > 0) {
		removeRequest(deptaskid);
	}

	try {
		stack.pop(APPLICATION_SCOPE);
		if(parentPushed) {
			stack.pop(APPLICATION_SCOPE);
		}

		if(envPushed) {
			stack.pop(ENVIRONMENT_SCOPE);
		}
	} catch(DMException &e) {
		writeToLogFile("Caught exception (2)");
		e.print(*this);
		ret = 3;
	} catch(...) {
		writeToStdErr("FATAL ERROR: Unhandled exception");
		writeToLogFile("Unhandled exception (2)");
		ret = 3;
	}

	return ret;
}


void DM::cleanup()
{
	ExtendedStmtImplRegistry::cleanup();
	FunctionNodeImplRegistry::cleanup();
	RepositoryImplRegistry::cleanup();
	TransferProviderImplRegistry::cleanup();
	NotifyProviderImplRegistry::cleanup();
	DatasourceProviderImplRegistry::cleanup();
	ModifyProviderImplRegistry::cleanup();
}


void DM::setPluginObject(const char *name, PluginObject *obj)
{
	if(m_model) {
		m_model->setPluginObject(name, obj);
	}
}


PluginObject *DM::getPluginObject(const char *name)
{
	return m_model ? m_model->getPluginObject(name) : NULL;
}
