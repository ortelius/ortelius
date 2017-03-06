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
