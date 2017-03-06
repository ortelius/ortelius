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

import dmadmin.model.TreeObject;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

public class GetFragmentContent extends HttpServlet
{
  private static final long serialVersionUID = 1L;

  protected void doGet(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException
  {
    response.setContentType("application/json");
    Integer domainid = Integer.valueOf(ServletUtils.getIntParameter(request, "domainid"));
    Integer catid = Integer.valueOf(ServletUtils.getIntParameter(request, "catid"));
    PrintWriter out = response.getWriter();
    HttpSession session = request.getSession();
    DMSession so = (DMSession)session.getAttribute("session");

    out.print("[");
    System.out.print("[");

    List<TreeObject> dmo = so.getTreeObjects(ObjectType.FRAGMENT, domainid.intValue(), catid.intValue());

    int k = 0;
    for (TreeObject dm : dmo)
    {
    	System.out.println("dm.Get="+dm.GetObjectType());
    	
      System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fg" + dm.getId() + "\", \"rel\" : \"" + dm.GetObjectType() + "\" }}");
      out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fg" + dm.getId() + "\", \"rel\" : \"" + dm.GetObjectType() + "\" }}");
      k++;
      if (k >= dmo.size())
        continue;
      System.out.println(",");
      out.println(",");
    }

    out.println("]");
    System.out.println("]");
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