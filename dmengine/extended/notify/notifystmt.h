#ifndef __extended_notifystmt_h

#define __extended_notifystmt_h


#include "../../dmapi/extended.h"


///////////////////////////////////////
// Notify
///////////////////////////////////////

class NotifyStmtImpl : public /*virtual*/ AuditableExtendedStmtImpl, public IObject
{
private:
	class ExtendedStmt &m_parent;
	List<class Attachment> *m_attachments;

public:
	NotifyStmtImpl(class DM &dm, class ExtendedStmt &parent);
	~NotifyStmtImpl();

	void executeWithAudit(class Context &ctx);

	void addAttachment(class Attachment *attach);

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_STATEMENT_IMPL; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//Object *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject() { return NULL; }
};


class NotifyStmtImplFactory : public /*virtual*/ ExtendedStmtImplFactory
{
public:
	NotifyStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


///////////////////////////////////////
// Attachment
///////////////////////////////////////

class DMAPI_API AttachmentStmtImpl : public /*virtual*/ ExtendedStmtImpl
{
private:
	ExtendedStmt &m_parent;

public:
	AttachmentStmtImpl(ExtendedStmt &parent);
	~AttachmentStmtImpl();

	void execute(class Context &ctx);
};


class AttachmentStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	AttachmentStmtImplFactory();

	bool allowsBody()    { return true; }
	bool allowsPrePost() { return false; }
	bool isThreaded()    { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__extended_notifystmt_h*/
