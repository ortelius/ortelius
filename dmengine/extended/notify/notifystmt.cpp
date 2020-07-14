#include <stdio.h>

#include "notifystmt.h"


#include "../../dmapi/audit.h"
#include "../../dmapi/context.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/node.h"
#include "../../dmapi/notify.h"
#include "../../dmapi/scopestack.h"


///////////////////////////////////////////////////////////////////////////////
// NotifyStmtImpl
///////////////////////////////////////////////////////////////////////////////

NotifyStmtImpl::NotifyStmtImpl(DM &dm, ExtendedStmt &parent)
	: AuditableExtendedStmtImpl(dm, parent.ident()), m_parent(parent), m_attachments(NULL)
{}


NotifyStmtImpl::~NotifyStmtImpl()
{
	SAFE_DELETE(m_attachments);
}

void NotifyStmtImpl::executeWithAudit(Context &ctx)
{
	ConstCharPtr notifier = m_parent.getArgAsString("notifier", ctx);
	if(!notifier) {
		throw RuntimeError(m_parent, ctx.stack(),
			"No notifier specified");
	}
	Notify *notify = ctx.dm().getNotifier((const char*) notifier);
	if(!notify) {
		throw RuntimeError(m_parent, ctx.stack(),
				"Notifier '%s' not found", (const char*) notifier);
	}

	ExprPtr ebody = m_parent.getArg("body", ctx);
	OutputStream *body = ebody ? ebody->toStream() : NULL;

	// If no body argument is specified, the body is created by the body of the notify
	bool ownBody = false;
	if(!body) {
		body = new OutputStream();
		ownBody = true;
		Scope *scope = notify->getVars(m_auditEntry->stepId(), m_parent, m_auditEntry, ctx);
		scope->setStatementObject(this);
		ctx.stack().push(scope);
		try {
			Context newctx(ctx, body);
			m_parent.executeBody(newctx);
		} catch(...) {
			ctx.stack().pop(NOTIFY_SCOPE);
			SAFE_DELETE(body);
			throw;
		}
		ctx.stack().pop(NOTIFY_SCOPE);
	}

	notify->notify(m_auditEntry->stepId(), m_parent, m_auditEntry, *body, m_attachments, ctx);

	if(ownBody) {
		SAFE_DELETE(body);
	}
}


void NotifyStmtImpl::addAttachment(Attachment *attach)
{
	if(!m_attachments) {
		m_attachments = new List<Attachment>(true);
	}
	m_attachments->add(attach);
}


Expr *NotifyStmtImpl::getAttribute(const char *name, class Context &ctx)
{
	//return NULL;
	return m_parent.getArg(name, ctx);
}


//class DMArray *NotifyStmtImpl::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


//Object *NotifyStmtImpl::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// NotifyStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

NotifyStmtImplFactory::NotifyStmtImplFactory()
{}


ExtendedStmtImpl *NotifyStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new NotifyStmtImpl(dm, parent);
}


///////////////////////////////////////////////////////////////////////////////
// AttachmentStmtImpl
///////////////////////////////////////////////////////////////////////////////

AttachmentStmtImpl::AttachmentStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


AttachmentStmtImpl::~AttachmentStmtImpl()
{}


void AttachmentStmtImpl::execute(Context &ctx)
{
	Scope *notifyScope = ctx.stack().getScope(NOTIFY_SCOPE);
	if(!notifyScope) {
		throw RuntimeError(m_parent, ctx.stack(),
				"attachment cannot be used outside of notify");
	}
	ExtendedStmtImpl *obj = notifyScope->getStatementObject();
	if(!obj) {
		throw RuntimeError(m_parent, ctx.stack(),
				"attachment cannot find its parent notify");
	}

	ConstCharPtr name = m_parent.getArgAsString("name", ctx);

	bool ownStream = false;
	ExprPtr ebody = m_parent.getArg("body", ctx);
	class OutputStream *stream = ebody ? ebody->toStream() : NULL;

	// If no body argument is specified, the body is created by the body of the attachment
	if(!stream) {
		stream = new OutputStream();
		ownStream = true;
		Context newctx(ctx, stream);
		m_parent.executeBody(newctx);
	}

	// This is slightly nasty, but we only ever associate this with a notify scope
	NotifyStmtImpl *notify = (NotifyStmtImpl*) obj;
	notify->addAttachment(new Attachment(name, stream, ownStream));
}


///////////////////////////////////////////////////////////////////////////////
// AttachmentStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

AttachmentStmtImplFactory::AttachmentStmtImplFactory()
{}


ExtendedStmtImpl *AttachmentStmtImplFactory::create(DM &dm, ExtendedStmt &parent)
{
	return new AttachmentStmtImpl(parent);
}
