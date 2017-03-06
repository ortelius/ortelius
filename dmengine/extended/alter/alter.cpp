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
#include <stdio.h>

#include "alter.h"

#include "../../dmapi/context.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/scopestack.h"
#include "../../dmapi/dm.h"


//////////////////////////////////////////////////////////////////////////////
// AlterStmtImpl
//////////////////////////////////////////////////////////////////////////////

AlterStmtImpl::AlterStmtImpl(class ExtendedStmt &parent)
	: m_parent(parent)
{}


AlterStmtImpl::~AlterStmtImpl()
{}


void AlterStmtImpl::execute(class Context &ctx)
{
	ExprPtr eobject = m_parent.getArg("object", ctx);
	if(!eobject) {
		throw RuntimeError(m_parent, ctx.stack(), "\"object\" must be specified");
	}
	ObjectReference *oref = eobject->toObjectReference();
	Object *obj = oref ? oref->toObject() : NULL;
	if(!obj) {
		throw RuntimeError(m_parent, ctx.stack(), "\"object\" must be an object");
	}

	ExprPtr eattrs = m_parent.getArg("attributes", ctx);
	if(!eattrs) {
		throw RuntimeError(m_parent, ctx.stack(), "\"attributes\" must be specified");
	}
	DMArray *attrs = eattrs->toArray();
	if(!attrs) {
		throw RuntimeError(m_parent, ctx.stack(), "\"attributes\" must be an array");
	}

	ctx.dm().getModel()->alterObjectVars(obj,attrs);

	debug1("ALTER OBJECT %d '%s'", obj->id(), obj->name());
}


//////////////////////////////////////////////////////////////////////////////
// AlterStmtImpl
//////////////////////////////////////////////////////////////////////////////

AlterStmtImplFactory::AlterStmtImplFactory()
{}


ExtendedStmtImpl *AlterStmtImplFactory::create(class DM &dm, class ExtendedStmt &parent)
{
	return new AlterStmtImpl(parent);
}
