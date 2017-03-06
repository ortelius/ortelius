typedef struct options_tag{
char	*option;
char	argument;
char	*envname;
} OPTIONS;

#ifndef WIN32
#ifndef __LINUX__
extern "C"
{
	int ScanOptions(OPTIONS *,int,int,char **);
}
#else
int ScanOptions(OPTIONS *,int,int,char **);
#endif
#else
// Windows
int ScanOptions(OPTIONS *,int,int,char **);
#endif
bool OptionSet(char *OptionName);
