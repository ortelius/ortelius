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

import dmadmin.model.Component;

/**
 * Servlet implementation class GetServerContent
 */
public class GetServerContent extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetServerContent() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("application/json");
		String serverid = request.getParameter("serverid");
		// Get the printwriter object from response to write the required json object to the output stream
		PrintWriter out = response.getWriter();

		List<Component> d = session.getComponents(ObjectType.SERVER,Integer.parseInt(serverid), false);

		out.print("[");

		boolean subenv=false;
		for (Component xd: d)
		{
			if (subenv) out.println(",");
			out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.getOtid() + "\", \"rel\" : \"" + ObjectType.COMPONENT + "\" }}");
			subenv=true;
		}
		out.println("]");
   	}
}
