/* options.c - reads command line options and sets the appropriate environment
 * variables to be picked up by the rest of the application.
 */


// #include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __SUNOS__
#include <strings.h>
#else
#include <string.h>
#endif

#include "options.h"

#ifdef WIN32
//
// Prevent ISO/Posix warnings
//
#define putenv _putenv
#endif


bool OptionSet(char *OptionName)
{
	char *t = getenv(OptionName);
	return (t && t[0]=='Y')?true:false;
}


void AddEnv(OPTIONS *options,int p,int argc,char **argv)
{
	char *EnvString;

	if (options->argument=='Y')
	{
		/* This option requires an argument */
		if (p+1 >= argc || *argv[p+1]=='-')
		{
			/* But the next value is another option! */
			fprintf(stderr,"Parameter %s requires an argument\n",
				options->option);
			exit(1);
		}
		/*
		 * Construct a string in the form envname=value and use
		 * "putenv" to add it to the environment. Note, we don't
		 * "free" the malloced string since the string is simply
		 * pointed to from the modified environment.
		 */
		EnvString=(char *)
			malloc(strlen(options->envname)+strlen(argv[p+1])+2);
		sprintf(EnvString,"%s=%s",options->envname,argv[p+1]);
	}
	else
	{
		/* This option requires no argument. Set the appropriate
		 * flag to "Y"
		 */
		EnvString=(char *)
			malloc(strlen(options->envname)+3);
		sprintf(EnvString,"%s=Y",options->envname);
	}
	putenv(EnvString);	
}



int ScanOptions(OPTIONS *options,int NumOptions,int argc, char **argv)
{
	int p;
	int s;
	int OptionValid;

	for (p=1;p<argc;p++)
	{
		/*
		 * For each passed parameter...
		 */
		if (*argv[p]=='-')
		{
			/*
			 * This is an option switch.
			 */
			OptionValid=0;
			for (s=0;s<NumOptions;s++)
			{
				if (strcmp((&options[s])->option,argv[p])==0)
				{
					/*
					 * This is a match - set the appropriate
					 * Environment Variable.
					 */
					AddEnv(&(options[s]),p,argc,argv);
					OptionValid=1;
					if ((&options[s])->argument=='Y')
					{
						p++;	/* point to next item */
					}
					break;
				}
			}
			if (!OptionValid)
			{
				fprintf(stderr,"Invalid option %s specified\n",argv[p]);
				exit(1);
			}
		}
		else break;	/* end of options */
	}
	return p;
}

