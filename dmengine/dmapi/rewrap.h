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
#ifndef __rewrap_h


class RegExpWrapper
{
private:
	struct regexp *m_re;

public:
	RegExpWrapper(const char *re, bool isPattern = false);
	~RegExpWrapper();

	int match(const char *str);
	char *getMatch(int sub);

	char *replaceAll(const char *str, const char *replace, bool noIterate=false);
};


#define __rewrap_h

#endif /*__rewrap_h*/
