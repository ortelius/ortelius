#ifdef WIN32
#ifdef PSFTPLIB_EXPORTS
#define PSFTPLIB_API __declspec(dllexport)
#else
#define PSFTPLIB_API __declspec(dllimport)
#endif
#else
#define PSFTPLIB_API
#define LPCTSTR const char*
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
//
// Following functions are used to invoke PSFTP functions from psftplib.cpp
//
#ifdef WIN32
int sftp_cmd_open(struct sftp_command *cmd);
int sftp_cmd_cd(struct sftp_command *cmd);
int sftp_cmd_lcd(struct sftp_command *cmd);
int sftp_cmd_mkdir(struct sftp_command *cmd);
int sftp_cmd_put(struct sftp_command *cmd);
int sftp_cmd_get(struct sftp_command *cmd);
int sftp_cmd_rm(struct sftp_command *cmd);
void sk_init(void);
void do_defaults(char *session, Config * cfg);
Config cfg;
#endif
char *pwd;
char *ReturnErrorPtr();
void SetSSHUserName(char *u);				// in ssh.c
void SetSSHPassword(char *p);				// in ssh.c
}
#endif

#ifndef CFILEINCLUDE
extern "C" {
#endif
int InvokeCallback(CALLBACK_REASON res,void *lParam,void *wParam);	// in psftplib.cpp
#ifndef CFILEINCLUDE
}
#endif
