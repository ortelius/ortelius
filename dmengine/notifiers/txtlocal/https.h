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
#ifndef __https_h

#define __https_h


class HTTPS
{
private:
	char *m_servername;
	int m_port;
	char *m_uri;
	const char *m_content;

	int m_sock;
	void *m_ssl_ctx;
	void *m_pSsl;

	bool ssl_connect();
	int sendData(const char *data, long len);
	char *readLine();
	bool readData(char *dataPtr, long length);
	void closeConnection();

public:
	HTTPS(
		const char *servername, int port, const char *uri,
		const char *content, const char *logfile);
	~HTTPS();

	bool isOkay();
	bool post();

	void dump();
};


#endif /*__https_h*/
