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
