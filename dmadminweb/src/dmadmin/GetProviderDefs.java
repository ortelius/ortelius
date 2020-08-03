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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.model.Datasource;
import dmadmin.model.ProviderDefinition;

/**
 * Servlet implementation class GetProviderDefs
 */
public class GetProviderDefs
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetProviderDefs() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		JSONArray ret = new JSONArray();
		String fn = request.getParameter("f");
		System.out.println("GetProviderDefs, fn="+fn);
		if (fn != null) {
			if (fn.equalsIgnoreCase("ldap")) {
				// requesting a list of data sources of type LDAP
				int id = getIntParameter(request,"id");
				System.out.println("ldap id="+id);
				List<Datasource> ds = session.getLDAPDataSources(id);
				for (Datasource d: ds) {
					ret.add(d.getLinkJSON());
				}
			}
			else
			if (fn.equals("bt")) {
				// requesting a list of data sources of type Bug Tracker
				int id = getIntParameter(request,"id");
				System.out.println("bugtracking id="+id);
				List<Datasource> ds = session.getBugTrackerDataSources(id);
				for (Datasource d: ds) {
					ret.add(d.getLinkJSON());
				}
			}
		} else {
			int ot = getIntParameter(request, "objtype");
			ObjectType objtype = ObjectType.fromInt(ot);
			if(objtype == null) {
				throw new RuntimeException("Invalid object type");
			}
			List<ProviderDefinition> defs = session.getProviderDefinitionsOfType(objtype);
			
			
			for(ProviderDefinition pd : defs) {
				ret.add(pd.getLinkJSON());
			}
		}
		return ret;
	}
}
