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
		Component comp = session.getComponentDetail(compid);
		JSONObject obj = new JSONObject();
		TableDataSet data = null;
		data = session.getComponentLocations(comp,t,isRelease);

		obj.add("readOnly", true);
		obj.add("result", true);
		obj.add("data",data);
		return obj;
	}
}
}
