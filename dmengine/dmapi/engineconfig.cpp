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
