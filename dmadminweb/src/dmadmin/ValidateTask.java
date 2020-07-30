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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
//import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
//import dmadmin.model.User;

/**
 * Servlet implementation class GetUsers
 */
public class ValidateTask
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ValidateTask() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		String TaskType = request.getParameter("tt");
		String IssueText = null;
		if (TaskType.equalsIgnoreCase("deploy")) {
			// Validate Deploy
			int appid = getIntParameter(request,"appid");
			int envid = getIntParameter(request,"envid");
			IssueText = session.ValidateDeploy(appid,envid);
		}
		else
		if (TaskType.equalsIgnoreCase("move")) {
			// Validate Move task
			int appid = getIntParameter(request,"appid");
			int tgtid = getIntParameter(request,"tgt");
			int srcid = getIntParameter(request,"d"); 
			IssueText = session.ValidateMove(appid,tgtid,srcid);
		}
		JSONObject ret = new JSONObject();
		ret.add("success",(IssueText==null)?true:false);
		ret.add("errtext",(IssueText!=null)?IssueText:"");
		return ret;
	}
}

