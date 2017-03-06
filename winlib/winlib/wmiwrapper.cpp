#define _WIN32_DCOM
#define UNICODE




#include "wmiwrapper.h"

// #include <tchar.h> // for tprintf

#include <iostream>
using namespace std;
#include <comdef.h>
#include <strsafe.h>




# pragma comment(lib, "wbemuuid.lib")
# pragma comment(lib, "credui.lib")


extern void reportError(int error);
extern int charToWchar(const char *str, wchar_t *buf, int buflen);
extern int sprintfCharToWchar(wchar_t *buf, int buflen, const char *fmt, ...);


WMIWrapper::WMIWrapper()
	: m_pSvc(NULL), m_pLoc(NULL), m_userAcct(NULL), m_coinited(false)
{
	m_useNTLM = true;
}


WMIWrapper::~WMIWrapper()
{
	if(m_pSvc) {
        m_pSvc->Release();
	}
	if(m_pLoc) {
		m_pLoc->Release();
	}
	if(m_coinited) {
		CoUninitialize();
	}
}


bool WMIWrapper::initialize()
{
    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    HRESULT hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
    if (FAILED(hres))
    {
        cout << "Failed to initialize COM library. Error code = 0x" 
            << hex << hres << endl;
        return false;                  // Program has failed.
    }

	m_coinited = true;

    // Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------

	/*
    hres =  CoInitializeSecurity(
        NULL, 
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_PKT, //DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IMPERSONATE, //IDENTIFY,    // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
        );
*/
	hres =  CoInitializeSecurity(
        NULL, 
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IMPERSONATE, //IDENTIFY,    // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
        );
                      
    if (FAILED(hres))
    {
        cout << "Failed to initialize security. Error code = 0x" 
            << hex << hres << endl;
        return false;                    // Program has failed.
    }

	return true;
}


bool WMIWrapper::createLocator()
{
    
    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    HRESULT hres = CoCreateInstance(
        CLSID_WbemLocator,             
        0, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator, (LPVOID *) &m_pLoc);
 
    if (FAILED(hres))
	{
        cout << "Failed to create IWbemLocator object."
            << " Err code = 0x"
            << hex << hres << endl;
        return false;
	}

	return true;
}


bool WMIWrapper::connectServer(const char *machine, const char * path,const wchar_t *name, const wchar_t *pwd)
{
	wchar_t pszServer[1204];
	sprintfCharToWchar(pszServer, sizeof(pszServer), "\\\\%s\\%s", machine, path);

	DWORD bufSize = MAX_PATH;
	TCHAR domainNameBuf[ MAX_PATH ];

	GetComputerNameEx(ComputerNameDnsDomain,domainNameBuf,&bufSize);
	char domain[MAX_PATH];
	wcstombs(domain,domainNameBuf,sizeof(domain));

	//printf("this domain=[%s]\n",domain);

	/* need to scan for a domain name in the username, remove it if present but set the
	 * domain to it and override the deploy engine domain.
	 *
	 */
	char nc[MAX_PATH];
	wchar_t wnc[MAX_PATH];

	HRESULT hres;

	if (name)
	{
		wcstombs(nc,name,sizeof(nc));
		char *bslash = strchr(nc,'\\');
		if (bslash)
		{
			*bslash='\0';
			sprintfCharToWchar(wnc,sizeof(wnc),bslash+1);
			strcpy(domain,nc);
			name = &(wnc[0]);
		}

		wchar_t pszAuthority[CREDUI_MAX_USERNAME_LENGTH+1];
		// sprintfCharToWchar(pszAuthority, sizeof(pszAuthority), (m_useNTLM ? "NTLMDomain:%s" : "kERBEROS:%s"), machine);
		sprintfCharToWchar(pszAuthority, sizeof(pszAuthority), (m_useNTLM ? "NTLMDomain:%s" : "kERBEROS:%s"), domain);
		//printf("name is [");
		//wprintf(name);
		//printf("]\n");
		//printf("pszServer is [");
		//wprintf(pszServer);
		//printf("]\n");
		//printf("pszAuthority is [");
		//if (domain[0]) {
		//	wprintf(pszAuthority);
		//} else {
		//	printf("NULL");
		//}
		//printf("]\n");
		hres = m_pLoc->ConnectServer(
        _bstr_t(pszServer),
        _bstr_t(name),					   // User name
        _bstr_t(pwd),					   // User password
        NULL,                              // Locale             
        NULL,                              // Security flags
		_bstr_t(domain[0]?pszAuthority:NULL),				// Authority        
        NULL,                              // Context object 
        &m_pSvc                              // IWbemServices proxy
        );
	}
	else
	{
		hres = m_pLoc->ConnectServer(
        _bstr_t(pszServer),
        NULL,							   // User name
        NULL,							   // User password
        NULL,                              // Locale             
        NULL,                              // Security flags
		NULL,								// Authority        
        NULL,                              // Context object 
        &m_pSvc                              // IWbemServices proxy
        );
	}
	
	/*
    HRESULT hres = m_pLoc->ConnectServer(
        _bstr_t(pszServer),
        _bstr_t(name),					   // User name
        _bstr_t(pwd),					   // User password
        NULL,                              // Locale             
        NULL,                              // Security flags
        _bstr_t(m_useNTLM?NULL:pszAuthority),// Authority        
        NULL,                              // Context object 
        &m_pSvc                              // IWbemServices proxy
        );
		*/
	/*
	HRESULT hres = m_pLoc->ConnectServer(
        _bstr_t(pszServer),
        _bstr_t(name),					   // User name
        _bstr_t(pwd),					   // User password
        NULL,                              // Locale             
        NULL,                              // Security flags
		_bstr_t(domain[0]?pszAuthority:NULL),				// Authority        
        NULL,                              // Context object 
        &m_pSvc                              // IWbemServices proxy
        );
    */
	//printf("done ConnectServer hres=0z%lx\n",hres);

    if (FAILED(hres))
    {
		reportError(hres);
        wcout << L"Could not connect to " << pszServer << L". Error code = 0x" 
             << hex << hres << endl;
        return false;                // Program has failed.
    }

	return true;
}

bool WMIWrapper::connectServerUsingToken(const char *machine, const char *path)
{
	return connectServer(machine, path, NULL, NULL);
}


bool WMIWrapper::connectServerUsingCreds(const char *machine, const char * path, const wchar_t *username, const wchar_t *password)
{
	return connectServer(machine, path, username, password);
}


bool WMIWrapper::createCoAuthority(const wchar_t *username, const wchar_t *password)
{
    // Create COAUTHIDENTITY that can be used for setting security on proxy

	//printf("createCoAuthority: username=[");
	//wprintf(username);
	//printf("]\n");
    memset(&m_authIdent, 0, sizeof(COAUTHIDENTITY));
    m_authIdent.PasswordLength = wcslen (password);
    m_authIdent.Password = (USHORT*)password;

    LPWSTR slash = (LPWSTR)wcschr (username, L'\\');
    if( slash == NULL )
    {
        cout << "Could not create Auth identity. No domain specified\n" ;
        return false;               // Program has failed.
    }

    StringCchCopy(m_pszUserName, CREDUI_MAX_USERNAME_LENGTH+1, slash+1);
    m_authIdent.User = (USHORT*)m_pszUserName;
    m_authIdent.UserLength = wcslen(m_pszUserName);

    StringCchCopyN(m_pszDomain, CREDUI_MAX_USERNAME_LENGTH+1, username, slash - username);
    m_authIdent.Domain = (USHORT*)m_pszDomain;
    m_authIdent.DomainLength = slash - username;
    m_authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;

    m_userAcct = &m_authIdent;

	return true;
}


bool WMIWrapper::createProxyBlanket()
{
    // Step 6: --------------------------------------------------
    // Set security levels on a WMI connection ------------------

    HRESULT hres = CoSetProxyBlanket(
       m_pSvc,                           // Indicates the proxy to set
       RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
       RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
       COLE_DEFAULT_PRINCIPAL,         // Server principal name 
       RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
       RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
       m_userAcct,                       // client identity
       EOAC_NONE                       // proxy capabilities 
    );

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket. Error code = 0x" 
            << hex << hres << endl;
        return false;               // Program has failed.
    }

	return true;
}

/*
bool WMIWrapper::createProxyBlanket(IEnumWbemClassObject *pEnumerator)
{

    // Step 8: -------------------------------------------------
    // Secure the enumerator proxy

    HRESULT hres = CoSetProxyBlanket(
        pEnumerator,                    // Indicates the proxy to set
        RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
        COLE_DEFAULT_PRINCIPAL,         // Server principal name 
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
        RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
        m_userAcct,                       // client identity
        EOAC_NONE                       // proxy capabilities 
        );

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket on enumerator. Error code = 0x" 
             << hex << hres << endl;
        pEnumerator->Release();
        return false;               // Program has failed.
    }

	return true;
}
*/








