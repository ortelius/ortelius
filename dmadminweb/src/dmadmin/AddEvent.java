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

import dmadmin.model.Application;
import dmadmin.model.DMCalendarEvent;

/**
 * Servlet implementation class AddEvent
 */
public class AddEvent extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public AddEvent() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		// TODO Auto-generated method stub
		response.setContentType("application/json");
		int envid = Integer.parseInt(request.getParameter("envid"));
		int id = getIntParameter(request,"id");
		String szAppid = request.getParameter("appid");
		int appid = (szAppid.length()==0)?0:Integer.parseInt(szAppid);
		String EventTitle = request.getParameter("Title");
		String EventDesc = request.getParameter("desc");
		int start = Integer.parseInt(request.getParameter("start"));
		int end = Integer.parseInt(request.getParameter("end"));
		String szAllDay = request.getParameter("allDay");
  		System.out.println("envid="+envid+" appid="+appid+" EventTitle="+EventTitle+" start="+start+" end="+end+" allDay="+szAllDay+" desc="+EventDesc);
  		boolean allDay = (szAllDay.equalsIgnoreCase("true"));
  		String et = request.getParameter("et");
  		System.out.println("et="+et);
  		DMEventType et2 = DMEventType.RESERVED;
  		if (et.equalsIgnoreCase("unavailable")) {
  			et2 = DMEventType.UNAVAILABLE;
  		}
  		else
  		if (et.equalsIgnoreCase("auto")) {
  			et2 = DMEventType.AUTO;
  		}
  		
  		if (id == -1) {
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
	  		if (pending) {
	  			// Add a Request entry for the environment owner
	  			session.AddPendingRequest(newid,appid,ce.getEventDesc());
	  		}
	    Application app = session.getApplication(ce.getAppID(),true);
	  		String appidstr =  app.getObjectType().getTypeString() + app.getId();
	  		  
	  		PrintWriter out = response.getWriter();
	  		out.println("{\"id\": \""+newid+"\",  \"appid\": \""+ appidstr +"\",  \"description\": \""+EventDesc+"\", \"title\": \""+EventTitle+"\", \"pending\": "+pending+"}");
	  		System.out.println("Returned from AddEvent (id="+id+")");
  		} else {
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
	  		Application app = session.getApplication(ce.getAppID(),true);
	  		String appidstr =  app.getObjectType().getTypeString() + app.getId();
	  		System.out.println("{\"id\": \""+id+"\",  \"appid\": \""+ appidstr +"\",  \"description\": \""+EventDesc+"\", \"title\": \""+EventTitle+"\", \"pending\": "+pending+"}");
	  		out.println("{\"id\": \""+id+"\",  \"appid\": \""+ appidstr +"\",  \"description\": \""+EventDesc+"\", \"title\": \""+EventTitle+"\", \"pending\": "+pending+"}");
  		}
	}
}
