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

#ifndef __repositories_http_h

#define __repositories_http_h


#include "repository.h"


///////////////////////////////////////////////////////////////////////////////
// HttpRepository

class HttpRepositoryImpl : public virtual RepositoryImpl
{
private:
	char	*m_host;
	int		m_port;
	char	*m_uri;
	char	*m_params;
	char	*m_version;
	char	*m_logfile;
	bool	m_secure;
	bool	CheckFileExists(int sock,char *AuthenticationString,char *offset, char *patt);
	char	**GetDirectoryContent(Context &ctx,/*int sock,*/char *AuthenticationString,char *offset,int *numFiles, FILE *logout);
	void	FreeDirectoryContent(char **dirlist, int numElements);

public:
	HttpRepositoryImpl(
		class RepositoryImplFactory &factory, DMINT32 implId, class Repository &repository,
		const char *host, int port, bool ssl, const char *uri, const char *params, const char *version, const char *logfile);

	virtual ~HttpRepositoryImpl();

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


class HttpRepositoryImplFactory : public virtual RepositoryImplFactory
{
public:
	HttpRepositoryImplFactory();

	RepositoryImpl *create(
		DMINT32 implId, class Repository &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__repositories_http_h*/
