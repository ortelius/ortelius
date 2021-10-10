// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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



