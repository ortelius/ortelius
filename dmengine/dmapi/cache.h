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
