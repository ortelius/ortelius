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
