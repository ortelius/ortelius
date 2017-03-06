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
