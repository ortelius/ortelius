/*
	Copyright (c) 2006 Talha Tariq [ talha.tariq@gmail.com ] 
	All rights are reserved.

	Permission to use, copy, modify, and distribute this software 
	for any purpose and without any fee is hereby granted, 
	provided this notice is included in its entirety in the 
	documentation and in the source files.

	This software and any related documentation is provided "as is" 
	without any warranty of any kind, either express or implied, 
	including, without limitation, the implied warranties of 
	merchantability or fitness for a particular purpose. The entire 
	risk arising out of use or performance of the software remains 
	with you. 

 	$Author:	Talha Tariq [ talha.tariq@gmail.com ] 
				uses some code from xCmd by Zoltan Csizmadia
	$Revision:	Talha Tariq [ talha.tariq@gmail.com ] 	
	
	$Date: 2006/10/04 09:00:00 $ 		
	
	$Version History: $			- Refactored and Restructured Code - Deleted Unnecessary variables and Functions for Memory Consumption and Optimisation.
								- Added Function StartLocalProcessAsUser for local user impersonation
								- Added Start Local Process for launching external commands
								- Added GetAdminSid, GetLocalSid, GetLogonSID, FreeLogonSid for getting tokens to pass on for logon impersonation
								- Added IsLaunchedFromAdmin to get the local admin sid
								- Added ExtractLocalBinaryResource to extract the local binary resource for local process impersonation
								- Added ProcComs to implement local  process functionality
								- Added RemCom   to implement remote process functionality
																
	$TODO:						- Add Getopt to parse command line parametres more effectively.
								- Implemement cleanup and disconnect remote share command
	
	$Description: $				- RemCom is RAT [Remote Administration Tool] that lets you execute processes on remote windows systems, copy files, 
								  process there output and stream it back. It allows execution of remote shell commands directly with full interactive console
	
	$Workfile: $				- RemCom.cpp
 */


#define _WIN32_WINNT 0x0500 //Will work only on W2K and above 

#include "RemCom.h"

#define		DESKTOP_ALL (DESKTOP_READOBJECTS | DESKTOP_CREATEWINDOW | \
			DESKTOP_CREATEMENU | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALRECORD | \
			DESKTOP_JOURNALPLAYBACK | DESKTOP_ENUMERATE | DESKTOP_WRITEOBJECTS | \
			DESKTOP_SWITCHDESKTOP | STANDARD_RIGHTS_REQUIRED)

#define		WINSTA_ALL (WINSTA_ENUMDESKTOPS | WINSTA_READATTRIBUTES | \
			WINSTA_ACCESSCLIPBOARD | WINSTA_CREATEDESKTOP | WINSTA_WRITEATTRIBUTES | \
			WINSTA_ACCESSGLOBALATOMS | WINSTA_EXITWINDOWS | WINSTA_ENUMERATE | \
			WINSTA_READSCREEN | STANDARD_RIGHTS_REQUIRED)

#define		GENERIC_ACCESS (GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE | GENERIC_ALL)

// Constant Definitions
#define		SIZEOF_BUFFER   0x100

// Local Machine Settings
TCHAR		szThisMachine[SIZEOF_BUFFER] = _T("");
TCHAR		szPassword[SIZEOF_BUFFER] = _T("");
TCHAR		szArguments[SIZEOF_BUFFER] = _T("");
TCHAR		szConsoleTitle[SIZEOF_BUFFER] = _T("");
TCHAR		szLocalBinPath[_MAX_PATH] = _T("");

// Windows Default Windows Path
LPCTSTR		lpszSystemRoot = "%SystemRoot%";
LPCTSTR		lpszLocalMachine = "\\\\localhost";
LPCTSTR		lpszLocalIP = "\\\\127.0.0.1";

// Remote Parameters
LPCTSTR		lpszMachine = NULL;
LPCTSTR		lpszPassword = NULL;
LPCTSTR		lpszUser = NULL;
LPCTSTR		lpszDomain = NULL;
LPCTSTR		lpszCommandExe = NULL;

// Named Pipes for Input and Output
HANDLE		hCommandPipe = INVALID_HANDLE_VALUE;
HANDLE		hRemoteOutPipe = INVALID_HANDLE_VALUE;
HANDLE		hRemoteStdInputPipe = INVALID_HANDLE_VALUE;
HANDLE		hRemoteErrorPipe = INVALID_HANDLE_VALUE;

//Method Declarations
BOOL		AddAceToWindowStation(HWINSTA hwinsta, PSID psid);
BOOL		AddAceToDesktop(HDESK hdesk, PSID psid);


// Show the last error's description
DWORD ShowLastError()
{
	LPVOID lpvMessageBuffer;
	DWORD rc = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		rc,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpvMessageBuffer,
		0,
		NULL
		);            
	
//	Error( _T("Error code = %d.\n", GetLastError()) );
	Error( (LPCTSTR)lpvMessageBuffer );
//	Error( _T("\n") );

	LocalFree (lpvMessageBuffer);
    //ExitProcess(GetLastError());
	return rc;
}

//Gets the SID for Admin
void* GetAdminSid()
{
	SID_IDENTIFIER_AUTHORITY ntauth = SECURITY_NT_AUTHORITY;

	void* psid = 0;
	
	if ( !AllocateAndInitializeSid( &ntauth, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &psid ) )

		ShowLastError();
	
	return psid;
}

// Gets the SID for System Account
void* GetLocalSystemSid()
{
	SID_IDENTIFIER_AUTHORITY ntauth = SECURITY_NT_AUTHORITY;
	
	void* psid = 0;
	
	if ( !AllocateAndInitializeSid( &ntauth, 1,
		SECURITY_LOCAL_SYSTEM_RID,
		0, 0, 0, 0, 0, 0, 0, &psid ) )
	
		ShowLastError();
	
	return psid;
}

// Checks if the launching process parent is local administrator
BOOL IsLaunchedFromAdmin()
{
	bool bIsAdmin = false;

	HANDLE hToken = 0;

	if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) ){
		ShowLastError();
	}

	DWORD cb = 0;
	
	GetTokenInformation( hToken, TokenGroups, 0, 0, &cb );
	
	TOKEN_GROUPS* pTokenGroups = (TOKEN_GROUPS*)malloc( cb );
	
	if ( !pTokenGroups )
		ShowLastError();
	
	if ( !GetTokenInformation( hToken, TokenGroups, pTokenGroups, cb, &cb ) )
		ShowLastError();

	void* pAdminSid = GetAdminSid();

	SID_AND_ATTRIBUTES* const end = pTokenGroups->Groups + pTokenGroups->GroupCount;
    SID_AND_ATTRIBUTES* it;

	for ( it = pTokenGroups->Groups; end != it; ++it )
		if ( EqualSid( it->Sid, pAdminSid ) )
			break;

	bIsAdmin = end != it;

	FreeSid( pAdminSid );
	free( pTokenGroups );
	CloseHandle( hToken );

	return bIsAdmin;
}

bool IsLocalSystem()
{
	bool bIsLocalSystem = false;
	HANDLE htok = 0;
	
	if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &htok ) )
		ShowLastError();

	BYTE userSid[256];
	
	DWORD cb = sizeof userSid;
	
	if ( !GetTokenInformation( htok, TokenUser, userSid, cb, &cb ) )
		ShowLastError();
	
	TOKEN_USER* ptu = (TOKEN_USER*)userSid;

	void* pLocalSystemSid = GetLocalSystemSid();

	bIsLocalSystem = EqualSid( pLocalSystemSid, ptu->User.Sid ) ? true : false;

	FreeSid( pLocalSystemSid );
	CloseHandle( htok );

	return bIsLocalSystem;
}

VOID FreeLogonSID (PSID *ppsid) 
{
	HeapFree(GetProcessHeap(), 0, (LPVOID)*ppsid);
}

BOOL GetLogonSID (HANDLE hToken, PSID *ppsid) 
{
	BOOL bSuccess = FALSE;
	DWORD dwIndex;
	DWORD dwLength = 0;
	PTOKEN_GROUPS ptg = NULL;

	// Verify the parameter passed in is not NULL.
	if (NULL == ppsid)
		goto Cleanup;

	// Get required buffer size and allocate the TOKEN_GROUPS buffer.

	if (!GetTokenInformation(
		hToken,         // handle to the access token
		TokenGroups,    // get information about the token's groups 
		(LPVOID) ptg,   // pointer to TOKEN_GROUPS buffer
		0,              // size of buffer
		&dwLength       // receives required buffer size
		)) 
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
			goto Cleanup;

		ptg = (PTOKEN_GROUPS)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, dwLength);

		if (ptg == NULL)
			goto Cleanup;
	}

	// Get the token group information from the access token.

	if (!GetTokenInformation(
		hToken,         // handle to the access token
		TokenGroups,    // get information about the token's groups 
		(LPVOID) ptg,   // pointer to TOKEN_GROUPS buffer
		dwLength,       // size of buffer
		&dwLength       // receives required buffer size
		)) 
	{
		goto Cleanup;
	}

	// Loop through the groups to find the logon SID.

	for (dwIndex = 0; dwIndex < ptg->GroupCount; dwIndex++) 
		if ((ptg->Groups[dwIndex].Attributes & SE_GROUP_LOGON_ID)
			==  SE_GROUP_LOGON_ID) 
		{
			// Found the logon SID; make a copy of it.

			dwLength = GetLengthSid(ptg->Groups[dwIndex].Sid);
			*ppsid = (PSID) HeapAlloc(GetProcessHeap(),
				HEAP_ZERO_MEMORY, dwLength);
			if (*ppsid == NULL)
				goto Cleanup;
			if (!CopySid(dwLength, *ppsid, ptg->Groups[dwIndex].Sid)) 
			{
				HeapFree(GetProcessHeap(), 0, (LPVOID)*ppsid);
				goto Cleanup;
			}
			break;
		}

		bSuccess = TRUE;

Cleanup: 

		// Free the buffer for the token groups.

		if (ptg != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)ptg);

		return bSuccess;
}


// Check the command line arguments
BOOL IsCmdLineParameter( LPCTSTR lpszParam )
{
	for( int i = 1; i < __argc; i++ )
		if ( __targv[i][0] == _T('\\') )
			continue;
		else
			if ( __targv[i][0] == _T('/') || __targv[i][0] == _T('-') )
			{
				if ( _tcsicmp( __targv[i] + 1, lpszParam ) == 0 )
					return TRUE;
			}
			else
				return FALSE;

	return FALSE;
}

LPCTSTR GetParamValue( LPCTSTR lpszParam )
{
	DWORD dwParamLength = _tcslen( lpszParam );

	for( int i = 1; i < __argc; i++ )
		if ( __targv[i][0] == _T('\\') || __targv[i][0] == _T('.'))
			continue;
		else
			if ( __targv[i][0] == _T('/') || __targv[i][0] == _T('-') )
			{
				if ( _tcsnicmp( __targv[i] + 1, lpszParam, dwParamLength ) == 0 )
					return __targv[i] + dwParamLength + 1;
			}
			else
				return NULL;

	return NULL;
}

LPCTSTR GetNthParameter( DWORD n, DWORD& argvIndex )
{
	DWORD index = 0;

	for( int i = 1; i < __argc; i++ )
	{
		if ( __targv[i][0] != _T('/') && __targv[i][0] != _T('-') )
			index++;

		if ( index == n )
		{
			argvIndex = i;
			return __targv[i];
		}
	}

	return NULL;
}

// Gets the arguments parameter
void GetRemoteCommandArguments( LPTSTR lpszCommandArguments )
{
	DWORD dwIndex = 0;
	lpszCommandArguments[0] = _T('\0');

	int StartParam=0;

	for( StartParam = 2; StartParam < __argc && (__targv[StartParam][0] == _T('/') || __targv[StartParam][0] == _T('-')); StartParam++ );
	
	for (int i=StartParam;i< __argc;i++)
	{
		if (i == StartParam)
		{
			// First Parameter - if it contains a file extension then surround with quotes. Otherwise assume it's
			// a DOS built-in command and don't put quotes around it.
			char *p;
			if (IsCmdLineParameter( _T("c")))
			{
				// We're copying the file - take the path off the command to execute
				char *x = __targv[i];
				p = (char *)0;
				while (*x)
				{
					if (*x=='\\' || *x=='/') p=x+1;
					x++;
				}
				if (!p) p = __targv[i];
			}
			else
			{
				// Not copying the file - just take the whole thing "as is"
				p = __targv[i];
				
			}
			int pl = strlen(p);
			if (p[pl-1]=='"') p[pl-1]='\0';
			if (strchr(p,'.'))
			{
				// It has a dot in it - quote it
				_tcscat( lpszCommandArguments, _T("\"") );
				_tcscat( lpszCommandArguments, p );
				_tcscat( lpszCommandArguments, _T("\"") );
			}
			else
			{
				// No dot - assume DOS built-in command and don't quote
				_tcscat( lpszCommandArguments, p );
			}
		}
		else
		{
			// parameters are always quoted
			// printf("__targv[%d]=[%s]\n",i,__targv[i]);
			_tcscat( lpszCommandArguments, _T("\"") );
			_tcscat( lpszCommandArguments, __targv[i] );
			_tcscat( lpszCommandArguments, _T("\"") );
		}
		if ( i + 1 < __argc )
				_tcscat( lpszCommandArguments, _T(" ") );
	}
}

// Gets the remote machine parameter
LPCTSTR GetRemoteMachineName()
{
	DWORD dwIndex = 0;
	LPCTSTR lpszMachine = GetNthParameter( 1, dwIndex );

	if ( lpszMachine == NULL )
		// return NULL;
		return lpszLocalIP;

	if ( _tcsnicmp( lpszMachine, _T(" "), 2 ) == 0 )
		return lpszLocalIP;

	if ( _tcsnicmp( lpszMachine, _T("\\\\"), 2 ) == 0 )
		return lpszMachine;
	// If a dot is entered we take it as localhost
	if ( _tcsnicmp( lpszMachine, _T("."), 2 ) == 0 )
		return lpszLocalIP;


	return NULL;
}

// Turns off the echo on a console input handle - Used for hiding password typing
BOOL EnableEcho( HANDLE handle, BOOL bEcho )
{
	DWORD mode;

	if ( !GetConsoleMode( handle, &mode ) )
		return FALSE;

	if ( bEcho )
		mode |= ENABLE_ECHO_INPUT;
	else
		mode &= ~ENABLE_ECHO_INPUT;

	return SetConsoleMode( handle, mode );
}

// Gets the password
BOOL PromptForPassword( LPTSTR lpszPwd )
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwRead = 0;

	Out( _T("Enter Password: ") );

	// Turn off echo
	if ( EnableEcho( hInput, FALSE ) )
	{
		// Read password from console
		::ReadConsole( hInput, lpszPwd, SIZEOF_BUFFER, &dwRead, NULL );

		// Ignore ENTER (0x0D0A) 
		lpszPwd[max( dwRead-2, 0 )] = _T('\0');

		// Turn echo on
		EnableEcho( hInput, TRUE );

		Out( _T("\n\n") );
	}
	else
	{
		//Console input doesn't support echo on/off
		Out( _T("\n") );
		Error( _T("Couldn't turn off echo to hide password chars.\n") );
	}

	return TRUE;
}

BOOL SetConnectionCredentials( BOOL bPromptForPassword )
{
	// Check the command line
	lpszPassword = GetParamValue( _T("pwd:") );
	lpszUser = GetParamValue( _T("user:") );

	if ( lpszUser != NULL && lpszPassword != NULL && !bPromptForPassword )
		if ( _tcscmp( lpszPassword, _T("*") ) == 0 )
			// We found user name, and * as password, which means prompt for password
			bPromptForPassword = TRUE;

	if ( bPromptForPassword )
	{
		// We found user name, and * as password, which means prompt for password
		lpszPassword = szPassword;
		if ( !PromptForPassword( szPassword ) )
			return FALSE;
	}

	return TRUE;
}

// Establish Connection to Remote Machine
BOOL EstablishConnection( LPCTSTR lpszRemote, LPCTSTR lpszResource, BOOL bEstablish )
{
	TCHAR szRemoteResource[_MAX_PATH];

	DWORD rc;

	// Remote resource, \\remote\ipc$, remote\admin$, ...
	_stprintf( szRemoteResource, _T("%s\\%s"), lpszRemote, lpszResource );

	//
	// disconnect or connect to the resource, based on bEstablish
	//
	if ( bEstablish ) 
	{
		NETRESOURCE nr;
		nr.dwType = RESOURCETYPE_ANY;
		nr.lpLocalName = NULL;
		nr.lpRemoteName = (LPTSTR)&szRemoteResource;
		nr.lpProvider = NULL;

		//Establish connection (using username/pwd)
		rc = WNetAddConnection2( &nr, lpszPassword, lpszUser, FALSE );

		switch( rc )
		{

		case ERROR_ACCESS_DENIED:
		case ERROR_INVALID_PASSWORD:
		case ERROR_LOGON_FAILURE:

		case ERROR_SESSION_CREDENTIAL_CONFLICT:

			// Prompt for password if the default(NULL) was not good
			if ( lpszUser != NULL && lpszPassword == NULL )
			{
				Out( _T("Invalid password\n\n") );
				SetConnectionCredentials( TRUE );
				Out( _T("Connecting to remote service ... ") );
				//Establish connection (using username/pwd) again
				rc = WNetAddConnection2( &nr, lpszPassword, lpszUser, FALSE );
			}
			break;
		}
	}
	else 
		// Disconnect
		rc = WNetCancelConnection2( szRemoteResource, 0, NULL );

	if ( rc == NO_ERROR ) 
		return TRUE; // indicate success

	SetLastError( rc );

	return FALSE;
}

// Copies the command's exe file to remote machine (\\remote\ADMIN$\System32 or \\remote\ADMIN$\SYSWOW64)
// This function called, if the /c option is used
BOOL CopyBinaryToRemoteSystem(TCHAR *szBinaryDir)
{
	if ( !IsCmdLineParameter(_T("c")) )
		return TRUE;

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	TCHAR szRemoteResource[_MAX_PATH];

	// Gets the file name and extension
	_tsplitpath( lpszCommandExe, drive, dir, fname, ext );
	_stprintf( szRemoteResource, _T("%s\\%s\\%s%s"), lpszMachine, szBinaryDir, fname, ext );

	// Copy the Command's exe file to \\remote\ADMIN$\System32 or \\remote\ADMIN$\SYSWOW64
	// printf("lpszCommandExe=[%s]\n",lpszCommandExe);

	return CopyFile( lpszCommandExe, szRemoteResource, FALSE );
}

// Copies the Local Process Launcher Executable from Self Resource -> Copies to Current Path 
BOOL ExtractLocalBinaryResource()
{
	DWORD dwWritten = 0;

	HMODULE hInstance = ::GetModuleHandle(NULL);

	// Find the binary file in resources
	HRSRC hLocalBinRes = ::FindResource( 
		hInstance, 
		MAKEINTRESOURCE(IDR_ProcComs), 
		_T("ProcComs") );

	HGLOBAL hLocalBinary = ::LoadResource( 
		hInstance, 
		hLocalBinRes );

	LPVOID pLocalBinary = ::LockResource( hLocalBinary );

	if ( pLocalBinary == NULL )
		return FALSE;

	DWORD dwLocalBinarySize = ::SizeofResource(
		hInstance,
		hLocalBinRes );
	GetCurrentDirectory(MAX_PATH , szLocalBinPath);

	_stprintf( szLocalBinPath, _T("%s\\%s"), szLocalBinPath, ProcComs );

	HANDLE hFileLocalBinary = CreateFile( 
		szLocalBinPath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	if ( hFileLocalBinary == INVALID_HANDLE_VALUE )
		return FALSE;

	WriteFile( hFileLocalBinary, pLocalBinary, dwLocalBinarySize, &dwWritten, NULL );
//	Out( _T("File Written ...\n") ); 
//	Sleep(10000);
	CloseHandle( hFileLocalBinary );

	return dwWritten == dwLocalBinarySize;
}

// Extracts the Service Executable from Self Resource -> Pushes to the remote machine
BOOL CopyServiceToRemoteMachine(TCHAR *szBinaryDir)
{
	DWORD dwWritten = 0;

	HMODULE hInstance = ::GetModuleHandle(NULL);

	// Find the binary file in resources
	HRSRC hSvcExecutableRes = ::FindResource( 
		hInstance, 
		MAKEINTRESOURCE(IDR_RemComSVC), 
		_T("RemComSVC") );

	HGLOBAL hSvcExecutable = ::LoadResource( 
		hInstance, 
		hSvcExecutableRes );

	LPVOID pSvcExecutable = ::LockResource( hSvcExecutable );

	if ( pSvcExecutable == NULL )
		return FALSE;

	DWORD dwSvcExecutableSize = ::SizeofResource(
		hInstance,
		hSvcExecutableRes );


	TCHAR szSvcExePath[_MAX_PATH];
	_stprintf( szSvcExePath, _T("%s\\%s\\%s"), lpszMachine, szBinaryDir, RemComSVCEXE );
	

	// Copy binary file from resources to \\remote\ADMIN$\System32 or ADMIN$\SYSWOW64
	HANDLE hFileSvcExecutable = CreateFile( 
		szSvcExePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );

	if ( hFileSvcExecutable == INVALID_HANDLE_VALUE )
		return FALSE;

	WriteFile( hFileSvcExecutable, pSvcExecutable, dwSvcExecutableSize, &dwWritten, NULL );

	CloseHandle( hFileSvcExecutable );

	return dwWritten == dwSvcExecutableSize;
}

// Installs and starts the remote service on remote machine
BOOL InstallAndStartRemoteService(TCHAR *szBinaryDir)
{
	// Open remote Service Manager
	SC_HANDLE hSCM = ::OpenSCManager( lpszMachine, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL) {
		printf("Fail (a)\n");
		return FALSE;
	}

	TCHAR szSvcExePath[_MAX_PATH];
	_stprintf( szSvcExePath, _T("%s\\%s\\%s"), getenv("SystemRoot"), szBinaryDir, RemComSVCEXE );

	// Maybe it's already there and installed, let's try to run
	SC_HANDLE hService =::OpenService( hSCM, SERVICENAME, SERVICE_ALL_ACCESS );

	


	// Creates service on remote machine, if it's not installed yet
	// Note, this is always created in system32 even on 64-bit machines. Windows will
	// check in SYSWOW64 if the executable is 32 bit anyway (which we are)
	if ( hService == NULL ) {
		printf("Creating service %s (%s)\n",SERVICENAME,RemComSVCEXE);
		hService = ::CreateService(
		hSCM, SERVICENAME, LONGSERVICENAME,
		SERVICE_ALL_ACCESS, 
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szSvcExePath,
		NULL, NULL, NULL, NULL, NULL );
	}
	if (hService == NULL)
	{
		printf("Fail (b)\n");
		::CloseServiceHandle(hSCM);
		return FALSE;
	}

	// Start service
	if ( !StartService( hService, 0, NULL ) )
	{
		printf("Fail (c) %s\n",SERVICENAME);
		return FALSE;
	}
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	return TRUE;
}

// Connects to the remote service
BOOL ConnectToRemoteService( DWORD dwRetry, DWORD dwRetryTimeOut )
{
	TCHAR szPipeName[_MAX_PATH] = _T("");

	// Remote service communication pipe name
	_stprintf( szPipeName, _T("%s\\pipe\\%s"), lpszMachine, RemComCOMM );

	SECURITY_ATTRIBUTES SecAttrib = {0};
	SECURITY_DESCRIPTOR SecDesc;
	InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&SecDesc, TRUE, NULL, TRUE);

	SecAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	SecAttrib.lpSecurityDescriptor = &SecDesc;;
	SecAttrib.bInheritHandle = TRUE;

	// Connects to the remote service's communication pipe
	while( dwRetry-- )
	{
		if ( WaitNamedPipe( szPipeName, 5000 ) )
		{
			hCommandPipe = CreateFile( 
				szPipeName,
				GENERIC_WRITE | GENERIC_READ, 
				0,
				&SecAttrib, 
				OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL );

			break;
		}
		else
			// Try Again
			Sleep( dwRetryTimeOut );
	}

	return hCommandPipe != INVALID_HANDLE_VALUE;
}

// Fill the communication message structure
// This structure will be transferred to remote machine
BOOL BuildMessageStructure( RemComMessage* pMsg )
{
	LPCTSTR lpszWorkingDir = GetParamValue( _T("d:") );

	// Info
	pMsg->dwProcessId = GetCurrentProcessId();
	_tcscpy( pMsg->szMachine, szThisMachine );

	// printf("lpszCommandExe = [%s]\n",lpszCommandExe);
	// printf("szArguments    = [%s]\n",szArguments);
	const char *s = lpszCommandExe;
	const char *p=(char *)0;
	while (*s)
	{
		if (*s=='.') p=s;
		s++;
	}
	
	if (p)
	{
		pMsg->bUseCScript = (stricmp(p,".vbs")==0 || stricmp(p,".js")==0);
		pMsg->bCmdBuiltIn = false;
	}
	else
	{
		// No extension - must be a built-in command (ls, mkdir etc)
		pMsg->bUseCScript = false;
		pMsg->bCmdBuiltIn = true;
	}

	// Cmd
	if ( !IsCmdLineParameter(_T("c")) )
		// _stprintf( pMsg->szCommand, _T("\"%s\" %s"), lpszCommandExe, szArguments );
		_stprintf( pMsg->szCommand, _T("%s"), szArguments );
	else
	{
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];

		_tsplitpath( lpszCommandExe, drive, dir, fname, ext );

		// _stprintf( pMsg->szCommand, _T("\"%s%s\" %s"), fname, ext, szArguments );
		_stprintf( pMsg->szCommand, _T("%s"), szArguments );
	}

	// printf("(1) pMsg->szCommand=[%s]\n",pMsg->szCommand);

	// Priority
	if ( IsCmdLineParameter( _T("realtime") ) )
		pMsg->dwPriority = REALTIME_PRIORITY_CLASS;
	else
		if ( IsCmdLineParameter( _T("high") ) )
			pMsg->dwPriority = HIGH_PRIORITY_CLASS; 
		else
			if ( IsCmdLineParameter( _T("idle") ) )
				pMsg->dwPriority = IDLE_PRIORITY_CLASS; 
			else
				pMsg->dwPriority = NORMAL_PRIORITY_CLASS; // default

	// No wait
	pMsg->bNoWait = IsCmdLineParameter( _T("nowait") );

	if ( lpszWorkingDir != NULL )
		_tcscpy( pMsg->szWorkingDir, lpszWorkingDir );

	// Console Title
	// _stprintf( szConsoleTitle, _T("%s : %s"), lpszMachine, pMsg->szCommand );
	// SetConsoleTitle( szConsoleTitle );

	return TRUE;
}

// Listens the remote stdout pipe
// Remote process will send its stdout to this pipe
void ListenRemoteOutPipeThread(void*)
{
	HANDLE hOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	TCHAR szBuffer[SIZEOF_BUFFER];
	DWORD dwRead;

	for(;;)	
	{ 
		if ( !ReadFile( hRemoteOutPipe, szBuffer, SIZEOF_BUFFER, &dwRead, NULL ) || 
			dwRead == 0 ) 
		{
			DWORD dwErr = GetLastError();
			if ( dwErr == ERROR_NO_DATA)
				break;
		}

		// Handle CLS command, just for fun :)
		switch( szBuffer[0] )
		{
		case 12: //cls
			{
				DWORD dwWritten;
				COORD origin = {0,0};
				CONSOLE_SCREEN_BUFFER_INFO sbi;

				if ( GetConsoleScreenBufferInfo( hOutput, &sbi ) )
				{
					FillConsoleOutputCharacter( 
						hOutput,
						_T(' '),
						sbi.dwSize.X * sbi.dwSize.Y,
						origin,
						&dwWritten );

					SetConsoleCursorPosition(
						hOutput,
						origin );
				}
			}
			continue;
			break;
		}

		szBuffer[ dwRead / sizeof(TCHAR) ] = _T('\0');

		// Send it to our stdout
		Out( szBuffer );
	} 

	CloseHandle( hRemoteOutPipe );

	hRemoteOutPipe = INVALID_HANDLE_VALUE;

	::ExitThread(0);
}

// Listens the remote stderr pipe
// Remote process will send its stderr to this pipe
void ListenRemoteErrorPipeThread(void*)
{
	TCHAR szBuffer[SIZEOF_BUFFER];
	DWORD dwRead;

	for(;;)	
	{ 
		if ( !ReadFile( hRemoteErrorPipe, szBuffer, SIZEOF_BUFFER, &dwRead, NULL ) || 
			dwRead == 0 ) 
		{
			DWORD dwErr = GetLastError();
			if ( dwErr == ERROR_NO_DATA)
				break;
		}

		szBuffer[ dwRead / sizeof(TCHAR) ] = _T('\0');

		// Write it to our stderr
		Error( szBuffer );
	} 

	CloseHandle( hRemoteErrorPipe );

	hRemoteErrorPipe = INVALID_HANDLE_VALUE;

	::ExitThread(0);
}

// Listens our console, and if the user types in something,
// we will pass it to the remote machine.
// ReadConsole return after pressing the ENTER
void ListenRemoteStdInputPipeThread(void*)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	TCHAR szInputBuffer[SIZEOF_BUFFER] = _T("");
	DWORD nBytesRead;
	DWORD nBytesWrote;

	for(;;)
	{
		// Read our console input
		if ( !ReadConsole( hInput, szInputBuffer, SIZEOF_BUFFER, &nBytesRead, NULL ) )
		{
			DWORD dwErr = GetLastError();
			if ( dwErr == ERROR_NO_DATA)
				break;
		}

		// Send it to remote process' stdin
		if ( !WriteFile( hRemoteStdInputPipe, szInputBuffer, nBytesRead, &nBytesWrote, NULL ) )
			break;
	} 

	CloseHandle( hRemoteStdInputPipe );

	hRemoteStdInputPipe = INVALID_HANDLE_VALUE;

	::ExitThread(0);
}

// Start listening stdout, stderr and stdin
void ListenToRemoteNamedPipes()
{
	// StdOut
	_beginthread( ListenRemoteOutPipeThread, 0, NULL );

	// StdErr
	_beginthread( ListenRemoteErrorPipeThread, 0, NULL );

	// StdIn
	_beginthread( ListenRemoteStdInputPipeThread, 0, NULL );
}

// Connects to the remote processes stdout, stderr and stdin named pipes
BOOL ConnectToRemotePipes( DWORD dwRetryCount, DWORD dwRetryTimeOut )
{
	TCHAR szStdOut[_MAX_PATH];
	TCHAR szStdIn[_MAX_PATH];
	TCHAR szStdErr[_MAX_PATH];

	
	SECURITY_ATTRIBUTES SecAttrib = {0};
	SECURITY_DESCRIPTOR SecDesc;
	
	InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&SecDesc, TRUE, NULL, FALSE);

	SecAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	SecAttrib.lpSecurityDescriptor = &SecDesc;;
	SecAttrib.bInheritHandle = TRUE;

	hRemoteOutPipe = INVALID_HANDLE_VALUE;
	hRemoteStdInputPipe = INVALID_HANDLE_VALUE;
	hRemoteErrorPipe = INVALID_HANDLE_VALUE;

	// StdOut pipe name
	_stprintf( szStdOut, _T("%s\\pipe\\%s%s%d"), 
		lpszMachine, 
		RemComSTDOUT, 
		szThisMachine, 
		GetCurrentProcessId() );

	// StdErr pipe name
	_stprintf( szStdIn, _T("%s\\pipe\\%s%s%d"), 
		lpszMachine, 
		RemComSTDIN, 
		szThisMachine, 
		GetCurrentProcessId() );

	// StdIn pipe name
	_stprintf( szStdErr, _T("%s\\pipe\\%s%s%d"), 
		lpszMachine, 
		RemComSTDERR, 
		szThisMachine, 
		GetCurrentProcessId() );

	while( dwRetryCount-- )
	{
		// Connects to StdOut pipe
		if ( hRemoteOutPipe == INVALID_HANDLE_VALUE )
			if ( WaitNamedPipe( szStdOut, NULL ) )
				hRemoteOutPipe = CreateFile( 
				szStdOut,
				GENERIC_READ, 
				0,
				&SecAttrib, 
				OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL );

		// Connects to Error pipe
		if ( hRemoteErrorPipe == INVALID_HANDLE_VALUE )
			if ( WaitNamedPipe( szStdErr, NULL ) )
				hRemoteErrorPipe = CreateFile( 
				szStdErr,
				GENERIC_READ, 
				0,
				&SecAttrib, 
				OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL );

		// Connects to StdIn pipe
		if ( hRemoteStdInputPipe == INVALID_HANDLE_VALUE )
			if ( WaitNamedPipe( szStdIn, NULL ) )
				hRemoteStdInputPipe = CreateFile( 
				szStdIn,
				GENERIC_WRITE, 
				0,
				&SecAttrib, 
				OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL );

		if (  hRemoteOutPipe != INVALID_HANDLE_VALUE &&
			hRemoteErrorPipe != INVALID_HANDLE_VALUE &&
			hRemoteStdInputPipe != INVALID_HANDLE_VALUE )
			break;

		// One of the pipes failed, try it again
		Sleep( dwRetryTimeOut );
	}

	if (  hRemoteOutPipe == INVALID_HANDLE_VALUE ||
		hRemoteErrorPipe == INVALID_HANDLE_VALUE ||
		hRemoteStdInputPipe == INVALID_HANDLE_VALUE )
	{
		CloseHandle( hRemoteOutPipe );
		CloseHandle( hRemoteErrorPipe );
		CloseHandle( hRemoteStdInputPipe );

		return FALSE;
	}

	// Start listening these pipes
	ListenToRemoteNamedPipes();

	return TRUE;
}

// 1. Send the message to remote service
// 2. Connects to remote pipes
// 3. Waiting for finishing remote process
int ExecuteRemoteCommand()
{
	DWORD dwTemp = 0;
	RemComMessage msg;
	RemComResponse response;

	::ZeroMemory( &msg, sizeof(msg) );
	::ZeroMemory( &response, sizeof(response) );

	BuildMessageStructure( &msg );

	// Send message to service
	WriteFile( hCommandPipe, &msg, sizeof(msg), &dwTemp, NULL );

	// Connects to remote pipes (stdout, stdin, stderr)
	if ( ConnectToRemotePipes( 5, 1000 ) )
	{
		// Waiting for response from service
		ReadFile( hCommandPipe, &response, sizeof(response), &dwTemp, NULL );
	}
	else
	{
		Out( _T("1) Failed to connect to remote pipes\n\n") );
	}
	if ( response.dwErrorCode == 0 )
	{
		return response.dwReturnCode;
	}
	else
	{
		return response.dwErrorCode;
	}
}

BOOL WINAPI ConsoleCtrlHandler( DWORD dwCtrlType )
{
	switch( dwCtrlType )
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
		break;
	}

	return FALSE;
}

void ShowUsage()
{
 Out( _T("------------------------------------------------------------------\n") );    
 Out( _T("| Usage: RemCom.exe [\\\\computer] [options] [cmd/exe arguments] |\n") );
 Out( _T("------------------------------------------------------------------\n") );
 Out( _T("\n") );
 Out( _T("Options:\n") );
 Out( _T("\n") );
 Out( _T(" /user:UserName\t\tUserName for Remote Connection\n") );
 Out( _T(" /pwd:[password|*]\tPassword. * will delay the input (if required)\n") );
 Out( _T("\n") );
 Out( _T(" /d:directory\t\tSet working directory\n") );
 Out( _T("\t\t\t(Default: \\\\RemoteSystem\"%SystemRoot%\\System32\")\n\n") );
 Out( _T(" [/idle | /normal | /high | /realtime]\tPriority class (use only one)\n") );
 Out( _T("  /nowait\t\tDon't wait for remote process to terminate\n") );
 Out( _T("\n") );
 Out( _T(" /c\t\t\tCopy the specified program to the remote machine's\n") );
 Out( _T("   \t\t\t\"%SystemRoot%\\System32\" directory\n") );
 Out( _T("   \t\t\tCommand's exe file must be absolute to local machine\n") );
 Out( _T("\n") );
 Out( _T("   .........................................................................\n") );
 Out( _T("\n") );
 Out( _T("Examples:\n") );
 Out( _T("\n") );
 Out( _T(" RemCom.exe \\\\remote cmd\t[Starts a \"telnet\" client]\n") );
 Out( _T(" RemCom.exe \\\\remote /user:Username /pwd:Password cmd.exe\t[Starts a \"telnet\" client]\n") );
 Out( _T(" RemCom.exe \\\\localhost /user:Username /pwd:Password  \"C:\\InstallMe.bat\"\t[A replacement for RunAs Command]\"\n") );
 Out( _T("\n") );
 Out( _T("   .........................................................................\n") );
 Out( _T("\n") );
 Out( _T("Notes:\n") );
 Out( _T("\n") );
 Out( _T("-  A \".\" for Machine Name will be treated as localhost\n") ); 
 Out( _T("-  Input is passed to remote machine when you press the ENTER.\n") ); 
 Out( _T("-  Ctrl-C terminates the remote process\n") );
 Out( _T("-  Command and file path arguments have to be absolute to remote machine\n") );
 Out( _T("-  If you are using /c option, command exe file path must be absolute to\n") );
 Out( _T("   local machine, but the arguments must be absolute to remote machine\n") );
 Out( _T("-  A dot . for machine name is taken as localhost\n") );
 Out( _T("-  Not providing any credentials, the Process will (impersonate and) run\n") );  
 Out( _T("   in the context of your account on the remote system, but will not have\n") );  
 Out( _T("   access to network resources \n") );  
 Out( _T("-  Specify a valid user name in the Domain\\User syntax if the remote process\n") );  
 Out( _T("   requires access to network resources or to run in a different account. \n") );  
 Out( _T("-  The password is transmitted in clear text to the remote system.\n") );  
 Out( _T("-  You can enclose applications that have spaces in their name with \n") );  
 Out( _T("   quotation marks  e.g. RemCom \\\\computername \"c:\\long name app.exe\".\\n") );  
 Out( _T("-  Input is only passed to the remote system when you press the enter key \n") );  
 Out( _T("-  Typing Ctrl-C terminates the remote process.            \n") );  
 Out( _T("-  Error codes returned by RemCom are specific to the applications you execute, not RemCom.\n") );  
 Out( _T(" \n") );  
 Out( _T("   .........................................................................\n") );

}


BOOL StartInteractiveClientProcess (
									LPTSTR lpszUsername,    // client to log on
									LPTSTR lpszDomain,      // domain of client's account
									LPTSTR lpszPassword,    // client's password
									LPTSTR lpCommandLine    // command line to execute
									) 
{
	HANDLE      hToken;
	HDESK       hdesk = NULL;
	HWINSTA     hwinsta = NULL, hwinstaSave = NULL;
	PROCESS_INFORMATION pi;
	PSID pSid = NULL;
	STARTUPINFO si;
	BOOL bResult = FALSE;

	// Log the client on to the local computer.

	if (!LogonUser(
		lpszUsername,
		lpszDomain,
		lpszPassword,
		LOGON32_LOGON_INTERACTIVE,
		LOGON32_PROVIDER_DEFAULT,
		&hToken) ) 
	{
		goto Cleanup;
	}

	// Save a handle to the caller's current window station.

	if ( (hwinstaSave = GetProcessWindowStation() ) == NULL)
		goto Cleanup;

	// Get a handle to the interactive window station.

	hwinsta = OpenWindowStation(
		"winsta0",                   // the interactive window station 
		FALSE,                       // handle is not inheritable
		READ_CONTROL | WRITE_DAC);   // rights to read/write the DACL

	if (hwinsta == NULL) 
		goto Cleanup;

	// To get the correct default desktop, set the caller's 
	// window station to the interactive window station.

	if (!SetProcessWindowStation(hwinsta))
		goto Cleanup;

	// Get a handle to the interactive desktop.

	hdesk = OpenDesktop(
		"default",     // the interactive window station 
		0,             // no interaction with other desktop processes
		FALSE,         // handle is not inheritable
		READ_CONTROL | // request the rights to read and write the DACL
		WRITE_DAC | 
		DESKTOP_WRITEOBJECTS | 
		DESKTOP_READOBJECTS);

	// Restore the caller's window station.

	if (!SetProcessWindowStation(hwinstaSave)) 
		goto Cleanup;

	if (hdesk == NULL) 
		goto Cleanup;

	// Get the SID for the client's logon session.

	if (!GetLogonSID(hToken, &pSid)) 
		goto Cleanup;

	// Allow logon SID full access to interactive window station.

	//TODO
	//   if (! AddAceToWindowStation(hwinsta, pSid) ) 
	//    goto Cleanup;

	// Allow logon SID full access to interactive desktop.

	if (! AddAceToDesktop(hdesk, pSid) ) 
		goto Cleanup;

	// Impersonate client to ensure access to executable file.

	if (! ImpersonateLoggedOnUser(hToken) ) 
		goto Cleanup;

	// Initialize the STARTUPINFO structure.
	// Specify that the process runs in the interactive desktop.

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb= sizeof(STARTUPINFO);

	//   si.lpDesktop = TEXT("winsta0\\default");
	//   si.wShowWindow
	si.hStdOutput = hRemoteOutPipe;
	si.hStdError = hRemoteOutPipe;

	// Launch the process in the client's logon session.

	bResult = CreateProcessAsUser(
		hToken,            // client's access token
		NULL,              // file to execute
		lpCommandLine,     // command line
		NULL,              // pointer to process SECURITY_ATTRIBUTES
		NULL,              // pointer to thread SECURITY_ATTRIBUTES
		FALSE,             // handles are not inheritable
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,   // creation flags
		NULL,              // pointer to new environment block 
		NULL,              // name of current directory 
		&si,               // pointer to STARTUPINFO structure
		&pi                // receives information about new process
		);



	// End impersonation of client.
	ShowLastError();   
	RevertToSelf();

	if (bResult && pi.hProcess != INVALID_HANDLE_VALUE) 
	{ 
		WaitForSingleObject(pi.hProcess, INFINITE); 
		CloseHandle(pi.hProcess); 
		//ShowLastError();
	} 

	if (pi.hThread != INVALID_HANDLE_VALUE)
		CloseHandle(pi.hThread);  

Cleanup: 

	if (hwinstaSave != NULL)
		SetProcessWindowStation (hwinstaSave);

	// Free the buffer for the logon SID.

	if (pSid)
		FreeLogonSID(&pSid);

	// Close the handles to the interactive window station and desktop.

	if (hwinsta)
		CloseWindowStation(hwinsta);

	if (hdesk)
		CloseDesktop(hdesk);

	// Close the handle to the client's access token.

	if (hToken != INVALID_HANDLE_VALUE)
		CloseHandle(hToken);  

	return bResult;
}

BOOL StartProcessWithUserLogon()
{   
	HANDLE    hToken;
	//  LPVOID    lpvEnv;
	PROCESS_INFORMATION pi = {0};
	STARTUPINFOW         si = {0};
//	TCHAR szUserProfile[SIZEOF_BUFFER] = _T("");

	// Initialize the STARTUPINFO structure.
	// Specify that the process runs in the interactive desktop.

	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb= sizeof(STARTUPINFOW);

	si.hStdOutput = hRemoteOutPipe;
	si.hStdError= hRemoteOutPipe;
	//	CreateEnvironmentBlock(&lpvEnv, hToken, TRUE);

	if (!LogonUser(lpszUser, NULL, lpszPassword, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken))
	{ 
		ShowLastError();
		return false;
	}

	//(LPCWSTR)
	int bResult;

	/*	bResult =  CreateProcessWithLogonW(		(LPCWSTR)lpszUser, 		(LPCWSTR)lpszDomain,		(LPCWSTR)lpszPassword,  		LOGON_WITH_PROFILE, 
	NULL,		(LPWSTR) lpszCommandExe, 		CREATE_UNICODE_ENVIRONMENT, NULL, 		NULL,  
	&si, 
	&pi
	);
	*/

	bResult =  CreateProcessWithLogonW(	
		(LPCWSTR)lpszUser, 		
		NULL,		
		(LPCWSTR)lpszPassword,  		
		LOGON_WITH_PROFILE, 
		NULL,		
		(LPWSTR) lpszCommandExe, 		
		CREATE_UNICODE_ENVIRONMENT, 		/*lpvEnv*/ NULL, 	(LPCWSTR) "%SYSTEMROOT%\\system32",  
		&si, 
		&pi
		);

	ShowLastError();

	//    if (!DestroyEnvironmentBlock(lpvEnv))        DisplayError(L"DestroyEnvironmentBlock");

	CloseHandle(hToken);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


	return bResult;
}


BOOL AddAceToWindowStation(HWINSTA hwinsta, PSID psid)
{
	ACCESS_ALLOWED_ACE   *pace;
	ACL_SIZE_INFORMATION aclSizeInfo;
	BOOL                 bDaclExist;
	BOOL                 bDaclPresent;
	BOOL                 bSuccess = FALSE;
	DWORD                dwNewAclSize;
	DWORD                dwSidSize = 0;
	DWORD                dwSdSizeNeeded;
	PACL                 pacl = 0;
	PACL                 pNewAcl = 0;
	PSECURITY_DESCRIPTOR psd = NULL;
	PSECURITY_DESCRIPTOR psdNew = NULL;
	PVOID                pTempAce;
	SECURITY_INFORMATION si = DACL_SECURITY_INFORMATION;
	unsigned int         i;

	__try
	{
		// Obtain the DACL for the window station.

		if (!GetUserObjectSecurity(
			hwinsta,
			&si,
			psd,
			dwSidSize,
			&dwSdSizeNeeded)
			)
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				psd = (PSECURITY_DESCRIPTOR)HeapAlloc(
					GetProcessHeap(),
					HEAP_ZERO_MEMORY,
					dwSdSizeNeeded);

				if (psd == NULL)
					__leave;

				psdNew = (PSECURITY_DESCRIPTOR)HeapAlloc(
					GetProcessHeap(),
					HEAP_ZERO_MEMORY,
					dwSdSizeNeeded);

				if (psdNew == NULL)
					__leave;

				dwSidSize = dwSdSizeNeeded;

				if (!GetUserObjectSecurity(
					hwinsta,
					&si,
					psd,
					dwSidSize,
					&dwSdSizeNeeded)
					)
					__leave;
			}
			else
				__leave;

		// Create a new DACL.

		if (!InitializeSecurityDescriptor(
			psdNew,
			SECURITY_DESCRIPTOR_REVISION)
			)
			__leave;

		// Get the DACL from the security descriptor.

		if (!GetSecurityDescriptorDacl(
			psd,
			&bDaclPresent,
			&pacl,
			&bDaclExist)
			)
			__leave;

		// Initialize the ACL.

		ZeroMemory(&aclSizeInfo, sizeof(ACL_SIZE_INFORMATION));
		aclSizeInfo.AclBytesInUse = sizeof(ACL);

		// Call only if the DACL is not NULL.

		if (pacl != NULL)
		{
			// get the file ACL size info
			if (!GetAclInformation(
				pacl,
				(LPVOID)&aclSizeInfo,
				sizeof(ACL_SIZE_INFORMATION),
				AclSizeInformation)
				)
				__leave;
		}

		// Compute the size of the new ACL.

		dwNewAclSize = aclSizeInfo.AclBytesInUse + (2*sizeof(ACCESS_ALLOWED_ACE)) + 
			(2*GetLengthSid(psid)) - (2*sizeof(DWORD));

		// Allocate memory for the new ACL.

		pNewAcl = (PACL)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			dwNewAclSize);

		if (pNewAcl == NULL)
			__leave;

		// Initialize the new DACL.

		if (!InitializeAcl(pNewAcl, dwNewAclSize, ACL_REVISION))
			__leave;

		// If DACL is present, copy it to a new DACL.

		if (bDaclPresent)
		{
			// Copy the ACEs to the new ACL.
			if (aclSizeInfo.AceCount)
			{
				for (i=0; i < aclSizeInfo.AceCount; i++)
				{
					// Get an ACE.
					if (!GetAce(pacl, i, &pTempAce))
						__leave;

					// Add the ACE to the new ACL.
					if (!AddAce(
						pNewAcl,
						ACL_REVISION,
						MAXDWORD,
						pTempAce,
						((PACE_HEADER)pTempAce)->AceSize)
						)
						__leave;
				}
			}
		}

		// Add the first ACE to the window station.

		pace = (ACCESS_ALLOWED_ACE *)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(psid) -
			sizeof(DWORD));

		if (pace == NULL)
			__leave;

		pace->Header.AceType  = ACCESS_ALLOWED_ACE_TYPE;
		pace->Header.AceFlags = CONTAINER_INHERIT_ACE |
			INHERIT_ONLY_ACE | OBJECT_INHERIT_ACE;
		pace->Header.AceSize  = sizeof(ACCESS_ALLOWED_ACE) +
			GetLengthSid(psid) - sizeof(DWORD);
		pace->Mask            = GENERIC_ACCESS;

		if (!CopySid(GetLengthSid(psid), &pace->SidStart, psid))
			__leave;

		if (!AddAce(
			pNewAcl,
			ACL_REVISION,
			MAXDWORD,
			(LPVOID)pace,
			pace->Header.AceSize)
			)
			__leave;

		// Add the second ACE to the window station.

		pace->Header.AceFlags = NO_PROPAGATE_INHERIT_ACE;
		pace->Mask            = WINSTA_ALL;

		if (!AddAce(
			pNewAcl,
			ACL_REVISION,
			MAXDWORD,
			(LPVOID)pace,
			pace->Header.AceSize)
			)
			__leave;

		// Set a new DACL for the security descriptor.

		if (!SetSecurityDescriptorDacl(
			psdNew,
			TRUE,
			pNewAcl,
			FALSE)
			)
			__leave;

		// Set the new security descriptor for the window station.

		if (!SetUserObjectSecurity(hwinsta, &si, psdNew))
			__leave;

		// Indicate success.

		bSuccess = TRUE;
	}
	__finally
	{
		// Free the allocated buffers.

		if (pace != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)pace);

		if (pNewAcl != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)pNewAcl);

		if (psd != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)psd);

		if (psdNew != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)psdNew);
	}

	return bSuccess;

}

BOOL AddAceToDesktop(HDESK hdesk, PSID psid)
{
	ACL_SIZE_INFORMATION aclSizeInfo;
	BOOL                 bDaclExist;
	BOOL                 bDaclPresent;
	BOOL                 bSuccess = FALSE;
	DWORD                dwNewAclSize;
	DWORD                dwSidSize = 0;
	DWORD                dwSdSizeNeeded;
	PACL                 pacl;
	PACL                 pNewAcl;
	PSECURITY_DESCRIPTOR psd = NULL;
	PSECURITY_DESCRIPTOR psdNew = NULL;
	PVOID                pTempAce;
	SECURITY_INFORMATION si = DACL_SECURITY_INFORMATION;
	unsigned int         i;

	__try
	{
		// Obtain the security descriptor for the desktop object.

		if (!GetUserObjectSecurity(
			hdesk,
			&si,
			psd,
			dwSidSize,
			&dwSdSizeNeeded))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				psd = (PSECURITY_DESCRIPTOR)HeapAlloc(
					GetProcessHeap(),
					HEAP_ZERO_MEMORY,
					dwSdSizeNeeded );

				if (psd == NULL)
					__leave;

				psdNew = (PSECURITY_DESCRIPTOR)HeapAlloc(
					GetProcessHeap(),
					HEAP_ZERO_MEMORY,
					dwSdSizeNeeded);

				if (psdNew == NULL)
					__leave;

				dwSidSize = dwSdSizeNeeded;

				if (!GetUserObjectSecurity(
					hdesk,
					&si,
					psd,
					dwSidSize,
					&dwSdSizeNeeded)
					)
					__leave;
			}
			else
				__leave;
		}

		// Create a new security descriptor.

		if (!InitializeSecurityDescriptor(
			psdNew,
			SECURITY_DESCRIPTOR_REVISION)
			)
			__leave;

		// Obtain the DACL from the security descriptor.

		if (!GetSecurityDescriptorDacl(
			psd,
			&bDaclPresent,
			&pacl,
			&bDaclExist)
			)
			__leave;

		// Initialize.

		ZeroMemory(&aclSizeInfo, sizeof(ACL_SIZE_INFORMATION));
		aclSizeInfo.AclBytesInUse = sizeof(ACL);

		// Call only if NULL DACL.

		if (pacl != NULL)
		{
			// Determine the size of the ACL information.

			if (!GetAclInformation(
				pacl,
				(LPVOID)&aclSizeInfo,
				sizeof(ACL_SIZE_INFORMATION),
				AclSizeInformation)
				)
				__leave;
		}

		// Compute the size of the new ACL.

		dwNewAclSize = aclSizeInfo.AclBytesInUse +
			sizeof(ACCESS_ALLOWED_ACE) +
			GetLengthSid(psid) - sizeof(DWORD);

		// Allocate buffer for the new ACL.

		pNewAcl = (PACL)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			dwNewAclSize);

		if (pNewAcl == NULL)
			__leave;

		// Initialize the new ACL.

		if (!InitializeAcl(pNewAcl, dwNewAclSize, ACL_REVISION))
			__leave;

		// If DACL is present, copy it to a new DACL.

		if (bDaclPresent)
		{
			// Copy the ACEs to the new ACL.
			if (aclSizeInfo.AceCount)
			{
				for (i=0; i < aclSizeInfo.AceCount; i++)
				{
					// Get an ACE.
					if (!GetAce(pacl, i, &pTempAce))
						__leave;

					// Add the ACE to the new ACL.
					if (!AddAce(
						pNewAcl,
						ACL_REVISION,
						MAXDWORD,
						pTempAce,
						((PACE_HEADER)pTempAce)->AceSize)
						)
						__leave;
				}
			}
		}

		// Add ACE to the DACL.

		if (!AddAccessAllowedAce(
			pNewAcl,
			ACL_REVISION,
			DESKTOP_ALL,
			psid)
			)
			__leave;

		// Set new DACL to the new security descriptor.

		if (!SetSecurityDescriptorDacl(
			psdNew,
			TRUE,
			pNewAcl,
			FALSE)
			)
			__leave;

		// Set the new security descriptor for the desktop object.

		if (!SetUserObjectSecurity(hdesk, &si, psdNew))
			__leave;

		// Indicate success.

		bSuccess = TRUE;
	}
	__finally
	{
		// Free buffers.

		if (pNewAcl != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)pNewAcl);

		if (psd != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)psd);

		if (psdNew != NULL)
			HeapFree(GetProcessHeap(), 0, (LPVOID)psdNew);
	}

	return bSuccess;
}

// xecutes a local process under the local user account who launched the process
BOOL StartLocalProcess(LPTSTR szCommandName){

	SECURITY_ATTRIBUTES secAttrib; 
	ZeroMemory(&secAttrib,sizeof(secAttrib));
	secAttrib.nLength = sizeof(secAttrib);
	secAttrib.bInheritHandle = TRUE;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	si.hStdInput = hRemoteStdInputPipe;
	si.hStdOutput = hRemoteOutPipe;
	si.hStdError = hRemoteOutPipe;

	// Start the child process. 
	if( !CreateProcess( NULL,   // No module name (use command line)
		szCommandName,      // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		ShowLastError();
		return false;
	}
	else{
	//	printf("CreateProcess Success (%d).\n", GetLastError());
	}
	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );
	
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	
	return true;
}

BOOL StartLocalProcessAsUser(){

	HANDLE	hToken;
	PSID pSid = NULL;

	SECURITY_ATTRIBUTES secAttrib; 
	ZeroMemory(&secAttrib,sizeof(secAttrib));
	secAttrib.nLength = sizeof(secAttrib);
	secAttrib.bInheritHandle = TRUE;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	LPTSTR szCmdline= (LPTSTR)lpszCommandExe;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	si.hStdOutput = hRemoteOutPipe;
	si.hStdError = hRemoteOutPipe;

	// Log the client on to the local computer.

	if (!LogonUser((LPTSTR)lpszUser, (LPTSTR)lpszDomain,  (LPTSTR)lpszPassword, LOGON32_LOGON_INTERACTIVE,  LOGON32_PROVIDER_DEFAULT,    &hToken) ) 
	{
		printf( "User Logon failed (%d).\n", GetLastError() );
		//goto Cleanup;
		ShowLastError();
		return false;
	}

	if (!GetLogonSID(hToken, &pSid)) {
		ShowLastError();
		return false;
	}

	// Impersonate client to ensure access to executable file.

	if (! ImpersonateLoggedOnUser(hToken) ) {
		ShowLastError();
		return false;
	}


	// Start the child process. 
	if( !CreateProcessAsUser(
		hToken, //Security Token
		NULL,   // No module name (use command line)
		szCmdline,      // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		printf( "CreateProcess failed (%d).\n", GetLastError() );
		ShowLastError();
		printf("User %s Password %s Command %s  ",  lpszUser, lpszPassword, szCmdline );
		return false;
	}else{

		printf("User Impersonation Success");
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );
	// Close process and thread handles. 

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return true;
}

// Main function
int _tmain( DWORD n, TCHAR** argv, TCHAR** )
{
	int   rc = 0;
	DWORD dwTemp = SIZEOF_BUFFER;
	DWORD dwIndex = 0;

	lpszMachine = GetRemoteMachineName();
	// Tidy all the parameters due to odd Micro$oft quoting issues with command lines
	// a) Remove any trailing " characters
	// b) Remove any single trailing space off the last parameter
	for(int  i = 1; i < __argc; i++ )
	{
		char *t = __argv[i];
		int n2 = strlen(t);
		if (t[n2-1]=='"')
		{
			t[n2-1]='\0';
		}
	}
	char *t3 = __argv[__argc-1];
	int n3 = strlen(t3);
	if (t3[n3-1]==' ')
	{
		t3[n3-1]='\0';
	}
	//for(  i = 1; i < __argc; i++ )
	//{
	//	printf("__argv[%d]=[%s]\n",i,__argv[i]);
	//}
	lpszCommandExe = GetNthParameter( 2, dwIndex );
	// rough and ready - we're getting closing quotes at the end of the command
	// chomp them

	int n2 = strlen(lpszCommandExe);
	if (lpszCommandExe[n2-1]=='"')
	{
		// printf("Trailing quote detected...\n");
		char *t = strdup(lpszCommandExe);
		t[n2-1]='\0';
		lpszCommandExe = t;
	}

	// printf("lpszCommandExe=[%s]\n",lpszCommandExe);

	GetRemoteCommandArguments( szArguments );

	// printf("szArguments=[%s]\n",szArguments);

	// Show help, if parameters are incorrect, or /?,/h,/help
	if (  IsCmdLineParameter( _T("h") ) || 
		IsCmdLineParameter( _T("?") ) || 
		IsCmdLineParameter( _T("help") ) ||
		lpszCommandExe == NULL || 
		lpszMachine == NULL )
	{
		ShowUsage();
		return -1;
	}


	// Initialize console's title
	// _stprintf( szConsoleTitle, _T("%s : Starting Connection to: "), lpszMachine );
	// SetConsoleTitle( szConsoleTitle );

	// Sets our Ctrl handler
	SetConsoleCtrlHandler( ConsoleCtrlHandler, TRUE );

	// Gets our computer's name
	if ( !GetComputerName( szThisMachine, &dwTemp ) )
	{
		Out( _T("GetComputerName() failed. Use a valid name! :)\n") );
		return -3;
	}

	// Check the user/pwd from command line, and prompts
	// for the password if needed
	if ( !SetConnectionCredentials( FALSE ) )
	{
		rc = -2;
		goto cleanup;
	}
			
	// If the Remote Machine is localhost or 127.0.0.1 or '.' Do not connect to the ADMIN$ or IPC$
	// Instead try to use the credentials provided to RUNAS command
	if( ( _tcsnicmp(lpszMachine, lpszLocalMachine, 16) == 0) || (_tcsnicmp(lpszMachine, lpszLocalIP, 16) == 0) || (_tcsnicmp(lpszMachine, ".", 2) == 0) )
	{
		if(IsLaunchedFromAdmin()){
			Out( _T("Local Admin\n\n") );   
		}
		Out( _T("Localhost entered for Target Machine .. Going to RunAs Command\n\n") );   
		lpszMachine = "\\\\127.0.0.1";

		if(ExtractLocalBinaryResource()){
			Out( _T("Launching Local Process ...\n") ); 
			TCHAR szExeCmdAsUser[MAX_PATH] = _T("");
			_stprintf( szExeCmdAsUser, _T("%s %s %s %s"), szLocalBinPath, lpszUser, lpszPassword, lpszCommandExe );
		
			/*
			printf("lpszUser is %s \n",lpszUser);
			printf("lpszPassword is %s \n",lpszPassword);
			printf("lpszCommandExe is %s \n",lpszCommandExe);
			printf("szExeCmdAsUser is %s \n",szExeCmdAsUser);
			*/
			if(!StartLocalProcess(szExeCmdAsUser)){
				Out( _T("Create Local Process Failed. Illegal Command") );   
			}
			DeleteFile(szLocalBinPath);
		}
		else{
			Out( _T("Cannot Extract Local Resources. Please check write access to local tmp filesystem\n") );   
			
			Out( _T("Create Process as User Failed. Trying to run the process with the current user\n\n") );   
			StartLocalProcess((LPTSTR) lpszCommandExe); // Creates a Run As Command	 
		}
		
		//	if(! StartInteractiveClientProcess((LPTSTR)lpszUser, (LPTSTR)lpszDomain, (LPTSTR)lpszPassword, (LPTSTR)lpszCommandExe))
	//		if(!StartProcessWithUserLogon())		{		}
		//	StartLocalProcessAsUser(); 
	
  }
   
   else
   //Remote Machine
   {
	// Connect to remote machine's ADMIN$
	   /*
	   		printf("lpszUser is %s \n",lpszUser);
			printf("lpszPassword is %s \n",lpszPassword);
			printf("lpszCommandExe is %s \n",lpszCommandExe);
		*/
   if ( !EstablishConnection( lpszMachine, _T("ADMIN$"), TRUE ) )
   {
      rc = -2;
      Error( _T("Couldn't connect to ") )
      Error( lpszMachine );
      Error( _T("\\ADMIN$: ") );
      ShowLastError();
      goto cleanup;
   }

   // Connect to remote machine IPC$
   if ( !EstablishConnection( lpszMachine, _T("IPC$"), TRUE ) )
   {
      rc = -2;
      Error( _T("Couldn't connect to ") )
      Error( lpszMachine );
      Error( _T("\\IPC$: ") );
      ShowLastError();
      goto cleanup;
   }

   	TCHAR *szSystemDir;
	TCHAR *szTargetDir;
	TCHAR szSysWow64Path[_MAX_PATH];

	_stprintf( szSysWow64Path,  _T("%s\\ADMIN$\\SYSWOW64"), lpszMachine);
	DWORD wow64 = GetFileAttributes(szSysWow64Path);

	if (wow64 != INVALID_FILE_ATTRIBUTES && (wow64 & FILE_ATTRIBUTE_DIRECTORY))
	{
		szSystemDir = _T("ADMIN$\\SYSWOW64");
		szTargetDir = _T("SYSWOW64");
	}
	else
	{
		szSystemDir = _T("ADMIN$\\System32");
		szTargetDir = _T("System32");
	}

    // _stprintf( szSvcExePath, _T("%s\\ADMIN$\\SYSWOW64\\%s"), lpszMachine, RemComSVCEXE );
   // Copy the command's exe file to remote machine (if using /c)
   if ( !CopyBinaryToRemoteSystem(szSystemDir) )
   {
      rc = -2;
      Error( _T("Couldn't copy ") );
      Error( lpszCommandExe );
      Error( _T(" to ") );
      Error( lpszMachine );
      Error( _T("\\") );
	  Error( szSystemDir );
	  Error( ": ");
      ShowLastError();
      goto cleanup;
   }
   
   // Connects to remote service, maybe it's already running :)
   if ( !ConnectToRemoteService( 1, 0 ) )
   {
      //We couldn't connect, so let's install it and start it

      // Copy the service executable to \\remote\ADMIN$\System32 or \remote\ADMIN$\SYSWOW64
      if ( !CopyServiceToRemoteMachine(szSystemDir) )
      {
         rc = -2;
         Error( _T("Couldn't copy service to ") );
         Error( lpszMachine );
         Error( _T("\\") );
		 Error( szSystemDir );
		 Error( ": ");
         ShowLastError();
         goto cleanup;
      }

      // Install and start service on remote machine
      if ( !InstallAndStartRemoteService(szTargetDir) )
      {
         rc = -2;
         Error( _T("Couldn't start remote service: ") );
         ShowLastError();
         goto cleanup;
      }
      
      // Try to connect again
      if ( !ConnectToRemoteService( 5, 1000 ) )
      {
         rc = -2;
         Error( _T("Couldn't connect to remote service: ") );
         ShowLastError();
         goto cleanup;
      }
   }  

   // Send the message to remote service to start the remote process
   rc = ExecuteRemoteCommand();
 
 }

 cleanup:

   // Disconnect from remote machine
   EstablishConnection( lpszMachine, _T("IPC$"), FALSE );
   EstablishConnection( lpszMachine, _T("ADMIN$"), FALSE );

   return rc;
}

