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

package dmadmin;

import java.io.UnsupportedEncodingException;

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
				if (CookieName.equals(cookies[i].getName()))
				{
				 String val = cookies[i].getValue();
				 try
     {
      val = java.net.URLDecoder.decode(val, "UTF-8");
     }
     catch (UnsupportedEncodingException e)
     {
      e.printStackTrace();
     }
				 return(val);
				}
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
