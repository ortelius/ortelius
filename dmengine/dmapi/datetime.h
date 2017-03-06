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