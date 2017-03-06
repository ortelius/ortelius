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