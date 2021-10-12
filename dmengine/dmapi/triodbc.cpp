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

//#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#ifdef WIN32
// These files need to be picked up from the Microsoft Development Environment
#include <windows.h>
#include <windef.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#else
// For UNIX systems, pick up the local versions of these files...
#include "sql.h"
#include "sqlext.h"
#include "sqltypes.h"
#endif
// #include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//RHT-:#ifdef WIN32
#include <string.h>
//RHT-:#else
/*RHT+:*/ #ifndef WIN32
#include <strings.h>
#include <unistd.h>
#endif
#include <stdarg.h>
/*
#include <varargs.h>
*/

#include "platform.h"
#include "odbcseeds.h"
#include "memory.h"
#include "autoptr.h"

#include "dm.h"
#include "triodbc.h"

#ifdef WIN32
#include <io.h>
#endif

#ifndef SETODBC
#include "expr.h"
#include "tinyxml.h"
#endif
/*******************************************************************
 *
 * RANDOM NUMBER GENERATOR COMPONENTS FOR CONNECTION PARAMETERS
 * ENCRYPTION....
 *
 ******************************************************************/

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
void sgenrand( unsigned long seed )
{
    /* setting initial seeds to mt[N] using         */
    /* the generator Line 25 of Table 1 in          */
    /* [KNUTH 1981, The Art of Computer Programming */
    /*    Vol. 2 (2nd Ed.), pp102]                  */
    mt[0]= seed & 0xffffffff;
    for (mti=1; mti<N; mti++)
        mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
}


unsigned long genrand()
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

/*******************************************************************
 *
 * END RANDOM NUMBER GENERATOR COMPONENTS FOR CONNECTION PARAMETERS
 * ENCRYPTION....
 *
 ******************************************************************/

void DecryptBuffer(char *Buffer,long BufferLen)
{
	int i;

	sgenrand(odbcseed3);
	for (i=0;i<BufferLen;i++)
	{
		Buffer[i]=Buffer[i] ^ (char)genrand();
	}
	sgenrand(odbcseed2);
	for (i=0;i<BufferLen;i++)
	{
		Buffer[i]=Buffer[i] ^ (char)genrand();
	}
	sgenrand(odbcseed1);
	for (i=0;i<BufferLen;i++)
	{
		Buffer[i]=Buffer[i] ^ (char)genrand();
	}
}

void EncryptBuffer(char *Buffer,long BufferLen)
{
	int i;

	sgenrand(odbcseed1);
	for (i=0;i<BufferLen;i++)
	{
		Buffer[i]=Buffer[i] ^ (char)genrand();
	}
	sgenrand(odbcseed2);
	for (i=0;i<BufferLen;i++)
	{
		Buffer[i]=Buffer[i] ^ (char)genrand();
	}
	sgenrand(odbcseed3);
	for (i=0;i<BufferLen;i++)
	{
		Buffer[i]=Buffer[i] ^ (char)genrand();
	}
}


static char to_hex(char code)
{
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

static char *url_encode(char *str)
{
	char *pstr = str;
	char *buf = (char *)malloc(strlen(str) * 3 + 1);
	char *pbuf = buf;
	while (*pstr) {
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
			*pbuf++ = *pstr;
		else if (*pstr == '/')
			*pbuf++ = '/';
		else {
			*pbuf++ = '%';
			*pbuf++ = to_hex(*pstr >> 4);
			*pbuf++ = to_hex(*pstr & 15);
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

#ifndef SETODBC
bool internalParseReply(char *xmlstr,TiXmlElement **result,int *errnum,char **errstr)
{
	static TiXmlDocument doc;
	doc.Clear();
	doc.Parse(xmlstr);
	try {
		if (doc.Error()) throw "Failed to parse XML from WebService";
		*result = doc.FirstChildElement("result");
		if (!(*result)) throw "result tag not found in XML reply from WebService";
		const char *success = (*result)->Attribute("success");
		if (!success) throw "success attribute not found in result tag";
		if (success[0]!='Y') {
			TiXmlElement *err = (*result)->FirstChildElement("error");
			if (err) {
				const char *szErrNum = err->Attribute("errnum");
				*errnum = szErrNum?atoi(szErrNum):0;
				throw err->GetText();
			} else {
				*errnum = 0;
				throw "Unknown (no error tag)";
			}
		}
		return true;
	} catch(const char *ErrStr) {
		*errstr = strdup(ErrStr);
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////
// triODBC
///////////////////////////////////////////////////////////////////////////////

// RHT 19/11/2012 - moved from header
triODBC::triODBC()
	: m_EnvHandle((SQLHENV) 0), m_EnvHandleSet(false),
	  m_ConnHandle((SQLHDBC) 0), m_ConnHandleSet(false),
	  m_triSQL(), m_DatabaseType(DATABASE_TYPE_UNKNOWN),
	  m_useWebService(false), m_clientid((char *)0),
	  m_LastErrorNum(0), m_LastErrorText((char *)0),
	  m_dsn((char *)0), m_UserName((char *)0),
	  m_Password((char *)0)
{}

triODBC::triODBC(Context *ctx,char *clientid)
	: m_EnvHandle((SQLHENV) 0), m_EnvHandleSet(false),
	  m_ConnHandle((SQLHDBC) 0), m_ConnHandleSet(false),
	  m_triSQL(), m_DatabaseType(DATABASE_TYPE_POSTGRES),
	  m_useWebService(true), m_ctx(ctx),
	  m_LastErrorNum(0), m_LastErrorText((char *)0),
	  m_dsn((char *)0), m_UserName((char *)0),
	  m_Password((char *)0)
{
	m_clientid = strdup(clientid);
#ifndef SETODBC
	m_CookieJar = new DMArray(false);
#endif
}


// RHT 19/11/2012 - moved from header
triODBC::~triODBC()
{
	// Destructor
	// m_triSQL.CloseAnyOpenHandles();
	if (m_EnvHandleSet)  FreeEnvironmentHandle();
	if (m_ConnHandleSet) FreeConnectionHandle();
	if (m_dsn) free(m_dsn);
	if (m_UserName) free(m_UserName);
	if (m_Password) free(m_Password);
	if (m_clientid) free(m_clientid);
	if (m_LastErrorText) free(m_LastErrorText);
}

#ifndef SETODBC
bool triODBC::parseReply(char *xmlstr,TiXmlElement **result)
{
	return internalParseReply(xmlstr,result,&m_LastErrorNum,&m_LastErrorText);
}
#endif

DATABASE_TYPE triODBC::GetDatabaseType()
{
	if (m_useWebService) return DATABASE_TYPE_POSTGRES;
	return m_DatabaseType;
}

long triODBC::AllocateAndSetEnvironmentHandle()
{
	long		res;
	if (!m_useWebService) {
		res=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&m_EnvHandle);
		if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
		{
			res=SQL_ERROR;
		}
		else
		{
			res=SQLSetEnvAttr(m_EnvHandle,SQL_ATTR_ODBC_VERSION,
						(void *)SQL_OV_ODBC3,0);
			if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
			{
				res=SQL_ERROR;
				SQLFreeHandle(SQL_HANDLE_ENV, m_EnvHandle);
			}
		}
	} else {
		// For Web Service just return success
		res=SQL_SUCCESS;
	}
	return res;
}

long triODBC::AllocateConnectionHandle()
{
	long		res;
	if (!m_useWebService) {
		res=SQLAllocHandle(SQL_HANDLE_DBC,m_EnvHandle,&m_ConnHandle);
		if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
		{
			res=SQL_ERROR;
		}
		SQLSetConnectAttr(m_ConnHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER *)5, 0);
	}
	else {
		// For Web Service just return success
		res=SQL_SUCCESS;
	}
	return res;
}

void triODBC::GetLastError(char **MsgPtr,SQLINTEGER *ErrNum)
{
	if (!m_useWebService) {
		SQLCHAR		SQLStatusString[10];
		SQLCHAR		ErrorText[1024];
		SQLSMALLINT	ErrorTextActualLength = 0;

		SQLRETURN ret = SQLGetDiagRecA(SQL_HANDLE_DBC,
				m_ConnHandle,
				1,
				SQLStatusString,
				ErrNum,
							ErrorText,
				sizeof(ErrorText),
				&ErrorTextActualLength);
		*MsgPtr = (ErrorTextActualLength > 0) ? strdup((const char *)ErrorText) : NULL;
	} else {
		*MsgPtr = strdup(m_LastErrorText);
		*ErrNum = m_LastErrorNum;
	}
}


long triODBC::FreeConnectionHandle()
{
	long res;
	if (!m_useWebService) {
		if (m_ConnHandleSet)
		{
			m_ConnHandleSet=false;
			res = SQLFreeHandle(SQL_HANDLE_DBC,m_ConnHandle);
		}
		else
		{
			res = SQL_SUCCESS;
		}
	} else {
		res = SQL_SUCCESS;
	}
	return res;
}

long triODBC::FreeEnvironmentHandle()
{
	long res;

	if (!m_useWebService) {
		if (m_EnvHandleSet)
		{
			m_EnvHandleSet=false;
			res = SQLFreeHandle(SQL_HANDLE_ENV, m_EnvHandle);
		}
		else
		{
			res = SQL_SUCCESS;
		}
	} else {
		res = SQL_SUCCESS;
	}
	return res;
}

long triODBC::ConnectToDataSource(DM *dm)
{
#ifndef SETODBC
	long res = SQL_ERROR;
	if (m_useWebService) {
		m_dm = dm;
		m_nowcolname = "now()";
		m_DatabaseType = DATABASE_TYPE_POSTGRES;
		Expr *ret;
		char *params = (char *)malloc(strlen(m_clientid)+128);
		sprintf(params,"clientid=%s",m_clientid);
		dm->API(*m_ctx,"/dmadminweb/API/sql",params,&ret,m_CookieJar);
		TiXmlElement *result;
		if (parseReply(ret->toString(),&result)) {
			res = SQL_SUCCESS;
		}
	}
	return res;
#else
	return SQL_ERROR;
#endif
}

long triODBC::ConnectToDataSource(DM *dm, char *DSN,char *Username,char *Password)
{
	long		res;

	// printf("DSN=[%s] Username=[%s] Password=[%s]\n",DSN,Username,Password);

	m_UserName = (char *)0;
	m_Password = (char *)0;
	m_dsn = (char *)0;

	res=AllocateAndSetEnvironmentHandle();
	if (res!=SQL_ERROR)
	{
		// Set ODBC Version
		res=SQLSetEnvAttr(	m_EnvHandle,
					SQL_ATTR_ODBC_VERSION,
					(SQLPOINTER)SQL_OV_ODBC3,
					0);
		if (res!=SQL_ERROR)
		{
			m_EnvHandleSet=true;
			res=AllocateConnectionHandle();
			if (res==SQL_ERROR)
			{
				FreeEnvironmentHandle();
			}
			else
			{
				m_ConnHandleSet=true;
			}
		}
	}

	if (res!=SQL_ERROR)
	{
		res = SQLConnectA(	m_ConnHandle,
					(SQLCHAR*)DSN,      SQL_NTS,
					(SQLCHAR*)Username, SQL_NTS,
					(SQLCHAR*)Password, SQL_NTS);

		if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
		{
//////
			SQLCHAR		SQLStatusString[10];
			SQLCHAR		ErrorText[1024];
			SQLSMALLINT	ErrorTextActualLength = 0;
			SQLINTEGER  ErrNum = 0;
			SQLRETURN ret = SQLGetDiagRecA(SQL_HANDLE_DBC,
					m_ConnHandle,
					1,
					SQLStatusString,
					&ErrNum,
					ErrorText,
					sizeof(ErrorText),
					&ErrorTextActualLength);
			//fprintf(stderr, "ret = %d; ErrorTextActualLength = %d\n", ret, ErrorTextActualLength);
			fprintf(stderr, "SQLConnect Error: %s (%d)\n", ErrorText, ErrNum);
//////
			res=SQL_ERROR;
			FreeConnectionHandle();
			FreeEnvironmentHandle();
		}
		else
		{
#ifdef DDODBC
			//
			// Pass the appropriate connection attributes so that the
			// branded ODBC drivers think we're actually Harvest...
			//

			SQLSetConnectAttr(m_ConnHandle,0x411,(void*)"IVPT.LIC",8);
			SQLSetConnectAttr(m_ConnHandle,0x412,(void*)"PLATINUM technology inc.",0x18);
#endif /*DDODBC*/
			m_UserName = strdup(Username);
			m_Password = strdup(Password);
			m_dsn = strdup(DSN);

			m_dm = dm;

			short dbnamelen=sizeof(m_dbname);
			short dmverlen=sizeof(m_dmver);
			short drivernamelen=sizeof(m_drivername);
			short driververlen=sizeof(m_driverver);
			SQLGetInfo(m_ConnHandle,SQL_DBMS_NAME,m_dbname,sizeof(m_dbname),&dbnamelen);
			SQLGetInfo(m_ConnHandle,SQL_DM_VER,m_dmver,sizeof(m_dmver),&dmverlen);
			SQLGetInfo(m_ConnHandle,SQL_DRIVER_NAME,m_drivername,sizeof(m_drivername),&drivernamelen);
			SQLGetInfo(m_ConnHandle,SQL_DRIVER_VER,m_driverver,sizeof(m_driverver),&driververlen);
			m_dbname[dbnamelen]='\0';
			m_dmver[dmverlen]='\0';
			m_drivername[drivernamelen]='\0';
			m_driverver[driververlen]='\0';
			if (STRCASECMP(m_dbname,"oracle") == 0) {
				// Oracle
				m_nowcolname = "sysdate";
				m_DatabaseType = DATABASE_TYPE_ORACLE;
			} else {
				// Postgres
				m_nowcolname = "now()";
				m_DatabaseType = DATABASE_TYPE_POSTGRES;
			}
		}
	}
	return res;
}

/*
long triODBC::ConnectToDataSource(DM *dm,char *FileName)
{
	long		res;
	int		in;
	struct stat	buf;
	char		*DSN;
	char		*Username;
	char		*Password;
	char		*Buffer;

	if (stat(FileName,&buf) == -1)
	{
		if (errno == ENOENT)
		{
			fprintf(stderr,"ODBC control file %s does not exist\n",FileName);
			fprintf(stderr,"Please run setodbc to set the ODBC parameters\n");
		}
		else
		{
			fprintf(stderr,"Could not stat %s: ",FileName);
			perror("");
		}
		exit(1);
	}

	in=open(FileName,O_RDONLY);
	if (in == -1)
	{
		fprintf(stderr,"Could not open %s: ",FileName);
		perror("");
		exit(1);
	}

	Buffer=(char *)malloc(buf.st_size);
	if (Buffer) {
		if (read(in,Buffer,buf.st_size) == -1)
		{
			fprintf(stderr,"Could not read %s: ",FileName);
			perror("");
			exit(1);
		}
		
		DecryptBuffer(Buffer,buf.st_size);

		DSN=strdup(Buffer);
		Username=strdup(&Buffer[strlen(DSN)+1]);
		Password=strdup(&Buffer[strlen(DSN)+strlen(Username)+2]);

		res = ConnectToDataSource(dm,DSN,Username,Password);
		free(Username);
		free(Password);
		free(DSN);
		free(Buffer);
	} else {
		res = -1;
	}
	return res;
}
*/

long triODBC::DisconnectFromDataSource()
{
	long res;
	if (!m_useWebService) {
		if (m_ConnHandleSet)
		{
    		// m_triSQL.CloseAnyOpenHandles(); // RHT - 28/11/2006 - this fixes a couple of IVRs in the destructor
			res=SQLDisconnect(m_ConnHandle);
			free(m_UserName);
			free(m_Password);
			free(m_dsn);
		}
		else
		{
			res=SQL_SUCCESS;
		}
		FreeConnectionHandle();
		FreeEnvironmentHandle();
	} else {
		res = SQL_SUCCESS;
	}
	return res;
}

/*
void triODBC::Reconnect()
{
	DisconnectFromDataSource();
	m_dm->writeToStdOut("Reconnecting dsn=[%s] username=[%s]",m_dsn,m_UserName);
	ConnectToDataSource(m_dm,m_dsn,m_UserName,m_Password);
}
*/

triSQL *triODBC::GetSQL()
{
	triSQL *p_triSQL = new triSQL;
	p_triSQL->setODBC(this);
	p_triSQL->AllocateStatementHandle(m_ConnHandle);
	p_triSQL->setCookieJar(m_CookieJar);
	return p_triSQL;
}
/*
bool triODBC::getUseWebService()
{
	return m_useWebService;
}
*/




///////////////////////////////////////////////////////////////////////////////
// triSQL
///////////////////////////////////////////////////////////////////////////////

#ifndef SETODBC
bool triSQL::parseReply(char *xmlstr,TiXmlElement **result)
{
	return internalParseReply(xmlstr,result,&m_LastErrorNum,&m_LastErrorText);
}
#endif

void triSQL::CloseAnyOpenHandles()
{
	if (m_StatHandleSet)
	{
#ifndef WIN32
		// This crashes windows servers for some reason.
		SQLCloseCursor(m_StatHandle);
#endif
		FreeStatementHandle();
	}
}

void triSQL::setODBC(triODBC *odbc)
{
	m_odbc = odbc;
}

void triSQL::setCookieJar(DMArray *cookieJar)
{
	m_CookieJar = cookieJar;
}

long triSQL::AllocateStatementHandle(SQLHDBC ConnHandle)
{
	//
	// Called from Constructor
	//
	long		res;
	m_ConnHandle = ConnHandle;	// Passed from parent triODBC class
	if (!m_odbc->getUseWebService()) {
		res=SQLAllocHandle(SQL_HANDLE_STMT,m_ConnHandle,&m_StatHandle);
		if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
		{
			res=SQL_ERROR;
			m_StatHandleSet=false;
		}
		else
		{
			m_StatHandleSet=true;
		}
	} else {
		// Web Service
		m_ConnHandle = (SQLHDBC)this;
		m_StatHandleSet=true;
		m_StatHandle=(SQLHSTMT)this;
		res = SQL_SUCCESS;
	}
	return res;
}

long triSQL::FreeStatementHandle()
{
	if (!m_odbc->getUseWebService()) {
#ifndef WIN32
		// This crashes windows servers for some reason - RHT - 28/11/2006 - modified to check set var
		if( m_StatHandleSet )
		{
			m_StatHandleSet=false;
			return SQLFreeHandle(SQL_HANDLE_STMT,m_StatHandle);
		}
#endif
		m_StatHandleSet=false;
		return 0;
	} else {
		// Web Service
#ifndef SETODBC
		if (m_StatHandleSet) {
			char params[256];
			Expr *ret;
			sprintf(params,"close=Y&stmtid=%lx",(long)m_StatHandle);
			m_odbc->getDM()->API(*(m_odbc->getCTX()),"/dmadminweb/API/sql",params,&ret,m_CookieJar);
		}
#endif
		m_StatHandleSet=false;
		return 0;
	}
}

//
// Constructor/Destructor/Copy
//
triSQL::triSQL()
{
	m_StatHandleSet=false;
	CharCols=(CHARCOLS *)0;
	m_RowArraySize=1;
    m_NumRowsFetched=0;
	m_IndicatorArray=(SQLLEN *)0;
	m_SingleIndicator=0;
	m_maxBindParams=0;
	m_BindParams = (BIND_PARAMS *)0;
	m_maxBindColumns=0;
	m_BindColumns = (BIND_COLUMNS *)0;
	m_returnedData = (char ***)0;
	m_retColCount = 0;
	m_retRowCount = 0;
	m_currentRow = 0;
	m_ParamCount = 0;
	m_RowsUpdated = 0;
	m_LastErrorText = (char *)0;
	m_LastErrorNum = 0;
#ifndef SETODBC
	m_CookieJar = new DMArray(false);
#endif
}


triSQL::triSQL(triSQL &copy)
{
	m_StatHandleSet=false;
	CharCols=(CHARCOLS *)0;
	m_RowArraySize=1;
    m_NumRowsFetched=0;
	m_IndicatorArray=(SQLLEN *)0;
	m_SingleIndicator=0;
	m_maxBindParams=0;
	m_BindParams = (BIND_PARAMS *)0;
	m_maxBindColumns=0;
	m_BindColumns = (BIND_COLUMNS *)0;
	m_returnedData = (char ***)0;
	m_retColCount = 0;
	m_retRowCount = 0;
	m_currentRow = 0;
	m_ParamCount = 0;
	m_RowsUpdated = 0;
	m_LastErrorText = (char *)0;
	m_LastErrorNum = 0;
#ifndef SETODBC
	DMArray cookieJar = new DMArray(false);
#endif
}


triSQL::~triSQL()
{
	CHARCOLS *c,*x;
	// CloseAnyOpenHandles();
	CloseSQL();
	c=CharCols;
	while (c)
	{
		x=c->next;
		free(c);
		c=x;
	}
	CharCols=(CHARCOLS *)0;
	if (m_RowArraySize > 1 && m_IndicatorArray) free(m_IndicatorArray);
	if (m_maxBindParams>0 && m_BindParams) free(m_BindParams);
	if (m_maxBindColumns>0 && m_BindColumns) free(m_BindColumns);
	if (m_returnedData) {
		for (int r=0;r<m_retRowCount;r++) {
			char **cd = m_returnedData[r];
			for (int c=0;c<m_retColCount;c++) {
				free(cd[c]);
			}
			free(cd);
		}
		free(m_returnedData);
	}
	if (m_LastErrorText) free(m_LastErrorText);
}


SQLRETURN triSQL::BindParameter(	SQLUSMALLINT	ColumnNumber,
							SQLSMALLINT		TargetType,
							SQLUINTEGER		Precision,
							SQLPOINTER		TargetValuePtr,
							SQLINTEGER		BufferLength)
{
	SQLSMALLINT Scale = 0;
	SQLSMALLINT SourceType = SQL_C_DEFAULT;
	static SQLLEN pcbValue[256];

	// printf("BindParameter = Binding Parameter %d (%s) Precision=%d\n",ColumnNumber,(TargetValuePtr == (SQLPOINTER)0)?"NULL":"Real Data",Precision);

	if (TargetValuePtr == (SQLPOINTER)0)
	{
		// No data specified - must be NULL
		pcbValue[ColumnNumber] = (SQLINTEGER)SQL_NULL_DATA;
	}
	else
	{
		// Real data
		pcbValue[ColumnNumber] = BufferLength;
	}

	if (TargetType == SQL_BINARY || TargetType == SQL_C_BINARY)
	{
		// printf("Setting pcbValue to SQL_DATA_AT_EXEC\n");
		
		SourceType = SQL_C_DEFAULT;
		TargetType = SQL_LONGVARBINARY;
		if (TargetValuePtr == (SQLPOINTER)0)
		{
			//
			// This is a null BLOB, set the Precision to 1 (0 is not allowed)
			//
			Precision=1;
		}
		else
		{
			//
			// This is a BLOB column and it's not null - set up to take the data later
			//
			pcbValue[ColumnNumber] = (SQLINTEGER)SQL_DATA_AT_EXEC;
		}
	}

	 // printf("Calling SQLBindParameter(m_StatHandle,%d,SQL_PARAM_INPUT,%d,%d,%d,%d,0x%lx,%d,0x%lx)\n",
		// ColumnNumber,SourceType,TargetType,Precision,Scale,TargetValuePtr,BufferLength,&(pcbValue[ColumnNumber]));
	
	SQLRETURN res;

	if (!m_odbc->getUseWebService()) {
#ifdef __SUNOS__
	if ((TargetType == SQL_INTEGER || TargetType == SQL_C_SLONG) && BufferLength == 8 && sizeof(long) == 8)
	{
		// Mapping to a long - breaks on Solaris 64 bit - remap
		TargetValuePtr = (SQLPOINTER)((unsigned char *)TargetValuePtr + 4);
		BufferLength = 4;
	}
#endif
		res = SQLBindParameter(	m_StatHandle,
										ColumnNumber,
										SQL_PARAM_INPUT,
										SourceType,
										TargetType,
										Precision,
										Scale,
										TargetValuePtr,
										BufferLength,
										&(pcbValue[ColumnNumber]));
	} else {
		// For Web Service - add to the bind list. We will get the values
		// and pass them to the Web Service when we execute the query.
		if (ColumnNumber > m_maxBindParams) {
			if (!m_maxBindParams) {
				m_BindParams = (BIND_PARAMS *)malloc(sizeof(BIND_PARAMS));
			} else {
				m_BindParams = (BIND_PARAMS *)realloc(m_BindParams,sizeof(BIND_PARAMS)*ColumnNumber);
			}
			m_maxBindParams = ColumnNumber;
		}
		m_BindParams[ColumnNumber-1].TargetType = TargetType;
		m_BindParams[ColumnNumber-1].Precision = Precision;
		m_BindParams[ColumnNumber-1].TargetValuePtr = TargetValuePtr;
		m_BindParams[ColumnNumber-1].BufferLength = BufferLength;
		res = SQL_SUCCESS;
	}
	return res;
}


long triSQL::BindColumn(	SQLUSMALLINT 	ColumnNumber,
				SQLSMALLINT	TargetType,
				SQLPOINTER	TargetValuePtr,
				SQLINTEGER	BufferLength,
				SQLLEN	*ni)
{
	long			res;
	static SQLLEN	StrLen_or_IndPtr;
	if (ColumnNumber==1)
	{
		// First of a new batch of binds - free up any existing
		// char columns in the list
		//
		CHARCOLS *c,*x;
		c=CharCols;
		while (c)
		{
			x=c->next;
			free(c);
			c=x;
		}
		CharCols=(CHARCOLS *)0;
	}
	if (TargetType==SQL_CHAR || TargetType==SQL_C_CHAR)
	{
		// Character data - we will do an automatic right trim on
		// all character data.
		
		if (CharCols==(CHARCOLS *)0)
		{
			CharCols=(CHARCOLS *)malloc(sizeof(CHARCOLS));
			xCol=CharCols;
		}
		else
		{
			xCol->next=(CHARCOLS *)malloc(sizeof(CHARCOLS));
			xCol=xCol->next;
		}
		if (xCol) {
			xCol->next=(CHARCOLS *)0;
			xCol->DataPtr=(char *)TargetValuePtr;
			xCol->DataLength=BufferLength;
			// ### Hack for Valgrind
			memset(xCol->DataPtr, 0, xCol->DataLength);
		}
	}
	if (!m_odbc->getUseWebService()) {
// For Solaris 64 bit
#ifdef __SUNOS__
	if ((TargetType == SQL_INTEGER || TargetType == SQL_C_SLONG) && BufferLength == 8 && sizeof(long) == 8)
	{
		// Mapping to a long - breaks on Solaris 64 bit - remap
		bzero(TargetValuePtr,BufferLength);
		TargetValuePtr = (SQLPOINTER)((unsigned char *)TargetValuePtr + 4);
		BufferLength = 4;
	}
#endif
		if (ni)
		{
			//
			// User has supplied a null indicator location
			//
			res=SQLBindCol(	m_StatHandle,
					ColumnNumber,
					TargetType,
					TargetValuePtr,
					BufferLength,
					ni);
		}
		else
		{
			//
			// User has not supplied a null indicator - default to local copy (ignore)
			//
			res=SQLBindCol(	m_StatHandle,
					ColumnNumber,
					TargetType,
					TargetValuePtr,
					BufferLength,
					m_IndicatorArray);
		}
		if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
		{
			res=SQL_ERROR;
		}
	} else {
		if (ColumnNumber > m_maxBindColumns) {
			if (!m_maxBindColumns) {
				m_BindColumns = (BIND_COLUMNS *)malloc(sizeof(BIND_COLUMNS));
			} else {
				m_BindColumns = (BIND_COLUMNS *)realloc(m_BindColumns,sizeof(BIND_COLUMNS)*ColumnNumber);
			}
			m_maxBindColumns = ColumnNumber;
		}
		m_BindColumns[ColumnNumber-1].TargetType = TargetType;
		m_BindColumns[ColumnNumber-1].TargetValuePtr = TargetValuePtr;
		m_BindColumns[ColumnNumber-1].BufferLength = BufferLength;
		if (ni) {
			m_BindColumns[ColumnNumber-1].NullIndicator = ni;
		} else {
			m_BindColumns[ColumnNumber-1].NullIndicator = m_IndicatorArray;
		}
		res = SQL_SUCCESS;
	}
	return res;
}


SQLRETURN triSQL::Execute()
{
	SQLRETURN res;
	if (!m_odbc->getUseWebService()) {
		res = SQLExecute(m_StatHandle);
	} else {
#ifndef SETODBC
		// For web services we need to bind all the parameters before executing
		// the query.
		Expr *ret;
		m_RowsUpdated = 0;
		for (int i=1;i<=m_ParamCount;i++) {
			char *params = (char *)0;
			switch(m_BindParams[i-1].TargetType) {
				case SQL_INTEGER:
					params = (char *)malloc(256);
					sprintf(params,"bind=Y&stmtid=%lx&colno=%d&coltype=2&val=%d",
						(long)m_StatHandle,i,*((long *)m_BindParams[i-1].TargetValuePtr));
					break;
				case SQL_CHAR:	// SQL_C_CHAR is the same
				case SQL_LONGVARCHAR:
					params = (char *)malloc(strlen((char *)m_BindParams[i-1].TargetValuePtr)+256);
					char *ev = url_encode((char *)m_BindParams[i-1].TargetValuePtr);
					sprintf(params,"bind=Y&stmtid=%lx&colno=%d&coltype=1&val=%s",
						(long)m_StatHandle,i,ev);
					free(ev);
					break;
			}
			m_odbc->getDM()->API(*(m_odbc->getCTX()),"/dmadminweb/API/sql",params,&ret,m_CookieJar);
			if (params) free(params);
		}
		// Now execute the query itself
		char p[128];
		sprintf(p,"execute=Y&stmtid=%lx",m_StatHandle);
		m_odbc->getDM()->API(*(m_odbc->getCTX()),"/dmadminweb/API/sql",p,&ret,m_CookieJar);
		TiXmlElement *result;
		if (parseReply(ret->toString(),&result)) {
			try {
				const char *szRowCount = result->Attribute("rowcount");
				if (szRowCount) m_RowsUpdated = atoi(szRowCount);
				TiXmlElement *cols = result->FirstChildElement("columns");
				if (cols) {
					// Column data is only returned for query statements
					const char *szCount = cols->Attribute("count");
					if (!szCount) throw "count attribute not found";
					m_retColCount = atoi(szCount);
					TiXmlElement *data = result->FirstChildElement("data");
					if (!data) throw "data tag not found";
					m_retRowCount=0;
					for(TiXmlElement *row = data->FirstChildElement("row"); row; row = row->NextSiblingElement("row")) {
						m_retRowCount++;
						if (!m_returnedData) {
							// First row
							m_returnedData = (char ***)malloc((sizeof(char **)));
						} else {
							// Subsequent row
							m_returnedData = (char ***)realloc(m_returnedData,sizeof(char **)*m_retRowCount);
						}
						char **cd = (char **)malloc(sizeof(char *)*m_retColCount);
						m_returnedData[m_retRowCount-1]=cd;
						int colnum=0;
						for (TiXmlElement *col = row->FirstChildElement(); col; col = col->NextSiblingElement()) {
							const char *szNull = col->Attribute("null");
							if (szNull && szNull[0]=='Y') {
								cd[colnum] = (char *)0;		// null indicator
							} else {
								const char *coltext = col->GetText();
								cd[colnum]=strdup(coltext?coltext:"");
							}
							colnum++;
						}
					}
				}
				res = SQL_SUCCESS;
				/*
				printf("Data is:\n");
				for (int r=0;r<m_retRowCount;r++) {
					printf("row %d\n",r);
					char **p = m_returnedData[r];
					for (int c=0;c<m_retColCount;c++) {
						printf("col %d data [%s]\n",c,p[c]);
					}
				}
				*/
			} catch(const char *errtext) {
				res = SQL_ERROR;
				m_LastErrorText = strdup(errtext);
			}
		} else {
			res = SQL_ERROR;
		}
#endif
	}
	return res;
}

SQLRETURN triSQL::ExecuteIgnoringErrors()
{
	SQLRETURN res;
	if (!m_odbc->getUseWebService()) {
		res = SQLExecute(m_StatHandle);
	} else {
		res = this->Execute();
	}
	if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO) && (res != SQL_NEED_DATA) && (res != SQL_NO_DATA))
	{
		// printf("SQLExecute failed with res=%d\n",res);
		res=SQL_ERROR;
	}

	return res;
}

SQLRETURN triSQL::ParamData()
{
	SQLPOINTER Value;
	SQLRETURN res = SQLParamData(m_StatHandle,&Value);
	if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO) && (res != SQL_NEED_DATA))
	{
		// printf("SQLExecute failed with res=%d\n",res);
		res=SQL_ERROR;
	}
	return res;
}

SQLRETURN triSQL::PutData(SQLPOINTER TargetValue,SQLLEN DataLength)
{
	SQLRETURN res = SQLPutData(m_StatHandle,TargetValue,DataLength);
	if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO) && (res != SQL_NEED_DATA))
	{
		// printf("SQLExecute failed with res=%d\n",res);
		res=SQL_ERROR;
	}
	return res;
}


// RHT 04/12/2013 - added
SQLRETURN triSQL::GetData(SQLUSMALLINT ColumnNumber,SQLSMALLINT	TargetType,SQLPOINTER TargetValuePtr,SQLLEN DataLength, SQLLEN *ni)
{
	SQLRETURN res = SQLGetData(m_StatHandle, ColumnNumber, TargetType, TargetValuePtr, DataLength, ni);
	if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO) /*&& (res != SQL_NEED_DATA)*/)
	{
		// printf("SQLExecute failed with res=%d\n",res);
		res=SQL_ERROR;
	}
	return res;
}


SQLRETURN triSQL::ExecuteSQL(const char *SQLformat, ...)
{
	SQLRETURN	res;
	va_list		ap;
	char		*SQLStatement;

#ifdef WIN32
	va_start(ap,SQLformat);
	long CurrentSize=strlen(SQLformat)+500;	// First Guess
	SQLStatement = (char *)malloc(CurrentSize);
	int cc = _vsnprintf(SQLStatement,CurrentSize,SQLformat,ap);
	while (cc == -1)
	{
		// Not enough space
		CurrentSize=CurrentSize+500;	// another 500 bytes
		SQLStatement=(char *)realloc(SQLStatement,CurrentSize);
		va_start(ap,SQLformat);
		cc = _vsnprintf(SQLStatement,CurrentSize,SQLformat,ap);
	}
	va_end(ap);
#else
	char		dummy[1];
	va_start(ap,SQLformat);
	int cc = vsnprintf(dummy,sizeof(dummy),SQLformat,ap);
	va_end(ap);
	SQLStatement=(char *)malloc(cc+10);
	va_start(ap,SQLformat);
	vsprintf(SQLStatement,SQLformat,ap);
	va_end(ap);
#endif


#ifndef SETODBC
	debug3("SQLStatement=[%s]\n",SQLStatement);
#endif

	if (!m_odbc->getUseWebService()) {
 		res=SQLExecDirectA(m_StatHandle,(unsigned char *)SQLStatement,SQL_NTS);
	} else {
		// For web service, prepare the statement first
		res = this->PrepareStatement(SQLStatement);
		if (res == SQL_SUCCESS) {
			res = this->Execute();
		}
	}
	if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
	{
		res=SQL_ERROR;
	}
	free(SQLStatement);

	return res;
}

// RHT 14/05/2011 - Merged from PD
SQLRETURN /*long*/ triSQL::ExecuteSQLIgnoringErrors(const char *SQLformat, ...)
{
	SQLRETURN /*long*/		res;
	va_list		ap;
	char		*SQLStatement;

#ifdef WIN32
	va_start(ap,SQLformat);
	long CurrentSize=strlen(SQLformat)+500;	// First Guess
	SQLStatement = (char *)malloc(CurrentSize);
	int cc = _vsnprintf(SQLStatement,CurrentSize,SQLformat,ap);
	while (cc == -1)
	{
		// Not enough space
		CurrentSize=CurrentSize+500;	// another 500 bytes
		SQLStatement=(char *)realloc(SQLStatement,CurrentSize);
		va_start(ap,SQLformat);
		cc = _vsnprintf(SQLStatement,CurrentSize,SQLformat,ap);
	}
	va_end(ap);
#else
	char		dummy[1];
	va_start(ap,SQLformat);
	int cc = vsnprintf(dummy,sizeof(dummy),SQLformat,ap);
	va_end(ap);
	SQLStatement=(char *)malloc(cc+10);
	va_start(ap,SQLformat);
	vsprintf(SQLStatement,SQLformat,ap);
	va_end(ap);
#endif

// printf("SQLStatement=[%s]\n",SQLStatement);
 	res=SQLExecDirectA(m_StatHandle,(unsigned char *)SQLStatement,SQL_NTS);

	if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO) && (res != SQL_NO_DATA))
	{
		res=SQL_ERROR;
	}
	free(SQLStatement);

	return res;
}


// RHT 22/09/2010 - merged from PM
SQLRETURN triSQL::PrepareStatement(const char *SQLformat, ...)
{
	SQLRETURN		res;
	va_list		ap;
	char		*SQLStatement;

#ifdef WIN32
	va_start(ap,SQLformat);
	long CurrentSize=strlen(SQLformat)+500;	// First Guess
	SQLStatement = (char *)malloc(CurrentSize);
	int cc = _vsnprintf(SQLStatement,CurrentSize,SQLformat,ap);
	while (cc == -1)
	{
		// Not enough space
		CurrentSize=CurrentSize+500;	// another 500 bytes
		SQLStatement=(char *)realloc(SQLStatement,CurrentSize);
		va_start(ap,SQLformat);
		cc = _vsnprintf(SQLStatement,CurrentSize,SQLformat,ap);
	}
	va_end(ap);
#else
	char		dummy[1];
	va_start(ap,SQLformat);
	int cc = vsnprintf(dummy,sizeof(dummy),SQLformat,ap);
	va_end(ap);
	SQLStatement=(char *)malloc(cc+10);
	va_start(ap,SQLformat);
	vsprintf(SQLStatement,SQLformat,ap);
	va_end(ap);
#endif
#ifndef SETODBC
	debug3("SQLStatement=[%s]\n",SQLStatement);
#endif

	if (!m_odbc->getUseWebService()) {
 		res=SQLPrepareA(m_StatHandle,(unsigned char *)SQLStatement,SQL_NTS);
	} else {
#ifndef SETODBC
		// Call web service to prepare statement
		Expr *ret = (Expr *)0;
		char *ustmt = url_encode(SQLStatement);
		char *p = (char *)malloc(strlen(ustmt)+128);
		sprintf(p,"stmtid=%lx&prepare=Y&query=%s",m_StatHandle,ustmt);
		m_odbc->getDM()->API(*(m_odbc->getCTX()),"/dmadminweb/API/sql",p,&ret,m_CookieJar);
		TiXmlElement *result;
		if (parseReply(ret->toString(),&result)) {
			try {
				const char *szParams = result->Attribute("params");
				if (!szParams) throw "No params attribute";
				m_ParamCount = atoi(szParams);
				res = SQL_SUCCESS;
			} catch(const char *errstr) {
				res = SQL_ERROR;
				m_LastErrorText = strdup(errstr);
				m_LastErrorNum = 0;
			}
		} else {
			res = SQL_ERROR;
		}
		free(p);
		free(ustmt);
		
#endif
	}

	free(SQLStatement);

	return res;
}


long triSQL::GetRowsReturned()
{
	return m_NumRowsFetched;
}

long triSQL::GetRowCount()
{
	SQLLEN RowCount;

	if (!m_odbc->getUseWebService()) {
		SQLRETURN res = SQLRowCount(m_StatHandle,&RowCount);
		if (res!=SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO)
		{
			char		*MsgPtr = NULL;
			SQLINTEGER	ErrNum;

			GetLastError(&MsgPtr,&ErrNum);
			fprintf(stderr,"[%s]\n",(MsgPtr ? MsgPtr : ""));
			if(MsgPtr) free(MsgPtr);
		}
		else
		{
			// printf("res=%s\n",res==SQL_SUCCESS?"SQL_SUCCESS":"SQL_SUCCESS_WITH_INFO");
		}
	} else {
		// For web service - need to get the result from the reply.
		RowCount = m_RowsUpdated;
	}
	return RowCount;
}


extern void dumpbuffer(const char *buf, int len);

SQLRETURN triSQL::FetchRow()
{
	SQLRETURN res;

	if (!m_odbc->getUseWebService()) {
 		res=SQLFetch(m_StatHandle);
	} else {
#ifndef SETODBC
		// For the web service, the rows should already have been fetched
		if (m_currentRow >= m_retRowCount) return SQL_NO_DATA;	// all rows retrieved
		char **cd = m_returnedData[m_currentRow++];
		for (int c=0;c<m_retColCount;c++) {
			// copy the data to the bind variables
			if (cd[c]) {
				// Not NULL data
				switch(m_BindColumns[c].TargetType) {
					case SQL_INTEGER:
						*((int *)(m_BindColumns[c].TargetValuePtr)) = atoi(cd[c]);
						if (m_BindColumns[c].NullIndicator) {
							*(SQLLEN *)(m_BindColumns[c].NullIndicator) = sizeof(int);
						}
						break;
					case SQL_CHAR: {
						char *dst = (char *)m_BindColumns[c].TargetValuePtr;
						int len = m_BindColumns[c].BufferLength-1;
						strncpy(dst,cd[c],len);
						dst[len]='\0';
						if (m_BindColumns[c].NullIndicator) {
							*(SQLLEN *)(m_BindColumns[c].NullIndicator) = strlen(dst);
						}
						}
						break;
					case SQL_TIMESTAMP: {
						char *t = strdup(cd[c]);
						SQL_TIMESTAMP_STRUCT ts;
						ts.second=atoi(&(t[12]));
						t[12]='\0';
						ts.minute=atoi(&(t[10]));
						t[10]='\0';
						ts.hour=atoi(&(t[8]));
						t[8]='\0';
						ts.day=atoi(&(t[6]));
						t[6]='\0';
						ts.month=atoi(&(t[4]));
						t[4]='\0';
						ts.year=atoi(t);
						free(t);
						memcpy((void *)m_BindColumns[c].TargetValuePtr,(void *)&ts,sizeof(ts));
						}
						break;
				}
			} else {
				// NULL data - set the null indicator
				*(SQLLEN *)(m_BindColumns[c].NullIndicator) = SQL_NULL_DATA;
			}
		}
		res = SQL_SUCCESS;
#endif
	}
	//
	// Now loop through, doing an "rtrim" on all the returned CHAR columns
	//
	xCol = CharCols;

	while (xCol && res != SQL_NO_DATA && res != SQL_ERROR)
	{
		for (unsigned long /*int*/ p=0;p<m_RowArraySize;p++)
		{
			int a = 0, b = 0;
			char *dp=(xCol->DataPtr)+(p*xCol->DataLength);
			int n=strlen(dp)-1;							// RHT 25/09/2012 - TODO: This is nasty - caller must allocate 1 byte over and null the string
			while((n > 0) && (dp[n]==' ')) n--;			// RHT 25/09/2012 - change from (n) to (n > 0) to fix IVR(1)
			dp[n+1]='\0';
		}
		xCol=xCol->next;
	}
	if(res == -1) {
		// printf("query failed\n");
	}
	return res;
}

#ifdef THIS_IS_NOT_DEFINED
long triSQL::FetchMultipleRows(unsigned long *RowCount)
{
	long		res;
	SQLUSMALLINT	*RowStats = (SQLUSMALLINT *)calloc(*RowCount,sizeof(SQLUSMALLINT));


	int n=*RowCount;
	SQLSetStmtAttr(m_StatHandle,SQL_ATTR_ROW_BIND_TYPE,SQL_BIND_BY_COLUMN,0);
	SQLSetStmtAttr(m_StatHandle,SQL_ATTR_ROW_ARRAY_SIZE,(SQLPOINTER)n,0);
	SQLSetStmtAttr(m_StatHandle,SQL_ATTR_ROW_STATUS_PTR,RowStats,0);
	SQLSetStmtAttr(m_StatHandle,SQL_ATTR_ROWS_FETCHED_PTR,RowCount,0);

	res=SQLSetStmtOption(m_StatHandle,SQL_ROWSET_SIZE,*RowCount);
	res=SQLFetch(m_StatHandle);
	//
	// Now loop through, doing an "rtrim" on all the returned CHAR columns
	//
	xCol = CharCols;

	return res;
}
#endif


void triSQL::GetLastError(char **MsgPtr,SQLINTEGER *ErrNum)
{
	if (!m_odbc->getUseWebService()) {
		SQLCHAR		SQLStatusString[10];
		SQLCHAR		ErrorText[1024];
		SQLSMALLINT	ErrorTextActualLength = 0;

		SQLRETURN ret = SQLGetDiagRecA(SQL_HANDLE_STMT,
				m_StatHandle,
				1,
				SQLStatusString,
				ErrNum,
							ErrorText,
				sizeof(ErrorText),
				&ErrorTextActualLength);
	//fprintf(stderr, "ret = %d; ErrorTextActualLength = %d\n", ret, ErrorTextActualLength);
		*MsgPtr = (ErrorTextActualLength > 0) ? strdup((const char *)ErrorText) : NULL;
	} else {
		// Web Service calls store the error in the triSQL object
		*MsgPtr = strdup(m_LastErrorText);
		*ErrNum = m_LastErrorNum;
	}
}

long triSQL::CloseSQL()
{
	long res=SQL_SUCCESS;

	if (m_StatHandleSet)
	{
		res = SQLCloseCursor(m_StatHandle);
		FreeStatementHandle();
		//
		// Now set up a new SQL object ready for a new query
		//
		AllocateStatementHandle(m_ConnHandle);
	}
	return res;
}

#define STR_LEN 128+1
#define REM_LEN 254+1

TABLEDATA *triSQL::GetTableInfo(unsigned char *TableName)
{
	TABLEDATA	*StartPointer=(TABLEDATA *)0;
	TABLEDATA	*CurrPointer;

	/* Declare buffers for result set data */

	SQLCHAR       szCatalog[STR_LEN], szSchema[STR_LEN];
	SQLCHAR       szTableName[STR_LEN], szTableType[STR_LEN];
	SQLCHAR		  szRemarks[REM_LEN];
	SQLRETURN     retcode;

	/* Declare buffers for bytes available to return */

	SQLLEN cbCatalog, cbSchema, cbTableName, cbTableType, cbRemarks;	// RHT 07/08/2012 - was SQLINTEGER

	unsigned char *UpperCaseTableName=(unsigned char *)strdup((char *)TableName);
	int slen=(int)strlen((const char *)UpperCaseTableName);
	for (int i=0;i<slen;i++)
	{
		UpperCaseTableName[i]=UpperCaseTableName[i] & 0xdf ;
	}

	retcode = SQLTablesA(m_StatHandle,
		NULL, 0,		/* All catalogs */
		NULL, 0,		/* All schemas */
		UpperCaseTableName, SQL_NTS,
		(SQLCHAR *)"TABLE", SQL_NTS );
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		/* Bind columns in result set to buffers */

		SQLBindCol(m_StatHandle, 1, SQL_C_CHAR, szCatalog, STR_LEN,&cbCatalog);
		SQLBindCol(m_StatHandle, 2, SQL_C_CHAR, szSchema, STR_LEN, &cbSchema);
		SQLBindCol(m_StatHandle, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
		SQLBindCol(m_StatHandle, 4, SQL_C_CHAR, szTableType, STR_LEN, &cbTableType);
		SQLBindCol(m_StatHandle, 5, SQL_C_CHAR, szRemarks, REM_LEN, &cbRemarks);

		retcode = SQLFetch(m_StatHandle);

		while(retcode != SQL_NO_DATA)
		{
			/* Process fetched data */

			if (!StartPointer)
			{
				StartPointer=new TABLEDATA;
				CurrPointer=StartPointer;
			}
			else
			{
				CurrPointer->next=new TABLEDATA;
				CurrPointer=CurrPointer->next;
			}
			CurrPointer->next=(TABLEDATA *)0;
			CurrPointer->TableName=strdup((char *)szTableName);
			CurrPointer->TableType=strdup((char *)szTableType);
			retcode = SQLFetch(m_StatHandle);
		}
	}
	CloseSQL();
	free(UpperCaseTableName);

	return StartPointer;
}


COLDATA *triSQL::GetColumnInfo(unsigned char *TableName)
{
	COLDATA		*StartPointer=(COLDATA *)0;
	COLDATA		*CurrPointer;

	/* Declare buffers for result set data */

	SQLCHAR       szCatalog[STR_LEN], szSchema[STR_LEN];
	SQLCHAR       szTableName[STR_LEN], szColumnName[STR_LEN];
	SQLCHAR       szTypeName[STR_LEN], szRemarks[REM_LEN];
	SQLCHAR       szColumnDefault[STR_LEN], szIsNullable[STR_LEN];
	SQLINTEGER    ColumnSize, BufferLength, CharOctetLength, OrdinalPosition;
	SQLSMALLINT   DataType, DecimalDigits, NumPrecRadix, Nullable;
	SQLSMALLINT   SQLDataType, DatetimeSubtypeCode;
	SQLRETURN     retcode;

	/* Declare buffers for bytes available to return */

	// RHT 07/08/2012 - was SQLINTEGER for all
	SQLLEN cbCatalog, cbSchema, cbTableName, cbColumnName;
	SQLLEN cbDataType, cbTypeName, cbColumnSize, cbBufferLength;
	SQLLEN cbDecimalDigits, cbNumPrecRadix, cbNullable, cbRemarks;
	SQLLEN cbColumnDefault, cbSQLDataType, cbDatetimeSubtypeCode, cbCharOctetLength;
	SQLLEN cbOrdinalPosition, cbIsNullable;

	unsigned char *UpperCaseTableName=(unsigned char *)strdup((char *)TableName);
	int slen=(int)strlen((const char *)UpperCaseTableName);
	for (int i=0;i<slen;i++)
	{
		UpperCaseTableName[i]=UpperCaseTableName[i] & 0xdf ;
	}

	retcode = SQLColumnsA(m_StatHandle,
	NULL, 0,			/* All catalogs */
	NULL, 0,			/* All schemas */
	UpperCaseTableName, SQL_NTS,   
	NULL, 0);			/* All columns */

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		/* Bind columns in result set to buffers */
		SQLBindCol(m_StatHandle, 1, SQL_C_CHAR, szCatalog, STR_LEN,&cbCatalog);
		SQLBindCol(m_StatHandle, 2, SQL_C_CHAR, szSchema, STR_LEN, &cbSchema);
		SQLBindCol(m_StatHandle, 3, SQL_C_CHAR, szTableName, STR_LEN,&cbTableName);
		SQLBindCol(m_StatHandle, 4, SQL_C_CHAR, szColumnName, STR_LEN, &cbColumnName);
		SQLBindCol(m_StatHandle, 5, SQL_C_SSHORT, &DataType, 0, &cbDataType);
		SQLBindCol(m_StatHandle, 6, SQL_C_CHAR, szTypeName, STR_LEN, &cbTypeName);
		SQLBindCol(m_StatHandle, 7, SQL_C_SLONG, &ColumnSize, 0, &cbColumnSize);
		SQLBindCol(m_StatHandle, 8, SQL_C_SLONG, &BufferLength, 0, &cbBufferLength);
		SQLBindCol(m_StatHandle, 9, SQL_C_SSHORT, &DecimalDigits, 0, &cbDecimalDigits);
		SQLBindCol(m_StatHandle, 10, SQL_C_SSHORT, &NumPrecRadix, 0, &cbNumPrecRadix);
		SQLBindCol(m_StatHandle, 11, SQL_C_SSHORT, &Nullable, 0, &cbNullable);
		SQLBindCol(m_StatHandle, 12, SQL_C_CHAR, szRemarks, REM_LEN, &cbRemarks);
		SQLBindCol(m_StatHandle, 13, SQL_C_CHAR, szColumnDefault, STR_LEN, &cbColumnDefault);
		SQLBindCol(m_StatHandle, 14, SQL_C_SSHORT, &SQLDataType, 0, &cbSQLDataType);
		SQLBindCol(m_StatHandle, 15, SQL_C_SSHORT, &DatetimeSubtypeCode, 0, &cbDatetimeSubtypeCode);
		SQLBindCol(m_StatHandle, 16, SQL_C_SLONG, &CharOctetLength, 0, &cbCharOctetLength);
		SQLBindCol(m_StatHandle, 17, SQL_C_SLONG, &OrdinalPosition, 0, &cbOrdinalPosition);
		SQLBindCol(m_StatHandle, 18, SQL_C_CHAR, szIsNullable, STR_LEN, &cbIsNullable);

		retcode = SQLFetch(m_StatHandle);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			fprintf(stderr,"FAILED (%d) in Fetching Column data\n",retcode);
		}

		while(retcode != SQL_NO_DATA)
		{
			/* Process fetched data */
			if (!StartPointer)
			{
				StartPointer = new COLDATA(szColumnName);
				CurrPointer = StartPointer;
			}
			else
			{
				CurrPointer->next = new COLDATA(szColumnName);
				CurrPointer = CurrPointer->next;
			}

			switch(DataType)
			{
			case SQL_VARCHAR:
				CurrPointer->BindType = SQL_C_CHAR;
				break;
			case SQL_CHAR:
				CurrPointer->BindType = SQL_C_CHAR;
				break;
			case SQL_INTEGER:
				CurrPointer->BindType = SQL_C_SLONG;
				break;
			case SQL_DOUBLE:
				CurrPointer->BindType = SQL_C_SLONG;
				break;
			case SQL_TYPE_TIMESTAMP:
				CurrPointer->BindType = SQL_C_TIMESTAMP;
				break;
			case SQL_DECIMAL:
				CurrPointer->BindType = SQL_C_SLONG;
				break;
			default:
				fprintf(stderr,"WARNING - unknown type %d\n",DataType);
				break;
			}
			CurrPointer->ColumnSize = ColumnSize;
			retcode = SQLFetch(m_StatHandle);
			//
			// For some reason the CA ODBC driver seems to return a double
			// list. So break if the ordinal position returns to 1....
			//
			if (OrdinalPosition==1) retcode=SQL_NO_DATA;
		}
	}
	CloseSQL();
	free(UpperCaseTableName);

	return StartPointer;
}


#define BUF_LEN 1024

COLDATA *triSQL::GetColumnInfoForStatement()
{
	COLDATA		*StartPointer=(COLDATA *)0;
	COLDATA		*CurrPointer;

	int bufferSize = BUF_LEN;
	SQLSMALLINT numColumns = 0;
	SQLSMALLINT bufferLenUsed = 0;
	SQLLEN Nullable;	// RHT 07/08/2012 - was SQLINTEGER
	SQLLEN DataType;	// RHT 07/08/2012 - was SQLINTEGER
	SQLLEN colsize;		// RHT 07/08/2012 - was SQLINTEGER

	
	SQLRETURN retCode = SQLNumResultCols(m_StatHandle, &numColumns);
	SQLCHAR szColumnName[BUF_LEN];

	if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		//SQLPOINTER* columnLabels = (SQLPOINTER *)malloc(numColumns*sizeof(SQLPOINTER*));
		for (int i = 0 ; i < numColumns ; i++ )
		{
			retCode = SQLColAttribute(m_StatHandle, (SQLUSMALLINT)i + 1, SQL_DESC_LABEL, szColumnName /*CurrPointer->ColumnName*/, (SQLSMALLINT)bufferSize, &bufferLenUsed, NULL);

			if (!StartPointer)
			{
				StartPointer = new COLDATA(szColumnName);
				CurrPointer = StartPointer;
			}
			else
			{
				CurrPointer->next = new COLDATA(szColumnName);
				CurrPointer = CurrPointer->next;
			}

			retCode = SQLColAttribute(m_StatHandle, (SQLUSMALLINT)i + 1, SQL_DESC_TYPE, NULL, (SQLSMALLINT)0, &bufferLenUsed, &DataType);
			
			switch(DataType)
			{
			case SQL_VARCHAR:
			case -1:	// RHT 25/07/2012 - PostgresSQL character varying
				CurrPointer->BindType = SQL_C_CHAR;
				retCode = SQLColAttribute(m_StatHandle, (SQLUSMALLINT)i + 1, SQL_DESC_LENGTH, NULL, (SQLSMALLINT)0, &bufferLenUsed, &colsize);
				CurrPointer->ColumnSize = colsize+1;
				break;
			case SQL_CHAR:
				CurrPointer->BindType = SQL_C_CHAR;
				retCode = SQLColAttribute(m_StatHandle, (SQLUSMALLINT)i + 1, SQL_DESC_LENGTH, NULL, (SQLSMALLINT)0, &bufferLenUsed, &colsize);
				CurrPointer->ColumnSize = colsize+1;
				break;
			case SQL_INTEGER:
				CurrPointer->BindType = SQL_C_LONG;
				CurrPointer->ColumnSize = sizeof(SQLINTEGER);
				break;
			case SQL_DOUBLE:
				CurrPointer->BindType = SQL_C_LONG;
				CurrPointer->ColumnSize = sizeof(SQLINTEGER);
				break;
			case SQL_SMALLINT:
				CurrPointer->BindType = SQL_C_SHORT;
				CurrPointer->ColumnSize = sizeof(SQLSMALLINT);
				break;
			case SQL_REAL:
				CurrPointer->BindType = SQL_C_FLOAT;
				CurrPointer->ColumnSize = sizeof(SQLFLOAT);
				break;
			case SQL_TYPE_TIMESTAMP:
				CurrPointer->BindType = SQL_C_TIMESTAMP;
				CurrPointer->ColumnSize = sizeof(SQL_TIMESTAMP_STRUCT);
				break;
			case SQL_DECIMAL:
				CurrPointer->BindType = SQL_C_SLONG;
				CurrPointer->ColumnSize = sizeof(SQLINTEGER);
				break;
			case SQL_NUMERIC:
				CurrPointer->BindType = SQL_C_NUMERIC;
				CurrPointer->ColumnSize = sizeof(SQLINTEGER);
				break;
			case SQL_DATETIME:
				CurrPointer->BindType = SQL_C_TIMESTAMP;
				CurrPointer->ColumnSize = sizeof(SQL_TIMESTAMP_STRUCT);
				break;
			default:
				CurrPointer->BindType = (SQLINTEGER)DataType;
				break;
			}
			
			retCode = SQLColAttribute(m_StatHandle, (SQLUSMALLINT)i + 1, SQL_DESC_TYPE, NULL, (SQLSMALLINT)0, &bufferLenUsed, &Nullable);
			if (Nullable == SQL_NULLABLE)
			{
				CurrPointer->NullInd = 1;
			}
			else
			{
				CurrPointer->NullInd = 0;
			}

			// printf("Column %d: %s (%d) %s type=%d\n", i, CurrPointer->ColumnName,CurrPointer->ColumnSize,CurrPointer->NullInd?"NULL":"NOT NULL",CurrPointer->BindType );
		}
	}

	return StartPointer;
}


SQLRETURN triSQL::SetAutoCommitMode(bool autoCommit)
{
	SQLRETURN res;
	if (!m_odbc->getUseWebService()) {
		res = SQLSetConnectAttr(m_ConnHandle, SQL_ATTR_AUTOCOMMIT,
			(SQLPOINTER) (autoCommit ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF),
			SQL_IS_UINTEGER);
	} else {
		// Send Auto Commit mode to Web Service
#ifndef SETODBC
		char params[256];
		Expr *ret;
		sprintf(params,"start=%s&stmtid=%lx",(autoCommit?"Y":"N"),(long)m_StatHandle);
		m_odbc->getDM()->API(*(m_odbc->getCTX()),"/dmadminweb/API/sql",params,&ret,m_CookieJar);
		TiXmlElement *e;
		if (parseReply(ret->toString(),&e)) {
			res = SQL_SUCCESS;
		} else {
			res = SQL_ERROR;
		}
#else
		res = SQL_ERROR;
#endif
	}

	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
	{
		char		*MsgPtr = NULL;
		SQLINTEGER	ErrNum;

		GetLastError(&MsgPtr,&ErrNum);
		fprintf(stderr,"[%s]\n",(MsgPtr ? MsgPtr : ""));
		if(MsgPtr) free(MsgPtr);
	}

	return res;
}


SQLRETURN triSQL::EndTransaction(bool commit)
{
	SQLRETURN res = SQL_SUCCESS;
	if (!m_odbc->getUseWebService()) {
		res = SQLEndTran(SQL_HANDLE_DBC, m_ConnHandle, (commit ? SQL_COMMIT : SQL_ROLLBACK));
	} else {
#ifndef SETODBC
		// Tell Web Service to commit
		char params[256];
		Expr *ret;
		sprintf(params,"end=%s&stmtid=%lx",(commit?"Y":"N"),(long)m_StatHandle);
		m_odbc->getDM()->API(*(m_odbc->getCTX()),"/dmadminweb/API/sql",params,&ret,m_CookieJar);
		TiXmlElement *e;
		if (parseReply(ret->toString(),&e)) {
			res = SQL_SUCCESS;
		} else {
			res = SQL_ERROR;
		}
#else
		res = SQL_ERROR;
#endif
	}

	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO))
	{
		char		*MsgPtr = NULL;
		SQLINTEGER	ErrNum;

		GetLastError(&MsgPtr,&ErrNum);
		fprintf(stderr,"[%s]\n",(MsgPtr ? MsgPtr : ""));
		if(MsgPtr) free(MsgPtr);
	}

	return res;
}


/*static*/ char *triSQL::EscapeLiteral(const char *str)
{
	int len = strlen(str) + 3;
	char *ret = (char*) malloc(len);
	if (ret) {
		*ret = '\'';
		int y = 1;
		for(const char *x = str; *x; x++) {
			if(*x == '\'') {
				ret = (char*) realloc(ret, ++len);
				ret[y++] = '\'';
				ret[y++] = *x;
			} else {
				ret[y++] = *x;
			}
		}
		ret[y++] = '\'';
		ret[y] = '\0';
	}
	return ret;
}


///////////////////////////////////////////////////////////////////////////////
// COLDATA
///////////////////////////////////////////////////////////////////////////////

COLDATA::COLDATA(const SQLCHAR *colName)
	: ColumnName(colName ? strdup((const char*) colName) : NULL), BindType(0),
	  ColumnSize(0), NullInd(0), DataPtr(NULL), next(NULL)
{}


COLDATA::~COLDATA()
{
	// ColumnName is strdup'ed in constructor, so free it
	if(ColumnName) {
		free(ColumnName);
		ColumnName = NULL;
	}

	// This should be allocated by allocDataPtr()
	if(DataPtr) {
		free(DataPtr);
		DataPtr = NULL;
	}

	// Delete rest of list
	if(next) {
		delete next;
	}
}


void COLDATA::allocDataPtr()
{
	DataPtr = malloc(ColumnSize+1);
	memset(DataPtr, 0, ColumnSize+1);
}
