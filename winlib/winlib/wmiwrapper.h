#ifndef __wmiwrapper_h

#define __wmiwrapper_h

#ifdef WIN32
#include <Wbemidl.h>
#include <wincred.h>
#endif

class WMIWrapper
{
private:
	IWbemServices *m_pSvc;
	IWbemLocator *m_pLoc;
	COAUTHIDENTITY *m_userAcct;
    COAUTHIDENTITY m_authIdent;
    wchar_t m_pszUserName[CREDUI_MAX_USERNAME_LENGTH+1];
    wchar_t m_pszDomain[CREDUI_MAX_USERNAME_LENGTH+1];
	bool m_coinited;
	bool m_useNTLM;

	bool connectServer(const char *machine, const char *path, const wchar_t *name, const wchar_t *pwd);

public:
	WMIWrapper();
	~WMIWrapper();

	bool initialize();

	bool createLocator();

	bool connectServerUsingToken(const char *machine, const char *path);
	bool connectServerUsingCreds(const char *machine, const char *path, const wchar_t *username, const wchar_t *password);

	bool createCoAuthority(const wchar_t *username, const wchar_t *password);

	bool createProxyBlanket();
	// bool createProxyBlanket(IEnumWbemClassObject *pEnumerator);

};


#endif /*__wmiwrapper_h*/
