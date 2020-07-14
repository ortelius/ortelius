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



