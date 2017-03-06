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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class RemoveNode
 */
public class RemoveNode extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RemoveNode() {
        super();
        // TODO Auto-generated constructor stub
    }

	
	
	
	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		response.setContentType("application/json");
		String rel   = request.getParameter("rel");
		String rel_param = rel;
		String idStr = request.getParameter("id");
		int id = 0;
		if (idStr.contains("-")) {
			String[] parts = idStr.split("-");
			id = new Integer(parts[0]).intValue();
		} else {
			id = new Integer(idStr).intValue();
		}
		PrintWriter out = response.getWriter();
		// String errtext = "";
		if (rel.equalsIgnoreCase("appversion") || rel.equalsIgnoreCase("release")) rel_param = "Application";
		
		session.RemoveObject(rel_param, id, out);
	}
}
