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
 * Servlet implementation class GetCompVersionLayout
 */
public class GetCompVersionLayout extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetCompVersionLayout() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json;charset=UTF-8");
		int compid = ServletUtils.getIntParameter(request, "compid");
		PrintWriter out = response.getWriter();
		Component comp = session.getComponent(compid,true);
		boolean bReadOnly = !comp.isUpdatable();
		
		List <Component> compvers = session.getComponentVersions(comp);
		
		JSONArray arr1 = new JSONArray();
		for(Component c: compvers) {
			JSONObject vobj = new JSONObject();
			vobj.add("nodeid", c.getId());
			vobj.add("name", c.getName());
			vobj.add("suffix", c.getIconSuffix());
			vobj.add("xpos", c.getXpos());
			vobj.add("ypos", c.getYpos());
			vobj.add("summary", c.getSummary());
			arr1.add(vobj);
		}

		JSONArray arr2 = new JSONArray();
		for(Component cl: compvers) {
			JSONObject vlobj = new JSONObject();
			vlobj.add("nodefrom", cl.getPredecessorId());
			vlobj.add("nodeto", cl.getId());
			vlobj.add("branch",cl.getLabel());
			arr2.add(vlobj);
		}

		JSONObject obj = new JSONObject();
		obj.add("Nodes", arr1);
		obj.add("Links", arr2);
		obj.add("LinkCount", compvers.size());
		obj.add("NodeCount", compvers.size());
		obj.add("readOnly", bReadOnly);
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
   	}
}
