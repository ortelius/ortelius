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
#include <time.h>
#include <string.h>

#include "datetime.h"
#include "platform.h"


// returns current time
DateTime::DateTime()
{
	time_t timenow;
	time(&timenow);
	// TODO: handle time failure == -1
	m_date = (long) timenow;
}


DateTime::DateTime(long date)
{
	m_date = date;
}


/*virtual*/ DateTime::~DateTime()
{}


int DateTime::compare(const DateTime &other)
{
	if(m_date > other.m_date) return 1;
	if(m_date < other.m_date) return -1;
	return 0;
}


int DateTime::toInt()
{
	return (int) m_date;
}


char *DateTime::toString(const char *fmt /*= 0*/)
{
	char buf[256];
	time_t tt = (time_t) m_date;
	struct tm *t = localtime(&tt);
	if(strftime(buf, sizeof(buf), (fmt ? fmt : "%#c"), t) > 0) {
		return strdup(buf);
	}
	return NULL;
}


DateTime *DateTime::operator +(long b) const
{
	return new DateTime(m_date + b);
}


DateTime *DateTime::operator -(long b) const
{
	return new DateTime(m_date - b);
}


int DateTime::operator -(const DateTime &b) const
{
	return (int) (m_date - b.m_date);
}
