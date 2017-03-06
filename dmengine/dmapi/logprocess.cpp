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
