// psftplib-win.cpp : Defines the exported functions for the DLL application.
// this is only for windows. For Unix/Linux ports see psftplib-unix.cpp
// Note, this code calls psftp for file transfer operations

#include "stdafx.h"
#include <io.h>
#include <process.h>
#include "psftplib.h"
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Following defines are the output messages from psftp. If the underlying psftp program
// changes then we may need to change these constants
#define HOST_KEY_NOT_CACHED "server's host key is not cached"
#define REMOTE_DIRECTORY "Remote directory is "	// note, include everything up to start of remote dir
#define ACCESS_DENIED "Access denied"

typedef struct _StdThreadInfo
{
	HANDLE	Handle;
	char	*buf;
	DWORD	datalen;
} StdThreadInfo;

char *m_HostName   = (char *)0;
char *m_UserName = (char *)0;
char *m_Password = (char *)0;
char *m_libloc = (char *)0;
int m_portnum = 22;	// default
bool m_usetty = false;
char *m_errorptr = (char *)0;
char *m_cwd = (char *)0;
bool m_usingKeyfile = false;
char *m_keyFile = (char *)0;
char *m_localDir = (char *)0;
char *m_CurrentPutList = (char *)0;

char *ReturnErrorPtr()
{
	return m_errorptr;
}

void safe_free(void *p)
{
	if (p) free(p);
}

// Version of vasprintf for Windows

int vasprintf(char **strp, const char *fmt, va_list ap)
{
	int r = -1, size = _vscprintf(fmt, ap);
	if ((size >= 0) && (size < INT_MAX)) {
		*strp = (char *)malloc(size+1); //+1 for null
		if (*strp) {
			r = vsnprintf(*strp, size+1, fmt, ap);  //+1 for null
			if (r == -1) free(*strp);
		}
	} else {
		*strp = 0;
	}
	return(r);
}

static char *last_strstr(char *haystack,char *needle)
{
    if (*needle == '\0')
        return (char *) haystack;

    char *result = NULL;
    for (;;) {
        char *p = strstr(haystack, needle);
        if (p == NULL)
            break;
        result = p;
        haystack = p + 1;
    }

    return result;
}

char *AllocateCmdLine(char *batchfile)
{
	return (char *)malloc(strlen(batchfile)+strlen(m_libloc)+strlen(m_Password)+strlen(m_UserName)+strlen(m_HostName)+128);
}

void WriteToDebugFile(const char *fmt, ...)
{
	char *msg = NULL;
	if (getenv("OMDEBUG")) {
		FILE *out = fopen(getenv("OMDEBUG"),"a+");
		if (out) {
			va_list args;
			va_start(args, fmt);
			vasprintf(&msg,fmt, args);
			fprintf(out,"%s\n",msg);
			free(msg);
			va_end(args);
			fclose(out);
		}
	}
}

char *CreateBatchFile(char *content)
{
	try {
		DWORD bytesWritten;
		TCHAR lpTempPathBuffer[MAX_PATH];
		TCHAR szTempFileName[MAX_PATH];
		DWORD dwRetVal = GetTempPath(MAX_PATH,lpTempPathBuffer);
		if (dwRetVal > MAX_PATH || dwRetVal == 0) throw "Could not get temporary path";
		UINT tf = GetTempFileName(lpTempPathBuffer,TEXT("DEPHUB"),0,szTempFileName);
		if (tf == 0) throw "Could not get temporary filename";
		HANDLE hTempFile = CreateFile((LPTSTR)szTempFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		BOOL fSuccess = WriteFile(hTempFile,content,strlen(content),&bytesWritten,NULL);
		CloseHandle(hTempFile);
		if (!fSuccess) throw "Failed to write temporary file";
		return strdup(szTempFileName);
	} catch(char *errstr) {
		WriteToDebugFile("ERROR: %s",errstr);
		safe_free(m_errorptr);
		m_errorptr = strdup(errstr);
		return (char *)0;
	}
}

unsigned int WINAPI StdInThread(PVOID arg)
{
	StdThreadInfo* info=(StdThreadInfo*)arg;
	char *buffer = info->buf;
	int BytesRemaining = buffer?strlen(buffer):0;
	DWORD bytesWritten;
	while (BytesRemaining) {
		// Data to send to standard input
		BOOL writeResult=WriteFile(info->Handle,buffer,BytesRemaining,&bytesWritten,NULL);
		if (!writeResult) break;	// error
		BytesRemaining-=bytesWritten;
		buffer=&(buffer[bytesWritten]);
	}
	CloseHandle(info->Handle);
	return 0;
}

unsigned int WINAPI StdOutThread(PVOID arg)
{
	StdThreadInfo* info = (StdThreadInfo*) arg;
	char ReadBuffer[1024];
	DWORD nb;
	info->buf = (char *)0;
	info->datalen = 0;
	while(ReadFile(info->Handle,ReadBuffer,sizeof(ReadBuffer),&nb,NULL)) {
		if (info->buf) {
			info->buf = (char *)realloc(info->buf,strlen(info->buf)+nb+1);
		} else {
			info->buf = (char *)malloc(nb+1);
		}
		memcpy(&(info->buf[info->datalen]),ReadBuffer,nb);
		info->datalen+=nb;
	}
	CloseHandle(info->Handle);
	return 0;
}

int ExecuteCommand(char *cmdline,char *charsin,char **charsout,char **errout)
{
	// Executes a command string, passing any standard input from charsin
	// and returning standard output to charsout
	char *fse = (char *)0;
	try {
		HANDLE StdOut[2];
		HANDLE StdErr[2];
		HANDLE StdIn[2];

		HANDLE ChildStdIn;
		HANDLE ChildStdOut;
		HANDLE ChildStdErr;

		SECURITY_ATTRIBUTES saErr;
		SECURITY_ATTRIBUTES saOut;
		SECURITY_ATTRIBUTES saIn;
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		int ErrNum=0;

		WriteToDebugFile("Executing Command [%s]",cmdline);

		saIn.nLength=sizeof(SECURITY_ATTRIBUTES);
		saIn.lpSecurityDescriptor=NULL;
		saIn.bInheritHandle=TRUE;
		if (!CreatePipe(&StdIn[0],&StdIn[1],&saIn,0)) throw "Couldn't create stdin pipe";
		if (!DuplicateHandle(GetCurrentProcess(),StdIn[1],GetCurrentProcess(),&ChildStdIn,0,FALSE,DUPLICATE_SAME_ACCESS)) throw "Couldn't duplicate stdin handle"; 
		CloseHandle(StdIn[1]);

		saOut.nLength=sizeof(SECURITY_ATTRIBUTES);
		saOut.lpSecurityDescriptor=NULL;
		saOut.bInheritHandle=TRUE;
		if (!CreatePipe(&StdOut[0],&StdOut[1],&saOut,0)) throw "Couldn't create stdout pipe";
		if (!DuplicateHandle(GetCurrentProcess(),StdOut[0],GetCurrentProcess(),&ChildStdOut,0,FALSE,DUPLICATE_SAME_ACCESS)) throw "Couldn't duplicate stdout handle";
		CloseHandle(StdOut[0]);

		saErr.nLength=sizeof(SECURITY_ATTRIBUTES);
		saErr.lpSecurityDescriptor=NULL;
		saErr.bInheritHandle=TRUE;
		if (!CreatePipe(&StdErr[0],&StdErr[1],&saErr,0)) throw "Couldn't create stderr pipe"; 
		if (!DuplicateHandle(GetCurrentProcess(),StdErr[0],GetCurrentProcess(),&ChildStdErr,0,FALSE,DUPLICATE_SAME_ACCESS)) throw "Couldn't duplicate stderr handle"; 
		CloseHandle(StdErr[0]);

		GetStartupInfoA(&si);
		si.dwFlags=STARTF_USESTDHANDLES;
		si.hStdOutput=StdOut[1];	// Write end of pipe for child StdOut
		si.hStdError=StdErr[1];		// Write end of pipe for child StdErr
		si.hStdInput=StdIn[0];		// Read end of pipe for child StdIn

		BOOL createRet = CreateProcess(NULL,cmdline,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
		if (!createRet) {
			DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
			int res=GetLastError();
			FormatMessage(dwFlags,NULL,res,dwLanguageId,(LPTSTR)&fse,0,NULL);
			throw fse;
		}
		// Process is now running - set threads to process stdin, stdout and stderr
		CloseHandle(pi.hThread);
		CloseHandle(StdOut[1]);
		CloseHandle(StdErr[1]);
		CloseHandle(StdIn[0]);

		StdThreadInfo inThreadInfo;
		StdThreadInfo outThreadInfo;
		StdThreadInfo errThreadInfo;

		inThreadInfo.Handle=ChildStdIn;
		inThreadInfo.buf = charsin;
		outThreadInfo.Handle=ChildStdOut;
		errThreadInfo.Handle=ChildStdErr;
	
		uintptr_t InThread=_beginthreadex(NULL,0,StdInThread,(LPVOID)&inThreadInfo,0,NULL);
		if (!InThread) throw "Failed to start inthread";
		uintptr_t OutThread=_beginthreadex(NULL,0,StdOutThread,(LPVOID)&outThreadInfo,0,NULL);
		if (!OutThread) throw "Failed to start outthread";
		uintptr_t ErrThread=_beginthreadex(NULL,0,StdOutThread,(LPVOID)&errThreadInfo,0,NULL);
		if (!ErrThread) throw "Failed to start errthread";
		// Now wait for the process to terminate
		DWORD WaitResult=WaitForSingleObject(pi.hProcess, INFINITE);
		DWORD WaitError=GetLastError();
		/*
		if (WaitResult==WAIT_TIMEOUT) {
		} else if (WaitResult==WAIT_ABANDONED) {
		}
		*/
		DWORD ExitStatus;
		GetExitCodeProcess(pi.hProcess,&ExitStatus);
		WaitForSingleObject((void *)InThread,INFINITE);
		WaitForSingleObject((void *)OutThread,INFINITE);
		WaitForSingleObject((void *)ErrThread,INFINITE);
		// At this point, all read/write threads have terminated along with the called process
		// get the output
		*charsout = outThreadInfo.datalen?(char *)malloc(outThreadInfo.datalen+1):0;
		if (*charsout) {
			memcpy(*charsout,outThreadInfo.buf,outThreadInfo.datalen);
			(*charsout)[outThreadInfo.datalen]='\0';
		}
		*errout = errThreadInfo.datalen?(char *)malloc(errThreadInfo.datalen+1):0;
		if (*errout) {
			memcpy(*errout,errThreadInfo.buf,errThreadInfo.datalen);
			(*errout)[errThreadInfo.datalen]='\0';
		}
		WriteToDebugFile("Exit Code: %d",(int)ExitStatus);
		WriteToDebugFile("---------------- STDOUT ----------------\n%s",*charsout);
		WriteToDebugFile("---------------- STDERR ----------------\n%s",*errout);
		WriteToDebugFile("----------------------------------------\n");
		free(outThreadInfo.buf);
		free(errThreadInfo.buf);
		return (int)ExitStatus;
	} catch(char *errstr) {
		WriteToDebugFile("ERROR: %s",errstr);
		safe_free(m_errorptr);
		m_errorptr = strdup(errstr);
		return 1;
	}
}

int ExecutePSFTP(char *BatchFileContent,char **charsout,char **errout)
{
	char *batchfile = CreateBatchFile(BatchFileContent);
	char *cmdline = AllocateCmdLine(batchfile);
	if (m_usingKeyfile) {
		// Passing keyfile
		sprintf(cmdline,"%s\\psftp.exe -batch -P %d -i \"%s\" -be -b \"%s\" %s@%s",m_libloc,m_portnum,m_keyFile,batchfile,m_UserName,m_HostName);
	} else {
		sprintf(cmdline,"%s\\psftp.exe -batch -P %d -pw \"%s\" -be -b \"%s\" %s@%s",m_libloc,m_portnum,m_Password,batchfile,m_UserName,m_HostName);
	}
	WriteToDebugFile("---- Batch File [%s] ----",batchfile);
	WriteToDebugFile("%s",BatchFileContent);
	WriteToDebugFile("-------------------------");
	int res = ExecuteCommand(cmdline,NULL,charsout,errout);
	unlink(batchfile);
	free(batchfile);
	free(cmdline);
	return res;
}

char *GetBaseDirectory()
{
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
	WriteToDebugFile("GetBaseDirectory returns [%s]",ModuleFileName);
	return ModuleFileName;
}

int CompareDirectories(char *dir1,char *dir2)
{
	// Compares the two directories, ignoring / and \ separators.
	char *p1=dir1;
	char *p2=dir2;
	while (*p1) {
		if ((*p1=='/' || *p1=='\\') && (*p2=='/' || *p2=='\\')) {
			p1++;
			p2++;
			continue;
		}
		if (*p1++ != *p2++) break;
	}
	return (*p1 || *p2)?0:1;
}


int protocol_initialise(int initcode)
{
	WriteToDebugFile("Initialise");
	if (initcode>0) {
		m_portnum = initcode & 0xffff;	// bottom 16 bits is port number
		if (initcode > 0xffff) {
			// Passing an initcode with anything in the top 16 bits sets use tty flag.
			m_usetty=true;
		}
	}
	m_libloc=strdup(GetBaseDirectory());
	m_CurrentPutList=(char *)0;
	return 0;
}

int protocol_connect(char *HostName,char *UserName,char *Password)
{
	int retcode=1;	// assume success unless something goes wrong
	m_UserName = strdup(UserName);
	m_Password = strdup(Password);
	m_HostName = _strdup(HostName);
	WriteToDebugFile("protocol_connect: HostName=[%s] UserName=[%s]",HostName,UserName);
	m_CurrentPutList=(char *)0;
	//
	// If we're using a keyfile, pass (char)1 followed by
	// the keyfile.
	//
	char *batchfile = CreateBatchFile("pwd\nquit\n");
	char *cmdline = AllocateCmdLine(batchfile);
	if (!batchfile) return 1;	// failed
	if (Password[0]=='\1') {
		// Passing keyfile in Password[1] onward
		m_keyFile = strdup(&(Password[1]));
		sprintf(cmdline,"%s\\psftp.exe -batch -P %d -i \"%s\" -be -b \"%s\" %s@%s",m_libloc,m_portnum,m_keyFile,batchfile,UserName,HostName);
		m_usingKeyfile = true;
	} else {
		sprintf(cmdline,"%s\\psftp.exe -batch -P %d -pw \"%s\" -be -b \"%s\" %s@%s",m_libloc,m_portnum,Password,batchfile,UserName,HostName);
		m_usingKeyfile = false;
	}
	char *charsout = (char *)0;
	char *errout = (char *)0;
	int res = ExecuteCommand(cmdline,NULL,&charsout,&errout);
	if (res != 0) {
		if (errout && strstr(errout,HOST_KEY_NOT_CACHED)) {
			WriteToDebugFile("Host key is not cached");
			free(errout);
			safe_free(charsout);
			// First time connecting to this server - reconnect and force acceptance of the key
			if (Password[0]=='\1') {
				sprintf(cmdline,"%s\\psftp.exe -P %d -i \"%s\" -b \"%s\" %s@%s",m_libloc,m_portnum,&(Password[1]),batchfile,UserName,HostName); 
			} else {
				sprintf(cmdline,"%s\\psftp.exe -P %d -pw \"%s\" -b \"%s\" %s@%s",m_libloc,m_portnum,Password,batchfile,UserName,HostName);
			}
			char *charsin="Y\n";
			res = ExecuteCommand(cmdline,charsin,&charsout,&errout);
		} else {
			// Error return for some other reason than host key not cached
			if (strstr(errout,ACCESS_DENIED)) {
				// failed to login
				m_errorptr = strdup(ACCESS_DENIED);
			}
			retcode = 0;
		}
	}
	if (retcode && errout) {
		if (strstr(errout,ACCESS_DENIED)) {
			// failed to login
			m_errorptr = strdup(ACCESS_DENIED);
			retcode=0;
		}
	}
	if (retcode && charsout) {
		// Look for the base working directory
		char *rd = last_strstr(charsout,REMOTE_DIRECTORY);
		if (rd) {
			// Found the Working Directory message
			char *t = rd+strlen(REMOTE_DIRECTORY);
			m_cwd = strdup(t);
			t=m_cwd;
			// Now terminate at the line break
			while (*t && *t!='\r' && *t!='\n') t++;
			*t='\0';
			WriteToDebugFile("Setting Current Directory to [%s]",m_cwd);
		}
		free(charsout);
	}
	unlink(batchfile);
	free(batchfile);
	free(cmdline);
	return retcode;
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
	WriteToDebugFile("protocol_disconnect()");
	if (m_CurrentPutList) {
		protocol_cd(m_cwd);	// force the remaining files to be put.
	}
	return 0;
}

int Execute(char **argv,bool CopyFromLocal,char *execstr)
{
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
	const char *sshexec = "\\sshexec.bat";
	
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
	return m_cwd;
}

int protocol_cd(char *NewDir)
{
	// In common with psftplib_unix, this will create the target directory and record the
	// new directory as the current working directory.
	WriteToDebugFile("protocol_cd(%s)",NewDir);
	// We don't know if all the elements of the target directory exist. So we'll create a batch
	// file containing all the mkdir commands up to the specified directory. The last command
	// will be a switch to that directory followed by a PWD. If the PWD equals the NewDir then
	// all is good. Note, that could get fooled by passing relative directories but dmtransfer
	// doesn't do that.
	int retcode=1;	// success until something goes wrong
	char *ts = strdup(NewDir);
	char *pdir = strtok(ts,"\\/");
	char *lastDir = (char *)0;
	char *lastPath = (char *)0;
	char *cmdChain = (char *)0;
	while (pdir) {
		if (cmdChain) {
			cmdChain = (char *)realloc(cmdChain,strlen(cmdChain)+strlen(lastDir)+strlen(pdir)+128);
			sprintf(&(cmdChain[strlen(cmdChain)]),"mkdir /%s/%s\n",lastDir,pdir);
			lastDir=(char *)realloc(lastDir,strlen(lastDir)+strlen(pdir)+16);
			strcat(lastDir,"/");
			strcat(lastDir,pdir);
		} else {
			cmdChain = (char *)malloc(strlen(pdir)+128);
			sprintf(cmdChain,"mkdir /%s\n",pdir);
			lastDir=strdup(pdir);
		}
		pdir=strtok(NULL,"\\/");
	}
	free(ts);
	// Now add the cd command to the last directory and do a PWD
	if (cmdChain && lastDir) {
		if (m_CurrentPutList) {
			// There's a list of files to store in our previous directory.
			cmdChain = (char *)realloc(cmdChain,strlen(cmdChain)+strlen(m_CurrentPutList)+strlen(lastDir)+128);
			strcat(cmdChain,m_CurrentPutList);	// put the current file list into the old dir before switching
			free(m_CurrentPutList);
			m_CurrentPutList=(char *)0;
		} else {
			cmdChain = (char *)realloc(cmdChain,strlen(cmdChain)+strlen(lastDir)+128);
		}
		strcat(cmdChain,"cd /");
		strcat(cmdChain,lastDir);
		strcat(cmdChain,"\npwd\n");
	}

	char *charsout;
	char *errout;
	int res = ExecutePSFTP(cmdChain,&charsout,&errout);
	if (res == 0 && charsout) {
		char *rd = last_strstr(charsout,REMOTE_DIRECTORY);
		if (rd) {
			// Found the Working Directory message
			char *t = rd+strlen(REMOTE_DIRECTORY);
			m_cwd = strdup(t);
			t=m_cwd;
			// Now terminate at the line break
			while (*t && *t!='\r' && *t!='\n') t++;
			*t='\0';
			retcode = CompareDirectories(NewDir,m_cwd);	// 0 if mismatch, 1 if match
			WriteToDebugFile("Setting Current Directory to+ [%s]",m_cwd);
		} else {
			// Couldn't find the directory message
			retcode=0;
		}
	} else {
		// Something has gone wrong with the psftp command
		retcode=0;
	}
	safe_free(lastDir);
	safe_free(cmdChain);
	return retcode;
}

void protocol_register_callback(CB_FUNCTION cbf)
{
	// GlobalCallbackFunction = cbf;
}

int protocol_lcd(char *NewDir)
{
	WriteToDebugFile("protocol_lcd(%s)",NewDir);
	safe_free(m_localDir);
	m_localDir = strdup(NewDir);
	return 1;
}

int protocol_mkdir(char *Dir)
{
	WriteToDebugFile("protocol_mkdir(%s)",Dir);
	protocol_cd(Dir);	// creates directories anyway
	return 1;
}

int doGetPut(char *cmdString,char *Filename)
{
	int retcode=1;	// success until something goes wrong
	char *charsout;
	char *errout;
	int res = ExecutePSFTP(cmdString,&charsout,&errout);
	if (res || strstr(charsout,"=>")==(char *)0) {
		WriteToDebugFile("Failed to get/put");
		// Error return or no Transfer message - must be a failure. Error message has the path to the file followed by a colon and the error message
		char *errmsg = strstr(charsout,Filename);
		if (errmsg) {
			errmsg+=strlen(Filename);
			errmsg+=2;
			safe_free(m_errorptr);
			m_errorptr = strdup(errmsg);
			// Now remove any trailing newlines
			char *t=&(m_errorptr[strlen(m_errorptr)-1]);
			while (*t=='\n' || *t=='\r') *t--='\0';
		}
		retcode=0;
	}
	safe_free(charsout);
	safe_free(errout);
	free(cmdString);
	return retcode;
}


int protocol_put(char *Filename)
{
	WriteToDebugFile("protocol_put(%s)",Filename);
	/* Caching code
	if (m_CurrentPutList) {
		m_CurrentPutList = (char *)realloc(m_CurrentPutList,strlen(m_CurrentPutList)+strlen(Filename)+128);
		strcat(m_CurrentPutList,"put ");
		strcat(m_CurrentPutList,Filename);
		strcat(m_CurrentPutList,"\n");
	} else {
		m_CurrentPutList = (char *)malloc(strlen(Filename)+strlen(m_cwd)+strlen(m_localDir)+128);
		sprintf(m_CurrentPutList,"cd %s\nlcd %s\nput %s\n",m_cwd,m_localDir,Filename);
	}
	*/
	// This code is the "direct" code - connects, puts the file and quits
	char *cmdString = (char *)malloc(strlen(Filename)+strlen(m_cwd)+strlen(m_localDir)+128);
	sprintf(cmdString,"cd %s\nlcd %s\nput %s\n",m_cwd,m_localDir,Filename);
	return doGetPut(cmdString,Filename);
}

int protocol_get(char *Filename)
{
	WriteToDebugFile("protocol_get(%s)",Filename);
	int retcode=1;	// success until something goes wrong
	char *cmdString = (char *)malloc(strlen(Filename)+strlen(m_cwd)+strlen(m_localDir)+128);
	sprintf(cmdString,"cd %s\nlcd %s\nget %s\n",m_cwd,m_localDir,Filename);
	return doGetPut(cmdString,Filename);
}

int protocol_delete(char *Filename)
{
	WriteToDebugFile("protocol_delete(%s)",Filename);
	int retcode=1;	// success until something goes wrong
	char *cmdString = (char *)malloc(strlen(Filename)+strlen(m_cwd)+strlen(m_localDir)+128);
	sprintf(cmdString,"cd %s\ndel %s\n",m_cwd,Filename);
	char *charsout;
	char *errout;
	int res = ExecutePSFTP(cmdString,&charsout,&errout);
	if (res || strstr(charsout,": OK\n")==(char *)0) {
		// Error return or no OK message
		safe_free(m_errorptr);
		m_errorptr = strdup("rm failed");
		retcode=0;
	}
	safe_free(charsout);
	safe_free(errout);
	free(cmdString);
	return retcode;
}


