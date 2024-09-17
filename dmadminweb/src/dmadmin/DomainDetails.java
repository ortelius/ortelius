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
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Domain;
import dmadmin.model.TaskList;
import dmadmin.model.TaskParameter;
import dmadmin.model.TreeObject;
import dmadmin.model.UserGroup;
import dmadmin.model.UserGroupList;

/**
 * Servlet implementation class DomainDetails
 */
public class DomainDetails extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public DomainDetails() {
        super();
        // TODO Auto-generated constructor stub
    }


    @Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
     PrintWriter out = response.getWriter();
    	int domainid = ServletUtils.getIntParameter(request, "domainid");
    	if (!isPost) {
    		// GET

    		TaskList tasks = session.getTasksInDomain(domainid);

    		String f = request.getParameter("f");
    		if (f == null) {
       JSONArray res = new JSONArray();
       for (int i=0;i<tasks.size();i++) {
        JSONObject obj = new JSONObject();
        obj.add("name", tasks.get(i).getName());
        obj.add("id", tasks.get(i).getOtid().toString());
        res.add(obj);
       }
       out.print(res.getJSON());
    		} else if (f.equalsIgnoreCase("gp")) {
    			// get parameters for a task
    			int tid = ServletUtils.getIntParameter(request,"tid");
    			List<TaskParameter> tparams = session.getTaskParameters(tid);
    			JSONArray res = new JSONArray();
    			for (int i=0;i<tparams.size();i++) {
    				JSONObject jo = new JSONObject();
    				jo.add("pos",tparams.get(i).getPos());
    				jo.add("label",tparams.get(i).getLabel());
    				jo.add("var",tparams.get(i).getVar());
    				jo.add("type",tparams.get(i).getType());
    				jo.add("arr",tparams.get(i).getArr());
    				res.add(jo);
    			}
    			out.print(res.getJSON());
    		}
    	} else {
    		// POST
    		response.setContentType("application/json;charset=UTF-8");
    		String f=request.getParameter("f");
    		String tasktype=request.getParameter("tasktype");
    		if (f.equalsIgnoreCase("at")) {
    			// Add a task
    			int id = session.getID("task");

    			String taskname = tasktype;

    			if (tasktype.equalsIgnoreCase("Approve"))
     			 taskname = "Approve Version for move to next pipeline stage";
    			else if (tasktype.equalsIgnoreCase("Move"))
    			  taskname = "Move Version to the next or previous stage in the pipeline";
    			else if (tasktype.equalsIgnoreCase("Remove"))
   			   taskname = "Delete all Versions from all Endpoints in an Environment";
    			else if (tasktype.equalsIgnoreCase("CreateVersion"))
    			  taskname = "Create new Version";
    			else if (tasktype.equalsIgnoreCase("Deploy"))
    			  taskname = "Deploy Version to an Environment";
    			else if  (tasktype.equalsIgnoreCase("Request"))
    			 taskname = "Request Calendar entry for deployment to an Environment";
    			else if  (tasktype.equalsIgnoreCase("RunAction"))
    				taskname = "Mannually trigger Action to be executed";

    			session.CreateNewTask(taskname,tasktype,domainid,id);
    			out.print("{\"name\" : \"" + taskname + "\", \"id\" : \"" + id + "\"}");
    		}
    		else
    		if (f.equalsIgnoreCase("dt")) {
    			// Delete a task
    			int tid = ServletUtils.getIntParameter(request, "tid");
    			session.DeleteTask(tid,domainid);
    			out.print("{\"errtext\" : \"\", \"errcode\" : \"0\"}");
    		}
    		else
    		if (f.equalsIgnoreCase("gg")) {
    			// Get Groups
    			int tid = ServletUtils.getIntParameter(request, "tid");
    			UserGroupList tg1 = session.getGroupsForTask(tid,true);
    			UserGroupList tg2 = session.getGroupsForTask(tid,false);
    			System.out.print("[");
    			out.print("[");
    			String sep="";
    			for (UserGroup g: tg1)
    			{
    				System.out.print(sep+"{\"mode\": \"a\", \"id\" : \""+g.getId()+"\", \"name\" : \""+g.getName()+"\"}");
    				out.print(sep+"{\"mode\": \"a\", \"id\" : \""+g.getId()+"\", \"name\" : \""+g.getName()+"\"}");
    				sep=",";
    			}
    			for (UserGroup g: tg2)
    			{
    				System.out.print(sep+"{\"mode\": \"b\", \"id\" : \""+g.getId()+"\", \"name\" : \""+g.getName()+"\"}");
    				out.print(sep+"{\"mode\": \"b\", \"id\" : \""+g.getId()+"\", \"name\" : \""+g.getName()+"\"}");
    				sep=",";
    			}
    			out.print("]");
    			System.out.print("]");
    		}
    		else
    		if (f.equalsIgnoreCase("ag")) {
    			// Add Group
    			int gid = ServletUtils.getIntParameter(request, "gid");
    			int tid = ServletUtils.getIntParameter(request, "tid");
    			session.AddGroupToTask(tid,gid);
    			out.print("{\"errtext\" : \"\", \"errcode\" : \"0\"}");

    		}
    		else
    		if (f.equalsIgnoreCase("rg")) {
    			// Remove Group
    			int gid = ServletUtils.getIntParameter(request, "gid");
    			int tid = ServletUtils.getIntParameter(request, "tid");
    			session.RemoveGroupFromTask(tid,gid);
    			out.print("{\"errtext\" : \"\", \"errcode\" : \"0\"}");
    		}
    		else
    		if (f.equalsIgnoreCase("gt")) {
    			// Get Task Details

    		}
    		else
    		if (f.equalsIgnoreCase("md")) {
    			// Move sub-domain order
    			String[] domorder = request.getParameterValues("dom");
    			session.updateDomainOrder(domorder);
    			out.print("{\"errtext\" : \"\", \"errcode\" : \"0\"}");
    		}
    		else if (f.equalsIgnoreCase("atp")) {
    			// add parameter to a task
    			System.out.println("POST atp - adding parameter");
    			int tid = ServletUtils.getIntParameter(request,"tid");
    			String label = request.getParameter("l");
    			String varname = request.getParameter("v");
    			String vartype = request.getParameter("t");
    			String arrname = request.getParameter("an");
    			int pos = session.addTaskParameter(tid,label,varname,vartype,arrname);
    			out.print("{\"errtext\" : \"\", \"errcode\" : \"0\", \"pos\" : \""+pos+"\"}");
    		}
    		else if (f.equalsIgnoreCase("etp")) {
    			// edit task parameter
    			System.out.println("POST etp - adding parameter");
    			int tid = ServletUtils.getIntParameter(request,"tid");
    			int pos = ServletUtils.getIntParameter(request,"p");
    			String label = request.getParameter("l");
    			String varname = request.getParameter("v");
    			String vartype = request.getParameter("t");
    			String arrname = request.getParameter("an");
    			session.editTaskParameter(tid,pos,label,varname,vartype,arrname);
    			out.print("{\"errtext\" : \"\", \"errcode\" : \"0\"}");
    		}
    		else if (f.equalsIgnoreCase("ctp")) {
    			// Change task parameter position
    			System.out.println("POST ctp - changing parameter position");
    			int tid = ServletUtils.getIntParameter(request,"tid");
    			String varname = request.getParameter("v");
    			int newpos = ServletUtils.getIntParameter(request,"np");
    			session.changeTaskParameterPos(tid,varname,newpos);
    		}
    		else if (f.equalsIgnoreCase("dtp")) {
    			// Delete task parameter
    			System.out.println("POST dtp - delete parameter");
    			int tid = ServletUtils.getIntParameter(request,"tid");
    			String varname = request.getParameter("v");
    			session.deleteTaskParameter(tid,varname);
    			out.print("{\"errtext\" : \"\", \"errcode\" : \"0\"}");
    		}
    	}
	}
}
