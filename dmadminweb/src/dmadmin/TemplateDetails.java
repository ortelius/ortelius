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

import dmadmin.model.NotifyTemplate;


/**
 * Servlet implementation class ServerDetails
 */
public class TemplateDetails
	extends HttpServletBase
{
	private static final long serialVersionUID = 6793674383538307787L;

	/**
     * @see HttpServlet#HttpServlet()
     */
    public TemplateDetails() {
        super();
    }


	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		if (!isPost) {
			// Pull template details and return
			int templateid = getIntParameter(request, "id");
			System.out.println("Getting template data for id="+templateid);
			NotifyTemplate template = session.getTemplate(templateid);
			PrintWriter out = response.getWriter();
			String subject = template.getSubject();
			String body = template.getBody();
			if (body == null) body="";
			if (subject == null) subject="";
			System.out.print("{\"subject\" : \"" + subject + "\", \"body\" : \"" + body.replace("\n","\\n").replace("\"","\\\"") + "\"}");
			out.print("{\"subject\" : \"" + subject + "\", \"body\" : \"" + body.replace("\n","\\n").replace("\"","\\\"") + "\"}");	
		} else {
			// Update
			int templateid = getIntParameter(request, "id");
			String f = request.getParameter("f");
			if (f.equalsIgnoreCase("seb")) {
				// save email body
				System.out.println("Saving template data for id="+templateid);
				String subject = request.getParameter("sub");
				String body = request.getParameter("b");
				System.out.println("subject=["+subject+"]");
				System.out.println("body=["+body+"]");
				session.saveTemplateBody(templateid,subject,body);
			} else {
				int rid = getIntParameter(request, "rid");
				if (f.equalsIgnoreCase("au")) {
					// Add user
					System.out.println("Adding user "+rid);
					session.addUserToTemplate(templateid,rid);
				}
				else
				if (f.equalsIgnoreCase("ag")) {
					// Add group
					System.out.println("Adding group "+rid);
					session.addGroupToTemplate(templateid,rid);
				}
				else
				if (f.equalsIgnoreCase("as")) {
					// Add special
					System.out.println("Adding special "+rid);
					session.addSpecialToTemplate(templateid,rid);
				}
				else
				if (f.equalsIgnoreCase("ru")) {
					// Add user
					System.out.println("Removing user "+rid);
					session.removeUserFromTemplate(templateid,rid);
				}
				else
				if (f.equalsIgnoreCase("rg")) {
					// Add group
					System.out.println("Removing group "+rid);
					session.removeGroupFromTemplate(templateid,rid);
				}
				else
				if (f.equalsIgnoreCase("rs")) {
					// Add special
					System.out.println("Removing special "+rid);
					session.removeSpecialFromTemplate(templateid,rid);
				}
			}
			PrintWriter out = response.getWriter();
			out.print("{\"errtext\" : \"" + "" + "\", \"errcode\" : \"" + "0" + "\"}");
		}
		
	}

}
