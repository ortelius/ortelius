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
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.MessageDigest;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.bind.DatatypeConverter;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

public class Reports extends HttpServletBase
{
 /**
  *
  */
 private static final long serialVersionUID = -9191721031006941798L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public Reports()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setContentType("text/html; charset=UTF-8");
  String type = request.getParameter("type");

  if (type.equalsIgnoreCase("FileAudit"))
  {
   // process only if its multipart content
   if (ServletFileUpload.isMultipartContent(request))
   {
    try
    {
     List<FileItem> multiparts = new ServletFileUpload(new DiskFileItemFactory()).parseRequest(request);

     String loc = "";
     for (FileItem item : multiparts)
     {
      if (!item.isFormField())
      {
       String name = new File(item.getName()).getName();
       File f = File.createTempFile("omaudit_" + name, "");

       item.write(f);

       String path = f.getAbsolutePath();
       MessageDigest md = MessageDigest.getInstance("MD5");
       md.update(Files.readAllBytes(Paths.get(path)));
       byte[] digest = md.digest();
       String digestInHex = DatatypeConverter.printHexBinary(digest).toUpperCase();
       loc += digestInHex + " ";
       f.delete();
      }
     }

     loc = loc.toLowerCase().trim();
     System.out.println("/WEB-INF/FileAudit.jsp?md5=" + loc);
     request.setAttribute("md5", loc);
     request.getRequestDispatcher("/WEB-INF/FileAudit.jsp?md5=" + loc).include(request, response);
    }
    catch (Exception e)
    {
    }
   }
  }
 }
}
