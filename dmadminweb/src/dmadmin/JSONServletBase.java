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

import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;

@SuppressWarnings("serial")
public abstract class JSONServletBase
	extends HttpServlet
{
 DMSession so = null;
 HttpSession session = null;
 
	public JSONServletBase()
	{
		super();
	}
	
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		internalHandleRequest(false, request, response);
	}

	
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		internalHandleRequest(true, request, response);
	}
	
	
	private void internalHandleRequest(boolean isPost, HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException
	{		
		response.setContentType("application/json");

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		response.setHeader("Content-Disposition", "inline");
		response.setHeader("Cache-Control", "no-cache");

		System.out.println("In " + getClass().getName());

		try {
			IJSONSerializable json = null;
			
			try {
				json = handleRequest(so, isPost, request, response);
			} catch(PermissionDeniedException e) {
				json = new JSONObject().add("result", false).add("error", e.getMessage()); 
			} catch(Exception e) {
				// Report any exceptions raised by the request
				e.printStackTrace();
				json = new JSONObject().add("result", false).add("error", e.toString()); 
			}
			
			if(json == null) {
				json = new JSONObject().add("result", false).add("error", "Request returned no result"); 
			}

			// Serialize as JSON
			String ret = json.getJSON();
			
			// Set the content length for http keep-alive
			response.setContentLength(ret.length());
			
			PrintWriter out = response.getWriter();
			
			out.println(ret);
			System.out.println(ret);
		} catch(Exception e) {
			// This really is unrecoverable!
			e.printStackTrace();
		}
  }
	}

	
	public abstract IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws PermissionDeniedException, ServletException, IOException;
	
	
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
			} catch(NumberFormatException e) {}
		}
		return defaultValue;
	}
	
	
	public static String urlDecode(String value)
	{
		try {
			return java.net.URLDecoder.decode(value, "ISO-8859-1");
		} catch (UnsupportedEncodingException e) {
			return null;
		}
	}
}
