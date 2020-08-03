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
import dmadmin.model.Component;
import dmadmin.model.ComponentItem;

/**
 * Servlet implementation class GetComponentItemLayout
 */
public class GetComponentItemLayout extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;      
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetComponentItemLayout() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setContentType("application/json;charset=UTF-8");
  response.setHeader("Cache-Control","no-cache");
  int compid = ServletUtils.getIntParameter(request, "compid");
  
  PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());
  
  Component comp = so.getComponent(compid,true);
  boolean bReadOnly = !comp.isUpdatable();

  List<ComponentItem> compitems = so.getComponentItems(compid);

   JSONArray arr1 = new JSONArray();
   for (ComponentItem ci : compitems)
   {
    JSONObject vobj = new JSONObject();
    vobj.add("nodeid", ci.getId());
    vobj.add("name", ci.getName());
    vobj.add("suffix", ci.getIconSuffix());
    vobj.add("xpos", ci.getXpos());
    vobj.add("ypos", ci.getYpos());
    vobj.add("summary", ci.getSummary());
    arr1.add(vobj);
   }

   JSONArray arr2 = new JSONArray();
   for (ComponentItem cil : compitems)
   {
    JSONObject vlobj = new JSONObject();
    vlobj.add("nodefrom", cil.getPredecessorId());
    vlobj.add("nodeto", cil.getId());
    arr2.add(vlobj);
   }

   JSONObject obj = new JSONObject();
   obj.add("Nodes", arr1);
   obj.add("Links", arr2);
   obj.add("LinkCount", compitems.size());
   obj.add("NodeCount", compitems.size());
   obj.add("readOnly", bReadOnly);
   String ret = obj.getJSON();
   System.out.println(ret);
   out.println(ret);
  }
 }

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
  response.setContentType("application/json;charset=UTF-8");
  int compid = ServletUtils.getIntParameter(request, "compid");
  // String rbrf = request.getParameter("rbrf");

  PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

  List<ComponentItem> compitems = so.getComponentItems(compid);

	 JSONArray arr1 = new JSONArray();
  for (ComponentItem ci : compitems)
  {
   JSONObject vobj = new JSONObject();
   vobj.add("id", ci.getId());
   vobj.add("rollback", ci.getRollback().toString());
   vobj.add("rollforward", ci.getRollup().toString());
   arr1.add(vobj);
  }
  JSONObject obj = new JSONObject();
  obj.add("data", arr1);
  String ret = obj.getJSON();
  System.out.println(ret);
  out.println(ret);
	}
	}
}
