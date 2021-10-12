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
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Base64;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
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
 DMSession so = null;
 HttpSession session = null;
 
    public GetAttachment() {
        super();
    }

    @Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
     
     try (DMSession so = DMSession.getInstance(request)) {
     session = request.getSession();
     session.setAttribute("session", so);
     so.checkConnection(request.getServletContext());


		response.setHeader("Content-Disposition", "inline");
		response.setHeader("Cache-Control", "no-cache");
		
		if (request.getParameter("attachid") != null) {
			// Retrieving attachment
			int attachid = ServletUtils.getIntParameter(request, "attachid");
			Attachment attach = so.getAttachment(attachid);
			
			String contentType = attach.getMimeType();
			if(contentType == null) {
				contentType = "application/octet-stream";
			}
			response.setHeader("Content-Transfer-Disposition", "binary");
			response.setContentType(contentType);
			
	//		response.setContentLength((int) attach.getSize());
			response.setHeader("Content-Disposition", "attachment; filename=" + attach.getName());
			
			OutputStream out = response.getOutputStream();
   String encoded = attach.getAttachmentString();
   
			byte[] decodedBytes = Base64.getDecoder().decode(encoded);
   ServletOutputStream sos = response.getOutputStream();
   sos.write(decodedBytes);
   sos.flush();
   sos.close();
		} else {
			// Retrieving function/procedure body from database
			PrintWriter out = response.getWriter();
			int actionid = ServletUtils.getIntParameter(request, "actionid");
			Action action = so.getAction(actionid, false);
			out.print(action.getText());	
		}
     }
	}

    @Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{

     try (DMSession so = DMSession.getInstance(request)) {
     session = request.getSession();
     session.setAttribute("session", so);
     so.checkConnection(request.getServletContext());

		
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
}
