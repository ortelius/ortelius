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
