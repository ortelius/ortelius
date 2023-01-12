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
