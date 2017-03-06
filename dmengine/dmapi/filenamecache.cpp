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
