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
// import java.io.PrintWriter;
import java.io.PrintWriter;
import java.sql.SQLException;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Application;
import dmadmin.model.Component;
import dmadmin.model.ComponentItem;
import dmadmin.model.FragmentDetails;

/**
 * Servlet implementation class UpdateAvailability
 */
public class UpdateAttrs extends HttpServletBase
{

 boolean isRelease = false;

 private int GetPosition(HttpServletRequest request, String pname)
 {
  String szPos = request.getParameter(pname);
  System.out.println(pname + "=" + szPos);
  int c;
  if ((c = szPos.indexOf('.')) > -1)
  {
   szPos = szPos.substring(0, c);
  }
  int res = Integer.parseInt(szPos);
  int rem = res % 20;
  rem = rem <= 10 ? -rem : 20 - rem;
  res = res + rem;
  System.out.println("Returning " + res);
  return res;
 }

 @SuppressWarnings("rawtypes")
 private void SaveDetails(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int actionid = Integer.parseInt(request.getParameter("a"));
  int windowid = Integer.parseInt(request.getParameter("w"));
  String fragtype = so.getFragmentType(actionid, windowid);
  System.out.println("actionid=" + actionid + " windowid=" + windowid);
  String title = request.getParameter("title");
  if (title.equalsIgnoreCase(""))
   title = fragtype;
  String summary = request.getParameter("summary");
  System.out.println("title=" + title + " summary=" + summary);
  Enumeration keys = request.getParameterNames();
  Map<String, String> keyvals;
  keyvals = new HashMap<String, String>();
  while (keys.hasMoreElements())
  {
   String key = (String) keys.nextElement();
   System.out.println("key=" + key);
   String val = request.getParameter(key);
   System.out.println("val=" + val);
   keyvals.put(key, val);
  }
  so.UpdateFragAttrs(keyvals);
  System.out.println("{\"title\": \"" + title + "\", \"summary\": \"" + summary + "\"}");
  out.println("{\"title\": \"" + title + "\", \"summary\": \"" + summary + "\"}");
 }

 private void WindowMoved(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  
  int actionid = Integer.parseInt(request.getParameter("a"));
  int windowid = Integer.parseInt(request.getParameter("w"));
  String spw = request.getParameter("pw");
  int pw = (spw != null && spw.length() > 0) ? Integer.parseInt(spw) : 0;
  System.out.println("WindowMoved windowid="+windowid+" pw="+pw+" spw="+spw);
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  int typeid = Integer.parseInt(request.getParameter("typeid"));
  so.MoveNode(actionid, windowid, pw, xpos, ypos, typeid);
  out.println("{\"success\": true}");
 }

 private void ComponentStartMoved(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int xpos = GetPosition(request, "xpos");
  so.MoveAppComponentStart(appid, xpos);
  out.println("{\"success\": true}");
 }

 private void ServerMoved(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int envid = Integer.parseInt(request.getParameter("e"));
  int serverid = Integer.parseInt(request.getParameter("s"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  so.MoveServer(envid, serverid, xpos, ypos);
  out.println("{\"success\": true}");
 }
 
 private void addServerComplete(HttpServletRequest request, DMSession so, PrintWriter out)
 {
	 int envid = Integer.parseInt(request.getParameter("e"));
	 String sl = request.getParameter("sl");
	 so.NotifyServersAddedToEnvironment(envid,sl);
 }

 private void applicationVersionMoved(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int verid = Integer.parseInt(request.getParameter("v"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  so.applicationMoveVersion(appid, verid, xpos, ypos);
  out.println("{\"success\": true}");
 }

 private void applicationNewVersion(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  int verid = so.applicationNewVersion(appid, xpos, ypos, isRelease);
  Application app = so.getApplication(verid, false);
  out.println("{\"verid\": \"" + verid + "\", \"vername\": \"" + app.getName() + "\"}");
 }

 private void componentVersionMoved(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int verid = Integer.parseInt(request.getParameter("v"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  so.componentMoveVersion(compid, verid, xpos, ypos);
  out.println("{\"success\": true}");
 }

 private void componentItemMoved(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int itemid = Integer.parseInt(request.getParameter("ci"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  so.componentItemMoveItem(compid, itemid, xpos, ypos);
  out.println("{\"success\": true}");
 }

 private void componentApplicationMoved(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int appid = Integer.parseInt(request.getParameter("a"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  so.addComponentToApplication(appid, compid, xpos, ypos, isRelease);
  out.println("{\"compid\": \"" + compid + "\", \"appid\": \"" + appid + "\"}"); // return some JSON so that the result fires
 }

 private void componentServerAdded(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int servid = Integer.parseInt(request.getParameter("s"));
  so.addComponentToServer(servid, compid);
  out.println("{\"success\": true}");
 }

 private void componentVersionServerAdded(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int servid = Integer.parseInt(request.getParameter("s"));
   
  so.addComponentVersionToServer(servid, compid);
  out.println("{\"success\": true}");
 }

 private void componentNewVersion(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  int verid = so.componentNewVersion(compid, xpos, ypos);
  Component comp = so.getComponent(verid, false);
  out.println("{\"verid\": \"" + verid + "\", \"vername\": \"" + comp.getName() + "\"}");
 }

 private void componentItemNewItem(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  String kind = request.getParameter("kind");
  String removeall = request.getParameter("removeall");
 
  if (removeall != null && removeall.equalsIgnoreCase("Y"))
  {
   try
   {
    so.DeleteComponentItems(compid);
   }
   catch (SQLException e)
   {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }
  int itemid = so.componentItemNewItem(compid, xpos, ypos,kind);
  ComponentItem compitem = so.getComponentItem(itemid, false);
  out.println("{\"itemid\": \"" + itemid + "\", \"itemname\": \"" + compitem.getName() + "\"}");
 }

 private void applicationNewComponent(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  int compid = so.applicationNewComponent(appid, xpos, ypos, isRelease);
  Component comp = so.getComponent(compid, false);
  out.println("{\"compid\": \"" + compid + "\", \"compname\": \"" + comp.getName() + "\"}");
 }

 private void serverNewComponent(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int servid = Integer.parseInt(request.getParameter("s"));
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  int compid = so.serverNewComponent(servid, xpos, ypos);
  Component comp = so.getComponent(compid, false);
  out.println("{\"compid\": \"" + compid + "\", \"compname\": \"" + comp.getName() + "\"}");
 }

 // Redundant - see componentApplicationMoved
 // private void applicationAddComponent(HttpServletRequest request,DMSession so,PrintWriter out)
 // {
 // int appid = Integer.parseInt(request.getParameter("a"));
 // int xpos = GetPosition(request,"xpos");
 // int ypos = GetPosition(request,"ypos");
 // int compid = Integer.parseInt(request.getParameter("c"));
 // so.addComponentToApplication(appid,compid,xpos,ypos);
 // /*Component comp =*/ so.getComponent(compid,false);
 // }

 // Redundant - see componentServerMoved
 // private void serverAddComponent(HttpServletRequest request,DMSession so,PrintWriter out)
 // {
 // int compid = Integer.parseInt(request.getParameter("c"));
 // int servid = Integer.parseInt(request.getParameter("s"));
 // int xpos = GetPosition(request,"xpos");
 // int ypos = GetPosition(request,"ypos");
 // so.addComponentToServer(servid,compid,xpos,ypos);
 // }

 private void EnterEditMode(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int actionid = Integer.parseInt(request.getParameter("a"));
  so.EnterEditMode(actionid);
  out.println("{\"success\": true}");
 }

 private void LeaveEditMode(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int actionid = Integer.parseInt(request.getParameter("a"));
  so.LeaveEditMode(actionid);
  out.println("{\"success\": true}");
 }

 private void AddFlow(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int actionid = Integer.parseInt(request.getParameter("a"));
  String fn = request.getParameter("fn");
  String tn = request.getParameter("tn");
  System.out.println("AddFlow fn=" + fn + " tn=" + tn + " actionid=" + actionid);
  int pos = Integer.parseInt(request.getParameter("pos"));
  if (fn.compareToIgnoreCase("start") == 0)
   fn = "0";
  if (tn.compareToIgnoreCase("start") == 0)
   tn = "0";
  so.AddFlow(actionid, fn, tn, pos);
  out.println("{\"success\": true}");
 }

 private void DeleteFlow(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int actionid = Integer.parseInt(request.getParameter("a"));
  String fn = request.getParameter("fn");
  String tn = request.getParameter("tn");
  so.DeleteFlow(actionid, fn, tn);
  out.println("{\"success\": true}");
 }

 private void AddConnector(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int envid = Integer.parseInt(request.getParameter("e"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  int fromside = Integer.parseInt(request.getParameter("fs"));
  int toside = Integer.parseInt(request.getParameter("ts"));
  so.AddConnector(envid, fromnode, tonode, fromside, toside);
  out.println("{\"success\": true}");
 }

 private void applicationAddDependency(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  so.applicationAddVersionDependency(appid, fromnode, tonode);
  out.println("{\"success\": true}");
 }

 private void componentAddDependency(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));							// remove old component items
  so.componentAddVersionDependency(compid, fromnode, tonode);	// add new component items from new predecessor
  out.println("{\"success\": true}");
 }

 private void componentItemAddLink(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  so.componentItemAddLink(compid, fromnode, tonode);
  out.println("{\"success\": true}");
 }

 private void applicationComponentAddLink(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  so.applicationComponentAddLink(appid, fromnode, tonode);
  out.println("{\"success\": true}");
 }

 private void applicationComponentDeleteLink(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  so.applicationComponentDeleteLink(appid, fromnode, tonode);
  out.println("{\"success\": true}");
 }

 private void applicationReplaceComponent(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int fromcompid = Integer.parseInt(request.getParameter("c"));
  int tocompid = Integer.parseInt(request.getParameter("nc"));
  Component comp = so.applicationReplaceComponent(appid, fromcompid, tocompid, isRelease);
  String prefix = (comp.getParentId()>0)?"cv":"co";	// Component Version/Base
  out.println("{\"success\": true, \"title\": \"" + comp.getName() + "\", \"prefix\": \""+prefix+"\", \"summary\": \""+ comp.getSummary() + "\" }");
 }

 private void DeleteConnector(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int envid = Integer.parseInt(request.getParameter("e"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  int fromside = Integer.parseInt(request.getParameter("fs"));
  int toside = Integer.parseInt(request.getParameter("ts"));
  so.DeleteConnector(envid, fromnode, tonode, fromside, toside);
  out.println("{\"success\": true}");
 }

 private void applicationDeleteDependency(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  System.out.println("DeleteDependency, appid=" + appid + " fromnode=" + fromnode + " tonode=" + tonode);
  so.applicationDeleteVersionDependency(appid, fromnode, tonode);
  out.println("{\"success\": true}");
 }

 private void componentDeleteDependency(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  System.out.println("DeleteDependency, appid=" + compid + " fromnode=" + fromnode + " tonode=" + tonode);
  so.componentDeleteVersionDependency(compid, fromnode, tonode);
  out.println("{\"success\": true}");
 }

 private void componentItemDeleteLink(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int fromnode = Integer.parseInt(request.getParameter("fn"));
  int tonode = Integer.parseInt(request.getParameter("tn"));
  System.out.println("delete component item link, compid=" + compid + " fromnode=" + fromnode + " tonode=" + tonode);
  so.componentItemDeleteLink(compid, fromnode, tonode);
  out.println("{\"success\": true}");
 }

 private void applicationSaveVersionInfo(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  String Name = request.getParameter("title");
  String Summary = request.getParameter("summary");
  System.out.println("SaveVersionInfo Name=[" + Name + "] Summary=[" + Summary + "]");
  so.applicationModifyVersion(appid, Name, Summary);
  out.println("{\"title\": \"" + Name + "\", \"summary\": \"" + Summary + "\"}");
 }

 private void componentSaveVersionInfo(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  String Name = request.getParameter("title");
  String Summary = request.getParameter("summary");
  System.out.println("SaveVersionInfo Name=[" + Name + "] Summary=[" + Summary + "]");
  so.componentModifyVersion(compid, Name, Summary);
  Component comp = so.getComponent(compid,false);
  String prefix = (comp.getParentId()>0)?"cv":"co";
  out.println("{\"title\": \"" + Name + "\", \"summary\": \"" + Summary + "\", \"prefix\": \""+prefix+"\"}");
 }

 private void componentItemSaveItemInfo(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int ccid = Integer.parseInt(request.getParameter("ci"));
  String Name = request.getParameter("title");
  String Summary = request.getParameter("summary");
  System.out.println("componentItemSaveItemInfo Name=[" + Name + "] Summary=[" + Summary + "]");
  so.componentItemModifyItem(ccid, Name, Summary);
  out.println("{\"title\": \"" + Name + "\", \"summary\": \"" + Summary + "\"}");
 }

 private void serverSaveDetails(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int servid = Integer.parseInt(request.getParameter("s"));
  String Name = request.getParameter("title");
  String Summary = request.getParameter("summary");
  System.out.println("serverSaveDetails Name=[" + Name + "] Summary=[" + Summary + "]");
  so.serverSaveSummary(servid, Name, Summary);
  out.println("{\"title\": \"" + Name + "\", \"summary\": \"" + Summary + "\"}");
 }

 private void AddNode(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int actionid = Integer.parseInt(request.getParameter("a"));
  int windowid = Integer.parseInt(request.getParameter("w"));
  String spw = request.getParameter("pw");
  int pw = (spw.length() > 0) ? Integer.parseInt(spw) : 0;
  int xpos = GetPosition(request, "xpos");
  int ypos = GetPosition(request, "ypos");
  int typeid = Integer.parseInt(request.getParameter("typeid"));
  String otid = so.MoveNode(actionid, windowid, pw, xpos, ypos, typeid);
  FragmentDetails fd = so.getFragmentDetails(actionid, windowid);
  int style=0;
  if (fd.getExitPoints()>1) {
	  // If blocks have different styles (true/false output flows)
	  if (fd.getTypeName().equalsIgnoreCase("if")) style=1;
  }
  out.println("{\"success\": true, \"otid\": \""+otid+"\", \"exitpoints\": \""+fd.getExitPoints()+"\", \"drilldown\": "+fd.isDrilldown()+", \"style\": \""+style+"\"}");
 }

 private void DeleteNode(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int actionid = Integer.parseInt(request.getParameter("a"));
  int windowid = Integer.parseInt(request.getParameter("w"));
  so.DeleteNode(actionid, windowid);
  out.println("{\"success\": true}");
 }

 private void DeleteServer(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int envid = Integer.parseInt(request.getParameter("e"));
  int windowid = Integer.parseInt(request.getParameter("w"));
  so.RemoveServerFromEnvironment(envid, windowid);
  out.println("{\"success\": true}");
 }

 private void applicationDeleteVersion(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int verid = Integer.parseInt(request.getParameter("v"));
  so.applicationRemoveVersion(appid, verid);
  out.println("{\"success\": true}");
 }

 private void componentDeleteVersion(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int verid = Integer.parseInt(request.getParameter("v"));
  so.componentRemoveVersion(compid, verid);
  out.println("{\"success\": true}");
 }

 private void componentItemDeleteItem(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int compid = Integer.parseInt(request.getParameter("c"));
  int itemid = Integer.parseInt(request.getParameter("ci"));
  so.componentItemRemoveItem(compid, itemid);
  out.println("{\"success\": true}");
 }

 private void applicationComponentDelete(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int appid = Integer.parseInt(request.getParameter("a"));
  int compid = Integer.parseInt(request.getParameter("c"));
  so.applicationRemoveComponent(appid, compid, isRelease);
  out.println("{\"success\": true}");
 }

 private void serverComponentDelete(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int servid = Integer.parseInt(request.getParameter("s"));
  int compid = Integer.parseInt(request.getParameter("c"));
  so.serverRemoveComponent(servid, compid);
  out.println("{\"success\": true}");
 }

 private void updateTaskCopy(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int tid = Integer.parseInt(request.getParameter("tid"));
  int tgt = Integer.parseInt(request.getParameter("tgt"));
  System.out.println("updateTaskCopy, tid=" + tid + " tgt=" + tgt);
  so.updateTaskCopy(tid, tgt);
  out.println("{\"success\": true}");
 }

 private void updateTaskMove(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int tid = Integer.parseInt(request.getParameter("tid"));
  int tgt = Integer.parseInt(request.getParameter("tgt"));
  System.out.println("updateTaskMove, tid=" + tid + " tgt=" + tgt);
  so.updateTaskMove(tid, tgt);
  out.println("{\"success\": true}");
 }

 private void updateTaskApprove(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int tid = Integer.parseInt(request.getParameter("tid"));
  int tgt = Integer.parseInt(request.getParameter("tgt"));
  System.out.println("updateTaskApprove, tid=" + tid + " tgt=" + tgt);
  so.updateTaskApprove(tid, tgt);
  out.println("{\"success\": true}");
 }

 private void updateTaskRequest(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int tid = Integer.parseInt(request.getParameter("tid"));
  int tgt = Integer.parseInt(request.getParameter("tgt"));
  System.out.println("updateTaskRequest, tid=" + tid + " tgt=" + tgt);
  so.updateTaskRequest(tid, tgt);
  out.println("{\"success\": true}");
 }
 
 private void updateTaskAction(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int tid = Integer.parseInt(request.getParameter("tid"));
  int tgt = Integer.parseInt(request.getParameter("tgt"));
  System.out.println("updateTaskAction, tid=" + tid + " tgt=" + tgt);
  so.updateTaskAction(tid, tgt);
  out.println("{\"success\": true}");
 }

 private void updateTaskCreateVersion(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int tid = Integer.parseInt(request.getParameter("tid"));
  int tgt = Integer.parseInt(request.getParameter("tgt"));
  System.out.println("updateTaskCreateVersion, tid=" + tid + " tgt=" + tgt);
  so.updateTaskCreateVersion(tid, tgt);
  out.println("{\"success\": true}");
 }

 private void updateBranchLabel(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  String t = request.getParameter("t");
  int a = Integer.parseInt(request.getParameter("a"));
  String bn = request.getParameter("bn");
  System.out.println("updateBranchLabel, a=" + a + "t="+t+" bn=" + bn);
  so.updateBranchLabel(a, t, bn);
  out.println("{\"success\": true}");
 }

 private void updateServerLabel(HttpServletRequest request, DMSession so, PrintWriter out)
 {
  int envid = Integer.parseInt(request.getParameter("envid"));
  int fromid = Integer.parseInt(request.getParameter("fid"));
  int toid = Integer.parseInt(request.getParameter("tid"));
  int fromside = Integer.parseInt(request.getParameter("fs"));
  int toside = Integer.parseInt(request.getParameter("ts"));
  String LabelName = request.getParameter("ln");
  so.updateServerLabel(envid, fromid, toid, fromside, toside, LabelName);
  out.println("{\"success\": true}");
 }

 private static final long serialVersionUID = 1L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public UpdateAttrs()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 // protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {

  response.setContentType("application/json");
  PrintWriter out = response.getWriter();

  String f = request.getParameter("f");
  if (request.getParameter("isRelease") != null && request.getParameter("isRelease").equalsIgnoreCase("Y"))
   isRelease = true;
  else
   isRelease = false;

  System.out.println("f=" + f);

  if (f.equalsIgnoreCase("ee"))
   EnterEditMode(request, session, out);
  else if (f.equalsIgnoreCase("le"))
   LeaveEditMode(request, session, out);
  else if (f.equalsIgnoreCase("wm"))
   WindowMoved(request, session, out);
  else if (f.equalsIgnoreCase("af"))
   AddFlow(request, session, out);
  else if (f.equalsIgnoreCase("df"))
   DeleteFlow(request, session, out);
  else if (f.equalsIgnoreCase("an"))
   AddNode(request, session, out);
  else if (f.equalsIgnoreCase("dn"))
   DeleteNode(request, session, out);
  else if (f.equalsIgnoreCase("sd"))
   SaveDetails(request, session, out);
  else if (f.equalsIgnoreCase("sm"))
   ServerMoved(request, session, out);
  else if (f.equalsIgnoreCase("ds"))
   DeleteServer(request, session, out);
  else if (f.equalsIgnoreCase("ac"))
   AddConnector(request, session, out);
  else if (f.equalsIgnoreCase("dc"))
   DeleteConnector(request, session, out);
  else if (f.equalsIgnoreCase("vm"))
   applicationVersionMoved(request, session, out);
  else if (f.equalsIgnoreCase("nv"))
   applicationNewVersion(request, session, out);
  else if (f.equalsIgnoreCase("dv"))
   applicationDeleteVersion(request, session, out);
  else if (f.equalsIgnoreCase("ad"))
   applicationAddDependency(request, session, out);
  else if (f.equalsIgnoreCase("dd"))
   applicationDeleteDependency(request, session, out);
  else if (f.equalsIgnoreCase("sv"))
   applicationSaveVersionInfo(request, session, out);
  else if (f.equalsIgnoreCase("cvm"))
   componentVersionMoved(request, session, out);
  else if (f.equalsIgnoreCase("cnv"))
   componentNewVersion(request, session, out);
  else if (f.equalsIgnoreCase("cdv"))
   componentDeleteVersion(request, session, out);
  else if (f.equalsIgnoreCase("cad"))
   componentAddDependency(request, session, out);
  else if (f.equalsIgnoreCase("cdd"))
   componentDeleteDependency(request, session, out);
  else if (f.equalsIgnoreCase("csv"))
   componentSaveVersionInfo(request, session, out);
  else if (f.equalsIgnoreCase("ivm"))
   componentItemMoved(request, session, out);
  else if (f.equalsIgnoreCase("inv"))
   componentItemNewItem(request, session, out);
  else if (f.equalsIgnoreCase("idv"))
   componentItemDeleteItem(request, session, out);
  else if (f.equalsIgnoreCase("iad"))
   componentItemAddLink(request, session, out);
  else if (f.equalsIgnoreCase("idd"))
   componentItemDeleteLink(request, session, out);
  else if (f.equalsIgnoreCase("isv"))
   componentItemSaveItemInfo(request, session, out);
  else if (f.equalsIgnoreCase("acvm"))
   componentApplicationMoved(request, session, out);
  else if (f.equalsIgnoreCase("scvm"))
   componentServerAdded(request, session, out);
  else if (f.equalsIgnoreCase("scam"))
   componentVersionServerAdded(request, session, out);
  else if (f.equalsIgnoreCase("acd"))
   applicationComponentDelete(request, session, out);
  else if (f.equalsIgnoreCase("scd"))
   serverComponentDelete(request, session, out);
  else if (f.equalsIgnoreCase("cal"))
   applicationComponentAddLink(request, session, out);
  else if (f.equalsIgnoreCase("cadl"))
   applicationComponentDeleteLink(request, session, out);
  else if (f.equalsIgnoreCase("anc"))
   applicationNewComponent(request, session, out); // right-click "new component" - creates a new component
  else if (f.equalsIgnoreCase("snc"))
   serverNewComponent(request, session, out);
  else if (f.equalsIgnoreCase("sssd"))
   serverSaveDetails(request, session, out);
  else if (f.equalsIgnoreCase("cswm"))
   ComponentStartMoved(request, session, out);
  else if (f.equalsIgnoreCase("ccv"))
   applicationReplaceComponent(request, session, out);
  else if (f.equalsIgnoreCase("utcp"))
   updateTaskCopy(request, session, out);
  else if (f.equalsIgnoreCase("utmp"))
   updateTaskMove(request, session, out);
  else if (f.equalsIgnoreCase("utap"))
   updateTaskApprove(request, session, out);
  else if (f.equalsIgnoreCase("utrp"))
   updateTaskRequest(request, session, out);
  else if (f.equalsIgnoreCase("utat"))
   updateTaskAction(request, session, out);
  else if (f.equalsIgnoreCase("utcv"))
   updateTaskCreateVersion(request, session, out);
  else if (f.equalsIgnoreCase("ubl"))
   updateBranchLabel(request, session, out);
  else if (f.equalsIgnoreCase("usl"))
   updateServerLabel(request, session, out);
  else if (f.equalsIgnoreCase("asc"))
   addServerComplete(request, session, out);
 
 }

 /**
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  // TODO Auto-generated method stub
  System.out.println("in doPost");

 }
}
