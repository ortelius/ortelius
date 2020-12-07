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
