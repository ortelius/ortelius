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
 DMSession so = null;
 HttpSession session = null;      
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
		
  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());
  
		List<UserGroup> usergroups = so.getGroupsForTemplate(tid,true);
		List<User> users = so.getUsersForTemplate(tid,true);
		List<UserGroup> avgroups = so.getGroupsForTemplate(tid,false);
		List<User> avusers = so.getUsersForTemplate(tid,false);
		
		JSONObject obj = new JSONObject();
		JSONArray arr1 = new JSONArray();
		JSONArray arr2 = new JSONArray();
		
		for (UserGroup g: usergroups) {
			JSONObject gobj = new JSONObject();
			String id = "ag" + g.getId();
   gobj.add("id",id);
			gobj.add("name", g.getName());
			arr1.add(gobj);
			arr2.add(gobj);
		}
		for (User u: users) {
	  if (u.getId()<0) {
    JSONObject gobj = new JSONObject();
    String id = "as" + u.getId()*-1;
    gobj.add("id",id);
    gobj.add("name", u.getName());
    arr1.add(gobj);
    arr2.add(gobj);
   }
	  else
	  {
			 JSONObject uobj = new JSONObject();
			 String id = "au" + u.getId();
    uobj.add("id",id);
			 uobj.add("name", u.getName());
			 arr1.add(uobj);
    arr2.add(uobj);
	  }
		}
		
		for (UserGroup g: avgroups) {
			JSONObject gobj = new JSONObject();
			String id = "ag" + g.getId();
   gobj.add("id",id);
			gobj.add("name", g.getName());
			arr2.add(gobj);
		}
		for (User s: avusers) {
			if (s.getId()<0) 
			{
				JSONObject gobj = new JSONObject();
				String id = "as" + s.getId()*-1;
				gobj.add("id",id);
				gobj.add("name", s.getName());
				arr2.add(gobj);
			}
			else
			{
    JSONObject uobj = new JSONObject();
    String id = "au" + s.getId();
    uobj.add("id",id);
    uobj.add("name", s.getName());
    arr2.add(uobj);
   }
		}
		
		obj.add("Recipients", arr1);
		obj.add("Available", arr2);
		
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
