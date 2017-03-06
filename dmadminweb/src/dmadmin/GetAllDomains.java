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
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;
import dmadmin.model.TreeObject;

/**
 * Servlet implementation class DomainDetails
 */
public class GetAllDomains extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetAllDomains() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    
    @Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{ 		
   		PrintWriter out = response.getWriter();
   		ArrayList<JSONObject> list = new ArrayList<JSONObject>();
  
   		RecurseDomains(session,list,"",0);
   		String jsonStr =  list.toString();
   		out.println(jsonStr);
	}
 
 void RecurseDomains(DMSession session, ArrayList<JSONObject> list,String Parent, int id)
 {
  List<TreeObject> subdomains = session.getDomains(id);
  for (int i=0;i<subdomains.size();i++)
  {
   TreeObject sub = subdomains.get(i);
   String newParent = Parent + sub.getName();
   JSONObject domain = new JSONObject();
   domain.add("name", newParent);
   domain.add("id", sub.getId());
   domain.add("type", "do");
   
   list.add(domain);
   newParent += ".";
   RecurseDomains(session,list,newParent,sub.getId());
  }
 }
}
