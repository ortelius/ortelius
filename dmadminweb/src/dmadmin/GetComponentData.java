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
import dmadmin.model.CompType;
import dmadmin.model.Component;
import dmadmin.model.DMObject;


/**
 * Servlet implementation class GetComponentsForApplication
 */
public class GetComponentData extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;      
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetComponentData() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

  response.setContentType("application/json");

  PrintWriter out = response.getWriter();

  System.out.println("In GetComponentData");

  String comptype = request.getParameter("comptype");
  String compidStr = request.getParameter("compid");

  if (compidStr != null)
  {
   int compid = new Integer(compidStr).intValue();
   JSONArray ret = so.getComp2Endpoints(compid);
   
   out.println(ret);
   System.out.println(ret);
  }
  else if (comptype != null)
  {
   List<DMObject> ct = so.getCompTypes();
   
   JSONArray arr = new JSONArray();
   for (DMObject x : ct)
   {
    CompType ctype = (CompType)x;
    JSONObject obj = new JSONObject();
    obj.add("id",ctype.getId());
    obj.add("name",ctype.getFullName());
    obj.add("database",ctype.getDatabase());
    obj.add("deletedir",ctype.getDeletedir());
    
    arr.add(obj);
   }
   JSONObject obj = new JSONObject();
   obj.add("data", arr);

   String ret = obj.toString();

   out.println(ret);
   System.out.println(ret);
  }
  else
  {
   int appid = ServletUtils.getIntParameter(request, "appid");

   System.out.println("appid=" + appid);

   List<Component> comp = so.getComponents(ObjectType.APPLICATION, appid, false);

   JSONArray arr = new JSONArray();
   for (Component x : comp)
   {
    JSONArray a = new JSONArray();
    a.add(x.getName());
    a.add(x.getSummary());
    arr.add(a);
   }

   JSONObject obj = new JSONObject();
   obj.add("data", arr);

   String ret = obj.toString();

   out.println(ret);
   System.out.println(ret);
  }
  }

	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
