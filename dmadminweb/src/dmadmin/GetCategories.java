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
