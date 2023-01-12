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

import java.io.PrintWriter;

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;

/**
 * Servlet implementation class Engine
 */
public class EngineEvent extends HttpServlet
{
 private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

 public EngineEvent()
 {
  super();
 }

 protected void doPost(HttpServletRequest request, HttpServletResponse response)
 {
  try (DMSession so = DMSession.getInstance(request)) {
  try
  {
   response.setContentType("application/text");

   session = request.getSession();
   session.setAttribute("session", so);
   so.checkConnection(request.getServletContext());

   String clientid = request.getParameter("clientid");
   String id = request.getParameter("id");
   String output = request.getParameter("output");
   String exitcode = request.getParameter("exitcode");

   so.addWaitForOutput(id, clientid, output, exitcode);
  }
  catch (Exception e)
  {
   System.out.println(e.getMessage());
  }
  }
 }

 protected void doGet(HttpServletRequest request, HttpServletResponse response)
 {

  try (DMSession so = DMSession.getInstance(request)) {
  try
  {
   PrintWriter out = response.getWriter();
   response.setContentType("application/text");

   session = request.getSession();
   session.setAttribute("session", so);
   so.checkConnection(request.getServletContext());

   String keys = request.getParameter("getkeys");
   String clientid = request.getParameter("clientid");
   String hostname = request.getParameter("hostname");
   String sinceStr = request.getParameter("since");
   String getevents = request.getParameter("getevents");

   if (getevents != null && getevents.equalsIgnoreCase("y"))
   {
    out.println(so.getListenNotifyEvents(clientid));
   }
   else
   {
    Long since = null;

    if (sinceStr != null)
     since = new Long(sinceStr);

    if (clientid == null)
    {
     JSONObject ret = new JSONObject();
     ret.add("error", "Y");
     ret.add("message", "invalid clientid");

     out.println(ret.getJSON());
    }
    else
    {
     System.out.println("clientid=" + clientid);
     if (so.confirmClientID(clientid))
     {
      so.saveClientID(clientid, hostname);

      try
      {

       if (keys != null)
       {
        out.println(so.getCryptKeys(clientid));
       }
       else
       {
        boolean found = false;
        int sleepcnt = 0;
        do
        {
         found = so.getListenNotify(clientid, since);
         if (!found)
         {
          Thread.sleep(500);
          sleepcnt++;
         }

         if (sleepcnt > 60)
          break;
        }
        while (!found);

        JSONObject ret = new JSONObject();
        if (sleepcnt > 60)
        {
         ret.add("error", "Y");
         ret.add("message", "timeout");
        }
        else
        {
         ret.add("error", "N");
         ret.add("message", "found");
        }

        out.println(ret.getJSON());
       }
      }
      finally
      {
      }
     }
     else
     {
      JSONObject ret = new JSONObject();
      ret.add("error", "Y");
      ret.add("message", "invalid clientid");

      out.println(ret.getJSON());
     }
    }
   }
  }
  catch (Exception e)
  {
   System.out.println(e.getMessage());
  }
 }
 }
}
