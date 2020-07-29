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

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;

/**
 * Servlet implementation class getApplicationsInDomain
 */
public class getApplicationsInDomain extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;      
    /**
     * @see HttpServlet#HttpServlet()
     */
    public getApplicationsInDomain() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("application/json;charset=UTF-8");
		
		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		boolean IncludeVersions = false;
		boolean IncludeReleases = true;
		//if (request.getParameter("cvs").equalsIgnoreCase("Y"))
		//{
			IncludeVersions=true;
		//}
		String ir = request.getParameter("ir");
		System.out.println("ir = "+((ir != null)?ir:"NULL"));
		if (ir != null && ir.equalsIgnoreCase("N")) IncludeReleases=false;
		List <Application> applications = so.getApplicationsInDomain(IncludeVersions,IncludeReleases);
		
		JSONArray arr1 = new JSONArray();
		for(Application c: applications) {
			JSONObject cobj = new JSONObject();
			cobj.add("id", c.getId());
			cobj.add("name", c.getName());
			cobj.add("isrelease", c.getIsRelease());
			cobj.add("summary",c.getSummary());
			List <Application> compvers = c.getVersions();
			JSONArray arr2 = new JSONArray();
			if (compvers != null) {
				for(Application cv: compvers) {
					JSONObject vobj = new JSONObject();
					vobj.add("id",cv.getId());
					vobj.add("name",cv.getName());
					vobj.add("isrelease", c.getIsRelease());
					vobj.add("summary",cv.getSummary());
					arr2.add(vobj);
				}
				cobj.add("vercount",compvers.size());
			} else {
				cobj.add("vercount",0);
			}
			cobj.add("versions",arr2);
			arr1.add(cobj);
		}
		
		JSONObject obj = new JSONObject();
		obj.add("Applications", arr1);
		obj.add("ApplicationCount", applications.size());
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
  }
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
