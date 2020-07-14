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
