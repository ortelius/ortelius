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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Category;
import dmadmin.model.Fragment;


/**
 * Servlet implementation class GetCategories
 */
public class GetCategories extends JSONServletBase {
	private static final long serialVersionUID = 1L;
       
    public GetCategories() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws PermissionDeniedException, ServletException, IOException
	{
		String reason = request.getParameter("reason");
		if(reason != null) {
			if(reason.equals("action")) {
				return doCategoriesForAction(session);
			} else if(reason.equals("newcat")) {
				return doNewCategory(session, request);
			}
			throw new RuntimeException("Not a good enough reason");
		}
		
		List <Category> categories = session.GetFragmentsAndCategories();
		
		JSONArray arr1 = new JSONArray();
		for(Category c: categories) {
			JSONObject cobj = new JSONObject();
			cobj.add("id", c.getId());
			cobj.add("name", c.getName());
			List <Fragment> fragments = c.getFragments();
			JSONArray arr2 = new JSONArray();
			for(Fragment f: fragments) {
				JSONObject fobj = new JSONObject();
				fobj.add("id",f.getId());
				fobj.add("name",f.getName());
				fobj.add("summary",f.getSummary());
				fobj.add("exitpoints",f.getExitpoints());
				fobj.add("drilldown", f.getDrillDown());
				fobj.add("actionid", f.getActionId());
				arr2.add(fobj);
			}
			cobj.add("fragments",arr2);
			cobj.add("fragcount",fragments.size());
			arr1.add(cobj);
		}
		
		JSONObject obj = new JSONObject();
		obj.add("Categories", arr1);
		obj.add("CategoryCount", categories.size());
		return obj;
	}

	private IJSONSerializable doCategoriesForAction(DMSession session)
	{
		JSONArray ret = new JSONArray();		
		ret.add(new Category(-1, "Create new category...").getLinkJSON());

		for(Category c: session.GetFragmentsAndCategories()) {
			ret.add(c.getLinkJSON());
		}
		return ret;
	}

	private IJSONSerializable doNewCategory(DMSession session, HttpServletRequest request)
	{
		String name = request.getParameter("name");
		if(name == null) {
			return new JSONObject().add("result", false).add("error", "No category name");
		}
		
		try {
			Category c = session.createNewCategory(name);
			return new JSONObject().add("result", true).add("newid", c.getId()).add("name", c.getName());
		} catch(RuntimeException e) {			
			return new JSONObject().add("result", false).add("error", e.getMessage());
		}
	}
}
