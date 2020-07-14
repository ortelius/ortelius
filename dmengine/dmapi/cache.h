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