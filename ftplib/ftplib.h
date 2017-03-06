#ifdef WIN32
#ifdef FTPLIB_EXPORTS
#define FTPLIB_API __declspec(dllexport)
#else
#define FTPLIB_API __declspec(dllimport)
#endif
#else
#define FTPLIB_API
#define LPCTSTR const char*
#endif


#include <psftplib/callback.h>

extern "C" {
FTPLIB_API	int		protocol_initialise(void *Data);
FTPLIB_API	int		protocol_connect(char *HostName,char *UserName,char *Password);
FTPLIB_API	int		protocol_put(char *Filename);
FTPLIB_API	int		protocol_get(char *Filename);
FTPLIB_API	int		protocol_disconnect();
FTPLIB_API	char	*protocol_exec(char **argv,char *shell,bool CopyFromLocal);
FTPLIB_API	int		protocol_cd(char *NewDir);
FTPLIB_API	void	protocol_register_callback(CB_FUNCTION cbf);
FTPLIB_API	char	*protocol_return_last_error();
FTPLIB_API	int		protocol_lcd(char *NewDir);
FTPLIB_API	int		protocol_mkdir(char *Dir);
FTPLIB_API	void	protocol_text();
FTPLIB_API	void	protocol_binary();
FTPLIB_API  char    *protocol_pwd();
FTPLIB_API	char	**protocol_get_file_list(char *Pattern,bool Recursive);
FTPLIB_API	char	**protocol_get_attributes(char *folder);
FTPLIB_API	void	protocol_set_attributes(char *folder,char *filename,char *mbrtype,char *mbrtext);
FTPLIB_API	int		protocol_delete(char *Filename);
}

