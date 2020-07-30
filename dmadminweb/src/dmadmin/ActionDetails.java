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
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.model.Action;
import dmadmin.model.Domain;
import dmadmin.model.Engine;
import dmadmin.util.CommandLine;

/**
 * Servlet implementation class ActionDetails
 */
public class ActionDetails
	extends HttpServletBase
{
	private static final long serialVersionUID = -226890201780242001L;

	
   public ActionDetails() {
        super();
    }

    
	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int actionid = getIntParameter(request, "actionid");
		Action action = session.getAction(actionid, true, ObjectType.ACTION);
		Domain domain = action.getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return;
		}
		CommandLine cmd = engine.showDMScript(action);
		int res = cmd.runWithTrilogy(true, null);
		response.setContentType("text/plain");
		PrintWriter out = response.getWriter();
		
		if (res == 0) {
			String DMScript = cmd.getOutput();
	  		out.println(DMScript);
	  		System.out.println("Returned from ActionDetails (id="+action.getId()+")");
		} else {
			// Failed to execute
			String errtext = cmd.getOutput();
			out.println("ERROR: "+errtext);
		}
	}
}
