#ifndef __function_h

#define __function_h


#include "hashtable.h"
#include "dmapiexport.h"


class DMAPI_API FunctionNodeImpl
{
public:
	virtual ~FunctionNodeImpl();

	virtual class Expr* evaluate(class ExprList *args, class Context &ctx, bool stringify) = 0;
};


class DMAPI_API FunctionNodeImplFactory
{
public:
	virtual ~FunctionNodeImplFactory();

	virtual FunctionNodeImpl *create(class DM &dm, class FunctionNode &parent) = 0;
};


class DMAPI_API FunctionNodeImplRegistry
{
private:
	Hashtable<FunctionNodeImplFactory> m_factories;

	static FunctionNodeImplRegistry *s_instance;

public:
	FunctionNodeImplRegistry();

	static FunctionNodeImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, FunctionNodeImplFactory *factory);
	void registerBuiltIns();

	FunctionNodeImplFactory *getFactory(const char *name);
};


class DMAPI_API LocalScriptFunctionImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;
	class Action &m_action;

public:
	LocalScriptFunctionImpl(class FunctionNode &parent, class Action &action);
	~LocalScriptFunctionImpl();

	class Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


class DMAPI_API RemoteScriptFunctionImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;
	class Action &m_action;

	class Expr *evaluateOne(
		class Server &target, class TransferProviderImpl &xfer,
		class ExprList *args, class Context &ctx, bool stringify);

public:
	RemoteScriptFunctionImpl(class FunctionNode &parent, class Action &action);
	~RemoteScriptFunctionImpl();

	class Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


#endif /*__function_h*/
