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

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.JSONArray;

public class ReportsData extends HttpServletBase
{
 /**
  * 
  */
 private static final long serialVersionUID = -9191721031006941798L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public ReportsData()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setContentType("text/html; charset=UTF-8");
  PrintWriter out = response.getWriter();
  String type = request.getParameter("type");
 
  out.println("{");
  out.println("\"data\":");
  
  if (type.equalsIgnoreCase("FileAudit"))
  {
   String md5 = request.getParameter("md5");
   
   if (md5 != null)
   {
    JSONArray arr = session.getFileAuditReport(md5);
    out.println(arr.getJSON());
    out.println("}");
    return;
   }
  }
  else if (type.equalsIgnoreCase("Success4Env"))
  {
    JSONArray arr = session.getSuccess4EnvReport();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }
  else if (type.equalsIgnoreCase("ServerAudit"))
  {
    JSONArray arr = session.getServerAuditReport();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }  
  else if (type.equalsIgnoreCase("ServerInventory"))
  {
    JSONArray arr = session.getServerInventoryReport();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }    
  else if (type.equalsIgnoreCase("AppList"))
  {
    JSONArray arr = session.getAppList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }  
  else if (type.equalsIgnoreCase("RelList"))
  {
    JSONArray arr = session.getReleaseList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }  
  else if (type.equalsIgnoreCase("CompList"))
  {
    JSONArray arr = session.getCompList();
    out.println(arr.getJSON());
    out.println("}");
    System.out.println("{");
    System.out.println(arr.getJSON());
    System.out.println("}");
    return;
  }  
  else if (type.equalsIgnoreCase("EnvList"))
  {
    JSONArray arr = session.getEnvList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("EndPointList"))
  {
    JSONArray arr = session.getEndPointList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }  
  else if (type.equalsIgnoreCase("ActionList"))
  {
    JSONArray arr = session.getActionList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }
  else if (type.equalsIgnoreCase("ProcedureList"))
  {
    JSONArray arr = session.getProcedureList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("NotifierList"))
  {
    JSONArray arr = session.getNotifierList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("RepositoryList"))
  {
    JSONArray arr = session.getRepositoryList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }  
  else if (type.equalsIgnoreCase("DatasourceList"))
  {
    JSONArray arr = session.getDatasourceList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("CredentialList"))
  {
    JSONArray arr = session.getCredentialList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("UserList"))
  {
    JSONArray arr = session.getUserList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("GroupList"))
  {
    JSONArray arr = session.getGroupList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("ServerCompTypeList"))
  {
    JSONArray arr = session.getServerCompTypeList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }
  else if (type.equalsIgnoreCase("TemplateList"))
  {
    JSONArray arr = session.getTemplateList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  else if (type.equalsIgnoreCase("BuildJobList"))
  {
    JSONArray arr = session.getBuildJobList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  }
  else if (type.equalsIgnoreCase("DomainList"))
  {
    JSONArray arr = session.getDomainTableList();
    out.println(arr.getJSON());
    out.println("}");
    return;
  } 
  out.println("}");
 }
 
}
