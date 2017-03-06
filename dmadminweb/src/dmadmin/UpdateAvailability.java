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
// import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

/**
 * Servlet implementation class UpdateAvailability
 */
public class UpdateAvailability extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UpdateAvailability() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
   	public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	int envid = getIntParameter(request,"envid");
		
		String m = request.getParameter("m");
		System.out.println("envid="+envid+" m="+m);
		if (m.equalsIgnoreCase("a")) {
			int c = getIntParameter(request,"c");
			session.DeleteAllAvailability(envid);
			for (int i=0;i<c;i++)
			{
				int start = getIntParameter(request,"start"+i);
				int end = getIntParameter(request,"end"+i);
				System.out.println("start="+start+" end="+end);
				session.AddAvailability(envid,start,end);
			}
		}
		else
		if (m.equalsIgnoreCase("s")) {
			// Change Calendar Start Time
			int newtime = getIntParameter(request,"nt");
			System.out.println("Changing calendar start time to "+newtime);
			session.setCalendarStartTime(envid,newtime);
		}
		else
		if (m.equalsIgnoreCase("e")) {
			// Change Calendar End Time
			int newtime = getIntParameter(request,"nt");
			System.out.println("Changing calendar end time to "+newtime);
			session.setCalendarEndTime(envid,newtime);
		}
		else
		if (m.equalsIgnoreCase("t")) {
			// Change Calendar availability type
			String na = request.getParameter("na");
			System.out.println("Changing calendar availability to "+na);
			session.setCalendarAvailability(envid,na);
		}
   	}
}
