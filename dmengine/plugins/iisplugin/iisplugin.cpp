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