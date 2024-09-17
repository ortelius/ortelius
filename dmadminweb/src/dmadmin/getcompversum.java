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
    	out.println("  <td><span >" + comp.getName() + "</span><br>");
    	out.println("<span >" + comp.getSummary() + "</span></td>");
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
