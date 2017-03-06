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
#ifndef __cmdline_h

#define __cmdline_h


#include "dmapiexport.h"


class DMAPI_API CmdLine
{
private:
	char **m_argv;
	int m_argc;
	int m_size;

public:
	CmdLine(const char *cmd = 0);
	~CmdLine();
	CmdLine &add(const char *arg);
	CmdLine &add(const class StringList &args);
	char *toCommandString(bool forpopen = false);
	char **toArgv();
	int exec(class Context &ctx);
	int exec(class CapturedData **cd, class Context &ctx);
};


#endif /*__cmdline_h*/
