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
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Domain;

/**
 * Servlet implementation class GetActions
 */
public class GetFullDomains extends JSONServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetFullDomains() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
	  HashMap<String, Integer> doms = new HashMap<String, Integer>();
   JSONArray ret = new JSONArray();
	  String domlist = session.getDomainList();
	  String parts[] = domlist.split(",");
	  
	  if (parts != null)
	  {
	   for (int i=0;i<parts.length;i++)
	   {
	    int domainid = new Integer(parts[i]).intValue();
	    Domain dom = session.getDomain(domainid);
	    doms.put(dom.getFullDomain(), new Integer(dom.getId()));
	   }
	  }

	  List<String> sortedKeys=new ArrayList<String>(doms.keySet());
	  Collections.sort(sortedKeys);
	  
	  for (int k=0;k<sortedKeys.size();k++)
	  {
    String name = sortedKeys.get(k);
    Integer id = doms.get(name);
    JSONObject t = new JSONObject();
    t.add("id", id.intValue());
    t.add("name", name);
    ret.add(t);
	  }
			return ret;
	}
}
