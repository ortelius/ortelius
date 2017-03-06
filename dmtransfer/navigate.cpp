#include <stdio.h>
#include "navigate.h"
#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif


HANDLE GetFirstFile(char *dir, FILE_DETAILS *f)
{
#ifdef WIN32
	// Windows Systems
	WIN32_FIND_DATA ffd;
	char *Pattern = (char *)malloc(strlen(dir)+10);
	sprintf(Pattern,"%s\\*",dir);
	HANDLE wHandle = FindFirstFile(Pattern, &ffd);
	f->Directory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?true:false;
	strncpy(f->Filename,ffd.cFileName,sizeof(f->Filename));
	f->Hidden = (ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)?true:false;
	free(Pattern);
	return wHandle;
#else
	// UNIX/Linux Systems
	char workdir[65000] = {""};
	char currfile[65000] = {""};
	struct stat entrystat;
	struct dirent *de;
	DIR *d = opendir(dir);
	
	strcpy(workdir,f->Filename);
	de = readdir(d);
	
	f->Directory = false;
	sprintf(currfile,"%s/%s",workdir,de->d_name);
 if (stat( currfile, &entrystat ) != -1)
	{	
  if (S_ISDIR( entrystat.st_mode ))
	   f->Directory = true;
 }
	strncpy(f->Filename,de->d_name,sizeof(f->Filename));
	f->Hidden = (f->Filename[0]=='.');
	return d;
#endif
}

int GetNextFile(HANDLE h,FILE_DETAILS *f)
{
#ifdef WIN32
	// Windows Systems
	WIN32_FIND_DATA ffd;
	int res = FindNextFile(h, &ffd);
	if (res)
	{
		f->Directory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?true:false;
		strncpy(f->Filename,ffd.cFileName,sizeof(f->Filename));
		f->Hidden = (ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)?true:false;
	}
#else
		// UNIX/Linux Systems
	char workdir[65000] = {""};
	char currfile[65000] = {""};
	struct stat entrystat;
	struct dirent *de;
	strcpy(workdir,f->Filename);
	
	de = readdir(h);
	int res = 0;
	if (de)
	{
		f->Directory = false;
		sprintf(currfile,"%s/%s",workdir,de->d_name);
		
  if (stat( currfile, &entrystat ) != -1)
		{	
   if (S_ISDIR( entrystat.st_mode ))
		   f->Directory = true;
  }
		strncpy(f->Filename,de->d_name,sizeof(f->Filename));
		f->Hidden = (f->Filename[0]=='.');
		res = 1;
	}
#endif
	return res;
}
