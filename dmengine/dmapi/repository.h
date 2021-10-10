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

#ifndef __repository_h

#define __repository_h


#include "hashtable.h"
#include "dmapiexport.h"
#include "model.h"


class DMAPI_API RepositoryImpl : public ProviderObjectImpl
{
protected:
	class RepositoryImplFactory &m_factory;
	class Repository &m_repository;

	RepositoryImpl(class RepositoryImplFactory &factory, DMINT32 implId, Repository &repository);

public:
	virtual ~RepositoryImpl();

	class Repository &repository()  { return m_repository; }

	/**
	 * dzbase    - base folder of the dropzone (usually dz.pathname())
	 * dzpath    - base folder with the target path appended
	 * recursive - whether to checkout subfolders - implementation may not
	 *             support this
	 * pattern   - list of patterns to checkout - implementation may need to
	 *             run multiple checkouts for this
	 * stmt      - parent statement which provides arguments
	 * callback  - callback object used for logging files and folders that have
	 *             been checked-out and the summary
	 * ctx       - context
	 */
	virtual void checkout(
		const char *dzbase, const char *dzpath, bool recursive,
		class StringList *pattern, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx) = 0;

	/**
	 * dzbase   - base folder of the dropzone (usually dz.pathname(server))
	 * dzpath   - base folder with the target path appended
	 * stmt     - parent statement which provides arguments
	 * callback - callback object used for logging files and folders that have
	 *            been checked-in and the summary
	 * ctx      - context
	 */
	virtual void checkin(
		const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx) = 0;

	virtual char *dirname(const char *filename) = 0;

	// ProviderObjectImpl
	OBJECT_KIND kind() { return OBJ_KIND_REPOSITORY_IMPL; }
	virtual class Expr *getAttribute(const char *name, class Context &ctx);
	//virtual class DMArray *getArrayAttribute(const char *name, class Context &ctx) { return NULL; }
	//virtual class IObject *getObjectAttribute(const char *name, class Context &ctx) { return NULL; }
	class Object *toObject() { return NULL; }
};


class DMAPI_API RepositoryImplFactory
{
public:
	RepositoryImplFactory();
	virtual ~RepositoryImplFactory();

	virtual RepositoryImpl *create(
		DMINT32 implId, class Repository &parent, class ExtendedStmt &stmt,
		class Context &ctx) = 0;
};


class DMAPI_API RepositoryImplRegistry
{
private:
	Hashtable<RepositoryImplFactory> m_factories;

	static RepositoryImplRegistry *s_instance;

public:
	RepositoryImplRegistry();

	static RepositoryImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, RepositoryImplFactory *factory);
	void registerBuiltIns(class DM &dm);

	RepositoryImplFactory *getFactory(class ProviderObjectDef &def);
};


#endif /*__repository_h*/
