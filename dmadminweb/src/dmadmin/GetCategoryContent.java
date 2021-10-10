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
import javax.servlet.http.HttpSession;

import dmadmin.model.TreeObject;

/**
 * Servlet implementation class GetComponentContent
 */
public class GetCategoryContent extends HttpServlet
{
 private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public GetCategoryContent()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setContentType("application/json");
  Integer domainid = ServletUtils.getIntParameter(request, "domainid");
  Integer catid = ServletUtils.getIntParameter(request, "catid");
  String objtype = request.getParameter("objtype");
  String nobuiltins = null;
  
  System.out.println("catid="+request.getParameter("catid")+" objtype="+objtype+" domainid="+domainid);
  PrintWriter out = response.getWriter();

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());

  // HashMap<Integer, Integer> hmap = new HashMap<Integer, Integer>();
  
  out.print("[");
  System.out.print("[");
  List<ObjectType> reltypes = new ArrayList<ObjectType>();
  
  if (objtype.equalsIgnoreCase("cy"))
  {
   reltypes.add(ObjectType.ACTION);
  }
  else if (objtype.equalsIgnoreCase("cp"))
  {
   reltypes.add(ObjectType.PROCEDURE);
   reltypes.add(ObjectType.FUNCTION);
  } 
  else if (objtype.equalsIgnoreCase("cf"))
  {
   reltypes.add(ObjectType.FRAGMENT);
  }    
  else if (objtype.equalsIgnoreCase("cc"))
  {
   reltypes.add(ObjectType.COMPONENT);
  }    
  
  boolean subv = false;
  for (ObjectType reltype : reltypes)
  {
   List<TreeObject> dmo = so.getTreeObjects(reltype, domainid, catid,nobuiltins);

   for (TreeObject dm : dmo)
   {
	   
    ObjectType ot = dm.GetOTID().getObjectType();
    System.out.println("GetCategoryContent ot="+ot+" dm.GetObjectType="+dm.GetObjectType());

    if (subv)
    {
     out.println(",");
     System.out.println(",");
    } 

    if (ot == ObjectType.ACTION)
    {
    	if (so.objHasChildren(dm, "action")) {
			out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"" + dm.GetOTID() +  "\", \"rel\" : \""+reltype+"\" },  \"children\": {}}");
		} else {
			out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"" + dm.GetOTID() +  "\", \"rel\" : \""+reltype+"\" }}");
		}
     // System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"ac" + dm.getId()  + "\", \"rel\" : \"action\" }}");
     // out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"ac" + dm.getId() +  "\", \"rel\" : \"action\" }}");
    }
    if (ot == ObjectType.PROCEDURE)
    {
    	if (so.objHasChildren(dm, "procedures")) {
  			System.out.println("PROCEDURE ");
  			System.out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+reltype+"\" }, \"children\": {}}");

  			out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+reltype+"\" }, \"children\": {}}");
  		} else {
  			out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+reltype+"\" }}");
  		}
     // System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
     // out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
    }
    else if (ot == ObjectType.FUNCTION)
    {
    	if (so.objHasChildren(dm, "functions")) {
  			out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+reltype+"\" }, \"children\": {}}");
  		} else {
  			out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+reltype+"\" }}");
  		}
     // System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
     // out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
    }
    else if (ot == ObjectType.COMPONENT)
    {
    	if (so.objHasChildren(dm,"components")) {
   	     System.out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"co" + dm.getId() + "\", \"rel\" : \"component\" }, \"children\": {}}");
   	     out.print("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"co" + dm.getId() + "\", \"rel\" : \"component\" }, \"children\": {}}");
    	} else {
	     System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"co" + dm.getId() + "\", \"rel\" : \"component\" }}");
	     out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"co" + dm.getId() + "\", \"rel\" : \"component\" }}");
    	}
    }
    subv = true;
   }
  }
  out.println("]");
  System.out.println("]");
  }
 }

 /**
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  // TODO Auto-generated method stub
 }

}
