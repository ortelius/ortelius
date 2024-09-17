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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.DMSession;
import dmadmin.HttpServletBase;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;

/**
 * Servlet implementation class GetCalendarData
 */
public class GetCalendarData extends HttpServletBase
{
 private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public GetCalendarData()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 protected String EventTypeText(DMEventType et)
 {
  String ret = et.toString();
  return Character.toUpperCase(ret.charAt(0)) + ret.substring(1).toLowerCase();
 }

 @Override
 public void handleRequest(DMSession so, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  // TODO Auto-generated method stub
  response.setContentType("application/json");
  int envid = Integer.parseInt(request.getParameter("envid"));
  int start = Integer.parseInt(request.getParameter("start"));
  int end = Integer.parseInt(request.getParameter("end"));
  String Schedule = request.getParameter("schedule");

  System.out.println("POST start=" + start + " end=" + end);
  // Get the printwriter object from response to write the required json object to the output stream
  PrintWriter out = response.getWriter();

  try (DMSessionPro session = new DMSessionPro(so))
  {
   List<DMCalendarEvent> evs;
   boolean EditStatus = false;
   if (Schedule != null)
   {
    System.out.println("Getting Weekly Schedule");
    evs = session.getWeeklySchedule(envid);
    EditStatus = true;
   }
   else
   {
    evs = session.getCalendarEvents(envid, start, end);
    EditStatus = false;
   }

   System.out.println("EdiStatus=" + EditStatus);

   int id = 0;
   int DeployID = 0;
   String BackgroundColor = "yellow";
   String TextColor = "black";
   String Unavailable = "N";
   boolean Editable = EditStatus;

   JSONArray arr = new JSONArray();

   int me = session.GetUserID();
   boolean envowner = session.userOwnsEnvironment(envid);

   for (DMCalendarEvent ce : evs)
   {
    int eventowner = ce.getCreatorID();
    DMEventType et = ce.getEventType();

    if (ce.getPending())
    {
     System.out.println("Event " + ce.getEventTitle() + " is pending!");
     EditStatus = true;
    }
    else
    {
     // Not pending - have we been approved?
     System.out.println("Event " + ce.getEventTitle() + " is not pending!");
     EditStatus = !ce.isApproved();
     System.out.println("isApproved = " + ce.isApproved() + " EditStatus=" + EditStatus);
    }

    switch (et)
    {
     case NOEVENT:
      System.out.println("NOEVENT eventowner=" + eventowner + " me=" + me);
      BackgroundColor = "#525564";
      TextColor = "white";
      Unavailable = "N";
      Editable = false;
      DeployID = 0;
      break;
     case UNAVAILABLE:
      System.out.println("UNAVAILABLE eventowner=" + eventowner + " me=" + me + " EditStatus" + EditStatus);
      BackgroundColor = "#525564";
      TextColor = "white";
      Unavailable = "Y";
      Editable = (eventowner == me) ? EditStatus : envowner;
      DeployID = 0;
      break;
     case RESERVED:
      System.out.println("RESERVED eventowner=" + eventowner + " me=" + me);
      Unavailable = "N";
      Editable = (eventowner == me) ? EditStatus : envowner;
      BackgroundColor = "#3a87ad";
      TextColor = "white";
      DeployID = 0;
      break;
     case DEPLOYMENT:
      System.out.println("DEPLOYMENT eventowner=" + eventowner + " me=" + me);
      BackgroundColor = "#BEB9B5";
      TextColor = "white";
      Unavailable = "N";
      Editable = true;
      DeployID = ce.getDeployID();
      break;
     case AUTO:
      System.out.println("AUTO eventowner=" + eventowner + " me=" + me);
      BackgroundColor = "#61838F";
      TextColor = "white";
      Unavailable = "N";
      Editable = (eventowner == me) ? EditStatus : envowner;
      DeployID = 0;
      break;
     default:
      System.out.println("defaulting (don't know what the event type is)");
    }
    if (ce.getPending())
    {
     System.out.println("Record is marked as PENDING");
     BackgroundColor = "#525564";
     TextColor = "white";
    }

    Application app = session.getApplication(ce.getAppID(), true);

    arr.add(new Integer(id).toString())
      .add(new JSONObject().add("allDay", ce.getAllDayEvent()).add("id", new Integer(ce.getID()).toString()).add("title", ce.getEventTitle()).add("color", BackgroundColor).add("textColor", TextColor).add("editable", Editable).add("deployid", DeployID).add("unavailable", Unavailable)
        .add("description", ce.getEventDesc()).add("application", session.GetApplicationNameFromID(ce.getAppID())).add("appid", app.getObjectType().getTypeString() + app.getId()).add("eventtype", EventTypeText(et)).add("pending", ce.getPending()).add("start", new Integer(ce.getStart()).toString())
        .add("end", new Integer(ce.getEnd()).toString()));
    id++;
   }

   String ret = arr.getJSON();
   System.out.println(ret);
   out.println(ret);
  }
 }
}
