#ifndef __netconn_h

#define __netconn_h


#ifdef UNICODE
#define CHAR_TYPE wchar_t
#else
#define CHAR_TYPE char
#endif /*UNICODE*/

class NetworkConnection
{
private:
	CHAR_TYPE m_share[1024];
	bool m_connected;

public:
	NetworkConnection(const char *machine, const char *share);
	~NetworkConnection();
	bool connect(const char *username = NULL, const char *password = NULL);
	bool disconnect();
	char *GetShareName();
};


#endif /*__netconn_h*/