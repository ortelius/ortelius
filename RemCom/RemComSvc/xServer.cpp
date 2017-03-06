/////////////////////////////////////////////////////////////
//
// Author: Zoltan Csizmadia, 7/2000
//
// This code is freeware! If you change it, please send me your version!
//
// Email: zoltan_csizmadia@yahoo.com
//
/////////////////////////////////////////////////////////////

#include "RemCom.h"
#include <stdio.h>
#include <stdlib.h>

long dwSvcPipeCount = 0;
HANDLE hPipeThreadExited = 0;

LPTSTR EmptyStrToNull( LPTSTR str )
{
    return str[0] == _T('\0') ? NULL : str;
}

HANDLE ConnectToRemoteStdOut( RemComMessage* pMsg )
{
    SECURITY_ATTRIBUTES SecAttrib = {0};
    SECURITY_DESCRIPTOR SecDesc;
    InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&SecDesc, TRUE, NULL, FALSE);

    SecAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
    SecAttrib.lpSecurityDescriptor = &SecDesc;;
    SecAttrib.bInheritHandle = TRUE;

    HANDLE hStdOut = INVALID_HANDLE_VALUE;
    
    hStdOut = CreateFile( 
                    pMsg->szStdOut,
                    GENERIC_WRITE, 
                    0,
                    &SecAttrib, 
                    OPEN_EXISTING, 
                    FILE_ATTRIBUTE_NORMAL, 
                    NULL);

    if ( hStdOut != INVALID_HANDLE_VALUE )
        WaitNamedPipe( pMsg->szStdOut, NULL );

    return hStdOut;
}

HANDLE ConnectToRemoteStdIn( RemComMessage* pMsg )
{
    SECURITY_ATTRIBUTES SecAttrib = {0};
    SECURITY_DESCRIPTOR SecDesc;
    InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&SecDesc, TRUE, NULL, FALSE);

    SecAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
    SecAttrib.lpSecurityDescriptor = &SecDesc;;
    SecAttrib.bInheritHandle = TRUE;

    HANDLE hStdOut = INVALID_HANDLE_VALUE;
    
    hStdOut = CreateFile( 
                    pMsg->szStdIn,
                    GENERIC_READ, 
                    0,
                    &SecAttrib, 
                    OPEN_EXISTING, 
                    FILE_ATTRIBUTE_NORMAL, 
                    NULL);

    if ( hStdOut != INVALID_HANDLE_VALUE )
        WaitNamedPipe( pMsg->szStdIn, NULL );

    return hStdOut;
}

DWORD Execute( RemComMessage* pMsg )
{
    DWORD rc = 0;

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset( &si, 0, sizeof(si) );
    si.cb = sizeof(si);

    si.lpTitle = pMsg->szTitle;
    si.wShowWindow = pMsg->wShow;
    si.dwFlags |= STARTF_USESHOWWINDOW;

   HANDLE hStdOut = ConnectToRemoteStdOut( pMsg );

    if ( hStdOut == INVALID_HANDLE_VALUE )
        return (DWORD)-1;

    HANDLE hStdIn = ConnectToRemoteStdIn( pMsg );

    if ( hStdIn == INVALID_HANDLE_VALUE )
        return (DWORD)-2;

    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdOutput = hStdOut;
    si.hStdError = hStdOut;
    si.hStdInput = hStdIn;

    TCHAR lpCmd[_MAX_PATH];

    if ( pMsg->bCmd )
        _stprintf( lpCmd, _T("cmd.exe /c %s"), pMsg->szCommand );
    else
        _tcscpy( lpCmd, pMsg->szCommand );

    if ( CreateProcess( 
        NULL, 
        lpCmd, 
        NULL, 
        NULL, 
        TRUE,
        pMsg->dwCreationFlag | pMsg->dwPriority, 
        NULL, 
        EmptyStrToNull(pMsg->szWorkingDir), 
        &si, 
        &pi ) )
    {
        HANDLE hProcess = pi.hProcess;

        if ( pMsg->bWait )
            WaitForSingleObject( hProcess, INFINITE );

        rc = 0;
        GetExitCodeProcess( hProcess, &rc );
    }
    else
    {
        rc = 1;
    }

    return rc;
}

void ExecPipeThreadProc( void* pParam )
{
    HANDLE hPipe = (HANDLE)pParam;
    RemComMessage msg;
    RemComResponse response;
    DWORD dwWritten;
    DWORD dwRead;

    InterlockedIncrement( &dwSvcPipeCount );

    if ( !ReadFile( hPipe, &msg, sizeof(msg), &dwRead, NULL ) || dwRead == 0 )
    {
        cerr << _T("Error in ExecPipeThreadProc()[ReadFile()] : ");
        ShowLastErrorText();
        cerr << endl;

        goto cleanup;
    }

    response.dwReturnCode = Execute( &msg );

    if ( !WriteFile( hPipe, &response, sizeof(response), &dwWritten, NULL ) || dwWritten == 0 )
    {
        cerr << _T("Error in ExecPipeThreadProc()[WriteFile()] : ");
        ShowLastErrorText();
        cerr << endl;

        goto cleanup;
    }

cleanup:

    DisconnectNamedPipe( hPipe );
    CloseHandle( hPipe );

    InterlockedDecrement( &dwSvcPipeCount );
    
    SetEvent( hPipeThreadExited );
}

void PipePoolThread( void* )
{
    HANDLE hPipe = NULL;
    
    for (;;)
    {
        SECURITY_ATTRIBUTES SecAttrib = {0};
        SECURITY_DESCRIPTOR SecDesc;
        InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);
        SetSecurityDescriptorDacl(&SecDesc, TRUE, NULL, TRUE);

        SecAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
        SecAttrib.lpSecurityDescriptor = &SecDesc;;
        SecAttrib.bInheritHandle = TRUE;

        hPipe = CreateNamedPipe(
            _T("\\\\.\\pipe\\")RemComPIPENAME, 
            PIPE_ACCESS_DUPLEX, 
            PIPE_TYPE_MESSAGE | PIPE_WAIT, 
            PIPE_UNLIMITED_INSTANCES,
            0,
            0,
            0xffffffff,
            &SecAttrib);

        if ( hPipe != NULL )
        {
            ConnectNamedPipe( hPipe, NULL );
            _beginthread(ExecPipeThreadProc, 0, (void*)hPipe);
        }
    }
}

DWORD ServerMain( DWORD, LPTSTR* )
{
    DWORD rc = 0;

    dwSvcPipeCount = 0;

    hPipeThreadExited = CreateEvent( NULL, FALSE, FALSE, NULL );

    _beginthread( PipePoolThread, 0, NULL );

    while( WaitForSingleObject( hPipeThreadExited, INFINITE ) )
    {
        if ( dwSvcPipeCount == 0 )
            break;
    }

    CloseHandle( hPipeThreadExited );

    return rc;
}
