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
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;













import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;
import dmadmin.model.DMAttribute;
import dmadmin.model.Deployment;
import dmadmin.model.Domain;
import dmadmin.model.Environment;
import dmadmin.model.Task;
import dmadmin.model.Task.TaskType;
import dmadmin.model.TaskApprove;
import dmadmin.model.TaskParameter;
import dmadmin.model.TaskRemove;
import dmadmin.model.TaskCreateVersion;
import dmadmin.model.TaskDeploy;
import dmadmin.model.TaskMove;
import dmadmin.model.TaskRequest;
import dmadmin.model.TaskUserDefined;

/**
 * Servlet implementation class RunTask
 */
public class RunTask
extends HttpServletBase
{
	private static final long serialVersionUID = 1L;

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public RunTask() {
		super();
	}

	private JSONObject doSubscribe(DMSession session, ObjectTypeAndId otid, boolean unsubscribe)
	{
		JSONObject obj = new JSONObject();
		try {
			if(unsubscribe) {
				session.newsUnsubscribeObject(otid);
			} else {
				session.newsSubscribeObject(otid);    
			}
			obj.add("result", true);
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("result", false);
			obj.add("error", e.getMessage());
		}
		return obj;
	}
 
	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
					throws ServletException, IOException
	{
		int id = ServletUtils.getIntParameter(request,"id");
		String isAudit = request.getParameter("tid");

		if (isAudit != null && isAudit.equalsIgnoreCase("Audit"))
		{
			String DMHome = getServletContext().getInitParameter("DMHOME");
			DMHome = DMHome.replaceAll("\\\\$", "");
			String ret = session.getTaskAudit(DMHome,id);
			PrintWriter out = response.getWriter();
			out.println(ret);
			System.out.println(ret);
			return;
		}
		else if (isAudit != null && isAudit.equalsIgnoreCase("AuditEnv"))
		{
			String DMHome = getServletContext().getInitParameter("DMHOME");
			DMHome = DMHome.replaceAll("\\\\$", "");
			String ret = session.getTaskAudit(DMHome,id, true);
			PrintWriter out = response.getWriter();
			out.println(ret);
			System.out.println(ret);
			return;
		}
		else if (isAudit != null && isAudit.equalsIgnoreCase("Subscribe"))
		{
			String sotid = request.getParameter("otid");
			ObjectTypeAndId otid = ((sotid != null) && (sotid.length() > 0)) ? new ObjectTypeAndId(sotid) : null;
  
			String ret = doSubscribe(session, otid, false).toString();
			PrintWriter out = response.getWriter();
			out.println(ret);
			System.out.println(ret);
			return;
		}
		else if (isAudit != null && isAudit.equalsIgnoreCase("Unsubscribe"))
		{
			String sotid = request.getParameter("otid");
			ObjectTypeAndId otid = ((sotid != null) && (sotid.length() > 0)) ? new ObjectTypeAndId(sotid) : null;
  
			String ret = doSubscribe(session, otid, true).toString();
			PrintWriter out = response.getWriter();
			out.println(ret);
			System.out.println(ret);
			return;
		}

		int tid = ServletUtils.getIntParameter(request,"tid");
		String pstr = request.getParameter("pid").replaceAll("-.*$", "");
		int pid = Integer.parseInt(pstr);
		String otype = request.getParameter("otid");
		String f = request.getParameter("f");
		Task t = session.getTask(tid,true);
		Task.TaskType tt = t.getTaskType();

		if (otype != null)
			otype = otype.substring(0, 2);
		else
			otype = "";
		
		System.out.println("in RunTask - tt="+tt+" tid="+tid+" id="+id+" f="+f+" pid="+pid + " otype=" + otype);

		if (f.equalsIgnoreCase("def") || f.equalsIgnoreCase("def2"))
		{
			// If task wants to show output, wrap in tabbed wrapper
			if(t.getShowOutput() && f.equalsIgnoreCase("def") && (tt != TaskType.DEPLOY)) {
				request.setAttribute("task", t);
				request.setAttribute("id", id);
				request.getRequestDispatcher("/WEB-INF/TaskWithOutput.jsp").forward(request, response);
				return;
			}

			handleDef(session, request, response, tt, tid, id);
		}
		else
		if (f.equalsIgnoreCase("run"))
		{
			//
			// Running a task (user has clicked "Ok" on one of the dialogs presented above
			//
			System.out.println("running task "+tid);
			System.out.println("session="+session.GetSessionId());

			handleRun(session, request, response, tt, tid, id);
		}
		else if (f.equalsIgnoreCase("tasktype"))
		{
			JSONObject obj = new JSONObject();
			obj.add("tasktype", tt.toString());
			Domain dom = session.getDomain(pid);
			String domain_name = "";

			if (dom != null) {
				domain_name = dom.getParentDomain();
			}

			if (tt == TaskType.DEPLOY && otype != null && otype.equalsIgnoreCase("do")) {
				Domain mydomain = session.getDomain(id);
				List<Environment> envs = session.getEnvironmentsInDomain(mydomain);
				JSONArray arr3 = new JSONArray();
				for (Environment c: envs) {
					JSONObject cobj = new JSONObject();
					cobj.add("id", c.getId());
					cobj.add("name", c.getName());
					arr3.add(cobj);
				}
				obj.add("Environments", arr3);
				obj.add("EnvironmentCount", envs.size());
				if (dom!=null) System.out.println("mydomain.id="+mydomain.getId()+" dom.id="+dom.getId());
				else System.out.println("dom is null");
				List<Application> applications = session.getDeployableApplicationsInDomain(mydomain, dom, true, false);
				JSONArray arr1 = new JSONArray();
				for(Application c: applications) {
					 JSONObject cobj = new JSONObject();
					 cobj.add("id", c.getId());
					 cobj.add("name", c.getName());
					 cobj.add("isrelease", c.getIsRelease());
					 cobj.add("summary",c.getSummary());
					 List <Application> compvers = c.getVersions();
					 JSONArray arr2 = new JSONArray();
					 if (compvers != null) {
						 for (Application cv: compvers) {
							 JSONObject vobj = new JSONObject();
							 vobj.add("id",cv.getId());
							 vobj.add("name",cv.getName());
							 vobj.add("isrelease", c.getIsRelease());
							 vobj.add("summary",cv.getSummary());
							 arr2.add(vobj);
						 }
						 cobj.add("vercount",compvers.size());
					 } else {
						 cobj.add("vercount",0);
					 }
					 cobj.add("versions",arr2);
					 arr1.add(cobj);
				}
				obj.add("Applications", arr1);
				obj.add("ApplicationCount", applications.size());
			}
			else if (tt == TaskType.MOVE)
			{
				TaskMove tm = session.getTaskMove(tid);
				Domain to = tm.getTargetDomain();
				if (to != null)
				{
					domain_name = to.getFullDomain();
				}
			}
			else if (tt == TaskType.REMOVE)
			{
				@SuppressWarnings("unused")
				TaskRemove tm = session.getTaskRemove(tid);
			}
			else if (tt == TaskType.REQUEST)
			{
				TaskRequest tm = session.getTaskRequest(tid);
				Domain to = tm.getDomain();
				if (to != null)
				{
					domain_name = to.getFullDomain();
				}
			}			
			else if (tt == TaskType.APPROVE)
			{
				TaskApprove tm = session.getTaskApprove(tid);
				Domain to = tm.getDomain();
				if (to != null)
				{
					domain_name = to.getFullDomain();
				}
			}
			else if (tt == TaskType.CREATE_VERSION)
			{
				TaskCreateVersion tcv = session.getTaskCreateVersion(tid);
				Domain to = tcv.getTargetDomain();
				if (to != null)
				{
					domain_name = to.getFullDomain();
				}
			}
			else if (tt == TaskType.RUN_ACTION)
			{
				
			}

			if (domain_name != null && !domain_name.isEmpty()) obj.add("domain", domain_name);

			obj.add("domainid", t.getDomainId());	  
			obj.add("showoutput", t.getShowOutput());
			//
			// Add any additional parameters required
			//
			JSONArray ja = new JSONArray();
			List<TaskParameter> params = session.getTaskParameters(tid);
			for (int i=0;i<params.size();i++) {
				TaskParameter tp = params.get(i);
				JSONObject jo = new JSONObject();
				jo.add("label",tp.getLabel());
				jo.add("varname",tp.getVar());
				jo.add("type",tp.getType());
				jo.add("arrname",tp.getArr());
				ja.add(jo);
			}
			obj.add("params",ja);
			PrintWriter out = response.getWriter();
			String ret = obj.getJSON();
			out.println(ret);
			System.out.println(ret);
		}
		else if (f.equalsIgnoreCase("getarrs")) {
			JSONArray ja = new JSONArray();
			String arrname = request.getParameter("arrname");
			String sappid = request.getParameter("appid");
			String senvid = request.getParameter("envid");
			List<DMAttribute> res = new ArrayList<DMAttribute>();
			if (sappid != null) {
				// Get array values for application
				res.addAll(session.getArrayValuesForObject(ObjectType.APPLICATION,Integer.parseInt(sappid),arrname));
			}
			if (senvid != null) {
				// Get array values for environment
				System.out.println("Getting array values for environment id "+senvid);
				res.addAll(session.getArrayValuesForObject(ObjectType.ENVIRONMENT,Integer.parseInt(senvid),arrname));
			}
			for(DMAttribute a: res) {
				JSONObject jo = new JSONObject();
				jo.add("name",a.getName());
				jo.add("val",a.getValue());
				ja.add(jo);
			}
			PrintWriter out = response.getWriter();
			String ret = ja.getJSON();
			out.println(ret);
			System.out.println(ret);
		}
	}



	private void handleDef(
			DMSession so, HttpServletRequest request,
			HttpServletResponse response, Task.TaskType tt, int tid, int id)
					throws ServletException, IOException
	{
		System.out.println("def is set, tt="+tt);
		// Returning dialog for task "display" (i.e.: the dialog the user interacts with before
		// clicking "ok" to run the job

		request.setAttribute("run","Y");
		Application app = null;
		System.out.println("tt = "+tt);
		if(tt != TaskType.RUN_ACTION) {
			app = so.getApplication(id,false);
			request.setAttribute("app",app);
		}

		switch(tt) {
		case APPROVE: {
			TaskApprove taskApp = so.getTaskApprove(tid);
			Domain domain = taskApp.getApprovalDomain();
			request.setAttribute("task",taskApp);
			request.setAttribute("domain",domain);
			request.getRequestDispatcher("/WEB-INF/TaskApprove.jsp").forward(request, response);
		}
		break;				
		case REMOVE: {
			TaskRemove taskRemove = so.getTaskRemove(tid);
			request.setAttribute("task",taskRemove);
			request.setAttribute("cormicon", "copyver");
			request.getRequestDispatcher("/WEB-INF/TaskMoveCopy.jsp").forward(request, response);
		}
		break;
		case MOVE: {
			TaskMove taskMove = so.getTaskMove(tid);
			request.setAttribute("task",taskMove);
			Domain domain = taskMove.getTargetDomain();
			request.setAttribute("domain",domain);
			request.setAttribute("cormicon", "movever");
			request.getRequestDispatcher("/WEB-INF/TaskMoveCopy.jsp").forward(request, response);
		}
		break;
		case DEPLOY: {
			System.out.println("In DEPLOY");
			TaskDeploy taskDeploy = so.getTaskDeploy(tid);
			if(app != null) {
				System.out.println("app is not null, getting environments");
				List<Environment> envs = app.getPermittedEnvironments(0);
				System.out.println("env list size = " + envs.size());
				request.setAttribute("envs", envs);
			}
			request.setAttribute("task",taskDeploy);
			request.setAttribute("app",app);
			request.getRequestDispatcher("/WEB-INF/TaskDeploy.jsp").forward(request, response);
		}
		break;
		case REQUEST: {
			TaskRequest taskReq = so.getTaskRequest(tid);
			Task linkedTask = taskReq.getLinkedTask();	// The linked "Approve" task
			Domain tgtdom = null;
			String titleText="";
			switch(linkedTask.getTaskType()) {
			case APPROVE:
				TaskApprove linkTaskApprove = so.getTaskApprove(linkedTask.getId());
				tgtdom = linkTaskApprove.getApprovalDomain();
				titleText="Request Approval of Application "+app.getName()+" for Domain "+tgtdom.getName();
				break;
			case MOVE:
				TaskMove linkTaskMove = so.getTaskMove(linkedTask.getId());
				tgtdom = linkTaskMove.getTargetDomain();
				titleText="Request Move of Application "+app.getName()+" to Domain "+tgtdom.getName();
				break;
			case REMOVE:
				@SuppressWarnings("unused")
				TaskRemove linkTaskRemove = so.getTaskRemove(linkedTask.getId());
				titleText="Request Remove of Application "+app.getName();
				break;
			case DEPLOY:
				titleText="Request Deploy of Application "+app.getName();
				break;
			default:
				break;	
			}
			request.setAttribute("task",taskReq);
			request.setAttribute("domain",tgtdom);
			request.setAttribute("titletext",titleText);
			request.getRequestDispatcher("/WEB-INF/TaskRequest.jsp").forward(request, response);
		}
		break;
		case CREATE_VERSION: {
			// TODO: Check if we are being run from an application version,
			// which will become the predecessor, or if we are being run from
			// the Versions tab, whereby we won't see a predecessor yet
			//if(predecessorIsKnown) {
			//	handleRun(so, request, response, tt, tid, id);
			//}
			TaskCreateVersion taskCrVer = so.getTaskCreateVersion(tid);
			request.setAttribute("task",taskCrVer);
			Domain domain = taskCrVer.getTargetDomain();
			request.setAttribute("domain",domain);
			request.setAttribute("predlist",so.getApplicationVersions(app));
			request.getRequestDispatcher("/WEB-INF/TaskCreateVersion.jsp").forward(request, response);
		}
		break;
		case RUN_ACTION: {
			TaskUserDefined taskUserDef = so.getTaskUserDefined(tid);
			request.setAttribute("task", taskUserDef);
			request.getRequestDispatcher("/WEB-INF/TaskUserDefined.jsp").forward(request, response);
		}
		break;
		case UNKNOWN:
			break;
		}
	}

	private void handleRun(
			DMSession so, HttpServletRequest request,
			HttpServletResponse response, Task.TaskType tt, int tid, int id)
					throws IOException
	{
		response.setContentType("application/json");

		JSONObject obj = new JSONObject();
		Hashtable<String,String> additionalParameters = new Hashtable<String,String>();

		Application app = null;
		Component comp = null;
		Environment env = null;
		System.out.println("handleRun: tt="+tt+" tid="+tid+" id="+id);
		// Get any additional parameters
		Enumeration<String> parameterNames = request.getParameterNames();
		while (parameterNames.hasMoreElements()) {
			String paramName = parameterNames.nextElement();
			System.out.println("paramName=["+paramName+"]");
			if (paramName.length()>2 && paramName.substring(0,2).equalsIgnoreCase("ap")) {
				System.out.println("Additional Parameter value=["+request.getParameter(paramName)+"]");
				additionalParameters.put(paramName.substring(2),request.getParameter(paramName));
			}
		}
		// We can run an user defined action from an application or from a component. All other
		// actions are pretty much tied to applications only
		if (tt == TaskType.RUN_ACTION) {
			String objtype = request.getParameter("objtype");
			if (objtype.equalsIgnoreCase("av") || objtype.equalsIgnoreCase("ap")) {
				// This is an application
				app = so.getApplication(id,false);
			}
			else
			if (objtype.equalsIgnoreCase("rl") || objtype.equalsIgnoreCase("rv")) {
				// This is an release
				app = so.getApplication(id,false);
			}
			else
			if (objtype.equalsIgnoreCase("cv") || objtype.equalsIgnoreCase("co")) {
				comp = so.getComponent(id,false);
			}
			else
			if (objtype.equalsIgnoreCase("en")) {
				env = so.getEnvironment(id,false);
			}
		} else {
			app = so.getApplication(id,false);
		}

		boolean result;
		switch(tt) {
		case APPROVE: {
			String approve=request.getParameter("approve");
			String notes=request.getParameter("notes");
			TaskApprove taskApp = so.getTaskApprove(tid);
			taskApp.setApplication(app);
			taskApp.setApproved((approve != null) && approve.equalsIgnoreCase("Y"));
			taskApp.setText(notes);
			taskApp.setAdditionalParameters(additionalParameters);
			result = taskApp.run();
			obj.add("result", result);
			obj.add("refreshapp",result);
			obj.add("output", taskApp.getFormattedOutput());
		}
		break;
		case REMOVE: {
			System.out.println("Calling run in TaskRemove");
			String envid = request.getParameter("envid");
			Environment remenv = (envid != null)?so.getEnvironment(Integer.parseInt(envid),true):null;
			TaskRemove taskRemove = so.getTaskRemove(tid);
			taskRemove.setApplication(app);
			taskRemove.setEnvironment(remenv);
			taskRemove.setAdditionalParameters(additionalParameters);
			result = taskRemove.run();
			obj.add("result", result);
			obj.add("refreshapp",result);
			obj.add("output", taskRemove.getFormattedOutput());
		}
		break;
		case MOVE: {
			System.out.println("Calling run in TaskMove");
			String notes=request.getParameter("notes");
			TaskMove taskMove = so.getTaskMove(tid);
			taskMove.setApplication(app);
			taskMove.setText(notes);
			taskMove.setAdditionalParameters(additionalParameters);
			result = taskMove.run();
			obj.add("result", result);
			obj.add("refreshapp",result);
			obj.add("output", taskMove.getFormattedOutput());
		}
		break;
		case DEPLOY: {
			if(app == null) {
				throw new RuntimeException("Application " + id + " not found");
			}
			int envid = ServletUtils.getIntParameter(request,"envid");
			Environment depenv = so.getEnvironment(envid, false);
			if(depenv == null) {
				throw new RuntimeException("Environment " + id + " not found");
			}
			TaskDeploy taskDeploy = so.getTaskDeploy(tid);
			taskDeploy.setApplication(app);
			taskDeploy.setEnvironment(depenv);
			taskDeploy.setDeploymentSessionId(so.GetSessionId() + "_" + System.currentTimeMillis());
			taskDeploy.setAdditionalParameters(additionalParameters);
			if(taskDeploy.run()) {
				Deployment dep = so.getDeploymentBySessionId(taskDeploy, 30);
				if(dep != null) {
					obj.add("depno", dep.getId());
					obj.add("error", dep.getSummary());
				} else {
					String msg = taskDeploy.getLastOutputLine();
					obj.add("error", "Timed out waiting for deployment to start" + ((msg != null) ? ("\n" + msg) : ""));
				}
			} else {
				String msg = taskDeploy.getLastOutputLine();
				obj.add("error", "Failed to start deployment" + ((msg != null) ? ("\n" + msg) : ""));				
			}}
		break;
		case REQUEST: {
			System.out.println("Calling run in TaskRequest");
			String notes=request.getParameter("notes");
			TaskRequest taskReq = so.getTaskRequest(tid);
			taskReq.setApplication(app);
			taskReq.setText(notes);
			taskReq.setAdditionalParameters(additionalParameters);
			result = taskReq.run();
			obj.add("result", result);
			obj.add("refreshapp",result);
			obj.add("output", taskReq.getFormattedOutput());
		}
		break;
		case CREATE_VERSION: {
			int pid = ServletUtils.getIntParameter(request,"pred");
			System.out.println("pid = " + pid);
			TaskCreateVersion taskCrVer = so.getTaskCreateVersion(tid);
			taskCrVer.setApplication(app);
			if(pid > 0) {
				Application pred = (pid != 0) ? so.getApplication(pid, false) : null;
				System.out.println("pred = " + pred);
				// TODO: Check that the predecessor has the correct parent
				//if(pred.getParentId() != app.getId()) {
				//	throw
				//}
				taskCrVer.setPredecessor(pred);
			} else if(pid < 0) {
				taskCrVer.setUseLatest(true /*branch*/);
			}
			taskCrVer.setAdditionalParameters(additionalParameters);
			result = taskCrVer.run();
			obj.add("result", result);
			obj.add("refreshapp",result);
			obj.add("output", taskCrVer.getFormattedOutput());
		}
		break;
		case RUN_ACTION: {
			TaskUserDefined taskUserDef = so.getTaskUserDefined(tid);
			if (app != null) taskUserDef.setApplication(app);
			if (comp != null) taskUserDef.setComponent(comp);
			if (env != null) taskUserDef.setEnvironment(env);
			taskUserDef.setAdditionalParameters(additionalParameters);
			result = taskUserDef.run();
			obj.add("result", result);
			obj.add("output", taskUserDef.getFormattedOutput());			
		}
		break;
		case UNKNOWN:
			break;
		}

		PrintWriter out = response.getWriter();
		String ret = obj.getJSON();
		out.println(ret);
		System.out.println(ret);
	}
	
}
