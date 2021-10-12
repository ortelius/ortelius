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

//import java.io.BufferedReader;
import java.io.IOException;
//import java.io.InputStream;
//import java.io.InputStreamReader;
import java.io.PrintWriter;
//import java.util.ArrayList;
//import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Domain;
import dmadmin.model.Engine;
import dmadmin.model.Notify;
import dmadmin.model.ProviderObject;
import dmadmin.util.CommandLine;

/**
 * Servlet implementation class RunProviderTest
 */
public class RunProviderTest
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RunProviderTest() {
        super();
    }
    
    @Override
   	public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("text/text");

		PrintWriter out = response.getWriter();

		int id = ServletUtils.getIntParameter(request, "id");
		int ot = ServletUtils.getIntParameter(request, "objtype");
		String rcpt = request.getParameter("rcpt");
		ObjectType objtype = ObjectType.fromInt(ot);

		ProviderObject po = session.getProviderObject(objtype, id, true);
		if (po.getObjectType() == ObjectType.NOTIFY && rcpt != null) {
			((Notify)po).setTestRecipient(rcpt);
		}
	
		Domain domain = po.getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		
		if(engine == null) {
			System.err.println("Engine was null");
			out.println("Unable to find suitable engine to conduct test");
			return;
		}
		

		
	 CommandLine cmd = engine.doProviderTest(po);
	 cmd.runWithTrilogy(true, null);
	 out.println(cmd.getOutput());
 }
}
