#include "ftpfilelist.h"
// #include "feedback.h"
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif


typedef enum ftp_reply_tag{
FTP_OKAY,
FTP_UNKNOWN_ERROR,
FTP_NO_SUCH_HOST,
FTP_NO_CONNECTION,
FTP_INVALID_LOGIN,
FTP_INVALID_DIR
} FTP_REPLY;


typedef enum ftp_system_type_tag{
FTP_SYSTEM_UNIX,
FTP_SYSTEM_WIN32,
FTP_SYSTEM_AS400,
FTP_SYSTEM_VMS,
FTP_SYSTEM_TANDEM_GUARDIAN,
FTP_SYSTEM_TANDEM_OSS,
FTP_SYSTEM_MVS,
FTP_SYSTEM_UNKNOWN
} FTP_SYSTEM_TYPE;


// RHT 12/08/2011 - bits masks for ftp hex dumps
#define FTP_DUMP_DIRECTORY_LISTINGS			1
#define FTP_DUMP_FILE_CONTENT				2
#define FTP_DUMP_FILE_CONTENT_BEFORE_XLATE	4
#define FTP_DUMP_ADDITIONAL_ATTRS			8
#define FTP_DUMP_AS400_NUL_CONVERSION		16

typedef struct socket_tag{
struct sockaddr_in	name;
int					port;
int					sock;
} TRISOCKET;


typedef struct ftp_connection_tag{
TRISOCKET		*trisocket;
void			*pSsl;
int				(*read_fn)(struct ftp_connection_tag*,char*,int);
int				(*write_fn)(struct ftp_connection_tag*,char*,int);
} FTPCONNECTION;

typedef struct ftp_socket_tag{
FTPCONNECTION	control;		// RHT - 08/04/2008 - converted to struct
FTPCONNECTION	data;			// RHT - 08/04/2008 - added struct for data conn
int				DataPort;
char			*HostName;
char			*UserName;
char			*Password;
FTP_SYSTEM_TYPE	SystemType;
char			*WelcomeText;	// RHT - 12/09/2007 - Added to give system type hints
char			DirSepChar;
char			*DefaultDir;
FTP_REPLY		Result;
char			*LastMessage;	// RHT - 10/04/2008 - space for message from server
char			*LastDir;		// For FTP checkouts
bool			LastMode;		// For FTP checkouts
void			*ssl_ctx;
char			*AS400IASP;		// RHT - 15/08/2011 - name of the AS400 IASPs or NULL for none
// FEEDBACKDATA	*FeedbackData;
} FTPSOCKET;

typedef struct serverreply_t{
int     ReplyCode;
char    *ReplyText;
} SERVERREPLY;

typedef char *caddr_t;

//extern "C" {

void LogFTPTraffic(const char* fmt, ...) ;
void LogFTPTraffic(FTPSOCKET *s, const char* fmt, ...) ;			// RHT 17/11/2011 - expose this too
void LogFTPBuffer(char *buf, int len, int level);					// RHT 12/08/2011 - need for additional attrs
void LogFTPTrace(int level, FTPSOCKET *s, const char* fmt, ...);	// RHT 17/11/2011 - for NUL stripping
FTP_REPLY FTP_LoginToServer(FTPSOCKET *s,char *Username,char *Password);
void FTP_EnterPassiveMode(FTPSOCKET *s);
void FTP_ReadDataFromServer(FTPCONNECTION *s);
void FTP_CloseDataConnection(FTPCONNECTION *s);
char *FTP_ReadLineFromServer(FTPCONNECTION *c);
SERVERREPLY FTP_SwitchDirectory(FTPSOCKET *s,char *newdir);
FTP_FILE_LIST *FTP_GetDirectoryContents(FTPSOCKET *s,char *Pattern = (char *)0);
FTP_FILE_LIST *FTP_GetFiles(FTPSOCKET *s,char *Dir,char *FilePattern,bool showAllVersions);
SERVERREPLY FTP_MakeDirectory(FTPSOCKET *s,char *DirName);
SERVERREPLY FTP_CreateFile(FTPSOCKET *s,char *FileName,char *FileData,long DataLen,bool Binary,bool BrowseMode,bool ReplaceReadOnlyFiles,bool Translate);
FTPSOCKET *FTP_Connect(char *ServerName,char *UserName,char *Password,bool secure,bool encryptData);
SERVERREPLY FTP_SendCommandToServer(FTPSOCKET *s,const char *cmd,const char *param);
char *FTP_GetCurrentDirectory(FTPSOCKET *s);
char *FTP_GetSystemType(FTPSOCKET *s);
SERVERREPLY FTP_GetFileData(FTPSOCKET *s,bool TextFile,char *FileName,unsigned char **FileData,unsigned long *DataLen);
SERVERREPLY FTP_DeleteFile(FTPSOCKET *ftpserver,char *RemoteFileName);
char **GetExtendedAttributes_iSeries(FTPSOCKET *ftpserver,char *folder);
void SetExtendedAttributes_iSeries(FTPSOCKET *ftpserver,char *folder,char *filename,char *mbrtype,char *mbrtext);
//}

char *ReturnErrorPtr();
void SetGlobalErrorPtr(char *ErrStr, ...);
