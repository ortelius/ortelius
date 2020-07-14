#include <stdio.h>

#include "logprocess.h"

#include "exceptions.h"
#include "charptr.h"
#include "regexp.h"


///////////////////////////////////////////////////////////////////////////////
// MessageMatcher
///////////////////////////////////////////////////////////////////////////////

MessageMatcher::MessageMatcher(const char *msgtext)
	: m_msgtext(DUP_NULL(msgtext)), m_re(NULL), m_map()
{}


MessageMatcher::~MessageMatcher()
{
	SAFE_FREE(m_msgtext);
	SAFE_FREE(m_re);
}


StringHashtable *MessageMatcher::match(const char *logmsg)
{ 
	if(!m_re) {
		return NULL;
	}

	if(regexec(m_re, (char*) logmsg)) {
		debug3("matched message %s", m_msgtext);

		StringHashtable *ret = new StringHashtable();

		for(int n = 1; m_re->startp[n]; n++) {
			CharPtr temp = (char*) malloc(m_re->endp[n] - m_re->startp[n] + 2);
			char *y = temp;
			for(char *x = m_re->startp[n]; x < m_re->endp[n]; x++) {
				*y++ = *x;
			}
			*y = '\0';
			debug3("%d = '%s'", n, (const char*) temp);

			char key[32];
			sprintf(key, "%d", n);
			const char *val = m_map.get(key);
			if(val) {
				ret->put(val, temp);
			}
		}

		return ret;
	}

	return NULL;
}


void MessageMatcher::setRegexp(const char *re)
{
	m_re = regcomp((char*) re);	// re library uses nasty char*
	if(!m_re) {
		throw RuntimeError("Bad regular expression '%s'", (const char*) re);
	}
}


void MessageMatcher::addMapping(int renum, const char *name)
{
	char count[32];
	sprintf(count, "%d", renum);
	m_map.put(count, name);
}
