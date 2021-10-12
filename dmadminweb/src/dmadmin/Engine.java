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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;
import dmadmin.model.User;

/**
 * Servlet implementation class Engine
 */
public class Engine extends HttpServlet
{
 private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;
 
 public Engine()
 {
  super();
 }

 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  PrintWriter out = response.getWriter();
  response.setContentType("application/json");
  
  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

  String userid = request.getParameter("userid");
  if (userid == null || userid.length() == 0)
  {
   JSONObject ret = new JSONObject();
   ret.add("runningcnt", "?");
   ret.add("totalcnt", "?");
   out.println(ret.getJSON());
  }
  else
  {
   User user = so.getUserByName(userid);

   JSONObject result = so.getSaasEnginesStatus(new Integer(user.getDomainId()).intValue());
   out.println(result.getJSON());
  }
  }
 }
}
