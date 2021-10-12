// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef WIN32
#include "windows.h"
#include <io.h>
#include <fcntl.h>
#define HTTP_EXPORT __declspec(dllexport)
#else
#define HTTP_EXPORT
#endif /*WIN32*/

#include <stdio.h>
#include <sys/stat.h>
#include "logprocess.h"
#include "httppattern.h"


HttpPattern::HttpPattern(char *pattern)
{
	// Split the pattern into groups of fields
	// if the pattern is:
	//		filename.{1-10}.{1-20}.{1-20}.{1-20}.jar
	// then the pattern will be stored as:
	//		filename.*.*.*.*.jar
	// there will be an array of 4 cfv's (current field values)
	// and an array of 4 ffv's (final field values)
	//
	// printf("In HttpPattern constructor pattern=[%s]\n",pattern);
	m_pattern = (char *)malloc(strlen(pattern)+1);
	
	m_curfv = (int *)0;
	m_minfv = (int *)0;
	m_maxfv = (int *)0;
	m_NumFields = 0;

	char *fpp = m_pattern;
	int	n,m;
	bool inField=false;
	for (char *p = pattern;*p;p++) {
		if (*p == '{') {
			inField=true;
			n = -1;
			m = -1;
			*fpp++ = '*';
			m_NumFields++;
			m_curfv = (int *)(m_curfv?realloc(m_curfv,m_NumFields*sizeof(int)):malloc(sizeof(int)));
			m_minfv = (int *)(m_minfv?realloc(m_minfv,m_NumFields*sizeof(int)):malloc(sizeof(int)));
			m_maxfv = (int *)(m_maxfv?realloc(m_maxfv,m_NumFields*sizeof(int)):malloc(sizeof(int)));
			continue;
		} else if (*p=='}') {
			inField=false;
			m_minfv[m_NumFields-1] = n;
			m_maxfv[m_NumFields-1] = m;
			m_curfv[m_NumFields-1] = n;
			continue;
		}
		if (inField) {
			if (n == -1) {
				// Not yet found n value
				n = atol(p);				// will take value up to -
				continue;
			}
			if (*p == '-') {
				// Got to the range indicator of n-m
				char *mp = p+1;	// get to next char
				if (*mp=='}') {
					// no maximum
				} else {
					m=atol(mp);
				}
			}
		} else {
			*fpp++ = *p;
		}
	}
	*fpp='\0';
	/*
	printf("pattern=[%s]\n",m_pattern);
	for (int i=0;i<m_NumFields;i++) {
		printf("m_minfv[%d]=%d m_maxfv[%d]=%d\n",i,m_minfv[i],i,m_maxfv[i]);
	}
	*/
}

HttpPattern::~HttpPattern()
{
}

char *HttpPattern::getTestFilename()
{
	// calculate size of return string
	int tlen=strlen(m_pattern);
	for (int i=0;i<m_NumFields;i++) {
		char t[128];
		sprintf(t,"%d",m_curfv[i]);
		tlen+=strlen(t);
	}
	char *ret = (char *)malloc(tlen+1);
	*ret='\0';
	int fn=0;
	for (char *p=m_pattern,*q=ret;*p;p++,q++) {
		if (*p == '*') {
			sprintf(q,"%d",m_curfv[fn++]);
			while (*q) q++;
			q--;
		} else {
			*q = *p;
			*(q+1)='\0';
		}
	}
	return ret;
}

char *HttpPattern::getVersionString()
{
	// Pattern is in form testfile.*.*.*.*.txt
	// Expand each * into current value
	int tlen=strlen(m_pattern);
	for (int i=0;i<m_NumFields;i++) {
		char t[128];
		sprintf(t,"%d",m_curfv[i]);
		tlen+=strlen(t);
	}
	debug1("tlen=%d",tlen);
	char *ret = (char *)malloc(tlen+1);
	*ret='\0';
	int fn=0;
	char *eop=(char *)0;	// end of pattern pointer
	debug1("getVersionString, m_pattern=[%s]",m_pattern);
	for (char *p=m_pattern,*q=ret;*p;p++) {
		if (*p == '*') {
			sprintf(q,"%d",m_curfv[fn++]);
			while (*q) q++;
			eop = q;
			debug1("ret=[%s]",ret);
		} else {
			if (eop) {
				// Only add characters to pattern if we've expanded our first *
				*q = *p;
				*(q+1)='\0';
				q++;
			}
		}
	}
	if (eop) *eop='\0';
	return ret;
}

void HttpPattern::setMidPoint(int field)
{
	int range = (m_maxfv[field-1] - m_minfv[field-1])+1;
	debug1("set mid point field %d - range = %d (from %d to %d)",field,range,m_minfv[field-1],m_maxfv[field-1]);
	m_curfv[field-1] = m_minfv[field-1] + (range / 2);
}

void HttpPattern::chop(int field,bool tob)
{
	if (tob) {
		debug1("setting max to %d",m_curfv[field-1]);
		m_maxfv[field-1] = m_curfv[field-1];
		
	} else {
		debug1("setting min to %d",m_curfv[field-1]);
		m_minfv[field-1] = m_curfv[field-1];
	}
	setMidPoint(field);
	debug1("cv = %d max = %d min = %d",m_curfv[field-1],m_maxfv[field-1],m_minfv[field-1]);
}

int	HttpPattern::getFieldCount()
{
	return m_NumFields;
}

int	HttpPattern::getRange(int field)
{
	debug1("getRange(%d) returns %d",field,m_maxfv[field-1] - m_minfv[field-1]);
	return m_maxfv[field-1] - m_minfv[field-1];
}

int HttpPattern::getFieldValue(int field)
{
	return m_curfv[field-1];
}

bool HttpPattern::incrField(int field)
{
	if (m_curfv[field-1] < m_maxfv[field-1]) {
		m_curfv[field-1]++;
		return true;
	} else {
		return false;
	}
}

void HttpPattern::setField(int field,int newval)
{
	m_curfv[field-1]=newval;
}