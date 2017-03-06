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

#include "task.h"

#include "audit.h"
#include "context.h"
#include "dm.h"
#include "exceptions.h"
#include "expr.h"
#include "node.h"
#include "scopestack.h"
#include "notifytemplate.h"
#ifdef WIN32
#include <crtdbg.h>
#endif
///////////////////////////////////////////////////////////////////////////////
// Approve
///////////////////////////////////////////////////////////////////////////////

class ApproveTaskImpl : public TaskImpl
{
private:
	Domain &m_tgtDomain;
	bool m_approve;
	char *m_note;

public:
	ApproveTaskImpl(
		class TaskImplFactory &factory, DMINT32 implId, class Task &task,
		class Domain &tgtDomain, bool approve, const char *note);
	~ApproveTaskImpl();

	int run(class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};


class ApproveTaskImplFactory : public TaskImplFactory
{
public:
	ApproveTaskImplFactory();

	TaskImpl *create(
		DMINT32 implId, class Task &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


///////////////////////////////////////////////////////////////////////////////
// Copy
///////////////////////////////////////////////////////////////////////////////

class CopyTaskImpl : public TaskImpl
{
private:
	Domain &m_tgtDomain;
	char *m_note;

public:
	CopyTaskImpl(
		class TaskImplFactory &factory, DMINT32 implId, class Task &task,
		class Domain &tgtDomain, const char *note);
	~CopyTaskImpl();

	int run(class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};


class CopyTaskImplFactory : public TaskImplFactory
{
public:
	CopyTaskImplFactory();

	TaskImpl *create(
		DMINT32 implId, class Task &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


///////////////////////////////////////////////////////////////////////////////
// Move
///////////////////////////////////////////////////////////////////////////////

class MoveTaskImpl : public TaskImpl
{
private:
	Domain &m_tgtDomain;
	char *m_note;

public:
	MoveTaskImpl(
		class TaskImplFactory &factory, DMINT32 implId, class Task &task,
		class Domain &tgtDomain, const char *note);
	~MoveTaskImpl();

	int run(class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};


class MoveTaskImplFactory : public TaskImplFactory
{
public:
	MoveTaskImplFactory();

	TaskImpl *create(
		DMINT32 implId, class Task &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


///////////////////////////////////////////////////////////////////////////////
// Request
///////////////////////////////////////////////////////////////////////////////

class RequestTaskImpl : public TaskImpl
{
private:
	Task &m_linkedTask;
	char *m_note;

public:
	RequestTaskImpl(
		class TaskImplFactory &factory, DMINT32 implId, class Task &task,
		class Task &linkedTask, const char *note);
	~RequestTaskImpl();

	int run(class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};


class RequestTaskImplFactory : public TaskImplFactory
{
public:
	RequestTaskImplFactory();

	TaskImpl *create(
		DMINT32 implId, class Task &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


///////////////////////////////////////////////////////////////////////////////
// CreateVersion
///////////////////////////////////////////////////////////////////////////////

class CreateVersionTaskImpl : public TaskImpl
{
private:
	Domain &m_tgtDomain;
	Application *m_predecessor;
	ApplicationVersion *m_newav;

public:
	CreateVersionTaskImpl(
		class TaskImplFactory &factory, DMINT32 implId, class Task &task,
		class Domain &tgtDomain, Application *predecessor);
	~CreateVersionTaskImpl();

	int run(class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};


class CreateVersionTaskImplFactory : public TaskImplFactory
{
public:
	CreateVersionTaskImplFactory();

	TaskImpl *create(
		DMINT32 implId, class Task &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


///////////////////////////////////////////////////////////////////////////////
// RunAction
///////////////////////////////////////////////////////////////////////////////

class RunActionTaskImpl : public TaskImpl
{
private:

public:
	RunActionTaskImpl(
		class TaskImplFactory &factory, DMINT32 implId, class Task &task);
	~RunActionTaskImpl();

	int run(class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};


class RunActionTaskImplFactory : public TaskImplFactory
{
public:
	RunActionTaskImplFactory();

	TaskImpl *create(
		DMINT32 implId, class Task &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};

///////////////////////////////////////////////////////////////////////////////
// TaskImpl
///////////////////////////////////////////////////////////////////////////////

TaskImpl::TaskImpl(class TaskImplFactory &factory, DMINT32 implId, class Task &task)
	: m_factory(factory), m_implId(implId), m_task(task), m_vars(NULL)
{}


/*virtual*/ TaskImpl::~TaskImpl()
{
	SAFE_DELETE(m_vars);
}


Scope *TaskImpl::getVars()
{
	if(!m_vars) {
		m_vars = new DelegatingScope(*(m_task.getVars()), this);
		m_vars->addRef();
	}
	return m_vars;
}


/*virtual*/ Expr *TaskImpl::getAttribute(
	const char *name, Context &ctx)
{
	return m_task.getAttribute(name, ctx);
}


int TaskImpl::notify(Context &ctx,bool useFailureTemplate /* = false */)
{
	debug3("notify - useFailureTemplate = %s",useFailureTemplate?"true":"false");
	NotifyTemplate *t = useFailureTemplate?
					ctx.dm().getFailureTemplate(ctx,m_task.id()):
					ctx.dm().getSuccessTemplate(ctx,m_task.id());
	debug3("t=0x%lx",t);

	if (t) {

		Notify *n = t->getNotify();
		int nfyid = n?n->id():1;
		const char **tos = t->getRecipients();

		try {
			Notify *notify = t->getNotify();

			DMArray *recipients = ctx.stack().newGlobalArray("RECIPIENTS", true);
			// TODO: change to: AutoPtr<DMArray> recipients = new DMArray(true);
			// AutoPtr<DMArray> recipients = new DMArray(true);

			for(const char **sotid = tos; sotid && *sotid; sotid++) {
				debug1("Recipient=[%s]",*sotid);
				ObjectKindAndId otid(*sotid);
				Object *obj = NULL;
				switch(otid.getObjectKind()) {
				case OBJ_KIND_USER:
					obj = ctx.dm().getUser(otid.getId());
					break;
				case OBJ_KIND_USERGROUP:
					obj = ctx.dm().getUserGroup(otid.getId());
					break;
				default:
					ctx.writeToStdOut("Invalid recipient type: '%s'", sotid);
					continue;
				}
				if(obj) {
					recipients->add(new Variable(NULL, obj));
				}
			}

			if(recipients->count() == 0) {
				ctx.writeToStdOut("No recipients specified");
				return 1;
			}

			StmtList *args = new StmtList();
			// TODO: allow multiple recipients - want to use: new Node((DMArray*) recipients)
			args->add(new Stmt(strdup("to"), new Node((DMArray*) recipients)));
			// args->add(new Stmt(strdup("to"), new Node(NODE_ARRAY, new Node(NODE_NLVAR, new Node(NODE_IDOROBJ, strdup("RECIPIENTS"))), new Node(0))));

			char *subject = strdup(t->getSubject());
			Node expandedText1(NODE_STR, subject, true);	// will free subject on delete
			ExprPtr etext1 = expandedText1.evaluate(ctx);
			ConstCharPtr stext1 = etext1->stringify();
			args->add(new Stmt(strdup("subject"), new Node(NODE_STR,strdup(stext1)))); //expsubject is freed when Node is deleted
			
			ExtendedStmt stmt(strdup("notify"), args);
			Audit &audit = ctx.dm().getDummyAudit();
			AuditEntry *ae = audit.newAuditEntry("notify");
			OutputStream body;
			char *text = strdup(t->getBody());
			Node expandedText2(NODE_STR, text, true);	// will free text on delete
			ExprPtr etext2 = expandedText2.evaluate(ctx);
			ConstCharPtr stext2 = etext2->stringify();
			body.writeToStdOut("%s", (const char*) stext2);
			ae->start();
			notify->notify(nfyid, stmt, ae, body, NULL, ctx);
			ae->finish();
			return 0;
		} catch(DMException &e) {
			fprintf(stderr, "Exception: %s\n", e.getMessage());
			return 1;
		} catch(...) {
			fprintf(stderr, "Unhandled exception\n");
		}
		return -1;
	}
	// No template
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// TaskImplFactory
///////////////////////////////////////////////////////////////////////////////

TaskImplFactory::TaskImplFactory()
{}


/*virtual*/ TaskImplFactory::~TaskImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// TaskImplRegistry
///////////////////////////////////////////////////////////////////////////////

TaskImplRegistry *TaskImplRegistry::s_instance = NULL;


TaskImplRegistry::TaskImplRegistry()
	: m_factories(false, true)
{}


/*static*/ TaskImplRegistry &TaskImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new TaskImplRegistry();
	}
	return *s_instance;
}


/*static*/ void TaskImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void TaskImplRegistry::registerFactory(const char *name, TaskImplFactory *factory)
{
	m_factories.put(name, factory);
}


void TaskImplRegistry::registerBuiltIns()
{
	registerFactory("approve",			new ApproveTaskImplFactory());
	registerFactory("copy",				new CopyTaskImplFactory());
	registerFactory("move",				new MoveTaskImplFactory());
	registerFactory("request",			new RequestTaskImplFactory());
	registerFactory("createversion",	new CreateVersionTaskImplFactory());
	registerFactory("runaction",		new RunActionTaskImplFactory());
}


TaskImplFactory *TaskImplRegistry::getFactory(const char *name)
{
	return m_factories.get(name);
}


///////////////////////////////////////////////////////////////////////////////
// ApproveTaskImpl
///////////////////////////////////////////////////////////////////////////////

ApproveTaskImpl::ApproveTaskImpl(
		TaskImplFactory &factory, DMINT32 implId, Task &task,
		Domain &tgtDomain, bool approve, const char *note
	)
	: TaskImpl(factory, implId, task), m_tgtDomain(tgtDomain),
	  m_approve(approve), m_note(DUP_NULL(note))
{}


ApproveTaskImpl::~ApproveTaskImpl()
{
	SAFE_FREE(m_note);
}


int ApproveTaskImpl::run(ExtendedStmt &stmt, Context &ctx)
{
	debug1("%s for '%s'", m_approve?"APPROVE":"REJECT",m_tgtDomain.name());

	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if(!app) {
		throw RuntimeError(ctx.stack(), "No application");
	}

	ctx.stack().setGlobal("NOTES",m_note);	// Set the notes field for expansion in notification body.
	Domain *domain = app->getDomain();
	if (domain) {
		ctx.stack().setGlobal("SRCDOMAIN",domain->name());
	}
	ctx.stack().setGlobal("TGTDOMAIN",m_tgtDomain.name());

	bool ok = app->approve(m_tgtDomain, m_approve, m_note);

	if(ok) {
		debug3("Calling notify");
		notify(ctx,!m_approve);
		debug3("notify called");
	}

	debug3("Returning from ApproveTaskImpl::run");
	return (ok ? 0 : 1);
}


Expr *ApproveTaskImpl::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "approve") == 0) {
		return new Expr(m_approve);
	} else if(strcmp(name, "status") == 0) {
		return new Expr(m_approve ? "approved" : "rejected");
	} else if(strcmp(name, "note") == 0) {
		return new Expr(m_note);
	} else if(strcmp(name, "target") == 0) {
		return new Expr(m_tgtDomain.toObject());
	}

	return TaskImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// ApproveTaskImplFactory
///////////////////////////////////////////////////////////////////////////////

ApproveTaskImplFactory::ApproveTaskImplFactory()
{}


TaskImpl *ApproveTaskImplFactory::create(
	DMINT32 implId, Task &parent, ExtendedStmt &stmt, Context &ctx)
{
	Domain *tgtDomain = parent.getApprovalDomain();
	if(!tgtDomain) {
		throw RuntimeError("ApproveTask %d '%s' is missing an approval domain",
			parent.id(), parent.name());
	}

	bool approve = stmt.getArgAsBoolean("approve", ctx);
	ConstCharPtr note = stmt.getArgAsString("note", ctx);

	return new ApproveTaskImpl(*this, implId, parent, *tgtDomain, approve, note);
}


///////////////////////////////////////////////////////////////////////////////
// CopyTaskImpl
///////////////////////////////////////////////////////////////////////////////

CopyTaskImpl::CopyTaskImpl(
		TaskImplFactory &factory, DMINT32 implId, Task &task,
		Domain &tgtDomain, const char *note
	)
	: TaskImpl(factory, implId, task), m_tgtDomain(tgtDomain),
	  m_note(DUP_NULL(note))
{}


CopyTaskImpl::~CopyTaskImpl()
{
	SAFE_FREE(m_note);
}


int CopyTaskImpl::run(ExtendedStmt &stmt, Context &ctx)
{
	debug1("COPY to '%s'", m_tgtDomain.name());

	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if(!app) {
		throw RuntimeError(ctx.stack(), "No application");
	}

	// TODO: Implement copying
	//bool ok = app->approve(m_tgtDomain, m_note);
	//
	//return (ok ? 0 : 1);
	return 1;
}


Expr *CopyTaskImpl::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "note") == 0) {
		return new Expr(m_note);
	} else if(strcmp(name, "target") == 0) {
		return new Expr(m_tgtDomain.toObject());
	}

	return TaskImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// CopyTaskImplFactory
///////////////////////////////////////////////////////////////////////////////

CopyTaskImplFactory::CopyTaskImplFactory()
{}


TaskImpl *CopyTaskImplFactory::create(
	DMINT32 implId, Task &parent, ExtendedStmt &stmt, Context &ctx)
{
	Domain *tgtDomain = parent.getTargetDomain();
	if(!tgtDomain) {
		throw RuntimeError("CopyTask %d '%s' is missing a target domain",
			parent.id(), parent.name());
	}

	ConstCharPtr note = stmt.getArgAsString("note", ctx);

	return new CopyTaskImpl(*this, implId, parent, *tgtDomain, note);
}


///////////////////////////////////////////////////////////////////////////////
// MoveTaskImpl
///////////////////////////////////////////////////////////////////////////////

MoveTaskImpl::MoveTaskImpl(
		TaskImplFactory &factory, DMINT32 implId, Task &task,
		Domain &tgtDomain, const char *note
	)
	: TaskImpl(factory, implId, task), m_tgtDomain(tgtDomain),
	  m_note(DUP_NULL(note))
{}


MoveTaskImpl::~MoveTaskImpl()
{
	SAFE_FREE(m_note);
}


int MoveTaskImpl::run(ExtendedStmt &stmt, Context &ctx)
{
	debug1("MOVE to '%s'", m_tgtDomain.name());

	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if(!app) {
		throw RuntimeError(ctx.stack(), "No application");
	}

	ctx.stack().setGlobal("NOTES",m_note);	// Set the notes field for expansion in notification bod
	Domain *srcdomain = app->getDomain();
	if (srcdomain) {
		ctx.stack().setGlobal("SRCDOMAIN",srcdomain->name());
	}
	ctx.stack().setGlobal("TGTDOMAIN",m_tgtDomain.name());

	bool ok = app->move(m_tgtDomain, m_note);
	debug3("Calling notify");
	notify(ctx,!ok);
	debug3("notify called");
	
	return (ok ? 0 : 1);
}


Expr *MoveTaskImpl::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "note") == 0) {
		return new Expr(m_note);
	} else if(strcmp(name, "target") == 0) {
		return new Expr(m_tgtDomain.toObject());
	}

	return TaskImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// MoveTaskImplFactory
///////////////////////////////////////////////////////////////////////////////

MoveTaskImplFactory::MoveTaskImplFactory()
{}


TaskImpl *MoveTaskImplFactory::create(
	DMINT32 implId, Task &parent, ExtendedStmt &stmt, Context &ctx)
{
	Domain *tgtDomain = parent.getTargetDomain();
	if(!tgtDomain) {
		throw RuntimeError("MoveTask %d '%s' is missing a target domain",
			parent.id(), parent.name());
	}

	ConstCharPtr note = stmt.getArgAsString("note", ctx);

	return new MoveTaskImpl(*this, implId, parent, *tgtDomain, note);
}


///////////////////////////////////////////////////////////////////////////////
// RequestTaskImpl
///////////////////////////////////////////////////////////////////////////////

RequestTaskImpl::RequestTaskImpl(
		TaskImplFactory &factory, DMINT32 implId, Task &task,
		Task &linkedTask, const char *note
	)
	: TaskImpl(factory, implId, task), m_linkedTask(linkedTask),
	  m_note(DUP_NULL(note))
{}


RequestTaskImpl::~RequestTaskImpl()
{
	SAFE_FREE(m_note);
}


int RequestTaskImpl::run(ExtendedStmt &stmt, Context &ctx)
{
	debug1("REQUEST for '%s'", m_linkedTask.name());

	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if(!app) {
		throw RuntimeError(ctx.stack(), "No application");
	}

	ctx.stack().setGlobal("NOTES",m_note);	// Set the notes field for expansion in notification bod
	
	ctx.stack().setGlobal("TASKNAME",m_linkedTask.name());
	
	bool ok = app->request(m_linkedTask,m_note);
	
	if (ok) {
		debug3("Calling notify");
		notify(ctx,false);
		debug3("notify called");
	}

	//
	return (ok ? 0 : 1);
	// return 1;
}


Expr *RequestTaskImpl::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "note") == 0) {
		return new Expr(m_note);
	} else if(strcmp(name, "linked") == 0) {
		return new Expr(m_linkedTask.toObject());
	}

	return TaskImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// RequestTaskImplFactory
///////////////////////////////////////////////////////////////////////////////

RequestTaskImplFactory::RequestTaskImplFactory()
{}


TaskImpl *RequestTaskImplFactory::create(
	DMINT32 implId, Task &parent, ExtendedStmt &stmt, Context &ctx)
{
	Task *linkedTask = parent.getLinkedTask();
	if(!linkedTask) {
		throw RuntimeError("RequestTask %d '%s' is missing a linked task",
			parent.id(), parent.name());
	}

	ConstCharPtr note = stmt.getArgAsString("note", ctx);

	return new RequestTaskImpl(*this, implId, parent, *linkedTask, note);
}


///////////////////////////////////////////////////////////////////////////////
// CreateVersionTaskImpl
///////////////////////////////////////////////////////////////////////////////

CreateVersionTaskImpl::CreateVersionTaskImpl(
		TaskImplFactory &factory, DMINT32 implId, Task &task,
		Domain &tgtDomain, Application *predecessor
	)
	: TaskImpl(factory, implId, task), m_tgtDomain(tgtDomain),
	  m_predecessor(predecessor), m_newav(NULL)
{}


CreateVersionTaskImpl::~CreateVersionTaskImpl()
{
	m_predecessor = NULL;	// Don't own
}


int CreateVersionTaskImpl::run(ExtendedStmt &stmt, Context &ctx)
{
	debug1("CREATE VERSION in '%s'", m_tgtDomain.name());

	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if(!app) {
		throw RuntimeError(ctx.stack(), "No application");
	}

	if(m_predecessor) {
		debug1("m_predecessor is %d '%s'", m_predecessor->id(), m_predecessor->name());
	} else {
		debug1("m_predecessor is null");
	}
	m_newav = app->newVersion(m_tgtDomain, m_predecessor);
	
	return (m_newav ? 0 : 1);
}


Expr *CreateVersionTaskImpl::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "target") == 0) {
		return new Expr(m_tgtDomain.toObject());
	} else if(strcmp(name, "predecessor") == 0) {
		return new Expr(m_predecessor);	// NULL is null objref
	} else if(strcmp(name, "newav") == 0) {
		return new Expr(m_newav);	// NULL is null objref
	}

	return TaskImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// CreateVersionTaskImplFactory
///////////////////////////////////////////////////////////////////////////////

CreateVersionTaskImplFactory::CreateVersionTaskImplFactory()
{}


TaskImpl *CreateVersionTaskImplFactory::create(
	DMINT32 implId, Task &parent, ExtendedStmt &stmt, Context &ctx)
{
	Domain *tgtDomain = parent.getTargetDomain();
	if(!tgtDomain) {
		throw RuntimeError("CreateVersionTask %d '%s' is missing a target domain",
			parent.id(), parent.name());
	}

	Application *predecessor = NULL;
	Expr *epred = stmt.getArg("predecessor", ctx);
	ObjectReference *oref = epred ? epred->toObjectReference() : NULL;
	Object *obj = oref ? oref->toObject() : NULL;
	if(obj) {
		if(obj->kind() != OBJ_KIND_APPVERSION && obj->kind() != OBJ_KIND_APPLICATION) {
			throw RuntimeError("CreateVersionTask %d '%s' has invalid predecessor",
				parent.id(), parent.name());
		}
		predecessor = (Application*) obj;
	}

	return new CreateVersionTaskImpl(*this, implId, parent, *tgtDomain, predecessor);
}


/*****/

///////////////////////////////////////////////////////////////////////////////
// RunActionTaskImpl
///////////////////////////////////////////////////////////////////////////////

RunActionTaskImpl::RunActionTaskImpl(
		TaskImplFactory &factory, DMINT32 implId, Task &task
	)
	: TaskImpl(factory, implId, task)
{}


RunActionTaskImpl::~RunActionTaskImpl()
{
}


int RunActionTaskImpl::run(ExtendedStmt &stmt, Context &ctx)
{
	Action *action = ctx.dm().getActionTask(m_task);
	try {
		ExtendedStmt stmt(action);
		stmt.execute(ctx);
	} catch(ReturnException &/*e*/) {
		// Normal return via return statement
	}

	return 0;

}


Expr *RunActionTaskImpl::getAttribute(const char *name, class Context &ctx)
{
	/*
	if(strcmp(name, "target") == 0) {
		return new Expr(m_tgtDomain.toObject());
	} else if(strcmp(name, "predecessor") == 0) {
		return new Expr(m_predecessor);	// NULL is null objref
	} else if(strcmp(name, "newav") == 0) {
		return new Expr(m_newav);	// NULL is null objref
	}
	*/

	return TaskImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// RunActionTaskImplFactory
///////////////////////////////////////////////////////////////////////////////

RunActionTaskImplFactory::RunActionTaskImplFactory()
{}


TaskImpl *RunActionTaskImplFactory::create(
	DMINT32 implId, Task &parent, ExtendedStmt &stmt, Context &ctx)
{
	/*
	Domain *tgtDomain = parent.getTargetDomain();
	if(!tgtDomain) {
		throw RuntimeError("CreateVersionTask %d '%s' is missing a target domain",
			parent.id(), parent.name());
	}

	Application *predecessor = NULL;
	Expr *epred = stmt.getArg("predecessor", ctx);
	ObjectReference *oref = epred ? epred->toObjectReference() : NULL;
	Object *obj = oref ? oref->toObject() : NULL;
	if(obj) {
		if(obj->kind() != OBJ_KIND_APPVERSION) {
			throw RuntimeError("CreateVersionTask %d '%s' has invalid predecessor",
				parent.id(), parent.name());
		}
		predecessor = (Application*) obj;
	}
	*/

	return new RunActionTaskImpl(*this, implId, parent);
}