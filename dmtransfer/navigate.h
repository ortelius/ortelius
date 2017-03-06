#ifdef WIN32
#include <windows.h>
#include <tchar.h> 
#include <strsafe.h>
#else
#include <sys/types.h>
#include <dirent.h>
#define HANDLE DIR *
#endif

typedef struct file_details_tag{
char	Filename[65000];
bool	Directory;
bool	Hidden;
} FILE_DETAILS;

HANDLE GetFirstFile(char *dir, FILE_DETAILS *f);
int GetNextFile(HANDLE h,FILE_DETAILS *f);