/*
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __notifiers_txtlocal_h

#define __notifiers_txtlocal_h


#include "../../dmapi/model.h"
#include "../../dmapi/notify.h"


class TxtLocalNotifyImpl
	: public virtual NotifyProviderImpl
{
private:
	class Expr *m_from;
	char *m_username;
	char *m_password;

public:
	TxtLocalNotifyImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify,
		class Expr *from, const char *username, const char *password);
	~TxtLocalNotifyImpl();

	bool testProvider(class Context &ctx);

	void notify(
		class ExtendedStmt &stmt, OutputStream &body,
		List<class Attachment> *atts, class Context &ctx);

	// NotifyImpl
	class Expr *getAttribute(const char *name, class Context &ctx);
};


class TxtLocalNotifyImplFactory : public virtual NotifyProviderImplFactory
{
public:
	TxtLocalNotifyImplFactory();

	NotifyProviderImpl *create(
		DMINT32 implId, class Notify &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__notifiers_txtlocal_h*/
