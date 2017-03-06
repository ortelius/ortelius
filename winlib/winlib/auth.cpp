/*
You can get a locally valid logon session (a network logon session) by using SSPI.
Simply specify alternate credentials when you call AcquireCredentialsHandle,
and then after you follow the normal InitializeSecurityContext/AcceptSecurityContext
handshake, you'll end up with a logon session for the user,
without having to have the TCB privilege.

See the attached example...

Keith
  */

// this version requires Windows 2000
#define _WIN32_WINNT 0x500
#define UNICODE
#include <windows.h>
#include <stdio.h>
//#define SECURITY_WIN32
#include <security.h>
#pragma comment(lib, "secur32.lib")

// brain-dead error routine that dumps the last error and exits
void _err(const wchar_t* pszFcn, DWORD nErr = GetLastError())
{
    wchar_t szErr[256];
    if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, nErr, 0,
                      szErr, sizeof szErr / sizeof *szErr, 0))
         wprintf(L"%s failed: %s", pszFcn, szErr);
    else wprintf(L"%s failed: 0x%08X", nErr);
    exit(1);
}


PSecurityFunctionTable pSSPI = NULL;
CtxtHandle hctxClient;
CtxtHandle hctxServer;
PCtxtHandle    pServerCtxHandleOut = NULL;
PCtxtHandle    pClientCtxHandleOut = NULL;
CredHandle hcredClient;
CredHandle hcredServer;


// Upon success, returns a handle to a NETWORK logon session
// for the specified principal (it will *not* have network
// credentials). Call CloseHandle to dispose of it when
// you are finished.
/*HANDLE*/ bool _logonUserWithSSPI(wchar_t* pszSSP,
                          //DWORD grfDesiredAccessToToken,
                          wchar_t* pszAuthority,
                          wchar_t* pszPrincipal,
                          wchar_t* pszPassword) {

    // the following code loads the SSPI interface DLL
    // and initializes it, getting a table of function ptrs
    HINSTANCE hdll = LoadLibrary(L"security.dll");
    if (!hdll)
        _err(L"LoadLibrary");
    INIT_SECURITY_INTERFACE_W initSecurityInterface =
            (INIT_SECURITY_INTERFACE_W)GetProcAddress(hdll,
                SECURITY_ENTRYPOINT_ANSIW);
    if (!initSecurityInterface)
        _err(L"GetProcAddress");
    /*PSecurityFunctionTable*/ pSSPI = initSecurityInterface();

    // here's where we specify the credentials to verify
    SEC_WINNT_AUTH_IDENTITY_EX authIdent = {
        SEC_WINNT_AUTH_IDENTITY_VERSION,
        sizeof authIdent,
        (unsigned short *)pszPrincipal, lstrlenW(pszPrincipal),
        (unsigned short *)pszAuthority, lstrlenW(pszAuthority),
        (unsigned short *)pszPassword,  lstrlenW(pszPassword),
        SEC_WINNT_AUTH_IDENTITY_UNICODE,
        0, 0
    };

    // get an SSPI handle for these credentials
    //CredHandle hcredClient;
    TimeStamp expiryClient;
    SECURITY_STATUS err =
        pSSPI->AcquireCredentialsHandle(0, pszSSP,
                                        SECPKG_CRED_OUTBOUND,
                                        0, &authIdent,
                                        0, 0,
                                        &hcredClient,
                                        &expiryClient);
    if (err)
        _err(L"AcquireCredentialsHandle for client", err);

    // use the caller's credentials for the server
    //CredHandle hcredServer;
    TimeStamp expiryServer;
    err = pSSPI->AcquireCredentialsHandle(0, pszSSP,
                                          SECPKG_CRED_INBOUND,
                                          0, 0, 0, 0,
                                          &hcredServer,
                                          &expiryServer);
    if (err)
        _err(L"AcquireCredentialsHandle for server", err);

    //CtxtHandle hctxClient;
    //CtxtHandle hctxServer;

    // create two buffers:
    //    one for the client sending tokens to the server,
    //    one for the server sending tokens to the client
    // (buffer size chosen based on current Kerb SSP setting
    //  for cbMaxToken - you may need to adjust this)
    BYTE bufC2S[8000];
    BYTE bufS2C[8000];
    SecBuffer sbufC2S = { sizeof bufC2S, SECBUFFER_TOKEN, bufC2S };
    SecBuffer sbufS2C = { sizeof bufS2C, SECBUFFER_TOKEN, bufS2C };
    SecBufferDesc bdC2S = { SECBUFFER_VERSION, 1, &sbufC2S };
    SecBufferDesc bdS2C = { SECBUFFER_VERSION, 1, &sbufS2C };

    // don't really need any special context attributes
    DWORD grfRequiredCtxAttrsClient = ISC_REQ_CONNECTION;
    DWORD grfRequiredCtxAttrsServer = ISC_REQ_CONNECTION;

    // set up some aliases to make it obvious what's happening
    PCtxtHandle    pClientCtxHandleIn  = 0;
    /*PCtxtHandle*/    pClientCtxHandleOut = &hctxClient;
    PCtxtHandle    pServerCtxHandleIn  = 0;
    /*PCtxtHandle*/    pServerCtxHandleOut = &hctxServer;
    SecBufferDesc* pClientInput  = 0;
    SecBufferDesc* pClientOutput = &bdC2S;
    SecBufferDesc* pServerInput  = &bdC2S;
    SecBufferDesc* pServerOutput = &bdS2C;
    DWORD          grfCtxAttrsClient = 0;
    DWORD          grfCtxAttrsServer = 0;
    TimeStamp      expiryClientCtx;
    TimeStamp      expiryServerCtx;

    // since the caller is acting as the server, we need
    // a server principal name so that the client will
    // be able to get a Kerb ticket (if Kerb is used)
    wchar_t szSPN[256];
    ULONG cchSPN = sizeof szSPN / sizeof *szSPN;
    GetUserNameEx(NameSamCompatible, szSPN, &cchSPN);

    // perform the authentication handshake, playing the
    // role of both client *and* server.
    bool bClientContinue = true;
    bool bServerContinue = true;
    while (bClientContinue || bServerContinue) {
        if (bClientContinue) {
            sbufC2S.cbBuffer = sizeof bufC2S;
            err = pSSPI->InitializeSecurityContext(
                &hcredClient, pClientCtxHandleIn,
                szSPN,
                grfRequiredCtxAttrsClient,
                0, SECURITY_NATIVE_DREP,
                pClientInput, 0,
                pClientCtxHandleOut,
                pClientOutput,
                &grfCtxAttrsClient,
                &expiryClientCtx);
            switch (err) {
                case 0:
                    bClientContinue = false;
                    break;
                case SEC_I_CONTINUE_NEEDED:
                    pClientCtxHandleIn = pClientCtxHandleOut;
                    pClientInput       = pServerOutput;
                    break;
                default:
                    _err(L"InitializeSecurityContext", err);
            }
        }

        if (bServerContinue) {
            sbufS2C.cbBuffer = sizeof bufS2C;
            err = pSSPI->AcceptSecurityContext(
                &hcredServer, pServerCtxHandleIn,
                pServerInput,
                grfRequiredCtxAttrsServer,
                SECURITY_NATIVE_DREP,
                pServerCtxHandleOut,
                pServerOutput,
                &grfCtxAttrsServer,
                &expiryServerCtx);
            switch (err) {
                case 0:
                    bServerContinue = false;
                    break;
                case SEC_I_CONTINUE_NEEDED:
                    pServerCtxHandleIn = pServerCtxHandleOut;
                    break;
                default:
                    _err(L"AcceptSecurityContext", err);
            }
        }
    }
    // if everything has gone smoothly, we've now got a logon
    // session for the client - let's grab the token now
    if(pSSPI->ImpersonateSecurityContext(pServerCtxHandleOut) != SEC_E_OK) {
		return false;
	}

	return true;
}

//    HANDLE htok;
//    if (!OpenThreadToken(GetCurrentThread(),
//                         grfDesiredAccessToToken,
//                         TRUE, &htok))
//        _err(L"OpenThreadToken");


void _revertUser()
{
    pSSPI->RevertSecurityContext(pServerCtxHandleOut);

    // clean up
    pSSPI->FreeCredentialsHandle(&hcredClient);
    pSSPI->FreeCredentialsHandle(&hcredServer);
    pSSPI->DeleteSecurityContext(pServerCtxHandleOut);
    pSSPI->DeleteSecurityContext(pClientCtxHandleOut);
    
    //return htok;
}


#if 0
// here's an example of usage
void wmain(int argc, wchar_t **argv) {
    // use "NTLM" or "Kerberos"
    //HANDLE htok = _logonUserWithSSPI(L"Kerberos",
    //                                 TOKEN_QUERY,
    //                                 L"bar.com",
    //                                 L"alice",
    //                                 L"alice");
	if(argc < 3) {
		wprintf(L"Usage: <domain> <user> <pass>\n");
		return;
	}

    HANDLE htok = _logonUserWithSSPI(L"NTLM",
                                     TOKEN_QUERY,
                                     argv[1],
                                     argv[2],
                                     argv[3]);
    if (htok) {
        // password is valid!
		wprintf(L"OK\n");
        CloseHandle(htok);
    } else {
		wprintf(L"NOK\n");
	}
}
#endif

