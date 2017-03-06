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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif /*WIN32*/
#include <time.h>


#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#include "smtp.h"

#include "../../dmapi/platform.h"
#include "../../dmapi/crypto.h"
#include "../../dmapi/outputstream.h"



void SMTP::LogSMTPTraffic(const char* fmt, ...) 
{
	if(m_logfile)
	{
		va_list args;

		va_start(args,fmt);
		FILE *out=fopen(m_logfile, "a");
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
			fprintf(out,"\n");
			fclose(out);
		}
		va_end(args);
	}

	if(m_logstream)
	{
		char *temp = NULL;
		va_list args;

		va_start(args,fmt);
		vasprintf(&temp,fmt,args);
		va_end(args);
		
		m_logstream->writevToStdOut(0, temp);
		free(temp);
	}
}


#ifdef WIN32
static bool winsockInitialised = false;

void InitialiseWinsock()
{
	if(!winsockInitialised) {
		WORD		wVersionRequested=MAKEWORD(2,2);	// version 2.2
		WSADATA		wsadata;

		int res=WSAStartup(wVersionRequested,&wsadata);

		winsockInitialised = true;
	}
}

void CloseWinsock()
{
	WSACleanup();
	winsockInitialised=false;
}
#else
#define InitialiseWinsock()
#define CloseWinsock()
#endif /*WIN32*/


int EstablishOutgoingConnection(int destination_port, const char *HostName)
{
	int					sock;
	int					res;
	struct sockaddr_in	name;
	struct hostent		*hp;

	hp = gethostbyname(HostName);
	if (hp == (struct hostent *)0)
	{
		// gethostbyname fails	
		errno=h_errno;
		sock = -1;
	}
	else
	{
		// Create a socket on which to send.
		sock = socket(AF_INET, SOCK_STREAM, 0);
		name.sin_family = AF_INET;
		name.sin_addr.s_addr = *((int *)hp->h_addr);
		name.sin_port = htons(destination_port);
		//printf("Resolved to %d.%d.%d.%d\n", hp->h_addr[0], hp->h_addr[1], hp->h_addr[2], hp->h_addr[3]);
		res = connect(sock,(struct sockaddr *)&name,sizeof(name));
		if (res != 0)
		{
			// Connection failure
#ifdef WIN32
			errno=WSAGetLastError();
#endif
			sock = 0;
		}
	}
	return sock;
}


char *quotedPrintableEncode(const char *data, int datalen)
{
	int n, m = 0, quotables = 0;

	for(n = 0; n < datalen; n++) {
		if((data[n] == '=') || (data[n] < 0) || (data[n] > 126)) {
			quotables++;
		}
	}

	char *output = (char*) malloc(datalen + quotables*2 + 1);
	if (output) {
		char buf[10];
		for(n = 0; n < datalen; n++) {
			if((data[n] == '=') || (data[n] < 0) || (data[n] > 126)) {
				output[m++] = '=';
				unsigned int qp = data[n] & 0xff;
				sprintf(buf, "%02X", qp);
				output[m++] = buf[0];
				output[m++] = buf[1];
			} else {
				output[m++] = data[n];
			}
		}
		output[m] = '\0';
	}
	return output;
}


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


class MailAddress
{
	friend class SMTP;

private:
	char *m_email;
	char *m_realname;
	MailAddress *m_next;

public:
	MailAddress(const char *email, const char *realname = NULL);
	~MailAddress();
};


class MailAttachment
{
	friend class SMTP;

private:
	char *m_name;
	unsigned long m_datalen;
	unsigned char *m_data;
	MailAttachment *m_next;

public:
	MailAttachment(const char *name, unsigned char *data, unsigned long datalen);
	~MailAttachment();
};

MailAddress::MailAddress(const char *email, const char *realname /*= NULL*/)
	: m_email(DUP_NULL(email)), m_realname(DUP_NULL(realname)), m_next(NULL)
{}

MailAddress::~MailAddress()
{
	if(m_email) free(m_email);
	if(m_realname) free(m_realname);
	m_email = NULL;
	m_realname = NULL;
}

MailAttachment::MailAttachment(const char *name, unsigned char *data, unsigned long datalen)
	: m_name(DUP_NULL(name)), m_data(data), m_datalen(datalen), m_next(NULL)
{}

MailAttachment::~MailAttachment()
{
	if(m_name) free(m_name);
	m_name = NULL;
}


SMTP::SMTP(const char *mailhost, int port, const char *username,
		   const char *password, const char *from, const char *subject,
		   const char *body, const char *logfile, bool useSSL, bool useHTML, IOutputStream *logstream)
  : m_mailhost(mailhost), m_port(port), m_username(username),
    m_password(password), m_from(from), m_subject(subject),
    m_body(body), m_logfile(logfile), m_useSSL(useSSL), m_useHTML(useHTML), m_logstream(logstream),
	m_rcpts(NULL), m_ccs(NULL), m_attachs(NULL), m_sslInitialised(false),
	m_sock(0), m_emails(false), m_capabilites(0), m_challenge(NULL),
	m_starttls(false), m_pSsl(NULL), m_ssl_ctx(NULL), m_LastMessage(NULL)
{}


SMTP::~SMTP()
{
	m_mailhost = NULL;	// Don't own
	m_username = NULL;	// Don't own
	m_password = NULL;	// Don't own
	m_from = NULL;		// Don't own
	m_subject = NULL;	// Don't own
	m_body = NULL;		// Don't own
	m_logfile = NULL;	// Don't own
	m_logstream = NULL;	// Don't own
	if(m_rcpts) {
		for(MailAddress *ma = m_rcpts, *next; ma; ma = next) {
			next = ma->m_next;
			delete ma;
		}
	}
	if(m_ccs) {
		for(MailAddress *ma = m_rcpts, *next; ma; ma = next) {
			next = ma->m_next;
			delete ma;
		}
	}
	if(m_attachs) {
		for(MailAttachment *ma = m_attachs, *next; ma; ma = next) {
			next = ma->m_next;
			delete ma;
		}
	}
	if(m_challenge) {
		free(m_challenge);
	}
}


int SMTP::sendDataToServer(const char *data, long len)
{
	if(!m_sock) { return -1; }
	int res = 0;
	if(m_pSsl) {
		res = SSL_write((SSL*)m_pSsl,data,len);
	} else {
		res = send(m_sock, data, len, 0);
	}
    if(res == -1)
    {
        perror("Failed to send");
    }
	return res;
}


char *SMTP::readLineFromServer()
{
	if(!m_sock) { return NULL; }

	static char Reply[1024];
	int n;

	// Read up to CR-LF
	char ch;
	int t=0;
	do
	{
		if (m_pSsl) {
			n = SSL_read((SSL*)m_pSsl,&ch,1);
		} else {
			n = recv(m_sock, &ch, 1, 0);
		}

		if(n == 0)
		{
			// Connection has been closed
			LogSMTPTraffic("Connection Closed -setting reply to 421 TIMED OUT (CONNECTION CLOSED)");
			ch='\0';
			//
			// Return a "TIMED OUT" indication for those servers that
			// don't handle it properly....
			//
			strcpy(Reply,"421 TIMED OUT (CONNECTION CLOSED)");
		}
		else if(n == 1)
		{
			if((ch == '\n') || (t >= (sizeof(Reply)-1))) ch='\0';
			if(ch != '\r') Reply[t++]=ch;
		}
		else
		{
#ifdef WIN32
			// RHT 15/07/2008 - try and figure out why we landed here!
			// Have to do this first as LogFTPTraffic corrupts the error number.
			if (n == SOCKET_ERROR)
			{
				LPVOID lpMsgBuf;
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf, 0, NULL  );
				LogSMTPTraffic("WSAGetLastError reports %s", (LPCTSTR)lpMsgBuf);
				LocalFree( lpMsgBuf );
			}
			else
			{
				LogSMTPTraffic("n = %d", n);
			}
#endif /*WIN32*/

			LogSMTPTraffic("recv failed - setting Reply to 421 TIMED OUT (RECV failed)");
			strcpy(Reply,"421 TIMED OUT (RECV failed)");
			break;	// recv failed
		}
	} while ((int)ch);
	return Reply;
}


void SMTP::getWelcomeText()
{
	bool MoreToRead = false;
	do
	{
		char *res = readLineFromServer();

		LogSMTPTraffic("<--- %s",res ? res : "(null)");

		if(!res) { break; }

		//
		// Welcome text always starts with 220. The character after the 220 is either a
		// space or a dash. A dash means there's more lines to follow....
		//
		char *x=res;
		while((*x == ' ') || (*x == '\t')) x++; // Find first significant digit
		if((*x >= '0') && (*x <= '9'))
		{
			// Digits...
			while(*x && (*x != ' ') && (*x != '-')) x++; // Find end of digits
			if(*x == '-')
			{
				MoreToRead = true;
			}
			else
			{
				MoreToRead = false;
			}
		}
		else
		{
			// No digits starting this line, probably a continuation line
			MoreToRead = true;
		}
	} while(MoreToRead);
}


/**
 * Records the capabilities of the server we are talking to, so that we can
 * pick a supported authentication mechanism.  A typical response looks like
 * this:
 *   250-SIZE 52428800
 *   250-8BITMIME
 *   250-PIPELINING
 *   250-AUTH CRAM-MD5
 *   250-STARTTLS
 *   250 HELP
 */
void SMTP::processCapabilities(const char *line)
{
	if(strstr(line, "AUTH ")) {
		if(strstr(line, "PLAIN")) { m_capabilites |= AUTH_PLAIN; }
		if(strstr(line, "LOGIN")) { m_capabilites |= AUTH_LOGIN; }
		if(strstr(line, "CRAM-MD5")) { m_capabilites |= AUTH_CRAM_MD5; }
	}
	if(strstr(line, "STARTTLS")) { m_starttls = true; }
}


void SMTP::processChallenge(const char *line)
{
	//debug1("processChallenge '%s'", line);
	if(strstr(line, "334 ") != line) {
		debug1("Challenge failed - not 334");
		return;
	}
	// Skip the "334 "
	line += 4;
	
	m_challenge = strdup(line);
}

int SMTP::sendCommandToServer(const char *cmd, const char *param /*= NULL*/, void (SMTP::*callback)(const char*) /*= NULL*/)
{
	/* Sends the specified "cmd" to the server and returns the
	 * result code.
	 */
	char		*x;
	static char Reply[1024];

	if(param && param[0])
	{
		x=(char *)malloc(strlen(cmd)+strlen(param)+4);
		sprintf(x,"%s %s\r\n",cmd,param);
		LogSMTPTraffic("---> %s %s",cmd,param);
	}
	else
	{
		x=(char *)malloc(strlen(cmd)+3);
		strcpy(x,cmd);
		strcat(x,"\r\n");
		LogSMTPTraffic("---> %s",cmd);
	}

	int a = sendDataToServer(x, strlen(x));
	free(x);
	if (a == -1)
	{
		// Send has failed - probably since other end has closed connection
		// due to a timeout or somesuch....
		//
		LogSMTPTraffic("Connection failed - setting reply to 421 TIMED OUT (CONNECTION FAILED)");
		strcpy(Reply,"421 TIMED OUT (CONNECTION FAILED)");
	}
	else
	{
		bool AnotherLineComing = false;
		do
		{
			strcpy(Reply,readLineFromServer());
			LogSMTPTraffic("<--- %s",Reply);	// RHT 15/07/2008 - Log hostname

			if(callback) {
				(this->*callback)(Reply);
			}

			// The reply should start with a number. If the first character
			// after the digits is a "-" then there's another line coming...
			char *t=Reply;
			if((*t >= '0') && (*t <= '9'))
			{
				while((*t >= '0') && (*t <= '9')) t++; // get to first non-numeric character
				if(*t == '-')
				{
					AnotherLineComing = true;
				}
				else
				{
					AnotherLineComing = false;
				}
			}
			else
			{
				AnotherLineComing = true;
			}
		}
		while(AnotherLineComing);
	}
	
	return (int) atol(Reply);
}


void SMTP::closeConnection()
{
	LogSMTPTraffic("Closing connection.");

	if(m_sock)
	{
#ifdef WIN32
		closesocket(m_sock);
#else
		close(m_sock);
#endif
		m_sock = 0;
	}
}


void SMTP::addRecipient(const char *rcpt, const char *realname)
{
	if(m_emails.get(rcpt)) {
		debug1("skipping duplicicate recipient '%s'", rcpt);
		return;
	}

	MailAddress *nma = new MailAddress(rcpt, realname);
	if(m_rcpts) {
		MailAddress *ma;
		for(ma = m_rcpts; ma->m_next; ma = ma->m_next) {}
		ma->m_next = nma;
	} else {
		m_rcpts = nma;
	}

	m_emails.put(rcpt, rcpt);
}


void SMTP::addCarbonCopy(const char *cc, const char *realname)
{
	if(m_emails.get(cc)) {
		debug1("skipping duplicicate cc recipient '%s'", cc);
		return;
	}

	MailAddress *nma = new MailAddress(cc, realname);
	if(m_ccs) {
		MailAddress *ma;
		for(ma = m_ccs; ma->m_next; ma = ma->m_next) {}
		ma->m_next = nma;
	} else {
		m_ccs = nma;
	}	 

	m_emails.put(cc, cc);
}


void SMTP::addAttachment(const char *name, unsigned char *data, unsigned long datalen)
{
	MailAttachment *nma = new MailAttachment(name, data, datalen);
	if(m_attachs) {
		MailAttachment *ma;
		for(ma = m_attachs; ma->m_next; ma = ma->m_next) {}
		ma->m_next = nma;
	} else {
		m_attachs = nma;
	}	 
}


bool SMTP::isOkay()
{
	if(!m_rcpts) {
		return false;
	}
	return true;
}

char* SMTP::get_ssl_error()
{
  SSL_load_error_strings();
  return ERR_error_string(ERR_get_error(), NULL);
}

bool SMTP::EstablishEncryptedConnection()
{
	// Set up SSL connection
	if (!m_sslInitialised) {
		m_sslInitialised = true;
		SSL_library_init();
	}
	if (!m_ssl_ctx) {
		SSL_CTX* p_ctx = SSL_CTX_new(SSLv23_method());
		if (!p_ctx) {
			LogSMTPTraffic("SSL_CTX_new failed\n");
			return false;
		}

		// SSL_OP_NO_SSLv2, SSL_OP_NO_SSLv3, SSL_OP_NO_TLSv1
		SSL_CTX_set_options(p_ctx, SSL_OP_ALL);

		if (SSL_CTX_set_cipher_list (p_ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH") != 1) {
			LogSMTPTraffic("SSL_CTX_set_cipher_list failed\n");
			return false;
		}

		if (RAND_status() != 1) {
			LogSMTPTraffic("RAND_status() did not return 1\n");
			return false;
		}
		m_ssl_ctx = p_ctx;
	}
	SSL_CTX* p_ctx = (SSL_CTX*) m_ssl_ctx;
	SSL* p_ssl = SSL_new(p_ctx);
	if (!p_ssl) {
		LogSMTPTraffic("SMTP: SSL_new failed\n");
		return false;
	}
	if (!SSL_set_fd(p_ssl, m_sock)) {
		LogSMTPTraffic("SMTP: SSL_set_fd failed\n");
		SSL_free(p_ssl);
		return false;
	}

	if (SSL_connect(p_ssl) <= 0)
	{
		m_LastMessage = strdup(get_ssl_error());
		LogSMTPTraffic("SMTP: SSL_connect failed: %s\n", m_LastMessage);
		SSL_free(p_ssl);
		return false;
	}
	m_pSsl = p_ssl;
	LogSMTPTraffic("SMTP: encrypted connection establised\n");
	return true;
}

bool SMTP::sendMessage()
{
	static const char *boundary = "----=_NextPart_000_0059_01BEA6E2.1A467F40";

	if(!isOkay()) {
		return false;
	}

	InitialiseWinsock();

	int tries = 0, maxtries = 4;
	do
	{
		tries++;
		LogSMTPTraffic("Connecting to %s:%d (%d)...", m_mailhost, m_port, tries);
		m_sock = EstablishOutgoingConnection(m_port, m_mailhost);
	} while(!m_sock && (tries < maxtries));

	if(!m_sock)
	{
		LogSMTPTraffic("Failed to connect to %s:%d", m_mailhost, m_port);
		return false;
	}
	else if((long) m_sock == -1)
	{
		LogSMTPTraffic("Unable to resolve host %s", m_mailhost);
		return false;
	}

	LogSMTPTraffic("Connected to %s:%d", m_mailhost, m_port);
	//
	// Do this if SSL is set
	//
	if (m_useSSL) {
		EstablishEncryptedConnection();
	}
	//
	// If we need SSL then establish encrypted connection here....
	//
	getWelcomeText();

	// RHT 10/01/2011 - use our own hostname instead of openmakesoftware.com if we have it
	char myhostname[128];
	int hnok = gethostname(myhostname, sizeof(myhostname));
	myhostname[sizeof(myhostname)-1] = '\0';

	// Say Hello to the mail server and wait for response
	// RHT 06/07/2012 - we use ESMTP EHLO if username is specified
	// RHT 30/01/2014 - use callback to record capabilities
	int res = sendCommandToServer((m_username ? "EHLO" : "HELO"),
		((hnok == 0) ? myhostname : "openmakesoftware.com"), &SMTP::processCapabilities);
	if((res < 200) || (res > 299))
	{
		closeConnection();
		return false;
	}

	char buf[1024];

	// RHT 06/07/2012 - new AUTH section
	if(m_username) {
		// If we didn't see any capabilities we recognise, try LOGIN
		if(!m_capabilites) {
			m_capabilites = AUTH_LOGIN;
		}

		if(m_starttls && !m_useSSL) {
			// Set up SSL connection
			int res = sendCommandToServer("STARTTLS");
			if (res != 220) {
				closeConnection();
				return false;
			}
			EstablishEncryptedConnection();
			// re-issue the greeting in case advertised services over encrypted
			// connections are different or in case the server insists on us
			// saying HELLO again after establishing an encrypted connection.
			res = sendCommandToServer((m_username ? "EHLO" : "HELO"),
				((hnok == 0) ? myhostname : "openmakesoftware.com"), &SMTP::processCapabilities);
		}
		
		if(m_capabilites & AUTH_CRAM_MD5) {
			res = sendCommandToServer("AUTH", "CRAM-MD5", &SMTP::processChallenge);
			if((res != 334) || !m_challenge) {
				closeConnection();
				return false;
			}
			char* resp = cmac_md5(m_challenge, m_username, m_password);
			if(!resp) {
				closeConnection();
				return false;
			}
			res = sendCommandToServer(resp);
			free(resp);
			if((res < 200) || (res > 299)) {
				closeConnection();
				return false;
			}
		} else if(m_capabilites & AUTH_LOGIN) {
			char *b64username = base64encode((unsigned char*) m_username, strlen(m_username));
			char *b64password = base64encode((unsigned char*) m_password, strlen(m_password));

			res = sendCommandToServer("AUTH", "LOGIN");
			if(res != 334) {
				closeConnection();
				return false;
			}
			res = sendCommandToServer(b64username);
			if(res != 334) {
				closeConnection();
				return false;
			}
			res = sendCommandToServer(b64password);
			if((res < 200) || (res > 299)) {
				closeConnection();
				return false;
			}

			free(b64username);
			free(b64password);
		} else if(m_capabilites & AUTH_PLAIN) {
			// Build auth string as authorization-id\0authentication-id\0passwd
			// - authorization-id is blank if same as authentication-id
			char plain[1024];
			plain[0] = '\0';
			sprintf(&plain[1], "%s", m_username);
			sprintf(&plain[strlen(m_username)+1], "%s", m_password);
			char *b64plain = base64encode((unsigned char*) plain, strlen(m_username)+strlen(m_password)+1);
			sprintf(buf, "PLAIN %s", b64plain);
			free(b64plain);

			res = sendCommandToServer("AUTH", buf);
			if((res < 200) || (res > 299)) {
				closeConnection();
				return false;
			}
		} else {
			LogSMTPTraffic("No supported authentication protcols");
			closeConnection();
			return false;
		}
	}

	// Request to send a mail from us
	sprintf(buf, "FROM:<%s>", m_from);
	res = sendCommandToServer("MAIL", buf);
	if((res < 200) || (res > 299)) {
		closeConnection();
		return false;
	}

	// Add all the recipients
	MailAddress *ma;
	for(ma = m_rcpts; ma; ma = ma->m_next)
	{
		sprintf(buf, "TO:<%s>", ma->m_email);
		res = sendCommandToServer("RCPT", buf);
		if((res < 200) || (res > 299))
		{
			closeConnection();
			return false;
		}
	}
	// Carbon Copies are also recipients
	for(ma = m_ccs; ma; ma = ma->m_next)
	{
		sprintf(buf, "TO:<%s>", ma->m_email);
		res = sendCommandToServer("RCPT", buf);
		if((res < 200) || (res > 299))
		{
			LogSMTPTraffic("Cc to %s rejected", ma->m_email);
		}
	}

	// Now the message - we will build this on the fly
	res = sendCommandToServer("DATA");
	if((res != 354))
	{
		closeConnection();
		return false;
	}

	// From line
	sprintf(buf,"From: %s <%s>\r\n",m_from,m_from);
	LogSMTPTraffic("%s", buf);
	sendDataToServer(buf, strlen(buf));

	// To lines
	for(ma = m_rcpts; ma; ma = ma->m_next)
	{
		if(ma->m_realname) {
			sprintf(buf, "To: %s <%s>\r\n", ma->m_realname, ma->m_email);
		} else {
			sprintf(buf, "To: %s\r\n", ma->m_email);
		}
		LogSMTPTraffic("%s", buf);
		sendDataToServer(buf, strlen(buf));
	}

	// Carbon Copy lines
	for(ma = m_ccs; ma; ma = ma->m_next)
	{
		char buf[1024];
		if(ma->m_realname) {
			sprintf(buf, "Cc: %s <%s>\r\n", ma->m_realname, ma->m_email);
		} else {
			sprintf(buf, "Cc: %s\r\n", ma->m_email);
		}
		LogSMTPTraffic("%s", buf);
		sendDataToServer(buf, strlen(buf));
	}

	// Subject line
	sprintf(buf, "Subject: %s\r\n", m_subject);
	sendDataToServer(buf, strlen(buf));

	// Now the headers for the message
	if(m_attachs)
	{
		char *ce=(char *)((m_useHTML)?
		"Content-Type: text/html; charset=\"iso-8859-1\"\r\n"
		"Content-Transfer-Encoding: 7bit;\r\n"
		:
		"Content-Type: text/plain; charset=\"iso-8859-1\"\r\n"
		"Content-Transfer-Encoding: quoted-printable\r\n"
		);

		char *data = (char*) malloc(10*1024);
		sprintf(data, "Mime-Version: 1.0\r\n"
			"Content-Type: multipart/mixed; boundary=\"%s\"\r\n"
			"Content-Transfer-Encoding: 7bit\r\n"
			"\r\n"
			"This is a multipart message!\r\n"
			"\r\n"
			"--%s\r\n"
			"%s"
			"\r\n",
			boundary, boundary,ce);
		//LogSMTPTraffic("Sending %d bytes of header", strlen(data));
		LogSMTPTraffic("%s", data);
		sendDataToServer(data, strlen(data));
		free(data);
	}
	else
	{
		// headers for plain message
		char *data = (char*) malloc(10*1024);
		sprintf(data, "Content-Type: text/%s; charset=\"iso-8859-1\"\r\n"
		"Content-Transfer-Encoding: 7bit\r\n"
		"\r\n",m_useHTML?"html":"plain");
		LogSMTPTraffic("Sending %d bytes of header", strlen(data));
		LogSMTPTraffic("%s", data);
		sendDataToServer(data, strlen(data));
		free(data);
	}

	// Message body
	// If the lines in the body end with simple 0A then replace with 0D0A
	bool fixedLineEndings=false;
	int nl=0;
	char *p=(char *)m_body;
	while(*p) {
		if (*p++=='\n') nl++;
	}
	char *newbody = (char *)malloc(strlen(m_body)+nl+1);
	char *q=newbody;
	p=(char *)m_body;
	int pc=0;
	while (m_body[pc]) {
		if (m_body[pc]=='\n' && (!pc || (pc && m_body[pc-1]!='\r'))) {
			fixedLineEndings=true;
			*q++ = '\r';
		}
		*q++ = m_body[pc++];
	}
	*q='\0';

	if(m_attachs)
	{
		// Send as quoted-printable
		char *data = fixedLineEndings?quotedPrintableEncode(newbody, strlen(newbody)):quotedPrintableEncode(m_body, strlen(m_body));
		LogSMTPTraffic("%s", data);
		sendDataToServer(data, strlen(data));
		free(data);
	}
	else
	{
		// Send as plain text
		if (fixedLineEndings) {
			LogSMTPTraffic("%s", newbody);
			sendDataToServer(newbody, strlen(newbody));
		} else {
			LogSMTPTraffic("%s", m_body);
			sendDataToServer(m_body, strlen(m_body));
		}
	}

	if (fixedLineEndings) free(newbody);

	// Attachments
	if(m_attachs)
	{
		for(MailAttachment *ma = m_attachs; ma; ma = ma->m_next)
		{
			char *data = (char*) malloc(10*1024);
			sprintf(data, "\r\n\r\n"
				"--%s\r\n"
				"Content-Type: text/plain; name=\"%s\"\r\n"
				"Content-Transfer-Encoding: base64\r\n"
				"Content-Disposition: attachment; filename=\"%s\"\r\n"
				"\r\n",
				boundary, ma->m_name, ma->m_name);
			LogSMTPTraffic(data);
			sendDataToServer(data, strlen(data));
			free(data);
			data = base64encode(ma->m_data, ma->m_datalen);
			if (strlen(data)<1024) {
				LogSMTPTraffic(data);
			} else {
				LogSMTPTraffic("Sending %d bytes of attachment", strlen(data));
			}
			sendDataToServer(data, strlen(data));
		}
		sprintf(buf, "\r\n--%s--\r\n", boundary);
		sendDataToServer(buf, strlen(buf));
	}
	else
	{
		// Make sure dot goes on blank line
		sendDataToServer("\r\n", strlen("\r\n"));
	}

	// Close message
	res = sendCommandToServer(".");
	if((res < 200) || (res > 299))
	{
		closeConnection();
		return false;
	}

	sendCommandToServer("QUIT");
	closeConnection();

	CloseWinsock();
	return true;
}


void SMTP::dump()
{
	printf("From: %s\n", m_from);

	MailAddress *ma;

	// To lines
	for(ma = m_rcpts; ma; ma = ma->m_next) {
		if(ma->m_realname) {
			printf("To: %s <%s>\n", ma->m_realname, ma->m_email);
		} else {
			printf("To: %s\n", ma->m_email);
		}
	}

	// Carbon Copy lines
	for(ma = m_ccs; ma; ma = ma->m_next) {
		if(ma->m_realname) {
			printf("Cc: %s <%s>\n", ma->m_realname, ma->m_email);
		} else {
			printf("Cc: %s\n", ma->m_email);
		}
	}

	// Subject line
	printf("Subject: %s\n", m_subject);

	// Body
	printf("\n%s\n", m_body);

	// Attachments
	if(m_attachs) {
		for(MailAttachment *ma = m_attachs; ma; ma = ma->m_next) {
			printf("Attachment: %s\n", ma->m_name);
		}
	}
}
