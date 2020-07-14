#include <stdio.h>

#include "filenamecache.h"

///////////////////////////////////////////////////////////////////////////////
// FilenameCache
///////////////////////////////////////////////////////////////////////////////

/*static*/ FilenameCache *FilenameCache::s_instance = NULL;


FilenameCache::FilenameCache()
{}


/*static*/ FilenameCache &FilenameCache::instance()
{
	if(!s_instance) {
		s_instance = new FilenameCache();
	}
	return *s_instance;
}

int FilenameCache::internalAddFilename(const char *filename)
{
	add(filename);
	return size();
}

const char *FilenameCache::internalGetFilename(int filenum)
{
	StringListIterator iter(*this);
	int n = 1;
	for(const char *f = iter.first(); f; f = iter.next(), n++) {
		if(n == filenum) {
			return f;
		}
	}
	return NULL;
}


/*static*/ int FilenameCache::addFilename(const char *filename)
{
	return instance().internalAddFilename(filename);
}


/*static*/ const char *FilenameCache::getFilename(int filenum)
{
	return instance().internalGetFilename(filenum);
}
