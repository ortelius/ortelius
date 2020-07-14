#ifndef __datetime_h

#define __datetime_h

#include "dmapiexport.h"

class DMAPI_API DateTime
{
private:
	long m_date;

public:
	DateTime();				// returns current time
	DateTime(long date);
	virtual ~DateTime();

	int compare(const DateTime &other);

	int toInt();
	char *toString(const char *fmt = 0);

	DateTime *operator +(long b) const;
	DateTime *operator -(long b) const;
	int operator -(const DateTime &b) const;
};

#endif /*__datetime_h*/