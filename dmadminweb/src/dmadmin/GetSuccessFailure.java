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
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;

/**
 * Servlet implementation class GetCalendarData
 */
public class GetSuccessFailure extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetSuccessFailure() {
        super();
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json");
    	int appid = ServletUtils.getIntParameter(request, "appid",-1);
		int servid = ServletUtils.getIntParameter(request, "servid", -1);
		int envid = ServletUtils.getIntParameter(request, "envid", -1);
		// Get the printwriter object from response to write the required json object to the output stream      
		PrintWriter out = response.getWriter();
		JSONObject obj = new JSONObject();
		if (appid >=0) {
			ReportDataSet data = session.getSuccessFailureForApplication(appid);
			obj.add("data", data.getLineDataSeriesJSON());
		}
		else if (envid>0) {
			ReportDataSet data = session.getSuccessFailureForEnvironment(envid);
			obj.add("data", data.getLineDataSeriesJSON());
		} else {
			ReportDataSet data = session.getSuccessFailureForServer(servid);
			obj.add("data", data.getLineDataSeriesJSON());
		}
		String ret = obj.toString();
		
		out.println(ret);
   	}
}
