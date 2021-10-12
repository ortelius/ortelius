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

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
//import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;

/**
 * Servlet implementation class GetCalendarData
 */
public class GetTimeToDeploy extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetTimeToDeploy() {
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
			ReportDataSet data = session.getTimeToDeployForApplication(appid);
			obj.add("data", data.getLineDataSeriesJSON());
		}
		else if (envid > 0) {
			ReportDataSet data = session.getTimeToDeployForEnvironment(envid);
			obj.add("data", data.getLineDataSeriesJSON());
		}
		else {
			ReportDataSet data = session.getTimeToDeployForServer(servid);
			obj.add("data", data.getLineDataSeriesJSON());
		}
		String ret = obj.toString();
		out.println(ret);
   	}
}
