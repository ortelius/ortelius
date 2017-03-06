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
import java.io.PrintWriter;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Server;

/**
 * Servlet implementation class Highlighter
 */
public class Highlighter extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Highlighter() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
    	   		HttpServletRequest request, HttpServletResponse response)
    	   throws ServletException, IOException
    {
    	response.setContentType("application/json;charset=UTF-8");
		String mode = request.getParameter("f");
		
		PrintWriter out = response.getWriter();
		
		if (mode.equalsIgnoreCase("swc"))
		{
			// Servers with components....
			int e = ServletUtils.getIntParameter(request, "e");	// environment id
			int c = ServletUtils.getIntParameter(request, "c");	// component id
			List <Server> sl = session.GetServersWithComponents(e,c);
			out.print("{");
			out.println("\"server\": [");
			
			boolean sep=false;
			for (Server s: sl)
			{
				if (sep) out.println(",");
				
				out.println("{");
				out.println("\"id\" : \""       + s.getId() + "\",");
				out.println("\"name\" : \""     + s.getName() + "\",");
				out.println("\"summary\" : \""  + s.getSummary() + "\"");
				out.println("}");
				sep=true;
			}
			out.println("],");
			out.println("\"ServerCount\":\"" + sl.size() + "\"");
			out.println("}");
		}
    }
}
