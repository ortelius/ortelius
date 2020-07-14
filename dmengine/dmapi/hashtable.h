#ifndef __HASHTABLE_H

#define __HASHTABLE_H


#include "dmapiexport.h"


template<class T> class Bucket
{
public:
	char *key;
	T *value;
	Bucket<T> *next;

	Bucket<T>(const char *k, T *v);
	~Bucket<T>();
};


template<class V> class DMAPI_API_EXPORT Hashtable
{
protected:
	class Bucket<V> **m_buckets;
	int m_bucketCount;
	int m_count;
	bool m_nocase;
	bool m_isOwner;

	int hash(const char *str) const;

public:
	Hashtable<V>(bool nocase = false, bool isOwner = false, int buckets = 10);
	virtual ~Hashtable<V>();
	V *get(const char *key) const;
	V *put(const char *key, V *value);
	void remove(const char *key, bool nodelete = false);
	int count() const;
	class StringList *keys() const;
	V **values() const;
	void rehash();
	void clear();
	void dump();
};


template<class T> class BucketById
{
public:
	int key;
	T *value;
	BucketById<T> *next;

	BucketById<T>(int k, T *v);
	~BucketById<T>();
};


template<class V> class DMAPI_API_EXPORT HashtableById
{
protected:
	class BucketById<V> **m_buckets;
	int m_bucketCount;
	int m_count;
	bool m_isOwner;

public:
	HashtableById<V>(bool isOwner = false, int buckets = 10);
	virtual ~HashtableById<V>();
	V *get(int key) const;
	V *put(int key, V *value);
	void remove(int key, bool nodelete = false);
	int count() const;
	class IntList *keys() const;
	V **values() const;
	void rehash();
	void clear();
	void dump();
};


class DMAPI_API StringHashtable
{
private:
	class StringBucket **m_buckets;
	int m_bucketCount;
	int m_count;
	bool m_nocase;

	int hash(const char *str) const;

public:
	StringHashtable(bool nocase = false, int buckets = 10);
	~StringHashtable();
	const char *get(const char *key) const;
	char *put(const char *key, const char *value);
	int count() const;
	class StringList *keys() const;
	class StringList *values() const;
	void rehash();
	void clear();
	void dump(int indent = 0);
};


#include "hashtable.impl"


#endif /*__HASHTABLE_H*/
