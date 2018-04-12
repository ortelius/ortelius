// winlib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>
#include "netconn.h"
#include <Winsock2.h>

#pragma warning(disable: 4995)

#include "winlib.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>

#include <comdef.h>
#include <wincred.h>

#include <process.h>
#include <security.h>
#include <sspi.h>
#include <strsafe.h>
#include "auth.h"
#include "wmiwrapper.h"
#include <errno.h>

#include "md5.h"

#define open _open
#define read _read
#define LAST_ERROR GetLastError()

char *m_ShareName  = (char *)0;
char *m_HostName   = (char *)0;

char *m_UserName = (char *)0;
char *m_Password = (char *)0;

CB_FUNCTION m_CallbackFunction = (CB_FUNCTION)0;

char m_TargetDir[2048];

NetworkConnection	*m_nc;
WMIWrapper			*m_wmi;

char *GlobalErrorPtr;

int cfileexists(const char* filename);

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

char *ReturnErrorPtr()
{
	return GlobalErrorPtr;
}

void MakeWindows(char *s)
{
	char *t = s;
	while (*t)
	{
		if (*t == '/') *t='\\';
		t++;
	}
}

char *CalculateUNCPath(char *Dir)
{
	char *sn = m_nc->GetShareName();
	char *tgt;
	if (sn)
	{
		tgt = (char *)malloc(strlen(sn)+strlen(Dir)+10);
		if (sn[1]=='$')
		{
			// Drive Share (C$ etc)
			sprintf(tgt,"%s%s",sn,&(Dir[2]));
		}
		else
		{
			// Directory Share (\myshare) - beginning of path will always be the share name
			char *p = Dir+1; // Always starts with leading /
			while (*p)
			{
				if (*p=='\\' || *p=='/') break;
				p++;
			}
			if (*p)
			{
				sprintf(tgt,"%s%s",sn,p);
			}
			else
			{
				sprintf(tgt,"%s",sn);
			}
		}
		MakeWindows(tgt);
	}
	else
	{
		tgt = strdup(Dir);
	}
	return tgt;
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
	return ModuleFileName;
}

void SetGlobalErrorPtr(char *ErrStr, ...)
{
	char buf[2048];
	va_list ap;
	va_start(ap,ErrStr);
	vsnprintf(buf,sizeof(buf),ErrStr,ap);
	va_end(ap);
	if (GlobalErrorPtr) free(GlobalErrorPtr);
	GlobalErrorPtr = _strdup(buf);
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

int charToWchar(const char *str, wchar_t *buf, int buflen)
{
	int wcs = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, strlen(str), buf, buflen);
	buf[wcs] = 0;
	return wcs;
}

int sprintfCharToWchar(wchar_t *buf, int buflen, const char *fmt, ...)
{
	char ret[1024];
	
	va_list args;

	va_start(args,fmt);
	StringCchVPrintfA(ret, sizeof(ret), fmt, args);
	va_end(args);

	return charToWchar(ret, buf, buflen);
}

bool logonUser(const char *username, const char *password)
{
    wchar_t pszName[CREDUI_MAX_USERNAME_LENGTH+1] = {0};
    wchar_t pszPwd[CREDUI_MAX_PASSWORD_LENGTH+1] = {0};

	charToWchar(username, pszName, sizeof(pszName));
	charToWchar(password, pszPwd, sizeof(pszPwd));

	wchar_t pszUser[256];
	wchar_t pszDomain[256];

	m_UserName = (char *)username;
	m_Password = (char *)password;

    LPWSTR slash = wcschr (pszName, L'\\');
    if( slash == NULL )
    {
        return false;               // Program has failed.
    }
    StringCchCopyW(pszUser, CREDUI_MAX_USERNAME_LENGTH+1, slash+1);
    StringCchCopyNW(pszDomain, CREDUI_MAX_USERNAME_LENGTH+1, pszName, slash - pszName);

	return _logonUserWithSSPI(L"NTLM", /*TOKEN_QUERY,*/ pszDomain, pszUser, pszPwd);
}

char *calcMD5(char *FileName,char **ErrorString)
{
	MD5 md5;
	*ErrorString=(char *)0;
	char *p = FileName;
	while (*p=='"') p++;
	int t=strlen(FileName);
	if (t>0) {
		char *x = &(FileName[strlen(FileName)-1]);
		while (x>p && *x=='"') *x--='\0';
	}
	return md5.digestFile(p,ErrorString);
}

int protocol_initialise(void *DataPtr)
{
	//
	// For the winlib interface DataPtr points to the target directory. We need
	// this information to set up the network share.
	//
	InitialiseWinsock();

	m_TargetDir[0]='\0';
	if (DataPtr && strlen((char *)DataPtr)>=2)
	{
		m_ShareName = (char *)malloc(strlen((char *)DataPtr)+2);
		strcpy(m_ShareName,(char *)DataPtr);
		if (m_ShareName[1] == ':')
		{
			m_ShareName[1]='$';
			m_ShareName[2]='\0';
			return 0;
		}
		else
		if (m_ShareName[0] == '\\')
		{
			strcpy(m_ShareName,&(m_ShareName[1]));
			return 0;
		}
		else
		{
			return 0;	// should really return 1 but the share may not be there for localhost
		}
	}
	return DataPtr?1:0;	// If DataPtr is NULL return success
}



int protocol_connect(char *HostName,char *UserName,char *Password)
{
	//
	// UserName will be in form domain\username or just username
	// if username is missing we'll turn it into domain\username
	// if domain = HostName we'll set the "noimpersonate" flag
	//
	WriteToDebugFile("protocol_connect(HostName=[%s] UserName=[%s]",HostName,UserName);
	wchar_t pszName[CREDUI_MAX_USERNAME_LENGTH+1] = {0};
    wchar_t pszPwd[CREDUI_MAX_PASSWORD_LENGTH+1] = {0};

	m_UserName = UserName;
	m_Password = Password;
	char thishostname[1024];
	gethostname(thishostname,sizeof(thishostname));

	bool localhost=false;
	if (_stricmp(thishostname,HostName)==0 || _stricmp(HostName,"localhost")==0)
	{
		// Connecting to local host
		localhost=true;
		if (!UserName) {
			return 1;	// just return success
		}
	}

	int res = 1;
	if (m_CallbackFunction) res = m_CallbackFunction(CALLBACK_PRECONNECT,(void *)1,0);

	m_HostName = _strdup(HostName);
	
	if (m_ShareName == (char *)0) {
		WriteToDebugFile("No share name, must be connecting prior to execution - returning success");
		return 1;	// Connecting prior to an exec/copyexec call.
	} else {
		WriteToDebugFile("Share name is [%s] - connecting prior to file transfer",m_ShareName);
	}

	bool NoImpersonate=false;
	char *dn = (char *)0;
	char *un = (char *)0;

	if (UserName)
	{
		char *c = strdup(UserName);
		char *bslash = strchr(c,'\\');
		
		if (bslash)
		{
			*bslash='\0';
			un=bslash+1;
			dn=c;
			if (_stricmp(HostName,dn)==0)
			{
				NoImpersonate=true;
			}
		}
		else
		{
			// No domain specified
			dn=HostName;
			un=UserName;
			NoImpersonate=true;
		}
	}
	else
	{
		SetGlobalErrorPtr("Connecting to remote server requires username");
		return 0;
	}

	if (res == 1) 
	{
		char *qun=(char *)malloc(strlen(dn)+strlen(un)+2);
		sprintf(qun,"%s\\%s",dn,un);
		WriteToDebugFile("qun=[%s]",qun);
		// sprintf(qun,UserName);	// just user name - we're passing domain in somewhere else now.

		WriteToDebugFile("Calling NetworkConnection(HostName=[%s] m_ShareName=[%s]\n",HostName,m_ShareName?m_ShareName:"C$");
		m_nc = new NetworkConnection(HostName,m_ShareName?m_ShareName:"C$");

		charToWchar(qun, pszName, sizeof(pszName));
		charToWchar(Password, pszPwd, sizeof(pszPwd));

		m_wmi = new WMIWrapper;
		if (!m_wmi->initialize())
		{
			SetGlobalErrorPtr(strerror(LAST_ERROR));
			return 0;
		}

		if (!m_wmi->createLocator())
		{
			SetGlobalErrorPtr(strerror(LAST_ERROR));
			return 0;
		}

		//wchar_t *p=L"About to connect server using creds (pszName=[%s] pszPwd=[%s])\n";

		//wprintf(p,pszName,pszPwd);

		if (!localhost)
		{
			//
			// not connecting to local host
			//
			//printf("Connecting using creds (pszName=");
			//wprintf(pszName);
			//printf("]\n");
			if (!m_wmi->connectServerUsingCreds(HostName, "root\\cimv2", pszName, pszPwd))
			{
				SetGlobalErrorPtr(strerror(LAST_ERROR));
				return 0;
			}
		}
		else
		{
			//
			// Connecting to local host
			//
			//printf("Connecting using token\n");
			WriteToDebugFile("Connecting to local host");
			NoImpersonate=false;
			if (!m_wmi->connectServerUsingToken(HostName,"root\\cimv2"))
			{
				//printf("There was a problem in connectServerUsingToken\n");
				SetGlobalErrorPtr(strerror(LAST_ERROR));
				//printf("Last error=[%s]\n",strerror(LAST_ERROR));
				//printf("returning 0\n");
				return 0;
			}
			char *fq = (char *)malloc(strlen(thishostname)+strlen(UserName)+10);
			sprintf(fq,"%s\\%s",thishostname,UserName);
			charToWchar(fq, pszName, sizeof(pszName));
			free(fq);
		}

		//printf("done connect server using creds\n");

		if (!m_wmi->createCoAuthority(pszName, pszPwd))
		{
			//printf("createCoAuthority failed\n");
			SetGlobalErrorPtr(strerror(LAST_ERROR));
			return 0;
		}

		if (!m_wmi->createProxyBlanket())
		{
			SetGlobalErrorPtr(strerror(LAST_ERROR));
			return 0;
		}

		if (NoImpersonate || true)
		{
			if (m_nc->connect(qun, Password))
			{
				return 1;
			}
			else
			{
				SetGlobalErrorPtr(strerror(LAST_ERROR));
				return 0;
			}
		}
		else
		{
			bool htok = logonUser(qun, Password);
			if (htok)
			{
				if (m_nc->connect())
				{
					return 1;
				}
				else
				{
					SetGlobalErrorPtr(strerror(LAST_ERROR));
					_revertUser();
					return 0;
				}
			}
			else
			{
				return 0;
			}
			return 0;
		}
	}
	else
	{
		return 1;
	}
}


char *protocol_return_last_error()
{
	return ReturnErrorPtr();
}


int protocol_disconnect()
{
	if (m_nc) {
		m_nc->disconnect();
	}
	return 0;
}

char *protocol_exec(char **argv,char *shell,bool CopyFromLocal)
{
	char *basedir = GetBaseDirectory();
	char *FileExts[]={
	"cmd",
	"bat",
	"exe"
	};
	int NumExts = sizeof(FileExts) / sizeof(FileExts[0]);
	//
	// Get length of all the parameters
	//
	int n=0;
	int plen=0;
	bool cs=false;
	while (argv[n])
	{
		plen+=(strlen(argv[n])+4);
		n++;
	}
	if (shell) plen+= (strlen(shell)+10);

	char *execstr = (char *)0;
	// printf("Malloced space for execstr=%d\n",strlen(basedir)+plen+strlen(m_HostName)+strlen(m_UserName)+strlen(m_Password)+60);

	if (CopyFromLocal)
	{
		//
		// We're copying from local directory - check to see if the file exists. If it doesn't, try some alternate extensions...
		//
		char *p2=(char *)0;
		char *p = argv[0];
		while (*p)
		{
			if (*p == '.') p2=p+1;
			p++;
		}
		if (p2)
		{
			// There's a file extension we can try changing...
			struct stat s;
			int e=0;
			int f=0;
			e = stat(argv[0],&s);
			while (e != 0 && errno == ENOENT)
			{
				// File does not exist - try the alternatives
				strcpy(p2,FileExts[f]);
				e = stat(argv[0],&s);
				f++;
				if (f>=NumExts) break;
			}
		}
	}

	bool bi=false;	// built in
	bool oq=false;	// outer quotes

	if (m_HostName)
	{
		char execpgm[4096] = {""};
		
		sprintf(execpgm,"%s\\psexec.exe",basedir);
		if (cfileexists(execpgm))
		{
		 oq=true;
		 execstr = (char *)malloc(strlen(basedir)+plen+strlen(m_HostName)+strlen(m_UserName)+strlen(m_Password)+60);
		 if (shell) {
			 if (CopyFromLocal)
			  sprintf(execstr,"\"\"%s\\psexec.exe\" \\\\%s -u \"%s\" -p \"%s\" -c \"%s\" cmd /c \"%s\" \"%s\"\"",basedir,m_HostName,m_UserName,m_Password,argv[0],shell,argv[0]);
				else
					sprintf(execstr,"\"\"%s\\psexec.exe\" \\\\%s -u \"%s\" -p %s cmd /c \"%s\" \"%s\"\"",basedir,m_HostName,m_UserName,m_Password,shell,argv[0]);
		} else {
			if (CopyFromLocal)
				sprintf(execstr,"\"\"%s\\psexec.exe\" \\\\%s -u \"%s\" -p \"%s\" -c \"%s\"\" \"%s\"\"",basedir,m_HostName,m_UserName,m_Password,argv[0], argv[0]);			
			else	
			 sprintf(execstr,"\"\"%s\\psexec.exe\" \\\\%s -u \"%s\" -p \"%s\" \"%s\"\"",basedir,m_HostName,m_UserName,m_Password,argv[0]);			
		}
		else
		{							
		 oq=true;
		 execstr = (char *)malloc(strlen(basedir)+plen+strlen(m_HostName)+strlen(m_UserName)+strlen(m_Password)+60);
		 if (shell) {
		 	char *FmtStr = CopyFromLocal?"\"\"%s\\winexec.exe\" \\\\%s /user:\"%s\" /pwd:\"%s\" /c /s:\"%s\" \"%s\"\"":"\"\"%s\\winexec.exe\" \\\\%s /user:\"%s\" /pwd:%s /s:\"%s\" \"%s\"\"";
			 sprintf(execstr,FmtStr,basedir,m_HostName,m_UserName,m_Password,shell,argv[0]);
		} else {
			char *FmtStr = CopyFromLocal?"\"\"%s\\winexec.exe\" \\\\%s /user:\"%s\" /pwd:\"%s\" /c \"%s\"\"":"\"\"%s\\winexec.exe\" \\\\%s /user:\"%s\" /pwd:%s \"%s\"\"";
			sprintf(execstr,FmtStr,basedir,m_HostName,m_UserName,m_Password,argv[0]);
		}
		WriteToDebugFile("m_HostName=[%s]",m_HostName);
		WriteToDebugFile("shell=[%s]\n",shell);
		WriteToDebugFile("execstr=[%s]\n",execstr);
	}
	else
	{
		// No hostname - must be local execution
		WriteToDebugFile("m_HostName not set: local execution");
		char *t=argv[0];
		char *e=(char *)0;
		while (*t)
		{
			if (*t=='.') e=t;
			t++;
		}
		execstr = (char *)malloc(plen+60);
		if (e && (stricmp(e,".vbs")==0 || stricmp(e,".js")==0))
		{
			sprintf(execstr,"cscript.exe /nologo \"%s\"",argv[0]);
			cs=true;
		}
		else
		if (!e && strncmp(argv[0],"md5sum",6))
		{
			// no extension and not md5sum - must be a built-in command (dir, mkdir etc)
			sprintf(execstr,"cmd /c %s",argv[0]);
			bi=true;
		}
		else
		{
			if (shell) {
				// Note shell should have [...] around the bits that need quoting
				sprintf(execstr,"\"%s \"%s\"",shell,argv[0]);
			} else {
				sprintf(execstr,"\"\"%s\"",argv[0]);
			}
		}
		// Now replace any [ .. ] chars with quotes (for local execution)
		char *bp=execstr;
		while (*bp) {
			if (*bp=='[' || *bp==']') *bp='"';
			bp++;
		}
	}
	
	if (oq && n>1) {
		WriteToDebugFile("quoted execstr=[%s], removing last quote\n",execstr);
		// Entire command line up until now is surrounded by " - lose the last quote. The arg
		// loop below will stick it back on the end of the parameter list
		char *p = &(execstr[strlen(execstr)-1]);
		WriteToDebugFile("*p=[%c]",*p);
		if (*p=='"') *p--='\0';
	}
	
	WriteToDebugFile("a) execstr=[%s]\n",execstr);

	for (int i=1;i<n;i++)
	{
		// printf("argv[%d]=[%s]\n",i,argv[i]);
		strcat(execstr," \"");
		strcat(execstr,argv[i]);
		if (execstr[strlen(execstr)-1]=='\\') strcat(execstr,"\\");
		strcat(execstr,(i==n-1)?cs?"\"":!bi?"\"\"":"\"":"\" ");
	}

	int res=0;
	WriteToDebugFile("b) execstr=[%s]\n",execstr);

	// If we're running locally and we're trying to run md5sum then calculate the checksum directly
	if (strncmp(execstr,"\"\"md5sum\"",7)==0) {
		char *p = &(execstr[10]);
		while (*p=='"') p++;
		int t=strlen(execstr);
		if (t>0) {
			char *x = &(execstr[strlen(execstr)-1]);
			while (x>p && *x=='"') *x--='\0';
		}
		char *errstr;
		char *md5 = calcMD5(p,&errstr);
		if (md5) {
			printf("%s  %s\n",md5,p);
		} else {
			printf("%s\n",errstr);
		}
	} else {
		// printf("Strlen of execstr=%d\n",strlen(execstr));
		// Replace any [...] chars with quotes
		
		// printf("%s\n",execstr);
		res = system(execstr);
	}
	/*
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	GetStartupInfo(&si);
	BOOL createRet = CreateProcess(NULL, 
	  execstr,				// command line 
	  NULL,					// process security attributes 
	  NULL,					// primary thread security attributes 
	  TRUE,					// handles are inherited 
	  0,					/fEX/ creation flags 
	  NULL,					// use parent environment 
	  NULL,					// use parent's current directory 
	  &si,					// STARTUPINFO pointer 
	  &pi);					// receives PROCESS_INFORMATION 

	  */
	// printf("res=%d\n",res);
	if (execstr) free(execstr);
	return (char *)res;
}

int protocol_cd(char *NewDir)
{
	char CurrentDir[4192];

	// printf("protocol_cd(%s)\n",NewDir);

	_getcwd(CurrentDir,sizeof(CurrentDir));
	
	char *tgt=CalculateUNCPath(NewDir);

	int rv=0;
	int res = _chdir(tgt);
	
	if (res == 0)
	{
		// Success
		strncpy(m_TargetDir,NewDir,sizeof(m_TargetDir)-1);
		rv=1;
	}
	else
	{
		// Failure
		SetGlobalErrorPtr(strerror(errno));
		rv=0;
	}
	free(tgt);
	_chdir(CurrentDir);
	return rv;
}

char *protocol_pwd()
{
	return m_TargetDir;
}


void protocol_register_callback(CB_FUNCTION cbf)
{
	m_CallbackFunction = cbf;
}


int protocol_lcd(char *NewDir)
{
	if (_chdir(NewDir) == 0)
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

int protocol_mkdir(char *Dir)
{
	char *tgt=CalculateUNCPath(Dir);
	int res = _mkdir(tgt);
	int rv=0;
	if (res == 0)
	{
		// Successfully created directory
		rv=1;
	}
	else
	{
		// Failed to create target directory
		SetGlobalErrorPtr(strerror(LAST_ERROR));
		rv=0;
	}
	free(tgt);
	return rv;	
}

int protocol_put(char *Filename)
{
	//
	// Open the file
	//
	bool ThisMode=true;
	bool BrowseMode=true;
	bool ReplaceReadOnlyFiles=true;
	bool Translate=true;
	struct stat s;
	struct stat t;
	char *tgtdir = (char *)0;
	char *TargetFileName = (char *)0;
	int in;
	int out;
	int TotalBytes=0;

	try
	{
		int res = stat(Filename,&s);
		if (res != 0) throw(1);
		
		in = open(Filename,O_RDONLY | _O_BINARY );
		if (in == -1) throw(1);

		
		
		tgtdir = CalculateUNCPath(m_TargetDir);
		TargetFileName = (char *)malloc(strlen(tgtdir)+strlen(Filename)+10);
		sprintf(TargetFileName,"%s\\%s",tgtdir,Filename);

		res = stat(TargetFileName,&t);
		if (res == 0)
		{
			// File exists - make sure it's writable
			res = _chmod(TargetFileName,_S_IREAD | _S_IWRITE);
			if (res != 0) throw(2);
		}

		out = open(TargetFileName,O_TRUNC | O_CREAT | _O_BINARY | O_WRONLY, _S_IREAD | _S_IWRITE);
		if (out == -1) throw(2);
		
		int BytesRead=0;
		do
		{
			unsigned char FileData[4096];
			BytesRead = read(in,FileData,sizeof(FileData));
			if (BytesRead == -1) throw(3);
			if (BytesRead > 0)
			{
				int BytesWritten = _write(out,FileData,BytesRead);
				if (BytesWritten == -1) throw(3);
			}
			TotalBytes+=BytesRead;
		} while (BytesRead > 0);
		if (TotalBytes != s.st_size) throw(3);
	}
	catch(int errpoint)
	{
		SetGlobalErrorPtr(strerror(errno));
		if (errpoint >= 2)
		{
			free(tgtdir);
			free(TargetFileName);
			_close(in);
		}
		if (errpoint >= 3)
		{
			_close(out);
		}
		return 0;
	}

	_close(in);
	_close(out);

	free(TargetFileName);
	free(tgtdir);
			
	return 1;
}

int protocol_get(char *Filename)
{
	return 1;
}

int protocol_delete(char *Filename)
{
	struct stat t;
	char *tgtdir = (char *)0;
	char *TargetFileName = (char *)0;
	try
	{
		tgtdir = CalculateUNCPath(m_TargetDir);
		TargetFileName = (char *)malloc(strlen(tgtdir)+strlen(Filename)+10);
		sprintf(TargetFileName,"%s\\%s",tgtdir,Filename);

		int res = stat(TargetFileName,&t);
		if (res == 0)
		{
			// File exists - make sure it's writable
			res = _chmod(TargetFileName,_S_IREAD | _S_IWRITE);
			if (res != 0) throw(2);
			// And remove it
			res = _unlink(TargetFileName);
			if (res != 0) throw(2);
		}
	}
	catch(int errpoint)
	{
		SetGlobalErrorPtr(strerror(errno));
		if (errpoint >= 2)
		{
			free(tgtdir);
			free(TargetFileName);
		}
		return 0;
	}
	return 1;
}

int cfileexists(const char* filename){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else // -1
        return 0;
}