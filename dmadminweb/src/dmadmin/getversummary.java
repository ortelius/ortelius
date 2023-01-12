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

import dmadmin.model.Application;
import dmadmin.model.Domain;


/**
 * Servlet implementation class getfragatt
 */
public class getversummary extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public getversummary() {
        super();
        // TODO Auto-generated constructor stub
    }


    // private void ProcessVersionSummary(HttpServletRequest request,HttpServletResponse response) throws ServletException, IOException {
    @Override
    public void handleRequest(DMSession session, boolean isPost,
    	   		HttpServletRequest request, HttpServletResponse response)
    	   throws ServletException, IOException
    {
    	response.setContentType("text/html;charset=UTF-8");
		PrintWriter out = response.getWriter();
		int appid = ServletUtils.getIntParameter(request,"appid");
    	//
    	//Get existing values (if any)
    	//
    	Application app = session.getApplication(appid,true);
    	Domain domain = app.getDomain();

    	String summary = app.getSummary();
    	if(summary == null) {
    		summary = "";
    	}

    	out.println("<html><body>");
    	out.println("<form id='fragattrs'>");
    	out.println("<table border=0><tr><td><img src='images/appver-large.png'></td><td valign='top'>");
    	out.println("<h3>"+app.getName()+"</h3>");
    	out.println(summary+"<br>");
    	out.println("Domain: "+domain.getName()+"<br>");
    	out.println("</table>");
    	out.println("<hr>");
    	out.println("<table border=0 class=\"atttable\" >");
    	out.print("<tr><td>Name</td><td><input id=\"title\" type=\"text\" value=\""+app.getName()+"\"</td></tr>");
    	out.print("<tr><td>Summary</td><td><input id=\"summary\" type=\"text\" value=\""+summary+"\"</td></tr>");
    	out.print("<tr><td>Branch Label</td><td><input id=\"summary\" type=\"text\" value=\""+app.getLabel(true)+"\" readonly disabled></td></tr>");
    	out.println("</table></form>");
    	out.println("</body></html>");
    }
}
