#ifndef __logprocess_h

#define __logprocess_h


#include "hashtable.h"
#include "dmapiexport.h"


class DMAPI_API MessageMatcher
{
protected:
	char *m_msgtext;
	struct regexp *m_re;
	StringHashtable m_map;

public:
	MessageMatcher(const char *msgtext);
	~MessageMatcher();

	void setRegexp(const char *re);	// throws RuntimeError
	void addMapping(int renum, const char *name);

	virtual StringHashtable *match(const char *logmsg);
};


#endif /*__logprocess_h*/
