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

#ifndef __extended_deploy_h

#define __extended_deploy_h


#include "../../dmapi/extended.h"

#include "../../dmapi/node.h"
#include "../../dmapi/thread.h"


///////////////////////////////////////
// Deploy
///////////////////////////////////////

typedef enum _deploy_kind_tag {
	DEPLOY_KIND_CHECKOUT_ONLY,
	DEPLOY_KIND_TRANSFER_ONLY,
	DEPLOY_KIND_CHECKOUT_AND_TRANSFER
} DEPLOY_KIND;


class DeployThread : public /*virtual*/ DMThread
{
private:
	class DeployStmtImpl &m_parent;
	class Dropzone &m_dropzone;
	class Server &m_target;
	class TransferProviderImpl *m_xfer;
	class ScopeStack *m_stack;
	class DropzoneCallback *m_callback;
	class Component *m_comp;

public:
	DeployThread(
		DMThreadList &threadList, class DeployStmtImpl &parent,
		class Dropzone &dropzone, class Server &target, class TransferProviderImpl *xfer,
		class DropzoneCallback *callback, class Component *comp,
		class Context &ctx, class ScopeStack *stack);
	~DeployThread();

	void marryResults(List<class TransferResult> &results, class IDropzone &dz);

	int execute(class Context &ctx);
	void addDropzoneRef();
	void relDropzoneRef();

	class Component *getComponent() {return m_comp;}
};


class DeployStmtImpl : public virtual AuditableExtendedStmtImpl, public virtual IObject
{
private:
	ExtendedStmt &m_parent;
	DEPLOY_KIND m_kind;

	void recordTransferResults(class Server &target, List<class TransferResult> &results);
	char *createFileList(List<class TransferResult> &list);
	void executePre(class Context &ctx);
	void executePost(class DropzoneCallback &cb, List<class TransferResult> *xfer, class Context &ctx);
	void executeComponentPre(class Component &comp, class Context &ctx);
	void executeComponentPost(class Component &comp, class Context &ctx);
	void executeComponentCustom(Component &comp, Context &ctx);

	void doCheckout(
		ExtendedStmt &parent, class Repository &repo, class Dropzone &dz,
		const char *target, class DropzoneCallback &callback, class Context &ctx);
	/*DMThreadList **/ void createDeployThreads(
		/*bool ownThreads*/ DMThreadList &threads, List<DeployThread> &myThreads, class Dropzone &dz,
		class Repository *repo, class DropzoneCallback *callback, class Component *comp, class Context &ctx);
	void startDeployThreads(List<DeployThread> &myThreads);
	////void waitForDeployThreads(
	////	bool ownThreads, DMThreadList *threads, Context &ctx);
	/*DMThreadList **/ void executeDeployComponent(
		/*bool ownThreads*/ DMThreadList &threads, List<DeployThread> &myThreads,
		class Component *comp, class Dropzone &dz, class Context &ctx);
	/*DMThreadList **/ void executeDeploySingle(
		/*bool ownThreads*/ DMThreadList &threads, List<DeployThread> &myThreads,
		class Dropzone &dz, class Context &ctx);

public:
	DeployStmtImpl(DM &dm, ExtendedStmt &parent, DEPLOY_KIND kind);

	void executeWithAudit(class Context &ctx);
	void executeWithAudit(class DMThreadList &threads, class Context &ctx);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//Object *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }

	friend class DeployThread;
};


class DeployStmtImplFactory : public virtual ExtendedStmtImplFactory
{
private:
	DEPLOY_KIND m_kind;

public:
	DeployStmtImplFactory(DEPLOY_KIND kind);

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return true; }
	bool isThreaded()    { return true; }
	ExtendedStmtImpl *create(class DM &dm, ExtendedStmt &parent);
};


class ComponentDeployStmt : public ExtendedStmt
{
private:
	ExtendedStmt &m_stmt;
	class ComponentItem &m_ci;

public:
	ComponentDeployStmt(ExtendedStmt &stmt, class ComponentItem &ci);
	~ComponentDeployStmt();

	class Node *getArgNode(const char *name);
	class Expr *getArg(const char *name, class Context &ctx);
	class StmtList *getArgs();
};


#endif /*__extended_deploy_h*/
