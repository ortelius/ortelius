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

#ifndef __envp_h

#define __envp_h

class Envp
{
private:
	char **m_envp;

#ifdef WIN32
	struct _RTL_CRITICAL_SECTION *m_criticalSection;
#endif /*WIN32*/

	void internalPut(const char* name, char *buf);

public:
	Envp();
	Envp(class ScopeStack &stack);
	Envp(Envp &copy);
	~Envp();
	void init(char **envp);
	char *get(const char *name);
	void put(const char* name, const char *value);
	void put(const char* name, int value, int digits);
	void putTri(const char* name, const char *value);
	void dump(const char *filename = 0);
	char **ptr();
};

#endif /*__environment_h*/
