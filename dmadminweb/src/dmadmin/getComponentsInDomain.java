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
import java.io.PrintWriter;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Component;


/**
 * Servlet implementation class GetCategories
 */
public class getComponentsInDomain extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public getComponentsInDomain() {
        super();
        // TODO Auto-generated constructor stub
    }
    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json;charset=UTF-8");
		
		PrintWriter out = response.getWriter();
		
		boolean IncludeVersions = false;
		if (request.getParameter("cvs").equalsIgnoreCase("Y"))
		{
			IncludeVersions=true;
		}
		List <Component> components = session.getComponentsInDomain(IncludeVersions);
		
		JSONArray arr1 = new JSONArray();
		for(Component c: components) {
			JSONObject cobj = new JSONObject();
			cobj.add("id", c.getId());
			cobj.add("name", c.getName());
			cobj.add("summary",c.getSummary());
			List <Component> compvers = c.getVersions();
			JSONArray arr2 = new JSONArray();
			if (compvers != null) {
				for(Component cv: compvers) {
					JSONObject vobj = new JSONObject();
					vobj.add("id",cv.getId());
					vobj.add("name",cv.getName());
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
		obj.add("Components", arr1);
		obj.add("ComponentCount", components.size());
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
   	}
}
