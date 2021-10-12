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
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.JSONObject;
import dmadmin.model.Domain;
import dmadmin.model.Engine;
import dmadmin.model.Server;
//import dmadmin.PropertyDataSet;
import dmadmin.util.CommandLine;

public class TestServerConnection extends HttpServletBase
{
	private static final long serialVersionUID = 1327862378913381548L;
	private Domain m_domain;
	private CommandLine m_cmd;
	
	public TestServerConnection() {
	}

	
	public Domain getTargetDomain() { return m_domain; }
	public void setTargetDomain(Domain domain) { m_domain = domain; }
	
	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		PrintWriter out = response.getWriter();
		int sid = getIntParameter(request,"id");
		Server server = session.getServer(sid,true);
		m_domain = server.getDomain();
		Engine engine = (m_domain != null) ? m_domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return;
		}
		System.out.println("testing server "+server.getId());
		m_cmd = engine.doTestServer(server);
		m_cmd.runWithTrilogy(true, "");
		JSONObject ret = new JSONObject();
		ret.add("success",true);
		out.println(ret.getJSON());
	}

 
	public String getOutput() {
		return m_cmd.getOutput();
	}
}
