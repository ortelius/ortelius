//#define UNICODE

#include <stdio.h>
#include <windows.h>

#include "netconn.h"

#include <iostream>
using namespace std;

#include <winnetwk.h>

#pragma comment(lib, "mpr.lib")		// for WNetAddConnection2


// extern void reportError(int error);
extern int charToWchar(const char *str, wchar_t *buf, int buflen);
extern int sprintfCharToWchar(wchar_t *buf, int buflen, const char *fmt, ...);

void reportError(int error)
{
	LPVOID lpMsgBuf;
	DWORD msglen = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		/*FORMAT_MESSAGE_FROM_HMODULE | //*/ FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		/*"C:\\Windows\\System32\\wbem\\wmiutils.dll", //*/ NULL,
		error, //GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	if(lpMsgBuf) {
		wcout << L"Error: " << (LPTSTR) lpMsgBuf << L" (" << msglen << L")" << endl;
	} else {
		wcout << L"Error: " << hex << error << endl;
	}
}

NetworkConnection::NetworkConnection(const char *machine, const char *share)
	: m_connected(false)
{
#ifdef UNICODE
	sprintfCharToWchar(m_share, sizeof(m_share), "\\\\%s\\%s$", machine, share);
#else
	sprintf(m_share, "\\\\%s\\%s", machine, share);
#endif /*UNICODE*/
}


NetworkConnection::~NetworkConnection()
{
	if(m_connected) {
		if(!disconnect()) {
#ifdef UNICODE
			wcout << L"Failed to disconnect from " << m_share << endl;
#else
			cout << "Failed to disconnect from " << m_share << endl;
#endif /*UNICODE*/
		}
	}
}


bool NetworkConnection::connect(const char *username /*= NULL*/, const char *password /*= NULL*/)
{
#ifdef UNICODE
	wchar_t szName[256];
	wchar_t szPwd[256];
	const CHAR_TYPE *pszName = NULL;
	const CHAR_TYPE *pszPwd = NULL;
#endif /*UNICODE*/

	NETRESOURCE n;
	n.dwScope      = RESOURCE_GLOBALNET;			// GlobalNetwork (2)
    n.dwType       = RESOURCETYPE_DISK;				// Disk (1)
    n.dwDisplayType = RESOURCEDISPLAYTYPE_SHARE;	// Share (3)
    n.dwUsage      = RESOURCEUSAGE_CONNECTABLE;     // Connectable (1)
    n.lpLocalName  = NULL;
    n.lpRemoteName = m_share;
	n.lpComment    = NULL;
    n.lpProvider   = NULL;
	DWORD dwFlags = 0;

#ifdef UNICODE
	if(username) {
		charToWchar(username, szName, sizeof(szName));
		pszName = szName;
		pszName = username;
	}

	if(password) {
		charToWchar(password, szPwd, sizeof(szPwd));
		pszPwd = szPwd;
	}

	wcout << L"Share: " << m_share << L" User: " << (pszName ? pszName : L"(null)") /*<< L" Pwd: " << pszPwd*/ << endl;

	// Username and password are null to use the impersonate creds
	DWORD res = WNetAddConnection2(&n, pszPwd, pszName, dwFlags);
	if(res == ERROR_SESSION_CREDENTIAL_CONFLICT) {
		wcout << L"Already connected to " << n.lpRemoteName << L" as different user" << endl;
		if(disconnect()) {
			return connect(username, password);
		}
		return false;
	} else if(res != NO_ERROR) {
		wcout << L"Failed to connect to " << n.lpRemoteName << " (" << res << ")"  << endl;
		reportError(GetLastError());
		return false;
	}
#else
	// temp set username
	//username="WaleC";
	// end temp
	// cout << "Share: " << m_share << " User: " << (username ? username : "(null)") /*<< " Pwd: " << password*/ << endl;

	if(!username) {
		char name[256];
		DWORD namelen = sizeof(name);
		DWORD ures = WNetGetUser(NULL, name, &namelen);
		if(ures == NO_ERROR) {
			cout << "Default User: " << name << endl;
		} else {
			cout << "Failed to determine default user (" << ures << ")" << endl;
			reportError(GetLastError());
		}
	}

	// Username and password are null to use the impersonate creds
	DWORD res = WNetAddConnection2(&n, password, username, dwFlags);
	if(res == ERROR_SESSION_CREDENTIAL_CONFLICT) {
		cout << "Already connected to " << n.lpRemoteName << " as different user" << endl;
		if(disconnect()) {
			return connect(username, password);
		}
		return false;
	} else if(res != NO_ERROR) {
		cout << "Failed to connect to " << n.lpRemoteName << " (" << res << ")" << endl;
		reportError(GetLastError());
		return false;
	}
#endif /*UNICODE*/

	m_connected = true;
	return true;
}


bool NetworkConnection::disconnect()
{
	printf("in disconnect, m_connected=%s\n",m_connected?"true":"false");
	if (m_connected == false)
	{
		printf("Forcing m_connected to true\n");
		m_connected=true;
	}
	if(m_connected) {
		DWORD res = WNetCancelConnection(m_share, true);
		if(res != NO_ERROR) {
			return false;
		}
	}

	m_connected = false;
	return true;
}

char *NetworkConnection::GetShareName()
{
	return m_share;
}
