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
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Action;
import dmadmin.model.ActionKind;

/**
 * Servlet implementation class GetActions
 */
public class GetActions extends JSONServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetActions() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
	 ArrayList<Integer> dups = new ArrayList<Integer>();

		String reason = request.getParameter("reason");
		JSONArray ret = new JSONArray();
		if (reason.equalsIgnoreCase("getrefs")) {
			String actionid = request.getParameter("actionid").replaceAll("-.*$","");
			return session.GetActionReferences(Integer.parseInt(actionid));
		}
		else if (reason.equalsIgnoreCase("dropdown"))
		{
   ret = session.getActionList();
   return ret;
		}
		else {
			String objid = request.getParameter("objid");
			String domstr = request.getParameter("domid");
			int domid = 0;
			if (domstr != null && domstr.length()>0) {
				domid = Integer.parseInt(domstr);
			}
			System.out.println("objid="+objid);
			List<Action> actions = session.getAccessibleActions(objid,domid);
			for(Action a : actions) {
				if (reason.equalsIgnoreCase("actonly")) {
					if (a.getKind() == ActionKind.GRAPHICAL || a.isFunction()) {
					 if (dups.contains(a.getId()))
					  continue;

					 dups.add(a.getId());
						JSONObject t = new JSONObject();
						t.add("action",a.getLinkJSON());
						System.out.println("** action="+a.getName()+" uselink="+session.ValidDomain(a.getDomainId())+" a.getDomainId()="+a.getDomainId());
						t.add("uselink",session.ValidDomain(a.getDomainId()));
						ret.add(t);
					}

				} else {
					ret.add(a.getLinkJSON());
				}
			}
			return ret;
		}
	}

}
