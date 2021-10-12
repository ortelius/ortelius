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
