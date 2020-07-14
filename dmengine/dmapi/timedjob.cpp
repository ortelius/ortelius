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