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
import dmadmin.model.User;
import dmadmin.model.UserGroup;

/**
 * Servlet implementation class GetNotifyRecipientList
 */
public class GetNotifyRecipientList extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetNotifyRecipientList() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		response.setContentType("application/json;charset=UTF-8");
		String templateid = request.getParameter("id");
		int tid = Integer.parseInt(templateid);
		
		PrintWriter out = response.getWriter();
		
		HttpSession session = request.getSession();
		DMSession so = (DMSession)session.getAttribute("session");
		List<UserGroup> usergroups = so.getGroupsForTemplate(tid,true);
		List<User> users = so.getUsersForTemplate(tid,true);
		List<UserGroup> avgroups = so.getGroupsForTemplate(tid,false);
		List<User> avusers = so.getUsersForTemplate(tid,false);
		
		JSONObject obj = new JSONObject();
		JSONArray arr1 = new JSONArray();
		JSONArray arr2 = new JSONArray();
		JSONArray arr3 = new JSONArray();
		JSONArray arr4 = new JSONArray();
		
		int NumSpecials=0;
		int NumUsers=0;
		
		for (UserGroup g: usergroups) {
			JSONObject gobj = new JSONObject();
			gobj.add("id",g.getId());
			gobj.add("name", g.getName());
			gobj.add("type","group");
			arr1.add(gobj);
		}
		for (User u: users) {
			JSONObject uobj = new JSONObject();
			uobj.add("id",u.getId());
			uobj.add("name", u.getName());
			uobj.add("type","user");
			arr1.add(uobj);
		}
		
		for (User u: avusers) {
			if (u.getId()>0) {
				JSONObject uobj = new JSONObject();
				uobj.add("id",u.getId());
				uobj.add("name", u.getName());
				arr2.add(uobj);
				NumUsers++;
			}
		}
		for (UserGroup g: avgroups) {
			JSONObject gobj = new JSONObject();
			gobj.add("id",g.getId());
			gobj.add("name", g.getName());
			arr3.add(gobj);
		}
		for (User s: avusers) {
			if (s.getId()<0) {
				JSONObject gobj = new JSONObject();
				gobj.add("id",0-s.getId());
				gobj.add("name", s.getName());
				arr4.add(gobj);
				NumSpecials++;
			}
		}
		
		
		obj.add("Recipients", arr1);
		obj.add("RecipientCount", usergroups.size() + users.size());
		obj.add("AvailableUsers", arr2);
		obj.add("UserCount", NumUsers);
		obj.add("AvailableGroups", arr3);
		obj.add("UserGroupCount", avgroups.size());
		obj.add("SpecialUsers", arr4);
		obj.add("SpecialCount", NumSpecials);
		
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
