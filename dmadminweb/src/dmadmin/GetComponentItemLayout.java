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
