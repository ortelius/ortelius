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

import dmadmin.model.Component;


/**
 * Servlet implementation class getfragatt
 */
public class getcompversum extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public getcompversum() {
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
    	int compid = ServletUtils.getIntParameter(request,"compid");
    	//
    	//Get existing values (if any)
    	//
    	Component comp = session.getComponent(compid,true);
    	out.println("<script language='javascript'>");
    	out.println("function redir() {");
    	out.println("var mainframe = parent.document.getElementById(\"mainframe3\");");
		out.println("mainframe.src = \"compdetails?a=N&compid="+comp.getId()+"\";");
		out.println("parent.$(\"#attributes\").dialog(\"close\");");
    	out.println("}");
    	out.println("</script>");
    	out.println("<html><body>");
    	out.println("<form id='cvs'>");
    	out.println("<table border=\"0\">");
    	out.println(" <tr>");
    	out.println(" <td><img src=\"images/component-large" + comp.getIconSuffix() + ".png\"></td>");
    	out.println("  <td><span style=\"font-family:Helvetica,Arial; font-size:20px\">" + comp.getName() + "</span><br>");
    	out.println("<span style=\"font-family:Helvetica,Arial; font-size:14px\">" + comp.getSummary() + "</span></td>");
    	out.println(" </tr>");
    	out.println("</table>");
    	out.println("<hr>");
    		
    	out.println("<table border=0 class=\"atttable\" >");
    	out.print("<tr><td>Name</td><td><input id=\"title\" type=\"text\" value=\""+comp.getName()+"\"</td></tr>");
    	out.print("<tr><td>Summary</td><td><input id=\"summary\" type=\"text\" style=\"width:200px\" value=\""+comp.getSummary()+"\"</td></tr>");  	
    	out.println("</table><br><br><br>");
    	out.println("<a href=\"#\" onClick='redir();'>More Details...</a>");
    	// out.println("<a href='compdetails?a=N&compid="+comp.getId()+"' target='mainframe3'>More Details...</a>");
    	out.println("</form>");
    	out.println("</body></html>");
   	}
}
