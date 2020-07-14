#ifndef __filenamecache_h

#define __filenamecache_h


#include "list.h"

class FilenameCache : public StringList
{
private:
	static FilenameCache *s_instance;

	FilenameCache();

	static FilenameCache &instance();

	int internalAddFilename(const char *filename);
	const char *internalGetFilename(int filenum);

public:
	static int addFilename(const char *filename);
	static const char *getFilename(int filenum);
};


#endif /*__filenamecache_h*/
