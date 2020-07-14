#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "platform.h"


///////////////////////////////////////////////////////////////////////////////
// StringListEntry
///////////////////////////////////////////////////////////////////////////////

StringListEntry::StringListEntry(const char *str)
	: m_str(DUP_NULL(str)), m_next(NULL)
{}


///////////////////////////////////////////////////////////////////////////////
// StringList
///////////////////////////////////////////////////////////////////////////////

StringList::StringList()
	: m_head(NULL), m_tail(NULL), m_count(0)
{}


/*virtual*/ StringList::~StringList()
{
	StringListEntry *e = m_head;
	while(e) {
		StringListEntry *n = e->m_next;
		if(e->m_str) {
			free(e->m_str); e->m_str = NULL;
		}
		delete e;
		e = n;
	}
}


StringList *StringList::add(const char *str)
{
	StringListEntry *e = new StringListEntry(str);
	m_count++;
	if(m_tail) {
		m_tail->m_next = e;
		m_tail = e;
	} else {
		m_head = m_tail = e;
	}
	return this;
}


void StringList::print() const
{
	for(StringListEntry *e = m_head; e; e = e->m_next) {
		printf("\"%s\" ", (e->m_str ? e->m_str : "(null)"));
	}
}


int StringList::size() const
{
	return m_count;
}


///////////////////////////////////////////////////////////////////////////////
// StringListIterator
///////////////////////////////////////////////////////////////////////////////

StringListIterator::StringListIterator(const StringList &list)
	: m_list(list), m_pos(NULL)
{}

	
const char *StringListIterator::first()
{
	m_pos = m_list.m_head;
	return m_pos ? m_pos->m_str : NULL;
}


const char *StringListIterator::next()
{
	m_pos = m_pos ? m_pos->m_next : NULL;
	return m_pos ? m_pos->m_str : NULL;
}


///////////////////////////////////////////////////////////////////////////////
// IntListEntry
///////////////////////////////////////////////////////////////////////////////

IntListEntry::IntListEntry(int val)
	: m_val(val), m_next(NULL)
{}


///////////////////////////////////////////////////////////////////////////////
// IntList
///////////////////////////////////////////////////////////////////////////////

IntList::IntList()
	: m_head(NULL), m_tail(NULL), m_count(0)
{}


/*virtual*/ IntList::~IntList()
{
	IntListEntry *e = m_head;
	while(e) {
		IntListEntry *n = e->m_next;
		delete e;
		e = n;
	}
}


IntList *IntList::add(int val)
{
	IntListEntry *e = new IntListEntry(val);
	m_count++;
	if(m_tail) {
		m_tail->m_next = e;
		m_tail = e;
	} else {
		m_head = m_tail = e;
	}
	return this;
}


void IntList::print() const
{
	for(IntListEntry *e = m_head; e; e = e->m_next) {
		printf("%d ", e->m_val);
	}
}


int IntList::size() const
{
	return m_count;
}


///////////////////////////////////////////////////////////////////////////////
// IntListIterator
///////////////////////////////////////////////////////////////////////////////

IntListIterator::IntListIterator(const IntList &list)
	: m_list(list), m_pos(NULL)
{}

	
int IntListIterator::first()
{
	m_pos = m_list.m_head;
	return m_pos ? m_pos->m_val : 0;
}


int IntListIterator::next()
{
	m_pos = m_pos ? m_pos->m_next : NULL;
	return m_pos ? m_pos->m_val : 0;
}


bool IntListIterator::more()
{
	return m_pos ? true : false;
}
