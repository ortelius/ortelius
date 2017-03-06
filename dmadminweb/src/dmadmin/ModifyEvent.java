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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;


import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.DMCalendarEvent;

/**
 * Servlet implementation class ModifyEvent
 */
public class ModifyEvent extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ModifyEvent() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
    	   		HttpServletRequest request, HttpServletResponse response)
    	   throws ServletException, IOException
    {
    	response.setContentType("application/json");
		String m = request.getParameter("m");
		int id = Integer.parseInt(request.getParameter("id"));
		
		if (m.equalsIgnoreCase("d"))
		{
			// Delete mode
			session.DeleteEvent(id);
	  PrintWriter out = response.getWriter();

	  JSONObject ret = new JSONObject().add("id", new Integer(id).toString());
	  out.println(ret.getJSON().toString());
	  return;
		}
		else
		if (m.equalsIgnoreCase("m"))
		{
			// Modify mode
			DMCalendarEvent ce = session.getCalendarEvent(id);
			int s = getIntParameter(request,"s");
			int e = getIntParameter(request,"e");
			String ad = request.getParameter("ad");
			
			ce.setStart(s);
			ce.setEnd(e);
			ce.setAllDayEvent(ad.equalsIgnoreCase("y"));
			System.out.println("Calling ModifyEvent with new CalendarEvent");
			session.ModifyEvent(ce);
		}
		else
		if (m.equalsIgnoreCase("a"))
		{
			// approve mode
			session.ApproveEvent(id);
		}
		DMCalendarEvent ce2 = session.getCalendarEvent(id);
		String BackgroundColor="#525564";
		String TextColor="white";
		if (ce2.getPending()) {
		 BackgroundColor="#525564";
			TextColor="white";
		} else {
			switch(ce2.getEventType()) {
			case AUTO:
			 BackgroundColor="#61838F";
				TextColor="white";
				break;
			case RESERVED:
			 BackgroundColor="#3a87ad";
				TextColor="white";
				break;
			case UNAVAILABLE:
			 BackgroundColor="#525564";
				TextColor="white";
				break;
			default:
				break;
			}
		}
		PrintWriter out = response.getWriter();
		
	 Application app = session.getApplication(ce2.getAppID(),true);
		
		JSONObject ret = new JSONObject()
		.add("success",true)
		.add("allDay", ce2.getAllDayEvent())
		.add("id", new Integer(ce2.getID()).toString())
		.add("title", ce2.getEventTitle())
		.add("color", BackgroundColor)
		.add("textColor", TextColor)
		.add("editable", false)
		.add("deployid", ce2.getDeployID())
		.add("unavailable", false)
		.add("description", ce2.getEventDesc())
		.add("application", session.GetApplicationNameFromID(ce2.getAppID()))
		.add("appid", app.getObjectType().getTypeString() + app.getId()) 
		.add("eventtype", ce2.getEventTypeString())
		.add("pending",ce2.getPending())
		.add("start", new Integer(ce2.getStart()).toString())
		.add("end", new Integer(ce2.getEnd()).toString());
		
		String ret2 = ret.getJSON();
		System.out.println(ret2);
		out.println(ret2);
    }
}
