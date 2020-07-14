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
