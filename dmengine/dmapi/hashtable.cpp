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
#ifdef WIN32
#include <malloc.h>
#endif
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "hashtable.h"
#include "platform.h"
#include "list.h"


class StringBucket
{
public:
	char *key;
	char *value;
	StringBucket *next;

	StringBucket(const char *k, const char *v)
		:key(DUP_NULL(k)), value(DUP_NULL(v)), next(NULL)
	{}

	~StringBucket()
	{
		if(key) { free(key); }
		if(value) { free(value); }
	}
};


//////////////////////////////////////////////////////////////////////////////
// StringHashtable
//////////////////////////////////////////////////////////////////////////////

StringHashtable::StringHashtable(bool nocase /*= false*/, int buckets /*= 10*/)
	: m_bucketCount(buckets), m_nocase(nocase), m_count(0)
{
	m_buckets = (StringBucket**) malloc(sizeof(StringBucket*) * buckets);
	if (m_buckets) {
		for(int n = 0; n < buckets; n++) {
			m_buckets[n] = (StringBucket*) NULL;
		}
	}
}


StringHashtable::~StringHashtable()
{
	clear();
	free(m_buckets);
	m_buckets = NULL;
	m_bucketCount = 0;
}


int StringHashtable::hash(const char *str) const
{
	int ret = 0;

	while(*str) {
		ret += m_nocase ? tolower(*str) : *str;
		str++;
	}

	return ret;
}


const char *StringHashtable::get(const char *key) const
{
	int n = hash(key) % m_bucketCount;

	for(StringBucket *b = m_buckets[n]; b; b = b->next) {
		if(b->key && ((m_nocase ? STRCASECMP(key, b->key) : strcmp(key, b->key)) == 0)) { return b->value; }
	}

	return NULL;
}


char *StringHashtable::put(const char *key, const char *value)
{
	int n = hash(key) % m_bucketCount;

	if(!m_buckets[n]) {
		m_buckets[n] = new StringBucket(key, value);
		m_count++;
		if(m_count/m_bucketCount > 5) {
			rehash();
		}
		return NULL;
	}

	StringBucket *prev = NULL;
	for(StringBucket *b = m_buckets[n]; b; b = b->next)
	{
		if(key && b->key && (m_nocase ? STRCASECMP(key, b->key) : strcmp(key, b->key)) == 0)
		{
			//printf("### bucket already contains %s\n", b->value);
			char *old = b->value;
			b->value = DUP_NULL(value);
			return old;
		}

		prev = b;
	}

	if(prev) {
		prev->next = new StringBucket(key, value);
		m_count++;
		if(m_count/m_bucketCount > 5) {
			rehash();
		}
	}

	return NULL;
}


// RHT 30/08/2011 - new append fn
//void Hashtable::append(const char *key, const char *value, const char *sep)
//{
//	const char *temp = get(key);
//	if(temp && value) {
//		if(!sep) sep = "";
//		char *newVal = (char*) malloc(strlen(temp) + strlen(sep) + strlen(value) + 1);
//		sprintf(newVal, "%s%s%s", temp, sep, value);
//		put(key, newVal);
//		free(newVal);
//	} else {
//		put(key, value);
//	}
//}


int StringHashtable::count() const
{
	return m_count;
}


StringList *StringHashtable::keys() const
{
	StringList *ret = new StringList();
	for(int n = 0; n < m_bucketCount; n++) {
		for(StringBucket *b = m_buckets[n]; b; b = b->next) {
			ret->add(b->key);
		}
	}
	return ret;
}


StringList *StringHashtable::values() const
{
	StringList *ret = new StringList();
	for(int n = 0; n < m_bucketCount; n++) {
		for(StringBucket *b = m_buckets[n]; b; b = b->next) {
			ret->add(b->value);
		}
	}
	return ret;
}


void StringHashtable::rehash()
{
	int buckets = m_count/2;
	int n;

	void *t = malloc(sizeof(StringBucket*) * buckets);
	if (t) {
		StringBucket **temp = (StringBucket**) t;
		for(n = 0; n < buckets; n++) {
			temp[n] = NULL;
		}
		
		for(n = 0; n < m_bucketCount; n++) {
			StringBucket *next = NULL;
			for(StringBucket *b = m_buckets[n]; b; b = next) {
				next = b->next;
				b->next = NULL;
				int m = hash(b->key) % buckets;
				if(temp[m]) {
					StringBucket *t;
					for(t = temp[m]; t && t->next; t = t->next) { /*empty*/ }
					if(t) {
						t->next = b;
					}
				} else {
					temp[m] = b;
				}
			}
		}
		
		free(m_buckets);
		m_buckets = temp;
		m_bucketCount = buckets;
	}
}


void StringHashtable::clear()
{
	for(int n = 0; n < m_bucketCount; n++) {
		for(StringBucket *b = m_buckets[n]; b; ) {
			StringBucket *prev = b;
			b = b->next;
			prev->next = NULL;
			delete prev;
		}

		m_buckets[n] = NULL;
	}
}


void StringHashtable::dump(int indent /*= 0*/)
{
	for(int n = 0; n < m_bucketCount; n++) {
		for(int t = 0; t < indent; t++) {
			printf("\t");
		}

		printf("Bucket #%d:\n", n);

		for(StringBucket *b = m_buckets[n]; b; b = b->next) {
			for(int t = 0; t < indent; t++) {
				printf("\t");
			}

			printf("\t%s -> %s\n", (b->key ? b->key : "(null)"), (b->value ? b->value : "(null)"));
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////


#if 0
void hashtable_test()
{
	Hashtable ht;
	ht.put("one", "1");
	ht.put("two", "2");
	ht.put("three", "3");
	ht.dump();
	printf("one -> %s\n", NULL_CHECK(ht.get("one")));
	printf("two -> %s\n", NULL_CHECK(ht.get("two")));
	printf("three -> %s\n", NULL_CHECK(ht.get("three")));
	printf("four -> %s\n", NULL_CHECK(ht.get("four")));

	Hashtable2 ht2;
	ht2.put("hash1", &ht);
	ht2.put("hash2", &ht);
	ht2.dump();
}
#endif /*0*/
