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
