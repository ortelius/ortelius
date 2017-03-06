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

#include "scriptgen.h"

#include "model.h"
#include "dropzone.h"
#include "charptr.h"
#include "rewrap.h"
#include "regexp.h"
#include "exceptions.h"
#include "triodbc.h"

#ifndef WIN32
#define stricmp strcasecmp
#endif


typedef enum tagTemplateType {
	TemplateType_Default      = 0,
	TemplateType_If_LeftRight = 1,
	TemplateType_If_LeftOnly  = 2,
	TemplateType_If_RightOnly = 3,
	TemplateType_Loop_Left    = 4,
	TemplateType_Loop_Right   = 5
} TemplateType;


#define LEFT_LINK  1			// @1@
#define RIGHT_LINK 2			// @2@
#define DOWN_LINK  0			// @0@

#define NEXT_POS  1				// Action editor records next as 1
#define LEFT_POS  3				// Action editor records false branch as 3
#define RIGHT_POS 2				// Action editor records false branch as 2
#define DOWN_POS  9				// Drilldown has the special link number 9


class FragmentTemplate
{
private:
	TemplateType m_tpltype;
	char *m_text;

public:
	FragmentTemplate(TemplateType tpltype, const char *text);
	~FragmentTemplate();

	friend class Fragment;
};


class Fragment
{
private:
	int m_typeid;
	char *m_name;
	int m_exitPoints;
	Action *m_action;
	HashtableById<FragmentTemplate> m_templates;

	FragmentTemplate *getTemplate(TemplateType tpltype);

public:
	Fragment(int fragtypeid, const char *name, int exitPoints);
	~Fragment();

	int id()            { return m_typeid; }
	const char *name()  { return m_name; }
	int exitPoints()    { return m_exitPoints; }

	Action *getAction()  { return m_action; }
	void setAction(Action *action)  { m_action = action; }

	void addTemplate(TemplateType tpltype, const char* text);

	void generate(
		FILE *out, int indent, int windowid,
		Hashtable<class ActionFragmentAttribute> &attrs,
		class ActionFragment *drilldown, class ActionFragment *left,
		class ActionFragment *right, TemplateType tpltype);
};


class ActionFragmentAttribute
{
private:
	char *m_name;
	char *m_value;
	bool m_notDefined;
	bool m_boolValue;
	bool m_isBool;

public:
	ActionFragmentAttribute(const char *name, const char *value, bool notDefined);
	ActionFragmentAttribute(const char *name, bool value, bool notDefined);
	~ActionFragmentAttribute();

	const char *name()   { return m_name; }
	const char *value()  { return m_value; }
	bool isNotDefined()  { return m_notDefined; }
	bool isBool()        { return m_isBool; }
	bool boolValue()     { return m_boolValue; }
};


class ActionFragment
{
private:
	int m_windowid;
	Fragment &m_fragtype;
	HashtableById<ActionFragment> m_linksOut;
	List<ActionFragment> m_linksIn;
	Hashtable<ActionFragmentAttribute> m_attrs;

	int m_isStart;	// set to 1 to indicate start and added to links in count
	int m_level;
	int m_visit;
	bool m_exitPoint;
	bool m_joinAfter;
	bool m_suppressJoin;
	bool m_loopStart;
	bool m_loopEnd;
	bool m_loopLeft;
	bool m_loopRight;

	ActionFragment* m_down;
	ActionFragment* m_next;
	ActionFragment* m_left;
	ActionFragment* m_right;
	ActionFragment *m_join;		// On an if, the next node outside the if
	ActionFragment *m_cond;		// On a join, the condition it is associated with

	ActionFragment *joinVisit(int id, bool isLeft);

public:
	ActionFragment(int windowid, Fragment &fragtype);
	~ActionFragment();

	int windowid()        { return m_windowid; }
	int fragtypeid()      { return m_fragtype.id(); }
	bool isConditional()  { return (m_fragtype.exitPoints() > 1) ? true : false; }
	bool isJoin()         { return ((m_isStart + m_linksIn.size()) > 1) ? true : false; }

	void setIsStart()     { m_isStart = 1; }
	void setLevel(int level)  { if(!m_level || (level < m_level)) { m_level = level; } }

	void addLink(int pos, ActionFragment *to);
	void addAttribute(const char *attrname, const char *value);
	void addAttribute(const char *attrname, bool value);
	void addUndefinedAttribute(const char *attrname);
	bool hasAttribute(const char *attrname);

	void pathCalc(class ActionPath *path, bool isDrillDown);
	void findJoin();
	void reset();

	void generate(FILE *out, int indent);

	friend class ActionPath;
	friend class ActionPathSet;
};


class ActionPathEntry
{
private:
	ActionFragment &m_actfrag;
	int m_pos;

public:
	ActionPathEntry(ActionFragment &actfrag, int pos);
	~ActionPathEntry();

	int id()   { return m_actfrag.windowid(); }
	int pos()  { return m_pos; }
	ActionFragment &actfrag()  { return m_actfrag; }
};


class ActionPath
{
private:
	class ActionPathSet &m_set;
	HashtableById<ActionPathEntry> m_visited;
	List<ActionPathEntry> m_path;
	bool m_isLoop;
	ActionPathEntry *m_last;
	int m_ifCount;

public:
	ActionPath(ActionPathSet &set);
	ActionPath(const ActionPath &copy);
	~ActionPath();

	void add(ActionFragment &actfrag, int pos);
	bool contains(ActionFragment &actfrag);

	void markAsLoop()       { m_isLoop = true; }
	bool isLoop()           { return m_isLoop; }
	ActionPathEntry *last()  { return m_last; }
	ActionPathEntry *prev(ActionPathEntry *frag);
	ActionPathEntry *get(ActionFragment &actfrag);

	ListIterator<ActionPathEntry> *iterator()  { return new ListIterator<ActionPathEntry>(m_path); }

	void dump();
};


class ActionPathSet
{
private:
	List<ActionPath> m_paths;

public:
	ActionPathSet();
	~ActionPathSet();
	void add(ActionPath *path);
	bool checkExitPoint();

	void dump();
};


void indent(FILE *out, int ind) {
	for(int n = 0; n < ind; n++) {
		fprintf(out, "  ");
	}
}


///////////////////////////////////////////////////////////////////////////////
// FragmentTemplate
///////////////////////////////////////////////////////////////////////////////

FragmentTemplate::FragmentTemplate(TemplateType tpltype, const char *text)
	: m_text(DUP_NULL(text))
{}


FragmentTemplate::~FragmentTemplate()
{
	SAFE_FREE(m_text);
}


///////////////////////////////////////////////////////////////////////////////
// Fragment
///////////////////////////////////////////////////////////////////////////////

Fragment::Fragment(int fragtypeid, const char *name, int exitPoints)
	: m_typeid(fragtypeid), m_name(DUP_NULL(name)), m_exitPoints(exitPoints),
	  m_action(NULL), m_templates(true)
{}


Fragment::~Fragment()
{
	SAFE_FREE(m_name);
	m_action = NULL;		// owned by cache
}


void Fragment::addTemplate(TemplateType tpltype, const char* text)
{
	m_templates.put((int) tpltype, new FragmentTemplate(tpltype, text));
}


/**
 * Gets the best match template for the given template type or returns NULL.
 */
FragmentTemplate *Fragment::getTemplate(TemplateType tpltype)
{
	FragmentTemplate *ret = m_templates.get((int) tpltype);
	if(ret) {
		return ret;
	}

	switch(tpltype) {
	case TemplateType_If_LeftRight:
	case TemplateType_Loop_Left:
	case TemplateType_Loop_Right:
		return getTemplate(TemplateType_Default);
	case TemplateType_If_LeftOnly:
	case TemplateType_If_RightOnly:
		 return getTemplate(TemplateType_If_LeftRight);
	}

	return NULL;
}


/**
 * Generates output from the template given a set of attribute values, optional
 * left and right branches and a template type.
 * 1. Find the closest match for the template
 * 2. Replace all the attribute values of the form @attr@ with their values
 * 3. Write the output a line at a time:
 *  a. #if 1 or #if 2 are used to detect if a branch is present
 *  b. #if attr is used to detect if an attribute has a value
 *  c. #endif closes either a. or b. above
 *  d. @1@ or @2@ cause the generator to recurse into the left and right branches
 *  e. all other lines are output verbatim
 * d. and e. are suppressed if we are in an if that is false
 */
void Fragment::generate(
	FILE *out, int ind, int windowid, Hashtable<ActionFragmentAttribute> &attrs,
	ActionFragment *drilldown, ActionFragment *left, ActionFragment *right, TemplateType tpltype)
{
	if(m_action) {
		// Direct action call
		AutoPtr<StringList> keys = attrs.keys();
		StringListIterator iter(*keys);
		int count = 0;
		for(const char *attrname = iter.first(); attrname; attrname = iter.next()) {
			ActionFragmentAttribute *attr = attrs.get(attrname);
			if(attr && !attr->isNotDefined()) {
				count++;
			}
		}

		if(count > 0) {
			indent(out, ind); fprintf(out, "%s(", m_action->name());
			count = 0;
			for(const char *attrname = iter.first(); attrname; attrname = iter.next()) {
				ActionFragmentAttribute *attr = attrs.get(attrname);
				if(attr && !attr->isNotDefined()) {
					if (attr->isBool()) {
						fprintf(out, "%s%s: %s", ((count > 0) ? ", " : ""), attrname, attr->boolValue()?"true":"false");
					} else {
						fprintf(out, "%s%s: @\"%s\"", ((count > 0) ? ", " : ""), attrname, attr->value());
					}
					count++;
				}
			}
			fprintf(out, ");\n");
		} else {
			indent(out, ind); fprintf(out, "%s;\n", m_action->name());
		}
	} else {
		// Find the best template
		FragmentTemplate *tpl = getTemplate(tpltype);
		if(!tpl) {
			fprintf(stderr,"ERROR: Fragment %d has no suitable template associated with it", windowid);
			return;
		}

		CharPtr text = strdup(tpl->m_text);
		if(!text) {
			fprintf(stderr,"ERROR: Fragment %d has no text associated with its template", windowid);
			return;
		}

		// Replace any attribute values

		AutoPtr<StringList> keys = attrs.keys();
		StringListIterator iter(*keys);
		for(const char *attrname = iter.first(); attrname; attrname = iter.next()) {
			debug3("text=\"%s\"",(char *)text);
			ActionFragmentAttribute *attr = attrs.get(attrname);

			debug3("Looking for attr name \"%s\"",attrname);

			// Find the placeholder here. Any spaces will be replace by underscores. The
			// placeholder must be in ALL lowercase.
			// So an attribute name of:
			// @MY Attribute@
			// will become
			// @my_attribute@.
			// Thus the template text must be assembled as shown above.
			//
			char search[256];
			sprintf(search, "@%s@", attrname);
			for(char *x = search; (x && *x); x++) {
				if(*x == ' ') {
					*x = '_';
				} else {
					*x = tolower(*x);
				}
			}

			debug3("search=\"%s\"",search);

			RegExpWrapper re(search);
			// Make any "auto changes" here.
			// Add Quotes if no quotes wrapped
			if (attr->isBool()) {
				text = re.replaceAll(text, attr->boolValue()?"true":"false");
			} else {
				text = re.replaceAll(text, attr->value());
			}
		}

		// match an entire line containing @1@ or @2@
		RegExpWrapper linkre("^[ \t]*@([012])@[ \t]*$");
		// match preprocessing instructions #if link, #if attr and #endif
		RegExpWrapper iflinre("^#if[ \t]*([12])[ \t]*$");
		RegExpWrapper ifattre("^#if[ \t]*([A-Za-z0-9_][A-Za-z0-9_]*)[ \t]*$");
		RegExpWrapper endifre("^#endif[ \t]*$");
		// match unexpanded attrs in output text
		RegExpWrapper unexatt("@([A-Za-z0-9_][A-Za-z0-9_]*)@");
		bool suppress = false;
		char *temp = NULL;

		for(char *line = STRTOK_R(text, "\r\n", &temp); line; line = STRTOK_R(NULL, "\r\n", &temp)) {
			if(iflinre.match(line)) {
				ConstCharPtr linknum = iflinre.getMatch(1);
				int link = atoi(linknum);
				suppress = (((link == LEFT_LINK) && !left) || ((link == RIGHT_LINK) && !right)) ? true : false;
			} else if(ifattre.match(line)) {
				ConstCharPtr attname = ifattre.getMatch(1);
				ActionFragmentAttribute *attr = attrs.get(attname);
				suppress = (!attr || attr->isNotDefined()) ? true : false;
			} else if(endifre.match(line)) {
				suppress = false;
			} else if(!suppress && linkre.match(line)) {
				//debug1("'%s' matches", line);
				ConstCharPtr linknum = linkre.getMatch(1);
				int link = atoi(linknum);
				ActionFragment *linkfrag = NULL;	// = (link == 1) ? left : ((link == 2) ? right : NULL);
				switch(link) {
				case DOWN_LINK:  linkfrag = drilldown; break;
				case LEFT_LINK:  linkfrag = left; break;
				case RIGHT_LINK: linkfrag = right; break;
				}
				if(linkfrag) {
					linkfrag->generate(out, (line[0] == ' ') ? ind+1 : ind);
					// fprintf(out, "#frag %d\n", windowid);
				}
			} else if(!suppress) {
				if(unexatt.match(line)) {
					debug1("WARNING: Possible unexpanded attribute '%s' in output", unexatt.getMatch(1));
				}
				indent(out, ind); fprintf(out, "%s\n", line);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// ActionFragmentAttribute
///////////////////////////////////////////////////////////////////////////////

ActionFragmentAttribute::ActionFragmentAttribute(
		const char *name, const char *value, bool notDefined
	)
	: m_name(DUP_NULL(name)), m_value(DUP_NULL(value)), m_notDefined(notDefined), m_boolValue(false), m_isBool(false)
{}

ActionFragmentAttribute::ActionFragmentAttribute(
		const char *name, bool value, bool notDefined
	)
	: m_name(DUP_NULL(name)), m_value(NULL), m_notDefined(notDefined), m_boolValue(value), m_isBool(true)
{}


ActionFragmentAttribute::~ActionFragmentAttribute()
{
	SAFE_FREE(m_name);
	SAFE_FREE(m_value);
}


///////////////////////////////////////////////////////////////////////////////
// ActionFragment
///////////////////////////////////////////////////////////////////////////////

ActionFragment::ActionFragment(int windowid, Fragment &fragtype)
	: m_windowid(windowid), m_fragtype(fragtype), m_linksIn(false),
	  m_linksOut(false), m_attrs(false, true), m_isStart(0), m_level(0),
	  m_visit(0), m_exitPoint(false), m_joinAfter(false), m_suppressJoin(false),
	  m_loopStart(false), m_loopEnd(false), m_loopLeft(false), m_loopRight(false),
	  m_down(NULL), m_next(NULL), m_left(NULL), m_right(NULL), m_join(NULL), m_cond(NULL)
{
	debug2("ActionFragment(%d, '%s')", m_windowid, fragtype.name());
}


ActionFragment::~ActionFragment()
{}


void ActionFragment::addLink(int pos, ActionFragment *to)
{
	ActionFragment *frag = m_linksOut.get(pos);
	if(frag) {
		if(frag == to) {
			debug1("Link out: %d -> %d (pos %d) - DUPLICATE", m_windowid, to->windowid(), pos);
		} else {
			debug1("Link out: %d -> %d (pos %d) - ERROR - DIFFERENT NODES", m_windowid, to->windowid(), pos);
		}
	} else {
		debug2("Link out: %d -> %d (pos %d)", m_windowid, to->windowid(), pos);
		m_linksOut.put(pos, to);
		to->m_linksIn.add(this);
	}
}


void ActionFragment::addAttribute(const char *attrname, const char *attrvalue)
{
	m_attrs.put(attrname, new ActionFragmentAttribute(attrname, attrvalue, false));
}

void ActionFragment::addAttribute(const char *attrname, bool attrvalue)
{
	m_attrs.put(attrname, new ActionFragmentAttribute(attrname, attrvalue, false));
}


void ActionFragment::addUndefinedAttribute(const char *attrname)
{
	m_attrs.put(attrname, new ActionFragmentAttribute(attrname, "", true));
}


bool ActionFragment::hasAttribute(const char *attrname)
{
	return m_attrs.get(attrname) ? true : false;
}


void ActionFragment::pathCalc(ActionPath *path, bool isDrillDown)
{
	// If we have already visited this node in this path, then we have a loop
	if(path->contains(*this)) {
		debug1("pathCalc: loop detected at %d", m_windowid);
		//path->dump();
		path->markAsLoop();
		if(!isJoin()) {
			throw RuntimeError("pathCalc: ERROR: fragment %d is NOT a join", m_windowid);
		}
		if(isConditional()) {
			ActionPathEntry *entry = path->get(*this);
			if(!entry) {
				throw RuntimeError("pathCalc: ERROR fragment %d can't find path entry", m_windowid);
			}
			switch(entry->pos()) {
			case LEFT_POS: 
				m_loopLeft = true;
				debug1("pathCalc: loop goes down left branch at %d", m_windowid);
				break;
			case RIGHT_POS:
				m_loopRight = true;
				debug1("pathCalc: loop goes down right branch at %d", m_windowid);
				break;
			default:
				debug1("pathCalc: ERROR: can't identify path %p for loop at %d", path, m_windowid);
				break;
			}
		}
		// Mark this node as the start of the loop
		m_loopStart = true;
		// Mark the last node in the path as the end of the loop
		ActionPathEntry *last = path->last();
		if(!last) {
			throw RuntimeError("pathCalc: ERROR fragment %d can't find end of path", m_windowid);
		}
		last->actfrag().m_loopEnd = true;
		return;
	}

	// This code is tied to one or two links out
	m_down  = m_linksOut.get(DOWN_POS);
	m_next  = m_linksOut.get(NEXT_POS);
	m_left  = m_linksOut.get(LEFT_POS);
	m_right = m_linksOut.get(RIGHT_POS);

	// This code will handle N links out
	AutoPtr<IntList> links = m_linksOut.keys();
	IntListIterator iter(*links);
	int count = m_linksOut.count(); 
	List<ActionPath> paths(false);
	for(int pos = iter.first(); iter.more(); pos = iter.next()) {
		ActionFragment *frag = m_linksOut.get(pos);
		--count;

		if(pos == DOWN_POS) {
			debug1("pathCalc: Drilldown at %d", windowid());
			ActionPathSet pathsetdd;
			ActionPath *pathdd = new ActionPath(pathsetdd);
			frag->pathCalc(pathdd, true);
			pathsetdd.dump();
			continue;
		}

		// Last child gets existing path, other children get a copy
		ActionPath *path2 = (count == 0) ? path : new ActionPath(*path);
		paths.add(path2);
		path2->add(*this, pos);
		frag->pathCalc(path2, isDrillDown);
	}

	if((paths.size() == 0) && !isDrillDown) {
		// No paths out, so completed path
		debug1("pathCalc: exit point found at %d", m_windowid);
		m_exitPoint = true;
		path->add(*this, 0);

		// Path does not contain a loop - mark all nodes before joins as join after
		AutoPtr<ListIterator<ActionPathEntry> > iter(path->iterator());
		for(ActionPathEntry *entry = iter->first(); entry; entry = iter->next()) {
			if(entry->actfrag().isJoin()) {
				ActionPathEntry *prev = path->prev(entry);
				if(prev) {
					if(!prev->actfrag().isConditional()) {
						prev->actfrag().m_joinAfter = true;
					} else {
						debug1("pathCalc: join at %d has if immediately before", entry->id());
					}
				} else {
					debug1("pathCalc: fragment %d has no prev", entry->id());
				}
			}
		}

		path->dump();
	}
}


/**
 * Navigates the tree looking for the join point between left and right branches.
 * If isLeft is true, we mark the tree as visited.  If isLeft is false, we
 * navigate the tree looking for nodes visited by the earlier marking.
 * Nested ifs are simply treated as points where we need to navigate both
 * sides.
 */
ActionFragment *ActionFragment::joinVisit(int id, bool isLeft)
{
	if(m_loopEnd) {
		debug1("join visit reached end of loop at %d", m_windowid);
		return NULL;
	}

	if(isLeft) {
		if(m_visit) {
			debug1("join visit already visited %d", m_windowid);
		}
		m_visit = id;
	} else {
		if(m_visit) {
			if(id != m_windowid) {
				debug1("join at %d from %d", m_windowid, id);
				return this;
			}
			// if we got loopEnd right, this should never be called
			debug1("loop back to self at %d", m_windowid);
			return NULL;
		}
	}

	if(isConditional()) {
		// found a nested if - visit both sides - use outer if isLeft to determine action
		debug1("nested condition at %d for %d", m_windowid, id);
		if(isLeft) {
			if(m_left) {
				m_left->joinVisit(id, true);
			}
			if(m_right) {
				m_right->joinVisit(id, true);
			}
			return NULL;
		} else {
			ActionFragment *ret = m_left ? m_left->joinVisit(id, false) : NULL;
			if(ret) {
				return ret;
			}
			ret = m_right ? m_right->joinVisit(id, true) : NULL;
			return ret;
		}
	}

	if(m_next) {
		return m_next->joinVisit(id, isLeft);
	}

	return NULL;
}


void ActionFragment::findJoin()
{
	if(m_left && m_right) {
		m_left->joinVisit(m_windowid, true);
		ActionFragment *join = m_right->joinVisit(m_windowid, false);
		if(join) {
			debug1("if at %d joins at %d", m_windowid, join->windowid());
			m_join = join;
			// Associate the join with the outermost condition
			if(join->m_cond) {
				// Join already has a condition - are we outside it?
				if(m_level < join->m_cond->m_level) {
					// Yes, get rid of the join on the existing (inner) if and mark as join after
					debug1("join at %d marked with outer if %d", join->windowid(), m_windowid);
					join->m_cond->m_suppressJoin = true;
					join->m_cond->m_joinAfter = true;
					join->m_cond = this;
				} else {
					// No, mark ourself as the inner if
					debug1("join at %d not marked with inner if %d", join->windowid(), m_windowid);
					m_suppressJoin = true;
					m_joinAfter = true;
				}
			} else {
				debug1("join at %d marked with if %d", join->windowid(), m_windowid);
				join->m_cond = this;
			}
		} else {
			debug1("if at %d does not join", m_windowid);
		}
	} else {
		debug1("if at %d has no branches", m_windowid);
	}
}


void ActionFragment::reset()
{
	m_visit = 0;
}


void ActionFragment::generate(FILE *out, int ind)
{
	// fprintf(out, "#frag %d\n", m_windowid);

	bool insertLoop = m_loopStart && !isConditional();

	if(insertLoop) {
		indent(out, ind); fprintf(out, "set count = 0;\n");
		indent(out, ind); fprintf(out, "while($count < 3)\n");
		indent(out, ind); fprintf(out, "{\n");
		ind++;
		indent(out, ind); fprintf(out, "set count = $count + 1;\n");
	}

	if(m_join && (m_left == m_join)) {
		m_left = NULL;
		debug1("if %d has left and join the same - killing left", m_windowid);
	}
	if(m_join && (m_right == m_join)) {
		m_right = NULL;
		debug1("if %d has right and join the same - killing right", m_windowid);
	}

	// Work out which template we would like (it might not be available)
	TemplateType tpltype = TemplateType_Default;
	if(isConditional()) {
		if(m_loopStart) {
			if(m_loopLeft) {
				tpltype = TemplateType_Loop_Left;
			} else if(m_loopRight) {
				tpltype = TemplateType_Loop_Right;
			}
			// TODO: Trap left/right not being set
			// TODO: Trap both left and right being set
		} else if(!m_right) {
			tpltype = TemplateType_If_LeftOnly;
		} else if(!m_left) {
			tpltype = TemplateType_If_RightOnly;
		} else {
			tpltype = TemplateType_If_LeftRight;
		}
	}

	m_fragtype.generate(out, ind, m_windowid, m_attrs, m_down, m_left, m_right, tpltype);

	if(insertLoop) {
		ind--;
		indent(out, ind); fprintf(out, "}\n");		
	}

	if(m_exitPoint) {
		// output a return for an exit node
		indent(out, ind); fprintf(out, "return;\n");
	}

	if(m_join && !m_suppressJoin) {
		// This is a condition with a join - resume from next joined node
		m_join->generate(out, ind);
	}

	if(m_joinAfter) {
		// Stop here - we need to resume generation from the next node
		/*
		if(m_next) {
			indent(out, ind); fprintf(out, "// join - next is %d\n", m_next->windowid());
		} else if(m_join) {
			indent(out, ind); fprintf(out, "// join - next join is %d\n", m_join->windowid());
		} else {
			indent(out, ind); fprintf(out, "// join\n");
		}
		*/
		return;
	}
	if(m_loopEnd) {
		// Stop here - we need to resume generation from the next node
		indent(out, ind); fprintf(out, "// end loop\n");
		return;
	}

	if(m_next) {
		m_next->generate(out, ind);
	}
}


///////////////////////////////////////////////////////////////////////////////
// ActionPathEntry
///////////////////////////////////////////////////////////////////////////////

ActionPathEntry::ActionPathEntry(ActionFragment &actfrag, int pos)
	: m_actfrag(actfrag), m_pos(pos)
{}


ActionPathEntry::~ActionPathEntry()
{}


///////////////////////////////////////////////////////////////////////////////
// ActionPath
///////////////////////////////////////////////////////////////////////////////

ActionPath::ActionPath(ActionPathSet &set)
	: m_set(set), m_visited(false), m_path(false), m_isLoop(false),
	  m_last(NULL), m_ifCount(0)
{
	m_set.add(this);
}


ActionPath::ActionPath(const ActionPath &copy)
	: m_set(copy.m_set), m_visited(false), m_path(false), m_isLoop(copy.m_isLoop),
	  m_last(copy.m_last), m_ifCount(copy.m_ifCount)
{
	ListIterator<ActionPathEntry> iter(copy.m_path);
	for(ActionPathEntry *entry = iter.first(); entry; entry = iter.next()) {
		m_visited.put(entry->id(), entry);
		m_path.add(entry);
	}
	m_set.add(this);
}


ActionPath::~ActionPath()
{}


void ActionPath::add(ActionFragment &actfrag, int pos)
{
	ActionPathEntry *ele = new ActionPathEntry(actfrag, pos);
	m_visited.put(ele->id(), ele);
	m_path.add(ele);
	m_last = ele;

	if(actfrag.isJoin()) {
		m_ifCount--;
	}
	if(actfrag.isConditional()) {
		m_ifCount++;
		actfrag.setLevel(m_ifCount);
	}
}


bool ActionPath::contains(ActionFragment &actfrag)
{
	return m_visited.get(actfrag.windowid()) ? true : false;
}


ActionPathEntry *ActionPath::prev(ActionPathEntry *entry)
{
	ListIterator<ActionPathEntry> iter(m_path);
	ActionPathEntry *p = NULL;
	for(ActionPathEntry *n = iter.first(); n; n = iter.next()) {
		if(n == entry) {
			return p;
		}
		p = n;
	}
	return NULL;
}


ActionPathEntry *ActionPath::get(ActionFragment &actfrag)
{
	return m_visited.get(actfrag.windowid());
}


void ActionPath::dump()
{
	ListIterator<ActionPathEntry> iter(m_path);
	bool first = true;
	for(ActionPathEntry *e = iter.first(); e; e = iter.next()) {
		ActionFragment &f = e->actfrag();
		debug1("%s%d", first ? "" : ", ", f.windowid());
		if(e->pos() == LEFT_POS) {
			debug1("L");
		} else if(e->pos() == RIGHT_POS) {
			debug1("R");
		} else if(e->pos() == DOWN_POS) {
			debug1("D");
		}
		if(f.isConditional()) {
			debug1(">");
		}
		if(f.isJoin()) {
			debug1("<");
		}
		if(f.m_joinAfter) {
			debug1("-");
		}
		if(f.m_loopStart) {
			if(e->pos() == 3) {
				debug1(f.m_loopLeft ? "{" : ".");
			} else if(e->pos() == 2) { 
				debug1(f.m_loopRight ? "{" : ".");
			} else {
				debug1("{");
			}
		}
		if(f.m_loopEnd) {
			debug1("}");
		}
		if(f.m_exitPoint) {
			debug1("x");
		}
		if(f.m_level) {
			debug1("(%d)", f.m_level);
		}
		first = false;
	}
	debug1("\n");
}


///////////////////////////////////////////////////////////////////////////////
// GraphicalScriptGenerator
///////////////////////////////////////////////////////////////////////////////

ActionPathSet::ActionPathSet()
	: m_paths(true)
{}


ActionPathSet::~ActionPathSet()
{}


void ActionPathSet::add(ActionPath *path)
{
	m_paths.add(path);
}


bool ActionPathSet::checkExitPoint()
{
	bool exitPointFound = false;
	ListIterator<ActionPath> iter(m_paths);
	for(ActionPath *path = iter.first(); path; path = iter.next()) {
		//path->dump();
		if(path->last() && path->last()->actfrag().m_exitPoint) {
			exitPointFound = true;
		}
	}

	return exitPointFound;
}


void ActionPathSet::dump()
{
	ListIterator<ActionPath> iter(m_paths);
	for(ActionPath *path = iter.first(); path; path = iter.next()) {
		path->dump();
	}
}


///////////////////////////////////////////////////////////////////////////////
// GraphicalScriptGenerator
///////////////////////////////////////////////////////////////////////////////

GraphicalScriptGenerator::GraphicalScriptGenerator(triODBC &odbc, Dropzone &dz, Action &action)
	: m_odbc(odbc), m_action(action), m_frags(NULL), m_fragCache(NULL), m_starts(NULL)
{
	// 10 is action + .dm and null; 32 is length of int as text
	// printf("***** DEBUG >>>>>> dz.pathname() returns 0x%lx\n",dz.pathname());
	// if (dz.pathname()) printf("dz.pathname() = [%s]\n",dz.pathname());
	m_filename = (char*) malloc(strlen(dz.pathname()) + 10 + 32);
	sprintf(m_filename, "%s%saction%d.dm", dz.pathname(), DIR_SEP_STR, action.id());
}


GraphicalScriptGenerator::~GraphicalScriptGenerator()
{
	SAFE_FREE(m_filename);
	SAFE_DELETE(m_frags);
	SAFE_DELETE(m_fragCache);
	SAFE_DELETE(m_starts);
}


Fragment *GraphicalScriptGenerator::internalGetFragment(int fragtypeid)
{
	Fragment *frag = m_fragCache ? m_fragCache->get(fragtypeid) : NULL;
	if(frag) {
		return frag;
	}

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	char name[DB_NAME_LEN];
	int exitPoints;
	int actionid;
	SQLLEN ni_actionid;
	sql->BindColumn(1, SQL_CHAR, name, sizeof(name));
	sql->BindColumn(2, SQL_INTEGER, &exitPoints, sizeof(exitPoints));
	sql->BindColumn(3, SQL_INTEGER, &actionid, sizeof(actionid), &ni_actionid);

	int res = sql->ExecuteSQL("SELECT f.name, f.exitpoints, f.actionid "
		"FROM dm_fragments f WHERE f.id=%d", fragtypeid);
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return NULL;
	}

	res = sql->FetchRow();
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return NULL;
	}

	frag = new Fragment(fragtypeid, name, exitPoints);
	sql->CloseSQL();

	if(NULL_IND(actionid, 0) != 0) {
		// Fragment is a direct action call
		Action *action = m_action.getCalledAction(actionid);
		if(!action) {
			// oh dear
			debug1("ERROR: Failed to find called action %d", actionid);
		} else {
			// debug1("Action is %d '%s'", actionid, action->name());
			frag->setAction(action);
		}
	} else {
		// Fetch any associated templates (fragment text)
		int tpltype;
		char text[4096];
		SQLLEN ni_text = 0;
		sql->BindColumn(1, SQL_INTEGER, &tpltype, sizeof(tpltype));
		sql->BindColumn(2, SQL_CHAR, text, sizeof(text), &ni_text);

		res = sql->ExecuteSQL("SELECT ft.type, ft.data "
			"FROM dm_fragmenttext ft WHERE ft.fragmentid=%d", fragtypeid);
		if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
			for(res = sql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = sql->FetchRow()) {
				frag->addTemplate((TemplateType) tpltype, NULL_IND(text, NULL));
			}
		}
	}

	if(!m_fragCache) {
		m_fragCache = new HashtableById<Fragment>(true);
	}
	m_fragCache->put(fragtypeid, frag);
	return frag;
}


void GraphicalScriptGenerator::internalGetActionFragments()
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	// Get the fragment instance
	int windowid;
	int fragtypeid;
	sql->BindColumn(1, SQL_INTEGER, &windowid, sizeof(windowid));
	sql->BindColumn(2, SQL_INTEGER, &fragtypeid, sizeof(fragtypeid));

	int res = sql->ExecuteSQL("SELECT af.windowid, af.typeid FROM dm_actionfrags af WHERE af.actionid=%d", m_action.id());
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return;
	}

	m_frags = new HashtableById<ActionFragment>(true);

	for(res = sql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = sql->FetchRow()) {
		Fragment *frag = internalGetFragment(fragtypeid);
		if(frag) {
			ActionFragment *actfrag = new ActionFragment(windowid, *frag);
			m_frags->put(actfrag->windowid(), actfrag);
		} else {
			debug1("Fragment type %d not found", fragtypeid);
		}
	}

	sql->CloseSQL();

	m_starts = new HashtableById<ActionFragment>(false);

	// For each fragment instance, get its attributes and key them by name
	debug2("Getting action fragment attributes");
	AutoPtr<IntList> keys = m_frags->keys();
	IntListIterator iter(*keys);
	for(int windowid = iter.first(); iter.more(); windowid = iter.next()) {
		debug2("windowid = %d",windowid);
		ActionFragment *actfrag = m_frags->get(windowid);

		char attrname[DB_NAME_LEN];
		char attrvalue[2049];
		char attrtype[DB_ATTYPE_LEN];
		sql->BindColumn(1, SQL_CHAR, attrname, sizeof(attrname));
		sql->BindColumn(2, SQL_CHAR, &attrvalue, sizeof(attrvalue));
		sql->BindColumn(3, SQL_CHAR, &attrtype, sizeof(attrtype));
		res = sql->ExecuteSQL("SELECT fa.atname, afa.value, fa.attype "
			"FROM dm_actionfragattrs afa, dm_fragmentattrs fa "
			"WHERE afa.attrid = fa.id AND afa.actionid=%d "
			"AND afa.windowid=%d AND fa.typeid=%d",
			m_action.id(), windowid, actfrag->fragtypeid());
		for(res = sql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = sql->FetchRow()) {
			debug2("attrname=[%s] attrvalue=[%s]",attrname,attrvalue);
			if(strlen(attrvalue) == 0) {
				debug1("Attribute '%s' of frag %d has empty value - treat as undefined", attrname, actfrag->windowid());
				actfrag->addUndefinedAttribute(attrname);
			} else {
				if (stricmp(attrtype,"checkbox")==0) {
					// This is a checkbox, turn Y into "true" and any other value to "false"
					actfrag->addAttribute(attrname, (*attrvalue=='Y' || *attrvalue=='y')?true:false);
				} else {
					actfrag->addAttribute(attrname, attrvalue);
				}
			}
		}
		sql->CloseSQL();

		debug2("Adding in undefined variables");
		// Now add in any attributes for which values have not been defined
		sql->BindColumn(1, SQL_CHAR, attrname, sizeof(attrname));
		res = sql->ExecuteSQL("SELECT fa.atname FROM dm_fragmentattrs fa "
			"WHERE fa.typeid=%d", actfrag->fragtypeid());
		for(res = sql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = sql->FetchRow()) {
			if(!actfrag->hasAttribute(attrname)) {
				debug1("Adding undefined attribute '%s' of frag %d", attrname, actfrag->windowid());
				actfrag->addUndefinedAttribute(attrname);
			}
		}
		sql->CloseSQL();
	}

	// Get the flows between the fragment instances
	int flowid, nodefrom, nodeto, pos, parentid;
	SQLLEN ni_parentid = 0;
	sql->BindColumn(1, SQL_INTEGER, &flowid, sizeof(flowid));
	sql->BindColumn(2, SQL_INTEGER, &nodefrom, sizeof(nodefrom));
	sql->BindColumn(3, SQL_INTEGER, &nodeto, sizeof(nodeto));
	sql->BindColumn(4, SQL_INTEGER, &pos, sizeof(pos));
	sql->BindColumn(5, SQL_INTEGER, &parentid, sizeof(parentid), &ni_parentid);

	debug2("Getting flows between fragments");
	// TODO: distinct is only because action editor seems to insert duplicates on drilldown
	res = sql->ExecuteSQL(
		"SELECT DISTINCT aw.flowid, aw.nodefrom, aw.nodeto, aw.pos, af.parentwindowid "
		"FROM dm_actionflows aw, dm_actionfrags af "
		"WHERE aw.actionid=%d AND af.actionid = aw.actionid AND af.windowid = aw.nodeto", m_action.id());
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return;
	}

	for(res = sql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = sql->FetchRow()) {
		debug2("Got row, nodefrom=%d nodeto=%d",nodefrom,nodeto);
		ActionFragment *fragto = m_frags->get(nodeto);
		debug2("set fragto");
		if(!fragto) {
			debug1("To fragment %d not found", nodeto);
			continue;
		}
		// Start node has id 0 and not actionfrag entry (null check excludes drilldowns)
		if(nodefrom == 0) {
			debug2("nodefrom is 0 (start)");
			int startkey = NULL_IND(parentid, 0);
			debug2("startkey is %d",startkey);
			if(m_starts->get(startkey)) {
				debug1("ERROR: More than one start fragment for %d", startkey);
			}
			debug2("putting startkey and fragto into m_starts list");
			m_starts->put(startkey, fragto);
			debug2("setting start flag against fragto");
			fragto->setIsStart();
			debug2("checking parentid > 0");
			if(NULL_IND(parentid,0) > 0) {
				debug2("parentid=%d setting parentfrag ptr",parentid);
				ActionFragment *parentfrag = m_frags->get(parentid);
				debug2("adding DOWN_POS fragto to parentfrag links");
				if (parentfrag) {
					parentfrag->addLink(DOWN_POS, fragto);
				} else {
					debug2("parentfrag not found for parentid=%d",parentid);
				}
			}
		} else {
			debug2("nodefrom is %d",nodefrom);
			ActionFragment *fragfrom = m_frags->get(nodefrom);
			debug2("got fragfrom");
			if(fragfrom) {
				debug2("adding link to fragfrom");
				fragfrom->addLink(pos, fragto);
			} else {
				debug1("From fragment %d not found", nodefrom);
			}
		}
	}
	debug2("Leaving GraphicalScriptGenerator::internalGetActionFragments()");
}


void GraphicalScriptGenerator::reset()
{
	AutoPtr<IntList> frags = m_frags->keys();
	IntListIterator resetiter(*frags);
	for(int resetid = resetiter.first(); resetiter.more(); resetid = resetiter.next()) {
		ActionFragment *resetfrag = m_frags->get(resetid);
		if(resetfrag) {
			resetfrag->reset();
		}
	}
}


bool GraphicalScriptGenerator::generate()
{
	internalGetActionFragments();

	ActionFragment *start = m_starts->get(0);

	if(!start) {
		fprintf(stderr,"ERROR: No start fragment\n");
		return false;
	}

	// Set pathCalc off to trace all the execution paths, find loops etc.
	ActionPathSet pathset;
	ActionPath *path = new ActionPath(pathset);
	start->pathCalc(path, false);

	debug1("Paths are as follows:");
	pathset.dump();

	if(!pathset.checkExitPoint()) {
		fprintf(stderr,"ERROR: No exit point\n");
		return false;
	}

	// Now for any conditions, look and see which joins belong to them
	AutoPtr<IntList> frags = m_frags->keys();
	IntListIterator iter(*frags);	
	for(int fragid = iter.first(); iter.more(); fragid = iter.next()) {
		ActionFragment *frag = m_frags->get(fragid);
		if(frag && frag->isConditional()) {
			frag->findJoin();
		}
		reset();
	}

	ConstCharPtr sig = m_action.getSignature();

	FILE *out = fopen(m_filename, "wb");
	if(!out) {
		return false;
	}

	fprintf(out, "%s\n{\n", (const char*) sig);

	start->generate(out, 1);

	fprintf(out, "}\n");
	fclose(out);

	extern void catFile(const char *filename);
	catFile(m_filename);

	return true;
}
