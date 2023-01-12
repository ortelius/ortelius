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
import dmadmin.model.Application;
import dmadmin.model.Task.TaskType;

/**
 * Servlet implementation class GetActions
 */
public class GetAppVersionAccess extends JSONServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetAppVersionAccess() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
	 JSONObject ret = new JSONObject();
	 int appid = getIntParameter(request,"appid");
	 Application app = session.getApplication(appid,false);

	 ret.add("hasaccess", session.getCreatePermission(ObjectType.APPVERSION));
	 ret.add("createversion", session.isTaskInDomain(app.getDomainId(),TaskType.CREATE_VERSION));

	 return ret;
	}

}
