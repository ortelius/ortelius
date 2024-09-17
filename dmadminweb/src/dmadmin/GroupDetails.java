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
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.User;
import dmadmin.model.UserGroup;
import dmadmin.model.UserList;
import dmadmin.model.UserPermissions;

/**
 * Servlet implementation class GroupDetails
 */
public class GroupDetails extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GroupDetails() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
    public void handleRequest(DMSession session, boolean isPost,
    	   		HttpServletRequest request, HttpServletResponse response)
    	   throws ServletException, IOException
    {
    	if (!isPost) {
    		//
    		// GET mode
    		// --------
    		// (set attributes and redirect to jsp)
    		//
    		String admin = request.getParameter("a");
    		String type = request.getParameter("type");
    		PanelTabsList pt = session.getTabsForPanel("group",admin);
    		request.setAttribute("admin",admin);
    		request.setAttribute("pt", pt);

    		int gid = ServletUtils.getIntParameter(request,"gid");

    		UserGroup grp = session.getGroup(gid);
    		UserList ul1 = session.getUsersInGroup(gid);
    		UserList ul2 = session.getUsersNotInGroup(gid);
    		UserPermissions up = session.getUserPermissions();
    		UserPermissions gp = grp.getUserPermissions();

    		String aclDisabled       = session.getAclOverride()?"":"disabled";
    		String tabEndDisabled    = session.getEndPointsTabAccess()?"":"disabled";
    		String tabAppDisabled    = session.getApplicationsTabAccess()?"":"disabled";
    		String tabActionDisabled = session.getActionsTabAccess()?"":"disabled";
    		String tabProvDisabled   = session.getProvidersTabAccess()?"":"disabled";
    		String tabUserDisabled   = session.getUsersTabAccess()?"":"disabled";

    		if (type.equalsIgnoreCase("AdminRights")) {
    			JSONArray js = new JSONArray();
    			if (up != null && gp != null) {
     Add2Array(js,"override", aclDisabled,grp.getAclChecked());
					Add2Array(js,"cusers",up.getUsersDisabled(),gp.getUsersChecked());
					Add2Array(js,"cgroups",up.getGroupsDisabled(),gp.getGroupsChecked());
					Add2Array(js,"cdomain",up.getDomainsDisabled(),gp.getDomainsChecked());
					Add2Array(js,"cenv",up.getEnvsDisabled(),gp.getEnvsChecked());
					Add2Array(js,"cserver",up.getServersDisabled(),gp.getServersChecked());
					Add2Array(js,"crepo",up.getReposDisabled(),gp.getReposChecked());
					Add2Array(js,"ccomp",up.getCompsDisabled(),gp.getCompsChecked());
					Add2Array(js,"ccred",up.getCredsDisabled(),gp.getCredsChecked());
					Add2Array(js,"capp",up.getAppsDisabled(),gp.getAppsChecked());
					Add2Array(js,"cappver",up.getAppversDisabled(),gp.getAppversChecked());
					Add2Array(js,"caction",up.getActionsDisabled(),gp.getActionsChecked());
					Add2Array(js,"cproc",up.getProcsDisabled(),gp.getProcsChecked());
					Add2Array(js,"cdsrc",up.getDatasrcDisabled(),gp.getDatasrcChecked());
					Add2Array(js,"cnotify",up.getNotifiersDisabled(),gp.getNotifiersChecked());
					Add2Array(js,"cengine",up.getEnginesDisabled(),gp.getEnginesChecked());
     Add2Array(js,"cservercomptype",up.getServerCompTypeDisabled(),gp.getServerCompTypeChecked());
					Add2Array(js,"endpoints",tabEndDisabled,grp.getTabEndChecked());
					Add2Array(js,"applications",tabAppDisabled,grp.getTabAppChecked());
					Add2Array(js,"actions", tabActionDisabled,grp.getTabActionChecked());
					Add2Array(js,"providers", tabProvDisabled,grp.getTabProvChecked());
					Add2Array(js,"users", tabUserDisabled,grp.getTabUserChecked());
    			}
    			PrintWriter out = response.getWriter();
    			out.println(js.toString());
    		} else if (type.equalsIgnoreCase("Users4Group")) {
    			JSONObject jo = new JSONObject();
    			JSONArray js = new JSONArray();
    			boolean acloverride = session.getAclOverride();
    			boolean readonly = !acloverride;
    			System.out.println("acloverride="+acloverride+" readonly="+readonly);
    			User myuser = session.getUser(session.GetUserID());
    			int mydom = myuser.getDomainId();

    			if (ul1 != null) {
    				for (int i=0;i<ul1.size();i++) {
    					JSONObject o = new JSONObject();
    					int x = ul1.get(i).getId();
    					if (x == session.GetUserID() && !acloverride) {
    						// We are in this user group, so we're allowed to add other people to this
    						// group (assuming we've got edit permission on the domain
    						System.out.println("in group - isUpdatable="+grp.isUpdatable());
    						readonly =!grp.isUpdatable();
    					}
    					User usr = ul1.get(i);
    					int domid = usr.getDomainId();
    					String domname = session.getDomainName(domid);
    					if (domid == mydom)
    					{
    					 o.add("id", x);
    					 o.add("name", domname + "." + usr.getName());
    					 js.add(o);
    					}
    				}
    			}
    			if (gid==1) readonly=true;	// Everyone (gid=1) should not be amendable by ANYONE!
    			PrintWriter out = response.getWriter();
    			jo.add("users", js);
    			jo.add("readonly",readonly);
    			out.println(jo.toString());
    		} else if (type.equalsIgnoreCase("AvailUsers4Group")) {
    			JSONArray js = new JSONArray();
    			if (ul2 != null) {
        User myuser = session.getUser(session.GetUserID());
        String mydoms = session.getDomainList();

    				for (int i=0;i<ul2.size();i++) {
    					JSONObject o = new JSONObject();
         User usr = ul2.get(i);
         int domid = usr.getDomainId();
         String domname = session.getDomainName(domid);
         if (mydoms.contains(Integer.valueOf(domid).toString()))
         {
          o.add("id", usr.getId());
          o.add("name", domname + "." + usr.getName());
          js.add(o);
         }
    				}
    			}
    			PrintWriter out = response.getWriter();
    			out.println(js.toString());
    		}
    	} else {
    		//
    		// POST mode
    		// ---------
    		// Update Group (user membership and permissions)
    		//
    		response.setContentType("application/json");
    		String func = request.getParameter("f");
    		int uid;
    		int gid = ServletUtils.getIntParameter(request, "gid");
    		PrintWriter out = response.getWriter();
    		System.out.println("In GroupDetails post func="+func);
    		int res=0;
    		if (func.equalsIgnoreCase("a"))
    		{
    			// adding a user to a group
    			uid = ServletUtils.getIntParameter(request,"id");
    			res = session.AddUserToGroup(gid,uid);

    		}
    		else
    		if (func.equalsIgnoreCase("r"))
    		{
    			// removing a user from a group
    			uid = ServletUtils.getIntParameter(request,"id");
    			res = session.RemoveUserFromGroup(gid,uid);
    		}
    		else
    		if (func.equalsIgnoreCase("sv"))
    		{
    			// Changing the group "create object" permissions
    			String szOverride = request.getParameter("override");
    			String szEndPoints = request.getParameter("endpoints");
    			String szApplications = request.getParameter("applications");
    			String szActions = request.getParameter("actions");
    			String szProviders = request.getParameter("providers");
    			String szUsers = request.getParameter("users");

    			// Double check the permissions here and remove those to which we don't have access. GroupDetails.jsp will
    			// already have done this ... but we shouldn't just trust the URL.
    			//
    			if (!session.getAclOverride()) szOverride=null;
    			if (!session.getEndPointsTabAccess()) szEndPoints=null;
    			if (!session.getApplicationsTabAccess()) szApplications=null;
    			if (!session.getActionsTabAccess()) szActions=null;
    			if (!session.getProvidersTabAccess()) szProviders=null;
    			if (!session.getUsersTabAccess()) szUsers=null;

    			session.setGroupTabAccess(gid, szOverride, szEndPoints, szApplications, szActions, szProviders, szUsers);

    			String szCreateUsers = request.getParameter("cusers");
    			String szCreateGroups = request.getParameter("cgroups");
    			String szCreateDomains = request.getParameter("cdomain");
    			String szCreateEnvs = request.getParameter("cenv");
    			String szCreateServers = request.getParameter("cserver");
    			String szCreateRepos = request.getParameter("crepo");
    			String szCreateComps = request.getParameter("ccomp");
    			String szCreateCreds = request.getParameter("ccred");
    			String szCreateApps = request.getParameter("capp");
    			String szCreateAppvers = request.getParameter("cappver");
    			String szCreateActions = request.getParameter("caction");
    			String szCreateProcs = request.getParameter("cproc");
    			String szCreateDatasrc = request.getParameter("cdsrc");
    			String szCreateNotifiers = request.getParameter("cnotify");
       String szCreateEngines = request.getParameter("cengine");
       String szCreateServerCompType = request.getParameter("cservercomptype");

    			UserPermissions up = new UserPermissions(session,gid);

    			// Check the user's permissions to make sure they're not firing us a dodgy URL
    			UserPermissions op = session.getUserPermissions();

    			if (szCreateUsers != null && op.getCreateUsers()) {up.setCreateUsers(szCreateUsers.equalsIgnoreCase("y"));}
    			if (szCreateGroups != null && op.getCreateGroups()) {up.setCreateGroups(szCreateGroups.equalsIgnoreCase("y"));}
    			if (szCreateDomains != null && op.getCreateDomains()) {up.setCreateDomains(szCreateDomains.equalsIgnoreCase("y"));}
    			if (szCreateEnvs != null && op.getCreateEnvs()) {up.setCreateEnvs(szCreateEnvs.equalsIgnoreCase("y"));}
    			if (szCreateServers != null && op.getCreateServers()) {up.setCreateServers(szCreateServers.equalsIgnoreCase("y"));}
    			if (szCreateRepos != null && op.getCreateRepos()) {up.setCreateRepos(szCreateRepos.equalsIgnoreCase("y"));}
    			if (szCreateComps != null && op.getCreateComps()) {up.setCreateComps(szCreateComps.equalsIgnoreCase("y"));}
    			if (szCreateCreds != null && op.getCreateCreds()) {up.setCreateCreds(szCreateCreds.equalsIgnoreCase("y"));}
    			if (szCreateApps != null && op.getCreateApps()) {up.setCreateApps(szCreateApps.equalsIgnoreCase("y"));}
    			if (szCreateAppvers != null && op.getCreateAppvers()) {up.setCreateAppvers(szCreateAppvers.equalsIgnoreCase("y"));}
    			if (szCreateActions != null && op.getCreateActions()) {up.setCreateActions(szCreateActions.equalsIgnoreCase("y"));}
    			if (szCreateProcs != null && op.getCreateProcs()) {up.setCreateProcs(szCreateProcs.equalsIgnoreCase("y"));}
    			if (szCreateDatasrc != null && op.getCreateDatasrc()) {up.setCreateDatasrc(szCreateDatasrc.equalsIgnoreCase("y"));}
    			if (szCreateNotifiers != null && op.getCreateNotifiers()) {up.setCreateNotifiers(szCreateNotifiers.equalsIgnoreCase("y"));}
    			if (szCreateEngines != null && op.getCreateEngines()) {up.setCreateEngines(szCreateEngines.equalsIgnoreCase("y"));}
       if (szCreateServerCompType != null && op.getCreateServerCompType()) {up.setCreateServerCompType(szCreateServerCompType.equalsIgnoreCase("y"));}

    			session.setPermissionsForGroup(gid,up);
    		}
    		String errtext;
    		errtext = "";
    		if (res>0) errtext="Failed to update database";
    		out.print("{\"errtext\" : \"" + errtext + "\", \"errcode\" : \"" + res + "\"}");
    	}
    }

    void Add2Array(JSONArray js, String key, String disabled, String checked)
    {
     JSONObject o = new JSONObject();
     o.add("key", "ar_" + key);
     o.add("disabled", disabled);
     o.add("checked", checked);
     js.add(o);
    }
}
