#include <stdio.h>

#include "appverloop.h"

#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/scopestack.h"


///////////////////////////////////////////////////////////////////////////////
// AppVerLoopStmtImpl
///////////////////////////////////////////////////////////////////////////////

AppVerLoopStmtImpl::AppVerLoopStmtImpl(ExtendedStmt &parent)
	: m_parent(parent), m_isFirst(true), m_isLast(false),
	  m_app(NULL), m_appVerScope(NULL), m_tgtAppVer(NULL),
	  m_cacheFirst(NULL), m_cacheLast(NULL)
{}


AppVerLoopStmtImpl::~AppVerLoopStmtImpl()
{
	m_appVerScope = NULL;	// don't own
	m_tgtAppVer = NULL;		// don't own
	m_cacheFirst = NULL;	// don't own
	m_cacheLast = NULL;		// don't own
}


void AppVerLoopStmtImpl::findApp(Context &ctx)
{
	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	m_app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if(!m_app) {
		ctx.stack().dump();
		throw RuntimeError(m_parent, ctx.stack(), "No application on stack");
	}
	OBJECT_KIND ok = m_app->kind();
	if(ok == OBJ_KIND_APPVERSION || ok == OBJ_KIND_APPLICATION) {
		m_appVerScope = appScope;
		m_tgtAppVer = (ok == OBJ_KIND_APPLICATION)?(ApplicationVersion *)m_app:m_app->toApplicationVersion();
		if (ok == OBJ_KIND_APPVERSION) {
			m_app = m_tgtAppVer->getParent();
			if(!m_app) {
				//throw RuntimeError(m_parent, ctx.stack(),
				//	"Application version '%s' has no parent application",
				//	m_tgtAppVer->name());
			}
		}
	}
}


ApplicationVersion *AppVerLoopStmtImpl::calcFrom(Context &ctx)
{
	if(!m_app) {
		findApp(ctx);
	}

	ApplicationVersion *avfrom = NULL;

	// From can be either an ApplicationVersion object (including null) OR a
	// string naming an application version.  If not specified we look at what
	// we have recorded in the DB.
	ExprPtr efrom = m_parent.getArg("from", ctx);
	if(efrom) {
		if(efrom->kind() == KIND_OBJECT) {
			ObjectReference *ofrom = efrom->toObjectReference();
			// null is allowed, so we leave avfrom as null
			if(ofrom) {
				Object *obj = ofrom->toObject();
				if(!obj) {
					throw RuntimeError(m_parent, ctx.stack(),
						"value for \"from\" is not a valid application version");
				}
				if(obj->kind() != OBJ_KIND_APPVERSION) {
					throw RuntimeError(m_parent, ctx.stack(),
						"'%s' is not a valid application version (a)", obj->name());
				}
				avfrom = (ApplicationVersion*) obj;
			}
		} else {
			ConstCharPtr from = efrom->toString();
			avfrom = m_app->getVersion(from);
			if(!avfrom) {
				throw RuntimeError(m_parent, ctx.stack(),
					"'%s' is not a valid application version (b)", (const char*) from);
			}
		}
	} else {
		Environment *env = ctx.dm().getTargetEnvironment();
		if(env) {
			Application *temp = env->getDeployedApplication(*m_app);
			if(temp) {
				avfrom = temp->toApplicationVersion();
				if(!avfrom) {
					throw RuntimeError(m_parent, ctx.stack(),
						"'%s' is not a valid application version (c)", temp->name());
				}
			}
		}
	}
	return avfrom;
}


ApplicationVersion *AppVerLoopStmtImpl::calcTo(Context &ctx)
{
	if(!m_app) {
		findApp(ctx);
	}

	/*
	ConstCharPtr to = m_parent.getArgAsString("to", ctx);
	if(!to && !m_tgtAppVer) {	
		throw RuntimeError(m_parent, ctx.stack(),
			"To version must be specified");
	}

	ApplicationVersion *avto = to ? m_app->getVersion(to) : m_tgtAppVer;
	if(!avto) {
		throw RuntimeError(m_parent, ctx.stack(),
			"'%s' is not a valid application version", (const char*) to);
	}
	*/
	ApplicationVersion *avto = NULL;
	ExprPtr eto = m_parent.getArg("to", ctx);
	if(eto) {
		
		if(eto->kind() == KIND_OBJECT) {
			ObjectReference *oto = eto->toObjectReference();
			if(oto) {
				Object *obj = oto->toObject();
				if(!obj) {
					throw RuntimeError(m_parent, ctx.stack(),
						"value for \"to\" is not a valid application version");
				}
				if(obj->kind() != OBJ_KIND_APPVERSION) {
					throw RuntimeError(m_parent, ctx.stack(),
						"'%s' is not a valid application version (d)", obj->name());
				}
				avto = (ApplicationVersion*) obj;
			}
		} else {
			ConstCharPtr to = eto->toString();
			avto = m_app->getVersion(to);
			if(!avto) {
				throw RuntimeError(m_parent, ctx.stack(),
					"'%s' is not a valid application version (e)", (const char*)to);
			}
		}
	} else {
		// "to" not specified
		if(!m_tgtAppVer) {	
			throw RuntimeError(m_parent, ctx.stack(),
				"To version must be specified");
		}

		avto = m_tgtAppVer;
	}

	return avto;
}


void AppVerLoopStmtImpl::execute(Context &ctx)
{
	ApplicationVersion *avfrom = calcFrom(ctx);
	ApplicationVersion *avto   = calcTo(ctx);

	debug1("AVLOOP from = %s TO = %s\n",avfrom?avfrom->name():"NULL",avto->name());

	// Make sure that there is a path between "to" and "from"
	// This loop assumes that "to" is later than "from".  If there is no "from"
	// version, then we have included all the versions we need to deploy and
	// found will be set to false.
	List<ApplicationVersion> path;
	bool found = false;
	bool rollback = false;
	bool isSame = false;
	for(ApplicationVersion *av = avto; av; av = av->getPredecessor()) {
		debug1("av=%s\n",av->name());
		path.prepend(av);
		if(av == avfrom) {
			found = true;
			break;
		}
		if (av->getParent()==0) break;	// end of loop (no more predecessors)
	}
	debug1("found=%s\n",found?"true":"false");

	// If there is no "from" then we can only be rolling forward from the first
	// version in the path, so skip looking for rollbacks, even though found is false
	if(avfrom) {
		// No path from "to" to "from", so try "from" to "to" for a rollback
		if(!found) {
			path.clear();

			debug1("going the other way\n");
			for(ApplicationVersion *av = avfrom; av; av = av->getPredecessor()) {
				debug3("av=0x%lx",av);
				debug1("av=%s\n",av->name());
				path.add(av);
				if(av == avto) {
					found = true;
					rollback = true;
					break;
				}
			}
		}

		if(!found) {
			throw RuntimeError(m_parent, ctx.stack(),
				"There is no path between '%s' and '%s'",
				(avfrom ? avfrom->name() : "(null)"),
				(avto ? avto->name() : "(null)"));
		}
		isSame = (avfrom->id() == avto->id());
	} else {
		// This alters the comploop processing to always include the components from the first AV as well
		m_isFirst = false;
	}

	if(m_appVerScope) {
		m_appVerScope->setOpen(false);
	}

	debug1("avloop rollback = %s", (rollback ? "true" : "false"));

	// New execution context with rollup/rollback set
	Context newctx(ctx, rollback);

	Scope *avloop = new Scope(AVLOOP_SCOPE, this);
	newctx.stack().push(avloop);

	ListIterator<ApplicationVersion> iter(path);
	try {
		for(ApplicationVersion *av = iter.first(); av; av = iter.next()) {
			if(av == avto) {
				m_isLast = true;
			}
			newctx.stack().push(av->getVars());
			try {
				debug1("avloop: %d, '%s' m_isLast=%s", av->id(), av->name(),m_isLast?"true":"false");
				avloop->set("dep.same",isSame);
				avloop->set("dep.first",m_isFirst);
				avloop->set("dep.last",m_isLast);
				avloop->set("dep.rollback",rollback);
				avloop->set("dep.rollforward",!rollback);
				m_parent.executeBody(newctx);
			} catch(ContinueException& /*e*/) {
				ctx.dm().writeToLogFile("continue avloop loop");
			} 
			newctx.stack().pop(APPLICATION_SCOPE);
			m_isFirst = false;
		}
	} catch(BreakException &/*ex*/) {
		newctx.stack().pop(APPLICATION_SCOPE);
		if(m_appVerScope) {
			m_appVerScope->setOpen(true);
		}
		ctx.dm().writeToLogFile("break out of avloop loop");
	} catch(...) {
		// If an exception is raised we still need to do the pop
		newctx.stack().dump();
		newctx.stack().pop(APPLICATION_SCOPE);
		newctx.stack().pop(AVLOOP_SCOPE);
		if(m_appVerScope) {
			m_appVerScope->setOpen(true);
		}
		throw;
	}

	newctx.stack().pop(AVLOOP_SCOPE);

	if(m_appVerScope) {
		m_appVerScope->setOpen(true);
	}
}


Expr *AppVerLoopStmtImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "first") == 0) {
		if(!m_cacheFirst) {
			m_cacheFirst = calcFrom(ctx);
		}
		return new Expr(m_cacheFirst);
	} else if(strcmp(name, "last") == 0) {
		if(!m_cacheLast) {
			m_cacheLast = calcTo(ctx);
		}
		return new Expr(m_cacheLast);
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// AppVerLoopStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

AppVerLoopStmtImplFactory::AppVerLoopStmtImplFactory()
{}


ExtendedStmtImpl *AppVerLoopStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new AppVerLoopStmtImpl(parent);
}
