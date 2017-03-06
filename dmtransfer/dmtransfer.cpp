// dmtransfer.cpp : Defines the entry point for the console application.
//
#include "openssl/md5.h"
#include "stdafx.h"
#include <string.h>
#include "options.h"
#include "navigate.h"
#include <psftplib/callback.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "tinyxml.h"

#define META_FILE_NAME "dm.info"

#ifdef WIN32
#include <process.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#define STRCASECMP stricmp
#define TEMPDIR getenv("TEMP")
#define DLOPEN(a) LoadLibrary(a)
#define DLSYM(a,b) GetProcAddress(a,b)
typedef HMODULE LIBPTR;
#define SepChar '\\'
#define READ_PERMS (O_RDONLY | O_BINARY)
#define strdup _strdup
#define read _read
#define open _open
#define close _close
#define mkdir _mkdir
#else
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <dlfcn.h>
#define STRCASECMP strcasecmp
#define TEMPDIR "/tmp"
#define DLOPEN(a) dlopen(a,RTLD_LAZY)
#define DLSYM(a,b) dlsym(a,b)
typedef void * LIBPTR;
#define SepChar '/'
#define READ_PERMS (O_RDONLY)
#endif

typedef enum transfer_mode_tag{
TRANSFER_MODE_TEXT,
TRANSFER_MODE_BINARY
} TRANSFER_MODE;

typedef enum target_line_endings_tag{
TARGET_UNIX,
TARGET_WINDOWS,
TARGET_MAC,
TARGET_NONE
} TARGET_LINE_ENDINGS;

typedef struct filedata_tag{
char			*FileName;
char			*SourceFileName;
size_t			FileSize;
TRANSFER_MODE	TransferMode;
char			md5string[33];
char			*MBR_Text;	// For AS400 source/target
char			*MBR_Type;	// For AS400 source/target
} FILEDATA;

typedef struct metadata_tag{
char *FilePattern;
bool TextFile;
bool Ignore;
bool IsDir;
char *MBR_Text;	// For AS400 source/target
char *MBR_Type; // For AS400 source/target
struct metadata_tag	*next;
} META_DATA;

OPTIONS options[]={
{"-protocol",		'Y',	"TRIPROTOCOL"},
{"-server",			'Y',	"TRISERVER"},
{"-sourcedir",		'Y',	"TRISOURCEDIR"},
{"-targetdir",		'Y',	"TRITARGETDIR"},
{"-user",			'Y',	"TRIUSERNAME"},
{"-password",		'Y',	"TRIPASSWORD"},
{"-target",			'Y',	"TRITARGET"},
{"-exec",			'N',	"TRIEXECUTE"},
{"-copyexec",		'N',	"TRICOPYEXEC"},
{"-dellist",		'Y',	"TRIDELLIST"},
{"-checkbase",		'N',	"TRICHECKBASE"},
{"-shell",			'Y',	"TRISHELL"},
{"-debug",			'N',	"TRIDEBUG"},
{"-port",			'Y',	"TRIPORTNUM"},
{"-tty",			'N',	"TRIUSETTY"},
{"-keyfile",		'Y',	"TRIKEYFILE"}
};

char *Usage="USAGE: dmtransfer -protocol <protocol> -server <servername> [-user <user>] [-password <password> | -keyfile <filename>] [-usetty] [-port <portnum>] [-sourcedir <sourcedir> -targetdir <targetdir> -target <lineendings> | -exec <command> | -dellist <file>]";

//
// Other options
// Password should be passed in through stdin and be base64 encrypted 3DES string
// -CMD (Create Missing Target Directories)
// -INN (Ignore New Key)
// -IDN (Ignore Different Key)
// -tf <file> Text File Name (File contains Text File Extensions (Used by FTP/FTPS to switch mode, by SFTP to convert file?)

typedef enum protocol_type_tag{
INIT_FTP,
INIT_SFTP,
INIT_FTPS,
INIT_WIN
} PROTOCOL_TYPE;

typedef struct protocol_tag{
char			*ProtocolName;
char			*ProtocolLibrary;
bool			ExtendedAttributes;
PROTOCOL_TYPE	InitType;
} PROTOCOL;

#ifdef WIN32
PROTOCOL protocols[]={
{"ftp",		"ftplib.dll",	true,	INIT_FTP},
{"sftp",	"psftplib.dll",	false,	INIT_SFTP},
{"ftps",	"ftplib.dll",	false,	INIT_FTPS},
{"win",		"winlib.dll",	false,	INIT_WIN}
};
#else
PROTOCOL protocols[]={
{"ftp",		"libftplib.so",	0,	INIT_FTP},
{"sftp",	"libpsftplib.so",	0,	INIT_SFTP},
{"win",	"libwinlib.so",	0,	INIT_FTPS},
{"ftps",	"libftplib.so",	1,	INIT_WIN}
};
#endif


typedef enum err_tag{
ERROR_NO_ERROR,
ERROR_INVALID_PROTOCOL,
ERROR_COULD_NOT_FIND_LIBRARY,
ERROR_NO_INITIALISE_FUNCTION,
ERROR_NO_CONNECT_FUNCTION,
ERROR_NO_DISCONNECT_FUNCTION,
ERROR_NO_PUT_FUNCTION,
ERROR_NO_GET_FUNCTION,
ERROR_NO_EXEC_FUNCTION,
ERROR_NO_CD_FUNCTION,
ERROR_NO_CALLBACK_FUNCTION,
ERROR_NO_RETURN_LAST_ERROR_FUNCTION,
ERROR_NO_LCD_FUNCTION,
ERROR_NO_MKDIR_FUNCTION,
ERROR_NO_PWD_FUNCTION,
ERROR_NO_GET_ATTRIBUTES_FUNCTION,
ERROR_NO_SET_ATTRIBUTES_FUNCTION,
ERROR_NO_DELETE_FUNCTION,
ERROR_INITIALISE_FAILED
} ERRORCODE;

typedef int					(*PROTOCOL_INITIALISE)(void *Data);
typedef int					(*PROTOCOL_CONNECT)(char *HostName,char *UserName,char *Password);
typedef int					(*PROTOCOL_PUT)(char *Filename);
typedef int					(*PROTOCOL_GET)(char *Filename);
typedef int					(*PROTOCOL_DISCONNECT)();
typedef char *				(*PROTOCOL_EXEC)(char **argv,char *shell,bool CopyFromLocal);
typedef int					(*PROTOCOL_CD)(char *NewDir);
typedef void				(*PROTOCOL_REGISTER_CALLBACK)(CB_FUNCTION cbf);
typedef char *				(*PROTOCOL_RETURN_LAST_ERROR)(void);
typedef int					(*PROTOCOL_LCD)(char *NewDir);
typedef int					(*PROTOCOL_MKDIR)(char *Dir);
typedef int					(*PROTOCOL_PUT)(char *FileName);
typedef char *              (*PROTOCOL_PWD)();
typedef char **				(*PROTOCOL_GET_ATTRIBUTES)(char *folder);
typedef void				(*PROTOCOL_SET_ATTRIBUTES)(char *folder,char *filename,char *mbrtype,char *mbrtext);
typedef int					(*PROTOCOL_DELETE)(char *Filename);

PROTOCOL_INITIALISE			protocol_initialise;
PROTOCOL_CONNECT			protocol_connect;
PROTOCOL_PUT				protocol_put;
PROTOCOL_GET				protocol_get;
PROTOCOL_DISCONNECT			protocol_disconnect;
PROTOCOL_EXEC				protocol_exec;
PROTOCOL_CD					protocol_cd;
PROTOCOL_REGISTER_CALLBACK	protocol_register_callback;
PROTOCOL_RETURN_LAST_ERROR	protocol_return_last_error;
PROTOCOL_LCD				protocol_lcd;
PROTOCOL_MKDIR				protocol_mkdir;
PROTOCOL_PWD				protocol_pwd;
PROTOCOL_GET_ATTRIBUTES		protocol_get_attributes;
PROTOCOL_SET_ATTRIBUTES		protocol_set_attributes;
PROTOCOL_DELETE				protocol_delete;



TiXmlElement *GetElementFromRelativePath(TiXmlElement *element, const char *Path)
{
	char *pc = strdup(Path);
	char *x=strtok(pc,"/");
	while (x)
	{
		if (element)
		{
			element=element->FirstChildElement(x);
		}
		x=strtok(NULL,"/");
	}
	free(pc);
	return element;
}

TiXmlElement *GetElementFromPath(TiXmlDocument *doc,const char *Path)
{
	char *pc = strdup(Path);
	char *x=strtok(pc,"/");
	TiXmlElement *element=(TiXmlElement *)0;
	while (x)
	{
		if (element)
		{
			element=element->FirstChildElement(x);
		}
		else
		{
			element=doc->FirstChildElement(x);
		}
		x=strtok(NULL,"/");
	}
	free(pc);
	return element;
}

const char *GetAttributeFromPath(TiXmlDocument *doc,const char *Path)
{
	TiXmlElement *e=GetElementFromPath(doc,Path);
	return e?e->GetText():(char *)0;
}

const char *GetAttributeFromRelativePath(TiXmlElement *element, const char *Path)
{
	TiXmlElement *e=GetElementFromRelativePath(element,Path);
	return e?e->GetText():(char *)0;
}

const char *GetAttribute(TiXmlElement *ele, const char *att)
{
	char *ret = (char *)ele->Attribute(att);
	if (ret==(char *)0)
	{
		ret = (char *)GetAttributeFromRelativePath(ele,att);
	}
	return (const char *)ret;
}

static void ExitWithError(char *ErrStr, ...)
{
	va_list ap;
	va_start(ap,ErrStr);
	vfprintf(stderr,ErrStr,ap);
	fprintf(stderr,"\n");
	va_end(ap);
	exit(1);
}

PROTOCOL *GetProtocolLibrary(char *ProtocolName)
{
	bool Found=false;
	int i;
	int NumProtocols = sizeof(protocols)/sizeof(protocols[0]);
	for (i=0;i<NumProtocols;i++)
	{
		if (strcmp(protocols[i].ProtocolName,ProtocolName)==0)
		{
			// Found it
			Found=true;
			break;
		}
	}
	if (!Found)
	{
		throw(ERROR_INVALID_PROTOCOL);	// Could not find Protocol
	}
	return &(protocols[i]);
}

void LoadProtocolLibrary(char *ProtocolName,char *TargetDir)
{
	try
	{
		PROTOCOL *Protocol = GetProtocolLibrary(ProtocolName);	// throws error if can't find library6
		LIBPTR protolib = DLOPEN(Protocol->ProtocolLibrary);
		if (protolib == NULL)
		{
#ifndef WIN32			
			printf("DLERR=%s errno=%d\n",dlerror(),errno); 
#endif			
			throw(ERROR_COULD_NOT_FIND_LIBRARY);	// Could not locate protocol library
		}	
		protocol_initialise = (PROTOCOL_INITIALISE)DLSYM(protolib,"protocol_initialise");
		protocol_connect = (PROTOCOL_CONNECT)DLSYM(protolib,"protocol_connect");
		protocol_disconnect = (PROTOCOL_DISCONNECT)DLSYM(protolib,"protocol_disconnect");
		protocol_put = (PROTOCOL_PUT)DLSYM(protolib,"protocol_put");
		protocol_get = (PROTOCOL_GET)DLSYM(protolib,"protocol_get");
		protocol_exec = (PROTOCOL_EXEC)DLSYM(protolib,"protocol_exec");
		protocol_cd = (PROTOCOL_CD)DLSYM(protolib,"protocol_cd");
		protocol_register_callback = (PROTOCOL_REGISTER_CALLBACK)DLSYM(protolib,"protocol_register_callback");
		protocol_return_last_error = (PROTOCOL_RETURN_LAST_ERROR)DLSYM(protolib,"protocol_return_last_error");
		protocol_lcd = (PROTOCOL_LCD)DLSYM(protolib,"protocol_lcd");
		protocol_mkdir = (PROTOCOL_MKDIR)DLSYM(protolib,"protocol_mkdir");
		protocol_pwd = (PROTOCOL_PWD)DLSYM(protolib,"protocol_pwd");
		protocol_delete = (PROTOCOL_DELETE)DLSYM(protolib,"protocol_delete");
		if (Protocol->ExtendedAttributes)
		{
			protocol_get_attributes = (PROTOCOL_GET_ATTRIBUTES)DLSYM(protolib,"protocol_get_attributes");
			protocol_set_attributes = (PROTOCOL_SET_ATTRIBUTES)DLSYM(protolib,"protocol_set_attributes");
		}
		else
		{
			protocol_get_attributes = (PROTOCOL_GET_ATTRIBUTES)0;
			protocol_set_attributes = (PROTOCOL_SET_ATTRIBUTES)0;
		}
		//
		// Make sure all the functions have been set up correctly, throw error otherwise
		//
		if (protocol_initialise == NULL) throw(ERROR_NO_INITIALISE_FUNCTION);
		if (protocol_connect == NULL) throw(ERROR_NO_CONNECT_FUNCTION);
		if (protocol_disconnect == NULL) throw(ERROR_NO_DISCONNECT_FUNCTION);
		if (protocol_put == NULL) throw(ERROR_NO_PUT_FUNCTION);
		if (protocol_get == NULL) throw(ERROR_NO_GET_FUNCTION);
		if (protocol_exec == NULL) throw(ERROR_NO_EXEC_FUNCTION);
		if (protocol_cd == NULL) throw(ERROR_NO_CD_FUNCTION);
		if (protocol_register_callback == NULL) throw(ERROR_NO_CALLBACK_FUNCTION);
		if (protocol_return_last_error == NULL) throw(ERROR_NO_RETURN_LAST_ERROR_FUNCTION);
		if (protocol_lcd == NULL) throw(ERROR_NO_LCD_FUNCTION);
		if (protocol_mkdir == NULL) throw(ERROR_NO_MKDIR_FUNCTION);
		if (protocol_pwd == NULL) throw(ERROR_NO_PWD_FUNCTION);
		if (protocol_delete == NULL) throw(ERROR_NO_DELETE_FUNCTION);
		if (Protocol->ExtendedAttributes)
		{
			if (protocol_get_attributes == NULL) throw(ERROR_NO_GET_ATTRIBUTES_FUNCTION);
			if (protocol_set_attributes == NULL) throw(ERROR_NO_SET_ATTRIBUTES_FUNCTION);
		}
		//
		// All functions present, correct and loaded. Call initialise function
		//
		
		void *pv = (void *)0;
		char *szPort = getenv("TRIPORTNUM");
		int port = (szPort)?atoi(szPort):0;
		bool useTTY = getenv("TRIUSETTY")?true:false;

		switch(Protocol->InitType)
		{
			case INIT_FTP:
				pv = (void *)0;
				break;
			case INIT_FTPS:
				pv = (void *)1;
				break;
			case INIT_SFTP:
				// Set port number here if we're overriding it
				if (useTTY) {
					pv = (void *)(0x10000 | port);	// Set top 16 bits to 1 if usetty set
				} else {
					pv = (void *)port;
				}
				break;
			case INIT_WIN:
				pv = (void *)TargetDir;
				break;
		}

		int initcode = protocol_initialise(pv);

		if (initcode != 0)
		{
			throw(ERROR_INITIALISE_FAILED);
		}
	}
	catch(ERRORCODE errcode)
	{
		switch(errcode)
		{
		case ERROR_INVALID_PROTOCOL:
			ExitWithError("ERROR: Invalid Protocol");
			break;
		case ERROR_COULD_NOT_FIND_LIBRARY:
			ExitWithError("ERROR: Failed to locate library for specified protocol");
			break;
		case ERROR_NO_INITIALISE_FUNCTION:
			ExitWithError("ERROR: Failed to locate \"protocol_initialise\" for specified protocol");
			break;
		case ERROR_NO_CONNECT_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_connect\" function in library");
			break;
		case ERROR_NO_DISCONNECT_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_disconnect\" function in library");
			break;
		case ERROR_NO_PUT_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_put\" function in library");
			break;
		case ERROR_NO_GET_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_get\" function in library");
			break;
		case ERROR_NO_EXEC_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_exec\" function in library");
			break;
		case ERROR_NO_CALLBACK_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_register_callback\" function in library");
			break;
		case ERROR_NO_CD_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_cd\" function in library");
			break;
		case ERROR_NO_RETURN_LAST_ERROR_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_return_last_error\" function in library");
			break;
		case ERROR_NO_LCD_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_lcd\" function in library");
			break;
		case ERROR_NO_MKDIR_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_mkdir\" function in library");
			break;
		case ERROR_NO_DELETE_FUNCTION:
			ExitWithError("ERROR: Cannot locate \"protocol_delete\" function in library");
			break;
		case ERROR_INITIALISE_FAILED:
			ExitWithError("ERROR: Failed to initialise protocol");
			break;
		default:
			ExitWithError("ERROR: Unknown error %d loading protocol library",errcode);
			break;
		}
	}
}

int CallbackFunction(CALLBACK_REASON res,void *lParam,void *wParam)
{
	int reply=1;
	switch (res)
	{
	case CALLBACK_PRECONNECT:
		if (lParam && (getenv("TRIEXECUTE") || getenv("TRICOPYEXEC"))) reply=0;	// Abandon connection (Windows protocol and executing)
		break;
	case CALLBACK_KEY_DIFFERS:
		// printf("KEY DIFFERS: Keytype: %s Key: %s\n",(char *)lParam,(char *)wParam);
		break;
	case CALLBACK_KEY_NEW:
		// printf("KEY NEW: Keytype: %s Key: %s\n",(char *)lParam,(char *)wParam);
		break;
	case CALLBACK_REMOTE_DIRECTORY:
		// printf("Callback Remote Directory=[%s]\n",(char *)lParam);
		break;
	case CALLBACK_PUT:
		// printf("Put: %s -> %s\n",(char *)lParam,(char *)wParam);
		break;
	}
	return reply;
}

int CreateFullTargetDirectory(char *TargetDirectory)
{
	char *tgt = strdup(TargetDirectory);
	int res = protocol_mkdir(tgt);
	if (!res)
	{
		//
		// mkdir failed - lop directory off the end and try again
		//
		char *ep = tgt;
		char *bp = (char *)0;
		while (*ep)
		{
			if (*ep == '/' || *ep == '\\') bp=ep;
			ep++;
		}
		if (bp)
		{
			*bp='\0';
			res = CreateFullTargetDirectory(tgt);
			if (res)
			{
				// Successfully created parent - try again
				res = protocol_mkdir(TargetDirectory);
			}
		}
		else
		{
			res = 1;	// Run out of directories
		}
	}
	free(tgt);
	return res;
}

void SwitchDirAndCreateIfNecessary(char *TargetDir)
{
	bool CreateTargetDirectories=true;
	int res = protocol_cd(TargetDir);
	if (res == 0)
	{
		// cd has failed - let's try just creating the directory
		if (CreateTargetDirectories)
		{
			res = CreateFullTargetDirectory(TargetDir);
			if (res == 0)
			{
				throw("Failed to create Target Directory\n");
			}
			else
			{
				// Success
				// printf("Create ok\ncd %s\n",TargetDir);
				res = protocol_cd(TargetDir);
				if (res == 0)
				{
					throw("Failed to cd to directory");
				}
			}
		}
		else
		{
			throw("Target Directory does not exist and auto create is off\n");
		}
	}
}



char *GetTempFileName(char *BaseName)
{
	int pid = getpid();
	char *TempDir = TEMPDIR;
	int sn = 1;
	struct stat s;
	char *TempDirPath = (char *)malloc(strlen(TempDir)+128);

	do
	{
		sprintf(TempDirPath,"%s%c%d%06d.temp",TempDir,SepChar,pid,sn);
		sn++;
	}
	while (stat(TempDirPath,&s)==0);

	char *TempFilePath = (char *)malloc(strlen(TempDir)+strlen(BaseName)+128);
	TempFilePath[0]='\0';
#ifdef WIN32
	int res = mkdir(TempDirPath);
#else
	int res = mkdir(TempDirPath,0777);
#endif
	if (res == 0)
	{
		sprintf(TempFilePath,"%s%c%s",TempDirPath,SepChar,BaseName);
	}

	return TempFilePath;
}

bool CompareFileNames(char *Pattern,char *FileName)
{
	if (Pattern[0]=='*' && Pattern[1]=='.')
	{
		// *.TXT etc - just compare extensions
		char *dp = FileName;
		char *dot = (char *)0;
		while (*dp)
		{
			if (*dp=='.') dot = dp;
			dp++;
		}
		if (dot)
		{
			if (strcmp(dot,&(Pattern[1]))==0)
			{
				// Match
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return (strcmp(Pattern,FileName)==0)?true:false;
	}
}


#ifdef WIN32
void ConvertFileToUnix(char *SrcDir,FILEDATA *FileData)
{
	MD5_CTX md5;
	char linebuf[8192];
	unsigned char md5res[16];
	size_t TotalBytes=0;
	for (int i=0;i<sizeof(md5res);i++) md5res[i]='\0';
	MD5_Init(&md5);
	char *FullPathName = (char *)malloc(strlen(SrcDir)+strlen(FileData->FileName)+10);
	char *OutputPathName = GetTempFileName(FileData->FileName);
	sprintf(FullPathName,"%s%c%s",SrcDir,SepChar,FileData->FileName);
	FILE *in = fopen(FullPathName,"r");		// Input is text mode
	FILE *out = fopen(OutputPathName,"wb");	// Output is binary mode
	if (out)
	{
		while (fgets(linebuf,sizeof(linebuf),in))
		{
			//
			// Since the file is being read in text mode (we didn't include "b" in the open mode)
			// we will just have \n at the end of the line. Just add this to our MD5 calculation
			// and write the line to our target file. Since that HAS been opened in binary mode,
			// we should just end up with a \n at the end of each line.
			//
			int LineLength = strlen(linebuf);
			MD5_Update(&md5,linebuf,LineLength);
			fprintf(out,linebuf);
			TotalBytes+=LineLength;
		}
		fclose(out);
	}
	else
	{
		ExitWithError("Could not open output file [%s]\n",OutputPathName);
	}
	fclose(in);
	MD5_Final(md5res,&md5);
	for (int i=0;i<sizeof(md5res);i++) sprintf(&(FileData->md5string[i*2]),"%02x",md5res[i]);
	FileData->FileSize = TotalBytes;
	FileData->TransferMode = TRANSFER_MODE_TEXT;
	FileData->SourceFileName = strdup(OutputPathName);
}
#else
void ConvertFileToWindows(char *SrcDir,FILEDATA *FileData)
{
	MD5_CTX md5;
	char linebuf[8192];
	unsigned char md5res[16];
	size_t TotalBytes=0;
	for (int i=0;i<sizeof(md5res);i++) md5res[i]='\0';
	MD5_Init(&md5);
	char *FullPathName = (char *)malloc(strlen(SrcDir)+strlen(FileData->FileName)+10);
	char *OutputPathName = GetTempFileName(FileData->FileName);
	sprintf(FullPathName,"%s%c%s",SrcDir,SepChar,FileData->FileName);
	FILE *in = fopen(FullPathName,"r");		// Input is text mode
	FILE *out = fopen(OutputPathName,"w");	// Output is binary mode
	if (out)
	{
		while (fgets(linebuf,sizeof(linebuf),in))
		{
			//
			// Since we're on Unix/Linux the source file will just have \n at the end of the line. Add a \r 
			// and add this to our MD5 calculation and write the line to our target file.
			//
			int n = strlen(linebuf);
			linebuf[n-1]='\r';
			linebuf[n]='\n';
			linebuf[n+1]='\0';
			MD5_Update(&md5,linebuf,n+1);
			fprintf(out,linebuf);
			TotalBytes+=n;
		}
		fclose(out);
	}
	else
	{
		ExitWithError("Could not open output file [%s]\n",OutputPathName);
	}
	fclose(in);
	MD5_Final(md5res,&md5);
	for (int i=0;i<sizeof(md5res);i++) sprintf(&(FileData->md5string[i*2]),"%02x",md5res[i]);
	FileData->FileSize = TotalBytes;
	FileData->TransferMode = TRANSFER_MODE_TEXT;
	FileData->SourceFileName = strdup(OutputPathName);
}
#endif

TARGET_LINE_ENDINGS TargetLineEndings = TARGET_UNIX;	// Default - can be overwritten

FILEDATA *GetFileData(char *SrcDir,char *Filename,META_DATA *MetaData)
{
	FILEDATA *res = (FILEDATA *)malloc(sizeof(FILEDATA));

	res->FileName = strdup(Filename);
	res->SourceFileName = (char *)0;
	//
	// Check to see if this file is to be transferred in text or binary mode
	// To do this, we run through each file or extension in the MetaData list
	// checking each one against our filename. The first one that matches gives
	// us our transfer mode.
	//
	META_DATA *Curr = MetaData;
	bool TextFile=false;
	res->MBR_Text = (char *)0;
	res->MBR_Type = (char *)0;
	while (Curr)
	{
		if (CompareFileNames(Curr->FilePattern,Filename))
		{
			TextFile = Curr->TextFile;
			res->MBR_Text = Curr->MBR_Text;
			res->MBR_Type = Curr->MBR_Type;
			break;
		}
		Curr=Curr->next;
	}
#ifdef WIN32
	// This is a windows machine - is the target Unix?
	// Note - we should only do this if the transfer mode is SFTP
	// (since FTP will use the appropriate transfer mode)
	//
	if (TextFile && TargetLineEndings == TARGET_UNIX)
	{
		ConvertFileToUnix(SrcDir,res);
	}
#else
	// This is a Unix machine - is the target Windows?
	if (TextFile && TargetLineEndings == TARGET_WINDOWS)
	{
		ConvertFileToWindows(SrcDir,res);
	}
#endif
	else
	{
		// Either file is binary or we don't need to convert - just set source file
		// name to be the same as our input file name and calculate the MD5
		//
		MD5_CTX md5;
		char buf[1024];
		res->TransferMode = TRANSFER_MODE_BINARY;
		res->SourceFileName = strdup(res->FileName);
		char *FullPathName = (char *)malloc(strlen(SrcDir)+strlen(Filename)+10);
		sprintf(FullPathName,"%s%c%s",SrcDir,SepChar,Filename);
		int in = open(FullPathName,READ_PERMS);
		if (in != -1)
		{
			unsigned char md5res[16];
			for (int i=0;i<sizeof(md5res);i++) md5res[i]='\0';
			size_t TotalBytes=0;

			MD5_Init(&md5);
			int BytesRead=0;
			do
			{
				BytesRead = read(in,buf,sizeof(buf));
				if (BytesRead > 0)
				{
					MD5_Update(&md5,buf,BytesRead);
					TotalBytes+=BytesRead;
				}
			} while (BytesRead > 0);
			close(in);
			MD5_Final(md5res,&md5);
			for (int i=0;i<sizeof(md5res);i++) sprintf(&(res->md5string[i*2]),"%02x",md5res[i]);
			res->FileSize = TotalBytes;
		}
		free(FullPathName);
	}
	return res;
}

META_DATA *GetMetaData(char *SrcDir,char *MetaDataFileName,META_DATA *ParentMetaData)
{
	TiXmlDocument doc;
	struct stat s;

	META_DATA *Head = (META_DATA *)0;
	META_DATA **ins = &Head;
	char *MetaDataPathName = (char *)malloc(strlen(SrcDir)+strlen(MetaDataFileName)+10);
	sprintf(MetaDataPathName,"%s%c%s",SrcDir,SepChar,MetaDataFileName);
	int res = stat(MetaDataPathName,&s);
	if (res == 0)
	{
		if (s.st_size > 0)
		{
			// File exists and has something in it
			doc.LoadFile(MetaDataPathName);
			if (doc.Error())
			{
				// Problem with parsing
				ExitWithError("Error at line %d column %d of config file %s\n%s",
				doc.ErrorRow(),
				doc.ErrorCol(),
				MetaDataPathName,
				doc.ErrorDesc());
			}
			//
			// Parse the XML and create the list of objects
			//
			TiXmlElement *f = GetElementFromPath(&doc,"files");
			TiXmlElement *fp = f->FirstChildElement("file");
			while (fp)
			{
				const char *Pattern = GetAttribute(fp,"name");
				const char *TextFile = GetAttribute(fp,"text");
				const char *Ignore = GetAttribute(fp,"ignore");
				const char *MBR_Text = GetAttribute(fp,"mbrtext");
				const char *MBR_Type = GetAttribute(fp,"mbrtype");
				*ins = (META_DATA *)malloc(sizeof(META_DATA));
				if (!Head) Head = *ins;
				(*ins)->next = (META_DATA *)0;
				(*ins)->FilePattern = strdup(Pattern);
				(*ins)->MBR_Text = (char *)MBR_Text;	// NULL if not present
				(*ins)->MBR_Type = (char *)MBR_Type;	// NULL if not present
				if (TextFile && STRCASECMP(TextFile,"true")==0)
				{
					(*ins)->TextFile = true;
				}
				else
				if (TextFile && STRCASECMP(TextFile,"false")==0)
				{
					(*ins)->TextFile = false;
				}
				else
				{
					(*ins)->TextFile = TextFile?(TextFile[0]=='Y' || TextFile[0]=='y')?true:false:false;
				}
				if (Ignore && STRCASECMP(Ignore,"true")==0)
				{
					(*ins)->Ignore = true;
				}
				else
				if (Ignore && STRCASECMP(Ignore,"false")==0)
				{
					(*ins)->Ignore = false;
				}
				else
				{
					(*ins)->Ignore = Ignore?(Ignore[0]=='Y' || Ignore[0]=='y')?true:false:false;
				}
				(*ins)->IsDir = false;
				ins = &((*ins)->next);
				fp = fp->NextSiblingElement("file");
			}
			fp = f->FirstChildElement("dir");
			while (fp)
			{
				const char *Pattern = GetAttribute(fp,"name");
				const char *Ignore = GetAttribute(fp,"ignore");
				*ins = (META_DATA *)malloc(sizeof(META_DATA));
				if (!Head) Head = *ins;
				(*ins)->next = (META_DATA *)0;
				(*ins)->FilePattern = strdup(Pattern);
				(*ins)->MBR_Text = (char *)0;
				(*ins)->MBR_Type = (char *)0;
				if (Ignore && STRCASECMP(Ignore,"true")==0)
				{
					(*ins)->Ignore = true;
				}
				else
				if (Ignore && STRCASECMP(Ignore,"false")==0)
				{
					(*ins)->Ignore = false;
				}
				else
				{
					(*ins)->Ignore = Ignore?(Ignore[0]=='Y' || Ignore[0]=='y')?true:false:false;
				}
				(*ins)->IsDir = true;
				ins = &((*ins)->next);
				fp = fp->NextSiblingElement("dir");
			}
		}
	}
	free(MetaDataPathName);
	//
	// Add existing ParentMetaData to end of any list
	//
	META_DATA *Curr = ParentMetaData;
	while (Curr)
	{
		*ins = (META_DATA *)malloc(sizeof(META_DATA));
		if (!Head) Head = *ins;
		(*ins)->next = (META_DATA *)0;
		(*ins)->FilePattern = strdup(Curr->FilePattern);
		(*ins)->TextFile = Curr->TextFile;
		(*ins)->IsDir = Curr->IsDir;
		(*ins)->Ignore = Curr->Ignore;
		(*ins)->MBR_Text = Curr->MBR_Text;
		(*ins)->MBR_Type = Curr->MBR_Type;
		ins = &((*ins)->next);
		Curr=Curr->next;
	}

	//
	// debug
	//
	/*
	printf("Return from GetMetaData, list is:\n");
	Curr = Head;
	while (Curr)
	{
		printf("Pattern = [%s] Text = [%s] \n",Curr->FilePattern,Curr->TextFile?"true":"false");
		Curr=Curr->next;
	}
	printf("End of List\n");
	*/
	return Head;
}

void FreeMetaData(META_DATA *MetaData)
{
	META_DATA *Curr = MetaData;
	while (Curr)
	{
		META_DATA *n = Curr->next;
		free(Curr->FilePattern);
		free(Curr);
		Curr=n;
	}
}

char *GetDirectoryName(char *FullPathName)
{
	char *res = strdup(FullPathName);
	int n = strlen(res);
	while (n && res[n] != '/' && res[n] != '\\') n--;
	res[n]='\0';
	return res;
}

bool Ignore(char *Filename,META_DATA *md)
{
	bool res=false;
	META_DATA *p = md;
	while (p)
	{
		if (p->Ignore  && CompareFileNames(p->FilePattern,Filename))
		{
			res=true;
			break;
		}
		p=p->next;
	}
	return res;
}	

void Iterate(char *SourceDir,char *TargetDir,char *DirName=(char *)0,META_DATA *ParentMetaData = (META_DATA *)0)
{
	bool CreateEmptyDirectories=true;
	FILE_DETAILS f;
	char *src;
	char *tgt;
	if (DirName)
	{
		src = (char *)malloc(strlen(SourceDir)+strlen(DirName)+10);
		sprintf(src,"%s%c%s",SourceDir,SepChar,DirName);
		tgt = (char *)malloc(strlen(TargetDir)+strlen(DirName)+10);
		sprintf(tgt,"%s/%s",TargetDir,DirName);
	}
	else
	{
		src = strdup(SourceDir);
		tgt = strdup(TargetDir);
	}
	bool SwitchedDirectory=false;
	bool FilesPut=false;
	strcpy(f.Filename,src);
	HANDLE h = GetFirstFile(src,&f);

#ifdef WIN32
	if (h == INVALID_HANDLE_VALUE) return;	// Source Directory does not exist
#else
	if (h == NULL) return;	// Source Directory does not exist
#endif
	
	META_DATA *MetaData = GetMetaData(src,META_FILE_NAME,ParentMetaData);

	do
	{	
		if (f.Directory)
		{
			if (f.Filename[0]=='.' && (f.Filename[1]=='\0' || (f.Filename[1]=='.' && f.Filename[2]=='\0')))
			{
				// Dot or dot-dot - ignore
			}
			else
			{
				if (!Ignore(f.Filename,MetaData))
				{
					Iterate(src,tgt,f.Filename,MetaData);
					SwitchedDirectory=false;
				}
			}
		}
		else
		{
			if (!SwitchedDirectory)
			{
				if (protocol_lcd(src) == 0)
				{
					// lcd failed - this shouldn't happen
					throw(protocol_return_last_error());
				}
				SwitchDirAndCreateIfNecessary(tgt);
				SwitchedDirectory=true;
			}

			if (STRCASECMP(f.Filename,META_FILE_NAME) && !Ignore(f.Filename,MetaData))
			{
				//
				// Do not transfer the Meta Data file or any file marked as ignored
				//
				char *ErrorString;
				FILEDATA *FileData = GetFileData(src,f.Filename,MetaData);
				char *TransferMode = (char *)0;
				char *OrigDirectory = (char *)0;
				if (FileData->TransferMode == TRANSFER_MODE_TEXT)	// And protocol = SFTP
				{
					//
					// We've had to translate the file - switch to the temporary directory containing
					// the source file.
					//
					OrigDirectory = protocol_pwd();
					char *TempDirectory = GetDirectoryName(FileData->SourceFileName);
					int res = protocol_lcd(TempDirectory);
					free(TempDirectory);
					TransferMode="Text";
				}
				else
				{
					TransferMode="Binary";
				}

				if (protocol_put(f.Filename) == 0)
				{
					ErrorString = protocol_return_last_error();
				}
				else
				{
					ErrorString = (char *)0;
					if (FileData->MBR_Text || FileData->MBR_Type)
					{
						// AS400 type/text information has been captured for this file.
						printf("Just put [%s] Type=[%s] Text=[%s]\n",f.Filename,
							FileData->MBR_Type?FileData->MBR_Type:"NULL",
							FileData->MBR_Text?FileData->MBR_Text:"NULL");
						if (protocol_set_attributes)
						{
							printf("About to call protocol_set_attributes tgt=[%s] f.Filename=[%s]\n",tgt,f.Filename);
							protocol_set_attributes(tgt,f.Filename,FileData->MBR_Type,FileData->MBR_Text);
						}
					}
				}

				if (OrigDirectory)
				{
					int res = protocol_cd(OrigDirectory);
				}


				printf("<file>\n");
				printf("  <sourcedir>%s</sourcedir>\n",src);
				printf("  <targetdir>%s</targetdir>\n",tgt);
				printf("  <filename>%s</filename>\n",f.Filename);
				printf("  <size>%ld</size>\n",FileData->FileSize);
				printf("  <mode>%s</mode>\n",TransferMode);
				printf("  <error>%c</error>\n",ErrorString?'Y':'N');
				printf("  <errortext>%s</errortext>\n",ErrorString?ErrorString:"");
				printf("  <md5>%s</md5>\n",FileData->md5string);
				printf("</file>\n");

				free(FileData);

				FilesPut=true;
			}
		}
		strcpy(f.Filename,src);
	}
	while (GetNextFile(h,&f));
	if (!FilesPut)
	{
		if (CreateEmptyDirectories)
		{
			SwitchDirAndCreateIfNecessary(tgt);
		}
	}
	// printf("Leaving iterate, cd back to [%s]\n",tgt);
	// protocol_cd(tgt);
	free(src);
	free(tgt);
	FreeMetaData(MetaData);
	
}

int DeleteFiles(char *filelist)
{
	// Reads a list of files in "filelist' and deletes them
	FILE *in = fopen(filelist,"r");
	if (in)
	{
		// Could open file containing list of files okay
		char linebuf[2048];
		while (!feof(in))
		{
			char *t = fgets(linebuf,sizeof(linebuf),in);
			if (t && strlen(t))
			{
				char sepchar='/';
				char *eol=&(t[strlen(t)-1]);
				while (*eol=='\n' || *eol=='\r') *eol--='\0';
				if (strlen(t)==0) continue;
				// Split into path and filename
				char *fn = &(t[strlen(t)-1]);
				while (*fn!='/' && *fn!='\\') fn--;
				*fn++='\0';
				// printf("t=[%s] fn=[%s]\n",t,fn);
				int res = protocol_cd(t);
				if (res == 1)
				{
					// Successfully switched directory
					// remove file
					res = protocol_delete(fn);
					if (res == 1)
					{
						if (strchr(t,'\\')) sepchar='\\';	// Windows sepchar
						printf("INFO: Removed file %s%c%s\n",t,sepchar,fn);
					}
				}
				else
				{
					printf("WARNING: Could not switch to directory \"%s\" to remove file \"%s\"\n",t,fn);
				}
			}
		}
	}
	else
	{
		throw("ERROR: Could not open dellist file");
		return 1;
	}
	return 0;
}

int dmmain(int argc,char **argv)
{
	
	int NumOptions = sizeof(options)/sizeof(options[0]);
	int p = ScanOptions(options,NumOptions,argc,argv);
	int ret=0;
	int z=0;
	char copyexec[20] = {"-copyexec"};
	char md5sum_path[10000] = {""};

	// printf("argc-p=%d\n",argc-p);
	char **ExecARGV = (char **)calloc(sizeof(char *),(argc-p)+1);
	try
	{
		char *Execute = getenv("TRIEXECUTE");
		char *CopyExec = getenv("TRICOPYEXEC");
		char *Protocol = getenv("TRIPROTOCOL");
		char *Server = getenv("TRISERVER");
		char *SourceDir = getenv("TRISOURCEDIR");
		char *TargetDir  = getenv("TRITARGETDIR");
		char *UserName = getenv("TRIUSERNAME");
		char *Password = getenv("TRIPASSWORD");
		char *Target = getenv("TRITARGET");
		char *DeleteFileList = getenv("TRIDELLIST");
		char *CheckBaseDir = getenv("TRICHECKBASE");
		char *Shell = getenv("TRISHELL");
		char *Port = getenv("TRIPORTNUM");
		char *Home = getenv("TRIHOME");
		char *Keyfile = getenv("TRIKEYFILE");

		if (!Password) Password = "";

		if (Home)
		{
		 sprintf(md5sum_path,"%s/lib/md5sum.exe",Home);	
		}

		if (!Execute && !CopyExec && !DeleteFileList && !CheckBaseDir)
		{
			// Must be file transfer - need source, target and line endings
			if (!Target || !(Target[0])) throw(Usage);
			if (!SourceDir || !(SourceDir[0])) throw(Usage);
			if (!TargetDir || !(TargetDir[0])) throw(Usage);
			if (STRCASECMP(Target,"win")==0) TargetLineEndings = TARGET_WINDOWS;
			else
			if (STRCASECMP(Target,"unix")==0) TargetLineEndings = TARGET_UNIX;
			else
			if (STRCASECMP(Target,"off")==0) TargetLineEndings = TARGET_NONE;
			else
			if (STRCASECMP(Target,"mac")==0) TargetLineEndings = TARGET_MAC;
			else throw(Usage);
		}
		else
		{
			if (Execute || CopyExec)
			{
				// -exec or -copyexec
				ExecARGV[0] = strdup(argv[p]);
				// printf("ExecARGV[0]=[%s]\n",ExecARGV[0]);
				for (int i=p+1;i<argc;i++)
				{
					// printf("ExecARGV[%d]=[%s]\n",(i-p),argv[i]);
					ExecARGV[i-p]=strdup(argv[i]);
				}
				ExecARGV[argc-p]=(char *)0;
				// printf("ExecARGV[%d]=null\n",argc-p);
			}
			else
			if (!DeleteFileList && !CheckBaseDir) throw(Usage);	// If not deletefilelist and not checkbasedir then throw usage error
		}
		if (!Protocol || !(Protocol[0])) throw(Usage);
		if (!Server || !(Server[0])) throw(Usage);
		if (Port && atoi(Port)==0) throw(Usage);	// Invalid port number

		// We can do without username and password but only if we're connecting locally
//		if (STRCASECMP(Server,"localhost") && (!UserName || (!Password && !Keyfile))) throw(Usage);
		if (!Execute && !CopyExec && !DeleteFileList && !CheckBaseDir)
		{
			// Copy source to target
			if (((TargetDir[0]>='a' && TargetDir[0]<='z') || (TargetDir[0]>='A' && TargetDir[0]<='Z')) && TargetDir[1]==':')
			{
				// Windows (drive letter followed by :)
				if (TargetDir[2]!='\\')
				{
					char *t = getenv("TRITARGETDIR");
					TargetDir = (char *)malloc(strlen(TargetDir)+10);
					sprintf(TargetDir,"%c:\\%s",t[0],&(t[2]));
				}
			}
			else
			if (TargetDir[0]!='/' && TargetDir[0]!='\\')
			{
				TargetDir = (char *)malloc(strlen(TargetDir)+20);
				sprintf(TargetDir,"/%s",getenv("TRITARGETDIR"));
			}
		}
		//
		// All parameters present and correct - try initialising protocol
		//
		LoadProtocolLibrary(Protocol,TargetDir);	// This will exit with error if any issues detected
		//
		// If we get here then library for the specified protocol is loaded and initialised.
		//
		protocol_register_callback(CallbackFunction);	// Called during various log-in/transfer operations

		//
		// Connect to Target Server
		//
		if (getenv("TRIDEBUG")) printf("About to connect, Server=[%s] UserName=[%s] Password=[%s]\n",Server?Server:"NULL",UserName?UserName:"NULL",Password?Password:"NULL");
		if (Keyfile) {
			Password = (char *)malloc(strlen(Keyfile)+10);
			sprintf(Password,"%c%s",'\1',Keyfile);
		}
		int i = protocol_connect(Server,UserName,Password);

		if (getenv("TRIDEBUG")) printf("protocol_connect returns %d\n",i);
		if (i <= 0)
		{
			ret=1;
			throw(protocol_return_last_error());
		}
		
		while(ExecARGV[z] != NULL)
		{
		 if (strcmp(ExecARGV[z],"md5sum") == 0)
		 {
		  Execute = NULL;
		  CopyExec = copyexec;
		  ExecARGV[z] = md5sum_path;
		  break;	 	
		 }	
		 z++;	
		}
		
		
		if (Execute)
		{
			char *res = protocol_exec(ExecARGV,Shell,false);
			ret = (int)((long)res);
		}
		else
		if (CopyExec)
		{
			char *res  = protocol_exec(ExecARGV,Shell,true);
			ret = (int)((long)res);
		}
		else
		if (CheckBaseDir)
		{
			if (p < argc) {
				// base directory supplied
				if (STRCASECMP(Server,"localhost")==0) {
					// Check cd to localhost
					ret=(chdir(argv[p])==0)?0:2;
					if (ret==2) printf("Base directory \"%s\" is invalid or inaccessible\n",argv[p]?argv[p]:"null");
				} else {
					if (protocol_cd(argv[p])) {
						// Success - base directory is valid
						printf("Base directory \"%s\" is valid\n",argv[p]?argv[p]:"null");
						ret = 0;
					} else {
						// Failure - base directory is invalid
						printf("Base directory \"%s\" is invalid or inaccessible\n",argv[p]?argv[p]:"null");
						ret = 2;	// to differentiate between connection failure (which is exit 1)
					}
				}
			} else {
				ret = 3;	// no base directory
				printf("Base Directory not specified\n");
			}
		}
		else
		if (DeleteFileList)
		{
			ret = DeleteFiles(DeleteFileList);
		}
		else
		{
			printf("<files>\n");
			Iterate(SourceDir,TargetDir);
			printf("</files>\n");
		}
	}
	catch(const char *ErrorString)
	{
		if (ErrorString)
		{
			ExitWithError((char *)ErrorString);
		}
		else
		{
			ExitWithError((char *)"No Error String");
		}
	}
	return ret;
}

#ifdef WIN32

int wstrlen(_TCHAR * wstr)
{
	int l_idx = 0;
	while (((char*)wstr)[l_idx]!=0) l_idx+=2;
	return l_idx;
}


// Allocate char string and copy TCHAR->char->string
char *wstrdup(_TCHAR * wSrc)
{
	int l_idx=0;
	int l_len = wstrlen(wSrc);
	char *l_nstr = (char*)malloc(l_len+1);
	if (l_nstr)
	{
		do
		{
			l_nstr[l_idx] = (char)wSrc[l_idx];
			l_idx++;
		} while ((char)wSrc[l_idx]!=0);
	}
	l_nstr[l_idx] = 0;
	return l_nstr;
}

// allocate argn structure parallel to argv
// argn must be released

char ** allocate_argn (int argc, _TCHAR* argv[])
{
	char ** l_argn = (char **)malloc(argc * sizeof(char*));
	for (int idx=0; idx<argc; idx++)
	{
		l_argn[idx] = wstrdup(argv[idx]);
	}
	return l_argn;
}

void release_argn(int argc, char ** nargv)
{
	for (int idx=0; idx<argc; idx++)
	{
		free(nargv[idx]);
	}
	free(nargv);
}

int _tmain(int argc, _TCHAR* argv[])
{
	char **argn = allocate_argn(argc, argv);
	int res = dmmain(argc,argn);
	release_argn(argc, argn);
	return res;
}
#else
int main(int argc,char **argv)
{
	FILE *fp=fopen("/tmp/trans.txt","a");
	if (fp) {
		int i=0;
		fprintf(fp,"\ndmtransfer ");
		for (i=1;i<argc;i++)
		{
		 fprintf(fp,"\"%s\" ",argv[i]);	
		}
		fprintf(fp,"\n");
		fclose(fp);
	}
	return dmmain(argc,argv);
}
#endif

