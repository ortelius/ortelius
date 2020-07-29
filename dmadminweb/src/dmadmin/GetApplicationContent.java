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
