/*
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "platform.h"
#include "triodbc.h"

#include "crypto.h"
#include "options.h"


OPTIONS options[]={
	{ "-dsn", 'Y', "setodbcdsn" },
	{ "-usr", 'Y', "setodbcusr" },
	{ "-pw", 'Y',  "setodbcpw" },
	{ "-test", 'N',  "setodbctest" },
	{ "-notest", 'N',  "setodbcnotest" }
};


void ExitWithError(char *errtext)
{
	fprintf(stderr,"%s\n",errtext);
	exit(1);
}


#ifdef WIN32
char *getHomeDirectory(const char *argv0)
{
	char moduleFileName[1024];
	GetModuleFileNameA(NULL, moduleFileName, sizeof(moduleFileName));
	// Split off the name of the executable
	char *lastsep = strrchr(moduleFileName, '\\');
	if(!lastsep) {
		// This should never happen!
		printf("Module filename is '%s'\n", moduleFileName);
		return strdup(".");
	}
	*lastsep = '\0';
	// If we still have a slash, then do dirname
	if((lastsep = strrchr(moduleFileName, '\\')) != NULL) {
		*lastsep = '\0';
	} else {
		// otherwise must be a relative path, so append "\.."
		strcat(moduleFileName, "\\..");
	}
	return strdup(moduleFileName);
}
#else
char *getHomeDirectory(const char *argv0)
{
	char *basedir = strdup(argv0);
	// Split off the name of the executable
	char *lastsep = strrchr(basedir, '/');
	if(!lastsep) {
		// This should never happen!
		printf("Basedir is '%s'\n", basedir);
		return strdup(".");
	}
	*lastsep = '\0';
	// If we still have a slash, then do dirname
	if((lastsep = strrchr(basedir, '/')) != NULL) {
		*lastsep = '\0';
	} else {
		// otherwise must be a relative path, so append "\.."
		strcat(basedir, "/..");
	}
	return basedir;
}
#endif


void ensurePassphraseFile(const char *homeDir)
{
	char ppfilename[1024];
	sprintf(ppfilename, "%s%sdm.asc", homeDir, DIR_SEP_STR);

	try {
		// Does the passphrase file exist?
		struct stat sb;
		if(stat(ppfilename, &sb) == -1) {
			if(errno == ENOENT) {
				// Doesn't exist - so create it
				printf("Creating passphrase file...\n");
				createPassphraseFile(ppfilename);
			} else {
				printf("FATAL ERROR: Unable to stat passphrase file\n");
				exit(1);
			}
		}

		// Read the passphrase file to allow us to do encryption
		if(!readPassphraseFile(ppfilename)) {
			printf("FATAL ERROR: Unable to initialise encryption\n");
			exit(1);
		}
	} catch(const char *e) {
		printf("ERROR: %s\n", e);
		printf("FATAL ERROR: Passphrase file mechanism failed\n");
		exit(1);
	}
}


int main(int argc,char **argv)
{
	char	DSN[50];
	char	UserName[40];
	char	Password[40];
	char	*Buffer;
	long	BufferLen;
	long	i;
	int 	opfile;
	char	*PathName;
	char	*HARVESTHOME;

	int n = ScanOptions(options, sizeof(options)/sizeof(options[0]), argc, argv);

	printf("Set ODBC Connection Parameters\n------------------------------\n");
	if(n >= argc) {
		fprintf(stderr,"USAGE: setodbc [-dsn <datasource>] [-usr <username>] [-pw <password>] <filename>\n");
		exit(1);
	}

	// Make sure the encryption layer is ready
	char *homeDir = getHomeDirectory(argv[0]);
	ensurePassphraseFile(homeDir);

	PathName=(char *)argv[n];

	const char *setodbcdsn = getenv("setodbcdsn");
	if(!setodbcdsn) {
		printf("DataSourceName:        ");
		fflush(stdin);
		fgets(DSN,sizeof(DSN),stdin);
		DSN[strlen(DSN)-1]='\0';
	} else {
		strcpy(DSN, setodbcdsn);
	}

	const char *setodbcusr = getenv("setodbcusr");
	if(!setodbcusr) {
		printf("Database User Name:    ");
		fflush(stdin);
		fgets(UserName,sizeof(UserName),stdin);
		UserName[strlen(UserName)-1]='\0';
	} else {
		strcpy(UserName, setodbcusr);
	}

	const char *setodbcpw = getenv("setodbcpw");
	if(!setodbcpw) {
		printf("Database Password:     ");
		fflush(stdin);
		fgets(Password,sizeof(Password),stdin);
		Password[strlen(Password)-1]='\0';
	} else {
		strcpy(Password, setodbcpw);
	}

	/*
	* Now put everything together and encrypt
	*/
	BufferLen=strlen(DSN)+strlen(UserName)+strlen(Password)+10;
	Buffer=(char *)malloc(BufferLen);
	for (i=0;i<BufferLen;i++) Buffer[i]='\0';
	strcpy(&(Buffer[0]),DSN);
	strcpy(&(Buffer[strlen(Buffer)+1]),UserName);
	strcpy(&(Buffer[strlen(Buffer)+strlen(UserName)+2]),Password);
	try {
		char *EncBuffer = encryptValue(Buffer, BufferLen);
		/*
		* Now write it to the output file
		*/
		
		opfile=open(PathName,O_WRONLY | O_CREAT | O_TRUNC,0600);
		if (opfile == -1)
		{
			perror("Failed to open output file for writing");
			exit(1);
		}
		if (write(opfile,EncBuffer,strlen(EncBuffer)) == -1)
		{
			perror("Error writing to odbc file");
			exit(1);
		}
		close(opfile);
	} catch(const char *e) {
		printf("ERROR: %s\n", e);
		exit(1);
	}
	printf("\nFile \"%s\" created successfully.\n\n",PathName);

	char Answer[10];
	Answer[0] = '\0';
	const char *setodbctest = getenv("setodbctest");
	const char *setodbcnotest = getenv("setodbcnotest");
	if(!setodbctest && !setodbcnotest) {
		printf("Do you want to try to connect to the database now? ");
		fgets(Answer,sizeof(Answer),stdin);
	} else if(setodbctest) {
		Answer[0] = 'Y';
		Answer[1] = '\0';
	}

	if (Answer[0]=='Y' || Answer[0]=='y')
	{
		//
		// Now test the connection...
		//
		printf("Attempting to connect to the database...\n");

		int len = 0;
		char *details = decryptFile(PathName, &len);
		const char *dsn = details;
		const char *user = NULL;
		const char *pass = NULL;

		int nulls = 0;
		for(int n = 0; n < len; n++) {
			if(!details[n]) {
				nulls++;
				if(nulls == 1) {
					user = &details[n+1];
				} else if(nulls == 2) {
					pass = &details[n+1];
				}
			}
		}
		if(nulls != 2) {
			printf("FATAL ERROR: dm.odbc is not a valid odbc file");
			exit(1);
		}
		//printf("dsn = '%s'; user = '%s'; pass = '%s'\n", (const char*) details, user, pass);

		triODBC		odbc;

		//SQLRETURN res = (SQLRETURN)odbc.ConnectToDataSource(PathName);
		SQLRETURN res = odbc.ConnectToDataSource(NULL, (char*) dsn, (char*) user, (char*) pass);
		if (res == SQL_ERROR)
		{
			fprintf(stderr,"Failed to connect to database\n");
			exit(1);
		}
		else
		{
			printf("Connected successfully...\nRetrieving DM Schema Version information...\n");
			triSQL *sql = odbc.GetSQL();
			long schemaver;
			sql->BindColumn(1, SQL_C_SLONG, &schemaver, sizeof(schemaver));
			res = sql->ExecuteSQL("select schemaver from dm_tableinfo");
			if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
				printf("No DM Schema in Database.\nDatabase Connection OK\n");
			} else {
				res = sql->FetchRow();
				sql->CloseSQL();
				if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
					printf("No DM Schema in Database.\nDatabase Connection OK\n");
				} else {
					printf("DM Schema Version from Database: %ld\nDatabase Connection OK\n", schemaver);
				}
			}
			delete sql;
		}
	}
	else
	{
		// User doesn't want to test the connection
		exit(0);
	}

	return 0;
}

#ifdef WIN32
void *Malloc(size_t s)
{
	return malloc(s);
}

void Free(void *ptr)
{
	free(ptr);
}

// RHT+ BEGIN - compatability - 25/08/2006
void FreeAnyThread(void *ptr)
{
    free(ptr);
}
// RHT+ END - 25/08/2006

void *Realloc(void *OldPtr,size_t NewSize)
{
	return realloc(OldPtr,NewSize);
}

char *Strdup(const char *OldString)
{
	return strdup(OldString);
}
#endif
