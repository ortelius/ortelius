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

#ifndef __task_h

#define __task_h


#include "hashtable.h"
#include "model.h"


class TaskImpl : public IObject
{
protected:
	DMINT32 m_implId;
	class TaskImplFactory &m_factory;
	class Task &m_task;
	class DelegatingScope *m_vars;

	TaskImpl(class TaskImplFactory &factory, DMINT32 implId, class Task &task);

	int notify(class Context &ctx,bool useFailureTemplate = false);

public:
	virtual ~TaskImpl();

	virtual int run(class ExtendedStmt &stmt, class Context &ctx) = 0;

	class Scope *getVars();

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_TASK_IMPL; }
	virtual class Expr *getAttribute(const char *name, class Context &ctx);
	class Object *toObject()  { return NULL; }
};


class TaskImplFactory
{
public:
	TaskImplFactory();
	virtual ~TaskImplFactory();

	virtual TaskImpl *create(
		DMINT32 implId, class Task &parent, class ExtendedStmt &stmt,
		class Context &ctx) = 0;
};


class TaskImplRegistry
{
private:
	Hashtable<TaskImplFactory> m_factories;

	static TaskImplRegistry *s_instance;

public:
	TaskImplRegistry();

	static TaskImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, TaskImplFactory *factory);
	void registerBuiltIns();

	TaskImplFactory *getFactory(const char *name);
};


#endif /*__notify_h*/
