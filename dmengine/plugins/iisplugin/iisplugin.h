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
