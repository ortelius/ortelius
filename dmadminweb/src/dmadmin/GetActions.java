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
		String reason = request.getParameter("reason");
		JSONArray ret = new JSONArray();
		if (reason.equalsIgnoreCase("getrefs")) {
			String actionid = request.getParameter("actionid").replaceAll("-.*$","");
			return session.GetActionReferences(Integer.parseInt(actionid));
		} else {
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
