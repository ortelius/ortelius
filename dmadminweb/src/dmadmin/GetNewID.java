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

import dmadmin.json.JSONObject;
import dmadmin.model.Action;
import dmadmin.model.ActionKind;
import dmadmin.model.Category;
import dmadmin.model.DMObject;
import dmadmin.model.Domain;

/**
 * Servlet implementation class GetNewID
 */
public class GetNewID extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetNewID() {
        super();
        // TODO Auto-generated constructor stub
    }

    @SuppressWarnings("unused")
	@Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	System.out.println("In GetNewID");
    	response.setContentType("application/json;charset=UTF-8");
		PrintWriter out = response.getWriter();
		String objtype = request.getParameter("objtype");

		System.out.println("objtype=["+objtype+"]");

		int xpos = ServletUtils.getIntParameter(request, "xpos", -1);
		int ypos = ServletUtils.getIntParameter(request, "ypos", -1);
		String catstring = request.getParameter("catid");
		String kindstring = request.getParameter("kindid");
		int catid = (catstring != null)?Integer.parseInt(catstring):0;
		int kindid = (kindstring != null)?Integer.parseInt(kindstring):0;

		String rt = objtype;
		int parid = ServletUtils.getIntParameter(request, "pid");
		String ptype = request.getParameter("ptype");
		int newid;
		String newname = request.getParameter("name");
		String treeid = request.getParameter("treeid");
		int domainid=0;

		if (ptype.equalsIgnoreCase("domain") || ptype.equalsIgnoreCase("lifecycle"))
		{
			// If we are creating a sub-domain in a lifecycle, display a lifecycle node
			domainid = parid;
			Domain pardomain = session.getDomain(domainid);
			if ((pardomain!=null) && pardomain.getLifecycle() && objtype.equalsIgnoreCase("domain")) rt="Lifecycle";
		}
		else
		{
			//
			// Get the domain of the parent id
			//
			System.out.println("Getting Domain For Object ptype="+ptype+" parid="+parid);
			domainid = session.GetDomainForObject(ptype,parid);
		}
		System.out.println("getting newid");
		newid = session.getID(objtype);

		System.out.println("got newid="+newid);

		if (newname == null || newname.length() == 0 || newname.equalsIgnoreCase("undefined"))
		 newname=objtype+newid;

		ObjectTypeAndId otid = null;
		String id = null;
		JSONObject obj = null;

		if (objtype.equalsIgnoreCase("action"))
		{
			String actiontype=request.getParameter("at");
			if (actiontype.equalsIgnoreCase("F")) {
				rt = "FUNCTION";
			} else if (actiontype.equalsIgnoreCase("P")) {
				rt = "PROCEDURE";
			}

			if (newname.indexOf(' ')>=0) {
				obj = new JSONObject().add("result", false).add("error", rt.substring(0,1)+rt.substring(1).toLowerCase()+" name cannot include spaces");
				out.println(obj.getJSON());
				System.out.println(obj.getJSON());
				session.rollback();
				return;
			}
			String testname = newname.replaceAll("[0-9A-Za-z_]","");
			if (testname.length()>0) {
				obj = new JSONObject().add("result", false).add("error", rt.substring(0,1)+rt.substring(1).toLowerCase()+" name can only include alpha-numeric characters or underscores");
				out.println(obj.getJSON());
				System.out.println(obj.getJSON());
				session.rollback();
				return;
			}
			if (newname.charAt(0)>='0' && newname.charAt(0)<='9') {
				obj = new JSONObject().add("result", false).add("error", rt.substring(0,1)+rt.substring(1).toLowerCase()+" name cannot start with a digit");
				out.println(obj.getJSON());
				System.out.println(obj.getJSON());
				session.rollback();
				return;
			}
			try {
				otid = session.CreateNewAction(actiontype,newname,domainid,parid,newid);
				session.addToCategory(catid,otid,true);	// action_category
			} catch(RuntimeException ex) {
				 obj = new JSONObject().add("result", false).add("error", ex.getMessage());
				 out.println(obj.getJSON());
				 System.out.println(obj.getJSON());
				 session.rollback();
				 return;
			}
			System.out.println("GetNewID, action created - otid="+otid.toString());
			id = otid.toString();
			if (kindid>0) id=id+"-"+kindid;
			System.out.println("id="+id);
		}
		else
		{
			System.out.println("Not an action");
		 String msg = null; // session.CheckLicense(objtype,parid);
		 if (msg == null)
		 {
			 try {
				 System.out.println("Calling CreateNewObject");
				 otid = session.CreateNewObject(objtype,newname,domainid,parid,newid,xpos,ypos,treeid);
				 id = otid.toString();
				 System.out.println("Done that, id="+id);
				 if (id.substring(0,2).equalsIgnoreCase("co") || id.substring(0,2).equalsIgnoreCase("cv")) {
					 // Created a Component or Component Version - add to initial category
					 session.addToCategory(catid,otid,true);
				 }
			 } catch(RuntimeException ex) {
				 obj = new JSONObject().add("result", false).add("error", ex.getMessage());
				 out.println(obj.getJSON());
				 System.out.println(obj.getJSON());
				 session.rollback();
				 return;
			 }
		 }
		 else
		 {
		   obj = new JSONObject().add("result", false).add("error", msg);
		   out.println(obj.getJSON());
		   System.out.println(obj.getJSON());
		   session.rollback();
		   return;
		 }
		}

		System.out.println("here rt="+rt);

		if (rt.equalsIgnoreCase("type"))
		 rt = "SERVERCOMPTYPE";

		if(otid != null) {
		 DMObject dmobj = session.getObject(otid.getObjectType(), otid.getId());

		 if (dmobj instanceof Domain)
		 {
		  String taskname = "Deploy Version to an Environment";
    int tid = session.getID("task");
		  session.CreateNewTask(taskname,"Deploy",dmobj.getId(),tid);
		 }

			obj = new JSONObject().add("result", true).add("name", newname).add("id", id).add("rt", rt);
			// Components (and Component Versions), Actions, Procedures and Functions are in
			// Categories. Need to send the category back to the client so that it can display it in
			// the tree properly.
			System.out.println("Okay initial pass okay - substring is ["+id.substring(0,2)+"]");
			System.out.println("catid is "+catid);
			if (id.substring(0,2).equalsIgnoreCase("co") || id.substring(0,2).equalsIgnoreCase("cv")) {
				System.out.println("Finding component/component version");
				if (catid != 0) {
					Category cat = session.getCategory(catid);
					obj.add("category","cc"+catid+"-"+domainid)
					.add("newcat",!session.CategoryInDomain(newid,catid,domainid,1))
					.add("catname",cat.getName());
				}
			}
			else
			if (id.substring(0,2).equalsIgnoreCase("ac") || id.substring(0,2).equalsIgnoreCase("pr") || id.substring(0,2).equalsIgnoreCase("fn")) {
				Action action = session.getAction(newid,true);
				if (action.getKind() == ActionKind.GRAPHICAL) {
					if (catid != 0) {
						Category cat = session.getCategory(catid);
						obj.add("category","cy"+cat.getId()+"-"+domainid)
						.add("newcat",!session.CategoryInDomain(newid,cat.getId(),domainid,2))
						.add("catname",cat.getName());
					}
				} else {
					if (catid != 0) {
						Category cat = session.getCategory(catid);
						obj.add("category","cp"+cat.getId()+"-"+domainid)
						.add("newcat",!session.CategoryInDomain(newid,cat.getId(),domainid,3))
						.add("catname",cat.getName());
					}
				}
			}
		} else {
			obj = new JSONObject().add("result", false).add("error", "Failed to create object");
		}
		out.println(obj.getJSON());
		System.out.println(obj.getJSON());

		//out.print("{\"name\" : \"" + newname + "\", \"id\" : \"" + newid + "\", \"rt\" : \""+rt+"\"}");
   	}
}
