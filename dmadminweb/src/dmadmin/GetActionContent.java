/*
 *
 *  Ortelius for Microservice Configuration Mapping
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
