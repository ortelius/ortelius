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
