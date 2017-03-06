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
