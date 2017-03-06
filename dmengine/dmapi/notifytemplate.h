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
#ifndef __notifytemplate_h

#define __notifytemplate_h

#include "model.h"

class NotifyTemplate
{
protected:
	char	*m_Name;
	char	*m_Subject;
	char	*m_Body;
	Notify	*m_Notify;
	char	**m_recipientotids;

public:
	NotifyTemplate(Notify &notify,const char *Name,const char *Subject,const char *Body);
	virtual ~NotifyTemplate();
	char *getName();
	char *getSubject();
	char *getBody();
	const char **getRecipients();
	void setRecipients(char **reclist);
	Notify *getNotify();
};

#endif /*__notifytemplate_h*/
