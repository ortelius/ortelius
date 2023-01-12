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
import dmadmin.model.ServerType;

/**
 * Servlet implementation class GetServerTypes
 */
public class GetServerTypes
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;

    public GetServerTypes() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		List<ServerType> types = session.getServerTypes();

		JSONArray ret = new JSONArray();
		for(ServerType t : types) {
		 if (t.getName().equalsIgnoreCase("AS400") || t.getName().equalsIgnoreCase("Mac"))
		  continue;

			ret.add(t.getLinkJSON());
		}
		return ret;
	}
}
