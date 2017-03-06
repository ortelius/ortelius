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
#ifndef __plugins_restful_h

#define __plugins_restful_h


#include "extended.h"
#include "function.h"


///////////////////////////////////////////////////////////////////////////////
// Post

class Restful_PostStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	Restful_PostStmtImpl(class ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class Restful_PostStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	Restful_PostStmtImplFactory();

	bool allowsBody()		{ return false; }
	bool allowsPrePost()	{ return false; }
	bool isThreaded()       { return false; }

	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


///////////////////////////////////////////////////////////////////////////////
// Post Function

class Restful_PostFunctionImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;

public:
	Restful_PostFunctionImpl(class FunctionNode &parent);

	class Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


class Restful_PostFunctionImplFactory : public virtual FunctionNodeImplFactory
{
public:
	Restful_PostFunctionImplFactory();

	FunctionNodeImpl *create(class DM &dm, class FunctionNode &parent);
};

#endif /*__plugins_restful_h*/
