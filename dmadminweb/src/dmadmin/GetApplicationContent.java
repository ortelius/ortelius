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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.model.Application;

/**
 * Servlet implementation class GetApplicationContent
 */
public class GetApplicationContent extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetApplicationContent() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("application/json");
		Integer appid = ServletUtils.getIntParameter(request,"appid"); 

		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		Application app = so.getApplication(appid,false);
		List<Application> a = so.getApplicationVersions(app);
		boolean isRelease = false;
		
		if (request.getParameter("isRelease") != null && request.getParameter("isRelease").equalsIgnoreCase("Y"))
		 isRelease = true;
		
		out.print("[");

		boolean subv=false;
		for (Application xav: a)
		{
			// Only include appversion if it's in the same domain as the parent
			if (xav.getDomainId() == app.getDomainId()) {
				if (subv) out.println(",");
				if (isRelease)
				 out.print("{\"data\" : \"" + xav.getName() + "\", \"attr\" : { \"id\" : \"" + xav.getOtid() + "\", \"rel\" : \"" + ObjectType.RELVERSION + "\" }}");
				else
				 out.print("{\"data\" : \"" + xav.getName() + "\", \"attr\" : { \"id\" : \"" + xav.getOtid() + "\", \"rel\" : \"" + ObjectType.APPVERSION + "\" }}");
	
				subv=true;
			}
		}
		out.println("]");
  }
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
