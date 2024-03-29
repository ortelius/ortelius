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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.JSONObject;

/**
 * Servlet implementation class GetSchemaVersion
 */
public class About extends HttpServletBase
{
 private static final long serialVersionUID = 1L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public About()
 {
  super();
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  int ver = session.getSchemaVersion();
  String lc = session.getLicenseKey();
  String dbuid = session.getDatabaseId();
  request.setAttribute("schemaver", ver);
  request.setAttribute("license", lc);
  request.setAttribute("dbuid", dbuid);

  String lickey = request.getParameter("k");

  if (lickey != null)
  {
   session.updateLicense(lickey);
   response.getOutputStream().println(new JSONObject().toString());
  }
  else
   request.getRequestDispatcher("/WEB-INF/About.jsp").forward(request, response);
 }
}
