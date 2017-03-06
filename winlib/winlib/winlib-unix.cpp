// winlib.cpp : Defines the exported functions for the DLL application.
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
#include <md5.h>
#include <sys/wait.h>
#include <ctype.h>
#include "winlib.h"
#define _strdup strdup
#define LAST_ERROR errno
#define _open open
#define _close close
#define _read read
#define _write write
#define _chmod chmod
#define _chdir chdir
#define _mkdir mkdir
#define _unlink unlink
#define _getcwd getcwd
#define _stricmp strcasecmp
#define stricmp strcasecmp
#define _S_IREAD S_IRUSR
#define _S_IWRITE S_IWUSR
#define _O_BINARY 0

enum Direction { leading, trailing, both };
	
char *m_ShareName  = (char *)0;
char *m_HostName   = (char *)0;
char *m_UserName = (char *)0;
char *m_Password = (char *)0;
CB_FUNCTION m_CallbackFunction = (CB_FUNCTION)0;
bool m_connected;
char m_TargetDir[40000] = "";
char *GlobalErrorPtr;
void StrLwr(char *str);
char *StripChar(register char *buffer, register char *removethis, enum Direction feature);
void CalculateUNCPath(char *cwd, char *FileName,char *TargetDir, char *TargetFileName);
char *CreateRsp(char *FileName, char *LogFile);
char *CreateBatchFile(char *FileName);
int OMExecuteCmd(char *TempFile,char *LogFile);

char *ReturnErrorPtr()
{
 return GlobalErrorPtr;
}

void MakeUnix(char *s)
{
 char *t = s;

 while (*t)
 {
  if (*t == '\\')
   *t='/';
  t++;
 }
}

void MakeWindows(char *s)
{
 char *t = s;

 while (*t)
 {
  if (*t == '/')
   *t='\\';
  t++;
 }
}

void CalculateUNCPath(char *cwd, char *FileName,char *TargetDir, char *TargetFileName)
{
 char WorkName[40000] = "";
 char *slash = NULL;

 sprintf(WorkName,"%s\\%s",cwd,FileName);
 MakeWindows(WorkName);

 if ((slash = strrchr(WorkName,'\\')) != NULL)
 {
  strcpy(TargetFileName,(slash+1));
  *slash = '\0';
  strcpy(TargetDir,WorkName);
 }

 if (TargetDir[1] == ':')
 {
  strcpy(WorkName,TargetDir+2);
  strcpy(TargetDir,WorkName);
 }

 if (cwd[1] == ':')
 {
  if (m_ShareName == NULL)
   m_ShareName = malloc(strlen(WorkName));

  sprintf(m_ShareName,"%c$",cwd[0]);
 }
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

void SetGlobalErrorPtr(char *ErrStr, ...)
{
 char buf[2048];
 va_list ap;

 va_start(ap,ErrStr);
 vsnprintf(buf,sizeof(buf),ErrStr,ap);
 va_end(ap);
 if (GlobalErrorPtr)
  free(GlobalErrorPtr);
 GlobalErrorPtr = _strdup(buf);
}

char *GetErrorString(int errnum)
{
 static char *res;
 static char buf[1024];

 res = buf;

 strcpy(buf,strerror(errnum));
 //
 // Remove any trailing \n chars
 //
 char *p = &(buf[strlen(buf)-1]);
 while (*p=='\n')
  *p-- = '\0';
 return (char *)res;
}

char *calcMD5(char *FileName,char **ErrorString)
{
 MD5 md5;

 *ErrorString=(char *)0;
 char *p = FileName;
 while (*p=='"')
  p++;
 int t=strlen(FileName);
 if (t>0)
 {
  char *x = &(FileName[strlen(FileName)-1]);
  while (x>p && *x=='"')
   *x--='\0';
 }
 return md5.digestFile(p,ErrorString);
}

int protocol_initialise(void *DataPtr)
{
 //
 // For the winlib interface DataPtr points to the target directory. We need
 // this information to set up the network share.
 //
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
  else if (m_ShareName[0] == '\\')
  {
   strcpy(m_ShareName,&(m_ShareName[1]));
   return 0;
  }
  else
  {
   return 0;    // should really return 1 but the share may not be there for localhost
  }
 }
 return DataPtr ? 1 : 0; // If DataPtr is NULL return success
}

int protocol_connect(char *HostName,char *UserName,char *Password)
{
 //
 // UserName will be in form domain\username or just username
 // if username is missing we'll turn it into domain\username
 // if domain = HostName we'll set the "noimpersonate" flag
 //
	char err[60000] = "";
 char cmd[40000] = "";
 char results[2048];
 char line[40000] = "";
 long filelen = 0L;
 char *buffer = NULL;
 FILE *fp = NULL;
 wchar_t pszName[1024] = {0};
 wchar_t pszPwd[1024] = {0};

 m_UserName = UserName;
 m_Password = Password;
 char thishostname[1024];
 gethostname(thishostname,sizeof(thishostname));

 bool localhost=false;
 if (_stricmp(thishostname,HostName)==0 || _stricmp(HostName,"localhost")==0)
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

 m_HostName = _strdup(HostName);
	
/**** Skip acutal connect since its slowing down the deployment process ****/
 return 1;
	
	/****
 sprintf(cmd,"winexe --uninstall --user=%s%%%s //%s \"cmd /c echo SUCCESS\"",m_UserName,m_Password,m_HostName);
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
    if ((buffer=strstr(line,"ERROR")) != NULL)
    {
     SetGlobalErrorPtr(buffer);
     res=0;
     break;
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
 else
	{
  if ((fp=fopen(results, "r")) != NULL)
  {
	 	*err = '\0';
   while (fgets(line, sizeof(line), fp) != NULL)
   {
	 		if (line[strlen(line)-1] == '\n')
	 		{
	 			line[strlen(line)-1] = '\0';
	 			strcat(line,"<BR>");
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
 return 0;
****/	
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
 char cmd[40000] = "";
	char cmd1[40000] = "";	
	char cmd2[40000] = "";	
	char remotescript[40000] = "";
	char err[60000] = "";
	char workshell[40000] = {""};
 char results[40000] = ""; 
	char results1[40000] = "";	
 char results2[40000] = "";	
 char results3[40000] =	"";
 char line[40000] = "";
 char *basedir = GetBaseDirectory();
 char *buffer = NULL;
 char *FileExts[]={
  "cmd","bat","exe"
 };
 int NumExts = sizeof(FileExts) / sizeof(FileExts[0]);
 //
 // Get length of all the parameters
 //
 int n=0;
 int plen=0;
 int TotalBytes = -1;
 bool cs=false;
 char *basename = NULL;
	char *rembasename = NULL;
 FILE *fp = NULL;
	FILE *fpremote = NULL;
 char execstr[60000];

 while (argv[n])
 {
  plen+=(strlen(argv[n])+4);
  n++;
 }
 if (shell)
	{
  plen+= (strlen(shell)+10);
		strcpy(workshell,shell);
	}


 if (CopyFromLocal)
 {
  //
  // We're copying from local directory - check to see if the file exists. If it doesn't, try some alternate extensions...
  //

	 int alen = strlen(argv[0]);
	 for (int k=0;k<alen;k++)
	 {
		 if (argv[0][k] == '\\')
		 	argv[0][k] = '/';
	 }
			
  char *p2=(char *)0;
  char *p = argv[0];
  while (*p)
  {
   if (*p == '.')
    p2=p+1;
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
    if (f>=NumExts)
     break;
   }
  }

		if (shell == NULL)
		{
		 if (strstr(argv[0],".vbs") != NULL || strstr(argv[0],".js") != NULL	|| strstr(argv[0],".ps1") != NULL)
			{	
				strcpy(workshell,"cscript /nologo");
				shell = workshell;
			}	
		}	
		
  MakeUnix(argv[0]);
  if ((basename = strrchr(argv[0],'/')) != NULL)
   basename++;
  else
   basename = argv[0];

  sprintf(cmd,"smbclient --user=%s%%%s //%s/C$ -c 'mkdir \"\\temp\";cd \"\\temp\";put \"%s\" \"%s\";allinfo \"%s\"'",m_UserName,m_Password,m_HostName,argv[0],basename,basename);
  //  output
  // altname: foo.txt
  // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
  // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
  // attributes: A (20)
  //  stream: [::$DATA], 1000 bytes

  if (OMExecuteCmd(cmd,results) == 0)
  {
   if ((fp=fopen(results, "r")) != NULL)
   {
    while (fgets(line, sizeof(line), fp) != NULL)
    {
     if (strstr(line,"stream:") != NULL)
     {
      char *p = line + strlen("stream: [::$DATA], ");
      int k = 0;
      for (k=0; k<strlen(line); k++)
      {
       if (*(p+k) == ' ')
        *(p+k) = '\0';
      }
      TotalBytes = atol(p);
      break;
     }
    }
    fclose(fp);
				if (getenv("TRIDEBUG") == NULL)
	     unlink(results);
   }
  }
  else
  {
   if ((fp=fopen(results, "r")) != NULL)
   {
				*err = '\0';
		  while (fgets(line, sizeof(line), fp) != NULL)
		  {
					if (line[strlen(line)-1] == '\n')
					{
						line[strlen(line)-1] = '\0';
						strcat(line,"<BR>");
					}
		   if (strlen(err) + strlen(line) < 59000)
						strcat(err,line);
		  }
				SetGlobalErrorPtr(err);
		  fclose(fp);
		 }
			if (getenv("TRIDEBUG") == NULL)
		   unlink(results);
   return (char *)1;
  }
  if (TotalBytes == -1)
   return (char *)1;
 }

 bool bi=false;

 CreateBatchFile(remotescript);
 if ((rembasename = strrchr(remotescript,'/')) != NULL)
  rembasename++;
 else
  rembasename = remotescript;
	
 if ((fpremote=fopen(remotescript,"w")) != NULL)
	{
	 if (m_HostName)
	 {
	  if (shell)
	  {
	   if (CopyFromLocal)
	    fprintf(fpremote,"%s \"c:\\temp\\%s\"",shell,basename);
	   else
	    fprintf(fpremote,"%s \"%s\"",shell,basename);
	  }
	  else
	  {
	   if (CopyFromLocal)
	    fprintf(fpremote,"\"c:\\temp\\%s\"",basename);
	   else
	    fprintf(fpremote,"\"\"%s\"",basename);
	  }
	 }		
 }
		
 if (m_HostName)
 {
  sprintf(execstr,"winexe --uninstall --user=%s%%%s //%s \"cmd /c \\\"c:\\temp\\%s\\\"\"",m_UserName,m_Password,m_HostName,rembasename);
 }
 else
 {
  printf("ERROR: No hostname.  Can't run locally\n");
  return (char *)1;
 }

 int addparms = 0;

 for (int i=1; i<n; i++)
 {
  int alen = strlen(argv[i]);
  int y = 0;
  addparms = 1;
  // printf("argv[%d]=[%s]\n",i,argv[i]);
//  strcat(execstr," \"");

		strcpy(line,argv[i]);
		StripChar(line,"\\\"",both);
//  *line = '\0';
//  for (int x=0; x<alen; x++)
//  {
//   line[y] = argv[i][x];
//   if (line[y] == '\\')
//   {
//    y++;
//    line[y] = '\\';
//   }
//   y++;
//  }
//  line[y] = '\0';
		fprintf(fpremote," \"");
  fprintf(fpremote,line);
//		strcat(execstr,"\\");
//  if (execstr[strlen(execstr)-1]=='\\')
		fprintf(fpremote,"\"");
//  strcat(execstr,(i==n-1) ? cs ? "\"" : !bi ? "\"\"" : "\"" : "\" ");
 }

// if (addparms == 0)
//  strcat(execstr,"\"");

 fclose(fpremote);
	
 sprintf(cmd1,"smbclient --user=%s%%%s //%s/C$ -c 'mkdir \"\\temp\";cd \"\\temp\";put \"%s\" \"%s\";allinfo \"%s\"'",m_UserName,m_Password,m_HostName,remotescript,rembasename,rembasename);
 //  output
 // altname: foo.txt
 // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
 // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
 // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
 // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
 // attributes: A (20)
 //  stream: [::$DATA], 1000 bytes

 if (OMExecuteCmd(cmd1,results1) == 0)
 {
  if ((fp=fopen(results1, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
    if (strstr(line,"stream:") != NULL)
    {
     char *p = line + strlen("stream: [::$DATA], ");
     int k = 0;
     for (k=0; k<strlen(line); k++)
     {
      if (*(p+k) == ' ')
       *(p+k) = '\0';
     }
     TotalBytes = atol(p);
     break;
    }
   }
   fclose(fp);
			if (getenv("TRIDEBUG") == NULL)
     unlink(results1);
  }
 }
 else
 {
  if ((fp=fopen(results1, "r")) != NULL)
  {
			*err = '\0';
	  while (fgets(line, sizeof(line), fp) != NULL)
	  {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR>");
				}
	   if (strlen(err) + strlen(line) < 59000)
					strcat(err,line);
	  }
			SetGlobalErrorPtr(err);
	  fclose(fp);
	 }
		if (getenv("TRIDEBUG") == NULL)
	   unlink(results1);
  return (char *)1;
 }
	
 int res=0; 
	
 if ((res=OMExecuteCmd(execstr,results2)) == 0)
 {
  if ((fp=fopen(results2, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR>");
				}
				printf("%s",line);
   }
   fclose(fp);
  }
		if (getenv("TRIDEBUG") == NULL)
    unlink(results2);
	}
	else
		{
   if ((fp=fopen(results2, "r")) != NULL)
   {
				*err = '\0';
		  while (fgets(line, sizeof(line), fp) != NULL)
		  {
					if (line[strlen(line)-1] == '\n')
					{
						line[strlen(line)-1] = '\0';
						strcat(line,"<BR>");
					}
		   if (strlen(err) + strlen(line) < 59000)
						strcat(err,line);
		  }
				SetGlobalErrorPtr(err);
		  fclose(fp);
		 }
			if (getenv("TRIDEBUG") == NULL)
		   unlink(results2);
		}	

// system(execstr);
// printf("res=%d\n",res);
 return (char *)res;
}

char *StripChar(register char *buffer, register char *removethis, enum Direction feature) /* JAG - 02.27.08 - case IUD-103 */
{
 register char *p; /* JAG - 02.27.08 - case IUD-103 */
 p = buffer;

 switch (feature)
 {
  case both:
   while (strchr(removethis, *p) != NULL && *p != '\0')
    p++;
   if (buffer != p)
    strcpy(buffer, p);

   p = buffer + strlen(buffer) - 1;
   if (p>=buffer)
   {
    while (p >=buffer && *p != '\0' && strchr(removethis, *p) != NULL)
     p--;
    *(p+1)='\0';
   }
   break;

  case leading:
   while (strchr(removethis, *p) != NULL && *p != '\0')
    p++;
   strcpy(buffer, p);
   break;

  case trailing:
   p = buffer + strlen(buffer) - 1;
   while ( p>=buffer &&  *p != '\0' && strchr(removethis, *p) != NULL)
    p--;
   *(p+1)='\0';
   break;

  default:
   break;
 }

 return (buffer);
}

int protocol_cd(char *NewDir)
{
 char cmd[40000] = "";
	char err[60000] = "";
 char results[40000] = "";
 char line[40000] = "";
 FILE *fp = NULL;
 char CurrentDir[40000];
 char WorkDir[40000];
 char myDir[65000] = {""};
 char *buffer = NULL;
 int res = 0;
 int rv = 0;
 int k=0;
	int j=0;
 int len=0;

 if (NewDir != NULL)
 {
  len = strlen(NewDir);
  for (k=0;k<len;k++)
	 {
	 	if ((NewDir[k] == '/' || NewDir[k] == '\\') && j > 0 && myDir[j-1] != '\\')
			{
	 		myDir[j] = '\\';
				j++;
			}	
			else if (NewDir[k] != '/' && NewDir[k] != '\\')
			{
				myDir[j] = NewDir[k];
			 j++;
		 }	
	 }
		myDir[j] = '\0';
	}	

 sprintf(cmd,"winexe --user=%s%%%s //%s 'cmd /c \"mkdir \"%s\"&cd /d \"%s\"&cd\"'",m_UserName,m_Password,m_HostName,myDir,myDir);
 //  output
 // Current directory is \\192.168.3.89\c$\temp\
 //  //
 if (OMExecuteCmd(cmd,results) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
    len = strlen(line);
    for (k=0; k<len; k++)
    {
     if (line[k] == '/')
      line[k] == '\\';
     if (line[k] == '\n' || line[k] == '\r')
      line[k] = '\0';
    }

    if (strcasecmp(myDir,line) == 0)
    {
     res=1;
     break;
    }
   }
   fclose(fp);
  }
 }
 else
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
			*err = '\0';
	  while (fgets(line, sizeof(line), fp) != NULL)
	  {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR>");
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

 if (res == 1)
 {
  // Success
  strcpy(m_TargetDir,myDir);
  rv=1;
 }
 else
 {
  // Failure
  SetGlobalErrorPtr(strerror(errno));
  rv=0;
 }
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

int protocol_mkdir(char *NewDir)
{
	char err[60000] = "";
 char cmd[40000] = "";
 char results[40000] = "";
 char line[40000] = "";
 char WorkDir[40000] = "";
 FILE *fp = NULL;
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
	 	if ((NewDir[k] == '/' || NewDir[k] == '\\') && j > 0 && myDir[j-1] != '\\')
			{
	 		myDir[j] = '\\';
				j++;
			}	
			else if (NewDir[k] != '/' && NewDir[k] != '\\')
			{
				myDir[j] = NewDir[k];
			 j++;
		 }	
	 }
		myDir[j] = '\0';
	}	
	
 sprintf(cmd,"winexe --user=%s%%%s //%s 'cmd /c \"mkdir \"%s\" 2>nul & cd \"%s\" & cd\"'",m_UserName,m_Password,m_HostName,myDir,myDir);
 //  output
 // Current directory is \\192.168.3.89\c$\temp\
 //  //
 if (OMExecuteCmd(cmd,results) == 0)
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
    len = strlen(line);
    for (k=0; k<len; k++)
    {
     if (line[k] == '/')
      line[k] == '\\';
     if (line[k] == '\n' || line[k] == '\r')
      line[k] = '\0';
    }

    if (strcasecmp(myDir,line) == 0)
    {
     res=1;
     break;
    }
   }
   fclose(fp);
  }
 }
 else
 {
  if ((fp=fopen(results, "r")) != NULL)
  {
			*err = '\0';
	  while (fgets(line, sizeof(line), fp) != NULL)
	  {
				if (line[strlen(line)-1] == '\n')
				{
					line[strlen(line)-1] = '\0';
					strcat(line,"<BR>");
				}
	   if (strlen(err) + strlen(line) < 59000)
					strcat(err,line);
	  }
			SetGlobalErrorPtr(err);
	  fclose(fp);
	 }
		if (getenv("TRIDEBUG") == NULL)
	   unlink(results);
		res=1;
  }

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
 return rv;
}

int protocol_put(char *Filename)
{
	char err[60000] = "";
 char cmd[40000] = "";
 char results[4192] = "";
 char line[40000] = "";
 FILE *fp = NULL;
 //
 // Open the file
 //
 int ReadOnly = 0;
 bool ThisMode=true;
 bool BrowseMode=true;
 bool ReplaceReadOnlyFiles=true;
 bool Translate=true;
 struct stat s;
 struct stat t;
 char tgtdir[400000];
 char TargetFileName[40000];
 int in;
 int out;
 int TotalBytes=0;
 int res=1;

 try
 {
  res = stat(Filename,&s);
  if (res != 0)
   throw(1);

  in = open(Filename,O_RDONLY | _O_BINARY );
  if (in == -1)
   throw(1);
  close(in);

  CalculateUNCPath(m_TargetDir,Filename,tgtdir,TargetFileName);
  sprintf(line,"%c:%s",*m_ShareName,tgtdir);
  protocol_mkdir(line);
		
  res = 1;
  sprintf(cmd,"smbclient --user=%s%%%s //%s/%s -c 'cd \"%s\";allinfo \"%s\"'",m_UserName,m_Password,m_HostName,m_ShareName,tgtdir,TargetFileName);
  //  output
  // altname: foo.txt
  // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
  // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
  // attributes: RA (20)
  //  stream: [::$DATA], 0 bytes

  OMExecuteCmd(cmd,results);
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
    if (strstr(line,"attributes:") != NULL && strstr(line,"R") != NULL)
    {
     ReadOnly = 1;
     break;
    }
   }
   fclose(fp);
  }

  if (ReadOnly)
  {
   // File exists - make sure it's writable

   sprintf(cmd,"smbclient --user=%s%%%s //%s/%s -c 'cd \"%s\";setmode \"%s\" -r;allinfo \"%s\"'",m_UserName,m_Password,m_HostName,m_ShareName,tgtdir,TargetFileName,TargetFileName);
   //  output
   // altname: foo.txt
   // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
   // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
   // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
   // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
   // attributes: A (20)
   //  stream: [::$DATA], 0 bytes

   ReadOnly = 0;
   if (OMExecuteCmd(cmd,results) == 0)
   {
    if ((fp=fopen(results, "r")) != NULL)
    {
     while (fgets(line, sizeof(line), fp) != NULL)
     {
      if (strstr(line,"attributes:") != NULL && strstr(line,"R") != NULL)
      {
       ReadOnly = 1;
       break;
      }
     }
     fclose(fp);
    }
   }
   if (ReadOnly)
    throw(2);
  }

  sprintf(cmd,"smbclient --user=%s%%%s //%s/%s -c 'cd \"%s\";put \"%s\" \"%s\";allinfo \"%s\"'",m_UserName,m_Password,m_HostName,m_ShareName,tgtdir,Filename,TargetFileName,TargetFileName);
  //  output
  // altname: foo.txt
  // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
  // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
  // attributes: A (20)
  //  stream: [::$DATA], 1000 bytes

  if (OMExecuteCmd(cmd,results) == 0)
  {
   if ((fp=fopen(results, "r")) != NULL)
   {
    while (fgets(line, sizeof(line), fp) != NULL)
    {
     if (strstr(line,"stream:") != NULL)
     {
      char *p = line + strlen("stream: [::$DATA], ");
      int k = 0;
      for (k=0; k<strlen(line); k++)
      {
       if (*(p+k) == ' ')
        *(p+k) = '\0';
      }
      TotalBytes = atol(p);
      break;
     }
    }
    fclose(fp);
   }
  }
		else
		{
   if ((fp=fopen(results, "r")) != NULL)
   {
				*err = '\0';
		  while (fgets(line, sizeof(line), fp) != NULL)
		  {
					if (line[strlen(line)-1] == '\n')
					{
						line[strlen(line)-1] = '\0';
						strcat(line,"<BR>");
					}
		   if (strlen(err) + strlen(line) < 59000)
						strcat(err,line);
		  }
				SetGlobalErrorPtr(err);
		  fclose(fp);
		 }
			if (getenv("TRIDEBUG") == NULL)
		   unlink(results);
		}

  if (TotalBytes != s.st_size)
   throw(3);
 }
 catch(int errpoint)
 {
  SetGlobalErrorPtr(strerror(errno));
  if (errpoint >= 2)
  {
  }
  if (errpoint >= 3)
  {
  }
  return 0;
 }

 return 1;
}

int protocol_get(char *Filename)
{
 return 1;
}

int protocol_delete(char *Filename)
{
	char err[60000] = "";
 char cmd[40000] = "";
 char results[4192] = "";
 char line[40000] = "";
 FILE *fp = NULL;
 //
 // Open the file
 //
 int ReadOnly = 0;
 bool ThisMode=true;
 bool BrowseMode=true;
 bool ReplaceReadOnlyFiles=true;
 bool Translate=true;
 struct stat s;
 struct stat t;
 char tgtdir[400000];
 char TargetFileName[40000];
 int in;
 int out;
 int TotalBytes=0;
 int res=1;

 try
 {
  res = stat(Filename,&s);
  if (res != 0)
   throw(1);

  in = open(Filename,O_RDONLY | _O_BINARY );
  if (in == -1)
   throw(1);
  close(in);

  CalculateUNCPath(m_TargetDir,Filename,tgtdir,TargetFileName);

  protocol_mkdir(tgtdir);
  res = 1;
  sprintf(cmd,"smbclient --user=%s%%%s //%s/%s -c 'cd \"%s\";allinfo \"%s\"'",m_UserName,m_Password,m_HostName,m_ShareName,tgtdir,TargetFileName);
  //  output
  // altname: foo.txt
  // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
  // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
  // attributes: RA (20)
  //  stream: [::$DATA], 0 bytes

  OMExecuteCmd(cmd,results);
  if ((fp=fopen(results, "r")) != NULL)
  {
   while (fgets(line, sizeof(line), fp) != NULL)
   {
    if (strstr(line,"attributes:") != NULL && strstr(line,"R") != NULL)
    {
     ReadOnly = 1;
     break;
    }
   }
   fclose(fp);
  }

  if (ReadOnly)
  {
   // File exists - make sure it's writable

   sprintf(cmd,"smbclient --user=%s%%%s //%s/%s -c 'cd \"%s\";setmode \"%s\" -r;allinfo \"%s\"'",m_UserName,m_Password,m_HostName,m_ShareName,tgtdir,TargetFileName,TargetFileName);
   //  output
   // altname: foo.txt
   // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
   // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
   // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
   // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
   // attributes: A (20)
   //  stream: [::$DATA], 0 bytes

   ReadOnly = 0;
   if (OMExecuteCmd(cmd,results) == 0)
   {
    if ((fp=fopen(results, "r")) != NULL)
    {
     while (fgets(line, sizeof(line), fp) != NULL)
     {
      if (strstr(line,"attributes:") != NULL && strstr(line,"R") != NULL)
      {
       ReadOnly = 1;
       break;
      }
     }
     fclose(fp);
    }
   }
   if (ReadOnly)
    throw(2);
  }

  sprintf(cmd,"smbclient --user=%s%%%s //%s/%s -c 'cd \"%s\";del \"%s\";allinfo \"%s\"'",m_UserName,m_Password,m_HostName,m_ShareName,tgtdir,TargetFileName,TargetFileName);
  //  output
  // altname: foo.txt
  // create_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // access_time:    Mon Oct 26 11:09:47 AM 2015 MDT
  // write_time:     Mon Oct 26 11:09:47 AM 2015 MDT
  // change_time:    Mon Oct 26 11:12:03 AM 2015 MDT
  // attributes: A (20)
  //  stream: [::$DATA], 1000 bytes
  TotalBytes = -1;

  if (OMExecuteCmd(cmd,results) == 0)
  {
   if ((fp=fopen(results, "r")) != NULL)
   {
    while (fgets(line, sizeof(line), fp) != NULL)
    {
     if (strstr(line,"stream:") != NULL)
     {
      char *p = line + strlen("stream: [::$DATA], ");
      int k = 0;
      for (k=0; k<strlen(line); k++)
      {
       if (*(p+k) == ' ')
        *(p+k) = '\0';
      }
      TotalBytes = atol(p);
      break;
     }
    }
    fclose(fp);
   }
  }
		else
		{
   if ((fp=fopen(results, "r")) != NULL)
   {
				*err = '\0';
		  while (fgets(line, sizeof(line), fp) != NULL)
		  {
					if (line[strlen(line)-1] == '\n')
					{
						line[strlen(line)-1] = '\0';
						strcat(line,"<BR>");
					}
		   if (strlen(err) + strlen(line) < 59000)
						strcat(err,line);
		  }
				SetGlobalErrorPtr(err);
		  fclose(fp);
		 }
			if (getenv("TRIDEBUG") == NULL)
		   unlink(results);
		}

  if (TotalBytes == -1)
   return 1;
 }
 catch(int errpoint)
 {
  SetGlobalErrorPtr(strerror(errno));
  if (errpoint >= 2)
  {
  }
  if (errpoint >= 3)
  {
  }
  return 0;
 }

 return 1;
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

 fprintf(fp,"%s 1>%s 2>&1",cmd,LogFile);
 fclose(fp);

// printf("CMD=%s\n",cmd);

 strcpy(CmdLine, "sh -c '");

 strcat(CmdLine, FileName);
 strcat(CmdLine, "'");

 chmod(FileName, 0777);

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
  execvp(argv[0], argv);   /* In child */
  exit(0);
 }
 else if (pid > 0)
 {
  /* wait */
  waitpid(pid, &stat_val, 0);   /* In parent */
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

char *CreateBatchFile(char *FileName)
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

 sprintf(FileName,"%s/%lu.cmd",local_builddir,FileName);

 return 0;
}

void StrLwr(char *str)
{
 register int i;
 register int len = strlen(str);

 for (i=0; i<len; i++)
  str[i] = tolower(str[i]);
}
