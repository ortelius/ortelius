#ifndef __notifiers_slack_h

#define __notifiers_slack_h


#include "../../dmapi/model.h"
#include "../../dmapi/notify.h"


class SlackNotifyImpl
	: public virtual NotifyProviderImpl
{
private:
	char	*m_servername;
	int		m_port;
	bool	m_secure;
	char	*m_url;
	char	*m_channel;
	char	*m_logfile;
	// void handleRecipients(ExtendedStmt &stmt, Expr *recip, StringList &list, Context &ctx);

public:
	SlackNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		const char *servername, int port, bool secure, const char *url, const char *channel, const char *logfile);
	virtual ~SlackNotifyImpl();

	bool testProvider(class Context &ctx);

	void notify(
		class ExtendedStmt &stmt, OutputStream &body,
		List<class Attachment> *atts, class Context &ctx);

	// NotifyImpl
	class Expr *getAttribute(const char *name, class Context &ctx);
};


class SlackNotifyImplFactory : public virtual NotifyProviderImplFactory
{
public:
	SlackNotifyImplFactory();

	NotifyProviderImpl *create(
		DMINT32 implId, class Notify &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__notifiers_slack_h*/
