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
import java.net.URLEncoder;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.JSONObject;
import dmadmin.model.DMObject;

/**
 * Servlet implementation class UpdateSummaryData
 */
public class UpdateSummaryData
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UpdateSummaryData() {
        super();
    }
    
    @Override
   	public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	int ot = ServletUtils.getIntParameter(request, "objtype");
		int id = ServletUtils.getIntParameter(request, "id");		
		ObjectType objtype = ObjectType.fromInt(ot);
		if(objtype == null) {
			throw new RuntimeException("Invalid object type " + ot);
		}
		
		System.out.println("In UpdateSummaryData");
  session.setPassword(request); 
		
		if (id < 0 && objtype == ObjectType.COMPONENTITEM)
		{
		 int compid = new Integer(request.getParameter("compid")).intValue();
		 String comptype = request.getParameter("comptype");
		  
 	 id = session.componentItemNewItem(compid, 100, 100, comptype);
		 session.setComponentItemKind(id,comptype);
		}
		
		DMObject dmobj = session.getDetailedObject(objtype, id);
		
		SummaryChangeSet changes = new SummaryChangeSet();
		
		String errtext = null;
		
		for(Object oparam : request.getParameterMap().keySet()) {
			String param = (String) oparam;
			if(param.startsWith("change_") && !param.equalsIgnoreCase("change_undefined")) {
				String value = request.getParameter(param);
				System.out.println("'" + param + "' = '" + value + "'");
				try {
					int sf = Integer.parseInt(param.substring(7));
					SummaryField field = SummaryField.fromInt(sf);
					if(field != null) {
						errtext = session.processField(dmobj, field, value, changes);
						if (errtext != null) {
							System.err.println(errtext);
							break;
						}
					} else {
						errtext = "ERROR: Unknown summary field " + sf;
						System.err.println(errtext);
						break;
					}
				} catch(NumberFormatException e) {
					e.printStackTrace();
				}
			}
		}
		
		System.out.println("no exception thrown yet");
		
		response.setContentType("application/json");
		PrintWriter out = response.getWriter();
		JSONObject obj = new JSONObject();
		
		System.out.println("errtext=["+errtext+"]");
		
		if (errtext != null) {
			obj.add("saved",false);
			obj.add("error",errtext);
		} else {
			try {
				if(dmobj.isUpdatable()) {
					boolean res = dmobj.updateSummary(changes);
					if (res && dmobj.getObjectType() == ObjectType.USER) {
						// Successful save of a user. Have we changed our own password?
						if (dmobj.getId() == session.GetUserID()) {
							String ep = URLEncoder.encode(session.getPassword(),"UTF-8");
							// p2 is stored unencoded as it is encoded when used
							// logindata has encoded password since it is used verbatim.
//							Cookie p2cookie = new Cookie("p2",session.getPassword());
//							Cookie ldcookie = new Cookie("logindata","domain=&username="+session.GetUserName()+"&password="+ep+"&newpassword=&newpasswordagain=");
//							
//							p2cookie.setDomain("deployhub.com"); p2cookie.setPath("/");
//							ldcookie.setDomain("deployhub.com"); ldcookie.setPath("/");
//							
//							response.addCookie(p2cookie);
//							response.addCookie(ldcookie);
							System.out.println("Saved change to self, recoded password cookies");
						}
					}
					obj.add("saved", res);
		   obj.add("id", dmobj.getId());
				} else {
					obj.add("saved", false);
					obj.add("error", "You do not have permission to update this object");				
				}
			} catch(Exception e) {
				System.out.println("exception thrown: "+e.getMessage());
				e.printStackTrace();
				obj.add("saved", false);
				obj.add("error", e.getMessage());
			}
		}
		
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
   	}
}
