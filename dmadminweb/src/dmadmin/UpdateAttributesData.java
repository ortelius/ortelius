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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;
import dmadmin.model.DMAttribute;
import dmadmin.model.DMObject;

/**
 * Servlet implementation class UpdateAttributesData
 */
public class UpdateAttributesData extends HttpServletBase
{
 private static final long serialVersionUID = 1L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public UpdateAttributesData()
 {
  super();
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  int ot = ServletUtils.getIntParameter(request, "objtype");
  int id = ServletUtils.getIntParameter(request, "id");
  ObjectType objtype = ObjectType.fromInt(ot);
  if (objtype == null)
  {
   throw new RuntimeException("Invalid object type " + ot);
  }

  response.setContentType("application/json");
  PrintWriter out = response.getWriter();
  JSONObject obj = new JSONObject();

  try
  {
   AttributeChangeSet changes = new AttributeChangeSet();

   for (Object oparam : request.getParameterMap().keySet())
   {
    String param = (String) oparam;
    if (param.startsWith("arr_"))
    {
     int arrayid = ServletUtils.getIntParameter(request, param, 0);
     String key = "";
     String value = "";
     if (param.startsWith("arr_chg_"))
     {
      String name = param.substring(8);
      System.out.println("change array '" + name + "' (" + arrayid + ")");
      changes.addChanged(new DMAttribute(name, arrayid, key, value));
     }
     else if (param.startsWith("arr_add_"))
     {
      String name = param.substring(8);
      System.out.println("add array '" + name + "' (" + arrayid + ")");
      changes.addAdded(new DMAttribute(name, arrayid, key, value));
     }
     else if (param.startsWith("arr_del_"))
     {
      String name = param.substring(8);
      System.out.println("delete array '" + name + "' (" + arrayid + ")");
      changes.addDeleted(new DMAttribute(name, arrayid, key, value));
     }
    }
    else if (param.startsWith("ele_"))
    {
     String value = request.getParameter(param);
     int index = param.lastIndexOf('_');
     if (index != -1)
     {
      index++;
      try
      {
       if (param.startsWith("ele_chg_"))
       {
        int arrayid = Integer.parseInt(param.substring(index));
        String work = param.substring(0,index-1);
        work = work.substring("ele_chg_".length());
        work = work.replaceAll("\"", "");
        work = work.replaceAll("'", "");
        String[] parts = work.split("[\\[\\]]");
        String name = parts[0];
        String key = parts[1];
        System.out.println("change array " + name + " (" + arrayid + ") element '" + key + "' = '" + value + "'");
        changes.addChanged(new DMAttribute(name, arrayid, key, value));
       }
       else if (param.startsWith("ele_add_"))
       {
        param = param.replace("_undefined", "_-1");
        int arrayid = -1;
        try
        {
         arrayid = Integer.parseInt(param.substring(index));
        } 
        catch (NumberFormatException e)
        {
         
        }
        String work = param.substring(0,index-1);
        work = work.substring("ele_add_".length());
        work = work.replaceAll("\"", "");
        work = work.replaceAll("'", "");
        String[] parts = work.split("[\\[\\]]");
        String name = parts[0];
        String key = parts[1];
        System.out.println("add array " + name + " (" + arrayid + ") element '" + key + "' = '" + value + "'");
        changes.addAdded(new DMAttribute(name, arrayid, key, value));
       }
       else if (param.startsWith("ele_del_"))
       {
        int arrayid = -1;
        try
        {
         arrayid = Integer.parseInt(param.substring(index));
        } 
        catch (NumberFormatException e)
        {
         
        }
        String work = param.substring(0,index-1);
        work = work.substring("ele_del_".length());
        work = work.replaceAll("\"", "");
        work = work.replaceAll("'", "");
        String[] parts = work.split("[\\[\\]]");
        if (parts.length == 2)
        {
         String name = parts[0];
         String key = parts[1];
         System.out.println("delete array (" + arrayid + ") element '" + name + "' = '" + value + "'");
         changes.addDeleted(new DMAttribute(name, arrayid, key, value));
        } 
        else
        {
         String name = work;
         System.out.println("delete array (" + arrayid + ") element '" + name + "' = '" + value + "'");
         changes.addDeleted(new DMAttribute(name, value));
        }

       }
      }
      catch (NumberFormatException e)
      {
       e.printStackTrace();
      }
     }
    }
    else
    {
     String value = request.getParameter(param);
     if (param.startsWith("chg_"))
     {
      String name = param.substring(4);
      System.out.println("change '" + name + "' = '" + value + "'");
      changes.addChanged(new DMAttribute(name, value));
     }
     else if (param.startsWith("add_"))
     {
      String name = param.substring(4);
      System.out.println("add '" + name + "' = '" + value + "'");
      changes.addAdded(new DMAttribute(name, value));
     }
     else if (param.startsWith("del_"))
     {
      String name = param.substring(4);
      System.out.println("delete '" + name + "' = '" + value + "'");
      changes.addDeleted(new DMAttribute(name, value));
     }
    }
   }
   System.out.println("id=" + id + "type string=" + objtype.getTypeString());
   DMObject dmobj = session.getObject(objtype, id);
   if (dmobj.isUpdatable())
   {
    boolean res = dmobj.updateAttributes(changes);
    obj.add("saved", res);
   }
   else
   {
    obj.add("saved", false);
    obj.add("error", "You do not have permission to update this object");
   }
  }
  catch (Exception e)
  {
   e.printStackTrace();
   obj.add("saved", false);
   obj.add("error", e.getMessage());
  }

  String ret = obj.getJSON();
  System.out.println(ret);
  out.println(ret);
 }
}
