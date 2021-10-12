// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>

#include "engineconfig.h"

#include "hashtable.h"


#define DEFAULT_THREADLIMIT 25

#define CONFIG_THREADLIMIT "thread_limit"


EngineConfig::EngineConfig()
	: m_entries(NULL), m_threadLimit(DEFAULT_THREADLIMIT)
{}


EngineConfig::~EngineConfig()
{
	SAFE_DELETE(m_entries);
}


const char *EngineConfig::get(const char *name)
{
	return m_entries ? m_entries->get(name) : NULL;
}


void EngineConfig::put(const char *name, const char *value)
{
	if(!m_entries) {
		m_entries = new StringHashtable();
	}

	m_entries->put(name, value);

	if(STRCASECMP(name, CONFIG_THREADLIMIT) == 0) {
		m_threadLimit = atoi(value);
	}
}


int EngineConfig::getThreadLimit()
{
	return m_threadLimit;
}
