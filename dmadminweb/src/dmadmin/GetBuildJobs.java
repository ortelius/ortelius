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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.model.BuildJob;
import dmadmin.model.Component;

/**
 * Servlet implementation class GetProviderDefs
 */
public class GetBuildJobs
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetBuildJobs() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		JSONArray ret = new JSONArray();
		int compid = getIntParameter(request,"compid");
		String domstr = request.getParameter("domid");
		int domid=0;
		if (domstr != null && domstr.length()>0) {
			domid = Integer.parseInt(domstr);
		}
		System.out.println("GetBuildJobs, compid="+compid+" domid="+domid);
		Component comp = session.getComponent(compid,true);
		List<BuildJob> jobs = session.getBuildJobsForComponent(comp,domid);	
		for(BuildJob bj : jobs) {
			ret.add(bj.getLinkJSON());
		}
		return ret;
	}
}
