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

		String redirect = System.getenv("SamlLogoutRedirect");

		if (redirect == null)
		 redirect = "";

  session.removeAttribute("session");
  session.invalidate();
  response.setContentType("text/html");

		//
		// Delete all user cookies
		// This will remove the cookies used by jstree to restore its last position plus
		// the DM cookies specifying selected tab(s) etc.
		//

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
  response.getWriter().write("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/3.7.0/jquery.min.js\" integrity=\"sha512-3gJwYpMe3QewGELv8k/BX9vcqhryRdzRMxVfq6ngyWXwo03GFEzjsUm8Q7RZcHPHksttq7/GFoxjCVUjkjvPdw==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>");
  response.getWriter().write("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/jquery-ui.min.js\" integrity=\"sha512-57oZ/vW8ANMjR/KQ6Be9v/+/h6bq9/l3f0Oc7vn6qMqyhvPd1cvKBRWWpzu0QoneImqr2SkmO4MSqU+RpHom3Q==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>");
  response.getWriter().write("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery-cookie/1.4.1/jquery.cookie.min.js\" integrity=\"sha512-3j3VU6WC5rPQB4Ld1jnLV7Kd5xr+cq9avvhwqzbH/taCRNURoeEpoPBK9pDyeukwSxwRPJ8fDgvYXd6SkaZ2TA==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>");

		response.getWriter().write("<script type=\"text/javascript\" src=\"js/ao/cookies.js\"></script>\n");
//		response.getWriter().write("<meta http-equiv=\"refresh\" content=\"1;URL=Home\">");
		response.getWriter().write("</head>");
		response.getWriter().write("<body onload=\"DeleteCookies('" + redirect + "')\">\n");
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
