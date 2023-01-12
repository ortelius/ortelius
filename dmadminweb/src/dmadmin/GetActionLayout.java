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
import javax.servlet.http.HttpSession;

import dmadmin.model.Action;

/**
 * Servlet implementation class GetActionLayout
 */
public class GetActionLayout extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetActionLayout() {
        super();
        // TODO Auto-generated constructor stub
    }

    private void processActionLayout(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    	response.setContentType("application/json;charset=UTF-8");
		String actionid = request.getParameter("actionid");
		String windowid = request.getParameter("windowid");

		if (actionid.indexOf("-") > -1) {
			actionid = actionid.substring(0,actionid.indexOf("-"));
			System.out.println("actionid now "+actionid);
		}

		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		Action action = so.getAction(Integer.parseInt(actionid), false);
		boolean readonly = !(action.isUpdatable());
		if (action.getParentId()>0) readonly=true;	// archived actions are always readonly

		out.print("{");

		int startx = windowid!=null?so.getStartXPosition(Integer.parseInt(actionid),Integer.parseInt(windowid)):so.getStartXPosition(Integer.parseInt(actionid),0);

		List <DMActionNode> ans = windowid!=null?so.GetActionNodes(Integer.parseInt(actionid),Integer.parseInt(windowid)):so.GetActionNodes(Integer.parseInt(actionid),0);
		List <DMActionLink> als = windowid!=null?so.GetActionLinks(Integer.parseInt(actionid),Integer.parseInt(windowid)):so.GetActionLinks(Integer.parseInt(actionid),0);

		out.println("\"Nodes\": [");
		boolean sep=false;
		for (DMActionNode a: ans)
		{
			if (sep) out.println(",");
			String Title = a.getTitle();
			if (Title.length()==0) Title = a.getDescriptor();
			int style=0;
			if (a.getTypeID()==60) style=1;	// Fragment ID of "if" is 60, change style for IF
			out.println("{");
			out.println("\"nodeid\" : \""     + a.getNodeID() + "\",");
			out.println("\"descriptor\" : \"" + a.getDescriptor() + "\",");
			out.println("\"xpos\" : \""       + a.getXpos() + "\",");
			out.println("\"ypos\" : \""       + a.getYpos() + "\",");
			out.println("\"typeid\" : \""     + a.getTypeID() + "\",");
			out.println("\"exitpoints\" : \"" + a.getExitPoints() + "\",");
			out.println("\"title\" : \""      + Title + "\",");
			out.println("\"summary\" : \""    + a.getSummary() + "\",");
			out.println("\"drilldown\" : \""  + a.getDrillDown() + "\",");
			out.println("\"functionid\" : \"" + a.getFunctionID() + "\",");
			out.println("\"procid\" : \""     + a.getProcedureID() + "\",");
			out.println("\"kind\" : \""       + a.getKind() +"\",");
			out.println("\"style\" : \""      + style +"\"");
			System.out.println("\"drilldown\" : \""  + a.getDrillDown() + "\"");
			out.println("}");
			sep=true;
		}
		out.println("],");
		out.println("\"Links\": [");
		sep=false;
		for (DMActionLink al: als)
		{
			if (sep) out.println(",");
			out.println("{");
			out.println("\"flowid\" : \""     + al.getFlowID() + "\",");
			out.println("\"nodefrom\" : \""   + al.getNodeFrom() + "\",");
			out.println("\"nodeto\" : \""     + al.getNodeTo() + "\",");
			out.println("\"pos\" : \""        + al.getPos() + "\"");
			out.println("}");
			sep=true;
		}
		out.println("],");
		out.println("\"LinkCount\":\"" + als.size() + "\",");
		out.println("\"NodeCount\":\"" + ans.size() + "\",");
		out.println("\"MaxWinID\":\""  + so.getMaxWindowID(Integer.parseInt(actionid)) + "\",");
		out.println("\"startx\": \""   + startx +"\",");
		out.println("\"readonly\": " + readonly);
		out.println("}");
		System.out.println("MaxWinID="+so.getMaxWindowID(Integer.parseInt(actionid)));
  }
    }

    private void processExport(HttpServletRequest request, HttpServletResponse response,boolean convert) throws ServletException, IOException {


		String actionid = request.getParameter("actionid");

		if (actionid.indexOf("-") > -1) {
			actionid = actionid.substring(0,actionid.indexOf("-"));
			System.out.println("actionid now "+actionid);
		}

		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		int id = Integer.parseInt(actionid);
		Action action = so.getAction(id, false);

		// response.reset();
    	response.setHeader("Expires", "0");
    	response.setHeader("Cache-Control","must-revalidate, post-check=0, pre-check=0");
    	response.setHeader("Pragma", "public");
    	response.setContentType("application/download");
    	if (convert) {
    		response.setHeader("Content-Disposition","attachment; filename=export_"+action.getName()+"_converted.re;");
	    	out.println("<!-- DeployHub Export File (converted from action \""+action.getName()+"\") -->");
	    	so.ExportGraphAsProcedure(id,out);
    	} else {
	    	response.setHeader("Content-Disposition","attachment; filename=export_"+action.getName()+".re;");
	    	out.println("<!-- DeployHub Export File -->");
	    	so.ExportFunction(id,out);
    	}
    }
   }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		String f = request.getParameter("f");
		if (f != null) {
			// Function is set
			if (f.equalsIgnoreCase("export")) processExport(request,response,false);
			if (f.equalsIgnoreCase("convert")) processExport(request,response,true);
		} else {
			processActionLayout(request,response);
		}

	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
