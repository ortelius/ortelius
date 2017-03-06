#ifndef __auth_h

#define __auth_h

/*HANDLE*/ bool _logonUserWithSSPI(wchar_t* pszSSP,
                          //DWORD grfDesiredAccessToToken,
                          wchar_t* pszAuthority,
                          wchar_t* pszPrincipal,
                          wchar_t* pszPassword);

void _revertUser();

#endif /*__auth_h*/
