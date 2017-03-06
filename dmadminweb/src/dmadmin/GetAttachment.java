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
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.model.Action;
import dmadmin.model.Attachment;

/**
 * Servlet implementation class GetAttachment
 */
public class GetAttachment
	extends HttpServlet
{
	private static final long serialVersionUID = 1L;
	       
    public GetAttachment() {
        super();
    }

    @Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		HttpSession session = request.getSession();
		DMSession so = (DMSession)session.getAttribute("session");

		response.setHeader("Content-Disposition", "inline");
		response.setHeader("Cache-Control", "no-cache");
		
		if (request.getParameter("attachid") != null) {
			// Retrieving attachment
			int attachid = ServletUtils.getIntParameter(request, "attachid");
			Attachment attach = so.getAttachment(attachid);
	
			InputStream in = attach.getAttachmentStream();
			
			String contentType = attach.getMimeType();
			if(contentType == null) {
				contentType = "application/octet-stream";
			}
			response.setHeader("Content-Transfer-Disposition", "binary");
			response.setContentType(contentType);
			
			response.setContentLength((int) attach.getSize());
			response.setHeader("Content-Disposition", "attachment; filename=" + attach.getName());
			
			OutputStream out = response.getOutputStream();
			byte[] buf = new byte[1024];
			int read = in.read(buf);
			while(read > 0) {
				out.write(buf, 0, read);
				read = in.read(buf);
			}
			in.close();
			out.close();
		} else {
			// Retrieving function/procedure body from database
			PrintWriter out = response.getWriter();
			int actionid = ServletUtils.getIntParameter(request, "actionid");
			Action action = so.getAction(actionid, false);
			out.print(action.getText());	
		}
	}

    @Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
    	HttpSession session = request.getSession();
		DMSession so = (DMSession)session.getAttribute("session");
		
    	int actionid = ServletUtils.getIntParameter(request, "actionid");
    	System.out.println("Saving actionid "+actionid);
    	String procbody = request.getParameter("procbody");
    	String parseResult = so.SaveProcBody(actionid,procbody);
    	parseResult = parseResult.replace("\"","'").replace("\n","").replace("\r","");
    	System.out.println("ParseResult="+parseResult);
    	int lineno=0;
    	int sp=0;
    	int ep=0;
    	if (parseResult.length() > 0) {
    		ep = parseResult.indexOf("at line ");
    		if (ep>0) {
    			String s1 = parseResult.substring(ep+8);
    			System.out.println("s1="+s1);
    			int fs = s1.indexOf(":");
    			lineno = Integer.parseInt(s1.substring(0,fs));
    			System.out.println("lineno="+lineno);
    			// Calculate character position from line number
    			sp=0;
    			for (int i=1;i<lineno;i++) {
    				sp = procbody.indexOf("\n", sp)+1;
    			}
    			ep = procbody.indexOf("\n",sp+1);
    		}
    	}
    	parseResult = parseResult.replace('\t',' ');		// tabs with spaces
    	parseResult = parseResult.replaceAll("\"", "\\\"");	// " with \"
    	parseResult = parseResult.replaceAll("\\\\", "\\\\\\\\");	// \ with \\
    	parseResult = parseResult.replaceAll("<","&lt;");
    	parseResult = parseResult.replaceAll(">","&gt;");
    	System.out.println("parseResult=["+parseResult+"]");
    	PrintWriter out = response.getWriter();
    	out.println("{\"success\": "+(parseResult.length()==0)+", \"sp\": "+sp+", \"ep\": "+ep+" , \"parseResult\": \""+parseResult+"\"}");
    	System.out.println("{\"success\": "+(parseResult.length()==0)+", \"sp\": "+sp+", \"ep\": "+ep+" , \"parseResult\": \""+parseResult+"\"}");
	}
}
