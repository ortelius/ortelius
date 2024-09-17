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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;

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
  else if (type.equalsIgnoreCase("Apps4Package"))
  {
   String pkgname = request.getParameter("pkgname");
   String pkgver = request.getParameter("pkgversion");
   String userid = request.getParameter("userid");

   if (userid != null && !userid.isEmpty())
   {
    int id = Integer.valueOf(userid);
    session.GetDomains(id);
   }

   if (pkgname != null)
   {
    JSONArray arr = session.getApps4PackageList(pkgname, pkgver);
    out.println(arr.getJSON());
    out.println("}");
    return;
   }
  }
  else if (type.equalsIgnoreCase("appcompList"))
  {
   String appid = request.getParameter("appid");

   if (appid != null)
   {
    JSONArray arr = new JSONArray();
    List<Component> comps = session.getComponents(ObjectType.APPLICATION, Integer.parseInt(appid), false);

    for (int i=0;i< comps.size(); i++)
    {
     Component c = comps.get(i);
     JSONObject obj = new JSONObject();
     obj.add("id", c.getOtid().toString());
     obj.add("name", c.getName());
     obj.add("domain", c.getDomain().getFullDomain());
     arr.add(obj);
    }

    out.println(arr.getJSON());
    out.println("}");
    return;
   }
  }
  out.println("}");
 }

}
