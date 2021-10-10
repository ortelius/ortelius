/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
