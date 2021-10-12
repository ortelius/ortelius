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
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Action;
import dmadmin.model.DMObject;
import dmadmin.model.ObjectAccess;
import dmadmin.model.UserGroup;

/**
 * Servlet implementation class GetAccess
 */
public class GetAccess extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetAccess() {
        super();
        // TODO Auto-generated constructor stub
    }
    
	public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
		int ot = ServletUtils.getIntParameter(request, "objtype");
		int id = ServletUtils.getIntParameter(request, "id");
		String din = request.getParameter("din");	// domain inheritance
		ObjectType objtype = ObjectType.fromInt(ot);
		if(objtype == null) {
			throw new RuntimeException("Invalid object type " + ot);
		}
		
		if (!isPost) {
			// GET - set session objects and redirect to JSP
			DMObject obj = session.getObject(objtype, id);
			boolean updatable=obj.isUpdatable();
			switch(objtype) {
			case ACTION:
			case PROCEDURE:
			case FUNCTION: {
				Action action = session.getAction(id, false);
				if (action.getParentId()>0) updatable=false;	// archived actions cannot be edited
			}
				break;
			default:
				break;
			}
			String agid;
			String vaid;
			String uaid;
			String raid;
			String waid;
			
			
			Hashtable<Integer, ObjectAccess> acl;
			int domainid = (objtype == ObjectType.DOMAIN)?id:obj.getDomainId();
			
			String readtitle=null;
			String writetitle=null;
			
			if (din != null && din.equalsIgnoreCase("y")) {
				// Domain Inheritance
				System.out.println("getAccessForDomain "+obj.getId());
				acl = session.getAccessForDomain(obj.getId());
				agid="xg";
				vaid="vx";
				uaid="ux";
				raid="rx";
				waid="wx";
				readtitle="Read Access";
				writetitle="Write Access";
			} else {
				acl = session.getAccessForObject(obj, true);
				agid="ag";
				vaid="va";
				uaid="ua";
				raid="ra";
				waid="wa";
				if (obj.hasReadWrite()) {
					readtitle=obj.getReadTitle();
					writetitle=obj.getWriteTitle();
				}
			}
		
			// TODO: Check that we have access to the object
			
			List<UserGroup> grouplist = session.getGroupsForDomain(domainid);
			List<UserGroup> viewlist = new ArrayList<UserGroup>();
			List<UserGroup> updatelist = new ArrayList<UserGroup>();
			List<UserGroup> readlist = new ArrayList<UserGroup>();
			List<UserGroup> writelist = new ArrayList<UserGroup>();
			
			List<UserGroup> inviewlist = new ArrayList<UserGroup>();
			List<UserGroup> inupdatelist = new ArrayList<UserGroup>();
			List<UserGroup> inreadlist = new ArrayList<UserGroup>();
			List<UserGroup> inwritelist = new ArrayList<UserGroup>();
			
			List<UserGroup> denyviewlist = new ArrayList<UserGroup>();
			List<UserGroup> denyupdatelist = new ArrayList<UserGroup>();
			List<UserGroup> denyreadlist = new ArrayList<UserGroup>();
			List<UserGroup> denywritelist = new ArrayList<UserGroup>();
			
			Enumeration<Integer> enumKey = acl.keys();
			while (enumKey.hasMoreElements()) {
			    Integer groupid = enumKey.nextElement();
			    ObjectAccess oa = acl.get(groupid);
			    UserGroup group = session.getGroup(groupid);
			    if(group == null) {
			    	continue;
			    }
			    
			    // Update Access
			    if (oa.isUpdatable()) {
			    	if (oa.isUpdateInherited()) {
			    		inupdatelist.add(group);
			    	} else {
			    		updatelist.add(group);
			    	}
			    }
			    if (oa.isUpdateDenied() && !oa.isUpdateInherited()) {
			    	inupdatelist.add(group);
			    	denyupdatelist.add(group);
			    }
			    
			    // View Access
			    if (oa.isViewable()) {
			    	if (oa.isViewInherited()) {
			    		inviewlist.add(group);
			    	} else {
			    		viewlist.add(group);
			    	}
			    } 
		    	if (oa.isViewDenied() && !oa.isViewInherited()) {
		    		inviewlist.add(group);
		    		denyviewlist.add(group);
		    	}
		    	
		    	// Write Access
			    if (oa.isWriteable()) {
			    	if (oa.isWriteInherited()) {
			    		inwritelist.add(group);
			    	} else {
			    		writelist.add(group);
			    	}
			    }
			    if (oa.isWriteDenied() && !oa.isWriteInherited()) {
			    	System.out.println("oa.isWriteDenied="+oa.isWriteDenied());
			    	System.out.println("oa.isWriteInherited"+oa.isWriteInherited());
			    	System.out.println("adding "+group.getName()+" to denywritelist");
			    	inwritelist.add(group);
			    	denywritelist.add(group);
			    }
			    
			    // Read Access
			    if (oa.isReadable()) {
			    	if (oa.isReadInherited()) {
			    		inreadlist.add(group);
			    	} else {
			    		readlist.add(group);
			    	}
			    }
			    if (oa.isReadDenied() && !oa.isReadInherited()) {
			    	inreadlist.add(group);
			   		denyreadlist.add(group);
			   	}
			}
						
			request.setAttribute("obj", obj);
			request.setAttribute("grouplist",grouplist);
			request.setAttribute("viewlist",viewlist);
			request.setAttribute("updatelist",updatelist);
			request.setAttribute("readlist",readlist);
			request.setAttribute("writelist",writelist);
			request.setAttribute("inviewlist",inviewlist);
			request.setAttribute("inupdatelist",inupdatelist);
			request.setAttribute("inreadlist",inreadlist);
			request.setAttribute("inwritelist",inwritelist);
			request.setAttribute("denyviewlist",denyviewlist);
			request.setAttribute("denyupdatelist",denyupdatelist);
			request.setAttribute("denyreadlist",denyreadlist);
			request.setAttribute("denywritelist",denywritelist);
			request.setAttribute("readtitle",readtitle);	
			request.setAttribute("writetitle",writetitle);
			request.setAttribute("din",din);
			request.setAttribute("agid",agid);
			request.setAttribute("vaid",vaid);
			request.setAttribute("uaid",uaid);
			request.setAttribute("raid",raid);
			request.setAttribute("waid",waid);
			request.setAttribute("em",updatable);
	        request.getRequestDispatcher("/WEB-INF/GetAccess.jsp").forward(request, response);
			
		} else {
			// POST - Ajax update call from JSP
			response.setContentType("application/json");
			
			PrintWriter out = response.getWriter();
			
			String f = request.getParameter("f");
			int gid = ServletUtils.getIntParameter(request, "gid");
			String st = request.getParameter("st");
			boolean DomainInheritance=false;
			if (din != null) {
				if (din.equalsIgnoreCase("y")) DomainInheritance=true;
			}
			String t = request.getParameter("t");
			DMObject obj = session.getObject(objtype,id);
			//
			// t is one of:
			// "va" (View Access) or "ua" (Update Access) (for the admin interface)
			//
			// "ra" (Read Access), "wa" (Write Access) (for the deployment engine) 
			//
			if (f.equalsIgnoreCase("ag")) {
				boolean added;
				// Adding a group
				added = session.AddAccess(obj,t,gid,st,DomainInheritance);
				
				System.out.println("added returned "+(added?"true":"false"));
				out.println("{\"added\": \"" + (added?"Y":"N") + "\"}");
			}
			else
			if (f.equalsIgnoreCase("rg")) {
				// Removing a group
				session.RemoveAccess(obj,t,gid,DomainInheritance);
				out.println("{\"success\": \"Y\"}");
			}
			else
			if (f.equalsIgnoreCase("rd")) {
				// Removing a deny group
				session.RemoveDenyAccess(obj,t,gid,DomainInheritance);
				out.println("{\"success\": \"Y\"}");
			}
		}
		
   	}
}
