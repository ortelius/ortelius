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