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
