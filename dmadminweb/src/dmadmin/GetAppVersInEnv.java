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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.model.Application;
import dmadmin.model.Environment;

/**
 * Servlet implementation class GetAppVersInEnv
 */
public class GetAppVersInEnv
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    public GetAppVersInEnv() {
        super();
    }

	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int envid = getIntParameter(request, "envid", 0);
		int appid = getIntParameter(request, "appid", 0);
		if((envid == 0) && (appid == 0)) {
			throw new RuntimeException("Either envid or appid must be specified");
		}
		if(envid > 0) {
			Environment env = session.getEnvironment(envid, false);
			request.setAttribute("env", env);
			request.setAttribute("mode", "apps");
		}
		if(appid > 0) {
			Application app = session.getApplication(appid, false);
			request.setAttribute("app", app);
			request.setAttribute("mode", "envs");
		}
		request.setAttribute("admin", getStringParameter(request, "admin", "N").equalsIgnoreCase("Y"));
		request.getRequestDispatcher("/WEB-INF/GetAppsInEnv.jsp").forward(request, response);		
	}
}
