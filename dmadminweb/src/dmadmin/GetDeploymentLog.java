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
