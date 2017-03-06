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
#ifdef WIN32
#include "windows.h"
#define RESTFUL_EXPORT __declspec(dllexport)
#else
#define RESTFUL_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#ifndef WIN32
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#endif /*!WIN32*/

#include "restful.h"

#include "dm.h"
#include "node.h"
#include "expr.h"
#include "context.h"
#include "exceptions.h"
#include "hashtable.h"
#include "scopestack.h"  // for DMArray
#include "lexer.h"

#include "include/xml2json.hpp"

//
// Same code as in smtp.cpp but replicated here because this is in its own library
//
char *base64encode(unsigned char *data, unsigned long datalen)
{
	static const char *b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int len = datalen/3 * 4;
	if(datalen % 3 > 0) len += 4;
	len += (len/76)*2;
	char *output = (char*) malloc(len+1);
	if (output) {

		// 11111100 = fc / 03; 11110000 = f0 / 0f; 11000000 = c0 / 3f
		int m = 0;
		unsigned m1, m2, m3, m4;
		int maxn = (datalen/3) * 3;
		for(int n = 0; n < maxn; n += 3)
		{
			m1 = (data[n] & 0xfc) >> 2; 
			m2 = ((data[n] & 0x03) << 4) + ((data[n+1] & 0xf0) >> 4);
			m3 = ((data[n+1] & 0x0f) << 2) + ((data[n+2] & 0xc0) >> 6);
			m4 = data[n+2] & 0x3f;
			output[m++] = b64[m1];
			output[m++] = b64[m2];
			output[m++] = b64[m3];
			output[m++] = b64[m4];

			if((n+3) % 57 == 0) {
				output[m++] = 0x0d;
				output[m++] = 0x0a;
			}
		}

		switch(datalen % 3)
		{
		case 0:
			break;
		case 1:
			m1 = (data[maxn] & 0xfc) >> 2;
			m2 = (data[maxn] & 0x03) << 4;
			output[m++] = b64[m1];
			output[m++] = b64[m2];
			output[m++] = '=';
			output[m++] = '=';
			break;
		case 2:
			m1 = (data[maxn] & 0xfc) >> 2;
			m2 = ((data[maxn] & 0x03) << 4) + ((data[maxn+1] & 0xf0) >> 4);
			m3 = ((data[maxn+1] & 0x0f) << 2);
			output[m++] = b64[m1];
			output[m++] = b64[m2];
			output[m++] = b64[m3];
			output[m++] = '=';
			break;
		}

		output[m] = '\0';
	}
	return output;
}

// For secure connections
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"

#define RESTFUL_PLUGIN_NAME    "restful"

#define RESTFUL_PLUGIN_VERSION "1.00"

#define RESTFUL_POST_NAME "restful_post"
#define RESTFUL_GET_NAME  "restful_get"
#define SOAP_NAME         "soap"

typedef enum message_type_t {
MESSAGE_TYPE_SOAP	= 1,
MESSAGE_TYPE_POST   = 2,
MESSAGE_TYPE_GET    = 3
} MESSAGE_TYPE;


#define CONTENT_TYPE_JSON     "application/json"
#define CONTENT_TYPE_TEXTXML  "text/xml"
#define CONTENT_TYPE_APPXML   "application/xml"


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
			  int *status, char **contentType, char **content)	// return
{
	int	sock;
	int	res;
	char *TransferEncoding = (char *)0;
	bool chunked=false;

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

/*
printf("Connecting to %d.%d.%d.%d:%d...\n",
	   name.sin_addr.S_un.S_un_b.s_b1, name.sin_addr.S_un.S_un_b.s_b2,
	   name.sin_addr.S_un.S_un_b.s_b3, name.sin_addr.S_un.S_un_b.s_b4, port);
*/
		res = connect(sock,(struct sockaddr*) &name, sizeof(name));
		if(res)
		{
			// Connection failure
#ifdef WIN32
			errno = WSAGetLastError();
#endif /*WIN32*/
			return -2;
		}
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
		request.writeToStdOut("User-Agent: ReleaseEngineer\r\n");
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

	// printf("Request: %s\n", request.buffer());

	char *line;
	if (isSecure) {
		SendDataToSocket(ssl, request.buffer(), request.size());
		line = ReadLineFromSocket(ssl);
	} else {
		SendDataToSocket(sock, request.buffer(), request.size());
		line = ReadLineFromSocket(sock);
	}

	// printf("line: [%s]\n",line);

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

	long length = -1;

	while(true) {
		if (isSecure) {
			line = ReadLineFromSocket(ssl);
		} else {
			line = ReadLineFromSocket(sock);
		}
		
		// printf("line=%s\n",line);
		//debug1("line = %s", line);

		if(!*line) break;

		if(strncmp(line, "Content-Length: ", 16) == 0) {
			length = atol(&line[16]);
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
		}
	}
	// printf("Length: %d\n", length);
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
			
		} else if(length == -1) {
			if (isSecure) {
				*content = ReadToEndOfStream(ssl);
			} else {
				*content = ReadToEndOfStream(sock);
			}
			
		}
	}
	//printf("Content is:\n");
	//printf(*content);
	//printf("end of content\n");

	if (isSecure) {
		SSL_free(ssl);
	} else {
		CLOSESOCKET(sock);
	}

	return 0;
}


char *urlEncode(const char *str)
{
	int len = strlen(str) + 1;
	char *ret = (char*) malloc(len);
	*ret  = '\0';
	
	char *y = ret;
	for(const char *x = str; x && *x; x++) {
		switch(*x) {
		case ' ': case '%': {
			int offset = y - ret;
			len += 2;
			ret = (char*) realloc(ret, len);
			y = ret + offset;
			y += sprintf(y, "%%%02X", *x);
			}
			break;
		default:
			*y = *x;
			y++;
			break;
		}
	}
	*y = '\0';

	return ret;
}


void DoRestfulPost(
	const char *server, int port,
	const char *uri, Expr *eparams, DMArray *header,
	const char *cookiesto, Expr *ecookiesfrom,
	char **contentType, char **content,MESSAGE_TYPE mt,
	bool isSecure,const char *soapaction,Context &ctx)
{
	char* encparams = (char *)0;
	DMArray *params = (DMArray *)0;
	if(eparams) {
		if (eparams->kind() == KIND_ARRAY) {
			//
			// Let's convert to JSON and see what happens
			//
			params = eparams->toArray();
			//printf("JSON is %s\n",params->toJSON());

			encparams = (char*) malloc(1);
			*encparams = '\0';
			AutoPtr<StringList> keys = params->keys();
			StringListIterator iter(*keys);
			for(const char *key = iter.first(); key; key = iter.next()) {
				Variable *var = params->get(key);
				if (var->type() == vartype_array) {
					// Looks like an JSON structure. Abandon this and send verbatim.
					char *json = params->toJSON();
					encparams = strdup(json);
					break;
				}
				ConstCharPtr val = (var ? var->toString() : NULL);
				ConstCharPtr encval = (val ? urlEncode(val) : strdup(""));
				//printf("allocating %d bytes for encparams\n",strlen(encparams)+strlen(key)+strlen(encval)+3);
				encparams = (char*) realloc(encparams, strlen(encparams) + strlen(key) + strlen(encval) + 3);
				if(*encparams) {
					sprintf(encparams, "%s&%s=%s", encparams, key, (const char*) encval);
				} else {
					sprintf(encparams, "%s=%s", key, (const char*) encval);
				}
				//printf("encparams is [%s] length is %d\n",encparams,strlen(encparams));
			}
		}
		else if (eparams->kind() == KIND_STR) {
			// Parameter is of kind string.
			encparams = eparams->toString();
		}
	}

	int httpStatus = 0;

	DMArray *cookieJar = NULL;
	if(cookiesto) {
		//debug1("Creating new cookie jar in $%s", cookiesto);
		cookieJar = ctx.stack().newGlobalArray(cookiesto);
	}
	if(ecookiesfrom) {
		//debug1("Using existing cookie jar");
		cookieJar = ecookiesfrom->toArray();
	}

	if(DoHttpRequest(server, port, uri, encparams, mt, isSecure, server, soapaction, cookieJar, header, &httpStatus, contentType, content) != 0) {
		throw RuntimeError(/*m_parent,*/ ctx.stack(), "Failed to send request to '%s'", (const char*) server);
	}

	SAFE_FREE(encparams);

	if((httpStatus < 200) || (httpStatus > 299)) {
		throw RuntimeError(/*m_parent, ctx.stack(),*/ "Server '%s' returned status %d", (const char*) server, httpStatus);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Restful_PostStmtImpl
///////////////////////////////////////////////////////////////////////////////

Restful_PostStmtImpl::Restful_PostStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void Restful_PostStmtImpl::execute(class Context &ctx)
{
	// This is the "procedure" version.
	ConstCharPtr server = m_parent.getArgAsString("server", ctx);
	ExprPtr eport       = m_parent.getArg("port", ctx);
	ConstCharPtr uri    = m_parent.getArgAsString("uri", ctx);
	ExprPtr eparams     = m_parent.getArg("params", ctx);
	ConstCharPtr cookiesto = m_parent.getArgAsString("cookiesto", ctx);
	ExprPtr ecookiesfrom   = m_parent.getArg("cookiesfrom", ctx);

	if(!server) {
		throw RuntimeError(m_parent, ctx.stack(), "server must be specified");
	}
	if(!uri) {
		throw RuntimeError(m_parent, ctx.stack(), "uri must be specified");
	}
	if(cookiesto && ecookiesfrom) {
		throw RuntimeError(m_parent, ctx.stack(), "only one of cookiesfrom and cookiesto must be specified");
	}

	int port = eport ? eport->toInt() : 80;
	// DMArray *params = eparams ? eparams->toArray() : NULL;

	char *contentType = NULL;
	char *content = NULL;
	DMArray *header = NULL;

	DoRestfulPost(server, port, uri, eparams, header, cookiesto, ecookiesfrom, &contentType, &content, MESSAGE_TYPE_POST, true, (const char *)0, ctx);

	SAFE_FREE(contentType);
	SAFE_FREE(content);
}


///////////////////////////////////////////////////////////////////////////////
// Restful_PostStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

Restful_PostStmtImplFactory::Restful_PostStmtImplFactory()
{}


ExtendedStmtImpl *Restful_PostStmtImplFactory::create(class DM &dm, ExtendedStmt &parent)
{
	return new Restful_PostStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// Restful_PostFunctionImpl
///////////////////////////////////////////////////////////////////////////////

Restful_PostFunctionImpl::Restful_PostFunctionImpl(FunctionNode &parent)
	: m_parent(parent)
{}


class Expr *Restful_PostFunctionImpl::evaluate(class ExprList *args, class Context &ctx, bool stringify)
{
	// restful_post(url, parameters, cookiejar, credentials)
	//  restful_get(url, parameters, cookiejar, credentials)
	//         soap(url, parameters, soapaction, cookiejar, credentials)
	// ----------------------------------------------------
	// url should be in form:
	// http[s]://server[:port]/url
	// soapaction (optional) is a String containing the SoapAction to call on the specified interface
	// parameters (optional) is a DMArray or an XML string (for SOAP)
	// cookiejar (optional) is a DMArray or the name of an array to store cookies in
	// credential (optional) is a DMCredential and is used for basic authentication if required
	//
	char *FunctionName = (char *)0;
	char *ParamErrorMsg = (char *)0;
	char *MalformedURLErrorMsg = (char *)0;
	int maxparams=4;
	int minparams=1;
	MESSAGE_TYPE mt;
	
	if (strcmp(m_parent.name(),RESTFUL_POST_NAME)==0) {
		FunctionName = RESTFUL_POST_NAME;
		ParamErrorMsg = RESTFUL_POST_NAME " takes 1-4 arguments";
		MalformedURLErrorMsg = RESTFUL_POST_NAME " - malformed URL";
		mt = MESSAGE_TYPE_POST;
	} else if (strcmp(m_parent.name(),RESTFUL_GET_NAME)==0) {
		FunctionName = RESTFUL_GET_NAME;
		ParamErrorMsg = RESTFUL_GET_NAME " takes 1-4 arguments";
		MalformedURLErrorMsg = RESTFUL_GET_NAME " - malformed URL";
		mt = MESSAGE_TYPE_GET;
	} else {
		FunctionName = SOAP_NAME;
		ParamErrorMsg = SOAP_NAME " takes 2-5 arguments";
		MalformedURLErrorMsg = SOAP_NAME " - malformed URL";
		minparams = 2;
		maxparams = 5;
		mt = MESSAGE_TYPE_SOAP;
	}

	if(!args || (args->size() < minparams) || (args->size() > maxparams)) {
		throw RuntimeError(m_parent, ctx.stack(), ParamErrorMsg);
	}

	Expr** argv = args->toArgv();	// TODO: Free this

	Expr *params = (Expr *)0;
	ConstCharPtr cookiesto;
	Expr* ecookiesfrom = NULL;
	ConstCharPtr credname;
	bool secure=false;
	char *url = (char *)0;
	int port=0;
	char *server=(char *)0;
	int sp;
	char *soapaction = (char *)0;
	DMArray *header = NULL;
	Expr *headerOrCredential = NULL;

	ConstCharPtr fullurl = argv[0]->toString();
	if (args->size()>1) params = argv[1];
	if (mt == MESSAGE_TYPE_SOAP) {
		if (args->size()>2) {
			if (argv[2]->kind() == KIND_STREAM) {
				// SOAP BODY is a stream
				OutputStream *stream = argv[2]->toStream();
				soapaction = (char *)malloc(stream->size()+1);
				memcpy(soapaction,stream->buffer(),stream->size());
				soapaction[stream->size()]='\0';
			} else if (argv[2]->kind() == KIND_STR) {
				soapaction = argv[2]->toString();
			} else {	
				// SOAP BODY can only be STREAM or STRING
				throw RuntimeError(m_parent, ctx.stack(), "SOAP Body must be Stream or String");
			}
		}
				
		if (args->size()>3) {
			if(argv[3]->kind() == KIND_ARRAY) {
				ecookiesfrom = argv[3];
			} else {
				cookiesto = argv[3]->toString();
			}
		}
		if (args->size()>4) {
			headerOrCredential = argv[4];
		}
	} else {
		if (args->size()>2) {
			if(argv[2]->kind() == KIND_ARRAY) {
				ecookiesfrom = argv[2];
			} else {
				cookiesto = argv[2]->toString();
			}
		}
		if (args->size()>3) {
			headerOrCredential = argv[3];
		}
	}
	if (headerOrCredential) {
		if (headerOrCredential->kind() == KIND_ARRAY) {
			// Array of header directives has been passed
			header = headerOrCredential->toArray();
		} else {
			// Not an Array, could be either the name of a credential or a credential object
			Credentials *c = NULL;
			if (headerOrCredential->kind() == KIND_STR) {
				// This should be the name of a credential
				c = ctx.dm().getModel()->getCredential(argv[3]->toString(),true);	// will throw runtime exception if not found
			} else if (headerOrCredential->kind() == KIND_OBJECT) {
				ObjectReference *obj = headerOrCredential->toObjectReference();
				if (obj->kind() == OBJ_KIND_CREDENTIALS) {
					c = (Credentials *)obj->toObject();
				} else {
					throw RuntimeError(m_parent, ctx.stack(), "Object passed is not a Credential Object");
				}
			} else throw RuntimeError(m_parent, ctx.stack(), "Invalid Parameter");
			if (c) {
				char *username = c->getDecryptedUsername(ctx);
				char *password = c->getDecryptedPassword(ctx);
				char *precode = (char *)malloc(strlen(username)+strlen(password)+5);
				sprintf(precode,"%s:%s",username,password);
				char *b64code = base64encode((unsigned char*) precode, strlen(precode));
				char *AuthenticationString = (char *)malloc(strlen(b64code)+32);
				sprintf(AuthenticationString,"Basic %s",b64code);
				header = new DMArray(false);
				header->put("Authorization",new Variable(NULL,AuthenticationString));
				free(AuthenticationString);
				free(precode);
			} else throw RuntimeError(m_parent, ctx.stack(), "Failed to case credential internally"); // should never happen
		}
	}
	//
	// Calculate SSL requirement, port and server name from URL.
	//
	// printf("fullurl=[%s]\n",fullurl);
	if (	tolower(fullurl[0])=='h' &&
			tolower(fullurl[1])=='t' &&
			tolower(fullurl[2])=='t' &&
			tolower(fullurl[3])=='p') {
		if (tolower(fullurl[4])=='s') {
			secure=true;
			sp=5;
		} else {
			secure=false;
			sp=4;
		}
		//printf("sp=%d\n",sp);
		//printf("fullurl[%d]='%c'\n",sp,fullurl[sp]);
		if (fullurl[sp]!=':') throw RuntimeError(m_parent, ctx.stack(), MalformedURLErrorMsg);
		if (fullurl[sp+1]!='/') throw RuntimeError(m_parent, ctx.stack(), MalformedURLErrorMsg);
		if (fullurl[sp+2]!='/') throw RuntimeError(m_parent, ctx.stack(), MalformedURLErrorMsg);
		// http://server[:port]/ or
		// http://server/
		char *serverport = (char *)strdup((const char *)&(fullurl[sp+3]));	// Point to server[:port]
		char *ep = (char *)strchr(serverport,'/');
		if (ep) {
			// Found a /
			url=strdup(ep);
			*ep='\0';
		} else {
			// No / - presumably just server[:port]
			url=strdup("");
		}
		// Is there a port ID?
		char *ps = (char *)strchr(serverport,':');
		if (ps) {
			// There's a port specification
			*ps='\0';
			port = atoi(ps+1);
		} else {
			// No port specified, use default
			port = secure?443:80;
		}
		server=strdup(serverport);
		free((void *)serverport);
	} else {
		// No http:// start string.
		char *ep = (char *)strchr(fullurl,'/');
		if (ep) {
			url=strdup(ep);
			*ep='\0';
		} else {
			url=strdup("");
		}
		char *serverport=strdup(fullurl);
		char *ps = strchr(serverport,':');
		if (ps) {
			// There's a port specification
			*ps='\0';
			port = atoi(ps+1);
		} else {
			// No port specified, use default
			port = 80;
		}
		server=strdup(serverport);
		free(serverport);
	}
	//printf("server=[%s]\n",server);
	//printf("port=[%d]\n",port);
	//printf("url=[%s]\n",url);

	char *contentType = NULL;
	char *content = NULL;
	

	
	DoRestfulPost(server, port, url, params, header, cookiesto, ecookiesfrom, &contentType, &content, mt, secure, soapaction, ctx);
	

	Expr *ret = NULL;

	if(contentType && (strncmp(contentType, CONTENT_TYPE_JSON, strlen(CONTENT_TYPE_JSON)) == 0)) {
		// Result is JSON - parse it
		debug1("Result is JSON");
		if(content) {
			LexerBuffer lb(expr_lexer, content, NULL);
			ret = lb.parseExpression(ctx); // May raise an exception
		} else {
			debug1("No content to parse");
		}
	}
	else
	if(contentType &&
		(
		(strncmp(contentType, CONTENT_TYPE_APPXML, strlen(CONTENT_TYPE_APPXML)) == 0) ||
		(strncmp(contentType, CONTENT_TYPE_TEXTXML, strlen(CONTENT_TYPE_TEXTXML)) == 0)
		)
	) {
		// printf("Result is XML\n");
		if(content) {
			// Convert XML to JSON format
			std::string json_str = xml2json(content);
			const char *c = json_str.c_str();
			// printf("JSON String is: %s\n",c);
			LexerBuffer lb(expr_lexer, (char *)c, NULL);
			ret = lb.parseExpression(ctx); // May raise an exception
		} else {
			debug1("No content to parse");
		}
	}
	else
	{
		// result is neither JSON or XML
		printf("Result (%s) is neither JSON nor XML\n",contentType?contentType:"NULL");
		ret = new Expr(content);
	}

	SAFE_FREE(soapaction);
	SAFE_FREE(contentType);
	SAFE_FREE(content);

	return ret ? ret : new Expr();
}


///////////////////////////////////////////////////////////////////////////////
// Restful_PostFunctionImplFactory
///////////////////////////////////////////////////////////////////////////////

Restful_PostFunctionImplFactory::Restful_PostFunctionImplFactory()
{}

FunctionNodeImpl *Restful_PostFunctionImplFactory::create(class DM &dm, FunctionNode &parent)
{
	return new Restful_PostFunctionImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////

extern "C" RESTFUL_EXPORT int PluginStart(DM &dm)
{
	dm.writeToLogFile("Restful plugin V" RESTFUL_PLUGIN_VERSION);

	ExtendedStmtImplRegistry::instance()
		.registerFactory(RESTFUL_POST_NAME, new Restful_PostStmtImplFactory());
	FunctionNodeImplRegistry::instance()
		.registerFactory(RESTFUL_POST_NAME, new Restful_PostFunctionImplFactory());
	ExtendedStmtImplRegistry::instance()
		.registerFactory(RESTFUL_GET_NAME, new Restful_PostStmtImplFactory());
	FunctionNodeImplRegistry::instance()
		.registerFactory(RESTFUL_GET_NAME, new Restful_PostFunctionImplFactory());
	ExtendedStmtImplRegistry::instance()
		.registerFactory(SOAP_NAME, new Restful_PostStmtImplFactory());
	FunctionNodeImplRegistry::instance()
		.registerFactory(SOAP_NAME, new Restful_PostFunctionImplFactory());
	return 0;
}


extern "C" RESTFUL_EXPORT int PluginInstall(DM &dm)
{
	return 0;
}
