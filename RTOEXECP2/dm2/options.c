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

void AddEnvFromFile(OPTIONS *options,char **p)
{
	char *EnvString;
	char *arg,*eoa,*ep;
	int isq,idq,EndOfLine;

	if (options->argument=='Y')
	{
		/* This option requires an argument */
		if (**p=='\0' || **p=='-')
		{
			fprintf(stderr,"Parameter %s requires an argument\n",
				options->option);
			exit(1);
		}
		arg=*p;
		isq=0;
		idq=0;
		eoa=arg;
		EndOfLine=0;
		while (*eoa)
		{
			if (*eoa == '\'') isq=!isq;
			if (*eoa == '"')  idq=!idq;
			if (*eoa=='\t' || *eoa==' ')
			{
				if (!isq && !idq)
				{
					// not in any quoted string - end of arg
					break;
				}
			}
			eoa++;
		}
		*eoa='\0';
		ep=(eoa-1);
		while (*ep=='\n' || *ep=='\r')
		{
			EndOfLine=1;
			*ep='\0';
			ep--;
		}
		if (*arg=='"')
		{
			while (*arg=='"')
			{
				while (*ep=='"') ep--;
				*(ep+1)='\0';
				arg++;
			}
		}
		else
		if (*arg=='\'')
		{
			while (*arg=='\'')
			{
				while (*ep=='\'') ep--;
				*(ep+1)='\0';
				arg++;
			}
		}
		EnvString=(char *)
			malloc(strlen(options->envname)+strlen(arg)+2);
		sprintf(EnvString,"%s=%s",options->envname,arg);
		if (EndOfLine)
		{
			*p=eoa;
		}
		else
		{
			*p = eoa+1;
		}
	}
	else
	{
		/* This option requires no argument. Set the appropriate
		 * flag to "Y"
		 */
		EnvString=(char *)malloc(strlen(options->envname)+3);
		sprintf(EnvString,"%s=Y",options->envname);
	}
	putenv(EnvString);	
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



void SetOptionsFromFile(OPTIONS *options,int NumOptions,FILE *in)
{
	char	buffer[1024];
	int	s;
	int 	OptionValid;

	while (fgets(buffer,sizeof(buffer),in))
	{
		//
		// read params. Assume first is an option of some sort...
		//
		char *p=buffer;
		while (*p)
		{
			while (*p && *p!='-') p++;
			if (*p == '-')
			{
				// Start of option
				char *opt = p;
				while (*p && *p!=' ' && *p!='\t' && *p!='\n' && *p!='\r') p++;
				*p++='\0';
				//
				// opt is now the option, p points to parameter for this
				// option or the next option or end of this line.
				//
				OptionValid=0;
				for (s=0;s<NumOptions;s++)
				{
					if (strcmp((&options[s])->option,opt)==0)
					{
						/*
						 * This is a match - set the appropriate
						 * Environment Variable.
						 */
						AddEnvFromFile(&(options[s]),&p);
						OptionValid=1;
						break;
					}
				}
				if (!OptionValid)
				{
					fprintf(stderr,"Invalid option %s specified\n",opt);
					exit(1);
				}
			}
		}
	}
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

