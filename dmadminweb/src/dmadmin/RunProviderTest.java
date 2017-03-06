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

//import java.io.BufferedReader;
import java.io.IOException;
//import java.io.InputStream;
//import java.io.InputStreamReader;
import java.io.PrintWriter;
//import java.util.ArrayList;
//import java.util.List;


import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;


import dmadmin.model.Domain;
import dmadmin.model.Engine;
import dmadmin.model.Notify;
import dmadmin.model.ProviderObject;
import dmadmin.util.CommandLine;

/**
 * Servlet implementation class RunProviderTest
 */
public class RunProviderTest
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RunProviderTest() {
        super();
    }
    
    @Override
   	public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("text/text");

		PrintWriter out = response.getWriter();

		int id = ServletUtils.getIntParameter(request, "id");
		int ot = ServletUtils.getIntParameter(request, "objtype");
		String rcpt = request.getParameter("rcpt");
		ObjectType objtype = ObjectType.fromInt(ot);

		ProviderObject po = session.getProviderObject(objtype, id, false);
		if (po.getObjectType() == ObjectType.NOTIFY && rcpt != null) {
			((Notify)po).setTestRecipient(rcpt);
		}
	
		Domain domain = po.getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		
		if(engine == null) {
			System.err.println("Engine was null");
			out.println("Unable to find suitable engine to conduct test");
			return;
		}
		
		CommandLine cmd = engine.doProviderTest(po);
		cmd.run(true, null, true);
		out.println(cmd.getOutput());
   	}
}
