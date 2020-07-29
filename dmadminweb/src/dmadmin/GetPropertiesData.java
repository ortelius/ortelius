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
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.DMProperty;
import dmadmin.model.DMPropertyDef;
import dmadmin.model.ProviderObject;


/**
 * Servlet implementation class GetPropertiesData
 */
public class GetPropertiesData
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetPropertiesData() {
        super();
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json");

		PrintWriter out = response.getWriter();
		int ot = ServletUtils.getIntParameter(request, "objtype");
		int id = ServletUtils.getIntParameter(request, "id");
		
		String deftype = request.getParameter("deftype");
		
		if (deftype != null)
		 id = session.getDefTypeId(deftype);
		
		String defonly =request.getParameter("defonly");
		 
		ObjectType objtype = ObjectType.fromInt(ot);
		if(objtype == null) {
			throw new RuntimeException("Invalid object type " + ot);
		}

  JSONObject obj = new JSONObject();

		if (defonly != null)
		{
	  JSONArray arr = new JSONArray();
	  obj.add("defs", arr);
	  obj.add("data", new JSONArray());
	  
	  // Lookup table of defs
	  Hashtable<String, DMPropertyDef> defs = new Hashtable<String, DMPropertyDef>();
	  List<DMPropertyDef> deflist = session.getPropertyDefs(id);
	  
	  ArrayList<DMPropertyDef> ordered_deflist = new ArrayList<DMPropertyDef>();
	  if (deftype.equalsIgnoreCase("ldap"))
	  {
	   for(DMPropertyDef d : deflist) 
	   {
	    if (d.getName().equalsIgnoreCase("LDAP Server"))
	     ordered_deflist.add(0,d);
	    else
	     ordered_deflist.add(d);
	   }
	   deflist = ordered_deflist;
	  }
	  
	  for(DMPropertyDef d : deflist) {
	   
	   if ((deftype.equalsIgnoreCase("odbc") || deftype.equalsIgnoreCase("smtpemail") || deftype.equalsIgnoreCase("txtlocal")) && (d.getName().equalsIgnoreCase("username") || d.getName().equalsIgnoreCase("password")))
	    continue;

	   defs.put(d.getName(), d);
	   arr.add(d);
	  }		 
		}
		else
		{
		 ProviderObject po = session.getProviderObject(objtype, id, true);
		 boolean readOnly = !po.isUpdatable();
		 deftype = po.getDef().getName();
		
	 	obj.add("readOnly", readOnly);
	
 		JSONArray arr = new JSONArray();
 		obj.add("defs", arr);
		
 		// Lookup table of defs
 		Hashtable<String, DMPropertyDef> defs = new Hashtable<String, DMPropertyDef>();
 		List<DMPropertyDef> deflist = po.getPropertyDefs();
 		for(DMPropertyDef d : deflist) {
    if ((deftype.equalsIgnoreCase("odbc") || deftype.equalsIgnoreCase("smtpemail") || deftype.equalsIgnoreCase("txtlocal")) && (d.getName().equalsIgnoreCase("username") || d.getName().equalsIgnoreCase("password")))
     continue;
    
  			defs.put(d.getName(), d);
 			arr.add(d);
 		}
		
	 	// Lookup table of required properties
	 	Hashtable<String, Integer> reqdprops = new Hashtable<String, Integer>();
			
	 	JSONArray arr2 = new JSONArray();
 		obj.add("data", arr2);
		
 		// Add the required properties first
 		for(DMProperty p: po.getProperties()) {
 			DMPropertyDef def = defs.get(p.getName());
 			if((def != null) && def.isRequired()) {
 				arr2.add(p);
	 			reqdprops.put(p.getName(), 1);
	 		}
	 	}
		
		// Add any missing required properties
//		for(DMPropertyDef d : deflist) {
//			if(d.isRequired() && (reqdprops.get(d.getName()) == null)) {
//				arr2.add(new DMProperty(d.getName(), "", false, false, false, true));
//			}
//		}	
		
		 // Add the optional properties
		 for(DMProperty p: po.getProperties()) {
		 	DMPropertyDef def = defs.get(p.getName());
		 	if((def == null) || !def.isRequired()) {
		 		arr2.add(p);
		 	}
		 }
		}	
		String ret = obj.toString();
		
		out.println(ret);
		System.out.println(ret);		
   	}
}
