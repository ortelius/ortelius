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
