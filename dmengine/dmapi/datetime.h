/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
