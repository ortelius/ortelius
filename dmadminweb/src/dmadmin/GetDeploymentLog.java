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
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Deployment;
import dmadmin.model.Deployment.DeploymentLogEntry;

/**
 * Servlet implementation class GetDeploymentLog
 */
public class GetDeploymentLog
	extends JSONServletBase
{
	private static final long serialVersionUID = 4402903956198244069L;


    /**
     * @see JSONServletBase#JSONServletBase()
     */
    public GetDeploymentLog() {
        super();
    }


	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int deployid = getIntParameter(request, "deployid");
		int lineno = getIntParameter(request, "lineno", 0);

		System.out.println("deployid = " + deployid + "; lineno = " + lineno);

		Deployment dep = session.getDeployment(deployid, false);
		List<DeploymentLogEntry> log = new ArrayList<DeploymentLogEntry>();
  JSONObject ret = new JSONObject();

		if (dep != null)
		{
		 log = dep.getLogSinceLine(lineno);

		 System.out.println("log.size() = " + log.size() + "; complete = " + dep.isComplete());

		 ret.add("complete", dep.isComplete());
		 if(dep.isComplete()) {
		 	ret.add("icon", dep.getLargeIcon());
		 	ret.add("exitcode", dep.getExitCode());
		 }
		}

		JSONArray arr = new JSONArray();
		ret.add("log", arr);

		for(DeploymentLogEntry entry: log) {
			arr.add(entry.getJSON());
		}
		return ret;
	}

}
