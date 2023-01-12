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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;
import dmadmin.model.Engine;
import dmadmin.model.ProviderDefinition;

/**
 * Servlet implementation class GetProiderDefDetailsData
 */
public class GetProviderDefDetailsData
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetProviderDefDetailsData() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int defid = getIntParameter(request, "defid");
		int engineid = getIntParameter(request, "engid");

		ProviderDefinition def = session.getProviderDefinition(defid);
		Engine engine = session.getEngine(engineid);

		JSONObject obj = new JSONObject();
		obj.add("data", def.getDetailsForEngine(engine).getJSON());
		return obj;
	}

}
