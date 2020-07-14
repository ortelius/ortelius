#ifndef __plugins_restful_h

#define __plugins_restful_h


#include "extended.h"
#include "function.h"


///////////////////////////////////////////////////////////////////////////////
// Post

class Restful_PostStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	Restful_PostStmtImpl(class ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class Restful_PostStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	Restful_PostStmtImplFactory();

	bool allowsBody()		{ return false; }
	bool allowsPrePost()	{ return false; }
	bool isThreaded()       { return false; }

	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


///////////////////////////////////////////////////////////////////////////////
// Post Function

class Restful_PostFunctionImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;

public:
	Restful_PostFunctionImpl(class FunctionNode &parent);

	class Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


class Restful_PostFunctionImplFactory : public virtual FunctionNodeImplFactory
{
public:
	Restful_PostFunctionImplFactory();

	FunctionNodeImpl *create(class DM &dm, class FunctionNode &parent);
};

#endif /*__plugins_restful_h*/
