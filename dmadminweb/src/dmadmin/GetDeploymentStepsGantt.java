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

/**
 * Servlet implementation class GetDeploymentStepsGantt
 */
public class GetDeploymentStepsGantt
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetDeploymentStepsGantt() {
        super();
    }
    
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json");
		int deployid = ServletUtils.getIntParameter(request, "deployid");
	
		PrintWriter out = response.getWriter();
		
		GanttDataSet data = session.getDeploymentStepsGantt(deployid);
		
		JSONObject obj = new JSONObject();
		obj.add("ticks", data.getTicksJSON());
		obj.add("series", data.getSeriesJSON());
		//obj.add("linePos", data.getNumberOfServers());
		obj.add("canvasOverlay", data.getOverlayJSON());
		obj.add("data", data.getDataJSON());
		String ret = obj.toString();
		
		out.println(ret);
		System.out.println(ret);
   	}
}
