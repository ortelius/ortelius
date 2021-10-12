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

#ifndef __repositories_filesystem_h

#define __repositories_filesystem_h


#include "../../dmapi/repository.h"


class FilesystemRepositoryImpl : public virtual RepositoryImpl
{
private:
	char *m_filepath;
	char *m_version;

	bool copyFile(const char *sourcePath, const char *targetPath);

	void ensureFolder(
		const char *dzbase, const char *dzpath,
		class DropzoneCallback &callback, class Context &ctx);

	void checkoutFolder(
		const char *dzbase, const char *dzpath, const char *repobase,
		const char *repopath, bool recursive, StringList *pattern,
		class DropzoneCallback &callback, class Context &ctx);

public:
	FilesystemRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId,
		class Repository &repository,
		const char *filepath, const char *version);

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


class FilesystemRepositoryImplFactory : public virtual RepositoryImplFactory
{
public:
	FilesystemRepositoryImplFactory();

	RepositoryImpl *create(
		DMINT32 implId, class Repository &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__repositories_filesystem_h*/
