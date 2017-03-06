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
#ifndef __repositories_ftp_h

#define __repositories_ftp_h

// #include "repository.h"


#include "../../dmapi/repository.h"
#include "../../dmapi/credentials.h"
#include "../../dmapi/extended.h"
#include "../../dmapi/model.h"

#ifdef WIN32
typedef HMODULE LIBPTR;
#define DLOPEN(a) LoadLibrary(a)
#define DLSYM(a,b) GetProcAddress(a,b)
#include <sys/utime.h>
#else
typedef void * LIBPTR;
#define DLOPEN(a) dlopen(a,RTLD_LAZY)
#define DLSYM(a,b) dlsym(a,b)
#include <sys/time.h>
#endif

typedef enum err_tag{
ERROR_NO_ERROR,
ERROR_INVALID_PROTOCOL,
ERROR_COULD_NOT_FIND_LIBRARY,
ERROR_NO_INITIALISE_FUNCTION,
ERROR_NO_CONNECT_FUNCTION,
ERROR_NO_DISCONNECT_FUNCTION,
ERROR_NO_PUT_FUNCTION,
ERROR_NO_GET_FUNCTION,
ERROR_NO_EXEC_FUNCTION,
ERROR_NO_CD_FUNCTION,
ERROR_NO_CALLBACK_FUNCTION,
ERROR_NO_RETURN_LAST_ERROR_FUNCTION,
ERROR_NO_LCD_FUNCTION,
ERROR_NO_MKDIR_FUNCTION,
ERROR_NO_PWD_FUNCTION,
ERROR_NO_FILE_LIST_FUNCTION,
ERROR_NO_TEXT_FUNCTION,
ERROR_NO_BINARY_FUNCTION,
ERROR_NO_GET_ATTRIBUTES_FUNCTION,
ERROR_NO_SET_ATTRIBUTES_FUNCTION,
ERROR_NO_DELETE_FUNCTION,
ERROR_INITIALISE_FAILED
} ERRORCODE;

typedef enum callback_reason_tag{
CALLBACK_NO_REASON,
CALLBACK_KEY_DIFFERS,
CALLBACK_KEY_NEW,
CALLBACK_ACCESS_DENIED,
CALLBACK_REMOTE_DIRECTORY,
CALLBACK_PUT,
CALLBACK_PRECONNECT
} CALLBACK_REASON;

typedef int (*CB_FUNCTION)(CALLBACK_REASON res,void *lParam,void *wParam);

typedef int					(*PROTOCOL_INITIALISE)(void *Data);
typedef int					(*PROTOCOL_CONNECT)(char *HostName,char *UserName,char *Password);
typedef int					(*PROTOCOL_PUT)(char *Filename);
typedef int					(*PROTOCOL_GET)(char *Filename);
typedef int					(*PROTOCOL_DISCONNECT)();
typedef char *				(*PROTOCOL_EXEC)(char **argv,bool CopyFromLocal);
typedef int					(*PROTOCOL_CD)(char *NewDir);
typedef void				(*PROTOCOL_REGISTER_CALLBACK)(CB_FUNCTION cbf);
typedef char *				(*PROTOCOL_RETURN_LAST_ERROR)(void);
typedef int					(*PROTOCOL_LCD)(char *NewDir);
typedef int					(*PROTOCOL_MKDIR)(char *Dir);
typedef int					(*PROTOCOL_PUT)(char *FileName);
typedef char *              (*PROTOCOL_PWD)();
typedef char **				(*PROTOCOL_GET_FILE_LIST)(char *patt,bool recursive);
typedef void				(*PROTOCOL_TEXT)();
typedef void				(*PROTOCOL_BINARY)();
typedef char **				(*PROTOCOL_GET_ATTRIBUTES)(char *folder);
typedef void				(*PROTOCOL_SET_ATTRIBUTES)(char *folder,char *filename,char *mbrtype,char *mbrtext);
typedef int					(*PROTOCOL_DELETE)(char *Filename);
///////////////////////////////////////////////////////////////////////////////
// CredentialsProvider

class FtpCredentialsProvider : public virtual CredentialsProvider
{
private:
	char *m_dfoFileName;

public:
	FtpCredentialsProvider(
		class CredentialsProviderFactory &factory,
		class Credentials &credentials);
	~FtpCredentialsProvider();

	bool canTransform();

	const char *transform(class Object &auth, class Context &ctx);
};


class FtpCredentialsProviderFactory : public virtual CredentialsProviderFactory
{
public:
	FtpCredentialsProviderFactory();

	CredentialsProvider *create(class Credentials &parent);
};


class FtpRepositoryImpl : public virtual RepositoryImpl
{
private:
	char	*m_filepath;
	char	*m_version;
	char	*m_server;
	LIBPTR	m_ftplib;
	bool	m_connected;
	

	PROTOCOL_INITIALISE			protocol_initialise;
	PROTOCOL_CONNECT			protocol_connect;
	PROTOCOL_PUT				protocol_put;
	PROTOCOL_GET				protocol_get;
	PROTOCOL_DISCONNECT			protocol_disconnect;
	PROTOCOL_EXEC				protocol_exec;
	PROTOCOL_CD					protocol_cd;
	PROTOCOL_REGISTER_CALLBACK	protocol_register_callback;
	PROTOCOL_RETURN_LAST_ERROR	protocol_return_last_error;
	PROTOCOL_LCD				protocol_lcd;
	PROTOCOL_MKDIR				protocol_mkdir;
	PROTOCOL_PWD				protocol_pwd;
	PROTOCOL_GET_FILE_LIST		protocol_get_file_list;
	PROTOCOL_TEXT				protocol_text;
	PROTOCOL_BINARY				protocol_binary;
	PROTOCOL_GET_ATTRIBUTES		protocol_get_attributes;
	PROTOCOL_SET_ATTRIBUTES		protocol_set_attributes;
	PROTOCOL_DELETE				protocol_delete;

	bool copyFile(const char *sourcePath, const char *targetPath);

	void handleCredentials(class ExtendedStmt &stmt, class Credentials *&creds,class Context &ctx);

	void ensureFolder(
		const char *dzbase, const char *dzpath,
		class DropzoneCallback &callback, class Context &ctx);

	void checkoutFolder(
		const char *dzbase, const char *dzpath, const char *repobase,
		const char *repopath, bool recursive, StringList *pattern,
		class DropzoneCallback &callback, class Context &ctx);

	void LoadFtpLibrary(bool usessl);
	RepositoryTextPattern *getTextPatternList(const char *dir,const char *basefile);

public:
	FtpRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId,
		class Repository &repository,
		const char *server, const char *filepath, const char *version,
		bool usessl);

	bool testProvider(class Context &ctx);

	void checkout(
		const char *dzbase, const char *dzpath, bool recursive,
		class StringList *pattern, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx);

	void checkin(
		const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx);

	char *dirname(const char *filename);

	// RepositoryImpl
	class Expr *getAttribute(const char *name, class Context &ctx);
};


class FtpRepositoryImplFactory : public virtual RepositoryImplFactory
{
public:
	FtpRepositoryImplFactory();

	RepositoryImpl *create(
		DMINT32 implId, class Repository &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__repositories_ftp_h*/
