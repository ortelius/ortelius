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
