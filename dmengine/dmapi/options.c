/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* options.c - reads command line options and sets the appropriate environment
 * variables to be picked up by the rest of the application.
 */

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

#include "memory.h"

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

void SetEnvironmentVariablesFromFile(OPTIONS *options,int NumOptions,char *FileName)
{
	FILE	*in;
	char	buf[1024];
	char	*CommentPtr;
	int	c;
	char	*cPtr;
	char	*bPtr;
	char	*EnvName;
	char	*EnvString;
	char	*Value;
	int	LineNo=0;
	int	EnvNameValid;

	in=fopen(FileName,"r");
	if (in)
	{
		/* File is readable and we've opened it okay */
		while (!feof(in))
		{
			LineNo++;
			fgets(buf,sizeof(buf),in);
			CommentPtr=strchr(buf,'#');
			if (CommentPtr) *CommentPtr='\0';
			/* Remove trailing free space (if any) */
			c=strlen(buf)-1;
			while ((buf[c]==' ' || buf[c]=='\t' || buf[c]=='\n') && c) c--;
			buf[c+1]='\0';
			/* Remove leading free space (if any) */
			cPtr=buf;
			while (*cPtr==' ' || *cPtr=='\t') cPtr++;
			if (strlen(cPtr))
			{
				/* Now look for the = sign and split the
				 * line at that point
				 */
				bPtr=strchr(cPtr,'=');
				if (bPtr==(char *)0)
				{
					fprintf(stderr,
						"Syntax error at line %d in file %s\n",
						LineNo,
						FileName);
					exit(1);
				}
				else
				{
					EnvName=cPtr;
					Value=bPtr+1;
					*bPtr='\0';
					/* Remove any leading whitespace from "Value" */
					cPtr=Value;
					while (*cPtr==' ' || *cPtr=='\t') cPtr++;
					Value=cPtr;
					/* Remove any trailing whitespace from "EnvName" */
					cPtr=(EnvName+strlen(EnvName))-1;
					while (cPtr>EnvName &&
						 (*cPtr==' ' || *cPtr=='\t' || *cPtr=='\n')) cPtr--;
					*(cPtr+1)='\0';
					/*
					 * Now check that the EnvName specified is in our list
					 * of variables that can be set or checked by this
					 * application.
					 */
					EnvNameValid=0;
					for (c=0;c<NumOptions;c++)
					{
						if (strcmp((&options[c])->envname,EnvName)==0)
						{
							EnvNameValid=1;
							break;
						}
					}
					if (EnvNameValid)
					{
						/* Only set the environment variable if it is not
						 * already declared. This ensures that a locally
						 * set environment variable will take precedence
						 * over one set in a configuration file.
						 */
						if (getenv(EnvName)==(char *)0)
						{
							EnvString=(char *)
								malloc(strlen(EnvName)+strlen(Value)+2);
							sprintf(EnvString,"%s=%s",EnvName,Value);
							putenv(EnvString);
							/* Do not free the string since it is now part
							 * of the environment.
							 */
						}
					}
					else
					{
						fprintf(stderr,
							"Invalid option '%s' at line %d of file %s\n",
							EnvName,
							LineNo,
							FileName);
						exit(1);
					}
				}
			}
		}
		fclose(in);
	}
}

#if 0
void LoadGlobalVariables(OPTIONS *options,int NumOptions)
{
	char		*HarvestHome=(char *)0;
	int		res;
	struct stat	statbuf;
	char		*HomeDir;
	char		*HarEnvFile;
	char		*HomeEnvFile;

	/* Check to see if there is a harvest environment file in $HARVESTHOME */
	HarvestHome=getenv("HARVESTHOME");
	if (HarvestHome)
	{
		/*
		 * First look in the user's home directory.
		 */
		HomeDir=getenv("HOME");
		if (HomeDir)
		{
			HomeEnvFile=(char *)malloc(strlen(HomeDir)+9);
			sprintf(HomeEnvFile,"%s/.harenv",HomeDir);
			res=stat(HomeEnvFile,&statbuf);
			if (res==0)
			{
				SetEnvironmentVariablesFromFile(options,NumOptions,HomeEnvFile);
			}
			free(HomeEnvFile);
		}
		/*
		 * Now look in $HARVESTHOME
		 */
		HarEnvFile=(char *)malloc(strlen(HarvestHome)+8);
		sprintf(HarEnvFile,"%s/harenv",HarvestHome);
		res=stat(HarEnvFile,&statbuf);
		if (res==0)
		{
			/* File $HARVESTHOME/harenv exists */
			SetEnvironmentVariablesFromFile(options,NumOptions,HarEnvFile);
		}
		else
		{
			/* File $HARVESTHOME/harenv does not exist or
			 * is not readable - try /etc/harenv
			 */
			res=stat("/etc/harenv",&statbuf);
			if (res==0)
			{
				/* File /etc/harenv exists */
				SetEnvironmentVariablesFromFile(options,NumOptions,"/etc/harenv");
			}
		}
		free(HarEnvFile);
	}
	else
	{
		fprintf(stderr,"HARVESTHOME is not set\n");
		exit(1);
	}
}
#endif

int ScanOptions(OPTIONS *options,int NumOptions,int argc, char **argv)
{
	int p;
	int s;
	int OptionValid;

	//LoadGlobalVariables(options,NumOptions);

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

