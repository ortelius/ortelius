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
