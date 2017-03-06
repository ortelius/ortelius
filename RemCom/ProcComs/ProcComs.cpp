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
	$Revision:	Talha Tariq [ talha.tariq@gmail.com ] 	
	$Date: 2006/11/03 09:00:00 $ 		
	$Version History: $			- Added OpenProcessToken to get the preocess token.
								- Fixed the bug for Windows 2000 where Calling LogonUser API needs an SE_TCB_NAME privilege which is not enabled by default
	$TODO:						- Code Cleanup. - Move the class as UniCode implementation to the RemCom project
	$Description: $				- ProcCom binary is contained in the parent binary as a local resource which is extracted at runtime from itself
								  for local prorcess and user impersonation
	$Workfile: $				- ProcComs.cpp
 */

#include "stdafx.h"

void ShowLastError(LPWSTR pszAPI)
{
    LPVOID lpvMessageBuffer;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, GetLastError(), 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        (LPWSTR)&lpvMessageBuffer, 0, NULL);

    //
    //... now display this string
    //
    wprintf(L"ERROR: API        = %s.\n", pszAPI);
    wprintf(L"       error code = %d.\n", GetLastError());
    wprintf(L"       message    = %s.\n", (LPWSTR)lpvMessageBuffer);

    //
    // Free the buffer allocated by the system
    //
    LocalFree(lpvMessageBuffer);

    ExitProcess(GetLastError());
}

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    ) 
{
TOKEN_PRIVILEGES tp;
LUID luid;

if ( !LookupPrivilegeValue( 
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid ) )        // receives LUID of privilege
{
	printf("LookupPrivilegeValue error: %u\n", GetLastError());
	return FALSE; 
}

tp.PrivilegeCount = 1;
tp.Privileges[0].Luid = luid;
if (bEnablePrivilege)
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
else
    tp.Privileges[0].Attributes = 0;

// Enable the privilege or disable all privileges.

if ( !AdjustTokenPrivileges(
       hToken, 
       FALSE, 
       &tp, 
       sizeof(TOKEN_PRIVILEGES), 
       (PTOKEN_PRIVILEGES) NULL, 
       (PDWORD) NULL) )
{ 
	  printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
      return FALSE; 
} 

if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

{	  
      printf("The token does not have the specified privilege. \n");
      return FALSE;
} 

return TRUE;
}
void wmain(int argc, WCHAR *argv[])
{
    DWORD     dwSize;
    HANDLE    hToken;
    LPVOID    lpvEnv;
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO         si = {0};
    WCHAR               szUserProfile[256] = L"";

    si.cb = sizeof(STARTUPINFO);
    
    if (argc != 4)
    {
    /*    wprintf(L"Usage: %s [user@domain] [password] [cmd]", argv[0]);
        wprintf(L"\n\n"); */
     //   return;
    }

  dwSize = sizeof(szUserProfile)/sizeof(WCHAR);

  
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken);
	//ShowLastError(L"OpenProcessToken");
    //SetPrivilege(&hToken, SE_SYSTEMTIME_NAME,TRUE);
	// if(!SetPrivilege(hToken, SE_TCB_NAME , TRUE)) printf("Set Privilege Failed");// DisplayError(L"SetPrivilege");
    //if (!LogonUser(argv[1], NULL , argv[2], LOGON32_LOGON_INTERACTIVE,    LOGON32_PROVIDER_DEFAULT, &hToken))       DisplayError(L"LogonUser");

    if (!CreateEnvironmentBlock(&lpvEnv, hToken, TRUE))    ShowLastError(L"CreateEnvironmentBlock");

    dwSize = sizeof(szUserProfile)/sizeof(WCHAR);

    if (!GetUserProfileDirectory(hToken, szUserProfile, &dwSize))        ShowLastError(L"GetUserProfileDirectory");
    //
    // TO DO: change NULL to '.' to use local account database
    //
    if (!CreateProcessWithLogonW(argv[1], NULL, argv[2],             LOGON_WITH_PROFILE, NULL, argv[3],             CREATE_UNICODE_ENVIRONMENT, lpvEnv, szUserProfile,             &si, &pi))
	
        ShowLastError(L"CreateProcessWithLogonW");

    if (!DestroyEnvironmentBlock(lpvEnv))        ShowLastError(L"DestroyEnvironmentBlock");


    CloseHandle(hToken);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

