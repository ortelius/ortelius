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

#include "cmdline.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"
#include "autoptr.h"
#include "context.h"
#include "execute.h"


CmdLine::CmdLine(const char *cmd /*= 0*/) {
	m_size = 10;
	m_argv = (char**) malloc((m_size+1) * sizeof(char*));
	if(cmd) {
		m_argv[0] = strdup(cmd);
		m_argv[1] = NULL;
		m_argc = 1;
	} else {
		m_argv[0] = NULL;
		m_argc = 0;
	}
}

CmdLine::~CmdLine() {
	for(int n = 0; n < m_argc; n++) {
		if(m_argv[n]) free(m_argv[n]);
		m_argv[n] = NULL;
	}
	free(m_argv);
}

CmdLine &CmdLine::add(const char *arg) {
	if(m_argc+1 >= m_size) {
		m_size += 10;
		m_argv = (char**) realloc(m_argv, m_size * sizeof(char*));
	}
	int n=strlen(arg);
	char *newarg=(char *)0;
	if (arg[n-1]=='\\') {
		// Last character is escape \ char - escape the escape
		newarg=(char *)malloc(n+5);
		sprintf(newarg,"%s\\",arg);
	}

	m_argv[m_argc] = strdup(newarg?newarg:arg);
	m_argv[m_argc+1] = NULL;
	m_argc++;
	SAFE_FREE(newarg);
	return *this;
}

CmdLine &CmdLine::add(const StringList &args) {
	StringListIterator iter(args);
	for(const char *arg = iter.first(); arg; arg = iter.next()) {
		add(arg);
	}
	return *this;
}

char *CmdLine::toCommandString(bool forpopen /*=false*/) {
	int len = 0;
	int n;

	for(n = 0; n < m_argc; n++) {
		//debug3("m_argv[%d] = '%s' strlen(m_argv[%d]) = %d", n, m_argv[n], n, strlen(m_argv[n]));
		len += strlen(m_argv[n]) + 3;
	}
	//debug3("len = %d", len);

	char *ret = (char*) malloc(len + 10);
	*ret = '\0';

#ifdef WIN32
	// Ugly hack to make Windows handle commands and params with spaces
	if(forpopen) {
		strcat(ret, "\"");
	}
#endif /*WIN32*/

	for(n = 0; n < m_argc; n++) {
		if(!strchr(m_argv[n], '"')) {
			strcat(ret, "\"");
		}
		strcat(ret, m_argv[n]);
		if(!strchr(m_argv[n], '"')) {
			strcat(ret, "\"");
		}
		if(n+1 < m_argc) {
			strcat(ret, " ");
		}
	}

#ifdef WIN32
	// Ugly hack to make Windows handle commands and params with spaces
	if(forpopen) {
		strcat(ret, "\"");
	}
#endif /*WIN32*/

	return ret;
}

char **CmdLine::toArgv() {
	return m_argv;
}


/**
 * Execute using standard execute and capture, with output being allowed to go
 * to the context, but not being captured in a buffer.
 */
int CmdLine::exec(Context &ctx)
{
	CapturedData* cd = NULL;
	int tempExitStatus = -1;
	int ret = executeAndCapture(ctx.stream(), *this, NULL, true, ctx.threadId(), &tempExitStatus, &cd, NULL);
	SAFE_DELETE(cd);
	if(ret) {
		//ctx.dm().writeToStdOut("trilogy job failed to execute");
		return -1;
	}
	return tempExitStatus;
}


/**
 * Execute using standard execute and capture, with output being captured into a buffer.
 */
int CmdLine::exec(CapturedData **cd, Context &ctx)
{
	int tempExitStatus = -1;
	int ret = executeAndCapture(ctx.stream(), *this, NULL, false, ctx.threadId(), &tempExitStatus, cd, NULL);
	if(ret) {
		//ctx.dm().writeToStdOut("trilogy job failed to execute");
		return -1;
	}
	return tempExitStatus;
}
