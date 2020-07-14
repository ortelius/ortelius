#ifndef __extended_appverloop_h

#define __extended_appverloop_h


#include "../../dmapi/extended.h"


///////////////////////////////////////
// AppVerLoop
///////////////////////////////////////

class AppVerLoopStmtImpl : public /*virtual*/ ExtendedStmtImpl, public IObject
{
private:
	class ExtendedStmt &m_parent;
	bool m_isFirst;
	bool m_isLast;
	class Application *m_app;
	class Scope *m_appVerScope;
	class ApplicationVersion *m_tgtAppVer;
	class ApplicationVersion *m_cacheFirst;
	class ApplicationVersion *m_cacheLast;

	void findApp(class Context &ctx);
	class ApplicationVersion *calcFrom(class Context &ctx);
	class ApplicationVersion *calcTo(class Context &ctx);

public:
	AppVerLoopStmtImpl(class ExtendedStmt &parent);
	~AppVerLoopStmtImpl();

	void execute(class Context &ctx);

	bool isFirst()  { return m_isFirst; }
	bool isLast()   { return m_isLast; }

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class AppVerLoopStmtImplFactory : public /*virtual*/ ExtendedStmtImplFactory
{
public:
	AppVerLoopStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__extended_appverloop_h*/
