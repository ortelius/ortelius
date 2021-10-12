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

#include "modify.h"
#include "exceptions.h"
#include "context.h"
#include "model.h"
#include "scopestack.h"
#include "node.h"


///////////////////////////////////////////////////////////////////////////////
// ModifyProviderImpl
///////////////////////////////////////////////////////////////////////////////

ModifyProviderImpl::ModifyProviderImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


/*virtual*/ ModifyProviderImpl::~ModifyProviderImpl()
{}


///////////////////////////////////////////////////////////////////////////////
// ModifyProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ ModifyProviderImplFactory::~ModifyProviderImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// ModifyProviderImplRegistry
///////////////////////////////////////////////////////////////////////////////

ModifyProviderImplRegistry *ModifyProviderImplRegistry::s_instance = NULL;


ModifyProviderImplRegistry::ModifyProviderImplRegistry()
	: m_factories(false, true)
{}


/*static*/ ModifyProviderImplRegistry &ModifyProviderImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new ModifyProviderImplRegistry();
	}
	return *s_instance;
}


/*static*/ void ModifyProviderImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void ModifyProviderImplRegistry::registerFactory(
		const char *name, ModifyProviderImplFactory *factory)
{
	m_factories.put(name, factory);
}


extern "C" int xmlmodify_PluginStart(DM &dm);
extern "C" int textmodify_PluginStart(DM &dm);

void ModifyProviderImplRegistry::registerBuiltIns(class DM &dm)
{
	xmlmodify_PluginStart(dm);
	textmodify_PluginStart(dm);
}


ModifyProviderImplFactory *ModifyProviderImplRegistry::getFactory(
		const char *name)
{
	return m_factories.get(name);
}


///////////////////////////////////////////////////////////////////////////////
// ModifyProviderStmtImpl
///////////////////////////////////////////////////////////////////////////////

ModifyProviderStmtImpl::ModifyProviderStmtImpl(class ExtendedStmt &parent)
	: m_parent(parent)
{}


/*virtual*/ ModifyProviderStmtImpl::~ModifyProviderStmtImpl()
{}


void ModifyProviderStmtImpl::execute(class Context &ctx)
{
	Scope *scope = ctx.stack().getScope(MODIFY_SCOPE);
	if(!scope) {
		throw SyntaxError(m_parent, ctx.stack(), 
			"modify statement '%s' is not valid outside modify", m_parent.ident());
	}

	IObject *obj = scope->getScopeObject();
	if(obj) {
		// This cast is okay here as we know the scope object is a modify
		((ModifyStmtImpl*) obj)->executeSubStmt(m_parent, ctx);
	}
}


///////////////////////////////////////////////////////////////////////////////
// ModifyProviderStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ ModifyProviderStmtImplFactory::~ModifyProviderStmtImplFactory()
{}


ExtendedStmtImpl *ModifyProviderStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new ModifyProviderStmtImpl(parent);
}
