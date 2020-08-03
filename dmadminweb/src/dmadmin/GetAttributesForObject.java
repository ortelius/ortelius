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

import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.DMAttribute;


/**
 * Servlet implementation class GetAttributesForObject
 */
public class GetAttributesForObject extends HttpServlet {
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;
 
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetAttributesForObject() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException
	{
		response.setContentType("application/json");
		
		System.out.println("In GetAttributesForObject");
		System.out.println("id"+request.getParameter("id"));
		
		String objtype = request.getParameter("objtype");
		int id = Integer.parseInt(request.getParameter("id"));
		

		// Get the printwriter object from response to write the required json object to the output stream      
		PrintWriter out = response.getWriter();
		
  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());
		
		List<DMAttribute> atts = so.getAttributes(objtype, id);
				
		JSONObject obj = new JSONObject();
		JSONArray data = new JSONArray();
		obj.add("data", data);
		
		for(DMAttribute a: atts) {
			data.add(new JSONArray().add(a.getName()).add(a.getValue()));
		}
		
		String ret = obj.getJSON();
		out.println(ret);
		System.out.println(ret);		
		out.flush();
  }
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}
}
