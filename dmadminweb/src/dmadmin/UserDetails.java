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
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Notify;
import dmadmin.model.ProviderDefinition;
import dmadmin.model.User;
import dmadmin.model.UserGroupList;

/**
 * Servlet implementation class UserDetails
 */
public class UserDetails extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UserDetails() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
   	public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	if (!isPost) {
    		// GET - display
    		String admin = request.getParameter("a");
    		String type = request.getParameter("type");
    		PanelTabsList pt = session.getTabsForPanel("user",admin);
    		request.setAttribute("admin",admin);
    		request.setAttribute("pt", pt);
    		if (type.equalsIgnoreCase("getemail"))
    		{
    			int notid = ServletUtils.getIntParameter(request,"notid");
    			Notify notify = session.getNotify(notid,false);
    			ProviderDefinition pd = notify.getDef();
    			System.out.println("ProviderDefinition="+pd.getName());
    			JSONObject jo = new JSONObject();
    			PrintWriter out = response.getWriter();
    			User me = session.getUser(session.GetUserID());
    			jo.add("provider",pd.getName());
    			jo.add("email",me.getEmail());
    			jo.add("number",me.getPhone());
    			out.println(jo.toString());
    		} else {
	    		int uid = ServletUtils.getIntParameter(request,"uid");
	
	    		User user = session.getUser(uid);
	    		UserGroupList gl1 = session.getGroupsForUser(uid);
	    		UserGroupList gl2 = session.getGroupsNotForUser(uid);
	
	    		if (type.equalsIgnoreCase("Groups4User"))
	    		{
	    			JSONObject jo = new JSONObject();
	    			boolean readonly = false;
	    			System.out.println("uid="+uid+" session.GetUserID()="+session.GetUserID());
	    			if (uid == session.GetUserID() && !session.getAclOverride()) readonly=true;	// Cannot edit your own group membership
	    			if (!user.isUpdatable()) readonly=true;	// Edit based on domain edit permission
	    			jo.add("readonly", readonly);
	    			JSONArray js = new JSONArray();
	    			if (gl1 != null) {
	    				for (int i=0;i<gl1.size();i++) {
	    					JSONObject o = new JSONObject();
	    					o.add("id", gl1.get(i).getId());
	    					o.add("name", gl1.get(i).getDomain().getFullDomain() + "." + gl1.get(i).getName());
	    					js.add(o);
	    				}
	    			}
	    			PrintWriter out = response.getWriter();
	    			jo.add("groups",js);
	    			out.println(jo.toString());
	    		}
	    		else if (type.equalsIgnoreCase("AvailGroups4User"))
	    		{
	    			JSONObject jo = new JSONObject();
	    			JSONArray js = new JSONArray();
	    			if (gl2 != null) {
	    				for (int i=0;i<gl2.size();i++) {
	    					JSONObject o = new JSONObject();
	    					o.add("id", gl2.get(i).getId());
	    					o.add("name", gl2.get(i).getDomain().getFullDomain() + "." +  gl2.get(i).getName());
	    					js.add(o);
	    				}
	    			}
	    			PrintWriter out = response.getWriter();
	    			jo.add("oac",session.getAclOverride());
	    			jo.add("groups",js);
	    			out.println(jo.toString());
	    		}
    		}
    	} else {
    		// POST - update
    		response.setContentType("application/json");
    		String func = request.getParameter("f");
    		int uid = ServletUtils.getIntParameter(request,"id");
    		int gid = ServletUtils.getIntParameter(request, "gid");
    		PrintWriter out = response.getWriter();
    		System.out.println("In GroupDetails post func="+func);
    		int res=0;
    		if (func.equalsIgnoreCase("a"))
    		{
    			// adding a user to a group
    			res = session.AddUserToGroup(gid,uid);
    			
    		}
    		else
    		if (func.equalsIgnoreCase("r"))
    		{
    			// removing a user from a group
    			res = session.RemoveUserFromGroup(gid,uid);
    		}
    		String errtext;
    		errtext = "";
    		if (res>0) errtext="Failed to update database";
    		out.print("{\"errtext\" : \"" + errtext + "\", \"errcode\" : \"" + res + "\"}");
    	}
   	}
}
