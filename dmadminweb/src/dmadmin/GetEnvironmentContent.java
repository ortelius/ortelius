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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Server;

/**
 * Servlet implementation class GetEnvironmentContent
 */
public class GetEnvironmentContent extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetEnvironmentContent() {
        super();
        // TODO Auto-generated constructor stub
    }
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json");
		String envid = request.getParameter("envid");
		// Get the printwriter object from response to write the required json object to the output stream      
		PrintWriter out = response.getWriter();
		
		List<Server> d = session.getServersInEnvironment(Integer.parseInt(envid));
		
		out.print("[");

		boolean subenv=false;
		for (Server xd: d)
		{
			if (subenv) out.println(",");
			out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.getOtid() + "\", \"rel\" : \"Server\" }}");
			subenv=true;
		}
		out.println("]");
   	}
}
