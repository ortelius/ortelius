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
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;
import dmadmin.model.Deployment;


/**
 * Servlet implementation class GetDeploymentStepDetailsData
 */
public class GetDeploymentStepDetailsData
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetDeploymentStepDetailsData() {
        super();
    }
    
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
		response.setContentType("application/json");
		PrintWriter out = response.getWriter();
		int depid = ServletUtils.getIntParameter(request, "id");
		int stepid = ServletUtils.getIntParameter(request, "step");
		int instid = ServletUtils.getIntParameter(request, "inst");
	
		Deployment dep = session.getDeployment(depid, false);
		
		JSONObject obj = new JSONObject();
		obj.add("readOnly", true);
		obj.add("data", dep.getStepDetails(stepid, instid).getStepJSON());
		
		String ret = obj.toString();
		
		out.println(ret);
		System.out.println(ret);	
   	}
}
