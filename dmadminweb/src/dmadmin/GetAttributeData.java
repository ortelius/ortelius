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
import dmadmin.model.DMObject;


/**
 * Servlet implementation class GetAttributeData
 */
public class GetAttributeData
	extends HttpServlet
{
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetAttributeData() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		response.setContentType("application/json");

		PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

		boolean isArrayRequest = (request.getParameter("arrid") != null);
		boolean bReadOnly=false;
		List<DMAttribute> attrs = null;
		
		if(isArrayRequest) {
			int arrid = ServletUtils.getIntParameter(request, "arrid");
			attrs = so.getArrayAttributes(arrid);
		} else {	 
			int ot = ServletUtils.getIntParameter(request, "objtype");
			int id = ServletUtils.getIntParameter(request, "id");
			ObjectType objtype = ObjectType.fromInt(ot);
			if(objtype == null) {
				throw new RuntimeException("Invalid object type " + ot);
			}			
			DMObject dmobj = so.getObject(objtype, id);
			bReadOnly = !dmobj.isUpdatable();
			attrs = dmobj.getAttributes();
		}

		JSONArray arr = new JSONArray();
		for(DMAttribute a: attrs) {
			arr.add(a);
		}
		
		JSONObject obj = new JSONObject();
		obj.add("readOnly",bReadOnly);
		obj.add("data", arr);
		
		String ret = obj.toString();
		
		out.println(ret);
		System.out.println(ret);	
  }
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{}
}
