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

#include "timedjob.h"
TimedJob::TimedJob(int eventid, int appid,int envid,int usrid)
{
	m_eventid = eventid;
	m_appid = appid;
	m_envid = envid;
	m_usrid = usrid;
}

TimedJob::~TimedJob()
{
}

int TimedJob::getEnvId()
{
	return m_envid;
}

int TimedJob::getAppId()
{
	return m_appid;
}

int TimedJob::getUsrId()
{
	return m_usrid;
}

int TimedJob::getEventId()
{
	return m_eventid;
}
