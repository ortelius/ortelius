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
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.model.Repository;

/**
 * Servlet implementation class GetRepositories
 */
public class GetRepositories
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetRepositories() {
        super();
    }

	@Override
 public IJSONSerializable handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws PermissionDeniedException, ServletException, IOException
 {
  JSONArray ret = new JSONArray();
  String objid = request.getParameter("objid");
  String domstr = request.getParameter("domid");
  int domid = 0;
  if (domstr != null && domstr.length() > 0)
  {
   domid = Integer.parseInt(domstr);
  }
  List<Repository> repos = session.getAccessibleRepositories(objid, domid);

  Comparator<Repository> compareByName = new Comparator<Repository>()
  {
   @Override
   public int compare(Repository o1, Repository o2)
   {
    return o1.toString().compareTo(o2.toString());
   }
  };

  Collections.sort(repos, compareByName);

  for (Repository r : repos)
  {
   ret.add(r.getLinkJSON());
   // ret.add(new LinkField(ObjectType.REPOSITORY, r.getId(), session.getDomainName(r.getDomainId()) + "." + r.getName(), true));
  }

  return ret;
 }
}
