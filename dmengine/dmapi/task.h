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
