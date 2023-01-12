// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#ifndef WIN32
#include <pthread.h>
#endif

#include "remotescript.h"


#include "../../dmapi/audit.h"
#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/engineconfig.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/transfer.h"

#if !defined(WIN32)
#define _stricmp strcasecmp
#endif

///////////////////////////////////////////////////////////////////////////////
// RemoteScriptThread
///////////////////////////////////////////////////////////////////////////////

RemoteScriptThread::RemoteScriptThread(
		DMThreadList &threadList, RemoteScriptStmtImpl &parent,
		ExtendedStmt &stmt, Server &target, TransferProviderImpl *xfer,
		Context &ctx, ScopeStack *stack
	)
	: DMThread(threadList, ctx.newThreadCopy(*stack,target)), m_parent(parent),
	  m_target(target), m_xfer(xfer), m_stack(stack), m_shell(NULL),
	  m_script(NULL), m_params(NULL)
{
	m_shell = stmt.getArgAsString("shell", *m_ctx);
	m_usetty = stmt.getArgAsBoolean("usetty", *m_ctx);

	char* filepath = stmt.getArgAsString("filepath", *m_ctx);
	if(!filepath) {
		throw RuntimeError(stmt, ctx.stack(),
				"filepath must be specified for remotescript");
	}
	m_script = filepath;

	// Get the script parameters - this is done here so that we can use the server
	// scope in the thread context, but we run it when we are doing the setup
	ExprPtr params = stmt.getArg("params", *m_ctx);
	if(params) {
		// Must be an array
		if(params->kind() != KIND_ARRAY) {
			throw RuntimeError(stmt, ctx.stack(),
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
			throw RuntimeError(stmt, ctx.stack(),
					"params must be a list with indices from 0 to %d", count-1);
		}
		m_params = new StringList();
		for(int n = minindex; n <= maxindex; n++) {
			Expr temp(n);
			ExprPtr e = params->array_access(temp);
			if(!e || (e->kind() == KIND_ERROR)) {
				throw RuntimeError(stmt, ctx.stack(),
						"could not evaluate remotescript parameter %d", n);
			}
			ConstCharPtr es = e->toString();
			m_params->add(es);
		}
	}
}


RemoteScriptThread::~RemoteScriptThread()
{
	SAFE_DELETE(m_xfer);
	SAFE_DELETE(m_ctx);
	SAFE_DELETE(m_stack);
	SAFE_FREE(m_shell);
	SAFE_FREE(m_script);
	SAFE_DELETE(m_params);
}


int RemoteScriptThread::execute(Context &ctx)
{
	ctx.dm().writeToLogFile("Running in remotescript thread 0x%08lxd", m_id);

	m_parent.executePre(ctx);

	m_exitCode = m_xfer->runScriptOnServer(m_shell, m_script, m_params, false, m_usetty, ctx);
	m_parent.recordRemoteScript(m_target, m_script, m_exitCode);

	m_parent.executePost(m_exitCode, ctx);
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// RemoteScriptStmtImpl
///////////////////////////////////////////////////////////////////////////////

RemoteScriptStmtImpl::RemoteScriptStmtImpl(DM &dm, ExtendedStmt &parent)
	: AuditableExtendedStmtImpl(dm, parent.ident()), m_parent(parent)
{}


void RemoteScriptStmtImpl::recordRemoteScript(Server &target, const char *cmd, int exitCode)
{
	m_auditEntry->recordRemoteScript(target, cmd, exitCode);
}


void RemoteScriptStmtImpl::executePre(Context &ctx)
{
	m_parent.executePre(ctx);
}


void RemoteScriptStmtImpl::executePost(int exitCode, Context &ctx)
{
	Scope *post = new Scope(POST_SCOPE, NULL, m_parent);
	post->set("?", exitCode);

	ctx.stack().push(post);

	// Must be executed in try/catch so that we always do the pop and delete the Scope
	try {
		m_parent.executePost(ctx);
	} catch(...) {
		ctx.stack().pop(POST_SCOPE);
		throw;
	}
	ctx.stack().pop(POST_SCOPE);
}


void RemoteScriptStmtImpl::executeWithAudit(Context &ctx)
{
	AutoPtr<DMThreadList> threads = new DMThreadList(
		ctx.dm().getEngineConfig().getThreadLimit());
	ctx.dm().writeToLogFile("Creating new thread list for remotescript");

	executeWithAudit(*threads, ctx);

	ctx.dm().writeToLogFile("Waiting for remotescript threads to finish");
	int exitCode = threads->waitForAll();
	ctx.stack().setGlobal("?", exitCode);
}

void RemoteScriptStmtImpl::setUseTTY(bool usetty)
{
	m_usetty = usetty;
}

void RemoteScriptStmtImpl::executeWithAudit(DMThreadList &threads, Context &ctx)
{
	////bool ownThreads = ctx.threads() ? false : true;
	//debug1("ownThreads = %s", ownThreads ? "true" : "false");

	// push a new scope for the remotescript action
	ctx.stack().push(new Scope(REMOTESCRIPT_SCOPE, this));

	List<RemoteScriptThread> myThreads;
	////AutoPtr<DMThreadList> threads;

	try {
		Environment *targetEnv = ctx.dm().getTargetEnvironment();
		if(!targetEnv) {
			throw RuntimeError(m_parent, ctx.stack(),
					"Target environment not set - unable to run remotescript");
		}

		////if(ownThreads) {
		////	ctx.dm().writeToLogFile("Creating new thread list for remotescript");
		////	ctx.setThreads(threads = new DMThreadList());
		////}

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
					ctx.dm().writeToStdOut("WARNING: No credentials for Server '%s' - unable to run remotescript",s->name());
					debug1("SKIPPING SERVER '%s'", s->name());
					continue;
				} else {
					if(debuglevel() >= 1) {
						printf("DEBUG: Credentials: "); creds->print(0); printf("\n");
					}
				}
			}

			// push the server onto the copied stack (if we are not in a psloop)
			if(!threadStack->contains(SERVER_SCOPE)) {
				threadStack->push(s->getVars());
			}

			// TODO: Should we reduce the server list to just this one server?

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

			// create thread with its own context using the copy of the stack
			RemoteScriptThread *t = new RemoteScriptThread(threads, *this, m_parent, *s, xfer, ctx, threadStack);
			myThreads.add(t);
			ctx.dm().writeToLogFile("Starting new remotescript thread");
		}
	} catch(...) {
		// Unwind the stack and rethrow the exception
		ctx.stack().pop(REMOTESCRIPT_SCOPE);
		throw;
	}

	// Start all the threads we have created above, after we are clear of setup exceptions
	ListIterator<RemoteScriptThread> myThreadsIter(myThreads);
	for(RemoteScriptThread *t = myThreadsIter.first(); t; t = myThreadsIter.next()) {
		t->start();
		//debug1("started remotescript thread %d", t->id());
	}

	// Pop the scope for the remotescript action - we always pop here as
	// threads will be operating on a copy of the stack and so we won't affect
	// them and waitForAll() might raise an exception, which might bypass us
	// afterwards - reference counting takes care of deletion
	ctx.stack().pop(REMOTESCRIPT_SCOPE);

	////if(ownThreads && threads) {
	////	ctx.setThreads(NULL);
	////	//debug1("Waiting for remotescript threads to finish");
	////	ctx.dm().writeToLogFile("Waiting for remotescript threads to finish");
	////	int exitCode = threads->waitForAll();
	////	ctx.stack().setGlobal("?", exitCode);
	////}
}


Expr *RemoteScriptStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	//// TODO: Free the string returned by toString()
	//ExprPtr expr = m_parent.getArg(name, ctx);
	//return expr ? expr->toString() : NULL;
	return m_parent.getArg(name, ctx);
}


//class DMArray *RemoteScriptStmtImpl::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


//Object *RemoteScriptStmtImpl::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// RemoteScriptStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

RemoteScriptStmtImplFactory::RemoteScriptStmtImplFactory()
{}


ExtendedStmtImpl *RemoteScriptStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new RemoteScriptStmtImpl(dm, parent);
}
