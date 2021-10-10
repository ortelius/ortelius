/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
