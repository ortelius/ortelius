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
			sobj.add("name", s.getName());
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
