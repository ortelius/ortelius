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
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Domain;

/**
 * Servlet implementation class GetActions
 */
public class GetFullDomains extends JSONServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetFullDomains() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
	  HashMap<String, Integer> doms = new HashMap<String, Integer>();
   JSONArray ret = new JSONArray();
	  String domlist = session.getDomainList();

	  session.getFullDomain(domlist, doms);

	  List<String> sortedKeys=new ArrayList<String>(doms.keySet());
	  Collections.sort(sortedKeys);

	  for (int k=0;k<sortedKeys.size();k++)
	  {
    String name = sortedKeys.get(k);
    Integer id = doms.get(name);
    JSONObject t = new JSONObject();
    t.add("id", id.intValue());
    t.add("name", name);
    ret.add(t);
	  }
			return ret;
	}
}
