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

#ifndef __timedjob_h

#define __timedjob_h

#include "dmapiexport.h"

class DMAPI_API TimedJob
{
private:
	int m_eventid;
	int m_envid;
	int m_appid;
	int m_usrid;

public:
	TimedJob(int eventid,int appid,int envid,int usrid);
	~TimedJob();

	int getEventId();
	int getEnvId();
	int getAppId();
	int getUsrId();
};

#endif /*__timedjob_h*/