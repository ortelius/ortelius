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
#ifndef __scriptgen_h

#define __scriptgen_h


#include "hashtable.h"


class GraphicalScriptGenerator
{
private:
	class triODBC &m_odbc;
	class Action &m_action;
	char *m_filename;
	HashtableById<class Fragment> *m_fragCache;
	HashtableById<class ActionFragment> *m_frags;
	HashtableById<class ActionFragment> *m_starts;

	class Fragment *internalGetFragment(int fragtypeid);
	void internalGetActionFragments();
	void reset();

public:
	GraphicalScriptGenerator(class triODBC &odbc, class Dropzone &dz, class Action &action);
	~GraphicalScriptGenerator();

	const char *filename()  { return m_filename; }

	bool generate();
};


#endif /*__scriptgen_h*/
