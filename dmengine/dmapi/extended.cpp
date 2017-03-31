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
#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#else
#include <pthread.h>
#include <unistd.h>
#define _stricmp strcasecmp
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif /*WIN32*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "extended.h"
#include "context.h"
#include "dm.h"
#include "expr.h"
#include "charptr.h"
#include "node.h"
#include "thread.h"
#include "exceptions.h"
#include "scopestack.h"
#include "model.h"
#include "dropzone.h"
#include "transfer.h"
#include "autoptr.h"
#include "cmdline.h"
#include "execute.h"
//#include "notify.h"
#include "envp.h"
#include "modify.h"
#include "properties.h"
#include "audit.h"
//#include "engineconfig.h"
#include "task.h"


// Just so we can register the impls
#include "../extended/alter/alter.h"
#include "../extended/appverloop/appverloop.h"
#include "../extended/deploy/deploy.h"
#include "../extended/notify/notifystmt.h"
#include "../extended/remotescript/remotescript.h"


///////////////////////////////////////
// Checkin
///////////////////////////////////////


class CheckinStmtImpl : public virtual AuditableExtendedStmtImpl, public virtual IObject
{
private:
	ExtendedStmt &m_parent;

public:
	CheckinStmtImpl(DM &dm, ExtendedStmt &parent);

	void executeWithAudit(class Context &ctx);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//Object *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class CheckinStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	CheckinStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return true; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Trilogy
///////////////////////////////////////

class TrilogyStmtImpl : public virtual AuditableExtendedStmtImpl, public virtual IObject
{
private:
	ExtendedStmt &m_parent;

public:
	TrilogyStmtImpl(DM &dm, ExtendedStmt &parent);

	void executeWithAudit(class Context &ctx);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//Object *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class TrilogyStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	TrilogyStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return true; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Test
///////////////////////////////////////

class TestStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	TestStmtImpl(ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class TestStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	TestStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Assert
///////////////////////////////////////

class AssertStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	AssertStmtImpl(ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class AssertStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	AssertStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Dump
///////////////////////////////////////

class DumpStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	DumpStmtImpl(ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class DumpStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	DumpStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Stacktrace
///////////////////////////////////////

class StacktraceStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	StacktraceStmtImpl(ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class StacktraceStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	StacktraceStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Abort
///////////////////////////////////////

class AbortStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	AbortStmtImpl(ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class AbortStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	AbortStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Query
///////////////////////////////////////

class QueryStmtImpl : public virtual AuditableExtendedStmtImpl, public IObject
{
private:
	ExtendedStmt &m_parent;

public:
	QueryStmtImpl(DM &dm, ExtendedStmt &parent);
	~QueryStmtImpl();

	void executeWithAudit(class Context &ctx);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//Object *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class QueryStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	QueryStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Modify
///////////////////////////////////////

class ModifyStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	ModifyStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Rename
///////////////////////////////////////

class RenameStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

	void doRename(IDropzone &indz, Node &nfrom, IDropzone &outdz, Node &nto, Context &ctx);

public:
	RenameStmtImpl(ExtendedStmt &parent);
	~RenameStmtImpl();

	void execute(class Context &ctx);
};


class RenameStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	RenameStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Delete
///////////////////////////////////////

class DeleteStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	DeleteStmtImpl(ExtendedStmt &parent);
	~DeleteStmtImpl();

	void execute(class Context &ctx);
};


class DeleteStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	DeleteStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};

///////////////////////////////////////
// Create
///////////////////////////////////////

class CreateStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	CreateStmtImpl(ExtendedStmt &parent);
	~CreateStmtImpl();

	void execute(class Context &ctx);
};


class CreateStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	CreateStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};

///////////////////////////////////////
// Read
///////////////////////////////////////

class ReadStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	ReadStmtImpl(ExtendedStmt &parent);
	~ReadStmtImpl();

	void execute(class Context &ctx);
};


class ReadStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	ReadStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// Sleep
///////////////////////////////////////

class SleepStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	SleepStmtImpl(ExtendedStmt &parent);
	~SleepStmtImpl();

	void execute(class Context &ctx);
};


class SleepStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	SleepStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// CompLoop
///////////////////////////////////////

class CompLoopStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	CompLoopStmtImpl(ExtendedStmt &parent);
	~CompLoopStmtImpl();

	void execute(class Context &ctx);
};


class CompLoopStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	CompLoopStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////
// RunTask
///////////////////////////////////////

class RunTaskStmtImpl : public AuditableExtendedStmtImpl //, public IObject
{
private:
	ExtendedStmt &m_parent;

public:
	RunTaskStmtImpl(DM &dm, ExtendedStmt &parent);
	~RunTaskStmtImpl();

	void executeWithAudit(class Context &ctx);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class RunTaskStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	RunTaskStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};

///////////////////////////////////////
// WaitFor
///////////////////////////////////////

class WaitForStmtImpl : public AuditableExtendedStmtImpl //, public IObject
{
private:
	ExtendedStmt &m_parent;

public:
	WaitForStmtImpl(DM &dm, ExtendedStmt &parent);
	~WaitForStmtImpl();

	void executeWithAudit(class Context &ctx);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class WaitForStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	WaitForStmtImplFactory();
	
	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }

	ExtendedStmtImpl *create(DM &dm, ExtendedStmt &parent);
};


///////////////////////////////////////////////////////////////////////////////
// ExtendedStmtImpl
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ ExtendedStmtImpl::~ExtendedStmtImpl()
{}


/*virtual*/ void ExtendedStmtImpl::execute(class DMThreadList &threads, class Context &ctx)
{
	execute(ctx);
}


///////////////////////////////////////////////////////////////////////////////
// ExtendedStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ ExtendedStmtImplFactory::~ExtendedStmtImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// ExtendedStmtImplRegistry
///////////////////////////////////////////////////////////////////////////////

ExtendedStmtImplRegistry *ExtendedStmtImplRegistry::s_instance = NULL;


ExtendedStmtImplRegistry::ExtendedStmtImplRegistry()
	: m_factories(false, true)
{}


/*static*/ ExtendedStmtImplRegistry &ExtendedStmtImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new ExtendedStmtImplRegistry();
	}
	return *s_instance;
}


/*static*/ void ExtendedStmtImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void ExtendedStmtImplRegistry::registerFactory(const char *name, ExtendedStmtImplFactory *factory)
{
	m_factories.put(name, factory);
}


ExtendedStmtImplFactory *ExtendedStmtImplRegistry::getFactory(const char *name)
{
	return m_factories.get(name);
}


void ExtendedStmtImplRegistry::registerBuiltIns()
{
	registerFactory("deploy",       new DeployStmtImplFactory(DEPLOY_KIND_CHECKOUT_AND_TRANSFER));
	registerFactory("checkout",     new DeployStmtImplFactory(DEPLOY_KIND_CHECKOUT_ONLY));
	registerFactory("transfer",     new DeployStmtImplFactory(DEPLOY_KIND_TRANSFER_ONLY));
	registerFactory("checkin",      new CheckinStmtImplFactory());
	registerFactory("trilogy",      new TrilogyStmtImplFactory());
	//registerFactory("trinotify",  new TrilogyStmtImplFactory());
	registerFactory("remotescript", new RemoteScriptStmtImplFactory());
	registerFactory("test",         new TestStmtImplFactory());
	registerFactory("assert",       new AssertStmtImplFactory());
	registerFactory("dump",         new DumpStmtImplFactory());
	registerFactory("stacktrace",   new StacktraceStmtImplFactory());
	registerFactory("abort",        new AbortStmtImplFactory());
	registerFactory("notify",       new NotifyStmtImplFactory());
	registerFactory("attachment",   new AttachmentStmtImplFactory());
	registerFactory("query",        new QueryStmtImplFactory());
	registerFactory("modify",       new ModifyStmtImplFactory());
	registerFactory("rename",       new RenameStmtImplFactory());
	registerFactory("delete",       new DeleteStmtImplFactory());
	registerFactory("create",		new CreateStmtImplFactory());
	registerFactory("read",			new ReadStmtImplFactory());
	registerFactory("sleep",        new SleepStmtImplFactory());
	registerFactory("avloop",       new AppVerLoopStmtImplFactory());
	registerFactory("comploop",     new CompLoopStmtImplFactory());
	registerFactory("runtask",		new RunTaskStmtImplFactory());
	registerFactory("alter",		new AlterStmtImplFactory());
	registerFactory("setatt",		new AlterStmtImplFactory());	// alternative name for alter
	registerFactory("waitfor",		new WaitForStmtImplFactory());
}


///////////////////////////////////////////////////////////////////////////////
// AuditableExtendedStmtImpl
///////////////////////////////////////////////////////////////////////////////

AuditableExtendedStmtImpl::AuditableExtendedStmtImpl(DM &dm, const char *type)
	: /*m_depMajor(0),*/ m_type(DUP_NULL(type))
{
	m_auditEntry = dm.getAudit().newAuditEntry(m_type);
	//m_depMajor = m_auditEntry->stepId();
}


AuditableExtendedStmtImpl::~AuditableExtendedStmtImpl()
{
	SAFE_FREE(m_type);
	SAFE_DELETE(m_auditEntry);
}


void AuditableExtendedStmtImpl::execute(class Context &ctx)
{
	m_auditEntry->start();
	try {
		executeWithAudit(ctx);
	} catch(...) {
		m_auditEntry->finish();
		SAFE_DELETE(m_auditEntry);
		throw;
	}
	m_auditEntry->finish();
}


void AuditableExtendedStmtImpl::execute(class DMThreadList &threads, class Context &ctx)
{
	m_auditEntry->start();
	try {
		executeWithAudit(threads, ctx);
	} catch(...) {
		m_auditEntry->finish();
		SAFE_DELETE(m_auditEntry);
		throw;
	}
	m_auditEntry->finish();
}


/*virtual*/ void AuditableExtendedStmtImpl::executeWithAudit(class DMThreadList &threads, class Context &ctx)
{
	executeWithAudit(ctx);
}


///////////////////////////////////////////////////////////////////////////////
// CheckinStmtImpl
///////////////////////////////////////////////////////////////////////////////

CheckinStmtImpl::CheckinStmtImpl(DM &dm, ExtendedStmt &parent)
	: AuditableExtendedStmtImpl(dm, parent.ident()), m_parent(parent)
{}


void CheckinStmtImpl::executeWithAudit(class Context &ctx)
{
	bool popdz = false;
	Dropzone &dz = m_parent.pushDropzone(popdz, ctx);

	Repository *repo = NULL;
	ConstCharPtr repository = m_parent.getArgAsString("repository", ctx);
	if(!repository) {
		throw RuntimeError(m_parent, ctx.stack(),
			"No repository specified");
	}
	if(!(repo = ctx.dm().getRepository(repository))) {
		throw RuntimeError(m_parent, ctx.stack(),
			"Repository '%s' not found", (const char*) repository);
	}

	// If target is specified as well, then this becomes a subfolder of the dropzone
	ConstCharPtr source = m_parent.getArgAsString("source", ctx);

	DropzoneCallback summary(dz);

	ListIterator<Server> iter(ctx.servers());
	for(Server *s = iter.first(); s; s = iter.next()) {
		// push the server onto the copied stack (if we are not in a psloop)
		bool popsvr = !ctx.stack().contains(SERVER_SCOPE);
		if(popsvr) {
			ctx.stack().push(s->getVars());
		}

		ServerDropzone *sdz = dz.getServerDropzone(*s);
		if(!sdz) {
			printf("WARNING: No server dropzone for server '%s' - nothing checked in\n", s->name());
			continue;
		}

		const char *dzbase = sdz->pathname();
		const char *dzpath = NULL;
		CharPtr tempdz;
		if(source) {
			// Append target path to dropzone path
			tempdz = (char*) malloc(strlen(dzbase) + strlen(source) + 2);
			sprintf(tempdz, "%s%s%s", dzbase, DIR_SEP_STR, (const char*) source);
			dzpath = tempdz;
		} else {
			dzpath = dzbase;
		}

		if(dzbase) {
			try {
				DropzoneCallback callback(repo, (s->id() *1000 + m_auditEntry->stepId()), *sdz, ctx);
				repo->checkin(dzbase, dzpath, m_parent, m_auditEntry, callback, ctx);
				summary.checkin_summary(callback.total(), callback.success(), callback.failed(), callback.notProcessed());
			} catch(...) {
				if(popsvr) { ctx.stack().pop(SERVER_SCOPE); }
				if(popdz)  { ctx.stack().pop(DROPZONE_SCOPE); }
				throw;
			}
		} else {
			ctx.writeToStdOut("WARNING: No server-specific dropzone for server '%s' to check-in", s->name());
		}

		if(popsvr) {
			ctx.stack().pop(SERVER_SCOPE);
		}
	}

	if(popdz) {
		ctx.stack().pop(DROPZONE_SCOPE);
	}
}

Expr *CheckinStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	//// TODO: Free the string returned by toString()
	//ExprPtr expr = m_parent.getArg(name, ctx);
	//return expr ? expr->toString() : NULL;
	return m_parent.getArg(name, ctx);
}

//class DMArray *CheckinStmtImpl::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}

//Object *CheckinStmtImpl::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// CheckinStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

CheckinStmtImplFactory::CheckinStmtImplFactory()
{}


ExtendedStmtImpl *CheckinStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new CheckinStmtImpl(dm, parent);
}


///////////////////////////////////////////////////////////////////////////////
// TrilogyStmtImpl
///////////////////////////////////////////////////////////////////////////////

TrilogyStmtImpl::TrilogyStmtImpl(DM &dm, ExtendedStmt &parent)
	: AuditableExtendedStmtImpl(dm, parent.ident()), m_parent(parent)
{}


void TrilogyStmtImpl::executeWithAudit(Context &ctx)
{
	ConstCharPtr jobname = m_parent.getArgAsString("jobname", ctx);
	if(!jobname) {
		throw RuntimeError(m_parent, ctx.stack(),
			"trilogy requires a jobname");
	}

	ConstCharPtr broker = m_parent.getArgAsString("broker", ctx);

	Variable *TRILOGYHOME = ctx.stack().get("TRILOGYHOME");
	ConstCharPtr TRILOGYHOME2 = (TRILOGYHOME ? TRILOGYHOME->toString() : NULL);

	char *trilogy = (char*) malloc((TRILOGYHOME2 ? strlen(TRILOGYHOME2) : 0) + 15);
	if(TRILOGYHOME) {
#ifdef WIN32
		sprintf(trilogy, "%s/trilogy", TRILOGYHOME2);
#else
		sprintf(trilogy, "%s/bin/trilogy", (const char *)TRILOGYHOME2);
#endif /*WIN32*/
	} else {
		sprintf(trilogy, "trilogy");
	}

	CmdLine cmd(trilogy);
#ifdef WIN32
	cmd.add("-nd");
#else
	cmd.add("-nx");
#endif /*WIN32*/
	if(broker) {
		cmd.add("-b").add(broker);
	}
	cmd.add(jobname);

	// Get the script parameters - this is done here so that we can use the server
	// scope in the thread context, but we run it when we are doing the setup
	ExprPtr params = m_parent.getArg("params", ctx);
	if(params) {
		// Must be an array
		if(params->kind() != KIND_ARRAY) {
			throw RuntimeError(m_parent, ctx.stack(),
				"params must be an list");
		}
		// Find lowest and highest indices
		int minindex = 1000;
		int maxindex = -1;
		AutoPtr<ExprList> keys = params->array_keys();
		ListIterator<Expr> iter(*keys);
		for(Expr *e = iter.first(); e; e = iter.next()) {
			int index = e->toInt();
			if(index > maxindex) { maxindex = index; }
			if(index < minindex) { minindex = index; }
		}
		ExprPtr ecount = params->array_length();
		int count = ecount->toInt();
		debug3("minindex = %d; maxindex = %d; count = %d", minindex, maxindex, count);
		if((minindex != 0) || (maxindex != (count-1))) {
			throw RuntimeError(m_parent, ctx.stack(),
					"params must be a list with indices from 0 to %d", count-1);
		}
		for(int n = minindex; n <= maxindex; n++) {
			Expr temp(n);
			ExprPtr e = params->array_access(temp);
			if(!e || (e->kind() == KIND_ERROR)) {
				throw RuntimeError(m_parent, ctx.stack(),
						"could not evaluate trilogy parameter %d", n);
			}
			ConstCharPtr es = e->toString();
			cmd.add(es);
		}
	}

	bool bShowOutput = true;

	// push a new scope for the trilogy action - TODO: auto ptr pop and delete
	ctx.stack().push(new Scope(TRILOGY_SCOPE, this));

	// TODO: Change to CmdLine::exec()
	CapturedData* cd = NULL;
	int tempExitStatus;
	int ret = executeAndCapture(ctx.stream(), cmd, NULL, bShowOutput, ctx.threadId(), &tempExitStatus, &cd, NULL);
	if(ret) {
		ctx.dm().writeToStdOut("trilogy job failed to execute");
	}
	SAFE_DELETE(cd);

	ctx.stack().pop(TRILOGY_SCOPE);
}


Expr *TrilogyStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	//// TODO: Free the string returned by toString()
	//ExprPtr expr = m_parent.getArg(name, ctx);
	//return expr ? expr->toString() : NULL;
	return m_parent.getArg(name, ctx);
}


//class DMArray *TrilogyStmtImpl::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


//Object *TrilogyStmtImpl::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// TrilogyStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

TrilogyStmtImplFactory::TrilogyStmtImplFactory()
{}


ExtendedStmtImpl *TrilogyStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new TrilogyStmtImpl(dm, parent);
}


///////////////////////////////////////////////////////////////////////////////
// TestStmtImpl
///////////////////////////////////////////////////////////////////////////////

TestStmtImpl::TestStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


/**
 * test(name: 'My test')
 */
void TestStmtImpl::execute(Context &ctx)
{
	ConstCharPtr name = m_parent.getArgAsString("name", ctx);

	bool result = true;
	ctx.stack().push(new Scope(TEST_SCOPE, NULL, m_parent));

	try {
		m_parent.executeBody(ctx);
	} catch(AssertionFailure &e) {
		e.print(ctx);
		result = false;
	} catch(...) {
		ctx.stack().pop(TEST_SCOPE);
		throw;
	}

	ctx.stack().pop(TEST_SCOPE);

	if(result) {
		ctx.writeToStdOut("** Test %s passed", (name ? (const char*) name : "(null)"));
	} else {
		ctx.writeToStdErr("** Test %s failed", (name ? (const char*) name : "(null)"));
	}

	ctx.stack().incrementAll("test_total", 1);
	if(!result) {
		ctx.stack().incrementAll("test_failures", 1);
	}
}


///////////////////////////////////////////////////////////////////////////////
// TestStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

TestStmtImplFactory::TestStmtImplFactory()
{}


ExtendedStmtImpl *TestStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new TestStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// AssertStmtImpl
///////////////////////////////////////////////////////////////////////////////

AssertStmtImpl::AssertStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


/**
 * assert(expr1: exp, expr2: exp)
 * assert(str1: str, str2: str)
 */
void AssertStmtImpl::execute(Context &ctx)
{
	ExprPtr expr  = m_parent.getArg("expr", ctx);
	ExprPtr expr1 = m_parent.getArg("expr1", ctx);
	ExprPtr expr2 = m_parent.getArg("expr2", ctx);
	ExprPtr str1  = m_parent.getArg("str1", ctx);
	ExprPtr str2  = m_parent.getArg("str2", ctx);
	ExprPtr obj1  = m_parent.getArg("obj1", ctx);
	ExprPtr obj2  = m_parent.getArg("obj2", ctx);
	if(expr1 && !expr2) { throw SyntaxError(m_parent, ctx.stack(), "Missing argument expr2"); }
	if(expr2 && !expr1) { throw SyntaxError(m_parent, ctx.stack(), "Missing argument expr1"); }
	if(str1 && !str2)   { throw SyntaxError(m_parent, ctx.stack(), "Missing argument str2"); }
	if(str2 && !str1)   { throw SyntaxError(m_parent, ctx.stack(), "Missing argument str1"); }
	if(obj1 && !obj2)   { throw SyntaxError(m_parent, ctx.stack(), "Missing argument obj2"); }
	if(obj2 && !obj1)   { throw SyntaxError(m_parent, ctx.stack(), "Missing argument obj1"); }
	if((expr && expr1) || (expr && str1) || (expr && obj1)
		|| (expr1 && str1) || (expr1 && obj1) || (str1 && obj1)) {
		throw SyntaxError(m_parent, ctx.stack(), "Invalid combination of arguments");
	}

	bool result = false;
	if(expr) {
		result = expr->toInt() ? true : false;
		if(!result) {
			throw AssertionFailure(m_parent, ctx.stack(), "false");
		}
	} else if(expr1) {
		result = (expr1->kind() != KIND_ERROR) && (expr2->kind() != KIND_ERROR) && (expr1->toInt() == expr2->toInt());
		if(!result) {
			if(expr1->kind() == KIND_ERROR) {
				throw AssertionFailure(m_parent, ctx.stack(),
					"error in calculating value 1: %s", NULL_CHECK(expr1->error()));
			}
			if(expr2->kind() == KIND_ERROR) {
				throw AssertionFailure(m_parent, ctx.stack(),
					"error in calculating value 2: %s", NULL_CHECK(expr2->error()));
			}
			throw AssertionFailure(m_parent, ctx.stack(),
				"value was %d expecting %d", expr1->toInt(), expr2->toInt());
		}
	} else if(str1) {
		ConstCharPtr s1 = str1->stringify(), s2 = str2->stringify();
		result = (strcmp(s1, s2) == 0);
		if(!result) {
			throw AssertionFailure(m_parent, ctx.stack(),
				"value was '%s' expecting '%s'", (const char*) s1, (const char*) s2);
		}
	} else if(obj1) {
		ObjectReference *or1 = obj1->toObjectReference();
		ObjectReference *or2 = obj2->toObjectReference();
		result = (or1 == or2);
		if(!result && or1 && or2) {
			result = (or1->compare(*or2) == 0);
		}
		if(!result) {
			Object *o1 = or1 ? or1->toObject() : NULL;
			Object *o2 = or2 ? or2->toObject() : NULL;
			if(!or1) {
				if(o2) {
					throw AssertionFailure(m_parent, ctx.stack(),
						"value was null expecting %d %d", o2->kind(), o2->id());
				}
			}
			if(!or2) {
				if(o1) {
					throw AssertionFailure(m_parent, ctx.stack(),
						"value was %d %d expecting null", o1->kind(), o1->id());
				}
			}
			if(o1 && o2) {
				throw AssertionFailure(m_parent, ctx.stack(),
					"value was %d %d expecting %d %d", o1->kind(), o1->id(), o2->kind(), o2->id());
			}
			throw AssertionFailure(m_parent, ctx.stack(),
				"object reference do not match");
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// AssertStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

AssertStmtImplFactory::AssertStmtImplFactory()
{}


ExtendedStmtImpl *AssertStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new AssertStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// DumpStmtImpl
///////////////////////////////////////////////////////////////////////////////

DumpStmtImpl::DumpStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void DumpStmtImpl::execute(Context &ctx)
{
	ExprPtr what = m_parent.getArg("what", ctx);
	if(what) {
		if(what->kind() == KIND_OBJECT) {
			ObjectReference *oref = what->toObjectReference();
			IObject *obj = oref ? oref->toIObject() : NULL;
			if(obj && (obj->kind() == OBJ_KIND_DROPZONE)) {
				((Dropzone*) obj)->dump();
			} else {
				throw RuntimeError(m_parent, ctx.stack(),
					"Unrecognised \"what\" object - don't know how to dump");
			}
		} else {
			throw RuntimeError(m_parent, ctx.stack(),
				"Unrecognised \"what\" type - don't know how to dump");
		}
	} else {
		ctx.stack().dump();

		if(ctx.dm().getTargetEnvironment()) {
			ctx.dm().getTargetEnvironment()->print(0);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// DumpStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

DumpStmtImplFactory::DumpStmtImplFactory()
{}


ExtendedStmtImpl *DumpStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new DumpStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// StacktraceStmtImpl
///////////////////////////////////////////////////////////////////////////////

StacktraceStmtImpl::StacktraceStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void StacktraceStmtImpl::execute(Context &ctx)
{
	ctx.stack().trace(ctx, m_parent);
}


///////////////////////////////////////////////////////////////////////////////
// StacktraceStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

StacktraceStmtImplFactory::StacktraceStmtImplFactory()
{}


ExtendedStmtImpl *StacktraceStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new StacktraceStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// AbortStmtImpl
///////////////////////////////////////////////////////////////////////////////

AbortStmtImpl::AbortStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void AbortStmtImpl::execute(Context &ctx)
{
	// TODO: Get the stream and exitCode from params
	ConstCharPtr msg = m_parent.getArgAsString("msg", ctx);
	throw AbortException(m_parent, ctx.stack(),
			(msg ? (const char*) msg : "ABORT"), 0, 0);
}


///////////////////////////////////////////////////////////////////////////////
// AbortStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

AbortStmtImplFactory::AbortStmtImplFactory()
{}


ExtendedStmtImpl *AbortStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new AbortStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// QueryStmtImpl
///////////////////////////////////////////////////////////////////////////////

QueryStmtImpl::QueryStmtImpl(DM &dm, ExtendedStmt &parent)
	: AuditableExtendedStmtImpl(dm, parent.ident()), m_parent(parent)
{}


QueryStmtImpl::~QueryStmtImpl()
{}


void QueryStmtImpl::executeWithAudit(Context &ctx)
{
	ConstCharPtr datasource = m_parent.getArgAsString("datasource", ctx);
	if(!datasource) {
		throw RuntimeError(m_parent, ctx.stack(),
			"No datasource specified");
	}
	Datasource *dsource = ctx.dm().getDatasource(datasource);
	if(!dsource) {
		throw RuntimeError(m_parent, ctx.stack(),
				"Datasource '%s' not found", (const char*) datasource);
	}

	ctx.stack().push(dsource->getVars(m_auditEntry->stepId(), m_parent, m_auditEntry, ctx));
	try {
		dsource->query(m_auditEntry->stepId(), m_parent, m_auditEntry, ctx);
	} catch(...) {
		ctx.stack().pop(DATASOURCE_SCOPE);
		throw;
	}
	ctx.stack().pop(DATASOURCE_SCOPE);
}


Expr *QueryStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	//return NULL;
	return m_parent.getArg(name, ctx);
}


//class DMArray *QueryStmtImpl::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


//Object *QueryStmtImpl::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// QueryStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

QueryStmtImplFactory::QueryStmtImplFactory()
{}


ExtendedStmtImpl *QueryStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new QueryStmtImpl(dm, parent);
}


///////////////////////////////////////////////////////////////////////////////
// ModifyStmtImpl
///////////////////////////////////////////////////////////////////////////////

ModifyStmtImpl::ModifyStmtImpl(ExtendedStmt &parent)
	: m_parent(parent), m_modifier(NULL), m_impl(NULL)
{}


ModifyStmtImpl::~ModifyStmtImpl()
{
	SAFE_FREE(m_modifier);
	SAFE_DELETE(m_impl);
}


void ModifyStmtImpl::process(
	IDropzone &indz, Node &ninfile, IDropzone &outdz,
	Node &noutfile, bool isnew, Context &ctx, Server *server)
{
	ConstCharPtr sinfile = ninfile.evaluateAsString(ctx);
	ConstCharPtr infile = sinfile ? Dropzone::slashify(sinfile) : NULL;
	ConstCharPtr soutfile = noutfile.evaluateAsString(ctx);
	ConstCharPtr outfile = soutfile ? Dropzone::slashify(soutfile) : NULL;

	if(!infile || !outfile) {
		// TODO: Should perhaps raise an exception?
		return;
	}

	bool ServerSpecific = (strcmp(indz.pathname(),outdz.pathname()));

	CharPtr inpath = (char*) malloc(strlen(indz.pathname()) + strlen(infile) + 3);
	sprintf(inpath, "%s%s%s", indz.pathname(), DIR_SEP_STR, (const char*) infile);
	CharPtr outpath = (char*) malloc(strlen(outdz.pathname()) + strlen(outfile) + 3);
	sprintf(outpath, "%s%s%s", outdz.pathname(), DIR_SEP_STR, (const char*) outfile);

	ConstCharPtr outdir = Dropzone::dirname(outfile);
	if(!outdz.dirExists(outdir)) {
		outdz.mkdir(outdir);
	}

	struct stat s;
	if (stat(outpath,&s)==0) {
		// Output file already exists - if server specific take the input from here.
		if (ServerSpecific) {
			infile = strdup(outfile);
			inpath = strdup(outpath);
		}
	}

	// Find the DropzoneFile for the input
	DropzoneFile *indf = indz.getFile(infile);
	if(!indf) {
		debug1("Input DropzoneFile NOT found for '%s'", (const char*) infile);
		indf = new DropzoneFile(indz, NULL, infile, true, false);	// created
		indz.addFile(indf);
	} else {
		debug2("Input DropzoneFile found for '%s'", indf->dzpath());
	}

	debug1("LOCK %s",(const char *)outpath);
	Thread::lock(__LINE__,__FILE__,outpath);
	try {
		m_impl->loadFile(inpath, ctx);
		m_parent.executeBody(ctx);
	} catch (...) {
		Thread::unlock(__LINE__,__FILE__,outpath);
		throw;
	}
	debug1("Saving file %s",(const char *)outpath);
	m_impl->saveFile(outpath, ctx);
	debug1("UNLOCK %s",(const char *)outpath);
	Thread::unlock(__LINE__,__FILE__,outpath);

	// If we have created a new file, rather than changed an existing one
	if(isnew) {
		outdz.addFile(new DropzoneFile(outdz, indf, outfile, true, true));	// created and modified
	} else {
		indf->markAsModified();
	}
	if (strcmp(infile,outfile)) {
		if (server) {
			ctx.dm().writeToStdOut("INFO: Modified File \"%s\" => \"%s\" for Server \"%s\"",(const char *)infile,(const char *)outfile,server->name());
		} else {
			ctx.dm().writeToStdOut("INFO: Modified File \"%s\" => \"%s\"",(const char *)infile,(const char *)outfile);
		}
	} else {
		if (server) {
			ctx.dm().writeToStdOut("INFO: Modified File \"%s\" for Server \"%s\"",(const char *)infile,server->name());
		} else {
			ctx.dm().writeToStdOut("INFO: Modified File \"%s\"",(const char *)infile);
		}
	}
}


void ModifyStmtImpl::executeSubStmt(class ExtendedStmt &stmt, class Context &ctx)
{
	if(!m_impl) {
		throw RuntimeError(stmt, ctx.stack(),
			"\"modify\" has no implementation");
	}

	m_impl->executeSubStmt(stmt, ctx);
}


void ModifyStmtImpl::execute(Context &ctx)
{
	m_modifier = m_parent.getArgAsString("modifier", ctx);
	if(!m_modifier) {
		throw RuntimeError(m_parent, ctx.stack(),
			"No modifier specified");
	}
	Node *ninfile = m_parent.getArgNode("file");
	if(!ninfile) {
		throw RuntimeError(m_parent, ctx.stack(),
			"No file specified");
	}
	Node *noutfile = m_parent.getArgNode("outfile");
	if(!noutfile) {
		noutfile = ninfile;
	}
	bool isServerSpecific = m_parent.getArgAsBoolean("serverspecific", ctx);

	bool popdz = false;
	Dropzone &dz = m_parent.pushDropzone(popdz, ctx);

	ModifyProviderImplFactory *factory = ModifyProviderImplRegistry::instance().getFactory(m_modifier);
	if(!factory) {
		throw RuntimeError(m_parent, ctx.stack(),
				"Modifier '%s' not found", m_modifier);
	}
	m_impl = factory->create(m_parent);

	ctx.stack().push(new Scope(MODIFY_SCOPE, this));

	try {
		if(isServerSpecific) {
			ListIterator<Server> sit(ctx.servers());
			// Ensure we have all dropzones
			for(Server *server = sit.first(); server; server = sit.next()) {
				Thread::lock(__LINE__,__FILE__,server->name());
				if(!dz.exists(*server)) {
					if(!dz.create(*server, true)) {
						throw RuntimeError(m_parent, ctx.stack(), 
							"Unable to create server dropzone '%s' for server '%s'",
							dz.name(), server->name());
					}
				}
				Thread::unlock(__LINE__,__FILE__,server->name());

				ServerDropzone *sdz = dz.getServerDropzone(*server);
				if(!sdz) {
					throw RuntimeError(m_parent, ctx.stack(), 
						"a) Failed to get server dropzone '%s' for server '%s'",
						dz.name(), server->name());
				}
			}

			// Now process the file once for each dropzone
			for(Server *server = sit.first(); server; server = sit.next()) {
				ServerDropzone *sdz = dz.getServerDropzone(*server);
				if(!sdz) {
					throw RuntimeError(m_parent, ctx.stack(), 
						"b) Failed to get server dropzone '%s' for server '%s'",
						dz.name(), server->name());
				}

				ctx.stack().push(server->getVars());
				try {
					// Same logic here as below - use *sdz if the file is in server-specific
					process(dz, *ninfile, *sdz, *noutfile, true, ctx, server);
				} catch(...) {
					ctx.stack().pop(SERVER_SCOPE);
					throw;
				}
				ctx.stack().pop(SERVER_SCOPE);
			}
		} else {
			// Get list of servers which have server specific versions of this file (created by an earlier modify)
			// THEN get dropzone for each server and process dz -> sdz
			process(dz, *ninfile, dz, *noutfile,
				((noutfile != ninfile) ? true : false), ctx, NULL);
		}
	} catch(...) {
		if(popdz) { ctx.stack().pop(DROPZONE_SCOPE); }
		ctx.stack().pop(MODIFY_SCOPE);
		throw;
	}

	if(popdz) {
		ctx.stack().pop(DROPZONE_SCOPE);
	}
	ctx.stack().pop(MODIFY_SCOPE);
}


Expr *ModifyStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	//// TODO: Free the string created by toString()
	//ExprPtr expr = m_parent.getArg(name, ctx);
	//return expr ? expr->toString() : NULL;
	return m_parent.getArg(name, ctx);
}


//class DMArray *ModifyStmtImpl::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


//Object *ModifyStmtImpl::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// ModifyStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

ModifyStmtImplFactory::ModifyStmtImplFactory()
{}


ExtendedStmtImpl *ModifyStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new ModifyStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// RenameStmtImpl
///////////////////////////////////////////////////////////////////////////////

RenameStmtImpl::RenameStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


RenameStmtImpl::~RenameStmtImpl()
{}


void RenameStmtImpl::doRename(IDropzone &indz, Node &nfrom, IDropzone &outdz, Node &nto, Context &ctx)
{
	ConstCharPtr sfrom = nfrom.evaluateAsString(ctx);
	ConstCharPtr from = sfrom ? Dropzone::slashify(sfrom) : NULL;
	ConstCharPtr sto = nto.evaluateAsString(ctx);
	ConstCharPtr to = sto ? Dropzone::slashify(sto) : NULL;

	// TODO: raise exception if from or to not set
	if(from && to) {
		CharPtr absfrom1 = (char*) malloc(strlen(indz.pathname()) + strlen(from) + 3);
		sprintf(absfrom1, "%s%s%s", indz.pathname(), DIR_SEP_STR, (const char*) from);
		CharPtr absfrom2 = (char*) malloc(strlen(outdz.pathname()) + strlen(from) + 3);
		sprintf(absfrom2, "%s%s%s", outdz.pathname(), DIR_SEP_STR, (const char*) from);
		CharPtr absto = (char*) malloc(strlen(outdz.pathname()) + strlen(to) + 3);
		sprintf(absto, "%s%s%s", outdz.pathname(), DIR_SEP_STR, (const char*) to);

		bool ServerSpecific = (strcmp(indz.pathname(),outdz.pathname()));

		int ret=0;
		if (!ServerSpecific) {
			// We're simply renaming in the current dropzone
			ret = rename(absfrom1, absto);
			debug3("rename '%s' to '%s' - returned %d", (const char*) absfrom1, (const char*) absto, ret);
		} else {
			//
			// Server Specific. If the file exists in the server specific dropzone, rename that. Otherwise
			// copy the file from the source dropzone to the server-specific dropzone with the new name.
			//
			struct stat t;
			ret = stat(absfrom2,&t);
			if (ret==0) {
				// File already exists in the target dropzone
				ret = rename(absfrom2,absto);
			} else {
				// File does not currently exist in the target dropzone. Copy the file, creating
				// the new file name. Note, we can't simply use "rename" since we need to leave
				// the original behind in case we have more than one target server.
				int fout = open(absto,FILE_CREATE_PERMISSIONS, FILE_CREATE_MODE);
				int fin = open(absfrom1,FILE_READ_PERMISSIONS);
				if (fout>0 && fin>0) {
					int n=0;
					unsigned char buf[4096];
					while (n=read(fin,buf,sizeof(buf))) {
						write(fout,buf,n);
					}
					close(fin);
					close(fout);
				}
			}
		}
		
		if(ret == 0) {
			DropzoneFile *dzf = indz.getFile(from);
			if(dzf) {
				debug2("rename - found DropzoneFile for '%s'", (const char*) from);
				dzf->rename(to);
			} else {
				debug1("rename - DropzoneFile for '%s' NOT found", (const char*) from);
			}
		} else {
			printf("WARNING: Failed to rename '%s' to '%s' error %d\n", (const char*) from, (const char*) to,errno);
		}
	}
}


void RenameStmtImpl::execute(Context &ctx)
{
	Node* nfrom = m_parent.getArgNode("from");
	Node* nto = m_parent.getArgNode("to");
	if(!nfrom || !nto) {
		throw RuntimeError(m_parent, ctx.stack(),
			"Both from and to must be specified");
	}
	ExprPtr eserverspecific = m_parent.getArg("serverspecific", ctx);
	bool isServerSpecific = eserverspecific ? eserverspecific->toBool() : false;

	bool popdz = false;
	Dropzone &dz = m_parent.pushDropzone(popdz, ctx);

	try {
		if(isServerSpecific) {
			ListIterator<Server> sit(ctx.servers());
			// Ensure we have all dropzones
			for(Server *server = sit.first(); server; server = sit.next()) {
				Thread::lock(__LINE__,__FILE__,server->name());
				if(!dz.exists(*server)) {
					if(!dz.create(*server, true)) {
						throw RuntimeError(m_parent, ctx.stack(), 
							"Unable to create server dropzone '%s' for server '%s'",
							dz.name(), server->name());
					}
				}
				Thread::unlock(__LINE__,__FILE__,server->name());

				ServerDropzone *sdz = dz.getServerDropzone(*server);
				if(!sdz) {
					throw RuntimeError(m_parent, ctx.stack(), 
						"Failed to get server dropzone '%s' for server '%s'",
						dz.name(), server->name());
				}
			}
			// Now process the file once for each dropzone
			for(Server *server = sit.first(); server; server = sit.next()) {
				ServerDropzone *sdz = dz.getServerDropzone(*server);
				if(!sdz) {
					throw RuntimeError(m_parent, ctx.stack(), 
						"Failed to get server dropzone '%s' for server '%s'",
						dz.name(), server->name());
				}

				ctx.stack().push(server->getVars());
				try {
					doRename(dz, *nfrom, *sdz, *nto, ctx);
				} catch(...) {
					ctx.stack().pop(SERVER_SCOPE);
					throw;
				}
				ctx.stack().pop(SERVER_SCOPE);
			}
		} else {
			// Get list of servers which have server specific versions of this file (created by an earlier modify or rename)
			doRename(dz, *nfrom, dz, *nto, ctx);
		}
	} catch(...) {
		if(popdz) { ctx.stack().pop(DROPZONE_SCOPE); }
		throw;
	}

	if(popdz) {
		ctx.stack().pop(DROPZONE_SCOPE);
	}
}


///////////////////////////////////////////////////////////////////////////////
// RenameStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

RenameStmtImplFactory::RenameStmtImplFactory()
{}


ExtendedStmtImpl *RenameStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new RenameStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// DeleteStmtImpl
///////////////////////////////////////////////////////////////////////////////

DeleteStmtImpl::DeleteStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


DeleteStmtImpl::~DeleteStmtImpl()
{}


// in dm.cpp
int clearDirectory(const char *pathname);

void DeleteStmtImpl::execute(Context &ctx)
{
	ConstCharPtr sfilename = NULL;

	Expr *e = m_parent.getArg("file",ctx);
	if (e) {
		if (e->kind() == KIND_OBJECT) {
			// We've been passed an object for "file" - is it a DropZoneFile Object?
			ObjectReference *obj = e->toObjectReference();
			if (!obj || (obj && obj->kind() != OBJ_KIND_DROPZONEFILE)) {
				throw RuntimeError(m_parent,ctx.stack(),"'file' object is not a DropZoneFile");
			}
			// Object passed was a DropZoneFile object - get the path
			DropzoneFile *dzf = (DropzoneFile*)obj->toObject();
			sfilename = strdup(dzf->dzpath());
		}
		else
		if (e->kind() == KIND_STR) {
			sfilename = e->toString();
		}
		else throw RuntimeError(m_parent,ctx.stack(),"'file' parameter must be a string or DropZoneFile object");
	} 

	// ConstCharPtr sfilename = m_parent.getArgAsString("file", ctx);
	ConstCharPtr filename = sfilename ? Dropzone::slashify(sfilename) : NULL;
	ConstCharPtr sdirectory = m_parent.getArgAsString("dir", ctx);
	ConstCharPtr directory = sdirectory ? Dropzone::slashify(sdirectory) : NULL;

	if((!filename && !directory) || (filename && directory)) {
		throw RuntimeError(m_parent, ctx.stack(),"Either file or dir must be specified");
	}

	bool popdz = false;
	Dropzone &dz = m_parent.pushDropzone(popdz, ctx);

	if(filename) {
		CharPtr absfilename = (char*) malloc(strlen(dz.pathname()) + strlen(filename) + 3);
		sprintf(absfilename, "%s%s%s", dz.pathname(), DIR_SEP_STR, (const char*) filename);
		CHMOD(absfilename, FILE_WRITE_PERMISSIONS);
		if(unlink(absfilename) == 0) {
			DropzoneFile *dzf = dz.getFile(filename);
			if(dzf) {
				debug2("delete - found DropzoneFile for '%s'", (const char*) filename);
				dzf->markAsDeleted();
			} else {
				debug1("delete - DropzoneFile NOT found for '%s'", (const char*) filename);
				throw RuntimeError(m_parent,ctx.stack(),"Failed to locate file '%s' in dropzone",(const char*)filename);
			}
		} else {
			throw RuntimeError(m_parent,ctx.stack(),"Failed to delete '%s' from dropzone: Error %d\n", (const char*) filename,errno);
		}
	} else if(directory) {
		CharPtr absdirectory = (char*) malloc(strlen(dz.pathname()) + strlen(directory) + 3);
		sprintf(absdirectory, "%s%s%s", dz.pathname(), DIR_SEP_STR, (const char*) directory);
		clearDirectory(absdirectory);
	}

	if(popdz) {
		ctx.stack().pop(DROPZONE_SCOPE);
	}
}


///////////////////////////////////////////////////////////////////////////////
// DeleteStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

DeleteStmtImplFactory::DeleteStmtImplFactory()
{}


ExtendedStmtImpl *DeleteStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new DeleteStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// CreateStmtImpl
///////////////////////////////////////////////////////////////////////////////

CreateStmtImpl::CreateStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


CreateStmtImpl::~CreateStmtImpl()
{}


void CreateStmtImpl::execute(Context &ctx)
{
	ConstCharPtr sfilename = m_parent.getArgAsString("file", ctx);
	ConstCharPtr filename = sfilename ? Dropzone::slashify(sfilename) : NULL;

	if (!filename) {
		throw RuntimeError(m_parent, ctx.stack(),
			"file must be specified");
	}

	bool popdz = false;
	Dropzone &dz = m_parent.pushDropzone(popdz, ctx);

	CharPtr absfilename = (char*) malloc(strlen(dz.pathname()) + strlen(filename) + 3);
	sprintf(absfilename, "%s%s%s", dz.pathname(), DIR_SEP_STR, (const char*) filename);
	int out = open(absfilename, FILE_CREATE_PERMISSIONS, FILE_CREATE_MODE);
	if (out) {
		// file was created okay
		
		Node* nStream = m_parent.getArgNode("stream");
		if (nStream) {
			// stream: specified
			Expr *e = nStream->evaluate(ctx);
			if (e->kind() != KIND_STREAM) {
				close(out);
				throw RuntimeError(m_parent, ctx.stack(),
				"passed 'stream' parameter is not a stream");
			}
			OutputStream *stream = e->toStream();
			//
			// Write the stream content to the file
			//
			write(out,stream->buffer(),stream->size());
		}
		close(out);
		DropzoneFile *df = new DropzoneFile(dz, NULL, filename, true, false);	// created
		dz.addFile(df);
	}

	if(popdz) {
		ctx.stack().pop(DROPZONE_SCOPE);
	}
}


///////////////////////////////////////////////////////////////////////////////
// CreateStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

CreateStmtImplFactory::CreateStmtImplFactory()
{}


ExtendedStmtImpl *CreateStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new CreateStmtImpl(parent);
}



///////////////////////////////////////////////////////////////////////////////
// ReadStmtImpl
///////////////////////////////////////////////////////////////////////////////

ReadStmtImpl::ReadStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


ReadStmtImpl::~ReadStmtImpl()
{}


void ReadStmtImpl::execute(Context &ctx)
{
	OutputStream *s = NULL;
	ConstCharPtr sfilename = m_parent.getArgAsString("file", ctx);
	ConstCharPtr filename = sfilename ? Dropzone::slashify(sfilename) : NULL;
	Expr *e = m_parent.getArg("stream",ctx);
	if (e) {
		if (e->kind() != KIND_STREAM) throw RuntimeError("'stream' parameter is not a stream");
		s = e->toStream();
	} else {
		// No stream parameter - if we're not in a using stream block, throw exception
		if (!ctx.inUsingStream()) {
			throw RuntimeError("read needs 'stream' parameter or must be in a 'using stream' block");
		}
	}
	if (!filename) {
		throw RuntimeError(m_parent, ctx.stack(),"file must be specified");
	}

	bool popdz = false;
	Dropzone &dz = m_parent.pushDropzone(popdz, ctx);

	CharPtr absfilename = (char*) malloc(strlen(dz.pathname()) + strlen(filename) + 3);
	sprintf(absfilename, "%s%s%s", dz.pathname(), DIR_SEP_STR, (const char*) filename);
	int in = open(absfilename, FILE_READ_PERMISSIONS);
	if (in <= 0) {
		throw RuntimeError(m_parent,ctx.stack(),"File '%s' could not be opened",(const char *)filename);
	}
	// file was opened okay, loop through the file, writing the data to the stream
	int n=0;
	unsigned char buf[1024];
	while (n = read(in,buf,sizeof(buf))) {
		if (n>0) {
			if (s) {
				// Stream parameter specified
				s->writeBufferToStdOut(0,(const char *)buf,n);
			} else {
				// In using stream block
				ctx.writeBufferToStdOut(0,(const char *)buf,n);
			}
		}
		if (n<0) {
			throw RuntimeError(m_parent,ctx.stack(),"Failed reading file '%s'",(const char *)filename);
		}
	}
	close(in);
	if(popdz) {
		ctx.stack().pop(DROPZONE_SCOPE);
	}
}


///////////////////////////////////////////////////////////////////////////////
// ReadStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

ReadStmtImplFactory::ReadStmtImplFactory()
{}


ExtendedStmtImpl *ReadStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new ReadStmtImpl(parent);
}

///////////////////////////////////////////////////////////////////////////////
// SleepStmtImpl
///////////////////////////////////////////////////////////////////////////////

SleepStmtImpl::SleepStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


SleepStmtImpl::~SleepStmtImpl()
{}


void SleepStmtImpl::execute(Context &ctx)
{
	ExprPtr delay = m_parent.getArg("delay", ctx);
	if(!delay) {
		throw RuntimeError(m_parent, ctx.stack(),
			"sleep requires a delay parameter");
	}

#ifdef WIN32
	Sleep(delay->toInt()*1000);
#else
	sleep(delay->toInt());
#endif /*WIN32*/
}


///////////////////////////////////////////////////////////////////////////////
// SleepStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

SleepStmtImplFactory::SleepStmtImplFactory()
{}


ExtendedStmtImpl *SleepStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new SleepStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// CompLoopStmtImpl
///////////////////////////////////////////////////////////////////////////////

CompLoopStmtImpl::CompLoopStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


CompLoopStmtImpl::~CompLoopStmtImpl()
{}


void CompLoopStmtImpl::execute(Context &ctx)
{
	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if(!app) {
		throw RuntimeError(m_parent, ctx.stack(), "No application on stack");
	}

	Scope *avloopScope = ctx.stack().getScope(AVLOOP_SCOPE);
	AppVerLoopStmtImpl *avloop = avloopScope ? (AppVerLoopStmtImpl*) avloopScope->getScopeObject() : NULL;
	bool isFirst = avloop ? avloop->isFirst() : false;
	bool isLast = avloop ? avloop->isLast() : false;

	bool filterlast = m_parent.getArgAsBoolean("filterlast", ctx);
	ExprPtr erollback = m_parent.getArg("rollback", ctx);

	bool rollback = erollback ? erollback->toBool() : ctx.isRollback();
	debug1("comploop rollback = %s", (rollback ? "true" : "false"));

	// New execution context with rollup/rollback set
	Context newctx(ctx, rollback);

	debug1("comploop: avloop=%s; filterlast=%s; rollback=%s; isFirst=%s; isLast=%s",
		(avloop ? "Y" : "N"), (filterlast ? "Y" : "N"), (newctx.isRollback() ? "Y" : "N"),
		(isFirst ? "Y" : "N"), (isLast ? "Y" : "N"));

	List<Component> *components = app->getComponents();
	ListIterator<Component> iter(*components);
	for(Component *comp = iter.first(); comp; comp = iter.next()) {
		bool include = comp->getFilterItems();
		debug1("comploop1: include=%s; comp=(%d,'%s',%d,%d)", (include ? "Y" : "N"), comp->id(), comp->name(), comp->getRollup(), comp->getRollback());
		if(!include) {
			if(!filterlast && isLast) {
				// last item is included provided that doing rollup and not a
				// rollback item, or doing rollback and not a rollup item
				// PAG change == OFF to == ALL since we don't want to apply the last one in the list unless it's ALL
				// PAG mod 2 - make it != ON (ALL or OFF)
				include = ctx.isRollback() ? (comp->getRollback() == ALL || comp->getRollback() == OFF) : (comp->getRollback() == OFF);
				debug1("comploop2: include=%s", include ? "Y" : "N");
			} else {
				if(ctx.isRollback()) {
					// for rollbacks we go from N to T+1 (ON) or N to T (ALL), so test last item differently
					include = (avloop && isLast) ? (comp->getRollback() == ALL) : (comp->getRollback() != OFF);
					debug1("comploop3: include=%s", include ? "Y" : "N");
				} else {
					// for rollups we go from N+1 to T (ON) or N to T (ALL), so test first item differently
					include = (avloop && isFirst) ? (comp->getRollup() == ALL) : (comp->getRollup() != OFF);
					debug1("comploop4: include=%s", include ? "Y" : "N");
				}
			}
		}
		if(include) {
			debug1("include is true: comploop: comp is %d, '%s'", comp->id(), comp->name());
			newctx.stack().push(comp->getVars());
			try {
				// Get List of servers to which this component is allocated
				List<Server> *servers = comp->getServerSubset(ctx);
				Context newctx2(newctx,*servers);
				m_parent.executeBody(newctx2);
			} catch(...) {
				// If an exception is raised we still need to do the pop
				newctx.stack().pop(COMPONENT_SCOPE);
				throw;
			}
			newctx.stack().pop(COMPONENT_SCOPE);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// CompLoopStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

CompLoopStmtImplFactory::CompLoopStmtImplFactory()
{}


ExtendedStmtImpl *CompLoopStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new CompLoopStmtImpl(parent);
}

///////////////////////////////////////////////////////////////////////////////
// WaitForStmtImpl
///////////////////////////////////////////////////////////////////////////////

WaitForStmtImpl::WaitForStmtImpl(DM &dm, ExtendedStmt &parent)
	: AuditableExtendedStmtImpl(dm, parent.ident()), m_parent(parent)
{}


WaitForStmtImpl::~WaitForStmtImpl()
{}


void WaitForStmtImpl::executeWithAudit(Context &ctx)
{
	ConstCharPtr taskname = m_parent.getArgAsString("task", ctx);
	if(!taskname) {
		throw RuntimeError(m_parent, ctx.stack(),
			"No task specified");
	}
	Task *task = ctx.dm().getTask((const char*) taskname);
	if(!task) {
		throw RuntimeError(m_parent, ctx.stack(),
			"Task '%s' not found", (const char*) taskname);
	}

	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if (!app) {
		throw RuntimeError("waitFor: no application on stack");
	}

	int deploymentid = ctx.dm().getAudit().deploymentId();

	if (deploymentid) {
		ctx.dm().writeToStdOut("INFO: Waiting for task \"%s\"",task->name());
		User *user = ctx.dm().waitForTask(*task,*app,deploymentid);
		ctx.dm().writeToStdOut("INFO: Task \"%s\" completed by user \"%s\"",task->name(),user->name());
	} else {
		throw RuntimeError("waitfor can only be called from within a deployment");
	}
}


Expr *WaitForStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	return m_parent.getArg(name, ctx);
}

///////////////////////////////////////////////////////////////////////////////
// RunTaskStmtImpl
///////////////////////////////////////////////////////////////////////////////

RunTaskStmtImpl::RunTaskStmtImpl(DM &dm, ExtendedStmt &parent)
	: AuditableExtendedStmtImpl(dm, parent.ident()), m_parent(parent)
{}


RunTaskStmtImpl::~RunTaskStmtImpl()
{}


void RunTaskStmtImpl::executeWithAudit(Context &ctx)
{
	ConstCharPtr taskname = m_parent.getArgAsString("task", ctx);
	if(!taskname) {
		throw RuntimeError(m_parent, ctx.stack(),
			"No task specified");
	}
	Task *task = ctx.dm().getTask((const char*) taskname);
	if(!task) {
		throw RuntimeError(m_parent, ctx.stack(),
			"Task '%s' not found", (const char*) taskname);
	}

	Scope *scope = task->getVars(m_auditEntry->stepId(), m_parent, m_auditEntry, ctx);
	scope->setStatementObject(this);
	//Scope *scope = new Scope(TASK_SCOPE, task);
	ctx.stack().push(scope);


	ctx.dm().writeToStdOut("INFO: Executing task \"%s\"",task->name());

	try {
		task->run(m_auditEntry->stepId(), m_parent, m_auditEntry, ctx);
	} catch(...) {
		ctx.stack().pop(TASK_SCOPE);
		throw;
	}

	ctx.stack().pop(TASK_SCOPE);
}


Expr *RunTaskStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	return m_parent.getArg(name, ctx);
}

///////////////////////////////////////////////////////////////////////////////
// WaitForStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

WaitForStmtImplFactory::WaitForStmtImplFactory()
{}


ExtendedStmtImpl *WaitForStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new WaitForStmtImpl(dm, parent);
}

///////////////////////////////////////////////////////////////////////////////
// RunTaskStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

RunTaskStmtImplFactory::RunTaskStmtImplFactory()
{}


ExtendedStmtImpl *RunTaskStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new RunTaskStmtImpl(dm, parent);
}


///////////////////////////////////////////////////////////////////////////////
// LocalScriptStmtImpl
///////////////////////////////////////////////////////////////////////////////

LocalScriptStmtImpl::LocalScriptStmtImpl(ExtendedStmt &parent, Action &action)
	: m_parent(parent), m_action(action)
{}


LocalScriptStmtImpl::~LocalScriptStmtImpl()
{}


void LocalScriptStmtImpl::execute(Context &ctx)
{
	const char *rawfilepath = m_action.filepath();
	const char *shell = m_action.shell();

	// Expand variables in the filepath
	Node nfilepath(NODE_STR, strdup(rawfilepath), true);
	ExprPtr efilepath = nfilepath.evaluate(ctx);
	ConstCharPtr filepath = efilepath->stringify();

	if(!filepath) {
		throw RuntimeError(m_parent, ctx.stack(),
			"filepath must be specified");
	}

	// Has a dropzone been specified?  If so, calculate the path and we will set this as the CWD
	const char *cwd = NULL;
	Dropzone *dz = m_parent.getOptionalDropzone(ctx);
	if(dz) {
		debug1("using dropzone '%s' - cwd will be '%s'", dz->name(), dz->pathname());
		cwd = dz->pathname();
	}

	CmdLine cmd;
#ifdef WIN32
	if(	STRCASECMP(&filepath[strlen(filepath)-4], ".vbs") == 0 ||
		STRCASECMP(&filepath[strlen(filepath)-4], ".wsf") == 0 ||
		STRCASECMP(&filepath[strlen(filepath)-3], ".js") == 0)  {
		cmd.add("cscript").add("/nologo");
	}
#endif /*WIN32*/
	cmd.add(filepath);

	Envp env(ctx.stack());

	//StmtList *args = m_parent.getArgs();
	//List<ActionArg> *ordargs = m_action.getArgs();
	//Hashtable<ActionArg> *argmap = m_action.getArgMap();

	//// Go through arg map and make sure we have all required args
	//if(argmap) {
	//	AutoPtr<StringList> keys = argmap->keys();
	//	StringListIterator kit(*keys);
	//	for(const char *key = kit.first(); key; key = kit.next()) {
	//		ActionArg *lsca = argmap->get(key);
	//		if(lsca && lsca->required()) {
	//			bool found = false;
	//			if(args) {
	//				ListIterator<Stmt> ait(*args);
	//				for(Stmt *arg = ait.first(); arg; arg = ait.next()) {
	//					if(strcmp(arg->name(), lsca->name()) == 0) {
	//						debug3("found required arg '%s' for localscript '%s'", arg->name(), m_action.name());
	//						found = true;
	//						break;
	//					}
	//				}
	//			}
	//			if(!found) {
	//				throw RuntimeError(m_parent, ctx.stack(),
	//						"Required arg '%s' must be specified for localscript '%s'",
	//						lsca->name(), m_action.name());
	//			}
	//		}
	//	}
	//}

	//// Go through provided args and make sure that they are all expected
	//if(args) {
	//	ListIterator<Stmt> ait(*args);
	//	for(Stmt *arg = ait.first(); arg; arg = ait.next()) {
	//		ActionArg *lsca = argmap ? argmap->get(arg->name()) : NULL;
	//		if(!lsca && (strcmp(arg->name(), "dropzone") != 0)) {
	//			throw RuntimeError(m_parent, ctx.stack(),
	//				"Arg '%s' is unknown for localscript '%s'",
	//				arg->name(), m_action.name());
	//		}
	//		debug3("found arg '%s' for localscript '%s'", arg->name(), m_action.name());
	//	}
	//}

	//// Everything seems okay, go through the args in order
	//if(ordargs) {
	//	ListIterator<ActionArg> oit(*ordargs);
	//	for(ActionArg *lsca = oit.first(); lsca; lsca = oit.next()) {
	//		ConstCharPtr val = m_parent.getArgAsString(lsca->name(), ctx);
	//		if(val) {
	//			// Prefix with optional switch
	//			if(lsca->getSwitch()) {
	//				cmd.add(lsca->getSwitch());
	//			}
	//			cmd.add(val);
	//			env.putTri(lsca->name(), val);
	//		} else if(lsca->pad()) {
	//			// Positional parameter was missing, so pad to keep arguments in correct positions
	//			cmd.add("");
	//		}
	//	}
	//}

	StringList *params = m_action.mapArgsForAction(m_parent, ctx, &env);
	debug3("mapped args for action(1)");
	cmd.add(*params);

	debug3("Running %s...", cmd.toCommandString());

	bool bShowOutput = true;

	CapturedData *cd = NULL;
	int tempExitStatus;
	int ret = executeAndCapture(ctx.stream(), cmd, cwd, bShowOutput, ctx.threadId(), &tempExitStatus, &cd, env.ptr());
	if(ret) {
		ctx.dm().writeToStdOut("localscript '%s' failed to execute", m_action.name());
	}
	ctx.stack().setGlobal("?",tempExitStatus);

	SAFE_DELETE(cd)
}


///////////////////////////////////////////////////////////////////////////////
// RemoteScriptActionImpl
///////////////////////////////////////////////////////////////////////////////

RemoteScriptActionImpl::RemoteScriptActionImpl(
		ExtendedStmt &parent, Action &action)
	: m_parent(parent), m_action(action)
{}


RemoteScriptActionImpl::~RemoteScriptActionImpl()
{}


int RemoteScriptActionImpl::executeOne(
	Server &target, TransferProviderImpl &xfer, Context &ctx)
{
	// Look for a platform-specific script first, then try the default, which may be null
	const char *rawfilepath = target.serverType() ? m_action.filepath(*target.serverType()) : NULL;
	if(!rawfilepath) {
		rawfilepath = m_action.filepath();
	}
	if(!rawfilepath) {
		if(target.serverType()) {
			throw RuntimeError(m_parent, ctx.stack(),
				"No filepath given for remote action '%s' with target system type '%s'",
				m_parent.name(), target.serverType()->hosttype());
		} else {
			throw RuntimeError(m_parent, ctx.stack(),
				"No filepath given for remote action '%s'", m_parent.name());
		}
	}

	

	// Expand variables in the filepath
	Node nfilepath(NODE_STR, strdup(rawfilepath), true);
	ExprPtr efilepath = nfilepath.evaluate(ctx);
	ConstCharPtr filepath = efilepath->stringify();

	if(!filepath) {
		throw RuntimeError(m_parent, ctx.stack(),
			"filepath must be specified");
	}

	const char *script = filepath;

	const char *rawshell = m_action.shell();
	const char *shell;
	if (rawshell && rawshell[0]) {
		// Expand variables in the shell
		Node nshell(NODE_STR, strdup(rawshell), true);
		ExprPtr eshell = nshell.evaluate(ctx);
		shell = eshell->stringify();
	} else {
		shell = (const char *)0;
	}
	StringList *params = m_action.mapArgsForAction(m_parent, ctx);
	if (shell) debug3("shell is %s",shell);
	debug3("Running %s...", (const char*) script);

	int exitCode = xfer.runScriptOnServer(shell, script, /*&*/params, m_action.copyToRemote(), m_action.useTTY(), ctx);
	debug1("exitCode = %d", exitCode);
	ctx.stack().setGlobal("?", exitCode);
	debug3("leaving executeOne");
	return exitCode;
}


void RemoteScriptActionImpl::execute(Context &ctx)
{
	Environment *targetEnv = ctx.dm().getTargetEnvironment();
	if(!targetEnv) {
		throw RuntimeError(m_parent, ctx.stack(),
				"Target environment not set - unable to run remote action");
	}

	DMArray *arr = NULL;

	ListIterator<Server> iter(ctx.servers());
	for(Server *s = iter.first(); s; s = iter.next()) {
		// each thread gets its own copy of the stack
		ScopeStack *threadStack = new ScopeStack(ctx.stack());

		debug1("Getting Server '%s' credentials", s->name());
		Credentials *creds = s->getCredentials();
		if(!creds) {
			debug1("Getting Environment '%s' credentials", targetEnv->name());
			creds = targetEnv->getCredentials();
		}
		if(!creds) {
			debug1("Getting dialog credentials");
			creds = ctx.dm().getDialogCredentials();
		}
		if (_stricmp(s->hostname(),"localhost")) {
			// Not localhost
			if(!creds) {
				// No credentials for server - abort
				//TODO: throw RuntimeError("No credentials for Server '%s' - unable to deploy", s->name());
				ctx.dm().writeToStdOut("WARNING: No credentials for Server '%s' - unable to execute",s->name());
				debug1("SKIPPING SERVER '%s'", s->name());
				continue;
			} else {
				if(debuglevel() >= 1) {
					printf("DEBUG: Credentials: "); creds->print(0); printf("\n");
				}
			}
		}

		// push the server onto the copied stack (if we are not in a psloop)
		bool popsrv = !threadStack->contains(SERVER_SCOPE);
		if(popsrv) {
			ctx.stack().push(s->getVars());
		}

		// TODO: Should we reduce the server list to just this one server?
		try {
			// Now create a transfer provider for us to talk to the server
			TransferProviderImplFactory *factory = TransferProviderImplRegistry::instance().getFactory(s->protocol());
			if(!factory) {
				throw RuntimeError(m_parent, ctx.stack(),
					"Unable to find transfer provider for protocol '%s'",
					(s->protocol() ? s->protocol() : "(null)"));
			}

			// TODO: Allow component and workdir to be passed in
			TransferProviderImpl *xfer = factory->create(*s, targetEnv, creds, NULL, NULL);	// PAG
			if(!xfer) {
				throw RuntimeError(m_parent, ctx.stack(),
					"Unable to create transfer provider for server '%s'", s->name());
			}
			int ret = executeOne(*s, *xfer, ctx);
			debug3("After call to executeOne - return code = %d",ret);
		} catch(...) {
			if(popsrv) { ctx.stack().pop(SERVER_SCOPE); }
			throw;
		}

		if(popsrv) {
			debug3("Popping server off stack");
			ctx.stack().pop(SERVER_SCOPE);
			debug3("done popping server");
		}
	}
}
