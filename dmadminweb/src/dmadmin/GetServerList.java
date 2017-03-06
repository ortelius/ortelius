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
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Server;

/**
 * Servlet implementation class GetActionLayout
 */
public class GetServerList extends HttpServletBase
{
 private static final long serialVersionUID = 1L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public GetServerList()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setContentType("text/html;charset=UTF-8");
  int envid = ServletUtils.getIntParameter(request, "envid");
  String m = request.getParameter("m");

  PrintWriter out = response.getWriter();

  List<Server> sl = session.getServers(envid, m.equalsIgnoreCase("u"));

  JSONArray list = new JSONArray();
  for (Server s : sl)
  {
   JSONObject o = new JSONObject();
   o.add("id", s.getId());
   o.add("name", s.getName());
   list.add(o);
  } 
  
  out.println(list.toString());
 }
}
