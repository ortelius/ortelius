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

#ifdef WIN32
#include <windows.h>
#define IISPLUGIN_EXPORT __declspec(dllexport)
#else
#define IISPLUGIN_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#include "iisplugin.h"

#include "dm.h"
#include "context.h"
#include "expr.h"


#define IISPLUGIN_PLUGIN_NAME "iisplugin"
#define IISPLUGIN_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////
// IIS_GetSitesStmtImpl
///////////////////////////////////////////////////////////////////////////////

IIS_GetSitesStmtImpl::IIS_GetSitesStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void IIS_GetSitesStmtImpl::execute(class Context &ctx)
{
	ctx.dm().writeToStdOut("IIS_GetSitesStmtImpl says hello");
}


///////////////////////////////////////////////////////////////////////////////
// IIS_GetSitesStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

IIS_GetSitesStmtImplFactory::IIS_GetSitesStmtImplFactory()
{}


ExtendedStmtImpl *IIS_GetSitesStmtImplFactory::create(class DM &dm, ExtendedStmt &parent)
{
	return new IIS_GetSitesStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// IIS_GetVersionFunctionImpl
///////////////////////////////////////////////////////////////////////////////

IIS_GetVersionFunctionImpl::IIS_GetVersionFunctionImpl(FunctionNode &parent)
	: m_parent(parent)
{}


class Expr *IIS_GetVersionFunctionImpl::evaluate(class ExprList *args, class Context &ctx, bool stringify)
{
	ctx.dm().writeToStdOut("IIS_GetVersionFunctionImpl says hello");

	return new Expr("1.23");
}


///////////////////////////////////////////////////////////////////////////////
// IIS_GetVersionFunctionImplFactory
///////////////////////////////////////////////////////////////////////////////

IIS_GetVersionFunctionImplFactory::IIS_GetVersionFunctionImplFactory()
{}

FunctionNodeImpl *IIS_GetVersionFunctionImplFactory::create(class DM &dm, FunctionNode &parent)
{
	return new IIS_GetVersionFunctionImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////

extern "C" IISPLUGIN_EXPORT int PluginStart(DM &dm)
{
	dm.writeToLogFile("IIS plugin V" IISPLUGIN_PLUGIN_VERSION);
	
	ExtendedStmtImplRegistry::instance()
		.registerFactory("iis_getsites", new IIS_GetSitesStmtImplFactory());
	FunctionNodeImplRegistry::instance()
		.registerFactory("iis_getversion", new IIS_GetVersionFunctionImplFactory());
	return 0;
}


extern "C" IISPLUGIN_EXPORT int PluginInstall(DM &dm)
{
	//List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	//propdefs->add(new PropertyDef("broker", true, false, false));
	//propdefs->add(new PropertyDef("project", false, false, false));
	//propdefs->add(new PropertyDef("state", false, false, false));
	//propdefs->add(new PropertyDef("viewpath", false, false, true));

	//dm.installProviderImpl(HARVEST_REPO_NAME, HARVEST_PLUGIN_NAME, OBJ_KIND_RESPOSITORY, propdefs);
	return 0;
}