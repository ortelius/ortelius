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
