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
