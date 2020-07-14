#ifndef __extended_alter_h

#define __extended_alter_h


#include "../../dmapi/extended.h"


//////////////////////////////////////////////////////////////////////////////
// Alter
//////////////////////////////////////////////////////////////////////////////

class AlterStmtImpl : public ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	AlterStmtImpl(class ExtendedStmt &parent);
	~AlterStmtImpl();

	void execute(class Context &ctx);
};


class AlterStmtImplFactory : public ExtendedStmtImplFactory
{
public:
	AlterStmtImplFactory();

	bool allowsBody()    { return false; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__extended_alter_h*/
