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
package dmadmin;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;

public class ServletUtils
{
	public static int getIntParameter(HttpServletRequest request, String name) {
		String value = request.getParameter(name);
		if(value != null) {
			try {
				return Integer.parseInt(value);
			} catch(NumberFormatException e) {}
		}
		throw new RuntimeException("Parameter '" + name + "' not specified or invalid value");
	}

	public static int getIntParameter(HttpServletRequest request, String name, int defaultValue) {
		String value = request.getParameter(name);
		if(value != null) {
			try {
				return Integer.parseInt(value);
			} catch(NumberFormatException e) {
				throw new RuntimeException("Parameter '" + name + "' has invalid value");
			}
		}
		return defaultValue;
	}
	
	public static String GetCookie(HttpServletRequest request,String CookieName)
	{
		Cookie[] cookies = request.getCookies();
		if (cookies != null) {
			for(int i=0; i<cookies.length; i++) {
				if (CookieName.equals(cookies[i].getName())) return(cookies[i].getValue());
			}
		}
		return null;
	}
	
	public static String GetURL(HttpServletRequest request)
	{
		String requestURI = request.getRequestURI();
		
		if (request.getQueryString() != null) {
		    requestURI=requestURI+"?"+request.getQueryString();
		}
		return requestURI.substring(requestURI.indexOf("/",1)+1);
	}
}
