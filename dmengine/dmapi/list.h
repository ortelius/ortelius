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
#ifndef __list_h

#define __list_h


#include "dmapiexport.h"


template<class T> class List;
template<class T> class ListIterator;

template<class T> class ListEntry
{
private:
	T *m_obj;
	class ListEntry<T> *m_next;

public:
	ListEntry<T>(T *obj);

	friend class List<T>;
	friend class ListIterator<T>;
};


template<class T> class DMAPI_API_EXPORT List
{
private:
	ListEntry<T> *m_head;
	ListEntry<T> *m_tail;
	int m_count;
	bool m_isOwner;

public:
	List<T>(bool isOwner = false);
	virtual ~List<T>();
	List<T> *add(T *obj);
	List<T> *prepend(T *obj);
	void remove(T *obj);
	//void print(int indent) const;
	int size() const;
	void setIsOwner(bool isOwner);
	void clear();

	friend class ListIterator<T>;
};


template<class T> class DMAPI_API_EXPORT ListIterator
{
private:
	const List<T> &m_list;
	ListEntry<T> *m_pos;

public:
	ListIterator<T>(const List<T> &list);
	T *first();
	T *next();
	int size();
};


///////////////////////////////////////////////////////////////////////////////

class StringListEntry
{
private:
	char *m_str;
	class StringListEntry *m_next;

public:
	StringListEntry(const char *str);

	friend class StringList;
	friend class StringListIterator;
};


class DMAPI_API StringList
{
private:
	StringListEntry *m_head;
	StringListEntry *m_tail;
	int m_count;

public:
	StringList();
	virtual ~StringList();
	StringList *add(const char *str);
	void print() const;
	int size() const;

	friend class StringListIterator;
};


class DMAPI_API StringListIterator
{
private:
	const StringList &m_list;
	StringListEntry *m_pos;

public:
	StringListIterator(const StringList &list);
	const char *first();
	const char *next();
};


///////////////////////////////////////////////////////////////////////////////

class IntListEntry
{
private:
	int m_val;
	class IntListEntry *m_next;

public:
	IntListEntry(int val);

	friend class IntList;
	friend class IntListIterator;
};


class DMAPI_API IntList
{
private:
	IntListEntry *m_head;
	IntListEntry *m_tail;
	int m_count;

public:
	IntList();
	virtual ~IntList();
	IntList *add(int val);
	void print() const;
	int size() const;

	friend class IntListIterator;
};


class DMAPI_API IntListIterator
{
private:
	const IntList &m_list;
	IntListEntry *m_pos;

public:
	IntListIterator(const IntList &list);
	int first();
	int next();
	bool more();
};


///////////////////////////////////////////////////////////////////////////////

#include "list.impl"


#endif /*__list_h*/
