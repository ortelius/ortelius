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
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Server;


/**
 * Servlet implementation class getfragatt
 */
public class getserversum extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public getserversum() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("text/html;charset=UTF-8");
    	
		PrintWriter out = response.getWriter();	
		System.out.println("in ProcessServerSummary");
    	int servid = ServletUtils.getIntParameter(request,"serverid");
    	//
    	//Get existing values (if any)
    	//
    	Server serv = session.getServer(servid,true);
    	
    	String summary = serv.getSummary();
    	if(summary == null) {
    		summary = "";
    	}
    	
    	out.println("<html><body>");
    	out.println("<form id='cvs'>");
    	out.println("<h3>"+serv.getName()+"</h3>");
    	out.println(summary+"<br>");
    	out.println("<hr>");
    		
    	out.println("<table border=0 class=\"atttable\" >");
    	out.print("<tr><td>Name</td><td><input id=\"title\" type=\"text\" value=\""+serv.getName()+"\"</td></tr>");
    	out.print("<tr><td>Summary</td><td><input id=\"summary\" type=\"text\" style=\"width:200px\" value=\""+summary+"\"</td></tr>");  	
    	out.println("</table></form>");
    	out.println("</body></html>");
   	}
}

