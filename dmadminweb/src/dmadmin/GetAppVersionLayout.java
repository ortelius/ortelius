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
import dmadmin.model.Domain;

/**
 * Servlet implementation class GetActionLayout
 */
public class GetAppVersionLayout extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetAppVersionLayout() {
        super();
        // TODO Auto-generated constructor stub
    }
    


	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		response.setContentType("application/json;charset=UTF-8");
		int appid = ServletUtils.getIntParameter(request, "appid");
		
		PrintWriter out = response.getWriter();
		
		HttpSession session = request.getSession();
		DMSession so = (DMSession)session.getAttribute("session");

		Application app = so.getApplication(appid,true);
		Domain domain = app.getDomain();
		String BaseDomain="";
		if (domain != null) {
			BaseDomain = domain.getName();
		}
		String BaseSummary = app.getSummary();
		
		List <Application> appvers = so.getApplicationVersions(app);
		
		JSONArray arr1 = new JSONArray();
		for(Application a: appvers) {
			JSONObject vobj = new JSONObject();
			vobj.add("nodeid", a.getId());
			vobj.add("name", a.getName());
			vobj.add("xpos", a.getXpos());
			vobj.add("ypos", a.getYpos());
			vobj.add("summary", a.getSummary());
			vobj.add("domain",a.getDomain().getName());
			arr1.add(vobj);
		}

		JSONArray arr2 = new JSONArray();
		for(Application al: appvers) {
			JSONObject vlobj = new JSONObject();
			vlobj.add("nodefrom", al.getPredecessorId());
			vlobj.add("nodeto", al.getId());
			vlobj.add("branch",al.getLabel());
			arr2.add(vlobj);
		}

		JSONObject obj = new JSONObject();
		obj.add("Nodes", arr1);
		obj.add("Links", arr2);
		obj.add("LinkCount", appvers.size());
		obj.add("NodeCount", appvers.size());
		obj.add("BaseDomain",BaseDomain);
		obj.add("BaseSummary",BaseSummary);
		
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}

