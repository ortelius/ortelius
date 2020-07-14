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
