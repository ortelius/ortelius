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
   merchantibility or fitness for a particular purpose. The entire 
   risk arising out of use or performance of the software remains 
   with you. 
   
*/
#include "ProcFunctions.h"

void DisplayError(LPWSTR pszAPI)
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

void CreatProc(int argc, WCHAR *argv[])
{
    DWORD     dwSize;
    HANDLE    hToken;
    LPVOID    lpvEnv;
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO         si = {0};
    WCHAR               szUserProfile[256] = L"";

    si.cb = sizeof(STARTUPINFO);
    
     //
    // TO DO: change NULL to '.' to use local account database
    //
    if (!LogonUser(argv[1], NULL, argv[2], LOGON32_LOGON_INTERACTIVE, 
            LOGON32_PROVIDER_DEFAULT, &hToken))
        DisplayError(L"LogonUser");

    //if (!CreateEnvironmentBlock(&lpvEnv, hToken, TRUE))        DisplayError(L"CreateEnvironmentBlock");

   // dwSize = sizeof(szUserProfile)/sizeof(WCHAR);

    //if (!GetUserProfileDirectory(hToken, szUserProfile, &dwSize))        DisplayError(L"GetUserProfileDirectory");

    //
    // TO DO: change NULL to '.' to use local account database
    //
    int bResult;
	bResult = CreateProcessWithLogonW(argv[1], NULL, argv[2],     
        LOGON_WITH_PROFILE, NULL, argv[3],             
		CREATE_UNICODE_ENVIRONMENT, /*lpvEnv*/ NULL, szUserProfile,             &si, &pi);
    DWORD dwError;
	dwError = ::GetLastError();
		DisplayError(L"CreateProcessWithLogonW");

//    if (!DestroyEnvironmentBlock(lpvEnv))        DisplayError(L"DestroyEnvironmentBlock");

    CloseHandle(hToken);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
