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
    	int domainid = ServletUtils.getIntParameter(request, "domainid");
    	if (!isPost) {
    		// GET
    		Domain domain = session.getDomain(domainid);
    		TaskList tasks = session.getTasksInDomain(domainid);
    		List<TreeObject> subdomains = session.getDomains(domainid);
    		String f = request.getParameter("f");
    		if (f == null) {
	    		request.setAttribute("domain", domain);
	    		request.setAttribute("tasklist",tasks);
	    		request.setAttribute("subdomains",subdomains);
	    		request.setAttribute("sdc",subdomains.size());
	            request.getRequestDispatcher("/WEB-INF/domaindetails.jsp").forward(request, response);
    		} else if (f.equalsIgnoreCase("gp")) {
    			// get parameters for a task
    			PrintWriter out = response.getWriter();
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
    			out.print(res.toString());
    		} 
    	} else {
    		// POST
    		response.setContentType("application/json;charset=UTF-8");
    		PrintWriter out = response.getWriter();
    		String f=request.getParameter("f");
    		String tasktype=request.getParameter("tasktype");
    		if (f.equalsIgnoreCase("at")) {
    			// Add a task
    			int id = session.getID("task");
    			String taskname=tasktype+"-"+Integer.toString(id);
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
