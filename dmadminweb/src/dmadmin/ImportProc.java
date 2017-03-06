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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItem;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;


/**
 * Servlet implementation class History New Feed
 */
public class ImportProc
	extends JSONMultipartServletBase
{
	private static final long serialVersionUID = 8314171259294636898L;

	
    public ImportProc() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response,
			HttpParameters params, List<FileItem> files)
		throws ServletException, IOException
	{				
		// Either result or data go in here
		JSONObject obj = new JSONObject();
		
		String textDomain = request.getParameter("domain");
		System.out.println("textDomain (2)="+textDomain);
		if (textDomain == null) {
			obj.add("result", false);
			obj.add("error","Domain not passed correctly");
			return obj;
		}
		int domainid = Integer.parseInt(textDomain);
		
		System.out.println("domainid="+domainid);

		if (files != null) {
			for (FileItem file : files) {
				long fileSize = file.getSize();
				System.out.println("fileSize="+fileSize);
				InputStream is = file.getInputStream();
				File tempFile = File.createTempFile("import", "tmp");
				FileOutputStream fos = new FileOutputStream(tempFile);
				byte buf[] = new byte[2048];
				int s, tl = 0;
				while ((s = is.read(buf, 0, 2048)) > 0) {
				    fos.write(buf, 0, s);
				    tl += s;
				}
				fos.close();
				if (tl != fileSize) {
					obj.add("result", false);
					obj.add("error","File failed to upload");
					return obj;
				}
				obj = session.ImportFunction(domainid, tempFile.getCanonicalPath());
				tempFile.delete();
			}
		}
		return obj;
	}
}
