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
