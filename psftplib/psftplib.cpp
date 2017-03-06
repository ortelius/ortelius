// psftplib.cpp : Defines the exported functions for the DLL application.
//

#ifdef _WINDOWS
#include "stdafx.h"
#include <Commdlg.h>
#include <io.h>
#else
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define _strdup strdup

#endif
extern "C" {
#include "putty.h"
#include "psftp.h"
}
#include "psftplib.h"
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//
// These are in psftp.c
//
extern char *pwd;
extern Config cfg;

char *m_HostName   = (char *)0;
char *m_UserName = (char *)0;
char *m_Password = (char *)0;
int m_portnum = 0;
bool m_usetty = false;

CB_FUNCTION GlobalCallbackFunction = (CB_FUNCTION)0;

//
// Called from various sources to call back to dmtransfer on certain events
// WINCONS.C
// UXCONS.C
//
int InvokeCallback(CALLBACK_REASON res,void *lParam,void *wParam)
{
	return GlobalCallbackFunction(res,lParam,wParam);
}

char *GetBaseDirectory()
{
#ifdef WIN32
	static char ModuleFileName[1024];
	GetModuleFileName(NULL,ModuleFileName,sizeof(ModuleFileName));
	for (int x=strlen(ModuleFileName)-1;x>0;x--)
	{
		if (ModuleFileName[x]=='\\')
		{
			ModuleFileName[x]='\0';
			break;
		}
	}
	return ModuleFileName;
#else
	// Unix/Linux version - since we're invoked from Trilogy, get the TRILOGYHOME as the base
	char *basedir=getenv("TRILOGYHOME");
	if (!basedir)
	{
		char* buf=(char*)malloc(1000);
		char* ret=getcwd(buf,1000);
		if (ret) {
			basedir=ret;
		}
	} 
	return basedir;
#endif
}


int protocol_initialise(int initcode)
{
	sk_init();
    do_defaults(NULL, &cfg);
	// flags |= FLAG_VERBOSE;
	// flags |= FLAG_STDERR;
	if (initcode>0) {
		m_portnum = initcode & 0xffff;	// bottom 16 bits is port number
		if (initcode > 0xffff) {
			// Passing an initcode with anything in the top 16 bits sets use tty flag.
			m_usetty=true;
		}
	}
	return 0;
}

int protocol_connect(char *HostName,char *UserName,char *Password)
{
	//
	// do_sftp in psftp.c is where all the action is
	//
	char szPortNum[128];
	struct sftp_command cmd;
	int n = (m_portnum>0)?3:2;
	cmd.nwords=n;
	cmd.words = (char **)malloc(n*sizeof(char *));
	cmd.words[0]=strdup("open");
	cmd.words[1]=strdup(HostName);
	if (m_portnum>0) {
		sprintf(szPortNum,"%d",m_portnum);
		cmd.words[2]=strdup(szPortNum);
	}

	SetSSHUserName(UserName);
	SetSSHPassword(Password);

	m_UserName = UserName;
	m_Password = Password;
	m_HostName = _strdup(HostName);
	// Horrible kludge to pass private key file through.
	// If we're using a keyfile, pass (char)1 followed by
	// the keyfile.
	if (Password[0]=='\1') {
		cfg.keyfile = filename_from_str(&(Password[1]));
	}

	return sftp_cmd_open(&cmd);
}

char *protocol_return_last_error()
{
	return ReturnErrorPtr();
}

int protocol_push(char *FromDir,char *ToDir,char *Pattern)
{
	return 0;
}

int protocol_pull(char *FromDir,char *ToDir,char *Pattern)
{
	return 0;
}

int protocol_disconnect()
{
	return 0;
}

int Execute(char **argv,bool CopyFromLocal,char *execstr)
{
	// printf("Execute(%s)\n",execstr);
#ifdef WIN32
	// Windows version
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	GetStartupInfo(&si);

	si.dwFlags=STARTF_USESTDHANDLES;
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (CopyFromLocal)
	{
		SECURITY_ATTRIBUTES sec; 
		sec.bInheritHandle=TRUE; 
		sec.lpSecurityDescriptor=NULL;
		si.hStdInput = CreateFile(argv[0],GENERIC_READ,FILE_SHARE_READ,&sec,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL); 
		if (si.hStdInput == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr,"Failed to open file \"%s\" for read\n",argv[0]);
			exit(1);
		}
	}
	else
	{
		si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	}
 
	// printf("execstr=[%s]\n",execstr);

	BOOL createRet = CreateProcess(NULL, 
	  execstr,				// command line 
	  NULL,					// process security attributes 
	  NULL,					// primary thread security attributes 
	  TRUE,					// handles are inherited 
	  0,					// creation flags 
	  NULL,					// use parent environment 
	  NULL,					// use parent's current directory 
	  &si,					// STARTUPINFO pointer 
	  &pi);					// receives PROCESS_INFORMATION

	if(!createRet) 
	{
		int res=GetLastError();
		fprintf(stderr,"Execute - Couldn't create process, %s",execstr);
		exit(1);
	}
	DWORD WaitResult=WaitForSingleObject(pi.hProcess, INFINITE);
	DWORD exitCode;
	DWORD result = GetExitCodeProcess(pi.hProcess, &exitCode);
#else
	// Linux/Unix version
	unsigned char buf[4096];
	FILE *p = popen(execstr,"w");
	FILE *in = fopen(argv[0],"r");
	while (!feof(in)) {
		int n = fread(buf,sizeof(buf),1,in);
		fwrite(buf,n,1,p);
	}
	fclose(in);
	int exitCode;
	exitCode = pclose(p);
#endif
	return (int)exitCode;
}

char *protocol_exec(char **argv,char *shell,bool CopyFromLocal)
{
	bool executeWithTTY=false;
	char *basedir = GetBaseDirectory();
	//
	// Get length of all the parameters
	//
	int n=0;
	int plen=0;
	while (argv[n])
	{
		plen+=(strlen(argv[n])+4);
		n++;
	}
#ifdef WIN32
	const char *sshexec = "\\sshexec.bat";	// temp
#else
	const char *sshexec = "/sshexec";
#endif
	
	char *execstr = (char *)malloc(strlen(basedir)+plen+strlen(m_HostName)+strlen(m_UserName)+strlen(m_Password)+(strlen(argv[0])*3)+200);
	// printf("Malloced space for execstr=%d\n",strlen(basedir)+plen+strlen(m_HostName)+strlen(m_UserName)+strlen(m_Password)+(strlen(argv[0])*3)+200);
	char portCmd[128];
	if (m_portnum > 0) {
		// Default port has changed
		sprintf(portCmd," -port %d ",m_portnum);
	} else {
		portCmd[0]='\0';
	}

	char *p = strdup(argv[0]);
	char *p1 = p;
	char *cmd=(char *)0;
	while (*p)
	{
		if (*p=='/' || *p=='\\') cmd=p+1;
		p++;
	}
	if (cmd == (char *)0) cmd = p1;

	bool quoteit=false;
	bool addCloseQuote=false;
	bool noMoreQuotes=false;

	char *qc="";
	char singletick[] = "'";
	char emptystr[] = "\0";

	
	if (CopyFromLocal)
	{
		// If we're allocating a terminal then we have to copy the file as a separate operation
		if (shell) {
			quoteit = (strcmp(shell,"echo")==0);	// true if shell is "echo" command
			qc=quoteit?singletick:emptystr;
			if (m_usetty) {
				char *FmtStr = "%s%s -force -m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" sh -c \"cat - | tr -d '\\015' >'/tmp/%s'";
				sprintf(execstr,FmtStr,basedir,sshexec,m_HostName,portCmd,m_UserName,m_Password,cmd);
				executeWithTTY=true;
			} else {
				char *FmtStr = "%s%s -force -m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" sh -c \"cat - | tr -d '\\015' >'/tmp/%s';%s %s'/tmp/%s'";
				sprintf(execstr,FmtStr,basedir,sshexec,m_HostName,portCmd,m_UserName,m_Password,cmd,shell,qc,cmd);
			}
		} else {
			if (m_usetty) {
				char *FmtStr = "%s%s -force -m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" sh -c \"cat - | tr -d '\\015' >'/tmp/%s';chmod +x '/tmp/%s'";
				sprintf(execstr,FmtStr,basedir,sshexec,m_HostName,portCmd,m_UserName,m_Password,cmd,cmd,cmd);
				executeWithTTY=true;
			} else {
				char *FmtStr = "%s%s -force -m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" sh -c \"cat - | tr -d '\\015' >'/tmp/%s';chmod +x '/tmp/%s';'/tmp/%s'";
				sprintf(execstr,FmtStr,basedir,sshexec,m_HostName,portCmd,m_UserName,m_Password,cmd,cmd,cmd);
			}
		}
		if (m_usetty) strcat(execstr,"\"");
		// printf("execstr=[%s] length=%d\n",execstr,strlen(execstr));
	}
	else
	{
		char dashT[] = "-t ";
		char emptystr[] = "\0";
		char *tflag = m_usetty?dashT:emptystr;
		if (shell) {
			char *FmtStr = "%s%s -force %s-m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" \"%s %s'%s'";
			sprintf(execstr,FmtStr,basedir,sshexec,tflag,m_HostName,portCmd,m_UserName,m_Password,shell,qc,argv[0]);
			addCloseQuote=true;
		} else {
			char *FmtStr = "%s%s -force %s-m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" '%s'";
			sprintf(execstr,FmtStr,basedir,sshexec,tflag,m_HostName,portCmd,m_UserName,m_Password,argv[0]);
		}
	}

	if (executeWithTTY) {
		// We should already have created the copy structure. No need to add the parameters for
		// this bit - just execute it
		int ec = Execute(argv,CopyFromLocal,execstr);
		//
		// Now construct the command to execute with terminal. Note, no need for -force since we'll
		// have already executed a command against this server now to copy the file to the target
		//
		if (shell) {
			char *FmtStr = "%s%s -t -m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" \"%s\" %s'/tmp/%s'";
			sprintf(execstr,FmtStr,basedir,sshexec,m_HostName,portCmd,m_UserName,m_Password,shell,qc,cmd);
		} else {
			char *FmtStr = "%s%s -t -m \"%s\"%s -syn -usr \"%s\" -pw \"%s\" '/tmp/%s'";
			sprintf(execstr,FmtStr,basedir,sshexec,m_HostName,portCmd,m_UserName,m_Password,cmd);
		}
	}

	for (int i=1;i<n;i++)
	{
		// printf("argv[%d]=[%s]\n",i,argv[i]);
		// Some bash interpreters seem to struggle during a copyexec with zero-length
		// parameters ("") but are happy if they're single quoted (''). 
		char *ap = argv[i];
		if (*ap=='|' /* && quoteit */) {
			// Pipe char - we're trying to pipe the result - can we even do this remotely?
			strcat(execstr," | ");
			ap++;
			quoteit=false;
			noMoreQuotes=true;
		}
		//strcat(execstr,(CopyFromLocal && strlen(argv[i])==0)?" \'":" \"");
		strcat(execstr,noMoreQuotes?" ":" '");
		strcat(execstr,ap);
		if (execstr[strlen(execstr)-1]=='\\') strcat(execstr,"\\");
		if (!noMoreQuotes) strcat(execstr,"'");
		//strcat(execstr,(CopyFromLocal && strlen(argv[i])==0)?"\' ":"\" ");
	}
	//if (!m_usetty && CopyFromLocal) strcat(execstr,"\"");
	if (addCloseQuote) strcat(execstr,"\"");
	if (quoteit) strcat(execstr,qc);

	char *szDebug = getenv("OMDEBUG");
	bool dbg=(szDebug && tolower(szDebug[0])=='y');
	
	if (dbg) printf("execstr=[%s]\n",execstr);

	int exitCode = Execute(argv,CopyFromLocal,execstr);

	free(execstr);
	free(p1);
	return (char *)exitCode;
}

char *protocol_pwd()
{
	return pwd;
}

int protocol_cd(char *NewDir)
{
	struct sftp_command cmd;
	cmd.nwords=2;
	cmd.words = (char **)malloc(2*sizeof(char *));
	cmd.words[0]=strdup("cd");
	cmd.words[1]=strdup(NewDir);
	return sftp_cmd_cd(&cmd);
}

void protocol_register_callback(CB_FUNCTION cbf)
{
	GlobalCallbackFunction = cbf;
}

int protocol_lcd(char *NewDir)
{
	struct sftp_command cmd;
	cmd.nwords=2;
	cmd.words = (char **)malloc(2*sizeof(char *));
	cmd.words[0]=strdup("lcd");
	cmd.words[1]=strdup(NewDir);
	return sftp_cmd_lcd(&cmd);
}

int protocol_mkdir(char *Dir)
{
	struct sftp_command cmd;
	cmd.nwords=2;
	cmd.words = (char **)malloc(2*sizeof(char *));
	cmd.words[0]=strdup("mkdir");
	cmd.words[1]=strdup(Dir);
	return sftp_cmd_mkdir(&cmd);
}

int protocol_put(char *Filename)
{
	struct sftp_command cmd;
	cmd.nwords=2;
	cmd.words = (char **)malloc(2*sizeof(char *));
	cmd.words[0]=strdup("put");
	cmd.words[1]=strdup(Filename);
	return sftp_cmd_put(&cmd);
}

int protocol_get(char *Filename)
{
	struct sftp_command cmd;
	cmd.nwords=2;
	cmd.words = (char **)malloc(2*sizeof(char *));
	cmd.words[0]=strdup("get");
	cmd.words[1]=strdup(Filename);
	return sftp_cmd_get(&cmd);
}

int protocol_delete(char *Filename)
{
	struct sftp_command cmd;
	cmd.nwords=2;
	cmd.words = (char **)malloc(2*sizeof(char *));
	cmd.words[0]=strdup("del");
	cmd.words[1]=strdup(Filename);
	return sftp_cmd_rm(&cmd);
}


