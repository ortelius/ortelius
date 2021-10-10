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

#ifndef __extended_appverloop_h

#define __extended_appverloop_h


#include "../../dmapi/extended.h"


///////////////////////////////////////
// AppVerLoop
///////////////////////////////////////

class AppVerLoopStmtImpl : public /*virtual*/ ExtendedStmtImpl, public IObject
{
private:
	class ExtendedStmt &m_parent;
	bool m_isFirst;
	bool m_isLast;
	class Application *m_app;
	class Scope *m_appVerScope;
	class ApplicationVersion *m_tgtAppVer;
	class ApplicationVersion *m_cacheFirst;
	class ApplicationVersion *m_cacheLast;

	void findApp(class Context &ctx);
	class ApplicationVersion *calcFrom(class Context &ctx);
	class ApplicationVersion *calcTo(class Context &ctx);

public:
	AppVerLoopStmtImpl(class ExtendedStmt &parent);
	~AppVerLoopStmtImpl();

	void execute(class Context &ctx);

	bool isFirst()  { return m_isFirst; }
	bool isLast()   { return m_isLast; }

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class AppVerLoopStmtImplFactory : public /*virtual*/ ExtendedStmtImplFactory
{
public:
	AppVerLoopStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__extended_appverloop_h*/
