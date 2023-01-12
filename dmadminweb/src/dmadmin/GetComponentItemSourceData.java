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
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONBoolean;
import dmadmin.json.JSONObject;
import dmadmin.json.LinkField;
import dmadmin.model.ComponentItem;
import dmadmin.model.DMProperty;
import dmadmin.model.DMPropertyDef;
import dmadmin.model.ProviderObject;
import dmadmin.model.Repository;

/**
 * Servlet implementation class GetComponentItemSourceData
 */
public class GetComponentItemSourceData
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetComponentItemSourceData() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int ciid = getIntParameter(request, "ciid");

		System.out.println("ciid="+ciid);

		String reason = request.getParameter("reason");
		if(reason != null) {
			if(reason.equalsIgnoreCase("repos")) {
				System.out.println("Calling getRepositories");
				return getRepositories(session,ciid);
			} else if(reason.equalsIgnoreCase("defs")) {
				int repoid = getIntParameter(request, "repid");
				return getDefsForRepository(session, repoid);
			}
			throw new RuntimeException("Bad reason '" + reason + "'");
		}

  ComponentItem ci = session.getComponentItem(ciid, true);

		ProviderObject po = ci.getRepository();
		boolean readOnly = !ci.isUpdatable(); 	// was !po.isUpdatable(), which is wrong and give an NPE if repo not set

		JSONObject ret = new JSONObject();
		ret.add("readOnly", readOnly);

		JSONArray arr = new JSONArray();
		ret.add("defs", arr);

		// Can only work out provider properties and defs if repository is set
		Hashtable<String, DMProperty> repoprops = new Hashtable<String, DMProperty>();
		if(po != null) {
			// Lookup table of properties defined on the Repository itself
			for(DMProperty p: po.getProperties()) {
				repoprops.put(p.getName(), p);
			}

			// Filter the defs
			for(DMPropertyDef d : po.getPropertyDefs()) {
				// Only pass the defs that are not defined or can be overridden or appended
				DMProperty rp = repoprops.get(d.getName());
				if((rp == null) || rp.isAppendable() || rp.isOverridable()) {
					arr.add(d);
				}
			}
		}

		JSONArray arr2 = new JSONArray();
		ret.add("data", arr2);

		// Add the repository as the first property
		arr2.add(new JSONArray().add(new JSONBoolean(false))	// dirty
				.add(new JSONBoolean(false))					// added
				.add(new JSONBoolean(false))					// deleted
				.add("Repository")								// name
				.add((po != null) ? ((LinkField) po.getLinkJSON()).setShowLink(false) : null)
				.add(false)										// encrypted
				.add(false)										// overridable
				.add(false));									// appendable

		// Add all the properties that are not defined or can be overridden or appended
		for(DMProperty p: ci.getProperties()) {
			DMProperty rp = repoprops.get(p.getName());
			if((rp == null) || rp.isAppendable() || rp.isOverridable()) {
				arr2.add(p);
			}
		}

		return ret;
	}

	private IJSONSerializable getRepositories(DMSession session, int ciid)
	{
	 if (ciid < 0)
	  return session.getRepositoryList();

		ComponentItem ci = session.getComponentItem(ciid, true);

		JSONArray ret = new JSONArray();
		List<Repository> repos = session.getAccessibleRepositories("ci"+ciid,ci.getDomainId());
		for(Repository r : repos) {
			System.out.println("Adding "+r.getName());
			ret.add(((LinkField) r.getLinkJSON()).setShowLink(false));
		}
		System.out.println("returning ret.size="+ret.length());
		return ret;
	}

	private IJSONSerializable getDefsForRepository(DMSession session, int repoid)
	{
		JSONObject ret = new JSONObject();
		JSONArray arr = new JSONArray();
		ret.add("defs", arr);

		if (repoid>0) {
			ProviderObject po = session.getRepository(repoid, false);

			// Lookup table of properties defined on the Repository itself
			Hashtable<String, DMProperty> repoprops = new Hashtable<String, DMProperty>();
			for(DMProperty p: po.getProperties()) {
				repoprops.put(p.getName(), p);
			}

			// Filter the defs
			for(DMPropertyDef d : po.getPropertyDefs()) {
				// Only pass the defs that are not defined or can be overridden or appended
				DMProperty rp = repoprops.get(d.getName());
				if((rp == null) || rp.isAppendable() || rp.isOverridable()) {
					arr.add(d);
				}
			}
		}
		return ret;
	}
}
