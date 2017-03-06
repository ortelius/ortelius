#ifdef WIN32
#ifdef WINLIB_EXPORTS
#define WINLIB_API __declspec(dllexport)
#else
#define WINLIB_API __declspec(dllimport)
#endif
#else
#define WINLIB_API
#define LPCTSTR const char*
#endif


#include <psftplib/callback.h>

extern "C" {
WINLIB_API	int		protocol_initialise(void *Data);
WINLIB_API	int		protocol_connect(char *HostName,char *UserName,char *Password);
WINLIB_API	int		protocol_put(char *Filename);
WINLIB_API	int		protocol_get(char *Filename);
WINLIB_API	int		protocol_disconnect();
WINLIB_API	char	*protocol_exec(char **argv,char *shell,bool CopyFromLocal);
WINLIB_API	int		protocol_cd(char *NewDir);
WINLIB_API	void	protocol_register_callback(CB_FUNCTION cbf);
WINLIB_API	char	*protocol_return_last_error();
WINLIB_API	int		protocol_lcd(char *NewDir);
WINLIB_API	int		protocol_mkdir(char *Dir);
WINLIB_API	int		protocol_put(char *Filename);
WINLIB_API	int		protocol_get(char *Filename);
WINLIB_API  char    *protocol_pwd();
WINLIB_API	int		protocol_delete(char *Filename);
}

