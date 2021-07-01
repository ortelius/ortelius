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

/**
 * Servlet implementation class ChangePassword
 */
public class TableFilter extends HttpServlet
{
 private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public TableFilter()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setHeader("Content-Type", "application/json");
  
  try (DMSession so = DMSession.getInstance(request))
  {
   session = request.getSession();
   session.setAttribute("session", so);
   so.checkConnection(request.getServletContext());
   PrintWriter out = response.getWriter();
   
   String filtername = request.getParameter("filtername");
   String filterjson = so.getTableFilter(filtername);

   out.println(filterjson);
   
  }
 }

 /**
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setHeader("Content-Type", "application/json");
  
  try (DMSession so = DMSession.getInstance(request))
  {
   session = request.getSession();
   session.setAttribute("session", so);
   so.checkConnection(request.getServletContext());

   PrintWriter out = response.getWriter();
   String filtername = request.getParameter("filtername");
   String filtervalue = request.getParameter("filtervalue");
   JSONObject ret = new JSONObject();

   if (so.saveTableFilter(filtername,filtervalue))
   {
    ret.add("result", true);
    out.println(ret.getJSON());    
   }
   else
   {
     ret.add("result", false);
     out.println(ret.getJSON());
   }
  }
 }
}
