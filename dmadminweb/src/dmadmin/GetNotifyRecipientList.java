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
