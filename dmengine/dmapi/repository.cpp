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
extern "C" int ftp_PluginStart(DM &dm);

void RepositoryImplRegistry::registerBuiltIns(DM &dm)
{
	//registerFactory("harvest", new HarvestRepositoryImplFactory());
	//registerFactory("svn",        new SvnRepositoryImplFactory());
	//registerFactory("filesystem", new FilesystemRepositoryImplFactory());

	svn_PluginStart(dm);
	filesystem_PluginStart(dm);
	ftp_PluginStart(dm);
}


RepositoryImplFactory *RepositoryImplRegistry::getFactory(ProviderObjectDef &def)
{
	return m_factories.get(def.name());
}
