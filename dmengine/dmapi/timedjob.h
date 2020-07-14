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