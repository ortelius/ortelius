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

import dmadmin.model.TreeObject;

public class GetFragmentContent extends HttpServlet
{
  private static final long serialVersionUID = 1L;
  DMSession so = null;
  HttpSession session = null;

  protected void doGet(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException
  {
    String nobuiltins = null;
    response.setContentType("application/json");
    Integer domainid = Integer.valueOf(ServletUtils.getIntParameter(request, "domainid"));
    Integer catid = Integer.valueOf(ServletUtils.getIntParameter(request, "catid"));
    PrintWriter out = response.getWriter();

    try (DMSession so = DMSession.getInstance(request)) {
    session = request.getSession();
    session.setAttribute("session", so);
    so.checkConnection(request.getServletContext());

    out.print("[");
    System.out.print("[");

    List<TreeObject> dmo = so.getTreeObjects(ObjectType.FRAGMENT, domainid.intValue(), catid.intValue(),nobuiltins);

    int k = 0;
    for (TreeObject dm : dmo)
    {
    	System.out.println("dm.Get="+dm.GetObjectType());
    	ObjectType reltype = dm.GetObjectType();
    	String uct = Character.toUpperCase(reltype.toString().charAt(0)) + reltype.toString().substring(1).toLowerCase();

      System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fg" + dm.getId() + "\", \"rel\" : \"" + uct + "\" }}");
      out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fg" + dm.getId() + "\", \"rel\" : \"" + uct + "\" }}");
      k++;
      if (k >= dmo.size())
        continue;
      System.out.println(",");
      out.println(",");
    }

    out.println("]");
    System.out.println("]");
    }
  }

  protected void doPost(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException
  {
  }
}

/* Location:           /Users/steve/git/DM2/dmadminweb/WebContent/WEB-INF/classes/
 * Qualified Name:     dmadmin.GetFragmentContent
 * JD-Core Version:    0.6.0
 */
