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
#ifndef __extended_alter_h

#define __extended_alter_h


#include "../../dmapi/extended.h"


//////////////////////////////////////////////////////////////////////////////
// Alter
//////////////////////////////////////////////////////////////////////////////

class AlterStmtImpl : public ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	AlterStmtImpl(class ExtendedStmt &parent);
	~AlterStmtImpl();

	void execute(class Context &ctx);
};


class AlterStmtImplFactory : public ExtendedStmtImplFactory
{
public:
	AlterStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__extended_alter_h*/
