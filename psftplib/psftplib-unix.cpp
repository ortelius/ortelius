// psftplib.cpp : Defines the exported functions for the DLL application.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "psftplib.h"

#define LAST_ERROR errno

//
// These are in psftp.c
//
char *m_HostName   = (char *)0;
char *m_UserName = (char *)0;
char *m_Password = (char *)0;
char m_identfile[4096] = {""};
int m_portnum = 22;
char *GlobalErrorPtr;
bool m_usetty = false;
bool m_sftp = false;
char *CreateRsp(char *FileName, char *LogFile);
CB_FUNCTION m_CallbackFunction = (CB_FUNCTION)0;
int OMExecuteCmd(char *TempFile,char *LogFile);
char *ReturnErrorPtr()
{
 return GlobalErrorPtr;
}

void SetGlobalErrorPtr(char *ErrStr, ...)
{
 char buf[2048];
 va_list ap;

 va_start(ap,ErrStr);
 vsnprintf(buf,sizeof(buf),ErrStr,ap);
 va_end(ap);

 if (GlobalErrorPtr)
  free(GlobalErrorPtr);
 GlobalErrorPtr = strdup(buf);
}

char *GetBaseDirectory()
{
#ifdef WIN32
 static char ModuleFileName[1024];

 GetModuleFileName(NULL,ModuleFileName,sizeof(ModuleFileName));

 for (int x=strlen(ModuleFileName)-1; x>0; x--)
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

  if (ret)
  {
   basedir=ret;
  }
 }
 return basedir;
#endif
}

int protocol_initialise(int initcode)
{
 if (initcode>0)
 {
  m_portnum = initcode & 0xffff;                // bottom 16 bits is port number

  if (initcode > 0xffff)
  {
   // Passing an initcode with anything in the top 16 bits sets use tty flag.
   m_usetty=true;
  }
 }
 SetGlobalErrorPtr("\0");
 return 0;
}

void protocol_register_callback(CB_FUNCTION cbf)
{
 m_CallbackFunction = cbf;
}

int protocol_connect(char *HostName,char *UserName,char *Password)
{
 char cmd[40000] = "";
	char err[60000] = "";
 char results[2048];
 char line[40000] = "";
 long filelen = 0L;
 char *buffer = NULL;
 FILE *fp = NULL;
 char thishostname[1024];

 m_UserName = UserName;
 m_Password = Password;
 // Horrible kludge to pass private key file through.
 // If we're using a keyfile, pass (char)1 followed by
 // the keyfile.
	if (m_Password == NULL)
		m_Password = strdup("\0");

	if (m_UserName == NULL)
		 m_UserName = strdup("\0");	
	
	if (m_Password[0]=='\1') 
	{
		sprintf(m_identfile,"-i %s",(m_Password+1));
		*m_Password = '\0';
	}

 gethostname(thishostname,sizeof(thishostname));

 bool localhost=false;
 if (strcasecmp(thishostname,HostName)==0 || strcasecmp(HostName,"localhost")==0)
 {
  // Connecting to local host
  localhost=true;
  if (!UserName)
  {
   return 1;    // just return success
  }
 }

 int res = 0;
 if (m_CallbackFunction)
  res = m_CallbackFunction(CALLBACK_PRECONNECT,(void *)1,0);

 m_HostName = strdup(HostName);
 pwd = strdup("/tmp");
	
/**** Skip acutal connect since its slowing down the deployment process ****/
 return 1;
	
/***
 if (m_usetty)
  sprintf(cmd,"sshexec -t %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' \"echo SUCCESS\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password);
 else
  sprintf(cmd,"sshexec %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' \"echo SUCCESS\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password);
 //    output
 //  SUCCESS
 // exec cmd
 // return rc;

 if (OMExecuteCmd(cmd,results) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR/>");
				}
				
    if (strstr(line,"SUCCESS") != NULL)
    {
     res=1;
     break;
    }
   }
   fclose(fp);
			
			if (getenv("TRIDEBUG") == NULL)
     unlink(results);
   return res;
  }
 }

 if ((fp=fopen(results, "r")) != NULL)
 {
		*err = '\0';
  while (fgets(line, sizeof(line), fp) != NULL)
  {
			if (line[strlen(line)-1] == '\n')
			{
				line[strlen(line)-1] = '\0';
				strcat(line,"<BR/>");
			}
   if (strlen(err) + strlen(line) < 59000)
				strcat(err,line);
  }
		SetGlobalErrorPtr(err);
  fclose(fp);
 }
	if (getenv("TRIDEBUG") == NULL)
   unlink(results);
 return 0;
***/	
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

char *protocol_exec(char **argv,char *shell,bool CopyFromLocal)
{
 char results[2048] = {""};
 char tmpcmd[2048] = {""};
 char line[40000] = {""};
	char execstr[65000] = {""};
 long filelen = 0L;
 char *buffer = NULL;
	int exitCode = 0;
 FILE *fp = NULL;
 char sftp[20] = {"-sftp"};
 bool executeWithTTY=false;
 char *basedir = GetBaseDirectory();	
 char thishostname[1024];
	
 gethostname(thishostname,sizeof(thishostname));
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
 const char *sshexec = "\\lib\\sshexec.bat";         // temp

#else
 const char *sshexec = "/lib/sshexec";
#endif
	
 if (m_sftp)
  m_sftp = false;
 else
  *sftp = '\0';

 *execstr = '\0';
	// printf("Malloced space for execstr=%d\n",strlen(basedir)+plen+strlen(m_HostName)+strlen(m_UserName)+strlen(m_Password)+(strlen(argv[0])*3)+200);

 char *p = strdup(argv[0]);
 char *p1 = p;
 char *cmd=(char *)0;

 while (*p)
 {
  if (*p=='/' || *p=='\\')
   cmd=p+1;
  p++;
 }

 if (cmd == (char *)0)
  cmd = p1;

 bool quoteit=false;
 bool addCloseQuote=false;
 bool noMoreQuotes=false;
 char *qc="";
 char singletick[] = "'";
 char emptystr[] = "\0";

if (m_HostName == NULL || strcasecmp(thishostname,m_HostName)==0 || strcasecmp(m_HostName,"localhost")==0)
{
 	*execstr = '\0';
  for (int i=0; i<n; i++)
  {
			if (i == 0)
			{
				int alen = strlen(argv[i]);
				for (int k=0;k<alen;k++)
				{
					if (argv[i][k] == '\\')
						argv[i][k] = '/';
				}
			}
	 	strcat(execstr,"\"");
			strcat(execstr,argv[i]);
			strcat(execstr,"\" ");
	 }			
}
else
{		
 if (CopyFromLocal)
 {
  // If we're allocating a terminal then we have to copy the file as a separate operation
  if (shell)
  {
   quoteit = (strcmp(shell,"echo")==0);                         // true if shell is "echo" command
   qc=quoteit ? singletick : emptystr;

   if (m_usetty)
   {
    char *FmtStr = "cat '%s' | tr -d \"\\r\" | %s%s -force %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s sh -c \"cat - >'/tmp/%s'";

    sprintf(execstr,FmtStr,argv[0],basedir,sshexec,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,cmd);
    executeWithTTY=true;
   }
   else
   {
    char *FmtStr = "cat '%s' | tr -d \"\\r\" | %s%s -force %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s sh -c \"cat - >'/tmp/%s';%s %s'/tmp/%s'";

    sprintf(execstr,FmtStr,argv[0],basedir,sshexec,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,cmd,shell,qc,cmd);
   }
  }
  else
  {
   if (m_usetty)
   {
    char *FmtStr = "cat '%s' | tr -d \"\\r\" | %s%s -force %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s sh -c \"cat - >'/tmp/%s';chmod +x '/tmp/%s'";

    sprintf(execstr,FmtStr,argv[0],basedir,sshexec,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,cmd,cmd,cmd);
    executeWithTTY=true;
   }
   else
   {
    char *FmtStr = "cat '%s' | tr -d \"\\r\" | %s%s -force %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s sh -c \"cat - >'/tmp/%s';chmod +x '/tmp/%s';'/tmp/%s'";

    sprintf(execstr,FmtStr,argv[0],basedir,sshexec,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,cmd,cmd,cmd);
   }
  }

	 if (m_usetty)
   strcat(execstr,"\"");
 }
 else
 {
  char dashT[] = "-t ";
  char emptystr[] = "\0";
  char *tflag = m_usetty ? dashT : emptystr;

  if (shell)
  {
   char *FmtStr = "%s%s -force %s %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s \"%s %s'%s'";

   sprintf(execstr,FmtStr,basedir,sshexec,tflag,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,shell,qc,argv[0]);
   addCloseQuote=true;
  }
  else
  {
   char *FmtStr = "%s%s -force %s %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s '%s'";

   sprintf(execstr,FmtStr,basedir,sshexec,tflag,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,argv[0]);
  }
 }

 if (executeWithTTY)
 {
  // We should already have created the copy structure. No need to add the parameters for
  // this bit - just execute it
	 if ((exitCode=OMExecuteCmd(execstr,results)) == 0)
	 {
	  if ((fp=fopen(results, "r")) != NULL)
	  {
	   while (fgets(line, sizeof(line), fp) != NULL)
	   {
					if (line[strlen(line)-1] == '\n')
					{
						line[strlen(line)-1] = '\0';
						strcat(line,"<BR/>");
					}
					
					printf("%s",line);
	   }
	   fclose(fp);
	  }
			if (getenv("TRIDEBUG") == NULL)
     unlink(results);
		}	

  //
  // Now construct the command to execute with terminal. Note, no need for -force since we'll
  // have already executed a command against this server now to copy the file to the target
  //
  if (shell)
  {
   char *FmtStr = "%s%s -t %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s \"%s\" %s'/tmp/%s'";

   sprintf(execstr,FmtStr,basedir,sshexec,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,shell,qc,cmd);
  }
  else
  {
   char *FmtStr = "%s%s -t %s -m \"%s\" -port %d -syn -usr \"%s\" -pw '%s' %s '/tmp/%s'";

   sprintf(execstr,FmtStr,basedir,sshexec,m_identfile,m_HostName,m_portnum,m_UserName,m_Password,sftp,cmd);
  }
 }

 for (int i=1; i<n; i++)
 {
  // printf("argv[%d]=[%s]\n",i,argv[i]);
  // Some bash interpreters seem to struggle during a copyexec with zero-length
  // parameters ("") but are happy if they're single quoted ('').
  char *ap = argv[i];

  if (*ap=='|' /* && quoteit */)
  {
   // Pipe char - we're trying to pipe the result - can we even do this remotely?
   strcat(execstr," | ");
   ap++;
   quoteit=false;
   noMoreQuotes=true;
  }
  //strcat(execstr,(CopyFromLocal && strlen(argv[i])==0)?" \'":" \"");
  strcat(execstr,noMoreQuotes ? " " : " '");
  strcat(execstr,ap);

  if (execstr[strlen(execstr)-1]=='\\')
   strcat(execstr,"\\");

  if (!noMoreQuotes)
   strcat(execstr,"'");
  //strcat(execstr,(CopyFromLocal && strlen(argv[i])==0)?"\' ":"\" ");
 }

 //if (!m_usetty && CopyFromLocal) strcat(execstr,"\"");
 if (addCloseQuote)
  strcat(execstr,"\"");

 if (quoteit)
  strcat(execstr,qc);

}
 char *szDebug = getenv("OMDEBUG");
 bool dbg=(szDebug && tolower(szDebug[0])=='y');

 if (dbg)
  printf("execstr=[%s]\n",execstr);

 if ((exitCode=OMExecuteCmd(execstr,results)) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR/>");
				}
				printf("%s",line);
   }
   fclose(fp);
  }
		if (getenv("TRIDEBUG") == NULL)
    unlink(results);
	}	

 free(p1);
 return (char *)exitCode;
}

char *protocol_pwd()
{
 return pwd;
}

void local_mkdir(const char *dir) {
        char tmp[65000];
        char *p = NULL;
        size_t len;

        snprintf(tmp, sizeof(tmp),"%s",dir);
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        mkdir(tmp, S_IRWXU);
                        *p = '/';
                }
        mkdir(tmp, S_IRWXU);
}

int protocol_cd(char *NewDir)
{
	char err[60000] = {""};
 char cmd[40000] = {""};
 char results[40000] = {""};
 char line[40000] = {""};
 FILE *fp = NULL;
 char CurrentDir[40000];
 char WorkDir[40000];
 char myDir[65000] = {""};
 char *buffer = NULL;
 int res = 0;
 int rv = 1;
 int k=0;
	int j=0;
 int len=0;

 if (NewDir != NULL)
 {
  len = strlen(NewDir);
  for (k=0;k<len;k++)
	 {
	 	if ((NewDir[k] == '/' || NewDir[k] == '\\') && j > 0 && myDir[j-1] != '/')
			{
	 		myDir[j] = '/';
				j++;
			}	
			else if ((NewDir[k] != '/' && NewDir[k] != '\\') || j == 0)
			{
				myDir[j] = NewDir[k];
			 j++;
		 }	
	 }
		myDir[j] = '\0';
	}	

 if (m_HostName == NULL || strcmp(m_HostName,"localhost") == 0)
	{
		local_mkdir(myDir);
		pwd=strdup(myDir);
	 return 1;
	}
	
	if (*myDir == '\0') // root dir
		return 1;
	
 if (m_usetty)
  sprintf(cmd,"sshexec -t %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' \"mkdir -p \"%s\";cd \"%s\";pwd\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password,myDir,myDir);
 else
  sprintf(cmd,"sshexec %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' \"mkdir -p \"%s\";cd \"%s\";pwd\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password,myDir,myDir);
 //    output
 //  SUCCESS
 // exec cmd
 // return rc;

 if (OMExecuteCmd(cmd,results) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
   bool match = true; // assume directories match
   while (fgets(line, sizeof(line), fp) != NULL)
   {
	len = strlen(line);
	for (k=0; k<len; k++)
	{
		if (line[k] == '\n' || line[k] == '\r') line[k] = '\0';
		else
		if (line[k] == '\\') line[k] == '/';
		else if (line[k] != myDir[k]) match=false;
	}
	if (match && strlen(line) == strlen(myDir))
	{
		res=1;
		pwd=strdup(myDir);
		break;
	}
   }
   fclose(fp);
			if (getenv("TRIDEBUG") == NULL)
     unlink(results);
   return res;
  }
 }

 if ((fp=fopen(results, "r")) != NULL)
 {
		*err = '\0';
  while (fgets(line, sizeof(line), fp) != NULL)
  {
			if (line[strlen(line)-1] == '\n')
			{
				line[strlen(line)-1] = '\0';
				strcat(line,"<BR/>");
			}
   if (strlen(err) + strlen(line) < 59000)
				strcat(err,line);
  }
		SetGlobalErrorPtr(err);
  fclose(fp);
 }
	if (getenv("TRIDEBUG") == NULL)
   unlink(results);
 return 0;
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

int protocol_mkdir(char *NewDir)
{
	char err[60000] = {""};
 char cmd[40000] = {""};
 char results[40000] = {""};
 char line[40000] = {""};
 FILE *fp = NULL;
 char CurrentDir[40000];
 char WorkDir[40000];
 char myDir[65000] = {""};
 char *buffer = NULL;
 int res = 0;
 int rv = 1;
 int k=0;
	int j=0;
 int len=0;

 if (NewDir != NULL)
 {
  len = strlen(NewDir);
  for (k=0;k<len;k++)
	 {
	 	if ((NewDir[k] == '/' || NewDir[k] == '\\') && j > 0 && myDir[j-1] != '/')
			{
	 		myDir[j] = '/';
				j++;
			}	
			else if ((NewDir[k] != '/' && NewDir[k] != '\\') || j == 0)
			{
				myDir[j] = NewDir[k];
			 j++;
		 }	
	 }
		myDir[j] = '\0';
	}	
	
	if (*myDir == '\0') // root dir
		return 1;

 if (m_usetty)
  sprintf(cmd,"sshexec -t %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' \"mkdir -p \"%s\";cd \"%s\";pwd\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password,myDir,myDir);
 else
  sprintf(cmd,"sshexec %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' \"mkdir -p \"%s\";cd \"%s\";pwd\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password,myDir,myDir);
 //    output
 //  SUCCESS
 // exec cmd
 // return rc;

 if (OMExecuteCmd(cmd,results) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR/>");
				}
				
    len = strlen(line);
    for (k=0; k<len; k++)
    {
     if (line[k] == '\\')
      line[k] == '/';
     if (line[k] == '\n' || line[k] == '\r')
      line[k] = '\0';
    }
    if (strcmp(myDir,line) == 0)
    {
     res=1;
     break;
    }
   }
   fclose(fp);
			if (getenv("TRIDEBUG") == NULL)
     unlink(results);
   return res;
  }
 }

 if ((fp=fopen(results, "r")) != NULL)
 {
		*err = '\0';
  while (fgets(line, sizeof(line), fp) != NULL)
  {
			if (line[strlen(line)-1] == '\n')
			{
				line[strlen(line)-1] = '\0';
				strcat(line,"<BR/>");
			}
			
   if (strlen(err) + strlen(line) < 59000)
				strcat(err,line);
  }
		SetGlobalErrorPtr(err);
  fclose(fp);
 }
 return 0;
}

int protocol_put(char *FileName)
{
	char err[60000] = {""};
 char cmd[40000] = {""};
 char results[40000] = {""};
 char line[40000] = {""};
 FILE *fp = NULL;
 char CurrentDir[40000];
 char WorkDir[40000];
 char myFileName[65000] = {""};
 char *buffer = NULL;
 int res = 1;
 int rv = 1;
 int k=0;
	int j=0;
 int len=0;
FILE *dbout = fopen("/tmp/debug.out","a");
fprintf(dbout,"protocol_put(%s)\n",FileName);

 if (FileName != NULL)
 {
  len = strlen(FileName);
  for (k=0;k<len;k++)
	 {
	 	if ((FileName[k] == '/' || FileName[k] == '\\') && j > 0 && myFileName[j-1] != '/')
			{
	 		myFileName[j] = '/';
				j++;
			}	
			else if ((FileName[k] != '/' && FileName[k] != '\\') || j == 0)
			{
				myFileName[j] = FileName[k];
			 j++;
		 }	
	 }
		myFileName[j] = '\0';
	}	

 if (m_HostName == NULL || strcmp(m_HostName,"localhost") == 0)
	{
		char lpwd[65000] = {""};
		getcwd(lpwd,sizeof(lpwd));
		
		sprintf(cmd,"cp -f -p \"%s/%s\" \"%s/%s\"",lpwd,myFileName,pwd,myFileName);
	}
	else
	{	
  sprintf(cmd,"sshexec -sftp %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' put \"%s\" \"%s/%s\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password,myFileName,pwd,myFileName);
 }
fprintf(dbout,"cmd=[%s]\n",cmd);
 if (OMExecuteCmd(cmd,results) == 0)
 {
fprintf(dbout,"OMExecuteCmd returns 0\n");
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
fprintf(dbout,"line=[%s]\n",line);
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR/>");
				}
				
    if (strstr(line,myFileName) != NULL)
    {
     res=1;
     break;
    }
   }
   fclose(fp);
			if (getenv("TRIDEBUG") == NULL)
     unlink(results);
fprintf(dbout,"about to return res=%d\n",res);
fclose(dbout);
   return res;
  }
 }
fprintf(dbout,"OMExecuteCmd returns non zero\n");

 if ((fp=fopen(results, "r")) != NULL)
 {
  *err = '\0';
  while (fgets(line, sizeof(line), fp) != NULL)
  {
fprintf(dbout,"line=[%s]\n",line);
	if (line[strlen(line)-1] == '\n')
	{
		line[strlen(line)-1] = '\0';
		strcat(line,"<BR/>");
	}
 	if (strlen(err) + strlen(line) < 59000) strcat(err,line);
  }
fprintf(dbout,"Setting global error ptr to [%s]\n",err);
  SetGlobalErrorPtr(err);
  fclose(fp);
 }
	if (getenv("TRIDEBUG") == NULL)
   unlink(results);
fprintf(dbout,"returning 0\n");
fclose(dbout);
 return 0;
}

int protocol_get(char *FileName)
{
	char err[60000] = {""};
 char cmd[40000] = {""};
 char results[40000] = {""};
 char line[40000] = {""};
 FILE *fp = NULL;
 char CurrentDir[40000];
 char WorkDir[40000];
 char myFileName[65000] = {""};
 char *buffer = NULL;
 int res = 1;
 int rv = 1;
 int k=0;
	int j=0;
 int len=0;

 if (FileName != NULL)
 {
  len = strlen(FileName);
  for (k=0;k<len;k++)
	 {
	 	if ((FileName[k] == '/' || FileName[k] == '\\') && j > 0 && myFileName[j-1] != '/')
			{
	 		myFileName[j] = '/';
				j++;
			}	
			else if ((FileName[k] != '/' && FileName[k] != '\\') || j == 0)
			{
				myFileName[j] = FileName[k];
			 j++;
		 }	
	 }
		myFileName[j] = '\0';
	}	

 sprintf(cmd,"sshexec -sftp %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' get \"%s/%s\" \"%s\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password,pwd,myFileName,myFileName);

 if (OMExecuteCmd(cmd,results) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
			if (line[strlen(line)-1] == '\n')
			{
				line[strlen(line)-1] = '\0';
				strcat(line,"<BR/>");
			}
			
   while (fgets(line, sizeof(line), fp) != NULL)
   {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR/>");
				}
				
    if (strstr(line,myFileName) != NULL)
    {
     res=1;
     break;
    }
   }
   fclose(fp);
			if (getenv("TRIDEBUG") == NULL)
     unlink(results);
   return res;
  }
 }

 if ((fp=fopen(results, "r")) != NULL)
 {
		*err = '\0';
  while (fgets(line, sizeof(line), fp) != NULL)
  {
			if (line[strlen(line)-1] == '\n')
			{
				line[strlen(line)-1] = '\0';
				strcat(line,"<BR/>");
			}
   if (strlen(err) + strlen(line) < 59000)
				strcat(err,line);
  }
		SetGlobalErrorPtr(err);
  fclose(fp);
 }
	if (getenv("TRIDEBUG") == NULL)
   unlink(results);
 return 0;
}

int protocol_delete(char *FileName)
{
	char err[60000] = {""};
 char cmd[40000] = {""};
 char results[40000] = {""};
 char line[40000] = {""};
 FILE *fp = NULL;
 char CurrentDir[40000];
 char WorkDir[40000];
 char myFileName[65000] = {""};
 char *buffer = NULL;
 int res = 1;
 int rv = 1;
 int k=0;
	int j=0;
 int len=0;

 if (FileName != NULL)
 {
  len = strlen(FileName);
  for (k=0;k<len;k++)
	 {
	 	if ((FileName[k] == '/' || FileName[k] == '\\') && j > 0 && myFileName[j-1] != '/')
			{
	 		myFileName[j] = '/';
				j++;
			}	
			else if ((FileName[k] != '/' && FileName[k] != '\\') || j == 0)
			{
				myFileName[j] = FileName[k];
			 j++;
		 }	
	 }
		myFileName[j] = '\0';
	}	

 if (m_HostName == NULL || strcmp(m_HostName,"localhost") == 0)
	{
		sprintf(cmd,"rm -f \"%s/%s\"",pwd,myFileName);
	}
	else
	{	
  sprintf(cmd,"sshexec -sftp %s -m \"%s\" -port \"%d\" -usr \"%s\" -pw '%s' del \"%s/%s\"",m_identfile,m_HostName,m_portnum,m_UserName,m_Password,pwd,myFileName);
 }
	
 if (OMExecuteCmd(cmd,results) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR/>");
				}
				
    if (strstr(line,myFileName) != NULL)
    {
     res=1;
     break;
    }
   }
   fclose(fp);
			if (getenv("TRIDEBUG") == NULL)
     unlink(results);
   return res;
  }
 }

 if ((fp=fopen(results, "r")) != NULL)
 {
		*err = '\0';
  while (fgets(line, sizeof(line), fp) != NULL)
  {
			if (line[strlen(line)-1] == '\n')
			{
				line[strlen(line)-1] = '\0';
				strcat(line,"<BR/>");
			}
   if (strlen(err) + strlen(line) < 59000)
				strcat(err,line);
  }
		SetGlobalErrorPtr(err);
  fclose(fp);
 }
	if (getenv("TRIDEBUG") == NULL)
   unlink(results);
 return 0;
}

int OMExecuteCmd(char *cmd,char *LogFile)
{
 int rc = 0;
 /* JAG - 06.13.05 - case 5814 */
 char CmdLine[2048] = "";
 char FileName[2048] = "";
 char arg0[1024];
 char arg1[1024];
 char arg2[1024];
 char *argv[20];
 char *nextarg;
 char *quotearg = NULL;
 int i = 0;
 int arg_i = 0;
 int in_quote = 0;
 int stat_val;
 pid_t pid;
 FILE *fp;
	
 char path[10000];
 char dest[10000];
	char *lastslash;

 memset(dest,0,sizeof(dest)); // readlink does not null terminate!
 struct stat info;
 pid = getpid();
// proc/%d/exe does not exist on Solaris
#ifdef __SUNOS__
 sprintf(path, "/proc/%d/path/a.out", pid);
#else
 sprintf(path, "/proc/%d/exe", pid);
#endif
 if (readlink(path, dest, sizeof(dest)) == -1)
   perror("readlink");

 lastslash = strrchr(dest,'/');
	*lastslash = '\0';
	
 /* JAG - 10.15.04 - bug. Need to init argv */
 while (i < 20)
 {
  argv[i] = NULL;
  i++;
 }
 i = 0;

 CreateRsp(FileName,LogFile);

 if ((fp=fopen(FileName,"w")) == NULL)
  return 1;

 fprintf(fp,"#!/bin/sh\n");
//	fprintf(fp,"env\n");
	fprintf(fp,"PATH=%s:$PATH;export PATH\n",dest);
	if (strstr(cmd,"cat -") != NULL && *(cmd + strlen(cmd) - 1) != '"')
		strcat(cmd,"\"");
 fprintf(fp,"%s 1>%s 2>&1",cmd,LogFile);
 fclose(fp);

// printf("CMD=%s\n",cmd);

 strcpy(CmdLine, "sh -c '");

 strcat(CmdLine, FileName);
 strcat(CmdLine, "'");

 chmod(FileName, 0500);	// PAG mod - prevent read by anyone else

 strcpy(arg0, "sh");
 strcpy(arg1, "-c");
 strcpy(arg2, FileName);
 arg_i = 2;

 argv[0] = arg0;
 argv[1] = arg1;
 argv[2] = arg2;

// printf("Execute: %s\n", CmdLine);

 pid = 0;

 fflush(stdout);

#ifndef __linux
 if ((pid = vfork()) == 0)
#else
 if ((pid = fork()) == 0)
#endif
 {
  /* execp replaces the process (om) with the script. Like 'exec' in Perl */
  execvp(argv[0], argv); /* In child */
  exit(0);
 }
 else if (pid > 0)
 {
  /* wait */
  waitpid(pid, &stat_val, 0);  /* In parent */
  // PAG - remove executable file (it may have passwords in it)
		if (getenv("TRIDEBUG") == NULL)
   unlink(FileName);
  return (stat_val);
 }
 return rc;
}

char *CreateRsp(char *FileName, char *LogFile)
{
 FILE *fp = NULL;
 int k = 0, len=0;
 char *var, *val;
 char *temp_dir = NULL;
 char local_builddir[2048] = "";
 char Str[100];

 temp_dir = getenv("TMP");

 if ( temp_dir == NULL )
  temp_dir = getenv("TEMP");

 if ( temp_dir == NULL )
  temp_dir = getenv("TMPDIR");

 if ( temp_dir != NULL )
 {
  strcpy( local_builddir, temp_dir);
 }
 else
 {
  strcpy( local_builddir, "/tmp");
 }

 sprintf(FileName,"%s/%lu.rsp",local_builddir,FileName);
 sprintf(LogFile,"%s/%lu.dat",local_builddir,FileName);

 return 0;
}
