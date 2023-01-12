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

import dmadmin.model.Component;

/**
 * Servlet implementation class GetComponentContent
 */
public class GetComponentContent extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetComponentContent() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("application/json");
		Integer compid = ServletUtils.getIntParameter(request,"compid");
		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		Component comp = so.getComponent(compid,false);

		out.print("[");

		if (comp != null) {
			List<Component> c = so.getComponentVersions(comp);

			boolean subv=false;
			for (Component xav: c)
			{
				if (subv) out.println(",");

				if (xav.getParentId() > 0)
					out.print("{\"data\" : \"" + xav.getName() + "\", \"attr\" : { \"id\" : \"" + xav.getOtid() + "\", \"rel\" : \"" + ObjectType.COMPVERSION + "\" }}");
				else
					out.print("{\"data\" : \"" + xav.getName() + "\", \"attr\" : { \"id\" : \"" + xav.getOtid() + "\", \"rel\" : \"" + ObjectType.COMPONENT + "\" }}");
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
