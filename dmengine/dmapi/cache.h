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
#ifndef __cache_h

#define __cache_h


template<class T> class DMAPI_API_EXPORT CacheById
{
private:
	HashtableById<T> *m_cache;

public:
	CacheById();
	virtual ~CacheById();

	T* get(int id);
	void put(T* obj);
};


template<class T> class DMAPI_API_EXPORT CacheByIdAndName
{
private:
	HashtableById<T> *m_cache;
	Hashtable<T> *m_names;

public:
	CacheByIdAndName();
	virtual ~CacheByIdAndName();

	T* get(int id);
	T* get(const char *name);
	void put(T* obj);
};


#include "cache.impl"

#endif /*__cache_h*/