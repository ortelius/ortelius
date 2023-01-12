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
//import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Server;
import dmadmin.model.ServerLink;

/**
 * Servlet implementation class GetActionLayout
 */
public class GetServerLayout extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetServerLayout() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json;charset=UTF-8");
		String envid = request.getParameter("envid");

		PrintWriter out = response.getWriter();

		List <Server> servers = session.getServersInEnvironment(Integer.parseInt(envid));
		List <ServerLink> sls = session.GetServerLinks(Integer.parseInt(envid));

		JSONArray arr1 = new JSONArray();
		for(Server s: servers) {
			JSONObject sobj = new JSONObject();
			sobj.add("nodeid", s.getId());
			sobj.add("name", s.getDomain().getFullDomain() + "." + s.getName());
			sobj.add("xpos", s.getXpos());
			sobj.add("ypos", s.getYpos());
			sobj.add("typeid", s.getTypeName());
			sobj.add("summary", s.getSummary());
			sobj.add("hostname", s.getHostName());
			arr1.add(sobj);
		}

		JSONArray arr2 = new JSONArray();
		for(ServerLink sl: sls) {
			JSONObject slobj = new JSONObject();
			slobj.add("nodefrom", sl.getNodeFrom());
			slobj.add("nodeto", sl.getNodeTo());
			slobj.add("fromside", sl.getSideFrom());
			slobj.add("toside", sl.getSideTo());
			slobj.add("label",sl.getLabel());
			arr2.add(slobj);
		}

		JSONObject obj = new JSONObject();
		obj.add("Nodes", arr1);
		obj.add("Links", arr2);
		obj.add("LinkCount", sls.size());
		obj.add("NodeCount", servers.size());

		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
   	}
}
