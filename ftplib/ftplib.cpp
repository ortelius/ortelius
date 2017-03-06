// ftplib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ftpcomm.h"
#include "ftplib.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>
#define chdir(x) _chdir(x)
#define open _open
#define read _read
#define O_RDONLY _O_RDONLY
#define LAST_ERROR GetLastError()
#else
#define LAST_ERROR errno
#define _O_BINARY 0
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define GetErrorString(LAST_ERROR) strerror(errno)
#endif

bool bSecure=false;
bool bEncryptData=false;
FTPSOCKET *ftpsock = (FTPSOCKET *)0;
bool bTextMode=false;

#ifdef WIN32
void InitialiseWinsock()
{
	WORD		wVersionRequested=MAKEWORD(2,2);	// version 2.2
	WSADATA		wsadata;

	int res;
	res=WSAStartup(wVersionRequested,&wsadata);
}

void CloseWinsock()
{
	WSACleanup();
}

char *GetErrorString(DWORD errnum)
{
	static LPSTR res;
	static char buf[1024];
	res = (LPSTR)buf;
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,0,errnum,0,res,sizeof(buf),NULL);
	//
	// Remove any trailing \n chars
	//
	char *p = &(buf[strlen(buf)-1]);
	while (*p=='\n') *p-- = '\0';
	return (char *)res;
}

#endif

int protocol_initialise(void *Data)
{
#ifdef WIN32
	InitialiseWinsock();
#endif
	if (Data)
	{
		bSecure = true;
		bEncryptData = true;
	}
	else
	{
		bSecure = false;
		bEncryptData = false;
	}
	return 0;
}

int protocol_connect(char *HostName,char *UserName,char *Password)
{
	ftpsock = FTP_Connect(HostName,UserName,Password,bSecure,bEncryptData);
	// return ftpsock->control.trisocket?1:0;	// 1 = success, 0 = failure
	if (ftpsock->Result != FTP_OKAY)
	{

	}
	return ftpsock->Result == FTP_OKAY?1:0;
}

char *protocol_return_last_error()
{
	return ReturnErrorPtr();
}


int protocol_disconnect()
{
	return 0;
}

char *protocol_exec(char **argv,char *shell,bool CopyFromLocal)
{
	SERVERREPLY	t;
	// Assemble command
	int n=0;
	int plen=0;
	while (argv[n])
	{
		plen+=(strlen(argv[n])+4);
		n++;
	}
	if (shell) plen+=(strlen(shell)+4);
	char *execstr = (char *)malloc(plen);
	execstr[0]='\0';
	if (shell) strcat(execstr,shell);
	for (int t=0;t<n;t++) {
		if (t>0 || shell) strcat(execstr," ");
		strcat(execstr,argv[t]);
	}


	if (ftpsock->SystemType == FTP_SYSTEM_AS400)
	{
		// AS400 is peculiar in that it uses RCMD
		t=FTP_SendCommandToServer(ftpsock,"RCMD",execstr);
	}
	else
	{
		// Other systems uses SITE EXEC, but be prepare to have this denied
		t=FTP_SendCommandToServer(ftpsock,"SITE EXEC",execstr);
	}
	// printf("t.replytext=[%s]\n",t.ReplyText);
	free(t.ReplyText);	// RHT 10/08/2011 - free immediately as not used
	int res = 0;
	if (t.ReplyCode < 200 || t.ReplyCode > 299)
	{
		// failed
		res=1;
	}
	return (char *)res;
}

int protocol_cd(char *NewDir)
{
	SERVERREPLY x = FTP_SwitchDirectory(ftpsock,NewDir);
	if (x.ReplyCode >= 200 && x.ReplyCode <= 299)
	{
		// Success
		return 1;
	}
	else
	{
		SetGlobalErrorPtr(x.ReplyText);
		return 0;
	}
}

char *protocol_pwd()
{
	return FTP_GetCurrentDirectory(ftpsock);
}


void protocol_register_callback(CB_FUNCTION cbf)
{
}


int protocol_lcd(char *NewDir)
{
	if (chdir(NewDir) == 0)
	{
		// Success
		return 1;
	}
	else
	{
		// Failure
		SetGlobalErrorPtr(strerror(LAST_ERROR));
		return 0;
	}
}

void protocol_text()
{
	bTextMode=true;
}

void protocol_binary()
{
	bTextMode=false;
}

int protocol_mkdir(char *Dir)
{
	SERVERREPLY x = FTP_MakeDirectory(ftpsock,Dir);
	if (x.ReplyCode>=200 && x.ReplyCode<=299)
	{
		// Success
		return 1;
	}
	else
	{
		SetGlobalErrorPtr(x.ReplyText);
		return 0;
	}
}

int protocol_put(char *Filename)
{
	//
	// Open the file
	//
	bool BrowseMode=true;
	bool ReplaceReadOnlyFiles=true;
	bool Translate=true;
	struct stat s;
	int res = stat(Filename,&s);
	if (res == 0)
	{
		char *FileData = (char *)malloc(s.st_size);
		int in = open(Filename,O_RDONLY | _O_BINARY );
		if (in != -1)
		{
			res = read(in,FileData,s.st_size);
			if (res == s.st_size)
			{
				SERVERREPLY x = FTP_CreateFile(ftpsock,Filename,FileData,s.st_size,bTextMode,BrowseMode,ReplaceReadOnlyFiles,Translate);
				free(FileData);
				if (x.ReplyCode>=100 && x.ReplyCode<=299)
				{
					// Success
					return 1;
				}
				else
				{
					SetGlobalErrorPtr(x.ReplyText);
					return 0;
				}
			}
			else
			{
				SetGlobalErrorPtr(GetErrorString(LAST_ERROR));
				return 0;
			}
		}
		else
		{
			SetGlobalErrorPtr(GetErrorString(LAST_ERROR));
			return 0;
		}
	}
	else
	{
		SetGlobalErrorPtr(GetErrorString(LAST_ERROR));
		return 0;
	}
	return 0;
}

int protocol_get(char *Filename)
{
	unsigned char *FileData;
	unsigned long DataLen;
	int binflag = _O_BINARY;
	int retcode=1;

	SERVERREPLY x = FTP_GetFileData(ftpsock,bTextMode,Filename,&FileData,&DataLen);
	if (x.ReplyCode>=100 && x.ReplyCode<=299)
	{
		// Success
		int out = open(Filename,O_WRONLY | binflag | O_CREAT | O_TRUNC );
		if (out != -1)
		{
			int res = write(out,FileData,DataLen);
			if (res != DataLen)
			{
				SetGlobalErrorPtr(GetErrorString(LAST_ERROR));
				retcode=0;
			}
			close(out);
		}
	}
	else
	{
		// Failed to get file
		SetGlobalErrorPtr(x.ReplyText);
		retcode=0;
	}
	free(FileData);
	return retcode;
}

int protocol_delete(char *Filename)
{
	int retcode=1;
	SERVERREPLY x = FTP_DeleteFile(ftpsock,Filename);
	if (x.ReplyCode<100 || x.ReplyCode>=300)
	{
		// Failed to delete file
		SetGlobalErrorPtr(x.ReplyText);
		retcode=0;
	}
	return retcode;
}

static char **internal_get_files(char *Pattern,bool Recursive, char ***res, int *n)
{
	// printf("internal_get_files *res = 0x%lx *n=%d\n",*res,*n);
	if (!*res) {
		*res = (char **)malloc(sizeof(char *));
		(*res)[0]=(char *)0;
	}
	FTP_FILE_LIST *d = (FTP_FILE_LIST *)0;
	if (Recursive) d = FTP_GetDirectoryContents(ftpsock);
	FTP_FILE_LIST *f = FTP_GetDirectoryContents(ftpsock,Pattern);
	FTP_FILE_LIST *c = f;
	char *curdir = FTP_GetCurrentDirectory(ftpsock);
	while (c) {
		if (!c->Directory) {
			// Add the directory name and the file name to the returned list
			char *fp = (char *)malloc(strlen(curdir)+strlen(c->FileName)+10);
			sprintf(fp,(curdir[strlen(curdir)-1]=='/')?"%s%s":"%s/%s",curdir,c->FileName);		
			(*n)++;
			*res = (char **)realloc(*res,sizeof(char *)*((*n)+1));
			(*res)[(*n)-1]=fp;
			(*res)[*n]=(char *)0;
		}
		c=c->next;
	}
	free(curdir);
	// recurse through the directories (if any)
	c = d;
	while (c) {
		if (c->Directory) {
			// This is a directory, recurse and look for files
			if (protocol_cd(c->FileName)) {
				internal_get_files(Pattern,true,res,n);
			}
		}
		c=c->next;
	}
	return *res;
}

char **protocol_get_file_list(char *Pattern,bool Recursive)
{
	char **res = (char **)0;
	int n=0;
	return internal_get_files(Pattern,Recursive,&res,&n);
}

char **protocol_get_attributes(char *folder)
{
	return GetExtendedAttributes_iSeries(ftpsock,folder);
}

void protocol_set_attributes(char *folder,char *filename,char *mbrtype,char *mbrtext)
{
	return SetExtendedAttributes_iSeries(ftpsock,folder,filename,mbrtype,mbrtext);
}
