/*
 *
 *  Ortelius for Microservice Configuration Mapping
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
