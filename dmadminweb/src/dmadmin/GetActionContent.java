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
import javax.servlet.http.HttpSession;

import dmadmin.model.Action;

/**
 * Servlet implementation class GetActionContent
 */
public class GetActionContent extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetActionContent() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		System.out.println("GetActionContent");
		response.setContentType("application/json");
		Integer actionid = ServletUtils.getIntParameter(request,"actionid"); 
		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());
  
		Action action = so.getAction(actionid,false);

		out.print("[");

		if (action != null) {  
			List<Action> a = so.getActionVersions(action);
	
			boolean subv=false;
			for (Action av: a)
			{
				ObjectType ot = av.getObjectType();
				if (subv) out.println(",");
				String kind;
				if (ot != ObjectType.ACTION) {
					// Functions/Procedures need kind appending
					kind = "-" + av.getKind().value();
				} else {
					kind = "";
				}
				
				if (av.getParentId() > 0)
					out.print("{\"data\" : \"" + av.getName() + "\", \"attr\" : { \"id\" : \"" + av.getOtid() + kind + "\", \"rel\" : \"" + ot + "\" }}");
				else
					out.print("{\"data\" : \"" + av.getName() + "\", \"attr\" : { \"id\" : \"" + av.getOtid() + kind + "\", \"rel\" : \"" + ot + "\" }}");
				subv=true;
			}
		}
		out.println("]");
  }
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
