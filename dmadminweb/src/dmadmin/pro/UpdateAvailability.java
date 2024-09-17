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
// import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.DMSession;
import dmadmin.HttpServletBase;

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
   	public void handleRequest(DMSession so, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{

     try (DMSessionPro session = new DMSessionPro(so))
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
}
