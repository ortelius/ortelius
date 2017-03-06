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
#include <stdio.h>

#include "harvestlog.h"

#include "exceptions.h"
#include "autoptr.h"
#include "charptr.h"
#include "node.h"
#include "expr.h"
#include "context.h"
#include "scopestack.h"


typedef struct tagMessageInfo {
	const char *msgkey;
	const char *s1, *s2, *s3, *s4;
	const char *d1, *d2, *d3, *d4;
} MessageInfo;

#define VPMV "vp,mv:(.*);([0-9.]*) ?"
#define CP   "cp:(.*[^ ]) ?"
#define RMCP "rm,cp:(.*)\\\\\\\\(.*[^ ]) ?"
#define TO   "to:([0-9]*) ?"
#define SU   "su:([0-9]*) ?"
#define FA   "fa:([0-9]*) ?"
#define NP   "np:([0-9]*) ?"

MessageInfo s_msgs[] = {
	// msgkey                      s1    s2    s3    s4    d1    d2    d3    d4         msgnum
	{ "HSRV_I_CHECKOUT",		   VPMV, RMCP, NULL, NULL, NULL, NULL, NULL, NULL }, // 00020110
	{ "HSRV_I_CHECKIN",            CP,   VPMV, NULL, NULL, NULL, NULL, NULL, NULL }, // 00020100
	{ "HSRV_I_CHECKIN_NO_CHANGES", CP,   NULL, NULL, NULL, NULL, NULL, NULL, NULL }, // 00020144
	{ "HAPI_I_CHECKIN_SUMMARY",    NULL, NULL, NULL, NULL, TO,   SU,   FA,   NP   }, // 00060079
	{ "HAPI_I_CHECKOUT_SUMMARY",   NULL, NULL, NULL, NULL, TO,   SU,   FA,   NP   }, // 00060080
	{ "CLIN_I_CHECKOUT_SUMMARY",   NULL, NULL, NULL, NULL, TO,   SU,   FA,   NP   }, // 000a0080
	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};


///////////////////////////////////////////////////////////////////////////////
// HarvestMessageMatcher
///////////////////////////////////////////////////////////////////////////////

HarvestMessageMatcher::HarvestMessageMatcher(
		HarvestMessageTable &table, const char *msgkey, const char *msgtext
	)
	: MessageMatcher(msgtext), m_table(table), m_msgkey(DUP_NULL(msgkey))
{}


HarvestMessageMatcher::~HarvestMessageMatcher()
{
	SAFE_FREE(m_msgkey);
}


StringHashtable *HarvestMessageMatcher::match(const char *logmsg)
{ 
	if(!m_re) {
		compile();
	}

	return MessageMatcher::match(logmsg);
}


/**
 * Compile a regexp that will match the given message text.  We replace '%s<n>'
 * and '%d<n>' with substring regexps that will match the values. e.g.
 *
 *   "File %s1 checked out to %s2 ."
 *
 * becomes:
 *
 *   "File (.*) checked out to (.*) \."
 *
 * and the map contains:
 *
 *   '1' => 's1'
 *   '2' => 's2'
 *
 * For more complex strings we can also specify a regexp in another map that
 * can divide values like 's1' into two or more strings.
 *
 *   's1' => 'vp,mv:(.*);([0-9.]*) ?'
 *
 * will produce an re like:
 *
 *   "File (.*);([0-9.]*) ? checked out to (.*) \."
 *
 * and the map contains:
 *
 *   '1' => 'vp'
 *   '2' => 'mv'
 *   '3' => 's2'
 */
/*private*/ void HarvestMessageMatcher::compile()
{
	StringHashtable *map = m_table.getMap(m_msgkey);

	CharPtr buf = (char*) malloc(2*strlen(m_msgtext));
	char *y = buf;
	int renum = 0;
	for(char *x = m_msgtext; x && *x; x++) {
		if(*x == '%') {
			char *t = x + 1;
			if(*t && (*t == 's' || *t == 'd')) {
				char *n = t + 1;
				if(*n && isdigit(*n)) {
					CharPtr valpat;
					const char *re = NULL;
					switch(*t) {
					case 's': re = "(.*)"; break; 
					case 'd': re = "([0-9]*)"; break;
					}
					if(re) {
						char val[3] = { *t, *n , '\0' };
						if(map && map->get(val)) {
							// use map's re instead
							valpat = strdup(map->get(val));
							char *colon = strchr((char*) valpat, ':');
							if(colon) {
								*colon = '\0';
								re = colon + 1;
								char *temp = NULL;
								for(char *key = STRTOK_R(valpat, ",", &temp); key; key = STRTOK_R(NULL, ",", &temp)) {
									addMapping(++renum, key);
									debug3("map %d => %s", renum, key);
								}
							}
						} else {
							addMapping(++renum, val);
							debug1("map %d => %s", renum, val);
						}
						*y = '\0';
						strcat(y, re);
						y += strlen(re);
						x = n;
						continue;
					}
				}
			}
		} else if((*x == '.') || (*x == '(') || (*x == ')')
			|| (*x == '[') || (*x == '*') || (*x == '+')
			|| (*x == '?') || (*x == '^') || (*x == '$')
			|| (*x == '|')) {
			// Escape these chars
			*y++ = '\\';
		} else if((*x == 0x0a) || (*x == 0x0d)) {
			// Skip these
			continue;
		}
		*y++ = *x;
	}
	*y = '\0';

	debug3("re = '%s'", (const char*) buf);
	setRegexp(buf); // may raise RuntimeError if invalid RE
}


///////////////////////////////////////////////////////////////////////////////
// HarvestMessageTable
///////////////////////////////////////////////////////////////////////////////

/*static*/ HarvestMessageTable *HarvestMessageTable::s_instance = NULL;


HarvestMessageTable::HarvestMessageTable()
	: m_messages(false, true), m_valmap(false, true)
{}


HarvestMessageTable::~HarvestMessageTable()
{}


void HarvestMessageTable::init(Context &ctx)
{
	char *hh = getenv("CA_SCM_HOME");
	if(!hh) { 
		hh = getenv("HARVESTHOME");
	}
	if(!hh) {
		throw RuntimeError("CA_SCM_HOME or HARVESTHOME not set");
	}

	// Run harvest_messagetable() to get the message table data
	// This returns an array of arrays with the outer array keyed by msgkey
	// and the inner array keyed by 's1', 's2', 'd1' etc.
	try {
		YYLTYPE loc = { 0, 0, 0, 0, 0 };
		FunctionNode fn(loc, strdup("harvest_messagetable"), NULL);
		ExprPtr result = fn.evaluate(ctx);
		DMArray *arr = result->toArray();
		if(arr) {
			AutoPtr<StringList> msgkeys = arr->keys();
			StringListIterator mkit(*msgkeys);
			for(const char *msgkey = mkit.first(); msgkey; msgkey = mkit.next()) {
				//printf("%s:\n", msgkeyscan);
				Variable *ventry = arr->get(msgkey);
				DMArray *entry = ventry ? ventry->getArray() : NULL;
				if(entry) {
					AutoPtr<StringList> keys = entry->keys();
					StringHashtable *map = new StringHashtable();
					StringListIterator ekit(*keys);
					for(const char *key = ekit.first(); key; key = ekit.next()) {
						Variable *vinfo = entry->get(key);
						ConstCharPtr info = (vinfo ? vinfo->toString() : NULL);
						if(info) {
							//printf("%s => %s\n", key, info);
							map->put(key, info);
						}
					}
					m_valmap.put(msgkey, map);
				}
			}
		}
	} catch(DMException &e) {
		ctx.writeToStdOut("WARNING: failed to parse 'harvest_messagetable', using fallback");
	}

	if(m_valmap.count() == 0) {
		// Build up the list of messages we want and how the values are mapped
		for(MessageInfo *info = s_msgs; info && info->msgkey; info++) {
			StringHashtable *map = new StringHashtable();
			if(info->s1) { map->put("s1", info->s1); }
			if(info->s2) { map->put("s2", info->s2); }
			if(info->s3) { map->put("s3", info->s3); }
			if(info->s4) { map->put("s4", info->s4); }
			if(info->d1) { map->put("d1", info->d1); }
			if(info->d2) { map->put("d2", info->d2); }
			if(info->d3) { map->put("d3", info->d3); }
			if(info->d4) { map->put("d4", info->d4); }
			m_valmap.put(info->msgkey, map);
		}
	}

	char msgfile[1024];
	sprintf(msgfile, "%s%shenglish.msg", hh, DIR_SEP_STR);

	char buf[2048];
	FILE *in = fopen(msgfile, "r");
	if(!in) {
		throw RuntimeError("Unable to open message file '%s'", msgfile);
	}
	while(fgets(buf, sizeof(buf), in)) {
		if((buf[0] == '/') && (buf[1] == '/')) { continue; } // comment

		char *num = strchr(buf, ' ');
		if(num) {
			*num = '\0';
			++num;

			// is this a message we are interested in?
			if(m_valmap.get(buf)) {
				if((num[0] == '0') && (num[1] == 'x')) {
					num += 2;
				}
				char *text = strchr(num, ' ');
				if(text) {
					*text = '\0';
					++text;

					m_messages.put(num, new HarvestMessageMatcher(*this, buf, text));
				}
			}
		}
	}
	fclose(in);
}


/*static*/ HarvestMessageTable &HarvestMessageTable::instance()
{
	if(!s_instance) {
		s_instance = new HarvestMessageTable();
	}
	return *s_instance;
}


StringHashtable *HarvestMessageTable::lookup(const char *logline, Context &ctx)
{
	if(m_messages.count() == 0) {
		init(ctx);
	}

	if((strlen(logline) > 11) && (logline[9] == ':')) {
		char *buf = strdup(logline);
		buf[9] = '\0';

		debug3("msgnum is '%s'", (buf + 1));
		
		HarvestMessageMatcher *msg = m_messages.get(buf + 1);
		if(msg) {
			StringHashtable *ret = msg->match(buf + 11);
			if(ret) {
				ret->put("msgkey", msg->key());
			}
			return ret;
		} else {
			debug1("unknown msgnum '%s'", (buf + 1));
		}
	}
	return NULL;
}


StringHashtable *HarvestMessageTable::getMap(const char *msgkey)
{
	return m_valmap.get(msgkey);
}


///////////////////////////////////////////////////////////////////////////////
// HarvestLogEntry
///////////////////////////////////////////////////////////////////////////////

HarvestLogEntry::HarvestLogEntry(const char *logline, Context &ctx)
	: m_logline(DUP_NULL(logline)), m_ctx(ctx), m_msgkey(NULL), m_values(NULL)
{}


HarvestLogEntry::~HarvestLogEntry()
{
	SAFE_FREE(m_logline);
	SAFE_FREE(m_msgkey);
	SAFE_DELETE(m_values);
}


bool HarvestLogEntry::isInfo()
{
	return m_logline && (strlen(m_logline) > 11)
		&& (m_logline[9] == ':') && (m_logline[0] == 'I') ? true : false;
}


bool HarvestLogEntry::isWarning()
{
	return m_logline && (strlen(m_logline) > 11)
		&& (m_logline[9] == ':') && (m_logline[0] == 'W') ? true : false;
}


bool HarvestLogEntry::isError()
{
	return m_logline && (strlen(m_logline) > 11)
		&& (m_logline[9] == ':') && (m_logline[0] == 'E') ? true : false;
}


const char *HarvestLogEntry::msgkey()
{
	if(!m_msgkey) {
		lookup();
	}
	return m_msgkey;
}


const char *HarvestLogEntry::viewpath()
{
	if(!m_msgkey) {
		lookup();
	}
	return m_values ? m_values->get("vp") : NULL;
}


const char *HarvestLogEntry::mappedversion()
{
	if(!m_msgkey) {
		lookup();
	}
	return m_values ? m_values->get("mv") : NULL;
}


const char *HarvestLogEntry::clientpath()
{
	if(!m_msgkey) {
		lookup();
	}
	return m_values ? m_values->get("cp") : NULL;
}


int HarvestLogEntry::total()
{
	if(!m_msgkey) {
		lookup();
	}
	const char *val = m_values ? m_values->get("to") : NULL;
	return val ? atoi(val) : -1;
}


int HarvestLogEntry::success()
{
	if(!m_msgkey) {
		lookup();
	}
	const char *val = m_values ? m_values->get("su") : NULL;
	return val ? atoi(val) : -1;
}


int HarvestLogEntry::failed()
{
	if(!m_msgkey) {
		lookup();
	}
	const char *val = m_values ? m_values->get("fa") : NULL;
	return val ? atoi(val) : -1;
}


int HarvestLogEntry::notProcessed()
{
	if(!m_msgkey) {
		lookup();
	}
	const char *val = m_values ? m_values->get("np") : NULL;
	return val ? atoi(val) : -1;
}


/*private*/ void HarvestLogEntry::lookup()
{
	debug3("lookup('%s')", m_logline);
	m_values = HarvestMessageTable::instance().lookup(m_logline, m_ctx);
	if(m_values) {
		m_msgkey = DUP_NULL(m_values->get("msgkey"));
		debug3("HarvestLogEntry msgkey = %s", (m_msgkey ? m_msgkey : "(null)"));
	}
}
