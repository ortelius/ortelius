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
