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

import dmadmin.model.Action;
import dmadmin.model.Application;
import dmadmin.model.CompType;
import dmadmin.model.Component;
import dmadmin.model.Credential;
import dmadmin.model.Datasource;
import dmadmin.model.Domain;
import dmadmin.model.Environment;
import dmadmin.model.Notify;
import dmadmin.model.NotifyTemplate;
import dmadmin.model.Repository;
// import javax.servlet.http.HttpSession;
import dmadmin.model.Server;
import dmadmin.model.User;
import dmadmin.model.UserGroup;


/**
 * Servlet implementation class GetMenu
 */
public class GetMenu extends HttpServletBase {
	private static final long serialVersionUID = 1L;
	
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetMenu() {
        super();
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json;charset=UTF-8");
		
		PrintWriter out = response.getWriter();
		
		String objtype = request.getParameter("t");
		String admin = request.getParameter("a");
		// admin should be just set to Y to get access to the create functions
		admin="Y";
		String ct = request.getParameter("ct");
		int domain = ServletUtils.getIntParameter(request, "d");
		String idStr = request.getParameter("id");
		int id = 0;
		if (idStr.contains("-"))
		{
		 String[] parts = idStr.split("-");
		 id = new Integer(parts[0]).intValue();
		}
		else
		 id = new Integer(idStr).intValue();
		
		System.out.println("GetMenu) objtype="+objtype+" admin="+admin+" id="+id+" ct="+ct);
		
		PropertyDataSet ds = new PropertyDataSet();
		
		String PasteObjectType = session.getPasteObjectType();
		if (PasteObjectType==null) PasteObjectType="";
		
		System.out.println("PasteObjectType="+PasteObjectType);
		
		if (objtype.equalsIgnoreCase("server"))
		{
			Server server = session.getServer(id,true);
			ds.addProperty("Copy Endpoint", "Copy");
			if (server.isUpdatable()) {
				if (ct.equalsIgnoreCase("servers"))
				 ds.addProperty("Delete this Endpoint","Delete");
			}
			if (ct.equalsIgnoreCase("servers"))
				ds.addProperty("Gather Audit Data for this Endpoint","Audit");
		}
		else
		if (objtype.equalsIgnoreCase("environment"))
		{
			Environment env = session.getEnvironment(id,true);
			if (admin.equalsIgnoreCase("y"))
			{
				if (PasteObjectType.equalsIgnoreCase("server") && env.isUpdatable()) ds.addProperty("Paste Endpoint","Paste");
//				ds.addProperty("Copy Environment", "Copy");
				if (env.isUpdatable()) {
					if (session.getCreatePermission(ObjectType.SERVER)) ds.addProperty("New Endpoint in this Environment", "NewServ");
					ds.addProperty("Delete this Environment","Delete");
				}
				
			}
			ds.addProperty("Gather Audit Data for this Environment","AuditEnv");
   
			session.GetWorkbenchTasks(domain, "environment", ds);
			if (session.CheckSubscription("environment",id)) {
				ds.addProperty("Unsubscribe from this Environment","Unsubscribe");
			} else {
				ds.addProperty("Subscribe to this Environment","Subscribe");
			}
		}
		else
		if (objtype.equalsIgnoreCase("application") || objtype.equalsIgnoreCase("appversion"))
		{
			Application app = session.getApplication(id,true);
			
			String subscript = (objtype.equalsIgnoreCase("application"))?"Base Version":"Version";
//			if (app.isUpdatable()) {
//				ds.addProperty("Delete this Application "+subscript, "Delete");
//			}
			if (domain==0) {
				// No domain specified, not been invoked from tree
				domain = app.getDomainId();
			}
			session.GetWorkbenchTasks(domain, objtype, ds);
			
			if (session.CheckSubscription(objtype,id)) {
				ds.addProperty("Unsubscribe from this Application "+subscript,"Unsubscribe");
			} else {
				ds.addProperty("Subscribe to this Application "+subscript,"Subscribe");
			}
			// COPY/PASTE
			ds.addProperty("Copy this Application","Copy");
			
			PropertyDataSet clean = new PropertyDataSet();
			ds.removeDupDeploy(clean);
			ds = clean;
		}
		else
		if (objtype.equalsIgnoreCase("release") || objtype.equalsIgnoreCase("relversion")) {
			Application app = session.getApplication(id,true);
			String subscript = (objtype.equalsIgnoreCase("release"))?"Base Version":"Version";
			if (app.isUpdatable()) {
				ds.addProperty("Delete this Release", "Delete");
			}
			session.GetWorkbenchTasks(domain, "release", ds);

			if (session.CheckSubscription(objtype,id)) {
				ds.addProperty("Unsubscribe from this Release "+subscript,"Unsubscribe");
			} else {
				ds.addProperty("Subscribe to this Release "+subscript,"Subscribe");
			}
		}
		else		 
		if (objtype.equalsIgnoreCase("domain") || objtype.equalsIgnoreCase("lifecycle")) {
			System.out.println("ct="+ct+" admin="+admin);
			System.out.println("getPasteObjectType="+session.getPasteObjectType());
			Domain dom = session.getDomain(id);
			if (dom.isUpdatable())
			{
				if (PasteObjectType.equalsIgnoreCase("environment")) {
					// ds.addProperty("Paste "+Character.toUpperCase(PasteObjectType.charAt(0))+PasteObjectType.substring(1).toLowerCase(),"Paste");
					ds.addProperty("Paste Environment", "Paste");
				}
				
				if (PasteObjectType.equalsIgnoreCase("component") || PasteObjectType.equalsIgnoreCase("compversion")) {
					ds.addProperty("Paste Component","Paste");
				}
				
				if (PasteObjectType.equalsIgnoreCase("server")) {
					ds.addProperty("Paste Endpoint","Paste");
				}
				
				if (PasteObjectType.equalsIgnoreCase("builder")) {
					ds.addProperty("Paste Build Engine","Paste");
				}
				
				if (PasteObjectType.equalsIgnoreCase("procedure")) {
					ds.addProperty("Paste Procedure","Paste");
				}
				
				if (PasteObjectType.equalsIgnoreCase("function")) {
					ds.addProperty("Paste Function","Paste");
				}
				
				if (PasteObjectType.equalsIgnoreCase("action")) {
					ds.addProperty("Paste Action","Paste");
				}
				
				if (ct.equalsIgnoreCase("domains"))
				{
					if (session.getCreatePermission(ObjectType.DOMAIN) && objtype.equalsIgnoreCase("domain")) {
						ds.addProperty("New Sub-Domain", "NewDomain");
					}
			 		ds.addProperty("Delete this Domain","Delete");
				}
				
				if (ct.equalsIgnoreCase("users"))
				{
					if (session.getCreatePermission(ObjectType.USER)) ds.addProperty("New User in this Domain","NewUser");
				}
				else
				if (ct.equalsIgnoreCase("groups"))
				{
					if (session.getCreatePermission(ObjectType.USERGROUP)) ds.addProperty("New Group in this Domain", "NewGroup");
				}
				else
				if (ct.equalsIgnoreCase("environments"))
				{
					if (session.getCreatePermission(ObjectType.ENVIRONMENT))	ds.addProperty("New Environment in this Domain","NewEnv");
				}
				else
				if (ct.equalsIgnoreCase("repositories"))
				{
					if (session.getCreatePermission(ObjectType.REPOSITORY))  ds.addProperty("New Repository in this Domain","NewRep");
				}
				else
				if (ct.equalsIgnoreCase("components"))
				{
					if (session.getCreatePermission(ObjectType.COMPONENT)) ds.addProperty("New Component Base Version in this Domain", "NewComp");
				}
				else
				if (ct.equalsIgnoreCase("credentials"))
				{
					if (session.getCreatePermission(ObjectType.CREDENTIALS)) ds.addProperty("New Credential in this Domain","NewCred");
				}
				else
				if (ct.equalsIgnoreCase("releases"))
				{
					if (session.getCreatePermission(ObjectType.APPLICATION)) ds.addProperty("New Release in this Domain","NewRelease");
				}
				else				 
				if (ct.equalsIgnoreCase("applications"))
				{
					System.out.println("PasteObjectType="+PasteObjectType);
					if (PasteObjectType.equalsIgnoreCase("appversion")) {
						ds.addProperty("Paste Application Version","Paste");
					}
					else if (PasteObjectType.equalsIgnoreCase("application")) {
						ds.addProperty("Paste Application","Paste");
					}
						// ds.addProperty("Paste "+Character.toUpperCase(PasteObjectType.charAt(0))+PasteObjectType.substring(1).toLowerCase(),"Paste");
					System.out.println("**** calling getCreatePermission for application");
					if (session.getCreatePermission(ObjectType.APPLICATION)) ds.addProperty("New Application Base Version in this Domain","NewApp");
					
					PropertyDataSet deployds = new PropertyDataSet();
					session.GetWorkbenchTasks(domain, "application", deployds);
					session.GetWorkbenchTasks(domain, "appversion", deployds);	   
		   
					deployds.copyOnlyDeploy(ds);
				}
				else
				if (ct.equalsIgnoreCase("types"))
				{
					if (session.getCreatePermission(ObjectType.SERVERCOMPTYPE)) ds.addProperty("New Component & Endpoint Type in this Domain","NewCompType");
				}
				else				 
				if (ct.equalsIgnoreCase("actions"))
				{
					if (session.getCreatePermission(ObjectType.ACTION)) {
						ds.addProperty("New Action in this Domain", "NewAction1");
					}
				}
				else
				if (ct.equalsIgnoreCase("procedures"))
				{
					if (session.getCreatePermission(ObjectType.PROCEDURE)) ds.addProperty("Import a Function or Procedure into this Domain", "Import");	// for 7.6
					if (session.getCreatePermission(ObjectType.PROCEDURE)) {
						ds.addProperty("New Procedure in this Domain", "NewAction3");
						ds.addProperty("New Function in this Domain", "NewAction2");
					}
				}
				else
				if (ct.equalsIgnoreCase("functions"))
				{
					if (session.getCreatePermission(ObjectType.PROCEDURE /* FUNCTION */)) ds.addProperty("New Function in this Domain", "NewAction2");
				}
				else
				if (ct.equalsIgnoreCase("datasources"))
				{
					if (session.getCreatePermission(ObjectType.DATASOURCE)) ds.addProperty("New Data Source in this Domain","NewDataSource");
				}
				else
				if (ct.equalsIgnoreCase("notifiers"))
				{
					if (session.getCreatePermission(ObjectType.NOTIFY)) ds.addProperty("New Notifier in this Domain", "NewNotifier");
				}
				else
				if (ct.equalsIgnoreCase("servers"))
				{
					if (session.getCreatePermission(ObjectType.SERVER)) ds.addProperty("New Endpoint in this Domain", "NewServ");
				}
			}
			session.GetWorkbenchTasks(domain, "domain", ds);
		}
		else
		if (objtype.equalsIgnoreCase("repository"))
		{
//			ds.addProperty("Copy Repository", "Copy");
			Repository repo = session.getRepository(id, true);
			if (repo.isUpdatable()) {
				ds.addProperty("Delete this Repository","Delete");
			}
		}
		else
		if (objtype.equalsIgnoreCase("user"))
		{
//			ds.addProperty("Copy User", "Copy");
			User user = session.getUser(id);
			if (user.isUpdatable()) {
				ds.addProperty("Delete this User","Delete");
			}
		}
		else
		if (objtype.equalsIgnoreCase("usergroup"))
		{
//			ds.addProperty("Copy User Group", "Copy");
			UserGroup ug = session.getGroup(id);
			if (ug.isUpdatable()) {
				ds.addProperty("Delete this User Group","Delete");
			}
		}
		else
		if (objtype.equalsIgnoreCase("component"))
		{
			Component comp = session.getComponent(id,true);
			session.GetWorkbenchTasks(domain, "component", ds);
			ds.addProperty("Copy Component", "Copy");
			if (comp.isUpdatable()) {
				ds.addProperty("Delete this Component Base Version","Delete");
			}
		}
		else
		if (objtype.equalsIgnoreCase("compversion"))
		{
			Component comp = session.getComponent(id,true);
			session.GetWorkbenchTasks(domain, "compversion", ds);
			ds.addProperty("Copy Component Version", "Copy");
			if (comp.isUpdatable()) {
				ds.addProperty("Delete this Component Version","Delete");
			}
		}
		else
		if (objtype.equalsIgnoreCase("procedure"))
		{
			Action action = session.getAction(id,true);
			ds.addProperty("Copy Procedure", "Copy");
			if (action.isUpdatable()) {
				ds.addProperty("Delete this Procedure", "Delete");
			}
			ds.addProperty("Export this Procedure", "Export");		// for 7.6
			ds.addProperty("Show References","Refs");
		}
		else
		if (objtype.equalsIgnoreCase("datasource"))
		{
			Datasource datasource = session.getDatasource(id,true);
			//   ds.addProperty("Copy Data Source", "Copy");
			//   ds.addProperty("Rename this Data Source", "Rename");
			ds.addProperty("Test Connection to this Data Source","TestDataSource");		// for 7.6
			if (datasource.isUpdatable()) {
				ds.addProperty("Delete this Data Source", "Delete");
			}
		}
		else
		if (objtype.equalsIgnoreCase("action"))
		{
			Action action = session.getAction(id,true);
			if (ct.equalsIgnoreCase("procedures"))
			{
				//	    ds.addProperty("Copy Procedure", "Copy");
				//	    ds.addProperty("Rename this Procedure", "Rename");
				if (action.isUpdatable()) {
					ds.addProperty("Delete this Procedure", "Delete");
				}
			}
			else if (ct.equalsIgnoreCase("functions"))
			{
				//ds.addProperty("Copy Function", "Copy");
				//			 ds.addProperty("Rename this Function", "Rename");
				if (action.isUpdatable()) {
					ds.addProperty("Delete this Function", "Delete");
				}
			}
			else
		    {
		//    ds.addProperty("Copy Action", "Copy");
		//    ds.addProperty("Rename this Action", "Rename");
				ds.addProperty("Copy Action", "Copy");
				ds.addProperty("Show References","Refs");
				if (action.isUpdatable()) {
					ds.addProperty("Delete this Action", "Delete");
				}
		    } 
		}
		else
		if (objtype.equalsIgnoreCase("function"))
		{
			Action action = session.getAction(id,true);
			ds.addProperty("Copy Function", "Copy");
//			ds.addProperty("Rename this Function", "Rename");
			if (action.isUpdatable()) {
				ds.addProperty("Delete this Function", "Delete");
			}
			ds.addProperty("Export this Function", "Export");		// for 7.6
			ds.addProperty("Show References","Refs");
		}
		else
	  if (objtype.equalsIgnoreCase("credentials"))
	  {
		  Credential cred = session.getCredential(id,false);
//	   ds.addProperty("Copy Credential", "Copy");
//	   ds.addProperty("Rename this Function", "Rename");
		  if (cred.isUpdatable()) {
			  ds.addProperty("Delete this Credential", "Delete");
		  }
	  }
	  else		 
	   if (objtype.equalsIgnoreCase("ServerCompType"))
	   {
		   CompType ctype = session.getServerCompTypeDetail(id);
		   
//	    ds.addProperty("Copy Credential", "Copy");
//	    ds.addProperty("Rename this Function", "Rename");
		   if (ctype.isUpdatable()) {
			   ds.addProperty("Delete this Endpoint Type", "Delete");
		   }
	   }
	   else    
		if (objtype.equalsIgnoreCase("notify"))
		{
			Notify notify = session.getNotify(id,true);
			boolean upd = notify.isUpdatable();
			if (PasteObjectType.equalsIgnoreCase("template") && upd) {
				ds.addProperty("Paste Template","Paste");
			}
			ds.addProperty("Send a Test Message using this Notifier","TestNotify");		// for 7.6
			if (upd) {
				ds.addProperty("New Notify Template for this Notifier","NewTemplate");
				ds.addProperty("Delete this Notifier", "Delete");
			}
		}
		else
		if (objtype.equalsIgnoreCase("template"))
		{
			NotifyTemplate template = session.getTemplate(id);
			ds.addProperty("Copy Template", "Copy");
//			ds.addProperty("Rename this Template", "Rename");
			if (template.isUpdatable()) {
				ds.addProperty("Delete this Template","Delete");
			}
		}
		out.print(ds.getMenuJSON());
   	}
}
