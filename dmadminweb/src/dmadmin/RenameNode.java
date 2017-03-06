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
 * Servlet implementation class RenameNode
 */
public class RenameNode extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RenameNode() {
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
		String id = request.getParameter("id");
		String newname = request.getParameter("newname");
		System.out.println("newname=["+newname+"] id=["+id+"] rel=["+rel+"]");
		// Get the printwriter object from response to write the required json object to the output stream      
		PrintWriter out = response.getWriter();
		
		String errtext = "";
		String aoran;
		if ("AEIOUaeiou".indexOf(rel.charAt(0))>0)
		{
			aoran="An";
		}
		else
		{
			aoran="A";
		}
		boolean dup = false;
		String origname = newname;
		int j = 1;
		boolean renmsg = false;
		int res = 0;
		
		do
		{
		 res = session.RenameObject(rel, Integer.parseInt(id), newname);
		 if (res == 1)
		 {
		  newname = origname + "_" + j;
		  renmsg = true;
		  dup = true;
		 }
		 else
		  dup = false;
		 j++;
		} while (dup || j > 100);
		
		if (renmsg)
		{
			errtext=aoran+" "+rel+" with " + origname + " already exists and has been set to " + newname;
			res = 1;
		} 

		 out.print("{\"errtext\" : \"" + errtext + "\", \"errcode\" : \"" + res + "\", \"newname\": \"" + newname + "\"}");
	}
}
