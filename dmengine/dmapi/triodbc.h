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
#ifndef _TRIODBC_H

#define _TRIODBC_H

#ifdef HAVE_DMAPIEXPORT
#include "dmapiexport.h"
#else
// Neither export nor import the classes
#define DMAPI_API
#endif /*HAVE_DMAPIEXPORT*/

#ifdef WIN32
#include <windows.h>
#include <windef.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#ifndef _WIN64
#define SQLLEN SQLINTEGER
#endif /*_WIN64*/
#else
#include "sql.h"
#include "sqlext.h"
#include "sqltypes.h"
#endif
#ifdef THIS_IS_NOT_DEFINED
#  if defined(__sun)
     typedef int bool;
#    define true 1
#    define false 0
#  endif
#endif

#include "tinyxml.h"	// for TiXmlElement

// For convenience - RHT 19/12/2013
#define IS_SQL_SUCCESS(x)		((x == SQL_SUCCESS) || (x == SQL_SUCCESS_WITH_INFO))
#define IS_NOT_SQL_SUCCESS(x)	((x != SQL_SUCCESS) && (x != SQL_SUCCESS_WITH_INFO))


class COLDATA {
public:
	char			*ColumnName;
	SQLSMALLINT		BindType;
	SQLINTEGER		ColumnSize;
	SQLLEN			NullInd;		// RHT 07/08/2012 - was SQLINTEGER
	void			*DataPtr;
	class COLDATA	*next;

	COLDATA(const SQLCHAR *colName);
	~COLDATA();

	void allocDataPtr();
};

typedef struct tabdata_t{
char				*TableName;
char				*TableType;
struct tabdata_t	*next;
} TABLEDATA;

typedef struct charcols_t{
char				*DataPtr;
SQLINTEGER			DataLength;
struct charcols_t	*next;
} CHARCOLS;


// RHT 14/05/2011 - Database type
typedef enum db_type_tag{
DATABASE_TYPE_UNKNOWN=0,
DATABASE_TYPE_ORACLE=10,
DATABASE_TYPE_POSTGRES=20
} DATABASE_TYPE;

typedef struct bind_params_tag{
SQLSMALLINT TargetType;
SQLSMALLINT Precision;
SQLPOINTER TargetValuePtr;
SQLLEN BufferLength;
} BIND_PARAMS;

typedef struct bind_columns_tag{
SQLSMALLINT TargetType;
SQLPOINTER TargetValuePtr;
SQLLEN BufferLength;
SQLPOINTER NullIndicator;
} BIND_COLUMNS;

class DMAPI_API triSQL
{
	friend class triODBC;

private:
	SQLHSTMT		m_StatHandle;
	SQLHDBC			m_ConnHandle;
	bool			m_StatHandleSet;
	CHARCOLS		*CharCols;
	CHARCOLS		*xCol;
	class triODBC	*m_odbc;
	int				m_maxBindParams;
	BIND_PARAMS		*m_BindParams;
	int				m_maxBindColumns;
	BIND_COLUMNS	*m_BindColumns;
	class DMArray	*m_CookieJar;
	char			***m_returnedData;
	int				m_retColCount;
	int				m_retRowCount;
	int				m_currentRow;
	char			*m_LastErrorText;
	int				m_LastErrorNum;
	int				m_ParamCount;
	int				m_RowsUpdated;

	long			AllocateStatementHandle(SQLHDBC ConnHandle);
	long			FreeStatementHandle();
	void			CloseAnyOpenHandles();
	unsigned long	m_RowArraySize;
	unsigned long	m_NumRowsFetched;
	SQLLEN			*m_IndicatorArray;
	SQLLEN			m_SingleIndicator;
	void			setODBC(triODBC *odbc);
	void			setCookieJar(class DMArray *cookieJar);
	bool			parseReply(char *xmlstr,TiXmlElement **result);

	triSQL();
	triSQL(triSQL &rhs);

public:
	~triSQL();

	long		BindColumn(SQLUSMALLINT,SQLSMALLINT,SQLPOINTER,SQLINTEGER,SQLLEN *ni=NULL);	// RHT 07/08/2012 - was SQLINTEGER
	SQLRETURN	BindParameter(SQLUSMALLINT,SQLSMALLINT,SQLUINTEGER,SQLPOINTER,SQLINTEGER);
	SQLRETURN	Execute();
	SQLRETURN	ExecuteIgnoringErrors();
	SQLRETURN	ExecuteSQL(const char *,...);
	SQLRETURN	ExecuteSQLIgnoringErrors(const char *,...);
	SQLRETURN	PrepareStatement(const char *SQLformat, ...);
	SQLRETURN	ParamData();
	SQLRETURN	PutData(SQLPOINTER TargetValue,SQLLEN DataLength);
	SQLRETURN	GetData(SQLUSMALLINT ColumnNumber,SQLSMALLINT	TargetType,SQLPOINTER TargetValuePtr,SQLLEN DataLength, SQLLEN *ni);	// RHT 04/12/2013
	long		GetRowsReturned();
	long		GetRowCount();
	SQLRETURN	FetchRow();
	void		GetLastError(char **MsgPtr,SQLINTEGER *ErrNum);
	long		CloseSQL();
	COLDATA		*GetColumnInfo(unsigned char *TableName);
	TABLEDATA	*GetTableInfo(unsigned char *TableName);
	COLDATA		*GetColumnInfoForStatement();
	SQLRETURN	SetAutoCommitMode(bool autoCommit);
	SQLRETURN	EndTransaction(bool commit);
	static char *EscapeLiteral(const char *str);
};


class DMAPI_API triODBC
{
private:
	SQLHENV			m_EnvHandle;
	SQLHDBC			m_ConnHandle;
	triSQL			m_triSQL;
	bool			m_EnvHandleSet;
	bool			m_ConnHandleSet;

	char			*m_UserName;
	char			*m_Password;
	char			*m_dsn;
	char			m_dbname[256];
	char			m_drivername[256];
	char			m_dmver[256];
	char			m_driverver[256];
	char			*m_nowcolname;
	DATABASE_TYPE	m_DatabaseType;
	class DM		*m_dm;
	class Context	*m_ctx;

	char			*m_clientid;
	bool			m_useWebService;	// If set all queries are routed to the webservice
	class DMArray	*m_CookieJar;
	char			*m_LastErrorText;
	int				m_LastErrorNum;


	long			AllocateAndSetEnvironmentHandle();
	long			AllocateConnectionHandle();
	long			FreeEnvironmentHandle();
	long			FreeConnectionHandle();
	bool			parseReply(char *xmlstr,TiXmlElement **result);

public:

	long			ConnectToDataSource(DM *dm);
	long			ConnectToDataSource(class DM *dm,char *DSN,char *Username,char *Password);
	long			DisconnectFromDataSource();
	void			GetLastError(char **MsgPtr,SQLINTEGER *ErrNum);
	char			*GetDatabaseVendor(){return m_dbname;}
	char			*GetDSN(){return m_dsn;}
	char			*GetDMVer(){return m_dmver;}
	char			*GetDriverVer(){return m_driverver;}
	char			*GetDriverName(){return m_drivername;}
	char			*GetUserName(){return m_UserName;}
	char			*getNowCol(){return m_nowcolname;}
	triSQL			*GetSQL();
	DATABASE_TYPE	GetDatabaseType();
	class DM		*getDM(){return m_dm;}
	bool			getUseWebService() {return m_useWebService;};
	class Context	*getCTX(){return m_ctx;}
	
	
	triODBC();
	triODBC(class Context *ctx,char *clientid);
	~triODBC();

	
};

void DecryptBuffer(char *Buffer,long BufferLen);
void EncryptBuffer(char *Buffer,long BufferLen);
void sgenrand(unsigned long seed);
unsigned long genrand();
#endif /*_TRIODBC_H*/
