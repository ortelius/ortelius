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
// import javax.servlet.http.HttpSession;

/**
 * Servlet implementation class MoveNode
 */
public class MoveNode extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public MoveNode() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
    public void handleRequest(DMSession session, boolean isPost,
    	   		HttpServletRequest request, HttpServletResponse response)
    	   throws ServletException, IOException
    {
    	response.setContentType("application/json");
		String nodetype = request.getParameter("nodetype");
		int nodeid   = ServletUtils.getIntParameter(request,"nodeid",0);
		String pid = request.getParameter("parentid");
		System.out.println("pid="+pid);
		int parentid;
		if (pid.indexOf('-')>0) {
			parentid = Integer.parseInt((pid.substring(0,pid.indexOf('-'))));
		} else {
			parentid = Integer.parseInt(pid);
		}
		/*
		else {
			parentid=0;
		}
		*/
		System.out.println("parentid="+parentid);
		String targettype = request.getParameter("targettype");
        int targetid = ServletUtils.getIntParameter(request,"targetid",0);

		PrintWriter out = response.getWriter();
		String errtext = session.MoveObject(nodetype, parentid, nodeid, targettype, targetid);
		out.print("{\"errtext\" : \"" + errtext + "\"}");
    }
}
