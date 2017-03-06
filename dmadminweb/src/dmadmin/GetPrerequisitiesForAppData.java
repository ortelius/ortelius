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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;

/**
 * Servlet implementation class GetPrerequisitiesForAppData
 */
public class GetPrerequisitiesForAppData extends JSONServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetPrerequisitiesForAppData() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int appid = getIntParameter(request, "appid");
		Application app = session.getApplication(appid, false);
		
		String reason = request.getParameter("reason");
		if(reason != null) {
			String objtype = request.getParameter("objtype");
			if (reason.equalsIgnoreCase("add")) {
				if (objtype.equalsIgnoreCase("c")) {
					// Component
					int compid = getIntParameter(request, "id");
					return addToPrerequisities(session, ObjectType.COMPONENT,appid, compid);
				}
				else
				if (objtype.equalsIgnoreCase("a") || objtype.equalsIgnoreCase("av")) {
					// Application or Application Version
					int depid = getIntParameter(request, "id");
					return addToPrerequisities(session, ObjectType.APPLICATION,appid, depid);
				}	
			} else 
			if (reason.equalsIgnoreCase("change")) {
				String coltype = request.getParameter("coltype");
				String colval = request.getParameter("colval");
				if (objtype.equalsIgnoreCase("c")) {
					// Component
					int compid = getIntParameter(request,"id");
					return changePrerequisite(session,ObjectType.COMPONENT,appid,compid,coltype,colval);
				} else {
					// Application
					int depid = getIntParameter(request,"id");
					return changePrerequisite(session,ObjectType.APPLICATION,appid,depid,coltype,colval);
				}
			} else {
				return new JSONObject().add("result", false).add("error", "Invalid reason");
			}
		}
		
		JSONObject obj = new JSONObject();
		obj.add("readOnly", !app.isUpdatable());
		TableDataSet data = session.getPreRequisitiesForApp(app); 
		obj.add("data", data);
		
		return obj;
	}
	
	private IJSONSerializable addToPrerequisities(DMSession session, ObjectType t,int appid, int id)
	{
		try {
			Application app = session.getApplication(appid, false);
			if(!app.isUpdatable()) {
				return new JSONObject().add("result", false).add("error", "Permision denied");
			}
			boolean result=false;
			if (t == ObjectType.COMPONENT) {
				Component comp = session.getComponent(id,false);
				result = session.addToPrerequisities(app,comp);
			}
			else
			if (t == ObjectType.APPLICATION) {
				Application depapp = session.getApplication(id,false);
				result = session.addToPrerequisities(app,depapp);
			}
			return new JSONObject().add("result", result);
		} catch(Exception e) {
			e.printStackTrace();
			return new JSONObject().add("result", false).add("error", e);
		}
	}
	
	private IJSONSerializable changePrerequisite(DMSession session, ObjectType t,int appid, int id,String coltype,String newval)
	{
		try {
			Application app = session.getApplication(appid, false);
			if(!app.isUpdatable()) {
				return new JSONObject().add("result", false).add("error", "Permision denied");
			}
			boolean result = session.ChangePrerequisite(app,t,id,coltype,newval);
			return new JSONObject().add("result", result);
		} catch(Exception e) {
			e.printStackTrace();
			return new JSONObject().add("result", false).add("error", e);
		}
	}
}




