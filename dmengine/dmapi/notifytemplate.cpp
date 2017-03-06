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
#include "model.h"
#include "notifytemplate.h"

NotifyTemplate::NotifyTemplate(Notify &notify,const char *Name,const char *Subject,const char *Body)
{
	m_Name    = strdup(Name);
	m_Notify  = &notify;
	m_Subject = strdup(Subject);
	m_Body    = strdup(Body);
}

NotifyTemplate::~NotifyTemplate()
{
	SAFE_FREE(m_Name);
	SAFE_FREE(m_Subject);
	SAFE_FREE(m_Body);
}

char *NotifyTemplate::getName()
{
	return m_Name;
}

char *NotifyTemplate::getSubject()
{
	return m_Subject;
}

char *NotifyTemplate::getBody()
{
	return m_Body;
}

Notify *NotifyTemplate::getNotify()
{
	return m_Notify;
}

const char **NotifyTemplate::getRecipients()
{
	return (const char **)m_recipientotids;
}

void NotifyTemplate::setRecipients(char **reclist)
{
	m_recipientotids = reclist;
}



