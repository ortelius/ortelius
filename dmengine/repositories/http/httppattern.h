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

#ifndef _httppattern_

class HttpPattern
{
private:
	char	*m_pattern;		// Original pattern
	char	*prefix;		// Prefix (bit before the varying fields)
	char	*postfix;		// Postfix (bit after the varying fields)
	int		m_NumFields;	// total number of fields in pattern
	int		*m_curfv;			// array of current field values
	int		*m_minfv;			// array of current field values
	int		*m_maxfv;			// array of current field values

public:
	HttpPattern(char *pattern);
	~HttpPattern();
	char	*getTestFilename();
	void	chop(int field,bool tob);
	void	setMidPoint(int field);
	int		getFieldCount();
	int		getRange(int field);
	int		getFieldValue(int field);
	bool	incrField(int field);
	void	setField(int field,int newval);
	char	*getVersionString();
};

#endif
