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
#ifndef __plugins_iisplugin_h

#define __plugins_iisplugin_h


#include "extended.h"
#include "function.h"


class IIS_GetSitesStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	IIS_GetSitesStmtImpl(class ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class IIS_GetSitesStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	IIS_GetSitesStmtImplFactory();

	bool allowsBody()		{ return true; }
	bool allowsPrePost()	{ return false; }
	bool isThreaded()       { return false; }

	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


class IIS_GetVersionFunctionImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;

public:
	IIS_GetVersionFunctionImpl(class FunctionNode &parent);

	class Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


class IIS_GetVersionFunctionImplFactory : public virtual FunctionNodeImplFactory
{
public:
	IIS_GetVersionFunctionImplFactory();

	FunctionNodeImpl *create(class DM &dm, class FunctionNode &parent);
};


#endif /*__plugins_iisplugin_h*/
