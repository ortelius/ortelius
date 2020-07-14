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
