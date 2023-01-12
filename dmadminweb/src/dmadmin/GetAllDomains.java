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
