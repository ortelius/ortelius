// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**************************************************************************
* license.cpp
* -----------
* This module controls the licensing aspect of any server component
* built with trinem IPC technology.
*
* Ensures the validity of the specified license file. Decrypts the
* license key and compares the decrypted contents with the unencrypted
* information contained within the same file.
*
* Note, triipc uses this information to record the number of distinct
* client nodes (based on their IP address).
*
* Release    Author     Date       Description
* -------    ------     --------   -----------
*   1        PAG        May 04     Initial Release
*   2        PAG        08/06/04   Minor Bug Fix - Terminate Node String
*   3        PAG        12/11/05   Change to Encryption Algorithm + Bug Fix
*                                  to handle non-alpha characters in hostname
*
**************************************************************************/
#include <stdio.h>
#include <string.h> /* RHT */
#ifdef WIN32
extern bool globalRunningAsService;
#include <winsock2.h>
#define STRCASECMP stricmp
#else
#include <strings.h>
#define STRCASECMP strcasecmp
#endif
#include "license.h"
#include <stdlib.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif


// #include "memory.h"

// Define the Product Code below. Note, all products must have their own
// Product Code in order to generate a different hash key. This guarantees
// that licenses for one product are not valid for any other.
//
#define PRODUCT_CODE 0x4445504c /* DEPL */

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializing the array with a NONZERO seed */
static void sgenrand(unsigned long seed)
{
    /* setting initial seeds to mt[N] using         */
    /* the generator Line 25 of Table 1 in          */
    /* [KNUTH 1981, The Art of Computer Programming */
    /*    Vol. 2 (2nd Ed.), pp102]                  */
    mt[0]= seed & 0xffffffff;
    for (mti=1; mti<N; mti++)
        mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
}

static unsigned long genrand()
{
    unsigned long y;
    static unsigned long mag01[2]={0x0, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if sgenrand() has not been called, */
            sgenrand(4357); /* a default initial seed is used   */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti = 0;
    }

    y = mt[mti++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L(y);

    return y;
}

bool License::CheckForInvalidHostName()
{
	char ThisHostName[128];

	int res = gethostname(ThisHostName,sizeof(ThisHostName));
	ThisHostName[sizeof(ThisHostName)-1]='\0';
	if (HostName)
	{
		// printf("HostName=[%s]\n",HostName);
		char *HostNameToCompare = strdup(HostName);
		char *x=HostNameToCompare;
		while (*x && *x != '@') x++;
		if (*x=='@') *x='\0';
		// Return 0 (success) if hostname matches
		// printf("Comparing ThisHostName=[%s] HostNameToCompare=[%s]\n",ThisHostName,HostNameToCompare);
		bool res = STRCASECMP(ThisHostName,HostNameToCompare)?true:false;   // RHT -warn removed bool cast - 25/08/2006
		free(HostNameToCompare);
		// printf("res=%s\n",res?"true":"false");
		return res;
	}
	else
	{
		// no hostname in license file - return successseed=seed+k2;
		return 0;
	}
}

bool KeyCompare(char *s1,char *s2)
{
	// Compares two strings character by character, ignoring anything that falls
	// outside the "comparison" range (A-Z and @). This is to allow strange
	// characters in the file to be ignored
	//
#ifdef DUMP_SEEDS
	printf("Comparing [%s] and [%s]\n",s1,s2);
#endif
	unsigned int s1len=strlen(s1);
	unsigned int s2len=strlen(s2);
	unsigned int minlen=(s1len<s2len)?s1len:s2len;
	for (unsigned int n=0;n<minlen;n++)
	{
		if (s1[n]>='A' && s1[n]<='Z')
		{
			// Upper case characters are compared normally
			if (s1[n]!=s2[n]) return true;	// mismatch
#ifdef DUMP_SEEDS
			else printf("s1[%d]=%c match\n",n,s1[n]);
#endif
		}
		else
		if (s1[n]>='a' && s1[n]<='z')
		{
			// Lower case characters should match equivalent upper case character
			if ((s1[n] & 0xdf) != s2[n]) return true;
#ifdef DUMP_SEEDS
			else printf("s1[%d]=%c match\n",n,s1[n]);
#endif
		}
		else
		if (s1[n]>='0' && s1[n]<='9')
		{
			// Numerics 0-9 match @-I
			if ((int)s1[n]+16 != (int)s2[n]) return true;
#ifdef DUMP_SEEDS
			else printf("s1[%d]=%c match\n",n,s1[n]);
#endif
		}
		else
		if (s1[n]==' ')
		{
			// Spaces match @
			if (s2[n]!='@') return true;	// mismatch
#ifdef DUMP_SEEDS
			else printf("s1[%d]=%c match\n",n,s1[n]);
#endif
		}
		else
		{
			// Everything else should match a 'X' char
			if (s2[n]!='X') return true;	// mismatch
#ifdef DUMP_SEEDS
			else printf("s1[%d]=%c match\n",n,s1[n]);
#endif
		}
	}
#ifdef DUMP_SEEDS
	printf("returning false (match)\n");
#endif
	return false;	// match
}

int License::CheckKeyAgainstDetails(char *Key,char *Customer,char *ExpiryDate,char *PassedNodes,char *HostName)
{
	char customer[11];
	char datestring[10];
	char nodes[5];
	char Nodes[5];
	char hostname[9];
	unsigned int k,seed;
	int Invalid;
	int NodeCount;
	unsigned char xorval;
	unsigned int KeyLength;
#ifdef DUMP_SEEDS
printf("Customer=[%s]\n",Customer);
printf("ExpiryDate=[%s]\n",ExpiryDate);
printf("PassedNodes=[%s]\n",PassedNodes);
printf("HostName=[%s]\n",HostName);
#endif

	if (PassedNodes)
	{
		NodeCount=atoi(PassedNodes);
		sprintf(Nodes,"%04d",NodeCount);
	}
	else
	{
		NodeCount=0;
	}
	seed=PRODUCT_CODE;
#ifdef DUMP_SEEDS
	printf("seed=%d\n",seed);
#endif
	for (k=0;k<strlen(Customer);k++) seed=seed+(int)Customer[k];
#ifdef DUMP_SEEDS
	printf("Customer=[%s]\n",Customer);
	printf("seed=%d\n",seed);
#endif
	for (k=0;k<=7;k++) seed=seed+(int)ExpiryDate[k];
#ifdef DUMP_SEEDS
printf("seed=%d\n",seed);
#endif
seed=seed+NodeCount;
#ifdef DUMP_SEEDS
printf("seed=%d\n",seed);
#endif
	if (HostName)
	{
		// Hostname is optional in license file - demo licenses have
		// no hostname
		//
		for (k=0;k<strlen(HostName);k++) seed=seed+(int)HostName[k];
		KeyLength=34;
	}
	else
	{
		KeyLength=24;
	}
#ifdef DUMP_SEEDS
printf("(hostname) seed=%d\n",seed);
#endif
	if (strlen(Key)!=KeyLength) return 1; // Invalid Key Length
	sgenrand(seed);

	for (k=0;k<KeyLength;k++)
	{
		if (k!=4 && k!=9 && k!=14 && k!=19 && k!=24 && k!=29)
		{
			xorval=(unsigned char)(genrand() % 26)+1;
			Key[k]=Key[k] - xorval;
			if (Key[k]<'@') Key[k]=Key[k]+26;
		}
	}

	if (KeyLength==34)
	{
		//
		// Real license (hostname in license file)
		//
		datestring[4]=Key[0]-16;
		nodes[0]=Key[1]-16;
		customer[3]=Key[2];
		hostname[3]=Key[3];

		customer[2]=Key[5];
		hostname[5]=Key[6];
		datestring[1]=Key[7]-16;
		customer[9]=Key[8];

		customer[8]=Key[10];
		nodes[1]=Key[11]-16;
		customer[0]=Key[12];
		datestring[3]=Key[13]-16;

		datestring[0]=Key[15]-16;
		datestring[6]=Key[16]-16;
		customer[1]=Key[17];
		nodes[2]=Key[18]-16;

		hostname[7]=Key[20];
		hostname[4]=Key[21];
		customer[4]=Key[22];
		nodes[3]=Key[23]-16;

		hostname[0]=Key[25];
		customer[5]=Key[26];
		hostname[6]=Key[27];
		hostname[2]=Key[28];

		datestring[7]=Key[30]-16;
		customer[7]=Key[31];
		hostname[1]=Key[32];
		customer[6]=Key[33];
	}
	else
	{
		//
		// Demo license (no hostname in license file)
		//
		customer[0]=Key[0];
		datestring[0]=Key[1]-16;
		customer[1]=Key[2];
		datestring[6]=Key[3]-16;

		customer[2]=Key[5];
		customer[3]=Key[6];
		datestring[3]=Key[7]-16;
		customer[4]=Key[8];

		customer[5]=Key[10];
		datestring[4]=Key[11]-16;
		customer[6]=Key[12];
		datestring[1]=Key[13]-16;

		customer[7]=Key[15];
		customer[8]=Key[16];
		customer[9]=Key[17];
		datestring[7]=Key[18]-16;

		nodes[0]=Key[20]-16;
		nodes[1]=Key[21]-16;
		nodes[2]=Key[22]-16;
		nodes[3]=Key[23]-16;
	}
	nodes[4]='\0';		// Release 2 BUG FIX

	datestring[2]='/';
	datestring[5]='/';
	datestring[8]='\0';
	customer[10]='\0';
	hostname[8]='\0';

	Invalid=0;
	if (KeyCompare(Customer,customer))
	{
		Invalid=1;
#ifdef DUMP_SEEDS
		printf("Customer mismatch Customer=[%s] customer=[%s]\n",Customer,customer);
#endif
	}
	if (strcmp(datestring,ExpiryDate))
	{
		Invalid=1;
#ifdef DUMP_SEEDS
		printf("datestring invalid, datestring=[%s], ExpiryDate=[%s]\n",datestring,ExpiryDate);
#endif
	}
	if (atoi(nodes)!=NodeCount)
	{
		Invalid=1;
#ifdef DUMP_SEEDS
		printf("node mismatch, nodes=[%s], NodeCount=%d\n",nodes,NodeCount);
#endif
	}
	if (HostName && KeyCompare(HostName,hostname))
	{
		Invalid=1;
#ifdef DUMP_SEEDS
		printf("HostName mismatch, HostName=[%s] hostname=[%s]\n",HostName,hostname);
#endif
	}
	return Invalid;
}

int License::ValidateLicenseFile(FILE *in,int *Day,int *Month,int *Year,int *NodeCount,char **pHostName)
{
	char *LicenseKey=(char *)0;
	char *CustomerName=(char *)0;
	char *ExpiryDate=(char *)0;
	char *Nodes=(char *)0;
	char *HostName=(char *)0;
	char linebuf[1024];
	char *expdate;
	int x;
	char *z,*mptr;
	int line = 0;
	int res=0;

	while (fgets(linebuf,sizeof(linebuf),in))
	{
        // RHT+ BEGIN - handles both windows and unix - 11/08/2006 re-added 25/08/2006
        // Remove CRs and LFs from end of line
        for( int n = strlen(linebuf) - 1; (n >= 0) && ((linebuf[n] == 10) || (linebuf[n] == 13)); n-- )
        {
    		linebuf[n]='\0';
        }
        // RHT+ END - re-added 25/08/2006

		line++;

		if (strncmp(linebuf,"LICENSE KEY:",12)==0)
		{
			x=12;
			while (linebuf[x]==' ' || linebuf[x]=='\t') x++;
			LicenseKey=strdup(&(linebuf[x]));
		}
		else if (strncmp(linebuf,"CUSTOMER NAME:",14)==0)
		{
			x=14;
			while (linebuf[x]==' ' || linebuf[x]=='\t') x++;
			CustomerName=strdup(&(linebuf[x]));
			// Customer Name has to be at least 10 chars + term
			// CustomerName=(char *)Realloc(CustomerName,11);
		}
		else if (strncmp(linebuf,"EXPIRY DATE:",12)==0)
		{
			x=12;
			while (linebuf[x]==' ' || linebuf[x]=='\t') x++;
			ExpiryDate=strdup(&(linebuf[x]));
			expdate=strdup(ExpiryDate);
			z=expdate;
			while (*z!='/' && *z) z++;
			*z='\0';
			*Day=atol(expdate);
			z++;
			mptr=z;
			while (*z!='/' && *z) z++;
			*z='\0';
			*Month=atol(mptr);
			*Year=atol(z+1)+2000;
			free(expdate);
		}
		else if (strncmp(linebuf,"NODES:",6)==0)
		{
			x=6;
			while (linebuf[x]==' ' || linebuf[x]=='\t') x++;
			Nodes=strdup(&(linebuf[x]));
			*NodeCount=atol(Nodes);
		}
		else if (strncmp(linebuf,"HOSTNAME:",9)==0)
		{
			x=9;
			while (linebuf[x]==' ' || linebuf[x]=='\t') x++;
			HostName=strdup(&(linebuf[x]));
			// Host Name has to be at least 8 chars + term
			// HostName=(char *)Realloc(HostName,9);
		}
		else
		{
			// License is invalid
			fprintf( stderr,"Mistake at line %d of license file \"%s\"\n",
			         line, this->LicenseFileName );
			res=1;
		}
	}
	if ((PRODUCT_CODE == 0x50524f4a) || (PRODUCT_CODE == 0x4445504c))
	{
		// Project Manager and Deployment Manager have no node count
		if (!CustomerName || !ExpiryDate || !LicenseKey) res=1;	// missing entry
	}
	else
	{
		if (!CustomerName || !Nodes || !ExpiryDate || !LicenseKey) res=1;	// missing entry
	}
	if (LicenseKey && strlen(LicenseKey)>24 && !HostName)  res=1;	// Locked key but no hostname
	if (res==0)
	{
		// So far, so good....
		*pHostName=HostName;
		if (CheckKeyAgainstDetails(LicenseKey,CustomerName,ExpiryDate,Nodes,HostName))
		{
			// License is invalid
			res=1;
		}
		else
		{
			// License is valid
			res=0;
		}
	}
	if (CustomerName) free(CustomerName);
	if (ExpiryDate)   free(ExpiryDate);
	if (LicenseKey)   free(LicenseKey);
	if (Nodes)        free(Nodes);
	return res;
}

License::License()
{
	LicenseFileName=(char *)0;
	HostName=(char *)0;
	Day=0;
	Month=0;
	Year=0;
	NodeCount=0;
}

License::~License()
{
	if (LicenseFileName) free(LicenseFileName);
	if (HostName) free(HostName);
}

void License::OpenLicenseFile(const char *BaseDir,const char *FileName)
{
	this->LicenseFileName=(char *)malloc(strlen(FileName)+strlen(BaseDir)+2);
	strcpy(this->LicenseFileName,BaseDir);
#ifdef WIN32
	strcat(this->LicenseFileName,"\\");
#else
	strcat(this->LicenseFileName,"/");
#endif
	strcat(this->LicenseFileName,FileName);
}

time_t License::CurrentLicenseTimeStamp()
{
        struct stat     details;

        stat(this->LicenseFileName,&details);
        return details.st_mtime;
}

// RHT 11/04/2012 - change return type and change to not exit here
LicenseError License::LoadLicenseFile()
{
	FILE *in=fopen(this->LicenseFileName,"r");
	if (!in)
	{
		//fprintf(stderr,"Could not find or open license file \"%s\"\n",this->LicenseFileName);
		//exit(1);
		return CouldNotOpenLicenseFile;
	}
	if (ValidateLicenseFile(in,&Day,&Month,&Year,&NodeCount,&HostName)!=0)
	{
		// License file has failed validation. Nothing to do but exit.
		//fprintf(stderr,"Invalid or corrupt license file \"%s\"\n",this->LicenseFileName);
		//exit(1);
		return CorruptLicenseFile;
	}
	this->OurLicenseTimeStamp = CurrentLicenseTimeStamp();
	fclose(in);
	return LicenceOkay;
}

void License::GetExpiryDate(int *pDay,int *pMonth,int *pYear)
{
	*pDay=Day;
	*pMonth=Month;
	*pYear=Year;
}

int License::GetAllowedNodeCount()
{
	return NodeCount;
}

bool License::CheckForExpiredLicense()
{
	time_t		LocalEpochTimeSecs;
	struct tm	*LocalEpochTime;
	unsigned long	LocalDate;
	unsigned long	ExpiryDate;

	time(&LocalEpochTimeSecs);
	LocalEpochTime=localtime(&LocalEpochTimeSecs);
	LocalDate=(
		((LocalEpochTime->tm_year+1900)*10000)+
		((LocalEpochTime->tm_mon+1)*100)+
		((LocalEpochTime->tm_mday))
	);
	ExpiryDate=(
		((Year)*10000)+
		((Month)*100)+
		Day
	);

	return (LocalDate > ExpiryDate);
}

bool License::ReadRequired()
{
	return (this->CurrentLicenseTimeStamp() > this->OurLicenseTimeStamp);
}
