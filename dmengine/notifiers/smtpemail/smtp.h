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
#ifndef __SMTP_H

#define __SMTP_H


#include "../../dmapi/hashtable.h"


// This is a bitmask - the higher the bit order the greater the preference
#define AUTH_PLAIN		1
#define AUTH_LOGIN		2
#define AUTH_CRAM_MD5	4


class SMTP
{
private:
	const char *m_mailhost;
	int m_port;
	const char *m_username;
	const char *m_password;
	const char *m_subject;
	const char *m_body;
	const char *m_from;
	class MailAddress *m_rcpts;
	class MailAddress *m_ccs;
	class MailAttachment *m_attachs;
	int m_sock;
	const char *m_logfile;
	bool m_useSSL;
	bool m_useHTML;
	class IOutputStream *m_logstream;
	StringHashtable m_emails;

	bool m_starttls;
	int m_capabilites;
	bool m_sslInitialised;
	char *m_challenge;
	void *m_ssl_ctx;
	void *m_pSsl;
	char *m_LastMessage;

	void LogSMTPTraffic(const char* fmt, ...); 
	int sendDataToServer(const char *data, long len);
	char *readLineFromServer();
	void getWelcomeText();
	void processCapabilities(const char *line);
	void processChallenge(const char *line);
	int sendCommandToServer(const char *cmd, const char *param = 0, void (SMTP::*callback)(const char*) = 0);
	void closeConnection();
	char* get_ssl_error();
	bool EstablishEncryptedConnection();

public:
	SMTP(const char *mailhost, int port, const char *username,
		 const char *password, const char *from, const char *subject,
		 const char *body, const char *logfile, bool useSSL, bool useHTML, class IOutputStream *logstream);
	~SMTP();
	void addRecipient(const char *rcpt, const char *realname = 0);
	void addCarbonCopy(const char *cc, const char *realname = 0);
	void addAttachment(const char *name, unsigned char *data, unsigned long datalen);
	bool isOkay();
	bool sendMessage();
	void dump();
};

#endif /*__SMTP_H*/
