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

#ifndef __datasource_h

#define __datasource_h


#include "hashtable.h"
#include "model.h"


class DatasourceProviderImpl : public ProviderObjectImpl
{
protected:
	class DatasourceProviderImplFactory &m_factory;
	class Datasource &m_datasource;

	DatasourceProviderImpl(
		class DatasourceProviderImplFactory &factory, DMINT32 implId,
		class Datasource &datasource);

public:
	virtual ~DatasourceProviderImpl();

	virtual void query(class Model &model,class ExtendedStmt &stmt, class Context &ctx) = 0;

	// ProviderObjectImpl
	OBJECT_KIND kind()  { return OBJ_KIND_DATASOURCE_IMPL; }
	virtual class Expr *getAttribute(const char *name, class Context &ctx);
	//virtual class DMArray *getArrayAttribute(const char *name, class Context &ctx)  { return NULL; }
	//virtual class Object *getObjectAttribute(const char *name, class Context &ctx)  { return NULL; }
	class Object *toObject() { return NULL; }
};


class DatasourceProviderImplFactory
{
public:
	DatasourceProviderImplFactory();
	virtual ~DatasourceProviderImplFactory();

	virtual DatasourceProviderImpl *create(
		DMINT32 implId, class Datasource &parent, class ExtendedStmt &stmt,
		class Context &ctx) = 0;
};


class DatasourceProviderImplRegistry
{
private:
	Hashtable<DatasourceProviderImplFactory> m_factories;

	static DatasourceProviderImplRegistry *s_instance;

public:
	DatasourceProviderImplRegistry();

	static DatasourceProviderImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, DatasourceProviderImplFactory *factory);
	void registerBuiltIns(class DM &dm);

	DatasourceProviderImplFactory *getFactory(class ProviderObjectDef &def);
};


#endif /*__datasource_h*/
