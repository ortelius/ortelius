#ifdef WIN32
#ifdef PSFTPLIB_EXPORTS
#define PSFTPLIB_API __declspec(dllexport)
#else
#define PSFTPLIB_API __declspec(dllimport)
#endif
#else
#define PSFTPLIB_API
#define LPCTSTR const char *
#endif


#include "callback.h"

#ifndef CFILEINCLUDE
extern "C" {
PSFTPLIB_API	int		protocol_initialise(int initcode);
PSFTPLIB_API	int		protocol_connect(char *HostName,char *UserName,char *Password);
PSFTPLIB_API	int		protocol_put(char *Filename);
PSFTPLIB_API	int		protocol_get(char *Filename);
PSFTPLIB_API	int		protocol_disconnect();
PSFTPLIB_API	char	*protocol_exec(char **argv,char *shell,bool CopyFromLocal);
PSFTPLIB_API	int		protocol_cd(char *NewDir);
PSFTPLIB_API	void	protocol_register_callback(CB_FUNCTION cbf);
PSFTPLIB_API	char	*protocol_return_last_error();
PSFTPLIB_API	int		protocol_lcd(char *NewDir);
PSFTPLIB_API	int		protocol_mkdir(char *Dir);
PSFTPLIB_API	int		protocol_put(char *Filename);
PSFTPLIB_API	int		protocol_get(char *Filename);
PSFTPLIB_API	char	*protocol_pwd();
PSFTPLIB_API	int		protocol_delete(char *Filename);

char *ReturnErrorPtr();
}
#endif
char *pwd;

#ifndef CFILEINCLUDE
extern "C" {
#endif
int InvokeCallback(CALLBACK_REASON res,void *lParam,void *wParam);	// in psftplib.cpp
#ifndef CFILEINCLUDE
}
#endif
