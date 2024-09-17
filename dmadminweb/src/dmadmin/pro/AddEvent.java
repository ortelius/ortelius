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
 */package dmadmin.pro;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.DMSession;
import dmadmin.HttpServletBase;
import dmadmin.model.Application;

/**
 * Servlet implementation class AddEvent
 */
public class AddEvent extends HttpServletBase
{
 private static final long serialVersionUID = 1L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public AddEvent()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 @Override
 public void handleRequest(DMSession so, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  try (DMSessionPro session = new DMSessionPro(so))
  {
   // TODO Auto-generated method stub
   response.setContentType("application/json");
   int envid = Integer.parseInt(request.getParameter("envid"));
   int id = getIntParameter(request, "id");
   String szAppid = request.getParameter("appid");
   int appid = (szAppid.length() == 0) ? 0 : Integer.parseInt(szAppid);
   String EventTitle = request.getParameter("Title");
   String EventDesc = request.getParameter("desc");
   int start = Integer.parseInt(request.getParameter("start"));
   int end = Integer.parseInt(request.getParameter("end"));
   String szAllDay = request.getParameter("allDay");
   System.out.println("envid=" + envid + " appid=" + appid + " EventTitle=" + EventTitle + " start=" + start + " end=" + end + " allDay=" + szAllDay + " desc=" + EventDesc);
   boolean allDay = (szAllDay.equalsIgnoreCase("true"));
   String et = request.getParameter("et");
   System.out.println("et=" + et);
   DMEventType et2 = DMEventType.RESERVED;
   if (et.equalsIgnoreCase("unavailable"))
   {
    et2 = DMEventType.UNAVAILABLE;
   }
   else if (et.equalsIgnoreCase("auto"))
   {
    et2 = DMEventType.AUTO;
   }

   if (id == -1)
   {
    // New Event
    DMCalendarEvent ce = new DMCalendarEvent();
    ce.setAppID(appid);
    ce.setEnvID(envid);
    ce.setEventTitle(EventTitle);
    ce.setEventDesc(EventDesc);
    ce.setStart(start);
    ce.setEnd(end);
    ce.setAllDayEvent(allDay);
    ce.setCreatorID(session.GetUserID());
    ce.setEventType(et2);
    System.out.println("About to call AddEvent");
    int newid = session.AddEvent(ce);
    boolean pending = !session.userOwnsEnvironment(envid);
    if (pending)
    {
     // Add a Request entry for the environment owner
     session.AddPendingRequest(newid, appid, ce.getEventDesc());
    }
    Application app = session.getApplication(ce.getAppID(), true);
    String appidstr = app.getObjectType().getTypeString() + app.getId();

    PrintWriter out = response.getWriter();
    out.println("{\"id\": \"" + newid + "\",  \"appid\": \"" + appidstr + "\",  \"description\": \"" + EventDesc + "\", \"title\": \"" + EventTitle + "\", \"pending\": " + pending + "}");
    System.out.println("Returned from AddEvent (id=" + id + ")");
   }
   else
   {
    // Existing event
    System.out.println("Modifying existing event");
    DMCalendarEvent ce = session.getCalendarEvent(id);
    ce.setAppID(appid);
    ce.setEnvID(envid);
    ce.setEventTitle(EventTitle);
    ce.setEventDesc(EventDesc);
    ce.setStart(start);
    ce.setEnd(end);
    ce.setAllDayEvent(allDay);
    ce.setCreatorID(session.GetUserID());
    ce.setEventType(et2);
    session.ModifyEvent(ce);
    boolean pending = !session.userOwnsEnvironment(envid);
    PrintWriter out = response.getWriter();
    Application app = session.getApplication(ce.getAppID(), true);
    String appidstr = app.getObjectType().getTypeString() + app.getId();
    System.out.println("{\"id\": \"" + id + "\",  \"appid\": \"" + appidstr + "\",  \"description\": \"" + EventDesc + "\", \"title\": \"" + EventTitle + "\", \"pending\": " + pending + "}");
    out.println("{\"id\": \"" + id + "\",  \"appid\": \"" + appidstr + "\",  \"description\": \"" + EventDesc + "\", \"title\": \"" + EventTitle + "\", \"pending\": " + pending + "}");
   }
  }
 }
}
