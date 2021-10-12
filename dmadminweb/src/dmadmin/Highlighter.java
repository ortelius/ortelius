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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Server;

/**
 * Servlet implementation class Highlighter
 */
public class Highlighter extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Highlighter() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    @Override
    public void handleRequest(DMSession session, boolean isPost,
    	   		HttpServletRequest request, HttpServletResponse response)
    	   throws ServletException, IOException
    {
    	response.setContentType("application/json;charset=UTF-8");
		String mode = request.getParameter("f");
		
		PrintWriter out = response.getWriter();
		
		if (mode.equalsIgnoreCase("swc"))
		{
			// Servers with components....
			int e = ServletUtils.getIntParameter(request, "e");	// environment id
			int c = ServletUtils.getIntParameter(request, "c");	// component id
			List <Server> sl = session.GetServersWithComponents(e,c);
			out.print("{");
			out.println("\"server\": [");
			
			boolean sep=false;
			for (Server s: sl)
			{
				if (sep) out.println(",");
				
				out.println("{");
				out.println("\"id\" : \""       + s.getId() + "\",");
				out.println("\"name\" : \""     + s.getName() + "\",");
				out.println("\"summary\" : \""  + s.getSummary() + "\"");
				out.println("}");
				sep=true;
			}
			out.println("],");
			out.println("\"ServerCount\":\"" + sl.size() + "\"");
			out.println("}");
		}
    }
}
