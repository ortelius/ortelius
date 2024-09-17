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
import java.util.HashMap;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.model.Domain;
import dmadmin.model.TreeObject;
import dmadmin.pro.DMCalendarEvent;
import dmadmin.pro.DMSessionPro;

public class GetDomainContent extends HttpServletBase
{
  private static final long serialVersionUID = 1L;

  void ProcessDomainContent(PrintWriter out, DMSession so, String typestr, int domainid, int catid, HashMap<Integer, Integer> hmap, String nobuiltins)
  {
    Domain pd = domainid > 0 ? so.getDomain(domainid) : null;
    String rel = pd != null ? (pd.getLifecycle() ? "Lifecycle" : "Domain") : "Domain";
    boolean subenv = false;

    if (catid < 0)
    {
      List<TreeObject> d = so.getDomains(Integer.valueOf(domainid));

      System.out.print("[");
      out.print("[");

      for (TreeObject xd : d)
      {
       rel = Character.toUpperCase(rel.charAt(0)) + rel.substring(1).toLowerCase();

       if ((typestr.equalsIgnoreCase("components") || typestr.equalsIgnoreCase("actions") ||
            typestr.equalsIgnoreCase("functions") || typestr.equalsIgnoreCase("procedures") || typestr.equalsIgnoreCase("notifiers")) && rel.equals("Lifecycle"))
        continue;

        if (subenv)
        {
          System.out.println(",");
          out.println(",");
        }
        String state = hmap.containsKey(Integer.valueOf(xd.getId())) ? "open" : domainid == 0 ? "open" : "closed";

        if (so.objHasChildren(xd, typestr))
        {
          System.out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \"" + state + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
          out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \"" + state + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
        }
        else
        {
          System.out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
          out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
        }

        if (state.equalsIgnoreCase("open"))
        {
          System.out.print(", \"children\": ");
          out.print(", \"children\": ");

          ProcessDomainContent(out, so, typestr, xd.getId(), -1, hmap, nobuiltins);
        }
        else
        {
          System.out.print(", \"children\": ");
          out.print(", \"children\": ");

          System.out.println("{}");
          out.println("{}");
        }
        System.out.print("}");
        out.print("}");
        subenv = true;
      }

    }

    List<ObjectType> reltypes = new ArrayList<ObjectType>();
    if ((domainid > 0) && (typestr != null))
    {
      if (typestr.equalsIgnoreCase("workflow"))
      {
        reltypes.add(ObjectType.APPLICATION);
        reltypes.add(ObjectType.APPVERSION);
      }
      else if (typestr.equalsIgnoreCase("environments"))
      {
        reltypes.add(ObjectType.ENVIRONMENT);
      }
      else if (typestr.equalsIgnoreCase("repositories"))
      {
        reltypes.add(ObjectType.REPOSITORY);
      }
      else if (typestr.equalsIgnoreCase("servers"))
      {
        reltypes.add(ObjectType.SERVER);
      }
      else if (typestr.equalsIgnoreCase("source"))
      {
        reltypes.add(ObjectType.REPOSITORY);
      }
      else if (typestr.equalsIgnoreCase("applications"))
      {
        reltypes.add(ObjectType.APPLICATION);
        reltypes.add(ObjectType.APPVERSION);
      }
      else if (typestr.equalsIgnoreCase("releases"))
      {
        reltypes.add(ObjectType.RELEASE);
      }
      else if (typestr.equalsIgnoreCase("actions"))
      {
        reltypes.add(ObjectType.ACTION_CATEGORY);
      }
      else if (typestr.equalsIgnoreCase("procedures"))
      {
        reltypes.add(ObjectType.PROCFUNC_CATEGORY);
      }
      else if (typestr.equalsIgnoreCase("pfcategory"))
      {
        reltypes.add(ObjectType.PROCEDURE);
        reltypes.add(ObjectType.FUNCTION);
      }
      else if (typestr.equalsIgnoreCase("functions"))
      {
        reltypes.add(ObjectType.FUNCTION);
      }
      else if (typestr.equalsIgnoreCase("components"))
      {
        reltypes.add(ObjectType.COMP_CATEGORY);
      }
      else if (typestr.equalsIgnoreCase("credentials"))
      {
        reltypes.add(ObjectType.CREDENTIALS);
      }
      else if (typestr.equalsIgnoreCase("datasources"))
      {
        reltypes.add(ObjectType.DATASOURCE);
      }
      else if (typestr.equalsIgnoreCase("notifiers"))
      {
        reltypes.add(ObjectType.NOTIFY);
      }
      else if (typestr.equalsIgnoreCase("users"))
      {
        reltypes.add(ObjectType.USER);
      }
      else if (typestr.equalsIgnoreCase("groups"))
      {
        reltypes.add(ObjectType.USERGROUP);
      }
      else if (typestr.equalsIgnoreCase("types"))
      {
        reltypes.add(ObjectType.SERVERCOMPTYPE);
      }
      else if (typestr.equalsIgnoreCase("domains"))
      {
       reltypes.add(ObjectType.RPROXY);
      }


      for (ObjectType reltype : reltypes)
      {
    	  System.out.println("** reltype="+reltype);
        List<TreeObject> dmo = so.getTreeObjects(reltype, domainid, catid, nobuiltins);

        String uct = Character.toUpperCase(reltype.toString().charAt(0)) + reltype.toString().substring(1).toLowerCase();

        long startTime = System.nanoTime();
        for (TreeObject dm : dmo)
        {
          if (subenv) {
            out.println(",");
          }
          System.out.println("** dm.GetObjectType="+dm.getObjectType());

          boolean parentisLifecycle = false;

          /* PAG mod - no longer take parent being at the top level into account when
           * calculating whether to display application versions. New logic always
           * displays an application version under the BASE application if the BASE
           * and the version are in the same domain.
           * This logic is encoded in getApplicationContent.java which is called when
           * expanding an application BASE in the tree-view.
           *
          if (dm.GetObjectType() == ObjectType.APPLICATION)
          {
            int appid = dm.getId();
            Application app = so.getApplication(appid, true);
            Domain d2 = app.getDomain();
            parentisLifecycle = d2.getLifecycle();
          }
          */

          if (((dm.GetObjectType() == ObjectType.ENVIRONMENT)
        	|| (dm.GetObjectType() == ObjectType.NOTIFY)
        	|| ((dm.GetObjectType() == ObjectType.APPLICATION) && (!parentisLifecycle))
        	|| (dm.GetObjectType() == ObjectType.RELEASE)
        	|| (dm.GetObjectType() == ObjectType.COMPONENT))
        	&& (so.objHasChildren(dm, typestr)))
          {
            System.out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"" + dm.GetOTID() + "\", \"rel\" : \"" + uct + "\" }}");
            out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"" + dm.GetOTID() + "\", \"rel\" : \"" + uct + "\" }}");
          }
          else
          {
            ObjectType ot = dm.GetOTID().getObjectType();

            System.out.println("xx) ot="+ot+" dm.GetObjectType="+dm.GetObjectType());
            System.out.println("typestr="+typestr);

            if ((ot == ObjectType.ACTION_CATEGORY) && (typestr.equalsIgnoreCase("actions")))
            {
              hmap.put(Integer.valueOf(dm.getId()), Integer.valueOf(0));
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cy" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cy" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");

              System.out.println("]}");
              out.println("]}");
            }
            else if ((ot == ObjectType.COMP_CATEGORY) && (typestr.equalsIgnoreCase("components")))
            {
              hmap.put(Integer.valueOf(dm.getId()), Integer.valueOf(0));
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cc" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cc" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");

              System.out.println("]}");
              out.println("]}");
            }
            else if ((ot == ObjectType.PROCFUNC_CATEGORY) && (typestr.equalsIgnoreCase("procedures")))
            {
              hmap.put(Integer.valueOf(dm.getId()), Integer.valueOf(0));
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cp" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cp" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");
              System.out.println("]}");
              out.println("]}");
            }
            else if ((ot == ObjectType.FRAG_CATEGORY) && (typestr.equalsIgnoreCase("procedures")))
            {
              hmap.put(Integer.valueOf(dm.getId()), Integer.valueOf(0));
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cf" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"cf" + dm.getId() + "-" + domainid + "\", \"rel\" : \"Category\" }, \"children\" : [");
              System.out.println("]}");
              out.println("]}");
            }
            else if ((ot == ObjectType.PROCEDURE) && (typestr.equalsIgnoreCase("procedures")))
            {
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + uct + "\" }}");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + uct + "\" }}");
            }
            else if ((ot == ObjectType.FUNCTION) && (typestr.equalsIgnoreCase("procedures")))
            {
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + uct + "\" }}");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + uct + "\" }}");
            }
            else
            {
              out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"" + dm.GetOTID() + "\", \"rel\" : \"" + uct + "\" }}");
            }
          }
          subenv = true;
        }
        long endTime = System.nanoTime();
        System.out.println("Loop processing took " + (endTime - startTime) + " nanosecs");
      }
    }

    System.out.println("]");
    out.println("]");
  }

  void ProcessComponents(PrintWriter out, DMSession so, String typestr, int domainid, HashMap<Integer, Integer> hmap)
  {
    String rel = "Category";
    List<TreeObject> d = so.GetCategoriesAsTree();

    System.out.print("[");
    out.print("[");

    int cnt = d.size();
    int i = 0;

    for (TreeObject xd : d)
    {

      System.out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \"closed\", \"attr\"  : { \"id\" : \"cc" + xd.getId() + "-" + domainid + "\", \"rel\" : \"" + rel + "\" }");
      out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \"closed\", \"attr\"  : { \"id\" : \"cc" + xd.getId() + "-" + domainid + "\", \"rel\" : \"" + rel + "\" }");

      System.out.print(", \"children\": ");
      out.print(", \"children\": ");

      System.out.print("[");
      out.print("[");

      System.out.print("]");
      out.print("]");

      System.out.print("}");
      out.print("}");
      i++;
      if (i >= cnt)
        continue;
      System.out.println(",");
      out.println(",");
    }

    System.out.println("]");
    out.println("]");
  }

  void ProcessFragments(PrintWriter out, DMSession so, String typestr, int domainid, int HierarchyDomain, HashMap<Integer, Integer> hmap)
  {
    String nobuiltins = null;
    String uct;
    if (typestr.equalsIgnoreCase("fragments")) {
     uct = "Actions";
    } else {
     uct = Character.toUpperCase(typestr.charAt(0)) + typestr.substring(1).toLowerCase();
    }

    System.out.print("[");
    out.print("[");

    System.out.print("{\"data\" : \"Global " + uct + "\", \"state\" : \"closed\", \"attr\" : { \"id\" : \"do-1\", \"rel\" : \"domain\" }, \"children\": [");
    out.print("{\"data\" : \"Global " + uct + "\", \"state\" : \"closed\", \"attr\" : { \"id\" : \"do-1\", \"rel\" : \"domain\" }, \"children\": [");
    boolean nv = false;

    List<TreeObject> dmo =  so.getTreeObjects(ObjectType.PROCFUNC_CATEGORY,1,-1,nobuiltins);

    try
    {
     Domain infra = so.getDomainByName("GLOBAL.Infrastructure");
     List<TreeObject> dmi =  so.getTreeObjects(ObjectType.DOMAIN,infra.getId(),-1,nobuiltins);
     dmo.addAll(dmi);
    }
    catch (Exception e)
    {}

  //  List<TreeObject> dmc =  so.getTreeObjects(ObjectType.PROCFUNC_CATEGORY,infra.getId(),-1);
  //  dmo.addAll(dmc);

    if (dmo != null) for (TreeObject xd : dmo) {
      String rt = "";
      if (nv) System.out.println(",");
      if (nv) out.println(",");

      String domstr = "";

      if (xd.GetObjectType() == ObjectType.DOMAIN)
       rt = "Domain";
      else if (xd.GetObjectType() == ObjectType.COMPONENT)
       rt = "Component";
      else if (xd.GetObjectType() == ObjectType.FRAGMENT)
       rt = "Fragment";
      else
      {
       rt = "Category";
       domstr = "-" + xd.GetDomainId();
      }

      if (so.objHasChildren(xd, "domain") || so.objHasChildren(xd, "procedures")) {
       String state = "closed"; // typestr.equalsIgnoreCase("category")?"closed":"open";
        System.out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \""+state+"\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
        out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \""+state+"\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }, \"children\": []}");
      } else {
        System.out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
        out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
      }
      nv = true;
    }
    System.out.print("]}");
    out.print("]}");

    String state;

    System.out.println(",");
    out.println(",");
    nv = false;

    Domain dom = so.getUserDomain();
    TreeObject treeObject = new TreeObject(dom.getId(),dom.getName(), dom.getId());
    treeObject.SetObjectType(ObjectType.DOMAIN);
    dmo.clear();
    dmo.add(treeObject);

    if (dmo != null) for (TreeObject xd : dmo) {
      String rt = "";
      if (nv) System.out.println(",");
      if (nv) out.println(",");

      String domstr = "";

      if (xd.GetObjectType() == ObjectType.DOMAIN)
       rt = "Domain";
      else if (xd.GetObjectType() == ObjectType.COMPONENT)
       rt = "Component";
      else if (xd.GetObjectType() == ObjectType.FRAGMENT)
       rt = "Fragment";
      else
      {
       rt = "Category";
       domstr = "-" + xd.GetDomainId();
      }

      if (so.objHasChildren(xd, "domain") || so.objHasChildren(xd, "procedures")) {
        state = typestr.equalsIgnoreCase("category")?"closed":"open";
        System.out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \""+state+"\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
        out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \""+state+"\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }, \"children\": []}");
      } else {
        System.out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
        out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
      }
      nv = true;
    }
//    System.out.print("]}");
//    out.print("]}");

    System.out.print("]");
    out.println("]");

  }

  void ProcessDomainContentHierarchy(PrintWriter out, DMSession so, String typestr, int domainid, HashMap<Integer, Integer> hmap, String nobuiltins)
  {
	  System.out.println("ProcessDomainContentHierarchy, typestr=["+typestr+"] domainid="+domainid);
    Domain pd = domainid > 0 ? so.getDomain(domainid) : null;
    String rel = pd != null ? (pd.getLifecycle() ? "Lifecycle" : "Domain") : "Domain";

    List<TreeObject> d = so.getDomains(Integer.valueOf(domainid));

    System.out.print("[");
    out.print("[");

    boolean subenv = false;
    List<TreeObject> dmo = new ArrayList<TreeObject>();
    if ((domainid == 0) && (so.UserBaseDomain() > 1))
    {
      String rt = rel;
      dmo = null;
      if (typestr.equalsIgnoreCase("category"))
      {
        // dmo = so.getInheritedTreeObjects(ObjectType.COMPONENT, so.UserBaseDomain());
    	  dmo = so.getInheritedTreeObjects(ObjectType.COMP_CATEGORY,so.UserBaseDomain());

       System.out.println("done, dmo.size()="+dmo.size());
       rt = "Category";
      }
      String uct;
      if (typestr.equalsIgnoreCase("category")) {
       uct = "Components";
      } else {
       uct = Character.toUpperCase(typestr.charAt(0)) + typestr.substring(1).toLowerCase();
      }

      Domain infra = so.getDomainByName("GLOBAL.Infrastructure");
      List<TreeObject> ilist = so.getDomains(infra.getId());
      dmo.addAll(ilist);

      System.out.print("{\"data\" : \"Global " + uct + "\", \"state\" : \"closed\", \"attr\" : { \"id\" : \"do-1\", \"rel\" : \"domain\" }, \"children\": [");
      out.print("{\"data\" : \"Global " + uct + "\", \"state\" : \"closed\", \"attr\" : { \"id\" : \"do-1\", \"rel\" : \"domain\" }, \"children\": [");
      boolean nv = false;
      System.out.println("dmo="+dmo);
      if (dmo != null) for (TreeObject xd : dmo) {
        if (nv) System.out.println(",");
        if (nv) out.println(",");

        String domstr = "";

        if (xd.GetObjectType() == ObjectType.DOMAIN)
         rt = "Domain";
        else if (xd.GetObjectType() == ObjectType.COMPONENT)
         rt = "Component";
        else if (xd.GetObjectType() == ObjectType.FRAGMENT)
         rt = "Fragment";
        else
        {
         rt = "Category";
         domstr = "-" + xd.GetDomainId();
        }

        if (typestr.equalsIgnoreCase("category")) {
         uct = "Components";
        } else {
         uct = Character.toUpperCase(typestr.charAt(0)) + typestr.substring(1).toLowerCase();
        }

        if (so.objHasChildren(xd, typestr)) {
        	String state = typestr.equalsIgnoreCase("category")?"closed":"open";
          System.out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \""+state+"\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
          out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \""+state+"\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }, \"children\": []}");
        } else {
          System.out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
          out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + domstr + "\", \"rel\" : \"" + rt + "\" }}");
        }
        nv = true;
      }
      System.out.print("]}");
      out.print("]}");
      subenv = true;
    }
    String state;
    for (TreeObject xd : d) {
      if ((!hmap.containsKey(Integer.valueOf(xd.getId()))) && (!hmap.containsValue(Integer.valueOf(xd.getId()))) && (xd.GetObjectType() == ObjectType.DOMAIN))
      {
        continue;
      }
      rel = Character.toUpperCase(rel.charAt(0)) + rel.substring(1).toLowerCase();

      if (subenv) {
        System.out.println(",");
        out.println(",");
      }
      state = domainid == 0 ? "open" : "open";

      System.out.println("GetDomainContent xd="+xd.getName());
      String uct;
      if (typestr.equalsIgnoreCase("category")) {
       uct = "Components";
      } else {
       uct = Character.toUpperCase(typestr.charAt(0)) + typestr.substring(1).toLowerCase();
      }

      if (so.objHasChildren(xd, uct)) {
        System.out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \"" + state + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
        out.print("{\"data\" : \"" + xd.getName() + "\", \"state\" : \"" + state + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
      } else {
        System.out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
        out.print("{\"data\" : \"" + xd.getName() + "\", \"attr\" : { \"id\" : \"" + xd.GetOTID() + "\", \"rel\" : \"" + rel + "\" }");
      }

      if (state.equalsIgnoreCase("open")) {
        System.out.print(", \"children\": ");
        out.print(", \"children\": ");

        System.out.println("Recursing typestr="+typestr+" xd.getId()="+xd.getId());
        // ProcessDomainContentHierarchy(out, so, typestr, xd.getId(), hmap);
        out.println("[]");	// because above is commented out
      } else {
        System.out.print(", \"children\": ");
        out.print(", \"children\": ");

        System.out.println("{}");
        out.println("{}");
      }
      out.print("}");
      subenv = true;
    }

    List<ObjectType> reltypes = new ArrayList<ObjectType>();
    if ((domainid > 0) && (typestr != null))
    {
      if (typestr.equalsIgnoreCase("workflow")) {
        reltypes.add(ObjectType.APPLICATION);
        reltypes.add(ObjectType.APPVERSION);
      } else if (typestr.equalsIgnoreCase("environments")) {
        reltypes.add(ObjectType.ENVIRONMENT);
      } else if (typestr.equalsIgnoreCase("repositories")) {
        reltypes.add(ObjectType.REPOSITORY);
      } else if (typestr.equalsIgnoreCase("servers")) {
        reltypes.add(ObjectType.SERVER);
      } else if (typestr.equalsIgnoreCase("source")) {
        reltypes.add(ObjectType.REPOSITORY);
      } else if (typestr.equalsIgnoreCase("applications")) {
        reltypes.add(ObjectType.APPLICATION);
        reltypes.add(ObjectType.APPVERSION);
      } else if (typestr.equalsIgnoreCase("releases")) {
        reltypes.add(ObjectType.RELEASE);
      } else if (typestr.equalsIgnoreCase("actions")) {
        reltypes.add(ObjectType.ACTION_CATEGORY);
      } else if (typestr.equalsIgnoreCase("fragments")) {
        reltypes.add(ObjectType.FRAG_CATEGORY);
      } else if (typestr.equalsIgnoreCase("procedures")) {
        reltypes.add(ObjectType.PROCFUNC_CATEGORY);
      } else if (typestr.equalsIgnoreCase("functions")) {
        reltypes.add(ObjectType.PROCFUNC_CATEGORY);
      } else if (typestr.equalsIgnoreCase("components")) {
        reltypes.add(ObjectType.COMP_CATEGORY);
      } else if (typestr.equalsIgnoreCase("credentials")) {
        reltypes.add(ObjectType.CREDENTIALS);
      } else if (typestr.equalsIgnoreCase("datasources")) {
        reltypes.add(ObjectType.DATASOURCE);
      } else if (typestr.equalsIgnoreCase("notifiers")) {
        reltypes.add(ObjectType.NOTIFY);
      } else if (typestr.equalsIgnoreCase("users")) {
        reltypes.add(ObjectType.USER);
      } else if (typestr.equalsIgnoreCase("groups")) {
        reltypes.add(ObjectType.USERGROUP);
      }

      for (ObjectType reltype : reltypes) {
        System.out.println("Processing " + reltype.getTypeString()+" in domainid "+domainid);
        dmo = so.getTreeObjects(reltype, domainid, -1,nobuiltins);
        System.out.println("Done Processing " + reltype.getTypeString() + " Objects returned =" + dmo.size());

        for (TreeObject dm : dmo) {
          if ((!hmap.containsKey(Integer.valueOf(dm.getId()))) && (!hmap.containsValue(Integer.valueOf(dm.getId()))) && (dm.GetObjectType() == ObjectType.DOMAIN)) {
            continue;
          }

          String uct = Character.toUpperCase(reltype.toString().charAt(0)) + reltype.toString().substring(1).toLowerCase();

          if (subenv) out.println(",");
          if (subenv) System.out.println(",");

          System.out.println("*** here ***");

          if (((dm.GetObjectType() == ObjectType.NOTIFY) || (dm.GetObjectType() == ObjectType.APPLICATION) || (dm.GetObjectType() == ObjectType.RELEASE) || (dm.GetObjectType() == ObjectType.COMPONENT)) && (so.objHasChildren(dm, typestr))) {
            System.out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"" + dm.GetOTID() + "\", \"rel\" : \"" + uct + "\" }}");
            out.print("{\"data\" : \"" + dm.getName() + "\", \"state\":\"closed\",\"attr\" : { \"id\" : \"" + dm.GetOTID() + "\", \"rel\" : \"" + uct + "\" }}");
          } else {
            ObjectType ot = dm.GetOTID().getObjectType();
            System.out.println("ot="+ot+" typestr="+typestr);
			if (typestr.equalsIgnoreCase("procedures")) {
				switch(ot) {
	    	  	case PROCEDURE:
	    	  		if (so.objHasChildren(dm, typestr)) {
	    	  			System.out.println("PROCEDURE ");
	    	  			System.out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+uct+"\" }, \"children\": {}}");

	    	  			out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+uct+"\" }, \"children\": {}}");
	    	  		} else {
	    	  			out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+uct+"\" }}");
	    	  		}
	    	  		break;
	    	  	case FUNCTION:
	    	  		if (so.objHasChildren(dm, typestr)) {
	    	  			out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+uct+"\" }, \"children\": {}}");
	    	  		} else {
	    	  			out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() +  "\", \"rel\" : \""+uct+"\" }}");
	    	  		}
	    	  		break;
	    	  	default:
	    	  		break;
				}
			}
			else if (typestr.equalsIgnoreCase("actions")) {
				if (ot == ObjectType.ACTION) {
					System.out.println("ACTION ["+dm.getName()+"] children="+so.objHasChildren(dm, typestr));
					if (so.objHasChildren(dm, typestr)) {
						out.println("{\"data\" : \"" + dm.getName() + "\", \"state\": \"closed\", \"attr\" : { \"id\" : \"" + dm.GetOTID() +  "\", \"rel\" : \""+uct+"\" },  \"children\": {}}");
					} else {
						out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"" + dm.GetOTID() +  "\", \"rel\" : \""+uct+"\" }}");
					}
				}
			}

            /*
            if ((ot == ObjectType.PROCEDURE) && (typestr.equalsIgnoreCase("procedures"))) {
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"pr" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
            } else if ((ot == ObjectType.FUNCTION) && (typestr.equalsIgnoreCase("functions"))) {
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"fn" + dm.getId() + "-" + dm.GetObjectKind() + "\", \"rel\" : \"" + reltype + "\" }}");
            } else {
              System.out.println("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"" + dm.GetOTID() + "\", \"rel\" : \"" + reltype + "\" }}");
              out.print("{\"data\" : \"" + dm.getName() + "\", \"attr\" : { \"id\" : \"" + dm.GetOTID() + "\", \"rel\" : \"" + reltype + "\" }}");
            }
            */
          }
          subenv = true;
        }
      }
      System.out.println("End of reltypes list processing");
    }
    System.out.print("]");
    out.println("]");
  }

  public void handleRequest(DMSession so, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
  {
   try (DMSessionPro session = new DMSessionPro(so))
   {
    long startTime = System.nanoTime();

    PrintWriter out = response.getWriter();

    String domainid = request.getParameter("domainid");
    String typestr = request.getParameter("typestr");
    String seldom = request.getParameter("seldom");
    String hierarchy = request.getParameter("hierarchy");
    String nobuiltins = request.getParameter("nobuiltins");

    System.out.println("typestr=[" + typestr + "]");
    if (typestr.equalsIgnoreCase("dh")) {
      response.setContentType("text/plain");

      String otid = request.getParameter("otid");
      String calid = request.getParameter("calid");
      if (calid != null)
      {
        DMCalendarEvent cal = session.getCalendarEvent(Integer.parseInt(calid));
        int st = cal.getStart();
        otid = "en" + cal.getEnvID();
        out.print(st + "," + otid + ",");
      }
      String res = session.getParentDomainsForObject(otid);
      out.println(res);
    } else {
      response.setContentType("application/json");
      int SelectedDomain = 0;
      if (seldom != null) {
        SelectedDomain = Integer.parseInt(seldom);
      }
      int HierarchyDomain = 0;
      if (hierarchy != null && !hierarchy.equalsIgnoreCase("undefined")) {
        HierarchyDomain = Integer.parseInt(hierarchy);
      }
      HashMap<Integer,Integer> hmap = new HashMap<Integer,Integer>();

      System.out.println("GetDomainContent, HierarchyDomain=" + HierarchyDomain + " typestr=" + typestr);

      if (HierarchyDomain > 0)
      {
        if (typestr.equalsIgnoreCase("fragments"))
        {
          ProcessFragments(out, session, typestr, Integer.parseInt(domainid), HierarchyDomain, hmap);
        }
        else
        {
          Domain sd = session.getDomain(HierarchyDomain);
          while ((sd != null) && (sd.getId() > 0))
          {
            hmap.put(Integer.valueOf(sd.getDomainId()), Integer.valueOf(sd.getId()));
            sd = session.getDomain(sd.getDomainId());
          }
          typestr="Category";
          ProcessDomainContentHierarchy(out, session, typestr, Integer.parseInt(domainid), hmap, nobuiltins);
        }
      } else {
        if (typestr.equalsIgnoreCase("fragments"))
         typestr = "procedures";

        if (SelectedDomain > 0)
        {
          Domain sd = session.getDomain(SelectedDomain);
          while ((sd != null) && (sd.getId() != session.UserBaseDomain()) && (sd.getId() > 0))
          {
            hmap.put(Integer.valueOf(sd.getDomainId()), Integer.valueOf(sd.getId()));
            sd = session.getDomain(sd.getDomainId());
          }
        }
        if (typestr.equalsIgnoreCase("procedures"))
        {
          ProcessDomainContent(out, session, typestr, Integer.parseInt(domainid), -1, hmap, nobuiltins);
        }
        else
        {
        	System.out.println("** ProcessDomainContent typestr="+typestr+" domainid="+domainid);
          ProcessDomainContent(out, session, typestr, Integer.parseInt(domainid), -1, hmap, nobuiltins);
        }
      }
      out.flush();
    }
    long endTime = System.nanoTime();
    System.out.println("GetDomainContent exits, total time taken =" + (endTime - startTime) + " nanosecs");
  }
 }
}
