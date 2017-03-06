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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.DMCalendarEvent;

/**
 * Servlet implementation class GetCalendarData
 */
public class GetCalendarData extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetCalendarData() {
        super();
        // TODO Auto-generated constructor stub
    }

    protected String EventTypeText(DMEventType et)
    {
    	String ret = et.toString();
    	return Character.toUpperCase(ret.charAt(0))+ret.substring(1).toLowerCase();
    }
	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		response.setContentType("application/json");
		int envid = Integer.parseInt(request.getParameter("envid"));
		int start = Integer.parseInt(request.getParameter("start"));
		int end = Integer.parseInt(request.getParameter("end"));
		String Schedule = request.getParameter("schedule");
		
		
		System.out.println("POST start="+start+" end="+end);
		// Get the printwriter object from response to write the required json object to the output stream      
		PrintWriter out = response.getWriter();
		
		HttpSession session = request.getSession();
		DMSession so = (DMSession)session.getAttribute("session");
		
		List<DMCalendarEvent> evs;
		boolean EditStatus=false;
		if (Schedule != null)
		{
			System.out.println("Getting Weekly Schedule");
			evs = so.getWeeklySchedule(envid);
			EditStatus=true;
		}
		else
		{
			evs = so.getCalendarEvents(envid,start,end);
			EditStatus=false;
		}
		
		System.out.println("EdiStatus="+EditStatus);
		
		int id=0;
		int DeployID=0;
		String BackgroundColor="yellow";
		String TextColor="black";
		String Unavailable="N";
		boolean Editable=EditStatus;
		
		JSONArray arr = new JSONArray();
		
		int me = so.GetUserID();
		boolean envowner=so.userOwnsEnvironment(envid);
		
		for (DMCalendarEvent ce: evs)
		{
			int eventowner = ce.getCreatorID();
			DMEventType et = ce.getEventType();
			
			if (ce.getPending()) {
				System.out.println("Event "+ce.getEventTitle()+" is pending!");
				EditStatus=true;
			} else {
				// Not pending - have we been approved?
				System.out.println("Event "+ce.getEventTitle()+" is not pending!");
				EditStatus = !ce.isApproved();
				System.out.println("isApproved = "+ce.isApproved()+" EditStatus="+EditStatus);
			}
			
			switch(et)
			{
			case NOEVENT:
				System.out.println("NOEVENT eventowner="+eventowner+" me="+me);
				BackgroundColor="#525564";
				TextColor="white";
				Unavailable="N";
				Editable=false;
				DeployID=0;
				break;
			case UNAVAILABLE:
				System.out.println("UNAVAILABLE eventowner="+eventowner+" me="+me+" EditStatus"+EditStatus);
				BackgroundColor="#525564";
				TextColor="white";
				Unavailable="Y";
				Editable=(eventowner==me)?EditStatus:envowner;
				DeployID=0;
				break;
			case RESERVED:
				System.out.println("RESERVED eventowner="+eventowner+" me="+me);
				Unavailable="N";
				Editable=(eventowner==me)?EditStatus:envowner;
				BackgroundColor="#3a87ad";
				TextColor="white";
				DeployID=0;
				break;
			case DEPLOYMENT:
				System.out.println("DEPLOYMENT eventowner="+eventowner+" me="+me);
				BackgroundColor="#BEB9B5";
				TextColor="white";
				Unavailable="N";
				Editable=true;
				DeployID=ce.getDeployID();
				break;
			case AUTO:
				System.out.println("AUTO eventowner="+eventowner+" me="+me);
				BackgroundColor="#61838F";
				TextColor="white";
				Unavailable="N";
				Editable=(eventowner==me)?EditStatus:envowner;
				DeployID=0;
				break;
			default:
				System.out.println("defaulting (don't know what the event type is)");
			}
			if (ce.getPending()) {
				System.out.println("Record is marked as PENDING");
				BackgroundColor="#525564";
				TextColor="white";
			}
			
		 Application app =	so.getApplication(ce.getAppID(),true);
			
			arr.add(new Integer(id).toString())
				.add(new JSONObject()
					.add("allDay", ce.getAllDayEvent())
					.add("id", new Integer(ce.getID()).toString())
					.add("title", ce.getEventTitle())
					.add("color", BackgroundColor)
					.add("textColor", TextColor)
					.add("editable", Editable)
					.add("deployid", DeployID)
					.add("unavailable", Unavailable)
					.add("description", ce.getEventDesc())
					.add("application", so.GetApplicationNameFromID(ce.getAppID()))
					.add("appid", app.getObjectType().getTypeString() + app.getId()) 
					.add("eventtype", EventTypeText(et))
					.add("pending",ce.getPending())
					.add("start", new Integer(ce.getStart()).toString())
					.add("end", new Integer(ce.getEnd()).toString()));
			id++;
		}
		
		String ret = arr.getJSON();
		System.out.println(ret);
		out.println(ret);
	}

}
