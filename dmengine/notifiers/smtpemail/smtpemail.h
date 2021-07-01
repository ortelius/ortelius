#ifndef __notifiers_smtpemail_h

#define __notifiers_smtpemail_h


#include "../../dmapi/model.h"
#include "../../dmapi/notify.h"


class SmtpEmailNotifyImpl
	: public virtual NotifyProviderImpl
{
public:
	char *m_mailserver;
	class Expr *m_from;
	char *m_username;
	char *m_password;
	int   m_mailport;
	bool  m_useSSL;
	bool  m_useHTML;
	char *m_logfile;

	SmtpEmailNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		const char *mailserver, class Expr *from, const char *username,
		const char *password, int port, const char *logfile,bool useSSL, bool useHTML);

	virtual ~SmtpEmailNotifyImpl();

	bool testProvider(class Context &ctx);

	void notify(
		class ExtendedStmt &stmt, OutputStream &body,
		List<class Attachment> *atts, class Context &ctx);

	// NotifyImpl
	class Expr *getAttribute(const char *name, class Context &ctx);
};


class SmtpEmailNotifyImplFactory : public virtual NotifyProviderImplFactory
{
public:
	SmtpEmailNotifyImplFactory();

	NotifyProviderImpl *create(
		DMINT32 implId, class Notify &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__notifiers_smtpemail_h*/
