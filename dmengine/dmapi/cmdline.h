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
