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

