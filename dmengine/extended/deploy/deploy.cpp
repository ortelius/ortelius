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

#include "deploy.h"

#include "../appverloop/appverloop.h"
#include "../../dmapi/audit.h"
#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/dropzone.h"
#include "../../dmapi/engineconfig.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/properties.h"
#include "../../dmapi/transfer.h"

#ifndef WIN32
#define _stricmp strcasecmp
#endif


///////////////////////////////////////////////////////////////////////////////
// DeployThread
///////////////////////////////////////////////////////////////////////////////

DeployThread::DeployThread(
		DMThreadList &threadList, DeployStmtImpl &parent, Dropzone &dropzone,
		Server &target, TransferProviderImpl *xfer, DropzoneCallback *callback,
		Component *comp, Context &ctx, ScopeStack *stack
	)
	: DMThread(threadList, ctx.newThreadCopy(*stack,target)),  m_dropzone(dropzone),
	  m_target(target), m_xfer(xfer), m_parent(parent), m_stack(stack),
	  m_callback(callback), m_comp(comp)
{
	if(!m_callback) { throw RuntimeError("DropzoneCallback needed"); }
}


DeployThread::~DeployThread()
{
	debug3("~DeployThread for thread %d", m_id);
	SAFE_DELETE(m_xfer);
	SAFE_DELETE(m_ctx);		// owned
	SAFE_DELETE(m_stack);	// owned
	m_comp = NULL;			// don't own
}


void DeployThread::marryResults(List<TransferResult> &results, IDropzone &dz)
{
	// Try to marry up the TransferResults to the DropzoneFiles
	ListIterator<TransferResult> triter(results);
	for(TransferResult *r = triter.first(); r; r = triter.next()) {
		CharPtr dzfn = r->dropzoneFilename();
		debug1("dzfn=[%s]",(const char *)dzfn);
		char *reldzfn = dzfn;
		if(strstr(reldzfn, dz.pathname())) {
			reldzfn += strlen(dz.pathname());
		}
		if(*reldzfn == DIR_SEP_CHAR) {
			reldzfn++;
		}
		debug1("reldzfn=[%s]",reldzfn);
		DropzoneFile *dzf = dz.getFile(reldzfn);
		
		if(dzf) {
			r->setDropzoneFile(dzf);
		} else {
			debug1("WARNING: Failed to find dropzone file '%s' ('%s')", reldzfn, (const char*) dzfn);
			//((DropzoneBase&) dz).dump();
		}
	}
}

void DeployThread::addDropzoneRef()
{
	m_dropzone.addRef();
}

void DeployThread::relDropzoneRef()
{
	m_dropzone.releaseRef();
}

int DeployThread::execute(Context &ctx)
{
	bool RemoveThisComponent = false;
	ctx.dm().writeToLogFile("Running in deploy thread 0x%08lxd", m_id);
	m_dropzone.addRef();
	debug3("execute - just added a reference");
	if (m_comp) {
		debug3("execute - checking if we should remove old component version files from this server");
		if (ctx.dm().getRemoveApp()) {
			Scope *avloopScope = ctx.stack().getScope(AVLOOP_SCOPE);
			Variable *vLast = avloopScope?avloopScope->get("dep.last"):NULL;
			bool isLast = vLast?(bool)vLast->getInt():false;
			if (isLast) RemoveThisComponent=true;
		} else {
			Component *ec = m_target.getDeployedComponent(*m_comp);
			if (ec && ec->id() != m_comp->id()) {
				// Different version of the same component
				m_xfer->deleteFromServer(*ec, ctx);
			}
		}
	}

	int retcode=0;

	//
	// Execute any "Pre" action on the deploy TASK
	try {
		debug3("about to call m_parent.executePre");
		m_parent.executePre(ctx);
		debug3("execute parent pre - done");
	} catch(...) {
		m_dropzone.releaseRef();
		throw;
	}

	if(m_comp && m_comp->getPreAction()) {
		try {
			debug3("about to call m_parent.executeComponentPre");
			m_parent.executeComponentPre(*m_comp, ctx);
			Variable *v = ctx.stack().get("?");	// Exit code from any post executed script
			retcode = v?v->getInt():0;
			ctx.dm().writeToStdOut("INFO: Pre Action for Component %s exits code %d",m_comp->name(),retcode);
			if (retcode != 0) throw RuntimeError(ctx.stack(),
					"Pre Action %s for Component %s Failed: Return Code: %d",
					m_comp->getPreAction()->name(),m_comp->name(),retcode);
			debug3("executeComponentPre - done");
		} catch(...) {
			m_dropzone.releaseRef();
			throw;
		}
	}


	AutoPtr<List<TransferResult> > results = NULL;
	AutoPtr<List<TransferResult> > ssresults = NULL;
	AutoPtr<List<TransferResult> > merged;
	merged = new List<TransferResult>(false);

	if (m_comp && m_comp->getCustomAction()) {
		// Custom action, this takes precedence over deploying the component items
		//
		// TO DO:
		// Logic may need to be fettled here.
		// a) When rolling back, the server list should be those servers to which the
		//    component has been deployed.
		// b) When rolling forward, the server list should be those servers with which the
		//    component is associated but to which it has not yet been deployed.
		// c) We need to ensure that dep.rollback and dep.rollforward vars are set properly
		List<Server> *remainingServerList = new List<Server>();	
		ListIterator<Server> iter(ctx.servers());
		Component *existingcomp = (Component *)0;
		for(Server *s = iter.first(); s; s = iter.next()) {
			// does this server already contain this component?
			if(m_comp && !m_comp->isDeployAlways() && !ctx.isRollback()) {
				existingcomp = s->getDeployedComponent(*m_comp);
				if(existingcomp && (existingcomp->id() == m_comp->id())) {
					// This version of the component is already on the server - skip deployment
					continue;
				} else {
					// Not yet deployed - add to list of outstanding servers for this component
					remainingServerList->add(s);
				}
			} else {
				// Add this component if we always deploy this component
				if (m_comp && m_comp->isDeployAlways()) remainingServerList->add(s);
			}
		}

		
		if (remainingServerList->size() > 0) {
			Context customctx(ctx, *remainingServerList);
			ctx.dm().writeToStdOut("INFO: Deploying Component %s using Custom Action %s",m_comp->name(),m_comp->getCustomAction()->name());
			m_parent.executeComponentCustom(*m_comp,customctx);
			Variable *v = customctx.stack().get("?");	// Exit code from any executed script
			retcode = v?v->getInt():0;
			ctx.dm().writeToStdOut("INFO: Return code is %d",retcode);
			if (retcode == 0) {
				// Success
				ListIterator<Server> iter(*remainingServerList);
				for(Server *server = iter.first(); server; server = iter.next()) {
					m_comp->recordDeployedToServer(ctx.dm(),*server,true);
				}
			} else {
				// If custom action has failed, throw a runtime error
				throw RuntimeError(ctx.stack(),
					"Custom Action %s for Component %s Failed: Return Code: %d",
					m_comp->getCustomAction()->name(),m_comp->name(),retcode);
			}
		}
		if (m_comp->getPostAction()) {
			m_parent.executeComponentPost(*m_comp, ctx);
			Variable *v = ctx.stack().get("?");	// Exit code from any post executed script
			retcode = v?v->getInt():0;
			ctx.dm().writeToStdOut("INFO: Post Action for Component %s exits code %d",m_comp->name(),retcode);
		}
		return retcode;
	} else {
		debug3("calling transfer to server");

		results = m_xfer->transferToServer(m_dropzone.pathname(), false, ctx);
		if(results) {
			marryResults(*results, m_dropzone);
			m_parent.recordTransferResults(m_target, *results);
		}
		
		// Does this server have a server-specific dropzone?
		ServerDropzone *sdz = m_dropzone.getServerDropzone(m_target);
		/* AutoPtr<List<TransferResult> > ssresults; */
		if(sdz) {
			ssresults = m_xfer->transferToServer(sdz->pathname(), true, ctx);
			marryResults(*ssresults, *sdz);
			m_parent.recordTransferResults(m_target, *ssresults);
		}

		debug3("After the transfer - getting transfer results");

		


		// debug3("results = 0x%lx ssresults = 0x%lx", results, ssresults);

		if(results || ssresults) {
			
			if(results) {
				ListIterator<TransferResult> iter(*results);
				for(TransferResult *result = iter.first(); result; result = iter.next()) {
					merged->add(result);
				}
			}
			if(ssresults) {
				ListIterator<TransferResult> iter(*ssresults);
				for(TransferResult *result = iter.first(); result; result = iter.next()) {
					merged->add(result);
				}
			}

			// TODO: This needs to go into all scopes
			// Create dep.files as an array of filenames
			DMArray *dep_files = new DMArray(true, false, true);
			ListIterator<TransferResult> dfiter(*merged);
			for(TransferResult *r = dfiter.first(); r; r = dfiter.next()) {
				// dep_files->put(r->targetFilename(), new Variable(NULL, r->targetFilename()));
				dep_files->add(new Variable(NULL, r->targetFilename()));
			}
			debug2(">> dep_files->count()=%d",dep_files->count());
			ctx.stack().set("dep.files", dep_files);

			if (m_comp) {
				//
				// Set the file list for the component scope (if we're deploying a component)
				//
				Scope *compScope = m_comp->getVars();
				if (compScope) {
					Variable *vPtr = compScope->get("dep.filesbyserver");
					DMArray *comp_dep_files = vPtr?vPtr->getArray():NULL;
					if (!comp_dep_files) {
						comp_dep_files = new DMArray(false,false,true);
						compScope->set("dep.filesbyserver",comp_dep_files);
					}
					Variable *vPtr2 = comp_dep_files->get(m_target.name());
					DMArray *server_files = vPtr2?vPtr2->getArray():NULL;
					if (!server_files) {
						server_files = new DMArray(false,false,true);
						comp_dep_files->put(m_target.name(),new Variable(NULL,server_files));
					}

					for(TransferResult *r = dfiter.first(); r; r = dfiter.next()) {
						server_files->put(r->targetFilename(), new Variable(NULL, r->targetFilename()));
					}
				}
			}
			
		}
	}

	debug3("About to check component for post etc");

	Scope *avloopScope = ctx.stack().getScope(AVLOOP_SCOPE);
	Variable *vRollback = avloopScope?avloopScope->get("dep.rollback"):NULL;
	bool rollback = vRollback?(bool)vRollback->getInt():false;
	// ctx.dm().writeToStdOut("rollback is %s\n",rollback?"true":"false");
	if(m_comp) {
		try {
			m_parent.executeComponentPost(*m_comp, ctx);
			Variable *v = ctx.stack().get("?");	// Exit code from any post executed script
			int retcode = v?v->getInt():0;
			bool usePredecessor=false;
			if (rollback) {
				// We're rolling back - check if this component has rollback items. If so, we need to record
				// the predecessor version against the server (If we're rolling back from v2 to v1 we deploy
				// v2 rollback item which would otherwise store v2 against the DB and not v1.
				//
				if (m_comp->getRollback() == OFF) {
					// No component level rollback (which is enforced through the WebUI but is still supported).
					// Check the items
					List<ComponentItem> *items = m_comp->getItems();
					ListIterator<ComponentItem> iter(*items);
					for(ComponentItem *item = iter.first(); item; item = iter.next()) {
						if (item->getRollback() == ON) {
							// item in this component has a rollback (we ignore OFF and ALL. ALL means we execute
							// the last one in the list anyway)
							usePredecessor=true;
							break;
						}
					}
				} else {
					usePredecessor=(m_comp->getRollback() == ON);	// Will be false if rollback is ALL
				}
			}
			// ctx.dm().writeToStdOut("usePredecessor is %s\n",usePredecessor?"true":"false");
			if (usePredecessor) {
				int predecessorid = m_comp->getPredecessorId();
				Component *precomp = ctx.dm().getComponentById(predecessorid);
				if (precomp) {
					ctx.dm().writeToStdOut("precomp is %s\n",precomp->name());
					precomp->recordDeployedToServer(ctx.dm(), m_target, retcode==0);
				}
			} else {
				m_comp->recordDeployedToServer(ctx.dm(), m_target, retcode==0);
			}

			if (RemoveThisComponent) {
				m_xfer->deleteFromServer(*m_comp, ctx);
				m_comp->recordDeployedToServer(ctx.dm(),m_target,false);	// success=false removes the component from the server.

			}
			debug3("after recordDeployedToServer");
		} catch(...) {
			m_dropzone.releaseRef();
			printf("Caught exception - rethrowing\n");
			throw;
		}
	} 

	try {
		debug3("About to executePost");
		m_parent.executePost(*m_callback, merged, ctx);
		debug3("done executePost");
	} catch(...) {
		m_dropzone.releaseRef();
		throw;
	}

	debug3("Calling releaseRef");
	m_dropzone.releaseRef();
	//TODO: delete m_callback after ALL threads exit, as it is shared
	return retcode;
}


///////////////////////////////////////////////////////////////////////////////
// DeployStmtImpl
///////////////////////////////////////////////////////////////////////////////

DeployStmtImpl::DeployStmtImpl(DM &dm, ExtendedStmt &parent, DEPLOY_KIND kind)
	: AuditableExtendedStmtImpl(dm, parent.ident()),
	  m_parent(parent), m_kind(kind)
{}


void DeployStmtImpl::recordTransferResults(Server &target, List<TransferResult> &results)
{
	m_auditEntry->recordTransferResults(target, results);
}


char *DeployStmtImpl::createFileList(List<TransferResult> &list)
{
	char *res=(char *)malloc(10);
	if (res) {
		res[0]='\0';

		ListIterator<TransferResult> iter(list);
		for(TransferResult *result = iter.first(); result; result = iter.next()) {
			char *str = result->targetFilename();
			if(str) {
				int newLen = strlen(res) + strlen(str) + 10;
				char *res2 = (char*) realloc(res, newLen);
				if (res2) {
					res = res2;
				}
				if(res[0]) {
					strcat(res, " ");
				}
				strcat(res, "\"");
				strcat(res, str);
				strcat(res, "\"");
			}
		}
	}
	return res;
}


void DeployStmtImpl::executePre(Context &ctx)
{
	m_parent.executePre(ctx);
}


void DeployStmtImpl::executePost(
	DropzoneCallback &cb, List<TransferResult> *results, Context &ctx)
{
	if(cb.repo()) {
		ctx.stack().push(cb.repo()->getVars(cb.repoImplId(), m_parent, m_auditEntry, ctx));
	}

	Scope *postScope = new Scope(POST_SCOPE, NULL, m_parent);
	ctx.stack().push(postScope);
	
	// Add these variables to the post scope
	postScope->set("TRIDM_DEP_TOTAL",   cb.total());
	postScope->set("TRIDM_DEP_SUCCESS", cb.success());
	postScope->set("TRIDM_DEP_FAILED",  cb.failed());
	postScope->set("TRIDM_DEP_NOTPROC", cb.notProcessed());
	postScope->set("TRIDM_DEP_ERROR",   0);  // TODO: This needs recoding and passing thru

	if(results) {
		char *fileList = createFileList(*results);
		debug1("fileList=[%s]",fileList);
		postScope->set("TRIDM_DEP_FILELIST", fileList);
		ctx.stack().appendAll("dep.filelist", fileList);
		SAFE_FREE(fileList);
	}

	// Must be executed in try/catch so that we always do the pop and delete the Scope
	try {
		m_parent.executePost(ctx);
	} catch(...) {
		ctx.stack().pop(POST_SCOPE);
		if(cb.repo()) {
			ctx.stack().pop(REPOSITORY_SCOPE);
		}
		throw;
	}
	ctx.stack().pop(POST_SCOPE);
	if(cb.repo()) {
		ctx.stack().pop(REPOSITORY_SCOPE);
	}
}


void DeployStmtImpl::executeComponentPre(Component &comp, Context &ctx)
{
	//m_auditEntry->stepId();
	ctx.dm().deployId();
	Action *preAction = comp.getPreAction();
	if(preAction) {
		// Use ExtendedStmt implementation mechanism to perform the action call.
		// This will handle parsing scripts in files or the db, mapping to external
		// scripts and even calling plugin actions directly.
		try {
			Audit &audit = ctx.dm().getDummyAudit();
			AuditEntry *ae = audit.newAuditEntry("ComponentPre");
			ae->m_compId = comp.id();
			ae->start();
			ExtendedStmt stmt(preAction);
			stmt.execute(ctx);
			ae->finish();
		} catch(ReturnException &/*e*/) {
			// Normal return via return statement
		}
	}
}


void DeployStmtImpl::executeComponentPost(Component &comp, Context &ctx)
{
	//m_auditEntry->stepId();
	//ctx.dm().deployId();
	Action *postAction = comp.getPostAction();
	if(postAction) {
		// Use ExtendedStmt implementation mechanism to perform the action call.
		// This will handle parsing scripts in files or the db, mapping to external
		// scripts and even calling plugin actions directly.
		try {
			Audit &audit = ctx.dm().getDummyAudit();
			AuditEntry *ae = audit.newAuditEntry("ComponentPost");
			ae->m_compId = comp.id();
			ae->start();
			ExtendedStmt stmt(postAction);
			stmt.execute(ctx);
			ae->finish();
		} catch(ReturnException &/*e*/) {
			// Normal return via return statement
		}
	}
}

void DeployStmtImpl::executeComponentCustom(Component &comp, Context &ctx)
{
	//m_auditEntry->stepId();
	//ctx.dm().deployId();
	Action *customAction = comp.getCustomAction();
	if(customAction) {
		// Use ExtendedStmt implementation mechanism to perform the action call.
		// This will handle parsing scripts in files or the db, mapping to external
		// scripts and even calling plugin actions directly.
		try {
			Audit &audit = ctx.dm().getDummyAudit();
			AuditEntry *ae = audit.newAuditEntry("ComponentCustom");
			ae->m_compId = comp.id();
			ae->start();
			ExtendedStmt stmt(customAction);
			stmt.execute(ctx);
			ae->finish();	
		} catch(ReturnException &/*e*/) {
			// Normal return via return statement
		}	
	}
}


/**
 * This is never called with m_kind == DEPLOY_KIND_TRANSFER_ONLY
 */
void DeployStmtImpl::doCheckout(
	ExtendedStmt &parent, Repository &repo, Dropzone &dz,
	const char *target, DropzoneCallback &cb, Context &ctx)
{
	const char *dzpath = NULL;
	CharPtr tempdz;
	if(target) {
		// Append target path to dropzone path
		tempdz = (char*) malloc(strlen(dz.pathname()) + strlen(target) + 2);
		sprintf(tempdz, "%s%s%s", dz.pathname(), DIR_SEP_STR, (const char*) target);
		dzpath = tempdz;
	} else {
		dzpath = dz.pathname();
	}

	// push a the scope for the repository
	ctx.stack().push(repo.getVars(cb.repoImplId(), parent, m_auditEntry, ctx));

	try {
		// This goes here to have a single dropzone for all servers - dropzone name
		// is just the deployment id (without the minor component) as we only use
		// it for this deployment
		repo.checkout(dz.pathname(), dzpath, parent, m_auditEntry, cb, ctx);
	} catch(...) {
		ctx.stack().pop(REPOSITORY_SCOPE);
		throw;
	}

	// Increment the cumulative totals
	ctx.stack().incrementAll("dep.total",    cb.total());
	ctx.stack().incrementAll("dep.success",  cb.success());
	ctx.stack().incrementAll("dep.failed",   cb.failed());
	ctx.stack().incrementAll("dep.not_proc", cb.notProcessed());

	ctx.stack().pop(REPOSITORY_SCOPE);
}


/**
 * This is never called with m_kind == DEPLOY_KIND_CHECKOUT_ONLY
 */
void DeployStmtImpl::createDeployThreads(
	DMThreadList &threads, List<DeployThread> &myThreads,
	Dropzone &dz, Repository *repo, DropzoneCallback *callback,
	Component *comp, Context &ctx)
{
	Environment *targetEnv = ctx.dm().getTargetEnvironment();
	if(!targetEnv) {
		throw RuntimeError(m_parent, ctx.stack(),
			"Target environment not set - unable to deploy");
	}

	debug3("in createDeployThreads - ctx.isRollback=%s",ctx.isRollback()?"true":"false");

	ListIterator<Server> iter(ctx.servers());
	Component *existingcomp = (Component *)0;
	for(Server *s = iter.first(); s; s = iter.next()) {
		// does this server already contain this component?
		if(comp && !comp->isDeployAlways() && !ctx.isRollback()) {
			existingcomp = s->getDeployedComponent(*comp);
			if(existingcomp && (existingcomp->id() == comp->id()))
			{
				// This version of the component is already on the server - skip deployment
				ctx.writeToStdOut("INFO: Component '%s' is already present on server '%s'", comp->name(), s->name());				
				continue;
			}
		}

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
				ctx.dm().writeToStdOut("WARNING: No credentials for Server '%s' - unable to deploy",s->name());
				debug1("SKIPPING SERVER '%s'", s->name());
				continue;
			} else {
				if(debuglevel() >= 1) {
					printf("DEBUG: Credentials: "); creds->print(0); printf("\n");
				}
			}
		}
		

		// push the repository (if only one) onto the copied stack
		if(repo) {
			threadStack->push(repo->getVars());
		}

		// push the server onto the copied stack (if we are not in a psloop)
		if(!threadStack->contains(SERVER_SCOPE)) {
			threadStack->push(s->getVars());
		}

		// We are in a deploy or transfer, so if target is specified it is
		// the target path on the remote server
		ConstCharPtr target;
		target = m_parent.getArgAsString("target", ctx);

		// TODO: Should we reduce the server list to just this one server?

		if(!s->protocol()) {
			throw RuntimeError(m_parent, ctx.stack(),
				"No protocol specified for server '%s'", s->name());
		}

		TransferProviderImplFactory *factory = TransferProviderImplRegistry::instance().getFactory(s->protocol());
		if(!factory) {
			throw RuntimeError(m_parent, ctx.stack(),
				"Unable to find transfer provider for protocol '%s'",
				(s->protocol() ? s->protocol() : "(null)"));
		}

		TransferProviderImpl *xfer = factory->create(*s, targetEnv, creds, comp, target);	// PAG
		if(!xfer) {
			throw RuntimeError(m_parent, ctx.stack(),
				"Unable to create transfer provider for server '%s'", s->name());
		}

		// create thread with its own context using the copy of the stack
		debug1("Creating a deploy thread");
		DeployThread *t = new DeployThread(threads, *this, dz, *s, xfer, callback, comp, ctx, threadStack);
		debug1("Thread Created id = %d",t->id());
		myThreads.add(t);
		ctx.dm().writeToLogFile("Starting new deploy thread");
	}
}


void DeployStmtImpl::startDeployThreads(List<DeployThread> &myThreads)
{
	debug1("startDeployThreads");
	// Start all the threads we have created above, after we are clear of setup exceptions
	ListIterator<DeployThread> myThreadsIter(myThreads);
	
	for(DeployThread *t = myThreadsIter.first(); t; t = myThreadsIter.next()) {	
		Component *comp = t->getComponent();
		if (comp && comp->isDeploySequentially()) {
			// Wait for this thread to complete before executing next thread. This forces
			// the deployment into sequential mode.
			t->addDropzoneRef();	// Prevent dropzone removal when thread exits
			t->start();	// Calls DeployThread::execute
			t->waitFor();
		} else {
			t->start();
		}
		debug1("started deploy thread %d", t->id());
	}
	for(DeployThread *t = myThreadsIter.first(); t; t = myThreadsIter.next()) {	
		Component *comp = t->getComponent();
		if (comp && comp->isDeploySequentially()) {
			// Remove reference for dropzone
			t->relDropzoneRef();
		}
	}
	debug1("Done startDeployThreads");
}


////void DeployStmtImpl::waitForDeployThreads(/*bool ownThreads,*/ DMThreadList *threads, Context &ctx)
////{
////	if(ownThreads && threads) {
////		ctx.setThreads(NULL);
////		//debug1("Waiting for deploy threads to finish");
////		ctx.dm().writeToLogFile("Waiting for deploy threads to finish");
////		int exitCode = threads->waitForAll();
////		ctx.stack().setGlobal("?", exitCode);
////	}
////}


void DeployStmtImpl::executeDeployComponent(
	DMThreadList &threads, List<DeployThread> &myThreads,
	Component *comp, Dropzone &dz, Context &ctx)
{
	// PAG MOD
	// If there are any arguments then this is a "checkout(...)" command with a
	// component on the stack. With no arguments, checkout each component item.
	// With any argument, just execute the checkout. Set the "skipComponentItems"
	// flag accordingly.
	//
	StmtList *args = m_parent.getArgs();
	bool skipComponentItems=(args && args->size() && m_kind == DEPLOY_KIND_CHECKOUT_ONLY);
	// If component is specified as a parameter then we'll need to process the component items
	if (skipComponentItems && m_parent.getArgAsString("component",ctx)) {
		// ... but only if the other parameters are not overriding component items
		int n = args->size()-1;										// knock one off for the "component" argument
		if (n && m_parent.getArgAsString("dropzone",ctx)) n--;		// "dropzone" is allowed
		if (n && m_parent.getArgAsString("target",ctx)) n--;		// "target" is allowed
		if (n==0) skipComponentItems = false;						// No parameters alongside component except dropzone and target
	}
	
	// Add compid to audit entry
	m_auditEntry->m_compId = comp->id();

	// Calculate the intersect of the current list of servers and the servers
	// upon which the component is deployed.  This will be copied when we
	// create a context for the thread, so we free it on exit from this method.
	AutoPtr<List<Server> > servers = comp->getServerSubset(ctx);

	if(servers && servers->size() == 0) {
		ctx.writeToStdOut("WARNING: component %d '%s' is not associated with any servers", comp->id(), comp->name());
		//return;	// we could bail out early here
	}

	// Create a new context with the component's servers selected
	Context newctx(ctx, *servers);

	// Are we in an AV loop?
	Scope *avloopScope = ctx.stack().getScope(AVLOOP_SCOPE);
	Variable *vFirst = avloopScope?avloopScope->get("dep.first"):NULL;
	Variable *vLast = avloopScope?avloopScope->get("dep.last"):NULL;
	Variable *vRollback = avloopScope?avloopScope->get("dep.rollback"):NULL;

	debug1("In deploy component - avloopScope = 0x%lx vFirst = 0x%lx vLast = 0x%lx\n",avloopScope,vFirst,vLast);
	//if (vFirst) printf("vFirst->getInt()=%d\n",vFirst->getInt());
	//if (vLast)  printf("vLast->getInt()=%d\n",vLast->getInt());

	AppVerLoopStmtImpl *avloop = avloopScope ? (AppVerLoopStmtImpl*) avloopScope->getScopeObject() : NULL;
	bool isFirst = avloop ? avloop->isFirst() : vFirst ? (bool)vFirst->getInt() : false;
	bool isLast = avloop ? avloop->isLast() : vLast ? (bool)vLast->getInt() : false;

	bool inLoop = avloop || vFirst;

	debug1("inLoop = %s isFirst = %s isLast = %s",inLoop?"true":"false",isFirst?"true":"false",isLast?"true":"false");

	bool filterlast = m_parent.getArgAsBoolean("filterlast", ctx);
	ExprPtr erollback = m_parent.getArg("rollback", ctx);

	bool rollback;
	if (vRollback) {
		rollback = (bool)vRollback->getInt();
	} else {
		rollback = erollback ? erollback->toBool() : newctx.isRollback();
	}
	debug1("Component '%s' rollback is %s isFirst is %s isLast is %s filterlast is %s parentid is %d",
		comp->name(), (rollback ? "true" : "false"), 
		(isFirst ? "true" : "false"),
		(isLast ? "true" : "false"),
		(filterlast ? "true" : "false"),
		comp->getParentId());

	// push the scope for the component if it isn't already the topmost component on the stack
	bool popcomp = false;
	Scope *compscope = newctx.stack().getScope(COMPONENT_SCOPE);
	if(!compscope || (compscope != comp->getVars())) {
		newctx.stack().push(comp->getVars());
		popcomp = true;
	}

	try {
		DropzoneCallback *summary = new DropzoneCallback(dz);
		bool FilesPulled=false;
		bool InfoLineWritten=false;
		ComponentFilter itemforwardflag=OFF;
		ComponentFilter itemrollbackflag=OFF;

		if(m_kind != DEPLOY_KIND_TRANSFER_ONLY && !skipComponentItems) {
			List<ComponentItem> *items = comp->getItems();
			ListIterator<ComponentItem> iter(*items);
			debug1("ComponentItem loop start");
			for(ComponentItem *item = iter.first(); item; item = iter.next()) {
				// always include if filtering at the component level
				bool include = !comp->getFilterItems();
				debug1("deploy1: include=%s; compitem=(%d,'%s',%d,%d)", (include ? "Y" : "N"), item->id(), item->name(), item->getRollup(), item->getRollback());
				if(!include) {
					itemforwardflag = item->getRollup();
					itemrollbackflag = item->getRollback();
					if (isLast && ctx.dm().getRemoveApp() && itemrollbackflag==ON) {
						// If we're deleting the application and this is the last (base) version
						// then set rollback to ALL to force the engine to deploy and run any
						// rollback items associated with the BASE version
						itemrollbackflag=ALL;
					}
					if(!filterlast && isLast) {
						// last item is included provided that doing rollup and not a
						// rollback item, or doing rollback and not a rollup item
						debug1("deploy2: filterlast = %s isLast = %s rollback = %s",
							filterlast ? "true" : "false",
							isLast ? "true" : "false",
							rollback ? "true" : "false");
						debug1("itemrollbackflag = ALL = %s",(itemrollbackflag == ALL)?"true":"false");
						debug1("check = %s",(itemrollbackflag == ALL || (itemrollbackflag == OFF && itemforwardflag == OFF))?"true":"false");
						include = rollback ? (itemrollbackflag == ALL || (itemrollbackflag == OFF && itemforwardflag == OFF)) : (itemrollbackflag == OFF);
						debug1("deploy2: include=%s", include ? "Y" : "N");
					} else {
						// printf("(a) rollback = %s\n",rollback?"true":"false");
						if(rollback) {
							// for rollbacks we go from N to T+1 (ON) or N to T (ALL), so test last item differently
							// printf("rollback - inloop = %s isLast = %s item->getRollback() = %d\n",inLoop?"true":"false",isLast?"true":"false",item->getRollback());
							include = (inLoop && isLast) ? (itemrollbackflag == ALL) : (itemrollbackflag != OFF);
							//debug1("deploy3: include=%s", include ? "Y" : "N");
						} else {
							// for rollups we go from N+1 to T (ON) or N to T (ALL), so test first item differently
							// printf("rollforward - inloop = %s isFirst = %s item->getRollup() = %d\n",inLoop?"true":"false",isFirst?"true":"false",item->getRollup());
							include = (inLoop && isFirst) ? (itemforwardflag == ALL) : (itemforwardflag != OFF);
							if (!include) {
								if (!inLoop) include=true;	// If not in a loop then just include it
							}
							//debug1("deploy4: include=%s", include ? "Y" : "N");
						}
					}
				} else {
					// filtering at component level (this is no longer provided by the Web UI but the engine still supports
					itemforwardflag  = comp->getRollup();
					itemrollbackflag = comp->getRollback();
				}
				if(include) {
					if (!InfoLineWritten) {
						/*
						ctx.dm().writeToStdOut("DEBUG: rollback=%s isLast=%s isFirst=%s itemforwardflag=%s itemrollbackflag=%s",
							rollback?"true":"false",
							isLast?"true":"false",
							isFirst?"true":"false",
							itemforwardflag?"true":"false",
							itemrollbackflag?"true":"false"
						);
						*/

						if(m_kind == DEPLOY_KIND_CHECKOUT_ONLY) {
							char *fmtstr = "INFO: Checking out items for Component %s";
							if (!rollback) {
								// Rolling forward
								if (itemforwardflag != OFF) fmtstr = "INFO: Checking out roll-forward items of Component %s";
							} else {
								// Rollback back
								if (itemrollbackflag != OFF) fmtstr = "INFO: Checking out rollback items of Component %s";
							}
						} else {
							char *fmtstr = "INFO: Deploying Component %s";
							if (!rollback) {
								// Rolling forward
								if (itemforwardflag != OFF) fmtstr = "INFO: Deploying roll-forward items of Component %s";
							} else {
								// Rollback back
								if (itemrollbackflag != OFF) fmtstr = "INFO: Deploying rollback items of Component %s";
							}
							ctx.dm().writeToStdOut(fmtstr,comp->name());
						}
						InfoLineWritten=true;
					}
					debug1("deploy: compitem is %d, '%s'", item->id(), item->name());

                   if (item->getKind() == NULL || strcmp(item->getKind(),"docker") != 0)
				   {
					Repository *repo = item->getRepository();
					if(repo) 
					{
					 const char *target = item->getTargetFolder();
					 debug1("ComponentItem(%d, '%s', '%s')", item->id(), (repo ? repo->name() : "(null)"), (target ? target : "(null)"));

					 // We use a delegating ComponentDeployStmt between us and the real parent stmt
					 ComponentDeployStmt newparent(m_parent, *item);
					 DropzoneCallback callback(repo, (item->id() * 1000 + m_auditEntry->stepId()), dz, ctx, item);
					 doCheckout(newparent, *repo, dz, target, callback, newctx);
					 summary->checkout_summary(callback.total(), callback.success(), callback.failed(), callback.notProcessed());

				     if (callback.total() == 0)
					 {
					  throw RuntimeError(m_parent, newctx.stack(),
							"\"%s\" - Item \"%s\": Checked out %d file(s) from repository \"%s\"",comp->name(),
						item->name() ? item->name() : "Unnamed",
						callback.success(),repo->name());
					 }
					 ctx.dm().writeToStdOut("INFO: \"%s\" - Item \"%s\": Checked out %d file(s) from repository \"%s\"",comp->name(),
						item->name() ? item->name() : "Unnamed",
						callback.success(),repo->name());
					 FilesPulled = true;
					}
				   }
				   else
				   {
					skipComponentItems = false;   
				   }	
				}
			}
		}

		if (m_kind == DEPLOY_KIND_CHECKOUT_ONLY && skipComponentItems) {
			// This is a checkout() with a component on the stack.
			char *reponame = m_parent.getArgAsString("repository",ctx);
			if (!reponame) throw RuntimeError("checkout requires a repository parameter");
			char *target = m_parent.getArgAsString("target", ctx);
			Repository *repo = ctx.dm().getRepository(reponame);
			DropzoneCallback *callback = new DropzoneCallback(repo, m_auditEntry->stepId(), dz, ctx);
			doCheckout(m_parent,*repo,dz,target,*callback,ctx);
		}

		// If we are only doing a checkout, then we execute post block here without any servers on the stack
		if(m_kind == DEPLOY_KIND_CHECKOUT_ONLY) {
			executePost(*summary, NULL, newctx);
			SAFE_DELETE(summary);
		} else {
			DMArray *dropzoneFiles = dz.find("*");
			if (dropzoneFiles->count()>0) FilesPulled=true;
			//printf("doing transfer, FilesPulled=%s\n",FilesPulled?"true":"false");
			dz.generateDmInfos();
			
			if (FilesPulled || comp->getCustomAction()) {
				createDeployThreads(threads, myThreads, dz, NULL, summary, comp, newctx);
			}
		}
	} catch(...) {
		// Unwind the stack and rethrow the exception
		if(popcomp) {
			newctx.stack().pop(COMPONENT_SCOPE);
		}
		throw;
	}

	// We can safely pop this here as it will remain on the stacks of each of
	// the threads we have created
	if(popcomp) {
		newctx.stack().pop(COMPONENT_SCOPE);
	}

	////return threads;
}


void DeployStmtImpl::executeDeploySingle(
	DMThreadList &threads, List<DeployThread> &myThreads, Dropzone &dz, Context &ctx)
{
	Repository *repo = NULL;
	if(m_kind != DEPLOY_KIND_TRANSFER_ONLY) {
		ConstCharPtr repository = m_parent.getArgAsString("repository", ctx);
		if(!repository) {
			throw RuntimeError(m_parent, ctx.stack(),
				"No repository specified");
		}
		if(!(repo = ctx.dm().getRepository(repository))) {
			throw RuntimeError(m_parent, ctx.stack(),
				"Repository '%s' not found", (const char*) repository);
		}
	}

	// If target is specified as well, then this becomes a subfolder of the dropzone
	ConstCharPtr target;
	if(m_kind == DEPLOY_KIND_CHECKOUT_ONLY) {
		target = m_parent.getArgAsString("target", ctx);
	}

	DropzoneCallback *callback = new DropzoneCallback(repo, m_auditEntry->stepId(), dz, ctx);

	if(m_kind != DEPLOY_KIND_TRANSFER_ONLY) {
		doCheckout(m_parent, *repo, dz, target, *callback, ctx);
	}

	// If we are only doing a checkout, then we execute post block here without any servers on the stack
	if(m_kind == DEPLOY_KIND_CHECKOUT_ONLY) {
		executePost(*callback, NULL, ctx);
		SAFE_DELETE(callback);
	} else {
		dz.generateDmInfos();

		// TODO: only pass callback if we did a checkout
		/*threads =*/ createDeployThreads(/*ownThreads*/ threads, myThreads, dz, repo, callback, NULL, ctx);
	}

	////return threads;
}


void DeployStmtImpl::executeWithAudit(Context &ctx)
{
	AutoPtr<DMThreadList> threads;
	if(m_kind != DEPLOY_KIND_CHECKOUT_ONLY) {
		threads = new DMThreadList(ctx.dm().getEngineConfig().getThreadLimit());
	}

	executeWithAudit(*threads, ctx);

	if(m_kind != DEPLOY_KIND_CHECKOUT_ONLY) {
		int exitCode = threads->waitForAll();
		ctx.stack().setGlobal("?", exitCode);
		if (threads->size() > 1) ctx.dm().writeToStdOut("INFO: Completed Parallel Deployment");
	}
}


void DeployStmtImpl::executeWithAudit(DMThreadList &threads, Context &ctx)
{
	char deploymentId[32];
	sprintf(deploymentId, "%d.%d", ctx.dm().deployId(), m_auditEntry->stepId());
	char depMajorStr[32];
	sprintf(depMajorStr, "%d", m_auditEntry->stepId());

	ConstCharPtr dropzone = m_parent.getArgAsString("dropzone", ctx);
	Scope *dzscope = ctx.stack().getScope(DROPZONE_SCOPE);
	Dropzone *dzscopj = dzscope ? (Dropzone*) dzscope->getScopeObject() : NULL;

	//                                              Added this to try and work around problem
	//                                                                  |
	//                                               --------------------------------------
	if(m_kind != DEPLOY_KIND_CHECKOUT_AND_TRANSFER && m_kind != DEPLOY_KIND_TRANSFER_ONLY && !dropzone && !dzscopj) {
		throw RuntimeError(m_parent, ctx.stack(),
				"'%s' requires a dropzone to be specified", m_parent.ident());
	}

	Dropzone *dz = NULL;
	if(dropzone) {
		dz = ctx.dm().getDropzone(dropzone);
	} else if(dzscopj) {
		dz = dzscopj;
	}

	if(!dz) {
		const char *dzname = (const char*) dropzone ? (const char*) dropzone : depMajorStr;
		dz = ctx.dm().newDropzone(dzname,
			(m_kind == DEPLOY_KIND_CHECKOUT_AND_TRANSFER) ? true : false);


		if(!dz->create(true)) {
			// Set flag to prevent us trying to delete the non-empty folder when we exit
			putenv(strdup("trinounlink=Y"));
			throw RuntimeError(m_parent, ctx.stack(),
				"a) Unable to create dropzone '%s' (%s)", dzname,dz->pathname());
		}
	}

	if(!dz->pathname()) {
		throw RuntimeError(m_parent, ctx.stack(),
			"Error creating dropzone '%s'", dz->name());
	}

	ConstCharPtr component = m_parent.getArgAsString("component", ctx);
	Component *comp = NULL;
	if(component) {
		comp = ctx.dm().getComponent(component);
		if(!comp) {
			throw RuntimeError(m_parent, ctx.stack(), "Not a valid component (%s)",(const char *)component);
		}
	} else {
		Scope *compscope = ctx.stack().getScope(COMPONENT_SCOPE);
		if (compscope) {
			comp = (Component*) compscope->getScopeObject();
			if (comp->getCustomAction()) {
				// This component has a custom action - set the a
				// marker on the stack. If the custom action calls
				// a checkout, then we'll come into here again,
				// this flag will be set and we'll drop into
				// executeSingle rather than executeComponent
				char vn[128];
				sprintf(vn,"?customcomp%d",comp->id());
				Variable *t = compscope->get(vn);
				if (t && t->getString()[0]=='Y') {
				
					// must be already in custom action for this component.
					// set comp to NULL so that we drop into executeSingle
					comp = NULL;
				} else {
					compscope->set(vn,"Y");
				}
			}
		} else {
			comp = NULL;
		}
		// comp = compscope ? (Component*) compscope->getScopeObject() : NULL;
	}

	// push a new scope for the deploy action
	ctx.stack().push(new Scope(DEPLOY_SCOPE, this));

	// push the scope for the dropzone if it isn't already the topmost dropzone on the stack
	bool popdz = false;
	if(!dzscope || (dzscope != dz->getVars())) {
		ctx.stack().push(dz->getVars());
		popdz = true;
	}

	////bool ownThreads = ctx.threads() ? false : true;
	////AutoPtr<DMThreadList> threads;
	List<DeployThread> myThreads;

	// DEPLOY_KIND_CHECKOUT_ONLY? We're executing checkout with a component
	// on the stack.
	try {
		if(comp) {
			executeDeployComponent(threads, myThreads, comp, *dz, ctx);
		} else {
			executeDeploySingle(threads, myThreads, *dz, ctx);
		}
	} catch(...) {
		// Unwind the stack and rethrow the exception - we haven't started any
		// threads yet, so we can safely delete the deploy scope
		if(popdz) {
			ctx.stack().pop(DROPZONE_SCOPE);
		}
		ctx.stack().pop(DEPLOY_SCOPE);
		throw;
	}

	if(m_kind != DEPLOY_KIND_CHECKOUT_ONLY) {
		if (comp && myThreads.size() > 1) {
			if (comp->isDeploySequentially()) {
				ctx.dm().writeToStdOut("INFO: Starting Sequential Deployment of Component %s",comp ? comp->name() : "(null)");
			} else {
				ctx.dm().writeToStdOut("INFO: Starting Parallel Deployment of Component %s",comp ? comp->name() : "(null)");
			}
		}
		startDeployThreads(myThreads);
	}

	// Pop the scope for the dropzone if we pushed it
	if(popdz) {
		ctx.stack().pop(DROPZONE_SCOPE);
	}

	// Pop and the scope for the deploy action - we always pop here as threads
	// will be operating on a copy of the stack and so we won't affect them
	// and waitForAll() might raise an exception, which might bypass us
	// afterwards - deletion is taken care of by reference counting
	ctx.stack().pop(DEPLOY_SCOPE);

	////if(m_kind != DEPLOY_KIND_CHECKOUT_ONLY) {
	////	waitForDeployThreads(ownThreads, threads, ctx);
	///}
}


Expr *DeployStmtImpl::getAttribute(const char *name, Context &ctx)
{
	//// TODO: Free the string created by toString()
	//ExprPtr expr = m_parent.getArg(name, ctx);
	//return expr ? expr->toString() : NULL;
	return m_parent.getArg(name, ctx);
}


//class DMArray *DeployStmtImpl::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


//Object *DeployStmtImpl::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// DeployStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

DeployStmtImplFactory::DeployStmtImplFactory(DEPLOY_KIND kind)
	: m_kind(kind)
{}


ExtendedStmtImpl *DeployStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new DeployStmtImpl(dm, parent, m_kind);
}


///////////////////////////////////////////////////////////////////////////////
// ComponentDeployStmt
///////////////////////////////////////////////////////////////////////////////

ComponentDeployStmt::ComponentDeployStmt(ExtendedStmt &stmt, ComponentItem &ci)
	: ExtendedStmt(DUP_NULL(stmt.ident()), NULL), m_stmt(stmt), m_ci(ci)
{}


ComponentDeployStmt::~ComponentDeployStmt()
{}


Node *ComponentDeployStmt::getArgNode(const char *name)
{
	Node *ret = m_stmt.getArgNode(name);
	if(!ret) {
		debug3("ComponentDeployStmt.getArg('%s')", name);
		Property *p = m_ci.getProperty(name);
		if(p) {
			// TODO: Need to handle encrypted properties
			// TODO: This needs to be freed
			ret = new Node(NODE_STR, DUP_NULL(p->value()), true);
		}
	}
	return ret;
}


Expr *ComponentDeployStmt::getArg(const char *name, Context &ctx)
{
	Expr *ret = m_stmt.getArg(name, ctx);
	if(!ret) {
		debug3("ComponentDeployStmt.getArg('%s')", name);
		Property *p = m_ci.getProperty(name);
		if(p) {
			debug3("p->value()=[%s]",p->value());
			// TODO: Need to handle encrypted properties
			Node nvalue(NODE_STR, DUP_NULL(p->value()), true);
			ret = nvalue.evaluate(ctx);
		} else {
			debug3("null return from m_ci.getProperty('%s')",name);
		}
	}
	return ret;
}


StmtList *ComponentDeployStmt::getArgs()
{
	debug3("ComponentDeployStmt.getArgs()\n");
	if(!m_args) {
		m_args = m_ci.getPropertiesAsArgs();
		StmtList *args = m_stmt.getArgs();
		if(args) {
			ListIterator<Stmt> iter(*args);
			for(Stmt *s = iter.first(); s; s = iter.next()) {
				m_args->add(new Stmt(*s));
			}
		}
	}

	return m_args;
}
