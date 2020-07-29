/*
 *
 *  DeployHub is an Agile Application Release Automation Solution
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