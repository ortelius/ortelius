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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rewrap.h"

#include "platform.h"
#include "regexp.h"
#include "exceptions.h"
#include "charptr.h"


///////////////////////////////////////////////////////////////////////////////
// RegExpWrapper
///////////////////////////////////////////////////////////////////////////////

RegExpWrapper::RegExpWrapper(const char *re, bool isPattern /*= false*/)
	: m_re(NULL)
{
	CharPtr temp;
	if(isPattern) {
		// Convert the pattern to a regexp for matching - * becomes .* and ? becomes .
		// e.g. "foo*.ht?" becomes "foo.*\.ht."
		temp = (char*) malloc(strlen(re) * 2);
		char *y = temp;
		for(const char *x = re; x && *x; x++) {
			if(*x == '*') {
				*y++ = '.'; *y++ = '*';
				continue;
			} else if(*x == '?') {
				*y++ = '.';
				continue;
			} else if((*x == '.') || (*x == '(') || (*x == ')')
					|| (*x == '[') || (*x == '+') || (*x == '^')
					|| (*x == '$') || (*x == '|')) {
				*y++ = '\\';
			}
			*y++ = *x;
		}
		*y++ = '$'; // tie to end of string
		*y = '\0';

		re = temp;
	}

	m_re = regcomp((char*) re);
	if(!m_re) {
		throw RuntimeError("Invalid regular expression '%s'", re);
	}
}


RegExpWrapper::~RegExpWrapper()
{
	SAFE_FREE(m_re);
}


int RegExpWrapper::match(const char *str)
{
	return regexec(m_re, (char*) str);
}


char *RegExpWrapper::getMatch(int sub)
{
	int len = m_re->endp[sub] - m_re->startp[sub];
	char *ret = (char*) malloc(len + 1);
	memcpy(ret, m_re->startp[sub], len);
	ret[len] = '\0';
	return ret;
}


extern void dumpbuffer(const char *buf, int len);

char *RegExpWrapper::replaceAll(const char *str, const char *replace, bool noIterate /* = false */)
{
	// debug1("data = '%s'", str);
	// dumpbuffer(str, strlen(str)+1);

	char *ret = strdup(str);

	int start = 0;
	while(true)
	{
		if (ret[start]=='\0') break;	// end of file
		// Find the first match

		if(!regexec(m_re, (char*) &ret[start]) || !m_re->startp[0] || !m_re->endp[0]) {
			break;
		}
		// Calculate the replacement for what we have matched
		char dst[8096];
		regsub(m_re, (char*)replace, dst);

		// debug1("dst = '%s'", dst);
		// dumpbuffer(dst, strlen(dst)+1);

		// Insert the match into the string
		size_t slen = m_re->startp[0] - ret;
		size_t elen = strlen(ret) - (m_re->endp[0] - ret);
		char *temp = (char*) malloc(slen + elen + strlen(dst) + 1);
		char *pos = temp;
		memcpy(pos, ret, slen);
		pos += slen;
		memcpy(pos, dst, strlen(dst));
		pos += strlen(dst);
		memcpy(pos, m_re->endp[0], elen);
		pos += elen;
		*pos = '\0';

		SAFE_FREE(ret);
		ret = temp;
		if (m_re->startp[0] == m_re->endp[0]) break;	// zero length subs, must be RE placeholder like ^ or $
		if (noIterate) break;
		start = slen + strlen(dst);
	}

	//debug1("new data = '%s'", ret);
	//dumpbuffer(ret, strlen(ret)+1);

	return ret;
}
