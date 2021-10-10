/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
