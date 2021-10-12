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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

/**
 * Servlet implementation class Logout
 */
public class Logout extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Logout() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		HttpSession session = request.getSession();
    	session.removeAttribute("session");
		session.invalidate();
		//
		// Delete all user cookies
		// This will remove the cookies used by jstree to restore its last position plus
		// the DM cookies specifying selected tab(s) etc.
		//
		response.setContentType("text/html");
//		Cookie[] cookies = request.getCookies();
//		String srv = "console.deployhub.com";
//		
//		if (cookies != null) {
//			for (Cookie usercookie : cookies) {
//				System.out.println("Clearing cookie "+ srv + ":" + usercookie.getName());
//				Cookie c = new Cookie(usercookie.getName(), null);
//				c.setMaxAge(0);
//				c.setPath("/");
//				c.setDomain(srv);
//				response.addCookie(c);
//		    }
//		}
		response.getWriter().write("<html>");
		response.getWriter().write("<head>");
		response.getWriter().write("<script type=\"text/javascript\" src=\"js/jquery.min.js\"></script>\n");
		response.getWriter().write("<script type=\"text/javascript\" src=\"js/jquery-ui.js\"></script>\n");
		response.getWriter().write("<script type=\"text/javascript\" src=\"jquery.jstree.js\"></script>\n");
		response.getWriter().write("<script type=\"text/javascript\" src=\"js/jquery.cookie.js\"></script>\n");
		response.getWriter().write("<script type=\"text/javascript\" src=\"js/cookies.js\"></script>\n");
//		response.getWriter().write("<meta http-equiv=\"refresh\" content=\"1;URL=Home\">");
		response.getWriter().write("</head>");
		response.getWriter().write("<body onload=\"DeleteCookies()\">\n");
		response.getWriter().write("You have been logged out.");
		response.getWriter().write("</body>");
		response.getWriter().write("</html>"); 
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
