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
import dmadmin.model.Repository;
import dmadmin.model.Repository.TextPattern;

/**
 * Servlet implementation class GetRepoTextPatternsData
 */
public class GetRepoTextPatternsData
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    public GetRepoTextPatternsData() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int repoid = getIntParameter(request, "id");
		Repository repo = session.getRepository(repoid, false);
		
		String reason = request.getParameter("reason");
		if(reason != null) {
			if(reason.equalsIgnoreCase("save")) {
				return saveTextPatterns(session, request, response, repo);
			}
			throw new RuntimeException("Bad reason \"" + reason + "\"");
		}
		
		boolean readOnly = !repo.isUpdatable();
				
		JSONObject obj = new JSONObject();
		obj.add("readOnly", readOnly);

		JSONArray arr = new JSONArray();
		obj.add("data", arr);
		
		List<TextPattern> patterns = repo.getTextPatterns();
		for(TextPattern pattern : patterns) {
			arr.add(pattern);
		}
		
		return obj;
	}
	
	private IJSONSerializable saveTextPatterns(
		DMSession session, HttpServletRequest request,
		HttpServletResponse response, Repository repo)
	{
		ACDChangeSet<TextPattern> changes = new ACDChangeSet<TextPattern>();
		String error = "";
		
		for(Object oparam : request.getParameterMap().keySet()) {
			String param = (String) oparam;
			if(!param.startsWith("chg_") && !param.startsWith("add_") && !param.startsWith("del_")) { continue; }
			
			String value = request.getParameter(param);
			System.out.println("value = '" + value + "'");
			String[] parts = value.split("&");
			if(parts.length != 3) {
				error += "\nInvalid value " + value;
				continue;
			}
			String path = urlDecode(parts[0]);
			String pattern = urlDecode(parts[1]);
			boolean isText = parts[2].equalsIgnoreCase("Y");
			
			if((path == null) || (path.length() == 0) || (pattern == null) || (pattern.length() == 0)) {
				error +=  "\nInvalid value " + value + " (2)";
				continue;				
			}
			
			if(param.startsWith("chg_")) {
				System.out.println("change '" + path + "', '" + pattern + "' = '" + isText + "'");
				changes.addChanged(repo.new TextPattern(path, pattern, isText,param.substring(4)));
			} else if(param.startsWith("add_")) {
				System.out.println("add '" + path + "', '" + pattern + "' = '" + isText + "'");
				changes.addAdded(repo.new TextPattern(path, pattern, isText));
			} else if(param.startsWith("del_")) {
				System.out.println("delete '" + path + "', '" + pattern + "' = '" + isText + "'");
				changes.addDeleted(repo.new TextPattern(path, pattern, isText));
			}
		}
		
		if(error.length() > 0) {
			return new JSONObject().add("saved", false).add("error", error);
		}

		if(!repo.isUpdatable()) {
			return new JSONObject().add("saved", false).add("error", "You do not have permission to update this object");
		}
		boolean res = repo.updateTextPatterns(changes);
		return new JSONObject().add("saved", res);
	}
}
