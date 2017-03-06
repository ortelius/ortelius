/*
 *
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
package dmadmin.json;

public class JSONUtil
{
	/**
	 * Escapes a Java string so that it can be used with JSON.  JSON defines \t
	 * to expand to a tab character, so if we have a string containing \t, we
	 * must escape it to \\t in order to get a literal backslash.  We also
	 * escape " as this is used to surround string literals.
	 * @param str
	 * @return
	 */
	public static String escape(String str)
	{
		if(str != null) {
			str = str.replace("\\", "\\\\");	// reverse solidus - MUST do this first
			str = str.replace("\"", "\\\"");	// quotation mark
			str = str.replace("/",  "\\/");		// solidus
			str = str.replace("\b", "\\b");		// backspace
			str = str.replace("\f", "\\f");		// formfeed
			str = str.replace("\n", "\\n");		// newline
			str = str.replace("\r", "\\r");		// carriage return
			str = str.replace("\t", "\\t");		// horizontal tab
			// TODO: u + 4 hex digits
		}
		return str;
	}
}
