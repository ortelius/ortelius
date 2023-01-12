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
import javax.servlet.http.HttpSession;

import dmadmin.model.Action;
import dmadmin.model.Category;
import dmadmin.model.Component;
import dmadmin.model.DMObject;

/**
 * Servlet implementation class CopyAndPaste
 */
public class CopyAndPaste extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public CopyAndPaste() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("application/json;charset=UTF-8");

		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		String mode = request.getParameter("f");
		if (mode.equalsIgnoreCase("c")) {
			// Copy Mode
			String objtype = request.getParameter("t");
//			int domain = ServletUtils.getIntParameter(request, "d");
			int id = ServletUtils.getIntParameter(request, "id");
			System.out.println("Copy) objtype="+objtype+" id="+id);
			so.CopyObject(objtype,id);
			out.print("{\"status\" : \"ok\"}");
		}
		else if (mode.equalsIgnoreCase("p")) {
			// Paste Mode
			String rt="";
			int domain = ServletUtils.getIntParameter(request, "d");
			String pie = request.getParameter("pie");
			boolean PastingToEnvironment=(pie != null && pie.equalsIgnoreCase("y"));
			System.out.println("id="+request.getParameter("id"));
			String sid = request.getParameter("id").replaceAll("-.*$", "");
			int inobject = Integer.parseInt(sid);
			int xpos = request.getParameter("xpos")!=null?ServletUtils.getIntParameter(request, "xpos"):-1;
			int ypos = request.getParameter("ypos")!=null?ServletUtils.getIntParameter(request, "ypos"):-1;
			System.out.println("Paste) objtype="+so.getPasteObjectType()+" pasteobjectid="+so.getPasteObjectId()+" inobject="+inobject);
			DMObject obj = so.PasteObject(domain,inobject,xpos,ypos,PastingToEnvironment,true);
			int kind=0;
			String catprefix="";
			if (obj == null) {
				// Couldn't paste object - return error
				System.out.println("{\"error\" : \"" + so.GetPasteError() + "\", objtype: \""+so.getPasteObjectType()+"\"}");
				out.print("{\"error\" : \"" + so.GetPasteError() + "\", \"objtype\" : \""+so.getPasteObjectType()+"\"}");
			} else {
				// Do we need to put this object into a category node?
				Category cat = null;
				int t=0;
				switch(obj.getObjectType()) {
				case ENVIRONMENT:
					rt="ENVIRONMENT";
					break;
				case SERVER:
					rt="SERVER";
					break;
				case TEMPLATE:
					rt="TEMPLATE";
					break;
				case ACTION:
					rt="ACTION";
					cat = ((Action)obj).getCategory();
					t=2;
					catprefix="cy";
					break;
				case ACTION_CATEGORY:
					break;
				case ACTION_KIND:
					break;
			    case ACTION_REPO:
			    	break;
			    case APPLICATION:
			    	rt="APPLICATION";
			    	break;
			    case APPROVAL:
			    	break;
			    case APPVERSION:
			    	rt="APPVERSION";
			    	break;
			    case ATTACHMENT_LIST:
			    	break;
			    case BOOLEAN:
			    	break;
			    case CALENDAR_ENTRY:
			    	break;
			    case COMPONENT:
					Component comp = so.getComponent(obj.getId(), true);
					cat = comp.getCategory();
					catprefix="cc";
					System.out.println("2) cat is "+cat.getName());
					t=1;
			    	break;
			    case COMPONENTITEM:
			    	break;
			    case COMPONENT_FILTER:
			    	break;
			    case COMPVERSION:
			    	Component compv = so.getComponent(obj.getId(), true);
					cat = compv.getCategory();
					catprefix="cc";
					System.out.println("2) cat is "+cat.getName());
					t=1;
			    	break;
			    case CREDENTIALS:
			    	break;
			    case CREDENTIAL_KIND:
			    	break;
			    case DATASOURCE:
			    	break;
			    case DATASOURCE_IMPL:
			    	break;
			    case DEPLOYMENT:
			    	break;
			    case DOMAIN:
			    	break;
			    case DROPZONE:
			    	break;
			    case DROPZONEFILE:
			    	break;
			    case ENCRYPTED:
			    	break;
			    case ENGINE:
			    	break;
			    case FILTER_ITEMS:
			    	break;
			    case FUNCTION: {
			    		System.out.println("Pasting FUNCTION");
				    	rt="FUNCTION";
				    	Action action=(Action) obj;
				    	kind=action.getKind().value();
				    	cat=action.getCategory();
				    	System.out.println("cat is "+cat);
				    	t=3;
				    	catprefix="cp";
				    }
			    	break;
			    case HISTORY_NOTE:
			    	break;
			    case MODIFIED:
			    	break;
			    case NOTIFY:
			    	break;
			    case NOTIFY_IMPL:
			    	break;
			    case OWNER:
			    	break;
			    case PASS_HASH:
			    	break;
			    case PLUGIN:
			    	break;
			    case PLUGIN_OBJECT:
			    	break;
			    case PROCEDURE: {
			    		System.out.println("Pasting PROCEDURE");
			    		rt="PROCEDURE";
			    		Action action=(Action) obj;
			    		kind=action.getKind().value();
			    		cat=action.getCategory();
			    		System.out.println("cat is "+cat);
			    		t=3;
			    		catprefix="cp";
			    	}
			    	break;
			    case PROVIDERDEF:
			    	break;
			    case RELEASE:
			    	break;
			    case RELVERSION:
			    	break;
			    case REPOSITORY:
			    	break;
			    case REQUEST:
			    	break;
			    case RESPOSITORY_IMPL:
			    	break;
			    case SERVER_TYPE:
			    	break;
			    case STATEMENT_IMPL:
			    	break;
			    case TASK:
			    	break;
			    case TRANSFER:
			    	break;
			    case USER:
			    	break;
			    case USERGROUP:
			    	break;
			    default:
			    	break;
				}
				String catname="";
				String catid="";
				boolean newcat=false;
				if (cat != null) {
					catname=cat.getName();
					catid=catprefix+cat.getId()+"-"+domain;
					System.out.println("category is "+cat.getName()+" catid="+catid);
					newcat = !so.CategoryInDomain(obj.getId(), cat.getId(), domain, t);
				}

				if (kind>0) {
					out.print("{\"error\" : \"\", \"name\" : \"" + obj.getName() + "\", \"id\" : \"" + obj.getOtid() + "-"+kind+"\", \"rt\" : \""+rt+"\", \"category\": \""+catid+"\", \"catname\": \""+catname+"\", \"newcat\": "+newcat+"}");
				} else {
					out.print("{\"error\" : \"\", \"name\" : \"" + obj.getName() + "\", \"id\" : \"" + obj.getOtid() + "\", \"rt\" : \""+rt+"\", \"category\": \""+catid+"\", \"catname\": \""+catname+"\", \"newcat\": "+newcat+"}");
				}
			}
		}
  }
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
