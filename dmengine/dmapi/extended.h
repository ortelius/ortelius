#ifndef __extended_h

#define __extended_h


#include "hashtable.h"
#include "dmapiexport.h"
#include "model.h"


class DMAPI_API ExtendedStmtImpl
{
public:
	virtual ~ExtendedStmtImpl();

	virtual void execute(class Context &ctx) = 0;
	virtual void execute(class DMThreadList &threads, class Context &ctx);
};


class DMAPI_API ExtendedStmtImplFactory
{
public:
	virtual ~ExtendedStmtImplFactory();

	virtual bool allowsBody() = 0;
	virtual bool allowsPrePost() = 0;
	virtual bool isThreaded() = 0;
	virtual ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent) = 0;
};


class DMAPI_API ExtendedStmtImplRegistry
{
private:
	Hashtable<ExtendedStmtImplFactory> m_factories;

	static ExtendedStmtImplRegistry *s_instance;

public:
	ExtendedStmtImplRegistry();

	static ExtendedStmtImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, ExtendedStmtImplFactory *factory);
	void registerBuiltIns();

	ExtendedStmtImplFactory *getFactory(const char *name);
};


class DMAPI_API AuditableExtendedStmtImpl : public /*virtual*/ ExtendedStmtImpl
{
protected:
	char *m_type;
	//int m_depMajor;
	class AuditEntry *m_auditEntry;

public:
	AuditableExtendedStmtImpl(class DM &dm, const char *type);
	virtual ~AuditableExtendedStmtImpl();

	void execute(class Context &ctx);
	void execute(class DMThreadList &threads, class Context &ctx);

	virtual void executeWithAudit(class Context &ctx) = 0;
	virtual void executeWithAudit(class DMThreadList &threads, class Context &ctx);
};


class DMAPI_API LocalScriptStmtImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;
	class Action &m_action;

public:
	LocalScriptStmtImpl(ExtendedStmt &parent, class Action &action);
	~LocalScriptStmtImpl();

	void execute(class Context &ctx);
};


class DMAPI_API RemoteScriptActionImpl : public virtual ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;
	class Action &m_action;

	int executeOne(
		class Server &target, class TransferProviderImpl &xfer,
		class Context &ctx);

public:
	RemoteScriptActionImpl(ExtendedStmt &parent, class Action &action);
	~RemoteScriptActionImpl();

	void execute(class Context &ctx);
};


class ModifyStmtImpl : public virtual ExtendedStmtImpl, public IObject
{
private:
	char *m_modifier;
	class ModifyProviderImpl *m_impl;
	ExtendedStmt &m_parent;

	void process(
		class IDropzone &indz, class Node &ninfile, class IDropzone &outdz,
		class Node &outfile, bool isnew, class Context &ctx, Server *server);

public:
	ModifyStmtImpl(ExtendedStmt &parent);
	~ModifyStmtImpl();

	const char *modifier()            { return m_modifier; }
	//class ModifyProviderImpl *impl()  { return m_impl; }
	void executeSubStmt(class ExtendedStmt &stmt, class Context &ctx);

	void execute(class Context &ctx);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//Object *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


#endif /*__extended_h*/
