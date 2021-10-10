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
