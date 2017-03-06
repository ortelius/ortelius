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
//import java.io.PrintWriter;
//import java.sql.PreparedStatement;
//import java.sql.ResultSet;
import java.util.List;

import javax.servlet.ServletException;
//import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
//import javax.servlet.http.HttpSession;

//import dmadmin.json.CreatedModifiedField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
//import dmadmin.json.JSONBoolean;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;
import dmadmin.model.ComponentLink;
//import dmadmin.model.Deployment;
//import dmadmin.model.Environment;
import dmadmin.model.Server;
//import dmadmin.model.User;

/**
 * Servlet implementation class GetComponentLayout
 */

public class GetComponentLayout extends JSONServletBase {
	private static final long serialVersionUID = 1L;
	
@Override
public IJSONSerializable handleRequest(DMSession session, boolean isPost,
		HttpServletRequest request, HttpServletResponse response)
	throws ServletException, IOException
{
	
	String szApplicationID = request.getParameter("appid");
	String szServerID = request.getParameter("servid");
	String szCompID = request.getParameter("compid");
	// String szType = request.getParameter("t");
	boolean bReadOnly;
	boolean isRelease = false;
	
	if (request.getParameter("isRelease") != null && request.getParameter("isRelease").equalsIgnoreCase("Y"))
	 isRelease = true;
	
	System.out.println("isRelease="+isRelease+" szApplicationID="+szApplicationID);
	
	if (szServerID != null) {
		// Components and Versions on Server
		int serverid = Integer.parseInt(szServerID);
		Server serv = session.getServer(serverid,true);
		bReadOnly = !serv.isUpdatable();
		JSONObject obj = new JSONObject();
		TableDataSet data = session.getComponentsOnServer(serv);
		obj.add("readOnly", bReadOnly);
		obj.add("result", true);
		obj.add("data",data);
		return obj;
	} else if (szApplicationID != null ) {
		// Component Versions in Application
		System.out.println("Getting component versions in application");
		int startx;
		int appid = Integer.parseInt(szApplicationID);
		Application app = session.getApplication(appid,true);
		startx = app.getStartX();
		bReadOnly = !app.isUpdatable();
		System.out.println("getComponents isRelease="+isRelease);
		List<Component> comps = session.getComponents(ObjectType.APPLICATION,appid,isRelease);
		System.out.println("comps.size="+comps.size());
		List<ComponentLink> complinks = session.getComponentLinks(appid);
		JSONObject obj = new JSONObject();
		
		JSONArray arr1 = new JSONArray();
		for(Component ci: comps) {
			JSONObject vobj = new JSONObject();
			vobj.add("nodeid", ci.getId());
			vobj.add("name", ci.getName());
			vobj.add("suffix", ci.getIconSuffix());
			vobj.add("xpos", ci.getXpos());
			vobj.add("ypos", ci.getYpos());
			vobj.add("summary", ci.getSummary());
			System.out.println("id="+ci.getId()+" parent="+ci.getParentId());
			if (isRelease) {
				vobj.add("prefix", (ci.getParentId()>0)?"av":"ap");	// application version or base application
			} else {
				vobj.add("prefix", (ci.getParentId()>0)?"cv":"co");	// component version or base component
			}
			arr1.add(vobj);
		}
		obj.add("Nodes", arr1);
		obj.add("NodeCount", comps.size());
		
		JSONArray arr2 = new JSONArray();
		for(ComponentLink cl: complinks) {
			JSONObject vlobj = new JSONObject();
			vlobj.add("nodefrom", cl.getCompFrom());
			vlobj.add("nodeto", cl.getCompTo());
			arr2.add(vlobj);
		}
		obj.add("Links", arr2);
		obj.add("LinkCount", complinks.size());
		obj.add("readOnly", bReadOnly);
		obj.add("startx",startx);
		return obj;
	} else {
		// Component locations - either servers or applications
		int compid = Integer.parseInt(szCompID);
		String t = request.getParameter("t");
		Component comp = session.getComponent(compid,true);
		JSONObject obj = new JSONObject();
		TableDataSet data = null;
		if (t!=null && t.equalsIgnoreCase("b")) {
			// Retrieving Builds
			data = session.getBuildsForComponent(comp);
		} else {
			data = session.getComponentLocations(comp,t,isRelease);
		}
		obj.add("readOnly", true);
		obj.add("result", true);
		obj.add("data",data);
		return obj;
	}
}
}

