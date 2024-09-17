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
 */package dmadmin.pro;

import java.io.IOException;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.DMSession;
import dmadmin.JSONServletBase;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;

/**
 * Servlet implementation class GetComponentLayout
 */

public class GetDefects extends JSONServletBase
{
 private static final long serialVersionUID = 1L;

 @Override
 public IJSONSerializable handleRequest(DMSession so, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  try (DMSessionPro session = new DMSessionPro(so))
  {

   String szApplicationID = request.getParameter("appid");
   String szComponentID = request.getParameter("compid");
   String szEnvironmentID = request.getParameter("envid");
   String szAction = request.getParameter("a");

   boolean bReadOnly = false;
   DefectList defectlist = null;

   System.out.println("in GetDefects");

   if (szComponentID != null)
   {
    // Defects associated with Component
    System.out.println("Getting defects for component");
    int compid = Integer.parseInt(szComponentID);
    Component comp = session.getComponent(compid, false);
    bReadOnly = !comp.isUpdatable();
    if (comp.getDatasource() == null)
     bReadOnly = true; // No datasource, disable buttons
    if (szAction != null)
    {
     if (szAction.equalsIgnoreCase("od"))
     {
      // Get Open Defects for the defect tracker associated with this component
      defectlist = session.getOpenDefects(comp.getDatasource()); // will pass comp.getDataSource();
     }
     else if (szAction.equalsIgnoreCase("ad"))
     {
      // Add a defect to the component
      String szBugId = request.getParameter("id");
      String szTitle = request.getParameter("title");
      String szURL = request.getParameter("url");
      String szAPIURL = request.getParameter("apiurl");
      String status = request.getParameter("status");
      System.out.println("szBugId=" + szBugId);
      System.out.println("szTitle=" + szTitle);
      System.out.println("szURL=" + szURL);
      System.out.println("szAPIURL=" + szAPIURL);
      System.out.println("status=" + status);
      defectlist = session.addDefectToComponent(comp, szBugId, szTitle, szURL, szAPIURL, status);
     }
     else if (szAction.equalsIgnoreCase("dd"))
     {
      String szBugId = request.getParameter("id");
      defectlist = session.removeDefectFromComponent(compid, szBugId);
     }
     else if (szAction.equalsIgnoreCase("dh"))
     {
      // Get defect history for component
      List<int[]> dh = session.getDefectHistory(comp);
      // result is a list of 3-element arrays containing timestamp (0), open count (1) and closed count (2)
      JSONArray ret = new JSONArray();
      for (int[] x : dh)
      {
       JSONObject t = new JSONObject();
       t.add("ts", x[0]);
       t.add("open", x[1]);
       t.add("closed", x[2]);
       ret.add(t);
      }
      System.out.println("getDefectHistory ret = " + ret);
      return ret;
     }
    }
    else
    {
     defectlist = session.getDefectsForComponent(compid);
    }
   }
   else if (szApplicationID != null)
   {
    // Defects associated with Application/Release

    int appid = Integer.parseInt(szApplicationID);
    Application app = session.getApplication(appid, false);
    if (app.getIsRelease().equalsIgnoreCase("y"))
    {
     System.out.println("Getting defects for release");
     bReadOnly = true;
    }
    else
    {
     System.out.println("Getting defects for application");
     bReadOnly = !app.isUpdatable();
     if (app.getDatasource() == null)
      bReadOnly = true; // no datasource, disable buttons
    }

    if (szAction != null)
    {
     if (szAction.equalsIgnoreCase("od"))
     {
      // Get Open Defects for the defect tracker associated with this application
      defectlist = session.getOpenDefects(app.getDatasource()); // will pass app.getDataSource();
     }
     else if (szAction.equalsIgnoreCase("ad"))
     {
      // Add a defect to the application
      System.out.println("Adding defect\n");
      String szBugId = request.getParameter("id");
      String szTitle = request.getParameter("title");
      String szURL = request.getParameter("url");
      String szAPIURL = request.getParameter("apiurl");
      String status = request.getParameter("status");
      System.out.println("szBugId=" + szBugId);
      System.out.println("szTitle=" + szTitle);
      System.out.println("szURL=" + szURL);
      System.out.println("status=" + status);
      defectlist = session.addDefectToApplication(app, szBugId, szTitle, szURL, szAPIURL, status);
     }
     else if (szAction.equalsIgnoreCase("dd"))
     {
      String szBugId = request.getParameter("id");
      defectlist = session.removeDefectFromApplication(appid, szBugId);
     }
     else if (szAction.equalsIgnoreCase("dh"))
     {
      // Get defect history for application
      List<int[]> dh = session.getDefectHistory(app);
      // result is a list of 3-element arrays containing timestamp (0), open count (1) and closed count (2)
      JSONArray ret = new JSONArray();
      for (int[] x : dh)
      {
       JSONObject t = new JSONObject();
       t.add("ts", x[0]);
       t.add("open", x[1]);
       t.add("closed", x[2]);
       ret.add(t);
      }
      return ret;
     }

    }
    else
    {
     defectlist = session.getDefectsForApplication(appid);
    }
   }
   else if (szEnvironmentID != null)
   {
    // Defects associated with Environment
    System.out.println("Getting defects for environment");
    int envid = Integer.parseInt(szEnvironmentID);
    defectlist = session.getDefectsForEnvironment(envid);
   }
   if (defectlist != null)
   {
    defectlist.setReadOnly(bReadOnly);
   }
   else
   {
    defectlist = new DefectList(session);
   }
   return defectlist.getJSON();
  }
 }

 // Code for defect list. Need to make second call from JS to get defects
 //
 // Now the defects for this component
 // JSONArray deflist = new JSONArray();
 // DefectList defectlist = so.getDefectsForApplication(app.getId());
 // for (Defect d : defectlist.getDefects())
 // {
 // JSONObject dobj = assembleJSONForDefect(so, d);
 // deflist.add(dobj);
 // }
 // je.add("change_requests", deflist);

 // Code for defects for deployment
 //
 // } else if (f.equalsIgnoreCase("d")) {
 // // defects
 // System.out.println("Getting defects for deployment #"+depid);
 // obj.add("data", dep.getDefectsJSON());

 // for dep nodes in map
 // DefectList defects = getDefectsForComponent(comp.getId());
 //
 // for (int k=0;k<defects.getDefects().size();k++)
 // {
 // Defect defect = defects.getDefects().get(k);
 //
 // // add defects to component
 // if (!added.contains("df" + defect.getId()))
 // {
 // added.add("df" + defect.getId());
 // nodes.add(new DeployDepsNode("df" + defect.getId(), "CR " + defect.getName()).getJSON());
 // }
 // if (!edges_added.contains("df" + defect.getId() + "-" + comp.getOtid().toString()))
 // {
 // edges_added.add("df" + defect.getId() + "-" + comp.getOtid().toString());
 // edges.add(new DeployDepsEdge("df" + defect.getId(), comp.getOtid().toString()).getJSON());
 // }
 // }
 // }
 //
 // DefectList defects = getDefectsJustForApplication(d.getApplication().getId());
 //
 // for (int k=0;k<defects.getDefects().size();k++)
 // {
 //
 //// add defects to application
 // Defect defect = defects.getDefects().get(k);
 // if (!added.contains("df" + defect.getId()))
 // {
 // added.add("df" + defect.getId());
 // nodes.add(new DeployDepsNode("df" + defect.getId(), "CR " + defect.getName()).getJSON());
 // }
 // if (!edges_added.contains("df" + defect.getId() + "-" + d.getApplication().getOtid().toString()))
 // {
 // edges_added.add("df" + defect.getId() + "-" + d.getApplication().getOtid().toString());
 // edges.add(new DeployDepsEdge("df" + defect.getId(), d.getApplication().getOtid().toString()).getJSON());
 // }
 // }

 // code for provider test
 // ProviderDefinition pdef = po.getDef();
 // if (pdef.getName().equals("jira"))
 // {
 // Datasource ds = session.getDatasource(id,true);
 // DefectList ret = session.getOpenDefectsJira(ds);
 //
 // if (ret.getErrorMessage() != null)
 // out.println(ret.getErrorMessage());
 // else
 // out.println("Successful connection to " + session.getDataSourceAttribute(ds.getId(),"Server URL"));
 // }

}
