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

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Environment;

/**
 * Servlet implementation class GetDeploymentStepsGantt
 */
public class GetDeployDeps extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetDeployDeps() {
        super();
    }
    
@Override    
 public IJSONSerializable handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setContentType("application/json");
  int deployid = -1;
  String appid = request.getParameter("appid");
  String compid = request.getParameter("compid");
  JSONObject data = null;

  String envid = request.getParameter("envid");
  
  if (envid != null && appid != null)
  {
   if (envid.isEmpty() || appid.isEmpty())
   { 
    data = new JSONObject();   
    JSONArray nodes = new JSONArray();
    JSONArray edges = new JSONArray();
    data.add("nodes",nodes);
    data.add("edges",edges);
    return data;
   }
   
   Application found = null;
   Application app = session.getApplication(new Integer(appid).intValue(), true);
   Application curr = app;
   Environment env = session.getEnvironment(new Integer(envid).intValue(), false);
   
   while (curr.getParentId() > 0)
   {
    curr = session.getApplication(curr.getParentId(), true);
    if (session.isAppInEnv(curr, env))
    {
     found = curr;
     break;
    } 
   }
   data = session.getDiffDepsNodesEdges(app, found, env);
  }
  else if (envid != null)
  {
   data = session.getEnvironmentDepsNodesEdges(envid);
  }
  else if (compid != null)
  {
   data = session.getComponentDepsNodesEdges(compid);
  }
  else
  {
   deployid = ServletUtils.getIntParameter(request, "deployid");

   data = session.getDeploymentDepsNodesEdges(deployid);
  }
  String ret = data.getJSON();
  System.out.println(ret);
  return data;
 }
}
