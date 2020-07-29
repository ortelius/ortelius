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
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

import dmadmin.json.IJSONSerializable;

@SuppressWarnings("serial")
public abstract class JSONMultipartServletBase
	extends HttpServlet
{
 DMSession so = null;
 HttpSession session = null;
 
	public class HttpParameters
		extends Hashtable<String,String>
	{
		public HttpParameters() {
			super();
		}
		
		public int getIntParameter(String name) {
			String value = get(name);
			if(value != null) {
				try {
					return Integer.parseInt(value);
				} catch(NumberFormatException e) {}
			}
			throw new RuntimeException("Parameter '" + name + "' not specified or invalid value");
		}
		
		public int getIntParameter(String name, int defaultValue) {
			String value = get(name);
			if(value != null) {
				try {
					return Integer.parseInt(value);
				} catch(NumberFormatException e) {}
			}
			return defaultValue;
		}
	}

	
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		internalHandleRequest(false, request, response);
	}

	
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		internalHandleRequest(true, request, response);
	}

	
	private void internalHandleRequest(boolean isPost, HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException
	{		
		response.setContentType("application/json");

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		response.setHeader("Content-Disposition", "inline");
		response.setHeader("Cache-Control", "no-cache");

		System.out.println("In " + getClass().getName());
		
		// Apache Commons FileUpload
		boolean isMultipart = ServletFileUpload.isMultipartContent(request);

		HttpParameters params = new HttpParameters();
		List<FileItem> files = null;

		try {
			IJSONSerializable json = null;
		
			if(!isMultipart) {
				for(Object name : request.getParameterMap().keySet()) {
					if(name instanceof String) {
						String value = request.getParameter((String) name);
						params.put((String) name, value);
					}
				}		
			} else {			
				DiskFileItemFactory factory = new DiskFileItemFactory();
		
				//factory.setSizeThreshold(yourMaxMemorySize);
				//factory.setRepository(yourTempDirectory);
		
				ServletFileUpload upload = new ServletFileUpload(factory);
		
				//upload.setSizeMax(yourMaxRequestSize);
				
				try {
					List<FileItem> items = upload.parseRequest(request);
				
					for(FileItem item : items) {	
					    if (item.isFormField()) {
							String name = item.getFieldName();
						    String value = item.getString();
						    params.put(name, value);
					    } else {
					    	if(files == null) {
					    		files = new ArrayList<FileItem>();
					    	}
					    	files.add(item);
					    }
					}
				} catch (FileUploadException e) {
					e.printStackTrace();
				}
			}
			
			json = handleRequest(so, isPost, request, response, params, files);
			
			PrintWriter out = response.getWriter();

			// Serialize as JSON
			String ret = (json != null) ? json.getJSON() : "{}"; 
			
			out.println(ret);
			System.out.println(ret);
		} catch(Exception e) {
			e.printStackTrace();
		}
  }
	}

	public abstract IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response,
			HttpParameters params, List<FileItem> files)
		throws ServletException, IOException;
}
