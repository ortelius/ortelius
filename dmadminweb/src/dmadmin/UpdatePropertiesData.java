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

import dmadmin.json.JSONObject;
import dmadmin.model.DMProperty;
import dmadmin.model.ProviderObject;

/**
 * Servlet implementation class UpdatePropertiesData
 */
public class UpdatePropertiesData
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UpdatePropertiesData() {
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
		
		response.setContentType("application/json");
		PrintWriter out = response.getWriter();
		JSONObject obj = new JSONObject();
		

		try {
			ACDChangeSet<DMProperty> changes = new ACDChangeSet<DMProperty>();
			
			for(Object oparam : request.getParameterMap().keySet()) {
				String param = (String) oparam;
				if(param.startsWith("chg_")) {
					String prop = param.substring(4);
					String pval = request.getParameter(param);
					System.out.println("chg '" + prop + "' = '" + pval + "'");
					changes.addChanged(processProperty(prop, pval));
				} else if(param.startsWith("add_")) {
					String prop = param.substring(4);
					String pval = request.getParameter(param);
					System.out.println("add '" + prop + "' = '" + pval + "'");
					changes.addAdded(processProperty(prop, pval));
				} else if(param.startsWith("delete_")) {
					String prop = param.substring(7);
					String pval = request.getParameter(param);
					System.out.println("delete '" + prop + "' = '" + pval + "'");
					changes.addDeleted(processProperty(prop, pval));
				}
			}
			
			ProviderObject po = session.getProviderObject(objtype, id, false);
			if(po.isUpdatable()) {
				boolean res = po.updateProperties(changes);
				obj.add("saved", res);
			} else {
				obj.add("saved", false);
				obj.add("error", "You do not have permission to update this object");				
			}
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("saved", false);
			obj.add("error", e.getMessage());
		}
		
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
	}

	private DMProperty processProperty(String prop, String pval)
	{
		if(pval.length() < 3) {
			pval="NNN";
		}
		char encr = pval.charAt(0);
		char over = pval.charAt(1);
		char apnd = pval.charAt(2);
		String value = pval.substring(3);
		return new DMProperty(prop, value, (encr == 'Y'), (over == 'Y'), (apnd == 'Y'));
	}
}
