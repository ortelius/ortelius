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
#ifndef WIN32
#include <unistd.h>
#endif /*!WIN32*/

#include "https.h"

#include "../../dmapi/platform.h"

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"


// in smtp.cpp
int EstablishOutgoingConnection(int destination_port, const char *HostName);

#ifdef WIN32
void InitialiseWinsock();
void CloseWinsock();
#else
#define InitialiseWinsock()
#define CloseWinsock()
#endif /*WIN32*/


static int sslInitialised = 0;


static char* get_ssl_error()
{
  SSL_load_error_strings();
  return ERR_error_string(ERR_get_error(), NULL);
}


HTTPS::HTTPS(
		const char *servername, int port, const char *uri,
		const char *content, const char *logfile
	)
	: m_servername(DUP_NULL(servername)), m_port(port),
	  m_uri(DUP_NULL(uri)), m_content(content),
	  m_sock(0), m_ssl_ctx(NULL), m_pSsl(NULL)
{}


HTTPS::~HTTPS()
{
	SAFE_FREE(m_servername);
	m_content = NULL; // Don't own
}


bool HTTPS::isOkay()
{
	return true;
}


bool HTTPS::post()
{
	if(!isOkay()) {
		return false;
	}

	InitialiseWinsock();

	m_sock = EstablishOutgoingConnection(m_port, m_servername);

	if(!ssl_connect()) {
		printf("failed to establish ssl connection\n");
		return false;
	}

	char buf[1024];
	sprintf(buf,
			"POST %s HTTP/1.0\r\n"
			"Host: %s\r\n"
			"User-Agent: Mozilla/4.0\r\n"
			"Content-Length: %d\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"\r\n",
			m_uri, m_servername, strlen(m_content));

	sendData(buf, strlen(buf));
	sendData(m_content, strlen(m_content));

	char *line = readLine();
//printf("line = %s\n", line);

	if(strncmp(line, "HTTP/1.", 7) != 0) {
		closeConnection();
		return false; // Not HTTP
	}

	int status = atoi(&line[9]);
/**/printf("Status: %d\n", status);

	long length = -1;

	while(true) {
		line = readLine();
/**/printf("line = %s\n", line);

		if(strncmp(line, "Content-Length: ", 15) == 0) {
			length = atol(&line[15]);
		}

		if(!*line) break;
	}
/**/printf("Length: %d\n", length);

	if(length > 0) {
		char *data = (char*) malloc(length + 1);
		if(readData(data, length)) {
			data[length] = '\0';
/**/printf("Data: %s\n", data);
		}
	}

	closeConnection();

	CloseWinsock();

	return true;
}


void HTTPS::dump()
{
	// URL
	printf("URL: http://%s:%d\n", m_servername, m_port);

	// Content
	printf("\n%s\n", m_content);
}


bool HTTPS::ssl_connect()
{
	//LogFTPTraffic("FTPS: FTPS_Connect on fd %d\n", c->trisocket->sock);

	if(!sslInitialised) {
		sslInitialised = 1;
		SSL_library_init();
	}

	if(!m_ssl_ctx) {
		SSL_CTX* p_ctx = SSL_CTX_new(SSLv23_method());
		if(!p_ctx) {
			/*LogFTPTraffic*/ printf("SSL_CTX_new failed\n");
			return false;
		}

		// SSL_OP_NO_SSLv2, SSL_OP_NO_SSLv3, SSL_OP_NO_TLSv1
		SSL_CTX_set_options(p_ctx, SSL_OP_ALL);

		if(SSL_CTX_set_cipher_list(p_ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH") != 1) {
			/*LogFTPTraffic*/ printf("SSL_CTX_set_cipher_list failed\n");
			return false;
		}

		if(RAND_status() != 1) {
			/*LogFTPTraffic*/ printf("RAND_status is not 1\n");
			return false;
		}

		m_ssl_ctx = p_ctx;
	}

	SSL_CTX* p_ctx = (SSL_CTX*) m_ssl_ctx;

	SSL* p_ssl = SSL_new(p_ctx);
	if(!p_ssl) {
		/*LogFTPTraffic*/ printf("HTTPS: SSL_new failed\n");
		return false;
	}

#if 0
	// FTPS Handshake debug
	SSL_set_info_callback(p_ssl, FTPS_InfoCallback);
	SSL_set_msg_callback(p_ssl, FTPS_MsgCallback);
#endif

	if(!SSL_set_fd(p_ssl, m_sock)) {
		/*LogFTPTraffic*/ printf("HTTPS: SSL_set_fd failed\n");
		SSL_free(p_ssl);
		return false;
	}

	if(m_pSsl) {
		SSL_copy_session_id(p_ssl, (const SSL*) m_pSsl);
	}

	if(SSL_connect(p_ssl) <= 0) {
		char *lastMessage = strdup(get_ssl_error());
		/*LogFTPTraffic*/ printf("HTTPS: SSL_connect failed: %s\n", lastMessage);
		SSL_free(p_ssl);
		return false;
	}

	m_pSsl = p_ssl;

	/*LogFTPTraffic*/ printf("HTTPS: encrypted connection establised\n");
	return true;
}


int HTTPS::sendData(const char *data, long len)
{
	if(!m_sock) { return -1; }

    int res = SSL_write((SSL*)m_pSsl, data, len);
    if(res == -1)
    {
        perror("Failed to send");
    }
	return res;
}


char *HTTPS::readLine()
{
	if(!m_sock) { return NULL; }

	static char reply[1024];
	int n;

	// Read up to CR-LF
	char ch;
	int t=0;
	do {
		n = SSL_read((SSL*)m_pSsl, &ch, 1);

		if(n == 0) {
			// Connection has been closed
			/*LogSMTPTraffic*/ printf("Connection Closed -setting reply to 421 TIMED OUT (CONNECTION CLOSED)\n");
			ch='\0';
			//
			// Return a "TIMED OUT" indication for those servers that
			// don't handle it properly....
			//
			strcpy(reply,"421 TIMED OUT (CONNECTION CLOSED)");
		} else if(n == 1) {
			if((ch == '\n') || (t >= (sizeof(reply)-1))) ch='\0';
			if(ch != '\r') reply[t++]=ch;
		} else {
#ifdef WIN32
			// RHT 15/07/2008 - try and figure out why we landed here!
			// Have to do this first as LogFTPTraffic corrupts the error number.
			if (n == SOCKET_ERROR) {
				LPVOID lpMsgBuf;
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf, 0, NULL  );
				/*LogSMTPTraffic*/ printf("WSAGetLastError reports %s", (LPCTSTR)lpMsgBuf);
				LocalFree( lpMsgBuf );
			} else {
				/*LogSMTPTraffic*/ printf("n = %d\n", n);
			}
#endif /*WIN32*/

			/*LogSMTPTraffic*/ printf("recv failed - setting Reply to 421 TIMED OUT (RECV failed)\n");
			strcpy(reply,"421 TIMED OUT (RECV failed)");
			break;	// recv failed
		}
	} while((int) ch);

	return reply;
}


bool HTTPS::readData(char *dataPtr, long length)
{
	// Reads "length" bytes of data from the socket.
	// If less than "Length" bytes are read then loop.
	//
	long		BytesRead=0;
	bool		res=true;

	char *xPtr=(char *)dataPtr;
	long BytesToRead=length;
	bool BytesRemaining=true;
	while (BytesRemaining) {
		BytesRead = SSL_read((SSL*)m_pSsl, xPtr, BytesToRead);
		if (BytesRead == -1) {
			// ShowErrorMessage("Failed to read from socket");
			BytesRemaining=false;
			res=false;
			break;
		}
		if (BytesRead == 0) {
			// Server has closed connection.
			// ShowErrorMessage("Server has closed connection");
			BytesRemaining=false;
			res=false;
			break;
		}
		if (BytesRead<BytesToRead) {
			BytesToRead=BytesToRead-BytesRead;
			xPtr=xPtr+BytesRead;
		} else {
			BytesRemaining=false;
		}
	}
	return res;
}


void HTTPS::closeConnection()
{
	if(m_sock) {
#ifdef WIN32
		closesocket(m_sock);
#else
		close(m_sock);
#endif
		m_sock = 0;
	}
}
