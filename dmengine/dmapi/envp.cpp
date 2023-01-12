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

#include <string.h>
#include <stdio.h>
#ifdef WIN32
#include <malloc.h>
#endif

//#ifdef WIN32
//#include <windows.h>
//#endif /*WIN32*/

#include "envp.h"
#include "platform.h"
#include "scopestack.h"
#include "charptr.h"


/**
 * Default constructor.
 */
Envp::Envp()
{
	m_envp = NULL;

//#ifdef WIN32
//	m_criticalSection = (_RTL_CRITICAL_SECTION*) malloc(sizeof(_RTL_CRITICAL_SECTION));
//	InitializeCriticalSection(m_criticalSection);
//#endif /*WIN32*/
}


/**
 * Builds an environment from our current crop of global variables ready to
 * pass to a sub-process.  We do not add the built-in $$ to the environment.
 */
Envp::Envp(ScopeStack &stack)
{
	m_envp = NULL;

	Scope *global = stack.getScope(GLOBAL_SCOPE);
	if(global && global->m_vars) {
		StringList *vars = global->m_vars->keys();
		if(vars) {
			StringListIterator iter(*vars);

			int count = 0;
			for(const char *name = iter.first(); name; name = iter.next()) {
				if(strcmp(name, "$") != 0) {
					Variable *var = global->get(name);
					if(var && var->isSimple()) { count++; }
				}
			}

			m_envp = (char**) malloc((count+1) * sizeof(char*));
			count = 0;
			//int size = 0;
			for(const char *name = iter.first(); name; name = iter.next()) {
				if(strcmp(name, "$") != 0) {
					Variable *var = global->get(name);
					if(var && var->isSimple()) {
						ConstCharPtr value = var->toString();
						m_envp[count] = (char*) malloc(strlen(name) + strlen(value) + 2);
						sprintf(m_envp[count], "%s=%s", name, (const char*) value);
						count++;
						//size += strlen(name) + strlen(var->value()) + 2;
					}
				}
			}
			m_envp[count] = NULL;
			SAFE_DELETE(vars);

			//printf("size = %d bytes\n", size);
		}
	}
}

/**
 * Copy constructor - make a deep copy - this is thread-safe.
 */
Envp::Envp(Envp &copy)
{
	m_envp = NULL;

//	EnterCriticalSection(copy.m_criticalSection);
	init(copy.m_envp);
//	LeaveCriticalSection(copy.m_criticalSection);

//#ifdef WIN32
//	m_criticalSection = (_RTL_CRITICAL_SECTION*) malloc(sizeof(_RTL_CRITICAL_SECTION));
//	InitializeCriticalSection(m_criticalSection);
//#endif /*WIN32*/
}


/**
 * Destructor.
 */
Envp::~Envp()
{
	if(m_envp) {
		for(char **e = m_envp; *e; e++) {
			SAFE_FREE(*e);
		}
		SAFE_FREE(m_envp);
	}
}


/**
 * Late initializtion method - constructor is too early for this.
 */
void Envp::init(char **envp)
{
	int count = 0;
	for(char **e = envp; e && *e; e++) {
		count++;
	}

	m_envp = (char**) malloc((count+1) * sizeof(char*));
	if (m_envp) {
		for(int n = 0; n < count; n++) {
			//fprintf(stderr, "init: %s\n", envp[n]);
			m_envp[n] = strdup(envp[n]);
		}
		m_envp[count] = NULL;
	}
}


/**
 * Public get method - this is thread-safe.
 */
char *Envp::get(const char *name)
{
//	EnterCriticalSection(m_criticalSection);
	for(char **e = m_envp; e && *e; e++)
	{
		int m;
		for(m = 0; name[m] && (*e)[m] && ((*e)[m] != '=')
			&& (/*tolower*/(name[m]) == /*tolower*/((*e)[m])); m++);
		if((*e)[m] == '=') {
			//fprintf(stderr, "get(%s) = '%s'\n", name, &(*e)[m+1]);
			char *temp = strdup(&(*e)[m+1]);
//			LeaveCriticalSection(m_criticalSection);
			return temp;
		}
	}
//	LeaveCriticalSection(m_criticalSection);
	//fprintf(stderr, "get(%s) = null\n", name);
	return NULL;
}


/**
 * Internal put method - this is thread-safe.
 */
void Envp::internalPut(const char* name, char *buf)
{
//	EnterCriticalSection(m_criticalSection);
	int count = 0;
	for(char **e = m_envp; e && *e; e++)
	{
		count++;
		int m;
		for(m = 0; name[m] && (*e)[m] && ((*e)[m] != '=')
			&& (/*tolower*/(name[m]) == /*tolower*/((*e)[m])); m++);
		if((*e)[m] == '=')
		{
			//fprintf(stderr, "put(%s) %s -> %s\n", name, *e, buf);
			free(*e);
			*e = buf;
//			LeaveCriticalSection(m_criticalSection);
			return;
		}
	}

	//fprintf(stderr, "put(%s) %s\n", name, buf);
	char **m_envp2 = (char**) realloc(m_envp, (count+2) * sizeof(char*));
	if (m_envp2) {
		m_envp = m_envp2;
		m_envp[count] = buf;
		m_envp[count+1] = NULL;
	}

//	LeaveCriticalSection(m_criticalSection);
}


/**
 * Public string put method - this is thread-safe by calling internal put.
 */
void Envp::put(const char* name, const char *value)
{
	char *buf = (char*) malloc(strlen(name) + strlen(value) + 2);
	if (value == NULL)
	 sprintf(buf, "%s=", name);
	else
	 sprintf(buf, "%s=%s", name, value);
	internalPut(name, buf);
}


/**
 * Public integer put method - this is thread-safe by calling internal put.
 */
void Envp::put(const char* name, int value, int digits)
{
	int modulus = 1;
	for(int n = 0; n < digits; n++) {
		modulus *= 10;
	}
	value = value % modulus;

	char fmt[10];
	sprintf(fmt, "%%s=%%0%dd", digits);

	char *buf = (char*) malloc(strlen(name) + digits + 2);
	sprintf(buf, fmt, name, value);

	internalPut(name, buf);
}


/**
 * Public string put method - this is thread-safe by calling internal put.
 * Same as put(const char*, const char*) but prefixes the env var with "TRI_"
 */
void Envp::putTri(const char* name, const char *value)
{
	char *buf = (char*) malloc(strlen(name) + strlen(value) + 6);
	sprintf(buf, "TRI_%s=%s", name, value);
	internalPut(name, buf);
}

/**
 * Public utility function to dump the Envp to stderr
 */
void Envp::dump(const char *filename /* = 0 */)
{
	FILE *f = filename ? fopen(filename, "w") : stderr;

	if(f) {
		for(char **e = m_envp; e && *e; e++) {
			fprintf(f, "%s\n", *e);
		}

		if(filename) { fclose(f); }
	}
}


/**
 * Public accessor to underlying data - used in exec calls.
 */
char **Envp::ptr()
{
	return m_envp;
}
