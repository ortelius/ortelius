#ifndef __notifiers_hipchat_h

#define __notifiers_hipchat_h


#include "../../dmapi/model.h"
#include "../../dmapi/notify.h"


class HipChatNotifyImpl
	: public virtual NotifyProviderImpl
{
private:
	char *m_servername;
	char *m_url;
	int m_port;
	bool m_secure;
	bool  m_useHTML;
	char *m_logfile;
	
public:
	HipChatNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		const char *servername, int port, bool secure, const char *url, const char *logfile, bool useHTML);

	virtual ~HipChatNotifyImpl();

	bool testProvider(class Context &ctx);

	void notify(
		class ExtendedStmt &stmt, OutputStream &body,
		List<class Attachment> *atts, class Context &ctx);

	// NotifyImpl
	class Expr *getAttribute(const char *name, class Context &ctx);
};


class HipChatNotifyImplFactory : public virtual NotifyProviderImplFactory
{
public:
	HipChatNotifyImplFactory();

	NotifyProviderImpl *create(
		DMINT32 implId, class Notify &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__notifiers_hipchat_h*/
