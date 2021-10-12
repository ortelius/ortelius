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
#include <string.h>
#include <stdlib.h>

#include "filefind.h"

#include "platform.h"

#ifndef WIN32
#include "exceptions.h"
#include "rewrap.h"
#include <sys/stat.h>
#endif /*WIN32*/

///////////////////////////////////////////////////////////////////////////////
// FindFile
///////////////////////////////////////////////////////////////////////////////

FileFind::FileFind(const char *path, const char *pattern)
	: m_filepath(DUP_NULL(path)), m_pattern(DUP_NULL(pattern)),
#ifdef WIN32
	  m_hList((HANDLE) 0), m_winpat(NULL)
#else
	  m_dir(NULL), m_entry(NULL), m_re(NULL)
#endif /*WIN32*/
{
	int i=0;
	int len=0;
	
	if (m_filepath != NULL)
	{
	 #ifdef WIN32
		 len = strlen(m_filepath);
		 for (i=0;i<len;i++)
			{
				if (m_filepath[i] == '/')
					m_filepath[i] = '\\';
			}
		#else
		 len = strlen(m_filepath);
		 for (i=0;i<len;i++)
			{
				if (m_filepath[i] == '\\')
					m_filepath[i] = '/';
			}				
		#endif		
	}	
#ifndef WIN32
	if(m_pattern && (strcmp(m_pattern, "*") != 0)) {
		// may raise and exception
		m_re = new RegExpWrapper(m_pattern, true);
	}
#endif /*UNIX*/
}


FileFind::~FileFind()
{
	close();

	SAFE_FREE(m_filepath);
	SAFE_FREE(m_pattern);
#ifdef WIN32
	SAFE_FREE(m_winpat);
#else
	SAFE_DELETE(m_re);
#endif /*WIN32*/
}


bool FileFind::find()
{
#ifdef WIN32
	char *winpat = (char*) malloc(strlen(m_filepath) + (m_pattern ? strlen(m_pattern) : 1) + 10);
	sprintf(winpat, "%s\\%s", m_filepath, (m_pattern ? m_pattern : "*"));

	// Tidy the path to windows format as find file is fussy
	for(char *x = winpat; x && *x; x++) {
		if(*x == '/') {
			*x = '\\';
		}
	}

	debug3("winpat  = '%s'", winpat);

	m_hList = FindFirstFile(winpat, &m_FileData);
	SAFE_FREE(winpat);
	if(m_hList != INVALID_HANDLE_VALUE) {
		debug3("found '%s'", m_FileData.cFileName);
		return true;
	}
	return false;
#else
	m_dir = opendir(m_filepath);
	if(m_dir) {
		for(m_entry = readdir(m_dir); m_entry; m_entry = readdir(m_dir)) {
			if(!m_re || m_re->match(m_entry->d_name)) {
				debug3("found '%s'", m_entry->d_name);
				return true;
			}
		}
	}
	return false;
#endif /*WIN32*/
}


bool FileFind::next()
{
#ifdef WIN32
	if(!FindNextFile(m_hList, &m_FileData)) {
		if(GetLastError() == ERROR_NO_MORE_FILES) {
			return false;
		}
	}
	debug3("found '%s'", m_FileData.cFileName);
	return true;
#else
	if(m_dir && m_entry) {
		for(m_entry = readdir(m_dir); m_entry; m_entry = readdir(m_dir)) {
			if(!m_re || m_re->match(m_entry->d_name)) {
				debug3("found '%s'", m_entry->d_name);
				return true;
			}
		}
	}
	return false;
#endif /*WIN32*/
}


const char *FileFind::filename()
{
#ifdef WIN32
	return m_FileData.cFileName;
#else
	return m_entry->d_name;
#endif /*WIN32*/
}


char *FileFind::fullpath()
{
	char *ret = (char*) malloc(strlen(m_filepath) + strlen(filename()) + 10);
	strcpy(ret, m_filepath);
	strcat(ret, DIR_SEP_STR);
	strcat(ret, filename());
	return ret;
}


bool FileFind::isDirectory()
{
#ifdef WIN32
	return (m_FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false;
#else
	char *fp = fullpath();
	struct stat buf;
	stat(fp, &buf);
	SAFE_FREE(fp);

	if(buf.st_mode & 0x4000) {
		// This is a directory
		return true;
	}
	return false;
#endif /*WIN32*/
}

bool FileFind::isHidden()
{
	#ifdef WIN32
	return (m_FileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ? true : false;
#else
	const char *fn = filename();
	return (fn && fn[0] == '.')?true:false;
#endif /*WIN32*/
}


bool FileFind::isDots()
{
	const char *fn = filename();
	return fn && (fn[0] == '.') && ((fn[1] == '\0')
		|| ((fn[1] == '.') && (fn[2] == '\0'))) ? true : false;
}


void FileFind::close()
{
#ifdef WIN32
	if(m_hList) {
		FindClose(m_hList);
		m_hList = (HANDLE) 0;
	}
#else
	if(m_dir) {
		closedir(m_dir);
		m_dir = NULL;
	}
#endif /*WIN32*/
}
