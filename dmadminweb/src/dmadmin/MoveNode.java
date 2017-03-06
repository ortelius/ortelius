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
// import javax.servlet.http.HttpSession;

/**
 * Servlet implementation class MoveNode
 */
public class MoveNode extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public MoveNode() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
    	   		HttpServletRequest request, HttpServletResponse response)
    	   throws ServletException, IOException
    {
    	response.setContentType("application/json");
		String nodetype = request.getParameter("nodetype");
		int nodeid   = ServletUtils.getIntParameter(request,"nodeid",0);
		String pid = request.getParameter("parentid");
		System.out.println("pid="+pid);
		int parentid;
		if (pid.indexOf('-')>0) {
			parentid = Integer.parseInt((pid.substring(0,pid.indexOf('-'))));
		} else {
			parentid = Integer.parseInt(pid);
		}
		/*
		else {
			parentid=0;
		}
		*/
		System.out.println("parentid="+parentid);
		String targettype = request.getParameter("targettype");
        int targetid = ServletUtils.getIntParameter(request,"targetid",0);
        
		PrintWriter out = response.getWriter();
		String errtext = session.MoveObject(nodetype, parentid, nodeid, targettype, targetid);
		out.print("{\"errtext\" : \"" + errtext + "\"}");
    }
}
