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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.JSONObject;
import dmadmin.model.Action;
import dmadmin.model.ActionKind;
import dmadmin.model.CompType;
import dmadmin.model.Component;
import dmadmin.model.ComponentFilter;
import dmadmin.model.ComponentItem;
import dmadmin.model.ComponentItemKind;
import dmadmin.model.DMObject;
import dmadmin.model.Server;
import dmadmin.model.Task;
import dmadmin.model.TaskAction;
import dmadmin.model.TaskApprove;
import dmadmin.model.TaskCreateVersion;
import dmadmin.model.TaskMove;
import dmadmin.model.TaskRemove;
import dmadmin.model.TaskRequest;

/**
 * Servlet implementation class GetSummaryData
 */
public class GetSummaryData extends HttpServletBase
{
 private static final long serialVersionUID = 1L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public GetSummaryData()
 {
  super();
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setContentType("application/json");

  PrintWriter out = response.getWriter();

  int ot = ServletUtils.getIntParameter(request, "objtype");
  String sid = request.getParameter("id");
  String comptype = request.getParameter("comptype");
  String actkind = request.getParameter("actkind");
  
  if (!sid.equalsIgnoreCase("-1")) sid = sid.replaceAll("-.*$","");
  int id = Integer.parseInt(sid);
  ObjectType objtype = ObjectType.fromInt(ot);
  if (objtype == null)
  {
   throw new RuntimeException("Invalid object type " + ot);
  }

  System.out.println("getDetailedObject for id "+id);
  DMObject dmobj;
  
  if (objtype == ObjectType.COMPONENTITEM && id < 0)
  { 
   ComponentItem ci = new ComponentItem();
   if (comptype.equalsIgnoreCase("docker"))
    ci.setItemkind(ComponentItemKind.DOCKER);
   else if (comptype.contains("database"))
   {
    ci.setItemkind(ComponentItemKind.DATABASE);
    if (comptype.startsWith("rf_"))
    {
     ci.setRollup(ComponentFilter.ON);
     ci.setRollback(ComponentFilter.OFF);
    }
    else
    {
     ci.setRollup(ComponentFilter.OFF);
     ci.setRollback(ComponentFilter.ON);
    }     
   }
   else
    ci.setItemkind(ComponentItemKind.FILE);
   dmobj = (DMObject)ci;
   dmobj.setSession(session);
  }
  else
   dmobj = session.getDetailedObject(objtype, id);
  
  boolean readOnly = 
		  (dmobj.getObjectType() == ObjectType.DEPLOYMENT) ||
		  (dmobj.getObjectType() == ObjectType.USER && id == session.GetUserID() && !session.getAclOverride()) ||	// Cannot edit yourself
		  !dmobj.isUpdatable();
  
  if (id < 0 && dmobj instanceof Component)
  {
   Component comp = (Component)dmobj;
   
   if (comptype.equalsIgnoreCase("docker"))
   {
    CompType ct = session.getCompTypeByName("Kubernetes");
    if (ct != null)
    {
     comp.setComptypeId(ct.getId());
     comp.setComptype(ct.getName());
    } 
    comp.setKind(ComponentItemKind.DOCKER);
   }
   else if (comptype.contains("database"))
   {
    CompType ct = session.getCompTypeByName("Postgres");
    if (ct != null)
    {
     comp.setComptypeId(ct.getId());
     comp.setComptype(ct.getName());
    } 
    comp.setKind(ComponentItemKind.DATABASE);
   } 
   else
   {
    CompType ct = session.getCompTypeByName("Application Server");
    if (ct != null)
    {
     comp.setComptypeId(ct.getId());
     comp.setComptype(ct.getName());
    } 
    comp.setKind(ComponentItemKind.FILE);
   } 
  }
  else if (id < 0 && dmobj instanceof Action)
  {
   Action act = (Action)dmobj;
   
   if (actkind == null)
    actkind = "" + act.getKind().value();
   
   if (actkind == null || actkind.equals("0"))
    act.setKind(ActionKind.LOCAL_EXTERNAL);
   else 
   {
    int akid = new Integer(actkind).intValue();
    ActionKind ak = ActionKind.fromInt(akid);
    act.setKind(ak);
   }
  }
  
  if (dmobj instanceof Action) {
	  if (((Action)dmobj).getParentId() > 0) {
		  // Archived actions are not editable
		  readOnly = true;
	  }
  }

  JSONObject obj = new JSONObject();
  obj.add("readOnly", readOnly);
  if (dmobj.getDomain() != null) {
	  obj.add("domain",dmobj.getDomain().getLinkJSON());
  }

  if (dmobj instanceof Task)
  {
   Task t = (Task) dmobj;

   if (t.getTaskType() == Task.TaskType.APPROVE)
   {
    TaskApprove task = session.getTaskApprove(id);
    if (task != null)
     obj.add("data", task.getSummaryJSON());
   }
   else if (t.getTaskType() == Task.TaskType.MOVE)
   {
    TaskMove task = session.getTaskMove(id);
    if (task != null)
     obj.add("data", task.getSummaryJSON());
   }
   else if (t.getTaskType() == Task.TaskType.REMOVE)
   {
    TaskRemove task = session.getTaskRemove(id);
    if (task != null)
     obj.add("data", task.getSummaryJSON());
   }
   else if (t.getTaskType() == Task.TaskType.CREATE_VERSION)
   {
    TaskCreateVersion task = session.getTaskCreateVersion(id);
    if (task != null)
     obj.add("data", task.getSummaryJSON());
   }
   else if (t.getTaskType() == Task.TaskType.REQUEST)
   {
    TaskRequest task = session.getTaskRequest(id);
    if (task != null)
     obj.add("data", task.getSummaryJSON());
   }
   else if (t.getTaskType() == Task.TaskType.RUN_ACTION)
   {
	   TaskAction task = session.getTaskAction(id);
	   if (task != null)
		obj.add("data", task.getSummaryJSON());
   }
   else // Just Task Object
    obj.add("data", dmobj.getSummaryJSON());
  }
  else
  if (dmobj instanceof Server)
  {
	  int status = ServletUtils.getIntParameter(request, "s",0);
	  if (status == 1) {
		  // We're displaying server status page
		  Server s = session.getServer(id,false);
		  obj.add("data", s.getStatusData());
	  } else {
		  // Server summary data
		  obj.add("data", dmobj.getSummaryJSON());
	  }
   }
   else // Just Object
   obj.add("data", dmobj.getSummaryJSON());
  
  String ret = obj.toString();

  out.println(ret);
  System.out.println(ret);
 }
}
