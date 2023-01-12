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
