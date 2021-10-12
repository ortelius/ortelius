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
