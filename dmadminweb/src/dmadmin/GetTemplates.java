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
import dmadmin.model.NotifyTemplate;

/**
 * Servlet implementation class GetActions
 */
public class GetTemplates extends JSONServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetTemplates() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		System.out.println("in GetTemplates");
		String objid = request.getParameter("objid");
		String domstr = request.getParameter("domid");
		int domid = 0;
		if (domstr != null && domstr.length()>0) {
			domid = Integer.parseInt(domstr);
		}
		System.out.println("objid="+objid);
		List<NotifyTemplate> templates = session.getAccessibleTemplates(objid,domid);
		JSONArray ret = new JSONArray();
		for(NotifyTemplate t : templates) {
			ret.add(t.getLinkJSON());
		}
		return ret;
	}

}
