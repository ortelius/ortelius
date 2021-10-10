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
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class RemoveNode
 */
public class RemoveNode extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public RemoveNode() {
        super();
        // TODO Auto-generated constructor stub
    }

	
	
	
	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		response.setContentType("application/json");
		String rel   = request.getParameter("rel");
		String rel_param = rel;
		String idStr = request.getParameter("id");
		int id = 0;
		if (idStr.contains("-")) {
			String[] parts = idStr.split("-");
			id = new Integer(parts[0]).intValue();
		} else {
			id = new Integer(idStr).intValue();
		}
		PrintWriter out = response.getWriter();
		// String errtext = "";
		if (rel.equalsIgnoreCase("appversion") || rel.equalsIgnoreCase("release")) rel_param = "Application";
		
		session.RemoveObject(rel_param, id, out);
	}
}
