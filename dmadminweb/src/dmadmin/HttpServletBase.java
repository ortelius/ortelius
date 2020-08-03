/*
 *
 *  Ortelius for Microservice Configuration Mapping
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

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

@SuppressWarnings("serial")
public abstract class HttpServletBase
	extends HttpServlet
{
 DMSession so = null;
 HttpSession session = null;
 
	public HttpServletBase()
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

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
//  so.checkConnection(request.getServletContext());
  
		if (so == null)  {
			session.invalidate();
				String origurl=ServletUtils.GetURL(request);
				Cookie origurlCookie = new Cookie("origurl",origurl);
				response.addCookie(origurlCookie);
				request.getRequestDispatcher("Login").forward(request, response);
			return;
		}

		response.setHeader("Content-Disposition", "inline");
		response.setHeader("Cache-Control", "no-cache");
		
		//System.out.println("********************** COOKIES ************************");
		//Cookie[] cookies = request.getCookies();
		//for(int i=0; i<cookies.length; i++) {
		//	System.out.println(cookies[i].getName() +"="+ cookies[i].getValue());
		//}
		//System.out.println("****************** END OF COOKIES *********************");
		
		// record the URL we're going to visit against the mainframe id
		String ut = ServletUtils.GetCookie(request,"ut");
		System.out.println("ut (from cookie)="+ut);
		if (ut == null) ut = "1";
		String url = ServletUtils.GetURL(request);
		System.out.println("url="+url);
		if (request.getParameter("ut") != null) {
			// Come from a tree view or we otherwise need to record this URL.
			Cookie urlCookie = new Cookie("mainframe"+ut,url);
			System.out.println("Adding "+urlCookie.getName()+" = "+urlCookie.getValue());
			response.addCookie(urlCookie);
		}

		try {
			handleRequest(so, isPost, request, response);
		} catch(FatalError e) {
			request.setAttribute("message", e.getMessage());
			request.getRequestDispatcher("WEB-INF/ErrorMessage.jsp").forward(request, response);
			return;
		} catch(Exception e) {
			e.printStackTrace();
		}
  }
		
	}

	public abstract void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException;
	
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
	
	public static String getStringParameter(HttpServletRequest request, String name, String defaultValue) {
		String value = request.getParameter(name);
		return (value != null) ? value : defaultValue;
	}
}
