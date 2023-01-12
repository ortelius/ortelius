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
