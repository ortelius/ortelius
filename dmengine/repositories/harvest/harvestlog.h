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
#ifndef __harvestlog_h

#define __harvestlog_h


#include "hashtable.h"
#include "logprocess.h"


class HarvestMessageMatcher : public MessageMatcher
{
private:
	class HarvestMessageTable &m_table;
	char *m_msgkey;

	void compile();

public:
	HarvestMessageMatcher(
		class HarvestMessageTable &table,
		const char *msgkey, const char *msgtext);
	~HarvestMessageMatcher();

	const char *key()  { return m_msgkey; }

	StringHashtable *match(const char *logmsg);
};


class HarvestMessageTable
{
private:
	Hashtable<HarvestMessageMatcher> m_messages;	// keyed by msgnum
	Hashtable<StringHashtable> m_valmap;	// keyed by msgkey, then vp -> s1 etc.

	static HarvestMessageTable *s_instance;

	HarvestMessageTable();
	~HarvestMessageTable();

	void init(class Context &ctx);

public:
	static HarvestMessageTable &instance();

	StringHashtable *lookup(const char *logline, class Context &ctx);
	StringHashtable *getMap(const char *msgkey);
};


class HarvestLogEntry
{
private:
	char *m_logline;
	class Context &m_ctx;
	char *m_msgkey;
	StringHashtable *m_values;

	void lookup();

public:
	HarvestLogEntry(const char *logline, class Context &ctx);
	~HarvestLogEntry();

	bool isInfo();
	bool isWarning();
	bool isError();

	const char *msgkey();

	const char *viewpath();
	const char *mappedversion();
	const char *clientpath();

	int total();
	int success();
	int failed();
	int notProcessed();
};


#endif /*__harvestlog_h*/
