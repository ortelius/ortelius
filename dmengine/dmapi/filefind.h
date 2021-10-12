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

#ifndef __filefind_h

#define __filefind_h


#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif /*WIN32*/

/**
 * Platform independant file find class that provides a wrapper around the
 * native platform APIs.
 */
class FileFind
{
private:
	char *m_filepath;
	char *m_pattern;

#ifdef WIN32
	char *m_winpat;
	WIN32_FIND_DATA m_FileData;
	HANDLE m_hList;
#else
	class RegExpWrapper *m_re;
	DIR *m_dir;
	struct dirent *m_entry;
#endif /*WIN32*/

public:
	FileFind(const char *path, const char *pattern);
	~FileFind();

	bool find();
	bool next();

	const char *filename();
	char *fullpath();
	bool isDirectory();
	bool isHidden();
	bool isDots();

	void close();
};

#endif /*__filefind_h*/
