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
 * Servlet implementation class RenameNode
 */
public class RenameNode extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public RenameNode() {
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
		String id = request.getParameter("id");
		String newname = request.getParameter("newname");
		System.out.println("newname=["+newname+"] id=["+id+"] rel=["+rel+"]");
		// Get the printwriter object from response to write the required json object to the output stream
		PrintWriter out = response.getWriter();

		String errtext = "";
		String aoran;
		if ("AEIOUaeiou".indexOf(rel.charAt(0))>0)
		{
			aoran="An";
		}
		else
		{
			aoran="A";
		}
		boolean dup = false;
		String origname = newname;
		int j = 1;
		boolean renmsg = false;
		int res = 0;

		do
		{
		 res = session.RenameObject(rel, Integer.parseInt(id), newname);
		 if (res == 1)
		 {
		  newname = origname + "_" + j;
		  renmsg = true;
		  dup = true;
		 }
		 else
		  dup = false;
		 j++;
		} while (dup || j > 100);

		if (renmsg)
		{
			errtext=aoran+" "+rel+" with " + origname + " already exists and has been set to " + newname;
			res = 1;
		}

		 out.print("{\"errtext\" : \"" + errtext + "\", \"errcode\" : \"" + res + "\", \"newname\": \"" + newname + "\"}");
	}
}
