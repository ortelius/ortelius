#include <stdio.h>

#include "repository.h"
#include "model.h"


///////////////////////////////////////////////////////////////////////////////
// RepositoryImpl
///////////////////////////////////////////////////////////////////////////////

RepositoryImpl::RepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId, Repository &repository
	)
	: ProviderObjectImpl(implId, repository), m_factory(factory), m_repository(repository)
{}


/*virtual*/ RepositoryImpl::~RepositoryImpl()
{}


Expr *RepositoryImpl::getAttribute(const char *name, class Context &ctx)
{
	return ProviderObjectImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// RepositoryImplFactory
///////////////////////////////////////////////////////////////////////////////

RepositoryImplFactory::RepositoryImplFactory()
{}


/*virtual*/ RepositoryImplFactory::~RepositoryImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// RepositoryImplRegistry
///////////////////////////////////////////////////////////////////////////////

RepositoryImplRegistry *RepositoryImplRegistry::s_instance = NULL;


RepositoryImplRegistry::RepositoryImplRegistry()
	: m_factories(false, true)
{}


/*static*/ RepositoryImplRegistry &RepositoryImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new RepositoryImplRegistry();
	}
	return *s_instance;
}


/*static*/ void RepositoryImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void RepositoryImplRegistry::registerFactory(const char *name, RepositoryImplFactory *factory)
{
	m_factories.put(name, factory);
}


// Temp includes whilst these are not plugins
// TODO: These will become DLLs/.so's which register themselves
extern "C" int svn_PluginStart(DM &dm);
extern "C" int filesystem_PluginStart(DM &dm);

void RepositoryImplRegistry::registerBuiltIns(DM &dm)
{
	//registerFactory("harvest", new HarvestRepositoryImplFactory());
	//registerFactory("svn",        new SvnRepositoryImplFactory());
	//registerFactory("filesystem", new FilesystemRepositoryImplFactory());

	svn_PluginStart(dm);
	filesystem_PluginStart(dm);
}


RepositoryImplFactory *RepositoryImplRegistry::getFactory(ProviderObjectDef &def)
{
	return m_factories.get(def.name());
}
