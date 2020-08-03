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
