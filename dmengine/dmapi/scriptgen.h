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
