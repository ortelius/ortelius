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
#ifndef __function_h

#define __function_h


#include "hashtable.h"
#include "dmapiexport.h"


class DMAPI_API FunctionNodeImpl
{
public:
	virtual ~FunctionNodeImpl();

	virtual class Expr* evaluate(class ExprList *args, class Context &ctx, bool stringify) = 0;
};


class DMAPI_API FunctionNodeImplFactory
{
public:
	virtual ~FunctionNodeImplFactory();

	virtual FunctionNodeImpl *create(class DM &dm, class FunctionNode &parent) = 0;
};


class DMAPI_API FunctionNodeImplRegistry
{
private:
	Hashtable<FunctionNodeImplFactory> m_factories;

	static FunctionNodeImplRegistry *s_instance;

public:
	FunctionNodeImplRegistry();

	static FunctionNodeImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, FunctionNodeImplFactory *factory);
	void registerBuiltIns();

	FunctionNodeImplFactory *getFactory(const char *name);
};


class DMAPI_API LocalScriptFunctionImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;
	class Action &m_action;

public:
	LocalScriptFunctionImpl(class FunctionNode &parent, class Action &action);
	~LocalScriptFunctionImpl();

	class Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


class DMAPI_API RemoteScriptFunctionImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;
	class Action &m_action;

	class Expr *evaluateOne(
		class Server &target, class TransferProviderImpl &xfer,
		class ExprList *args, class Context &ctx, bool stringify);

public:
	RemoteScriptFunctionImpl(class FunctionNode &parent, class Action &action);
	~RemoteScriptFunctionImpl();

	class Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


#endif /*__function_h*/
