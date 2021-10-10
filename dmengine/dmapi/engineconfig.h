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

#ifndef __engineconfig_h

#define __engineconfig_h

#include "dmapiexport.h"

class DMAPI_API EngineConfig
{
private:
	class StringHashtable *m_entries;
	int m_threadLimit;

public:
	EngineConfig();
	~EngineConfig();

	const char *get(const char *name);
	void put(const char *name, const char *value);

	int getThreadLimit();
};

#endif /*__engineconfig_h*/
