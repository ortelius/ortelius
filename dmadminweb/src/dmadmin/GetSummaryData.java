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

import dmadmin.json.JSONObject;
import dmadmin.model.Action;
import dmadmin.model.ActionKind;
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
    comp.setKind(ComponentItemKind.DOCKER);
   else if (comptype.contains("database"))
    comp.setKind(ComponentItemKind.DATABASE);
   else
    comp.setKind(ComponentItemKind.FILE);
  }
  else if (id < 0 && dmobj instanceof Action)
  {
   Action act = (Action)dmobj;
   
   if (actkind == null)
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
