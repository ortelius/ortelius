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
