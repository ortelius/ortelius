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

#include <stdio.h>

#include "datasource.h"


///////////////////////////////////////////////////////////////////////////////
// DatasourceProviderImpl
///////////////////////////////////////////////////////////////////////////////

DatasourceProviderImpl::DatasourceProviderImpl(
		DatasourceProviderImplFactory &factory,
		DMINT32 implId, Datasource &datasource
	)
	: ProviderObjectImpl(implId, datasource),
	  m_factory(factory), m_datasource(datasource)
{}


/*virtual*/ DatasourceProviderImpl::~DatasourceProviderImpl()
{}


Expr *DatasourceProviderImpl::getAttribute(const char *name, class Context &ctx)
{
	return ProviderObjectImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// DatasourceProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

DatasourceProviderImplFactory::DatasourceProviderImplFactory()
{}


/*virtual*/ DatasourceProviderImplFactory::~DatasourceProviderImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// DatasourceProviderImplRegistry
///////////////////////////////////////////////////////////////////////////////

DatasourceProviderImplRegistry *DatasourceProviderImplRegistry::s_instance = NULL;


DatasourceProviderImplRegistry::DatasourceProviderImplRegistry()
	: m_factories(false, true)
{}


/*static*/ DatasourceProviderImplRegistry &DatasourceProviderImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new DatasourceProviderImplRegistry();
	}
	return *s_instance;
}


/*static*/ void DatasourceProviderImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void DatasourceProviderImplRegistry::registerFactory(const char *name, DatasourceProviderImplFactory *factory)
{
	m_factories.put(name, factory);
}


extern "C" int odbcdatasource_PluginStart(DM &dm);

void DatasourceProviderImplRegistry::registerBuiltIns(DM& dm)
{
#ifdef HAVE_ODBC
	//registerFactory("odbc", new OdbcDatasourceImplFactory());
	odbcdatasource_PluginStart(dm);
#endif /*HAVE_ODBC*/
	//registerFactory("postgres", new PostgresDatasourceImplFactory());
}


DatasourceProviderImplFactory *DatasourceProviderImplRegistry::getFactory(ProviderObjectDef &def)
{
	return m_factories.get(def.name());
}
