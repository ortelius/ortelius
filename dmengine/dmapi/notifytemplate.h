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
