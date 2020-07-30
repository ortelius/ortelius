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
import dmadmin.model.LoginException;
import dmadmin.model.LoginException.LoginExceptionType;

/**
 * Servlet implementation class ChangePassword
 */
public class ChangePassword extends HttpServlet
{
 private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public ChangePassword()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
 }

 /**
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  System.out.println("In ChangePassword post");

  try (DMSession so = DMSession.getInstance(request))
  {
   session = request.getSession();
   session.setAttribute("session", so);
   so.checkConnection(request.getServletContext());

   PrintWriter out = response.getWriter();
   String oldpass = request.getParameter("oldpw");
   String newpass = request.getParameter("newpw");
   String myuserid = request.getParameter("username");
   System.out.println("newpw=" + newpass);
   JSONObject ret = new JSONObject();

   LoginException le = so.Login(myuserid, oldpass, null);

   if (le != null && le.exceptionType != LoginExceptionType.LOGIN_OKAY &&  le.exceptionType != LoginExceptionType.LOGIN_CHANGE_PASSWORD)
   {
    ret.add("res", false);
    ret.add("errtext", le.getMessage());
    out.println(ret.getJSON());
   }
   else
   {
    String ResultText = so.ChangePassword(oldpass, newpass);

    if (ResultText != null)
    {
     ret.add("res", false);
     ret.add("errtext", ResultText);
     out.println(ret.getJSON());
    }
    else
    {
     ret.add("res", true);
     ret.add("errtext", "");
     out.println(ret.getJSON());
    }
   }
  }
 }
}
