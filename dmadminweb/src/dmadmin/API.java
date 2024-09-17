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

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.DomainLoadStoreParameter;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SignatureException;
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Base64;
import java.util.Comparator;
import java.util.Date;
import java.util.Enumeration;
import java.util.Formatter;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.UUID;
import java.util.stream.Collectors;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.commons.lang3.StringUtils;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonPrimitive;

import dmadmin.API.ApiException;
// import oracle.net.aso.i;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.json.LinkField;
import dmadmin.model.Action;
import dmadmin.model.Application;
import dmadmin.model.Category;
import dmadmin.model.CompType;
import dmadmin.model.Component;
import dmadmin.model.ComponentItem;
import dmadmin.model.ComponentLink;
import dmadmin.model.Credential;
import dmadmin.model.CredentialKind;
import dmadmin.model.DMAttribute;
import dmadmin.model.DMObject;
import dmadmin.model.DMProperty;
import dmadmin.model.Datasource;
import dmadmin.model.DeployedApplication;
import dmadmin.model.Deployment;
import dmadmin.model.Deployment.DeploymentLogEntry;
import dmadmin.model.Domain;
import dmadmin.model.Engine;
import dmadmin.model.Environment;
import dmadmin.model.FragmentAttributes;
import dmadmin.model.GetObjectException;
import dmadmin.model.JWTGenerateValidateRSA;
import dmadmin.model.LoginException.LoginExceptionType;
import dmadmin.model.Notify;
import dmadmin.model.NotifyTemplate;
import dmadmin.model.ProviderDefinition;
import dmadmin.model.ProviderObject;
import dmadmin.model.Repository;
import dmadmin.model.SaasClient;
import dmadmin.model.Server;
import dmadmin.model.ServerType;
import dmadmin.model.Task;
import dmadmin.model.Task.TaskType;
import dmadmin.pro.DMSessionPro;
import dmadmin.model.TaskApprove;
import dmadmin.model.TaskCreateVersion;
import dmadmin.model.TaskDeploy;
import dmadmin.model.Transfer;
import dmadmin.model.User;
import dmadmin.model.UserGroup;
import dmadmin.model.UserGroupList;
import dmadmin.model.UserList;
import dmadmin.util.CommandLine;
import dmadmin.util.SendMail;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jws;

/**
 * API redirector servlet - all calls to the RESTful API are routed through here.
 */
public class API extends HttpServlet
{
 private static final long serialVersionUID = 1L;

 public class ApiException extends Exception
 {
  private static final long serialVersionUID = 1L;

  public ApiException(String msg)
  {
   super(msg);
  }
 }

 /**
  * @see HttpServlet#HttpServlet()
  */
 public API()
 {
  super();
 }

 private JSONObject assembleJSONForAppApprovals(DMSession so, Application app)
 {
  return so.applicationApprovalDomains(app);
 }

 private JSONObject assembleJSONForComponentItem(DMSession so, ComponentItem compitem)
 {
  JSONObject je = new JSONObject();
  je.add("id", compitem.getId());
  je.add("name", compitem.getName());
  return je;
 }

 private JSONObject exportJSONForApplication(DMSession so, Application app)
 {
  JSONObject je = assembleJSONForAppApprovals(so, app);
  je.add("name", app.getFullName());
  if (app.getDomain() != null)
   je.add("domain", app.getDomain().getFullDomain());
  else
   je.add("domain", "");
  je.add("branch", app.getLabel(true));
  je.add("lastcompver", so.getLastComp4App(app.getId()));

  DMObject owner = app.getOwner();
  if (owner != null)
  {
   switch (owner.getObjectType())
   {
    case USERGROUP:
     je.add("ownergroup", owner.getName());
     break;
    case USER:
     je.add("owneruser", owner.getName());
     break;
    default:
     break;
   }
  }
  je.add("summary", app.getSummary());
  //
  // If this is a base version list the versions
  //
  System.out.println("getId()=" + app.getId());
  System.out.println("getParentId()=" + app.getParentId());
  if (app.getParentId() == 0)
  {
   // This is a base application - list versions
   JSONArray verlist = new JSONArray();
   List<Application> versions = app.getVersions();
   if (versions != null)
   {
    for (Application v : versions)
    {
     JSONObject vobj = new JSONObject();
     vobj.add("name", v.getFullName());
     vobj.add("summary", v.getSummary());
     verlist.add(vobj);
    }
    je.add("versions", verlist);
   }
  }
  else
  {
   // This is a version - list predecessor
   JSONObject pred = new JSONObject();
   Application p = so.getApplication(app.getPredecessorId(), true);
   pred.add("name", p.getFullName());
   pred.add("summary", p.getSummary());
   je.add("predecessor", pred);
  }
  // List the application components
  List<Component> comps = so.getComponents(ObjectType.APPLICATION, app.getId(), false);
  JSONArray complist = new JSONArray();
  for (Component c : comps)
  {
   JSONObject co = new JSONObject();
   co.add("name", c.getFullName());
   co.add("summary", c.getSummary());
   co.add("xpos", c.getXpos());
   co.add("ypos", c.getYpos());
   System.out.println("component domain is " + c.getDomainId());
   co.add("lastbuild", c.getLastBuildNumber());
   complist.add(co);
  }
  je.add("components", complist);

  List<DMAttribute> attrs = app.getAttributes();
  JSONArray arr = new JSONArray();

  if (attrs != null)
  {
   for (int i=0;i<attrs.size();i++)
    arr.add(attrs.get(i).toJSON());
  }
  je.add("attrs", arr);
  return je;
 }

 private JSONObject exportJSONForAction(DMSession so, Action act)
 {
  JSONObject je = new JSONObject();
  je.add("name", act.getFullName());

  DMObject owner = act.getOwner();
  if (owner != null)
  {
   switch (owner.getObjectType())
   {
    case USERGROUP:
     je.add("ownergroup", owner.getName());
     break;
    case USER:
     je.add("owneruser", owner.getName());
     break;
    default:
     break;
   }
  }
  je.add("summary", act.getSummary());
  je.add("category", act.getCategory().getDomain().getFullDomain() + "." +  act.getCategory().getName());

  return je;
 }

 private JSONObject exportJSONForComponent(DMSession so, Component comp)
 {
  JSONObject je = new JSONObject();
  je.add("name", comp.getFullName());
  if (comp.getDomain() != null)
   je.add("domain", comp.getDomain().getFullDomain());
  else
   je.add("domain", "");
  je.add("branch", comp.getLabel(true));

  DMObject owner = comp.getOwner();
  if (owner != null)
  {
   switch (owner.getObjectType())
   {
    case USERGROUP:
     je.add("ownergroup", owner.getName());
     break;
    case USER:
     je.add("owneruser", owner.getName());
     break;
    default:
     break;
   }
  }
  je.add("summary", comp.getSummary());
  je.add("lastbuild", comp.getLastBuildNumber());
  //
  // If this is a base version list the versions
  //
  System.out.println("getId()=" + comp.getId());
  System.out.println("getParentId()=" + comp.getParentId());
  if (comp.getParentId() == 0)
  {
   // This is a base component - list versions
   System.out.println("getting versions");
   JSONArray verlist = new JSONArray();
   List<Component> versions = comp.getVersions();
   if (versions != null)
   {
    System.out.println("versions is set");
    for (Component v : versions)
    {
     JSONObject vobj = new JSONObject();
     vobj.add("name", v.getFullName());
     vobj.add("summary", v.getSummary());
     verlist.add(vobj);
    }
    je.add("versions", verlist);
   }
   System.out.println("end of versions list");
  }
  else
  {
   // This is a version - list predecessor
   JSONObject pred = new JSONObject();
   Component p = so.getComponent(comp.getPredecessorId(), true);
   pred.add("name", p.getFullName());
   pred.add("summary", p.getSummary());
   je.add("predecessor", pred);
  }

  List<DMAttribute> attrs = comp.getAttributes();
  JSONArray arr = new JSONArray();

  if (attrs != null)
  {
   for (int i=0;i<attrs.size();i++)
    arr.add(attrs.get(i).toJSON());
  }
  je.add("attrs", arr);

  List<ComponentItem> compitemlist = so.getComponentItems(comp.getId());
  arr = new JSONArray();

  if (compitemlist  != null)
  {
   for (int i=0;i<compitemlist.size();i++)
   {
    arr.add(compitemlist.get(i).toJSON());
   }
  }
  je.add("componentitems", arr);

  return je;
 }

 private JSONObject assembleJSONForComponent(DMSession so, Component comp, boolean idonly)
 {
  JSONObject je = new JSONObject();
  je.add("id", comp.getId());
  je.add("name", comp.getName());
  je.add("branch", comp.getLabel(true));
  Domain dom = comp.getDomain();
  if (dom != null)
  {
   je.add("domain", dom.getFullDomain());
  }

  DMObject owner = comp.getOwner();
  if (owner != null)
  {
   switch (owner.getObjectType())
   {
    case USERGROUP:
     je.add("ownergroup", owner.getName());
     break;
    case USER:
     je.add("owneruser", owner.getName());
     break;
    default:
     break;
   }
  }
  je.add("summary", comp.getSummary());
  je.add("lastbuild", comp.getLastBuildNumber());
  je.add("gitcommit", so.getComponentGitCommit(comp.getId()));
  //
  // If this is a base version list the versions
  //
  System.out.println("getId()=" + comp.getId());
  System.out.println("getParentId()=" + comp.getParentId());
  if (comp.getParentId() == 0)
  {
   // This is a base component - list versions
   System.out.println("getting versions");
   JSONArray verlist = new JSONArray();
   List<Component> versions = comp.getVersions();
   if (versions != null)
   {
    System.out.println("versions is set");
    for (Component v : versions)
    {
     JSONObject vobj = new JSONObject();
     vobj.add("id", v.getId());
     vobj.add("name", v.getName());
     vobj.add("summary", v.getSummary());
     verlist.add(vobj);
    }
    je.add("versions", verlist);
   }
   System.out.println("end of versions list");
  }
  else
  {
   // This is a version - list predecessor
   JSONObject pred = new JSONObject();
   Component p = so.getComponent(comp.getPredecessorId(), true);
   pred.add("id", p.getId());
   pred.add("name", p.getName());
   pred.add("summary", p.getSummary());
   je.add("predecessor", pred);
  }

  if (idonly)
   return je;
  //
  // Now add all the applications with which this component is associated
  //
  JSONArray applist = new JSONArray();
  List<Application> applications = so.getAppsForComponent(comp);
  for (Application a : applications)
  {
   if (!so.ValidDomain(a.getDomainId()))
    continue;

   JSONObject ao = assembleJSONForApplication(so, a, false);
   applist.add(ao);
  }
  je.add("applications", applist);

  return je;
 }

 private JSONObject assembleJSONForAction(DMSession so, Action act)
 {
  JSONObject res = new JSONObject();
  res.add("name", act.getDomain().getFullName() + "." + act.getName());
  res.add("summary", act.getSummary());
  DMObject owner = act.getOwner();
  if (owner != null)
  {
   switch (owner.getObjectType())
   {
    case USERGROUP:
     res.add("ownergroup", owner.getName());
     break;
    case USER:
     res.add("owneruser", owner.getName());
     break;
    default:
     break;
   }
  }
  res.add("category", act.getCategory().getName());

  int actionid = act.getId();
  JSONArray nodes = new JSONArray();
  res.add("nodes", nodes);

  int startx = so.getStartXPosition(actionid,0);

  List <DMActionNode> ans = so.GetActionNodes(actionid,0);
  List <DMActionLink> als = so.GetActionLinks(actionid,0);

  for (DMActionNode a: ans)
  {
   String Title = a.getTitle();
   if (Title.length()==0) Title = a.getDescriptor();
   int style=0;
   if (a.getTypeID()==60) style=1; // Fragment ID of "if" is 60, change style for IF

   JSONObject node = new JSONObject();

   Action fp;

   if (a.getProcedureID() > 0)
    fp = so.getAction(a.getProcedureID(), true, ObjectType.PROCEDURE);
   else
    fp = so.getAction(a.getFunctionID(), true, ObjectType.FUNCTION);

   node.add("name", fp.getDomain().getFullDomain() + "." + fp.getName());
   node.add("descriptor", a.getDescriptor());
   node.add("xpos",a.getXpos());
   node.add("ypos", a.getYpos());
   node.add("exitpoints", a.getExitPoints());
   node.add("title", Title);
   node.add("summary", a.getSummary());
   node.add("drilldown", a.getDrillDown());
   node.add("kind", a.getKind());
   node.add("style", style);
   JSONArray args = new JSONArray();
   node.add("args", args);

   List<FragmentAttributes> fa = so.getFragmentAttributes(actionid,a.getNodeID(),0);

   for (FragmentAttributes attr: fa)
   {
    JSONObject obj = new JSONObject();
    obj.add("field", attr.getAttrName());
    obj.add("type", attr.getAttrType());
    obj.add("value", attr.getAttrVal());

    args.add(obj);
   }
   nodes.add(node);
  }

  JSONArray links = new JSONArray();
  res.add("links", links);

  for (DMActionLink al: als)
  {
   JSONObject link = new JSONObject();
   link.add("flowid", al.getFlowID());
   link.add("nodefrom", al.getNodeFrom());
   link.add("nodeto", al.getNodeTo());
   link.add("pos", al.getPos());
   links.add(link);
  }
  return res;
 }

 private JSONObject assembleJSONForCredential(DMSession so, Credential cred)
 {
  JSONObject res = new JSONObject();
  res.add("id", cred.getId());
  res.add("kind", cred.getKindAsString());
  res.add("username", cred.getVarUsername());
  res.add("password", cred.getVarPassword());
  res.add("name", cred.getName());
  res.add("domainid", cred.getDomainId());
  res.add("keyfile", cred.getFilename());
  return res;
 }

 private JSONObject assembleJSONForApplication(DMSession so, Application app, boolean idonly)
 {
  JSONObject je = assembleJSONForAppApprovals(so, app);
  je.add("id", app.getId());
  je.add("name", app.getName());
  je.add("branch", app.getLabel(true));
  je.add("lastcompver", so.getLastComp4App(app.getId()));

  if (idonly)
   return je;

  Domain dom = app.getDomain();
  if (dom != null)
  {
   je.add("domain", dom.getFullDomain());
  }
  DMObject owner = app.getOwner();
  if (owner != null)
  {
   switch (owner.getObjectType())
   {
    case USERGROUP:
     je.add("ownergroup", owner.getName());
     break;
    case USER:
     je.add("owneruser", owner.getName());
     break;
    default:
     break;
   }
  }
  je.add("summary", app.getSummary());
  //
  // If this is a base version list the versions
  //
  System.out.println("getId()=" + app.getId());
  System.out.println("getParentId()=" + app.getParentId());
  if (app.getParentId() == 0)
  {
   // This is a base application - list versions
   JSONArray verlist = new JSONArray();
   List<Application> versions = app.getVersions();
   if (versions != null)
   {
    for (Application v : versions)
    {
     JSONObject vobj = new JSONObject();
     vobj.add("id", v.getId());
     vobj.add("name", v.getName());
     vobj.add("summary", v.getSummary());
     verlist.add(vobj);
    }
    je.add("versions", verlist);
   }
  }
  else
  {
   // This is a version - list predecessor
   JSONObject pred = new JSONObject();
   Application p = so.getApplication(app.getPredecessorId(), true);
   pred.add("id", p.getId());
   pred.add("name", p.getName());
   pred.add("summary", p.getSummary());
   je.add("predecessor", pred);
  }
  // List the application components
  List<Component> comps = so.getComponents(ObjectType.APPLICATION, app.getId(), false);
  JSONArray complist = new JSONArray();
  for (Component c : comps)
  {
   if (!so.ValidDomain(c.getDomainId()))
    continue;

   JSONObject co = new JSONObject();
   co.add("id", c.getId());
   co.add("name", c.getName());
   co.add("summary", c.getSummary());
   co.add("xpos", c.getXpos());
   co.add("ypos", c.getYpos());
   System.out.println("component domain is " + c.getDomainId());
   co.add("domain", c.getDomain().getFullDomain());
   co.add("lastbuild", c.getLastBuildNumber());
   complist.add(co);
  }
  je.add("components", complist);
  return je;
 }

 private JSONObject assembleJSONForEnvironment(DMSession so, Environment env)
 {
  JSONObject je = new JSONObject();
  je.add("id", env.getId());
  je.add("name", env.getName());

  Domain dom = env.getDomain();

  je.add("domain", dom.getFullDomain());
  DMObject owner = env.getOwner();
  System.out.println("owner is " + owner);
  if (owner != null)
  {
   System.out.println("owner is " + owner.getId());
   switch (owner.getObjectType())
   {
    case USERGROUP:
     je.add("ownergroup", owner.getName());
     break;
    case USER:
     je.add("owneruser", owner.getName());
     break;
    default:
     break;
   }
  }
  je.add("summary", env.getSummary());
  //
  // Get applications associated/deployed to this environment
  //
  List<DeployedApplication> das = so.getDeployedApplicationsInEnvironment(env.getId());
  JSONArray applist = new JSONArray();
  for (DeployedApplication da : das)
  {
   JSONObject ja = new JSONObject();
   ja.add("id", da.getApplicationID());
   ja.add("name", da.getApplicationName());
   ja.add("versionid", da.getVersionID());
   ja.add("versionname", da.getVersionName());
   ja.add("predecessorid", da.getPredecessorID());
   ja.add("deploymentid", da.getDeploymentID());
   ja.add("exitcode", da.getExitCode());
   ja.add("completed", da.getCompletedDate());
   applist.add(ja);
  }
  je.add("applications", applist);

  List<Server> slist = so.getServersInEnvironment(env.getId());
  JSONArray servers = new JSONArray();
  for (Server s : slist)
  {
   JSONObject ja = new JSONObject();
   ja.add("id", s.getId());
   ja.add("name", s.getName());
   servers.add(ja);
  }
  je.add("servers", servers);

  return je;
 }

 private JSONObject assembleJSONForSaasClient(DMSession so, SaasClient sc)
 {
  JSONObject je = new JSONObject();
  je.add("id", sc.getClientId());
  je.add("licensetype", sc.getLicenseType());
  je.add("licensecnt", sc.getLicenseCnt());
  je.add("lastseen", sc.getLastseen());
  return je;
 }

 private JSONObject assembleJSONForDomain(DMSession so, Domain domain)
 {
  JSONObject je = new JSONObject();
  je.add("id", domain.getId());
  je.add("name", domain.getName());
  je.add("domain", domain.getDomain().getFullDomain());
  je.add("lifecycle", domain.getLifecycle());
  return je;
 }

 private JSONObject assembleJSONForUser(DMSession so, User u)
 {
  JSONObject je = new JSONObject();
  je.add("id", u.getId());
  je.add("name", u.getName());
  je.add("domain", u.getDomain().getFullDomain());
  return je;
 }

 private JSONObject assembleJSONForTestServer(DMSession so, Server server)
 {
  JSONObject ret = new JSONObject();
  ret.add("id", server.getId());
  CommandLine m_cmd;
  Domain m_domain = server.getDomain();
  Engine engine = (m_domain != null) ? m_domain.findNearestEngine() : null;

  if (engine == null)
  {
   System.err.println("Engine was null");
   ret.add("success", false);
  }
  else
  {
   System.out.println("testing server " + server.getId());
   m_cmd = engine.doTestServer(server);
   m_cmd.runWithTrilogy(true, "");

   Server s = so.getServer(server.getId(), false);
   ret.add("data", s.available());
  }
  return ret;
 }

 private JSONObject assembleJSONForServer(DMSession so, Server server)
 {
  JSONObject je = new JSONObject();
  je.add("id", server.getId());
  je.add("name", server.getName());
  je.add("domain", server.getDomain().getFullDomain());
  je.add("hostname", server.getHostName());
  je.add("protocol", server.getProtocol());
  je.add("autoping", server.getAutoPing());
  je.add("automd5", server.getAutoMD5());
  if (server.getAutoPing())
  {
   je.add("pingstart", encodeHM(server.getPingStart()));
   je.add("pingend", encodeHM(server.getPingEnd()));
  }
  je.add("basedir", server.getBaseDir());
  Credential cred = server.getCredential();
  if (cred != null)
  {
   je.add("credential", cred.getName());
  }
  NotifyTemplate md5template = server.getMD5Template();
  NotifyTemplate pingtemplate = server.getPingTemplate();
  if (md5template != null)
  {
   je.add("md5template", md5template.getName());
  }
  if (pingtemplate != null)
  {
   je.add("pingtemplate", pingtemplate.getName());
  }
  je.add("summary", server.getSummary());
  DMObject owner = server.getOwner();
  if (owner != null)
  {
   if (owner.getObjectType() == ObjectType.USER)
    je.add("owneruser", owner.getName());
   if (owner.getObjectType() == ObjectType.USERGROUP)
    je.add("ownergroup", owner.getName());
  }
  List<CompType> cts = server.getServerCompType();
  if (cts.size() > 0)
  {
   JSONArray comptypes = new JSONArray();
   for (CompType ct : cts)
   {
    comptypes.add(ct.getName());
   }
   je.add("comptypes", comptypes);
  }
  List<Environment> envs = so.getEnvironmentsForServer(server);
  if (envs.size() > 0)
  {
   JSONArray envlist = new JSONArray();
   for (Environment env : envs)
   {
    Domain envdom = env.getDomain();
    if (envdom != null)
    {
     // If the domain is outside our domain list, getDomain will return null
     JSONObject eo = new JSONObject();
     eo.add("id", env.getId());
     eo.add("name", env.getName());
     eo.add("domain", env.getDomain().getFullDomain());
     envlist.add(eo);
    }
   }
   je.add("environments", envlist);
  }
  return je;
 }



 protected Component getComponentFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Component) so.getObjectFromNameOrID(ObjectType.COMPONENT, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 protected Application getApplicationFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Application) so.getObjectFromNameOrID(ObjectType.APPLICATION, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 protected Environment getEnvironmentFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Environment) so.getObjectFromNameOrID(ObjectType.ENVIRONMENT, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }


 private Notify getNotifyFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Notify) so.getObjectFromNameOrID(ObjectType.NOTIFY, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private Server getServerFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Server) so.getObjectFromNameOrID(ObjectType.SERVER, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private Action getActionFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Action) so.getObjectFromNameOrID(ObjectType.ACTION, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 public Domain getDomainFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Domain) so.getObjectFromNameOrID(ObjectType.DOMAIN, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private User getUserFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (User) so.getObjectFromNameOrID(ObjectType.USER, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private Datasource getDatasourceFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Datasource) so.getObjectFromNameOrID(ObjectType.DATASOURCE, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private Repository getRepositoryFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Repository) so.getObjectFromNameOrID(ObjectType.REPOSITORY, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private Credential getCredentialFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (Credential) so.getObjectFromNameOrID(ObjectType.CREDENTIALS, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private NotifyTemplate getTemplateFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (NotifyTemplate) so.getObjectFromNameOrID(ObjectType.TEMPLATE, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private UserGroup getGroupFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (UserGroup) so.getObjectFromNameOrID(ObjectType.USERGROUP, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private CompType getCompTypeFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  try
  {
   return (CompType) so.getObjectFromNameOrID(ObjectType.SERVERCOMPTYPE, idOrName);
  }
  catch (GetObjectException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private void internalModUser(DMSession so, User moduser, HashMap<String, String> params) throws ApiException
 {
  Datasource ldap = null;
  SummaryChangeSet changes = new SummaryChangeSet();
  boolean bcpw = false;
  boolean blocked = false;
  String realname = params.get("realname");
  String ldapname = params.get("ldap");
  if (ldapname != null)
  {
   ldap = getDatasourceFromNameOrID(so, ldapname);
  }
  String tel = params.get("tel");
  String email = params.get("email");
  String pw = params.get("pw");
  String cpw = params.get("cpw");
  String locked = params.get("locked");

  if (cpw != null)
   bcpw = (cpw.equalsIgnoreCase("y"));
  if (locked != null)
   blocked = (locked.equalsIgnoreCase("y"));
  if (realname != null)
   changes.add(SummaryField.USER_REALNAME, realname);
  if (ldap != null)
   changes.add(SummaryField.USER_DATASOURCE, ldap);
  if (tel != null)
   changes.add(SummaryField.USER_PHONE, tel);
  if (email != null)
   changes.add(SummaryField.USER_EMAIL, email);
  if (pw != null)
   changes.add(SummaryField.USER_PASSWORD, pw);
  if (cpw != null)
   changes.add(SummaryField.USER_CHNG_PASS, bcpw);
  if (locked != null)
   changes.add(SummaryField.USER_LOCKED, blocked);
  if (!changes.isEmpty())
  {
   so.updateUser(moduser, changes);
  }
  else
  {
   try
   {
    so.GetConnection().commit();
   }
   catch (SQLException e)
   {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }
 }

 private void internalModUser(DMSession so, User moduser, HttpServletRequest request) throws ApiException
 {
  Datasource ldap = null;
  SummaryChangeSet changes = new SummaryChangeSet();
  boolean bcpw = false;
  boolean blocked = false;
  String realname = request.getParameter("realname");
  String ldapname = request.getParameter("ldap");
  if (ldapname != null)
  {
   ldap = getDatasourceFromNameOrID(so, ldapname);
  }
  String tel = request.getParameter("tel");
  String email = request.getParameter("email");
  String pw = request.getParameter("pw");
  String cpw = request.getParameter("cpw");
  String locked = request.getParameter("locked");

  if (cpw != null)
   bcpw = (cpw.equalsIgnoreCase("y"));
  if (locked != null)
   blocked = (locked.equalsIgnoreCase("y"));
  if (realname != null)
   changes.add(SummaryField.USER_REALNAME, realname);
  if (ldap != null)
   changes.add(SummaryField.USER_DATASOURCE, ldap);
  if (tel != null)
   changes.add(SummaryField.USER_PHONE, tel);
  if (email != null)
   changes.add(SummaryField.USER_EMAIL, email);
  if (pw != null)
   changes.add(SummaryField.USER_PASSWORD, pw);
  if (cpw != null)
   changes.add(SummaryField.USER_CHNG_PASS, bcpw);
  if (locked != null)
   changes.add(SummaryField.USER_LOCKED, blocked);
  if (!changes.isEmpty())
  {
   so.updateUser(moduser, changes);
  }
  else
  {
   try
   {
    so.GetConnection().commit();
   }
   catch (SQLException e)
   {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }
 }

 private int convertTime(String timein) throws ApiException
 {
  if (!timein.contains(":"))
   throw new ApiException("Invalid time " + timein);
  String hrs = timein.substring(0, timein.indexOf(':'));
  String mins = timein.substring(timein.indexOf(':') + 1);
  try
  {
   int h = Integer.parseInt(hrs);
   int m = Integer.parseInt(mins);
   if (h < 0 || h > 23)
    throw new ApiException("Invalid time " + timein);
   if (m < 0 || m > 59)
    throw new ApiException("Invalid time " + timein);
   if ((m % 15) > 0)
    throw new ApiException("Invalid time " + timein + " (mins must be multiple of 15)");
   return (h * 3600) + (m * 60);
  }
  catch (NumberFormatException ex)
  {
   throw new ApiException("Invalid time " + timein);
  }
 }

 private JSONObject tsObject(int timein)
 {
  JSONObject to = new JSONObject();
  to.add("timestamp", timein);
  to.add("datetime", decodeDateTime(timein));
  return to;
 }

 private String encodeHM(int timein)
 {
  Integer hours = timein / 3600;
  Integer mins = ((timein % 3600) / 60);
  String hs = (hours <= 9) ? "0" + hours.toString() : hours.toString();
  String ms = (mins <= 9) ? "0" + mins.toString() : mins.toString();
  return hs + ":" + ms;
 }

 private JSONObject userObject(DMSession so, int uid)
 {
  User user = so.getUser(uid);
  JSONObject uo = new JSONObject();
  if (user != null)
  {
   uo.add("id", uid);
   uo.add("name", user.getName());
  }
  return uo;
 }

 private String decodeDateTime(int timein)
 {
  Date date = new Date(timein * 1000L);
  DateFormat df = new SimpleDateFormat("yyyy:MM:dd:HH:mm");
  return df.format(date);
 }

 private long convertDateTime(String timein) throws ApiException
 {
  // Converts a date/time string in format
  // YYYY:MM:DD:HH:MM
  // or
  // epoch (just returns atol value
  //
  long ret = 0;
  if (timein.contains(":"))
  {
   try
   {
    String[] parts = timein.split(":");
    if (parts.length == 3)
    {
     // YYYY:MM:DD
     SimpleDateFormat df = new SimpleDateFormat("yyyy:MM:dd");
     Date date = df.parse(timein);
     ret = (date.getTime() / 1000);
    }
    else if (parts.length == 5)
    {
     // YYYY:MM:DD:HH:MM
     SimpleDateFormat df = new SimpleDateFormat("yyyy:MM:dd:HH:mm");
     Date date = df.parse(timein);
     ret = (date.getTime() / 1000);
    }
    else
     throw new ApiException("Invalid date " + timein);
   }
   catch (ParseException e)
   {
    throw new ApiException("Invalid date " + timein + ": " + e.getMessage());
   }
  }
  else
  {
   try
   {
    ret = Integer.parseInt(timein);
   }
   catch (NumberFormatException ex)
   {
    throw new ApiException("Invalid epoch time " + timein);
   }
  }
  return ret;
 }

 private void internalModCredential(DMSession so, Credential modcred, HttpServletRequest request, Domain domain) throws ApiException
 {
  SummaryChangeSet changes = new SummaryChangeSet();
  String kindString = request.getParameter("kind");
  String username = request.getParameter("username");
  String password = request.getParameter("password");
  String keyfile = request.getParameter("keyfile");
  String summary = request.getParameter("summary");
  String filename = request.getParameter("filename");

  CredentialKind ck = CredentialKind.UNCONFIGURED;
  boolean newCredential = false;
  if (kindString == null)
  {
   // Must be modification of existing credential
   ck = modcred.getKind();
  }
  else
  {
   // Must be new credential;
   if (kindString.equalsIgnoreCase("encrypted"))
    ck = CredentialKind.ENCRYPTED;
   else if (kindString.equalsIgnoreCase("vars"))
    ck = CredentialKind.FROM_VARS;
   else if (kindString.equalsIgnoreCase("ppk"))
    ck = CredentialKind.PPK;
   else if (kindString.equalsIgnoreCase("dfo"))
    ck = CredentialKind.HARVEST_DFO_IN_FILESYSTEM;
   else
    throw new ApiException("invalid value of \'kind\': Must be \'encrypted\',\'vars\',\'ppk\' or \'dfo\'");
   newCredential = true;
  }

  Engine eng = (domain != null) ? domain.findNearestEngine() : null;
  if (eng == null)
  {
   throw new ApiException("ERROR: Could not find engine to encrypt credential data");
  }

  switch (ck)
  {
   case ENCRYPTED:
    // Encrypted username/password
    if (newCredential && (username == null || password == null))
    {
     throw new ApiException("Both \'username\' and \'password\' parameters must be specified for kind \'encrypted\'");
    }
    String encUsername = (username != null) ? eng.encryptValue(username, so.GetUserName()) : null;
    String encPassword = (password != null) ? eng.encryptValue(password, so.GetUserName()) : null;
    if (newCredential)
     changes.add(SummaryField.CRED_KIND, ck);
    if (encUsername != null)
     changes.add(SummaryField.CRED_ENCUSERNAME, encUsername);
    if (encPassword != null)
     changes.add(SummaryField.CRED_ENCPASSWORD, encPassword);
    break;
   case FROM_VARS:
    // From variables
    if (newCredential && (username == null || password == null))
    {
     throw new ApiException("Both \'username\' and \'password\' parameters must be specified for kind \'vars\'");
    }
    if (newCredential)
     changes.add(SummaryField.CRED_KIND, ck);
    if (username != null)
     changes.add(SummaryField.CRED_VARUSERNAME, username);
    if (password != null)
     changes.add(SummaryField.CRED_VARPASSWORD, password);
    break;
   case PPK:
    // Public/Private Key File
    if (newCredential && (keyfile == null || username == null))
    {
     throw new ApiException("Both \'keyfile\' and \'username\' parameters must be specified for kind \'ppk\'");
    }
    if (newCredential)
     changes.add(SummaryField.CRED_KIND, ck);
    if (username != null)
     changes.add(SummaryField.CRED_VARUSERNAME, username);
    if (keyfile != null)
     changes.add(SummaryField.CRED_FILENAME, keyfile);
    break;
   case HARVEST_DFO_IN_FILESYSTEM:
    // Harvest DFO file
    if (newCredential && filename == null)
    {
     throw new ApiException("\'filename\' parameter must be specified for kind \'dfo'");
    }
    if (newCredential)
     changes.add(SummaryField.CRED_KIND, ck);
    if (filename != null)
     changes.add(SummaryField.CRED_FILENAME, filename);
    break;
   default:
    break;
  }

  if (summary != null)
   changes.add(SummaryField.SUMMARY, summary);
  if (!changes.isEmpty())
  {
   so.updateCredential(modcred, changes); // Commits on success
  }
  else
  {
   try
   {
    so.GetConnection().commit();
   }
   catch (SQLException e)
   {
    e.printStackTrace();
   }
  }
 }

 private void internalModDomain(DMSession so, Domain moddom, HttpServletRequest request, boolean lifecycle) throws ApiException
 {
  SummaryChangeSet changes = new SummaryChangeSet();

  if (lifecycle)
   changes.add(SummaryField.DOMAIN_LIFECYCLE, true);
  if (!changes.isEmpty())
  {
   so.updateDomain(moddom, changes); // Commits on success
  }
  else
  {
   try
   {
    so.GetConnection().commit();
   }
   catch (SQLException e)
   {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }
 }

 private void internalModEnvironment(DMSession so, Environment modenv, HttpServletRequest request) throws ApiException
 {
  SummaryChangeSet changes = new SummaryChangeSet();
  String summary = request.getParameter("summary");
  if (summary != null)
   changes.add(SummaryField.SUMMARY, summary);
  if (!changes.isEmpty())
  {
   so.updateEnvironment(modenv, changes); // Commits on success
  }
  else
  {
   try
   {
    so.GetConnection().commit();
   }
   catch (SQLException e)
   {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }
 }

 private void internalModServer(DMSession so, Server modserv, HttpServletRequest request) throws ApiException
 {
  SummaryChangeSet changes = new SummaryChangeSet();
  boolean lookupOK = false;

  String summary = request.getParameter("summary");
  String type = request.getParameter("type");
  String hostname = request.getParameter("hostname");
  String protocol = request.getParameter("protocol");
  String basedir = request.getParameter("basedir");
  String credname = request.getParameter("credential");
  String autoping = request.getParameter("autoping");
  String automd5 = request.getParameter("automd5");
  String comptypes = request.getParameter("comptypes");
  String pinginterval = request.getParameter("pinginterval");
  String pingstart = request.getParameter("pingstart");
  String pingend = request.getParameter("pingend");
  String pingtemplate = request.getParameter("pingtemplate");
  String md5template = request.getParameter("md5template");
  String sshport = request.getParameter("sshport");
  String force = request.getParameter("force");

  if (force != null)
  {
   TableDataSet comps = so.getComponentsOnServer(modserv);
   if (comps != null)
   {
    for (int k = 0; k <= comps.getRows(); k++)
    {
     LinkField c = (LinkField) comps.get(k, 1);
     LinkField c3 = (LinkField) comps.get(k, 3);
     if (c3.getDeploymentid() > 0)
      so.removeAppVersionFromEnv(c3.getDeploymentid());

     so.serverRemoveComponent(modserv.getId(), c.getId());
    }
   }
  }

  if (comptypes != null)
  {
   String idlist = "";
   String sep = "";
   List<DMObject> vts = so.getCompTypes();
   String[] cts = comptypes.split(",");
   for (int i = 0; i < cts.length; i++)
   {
    System.out.println("cts[" + i + "]=[" + cts[i] + "]");
    lookupOK = false;
    for (DMObject vt : vts)
    {
     if (vt.getName().equalsIgnoreCase(cts[i]))
     {
      idlist = idlist + sep + vt.getId();
      sep = ";";
      lookupOK = true;
      break;
     }
    }
    if (!lookupOK)
     throw new ApiException("Invalid Endpoint Type " + cts[i]);
   }
   changes.add(SummaryField.SERVER_COMPTYPE, idlist);
  }

  if (autoping != null)
  {
   boolean bautoping = (autoping.equalsIgnoreCase("y"));
   changes.add(SummaryField.SERVER_AUTOPING, bautoping);
  }
  if (automd5 != null)
  {
   boolean bautomd5 = (automd5.equalsIgnoreCase("y"));
   changes.add(SummaryField.SERVER_AUTOMD5, bautomd5);
  }

  if (pinginterval != null)
  {
   try
   {
    int pi = Integer.parseInt(pinginterval);
    if (pi < 0 || pi > 240)
     throw new ApiException("Ping Interval " + pinginterval + " invalid (valid range 0 - 240 mins)");
    if ((pi % 15) > 0)
     throw new ApiException("Ping Interval " + pinginterval + " invalid (must be a multiple of 15 mins)");
    changes.add(SummaryField.SERVER_PINGINTERVAL, pinginterval);
   }
   catch (NumberFormatException ex)
   {
    throw new ApiException("Ping Interval " + pinginterval + " invalid (format error)");
   }
  }
  if (pingstart != null)
  {
   Integer t = convertTime(pingstart);
   changes.add(SummaryField.SERVER_PINGSTART, t.toString());
  }
  if (pingend != null)
  {
   Integer t = convertTime(pingend);
   changes.add(SummaryField.SERVER_PINGEND, t.toString());
  }

  if (type != null)
  {
   // Look up type
   lookupOK = false;
   List<ServerType> sts = so.getServerTypes();
   for (ServerType st : sts)
   {
    if (st.getName().equalsIgnoreCase(type))
    {
     changes.add(SummaryField.SERVER_TYPE, st);
     lookupOK = true;
     break;
    }
   }
   if (!lookupOK)
    throw new ApiException("Invalid Endpoint Type \"" + type + "\" specified");
  }
  if (hostname != null)
   changes.add(SummaryField.SERVER_HOSTNAME, hostname);
  if (basedir != null)
   changes.add(SummaryField.SERVER_BASEDIR, basedir);
  if (sshport != null)
   changes.add(SummaryField.SERVER_SSHPORT, sshport);
  if (summary != null)
   changes.add(SummaryField.SUMMARY, summary);
  if (protocol != null)
  {
   // Look up protocol
   lookupOK = false;
   List<Transfer> ts = so.getTransfers();
   for (Transfer t : ts)
   {
    if (t.getName().equalsIgnoreCase(protocol))
    {
     changes.add(SummaryField.SERVER_PROTOCOL, protocol);
     lookupOK = true;
     break;
    }
   }
   if (!lookupOK)
    throw new ApiException("Invalid protocol type \"" + protocol + "\" specified");
  }
  if (credname != null)
  {
   Credential cred = getCredentialFromNameOrID(so, credname);
   changes.add(SummaryField.SERVER_CRED, cred);
  }
  if (pingtemplate != null)
  {
   NotifyTemplate tmpl = getTemplateFromNameOrID(so, pingtemplate);
   changes.add(SummaryField.SERVER_PINGTEMPLATE, tmpl);
  }
  if (md5template != null)
  {
   NotifyTemplate tmpl = getTemplateFromNameOrID(so, md5template);
   changes.add(SummaryField.SERVER_MD5TEMPLATE, tmpl);
  }

  if (!changes.isEmpty())
  {
   so.updateServer(modserv, changes); // Commits on success
  }
  else
  {
   try
   {
    so.GetConnection().commit();
   }
   catch (SQLException e)
   {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }
 }

 private void ModUser(DMSession so, String username, HttpServletRequest request) throws ApiException
 {
  Domain tgtdomain = null;
  if (username == null || username.length() == 0)
   throw new ApiException("username must be specified");
  User moduser = getUserFromNameOrID(so, username);
  if (!moduser.isUpdatable())
   throw new ApiException("Permission Denied");
  internalModUser(so, moduser, request);
  String domain = request.getParameter("domain");
  if (domain != null)
  {
   // Switching domain
   tgtdomain = getDomainFromNameOrID(so, domain);
   String res = so.MoveObject("user", 0, moduser.getId(), "domain", tgtdomain.getId());
   if (res != null && res != "")
   {
    throw new ApiException(res);
   }
  }
 }

 private void ModServer(DMSession so, String servername, HttpServletRequest request) throws ApiException
 {
  Domain tgtdomain = null;
  if (servername == null || servername.length() == 0)
   throw new ApiException("servername must be specified");
  Server modserv = getServerFromNameOrID(so, servername);
  if (!modserv.isUpdatable())
   throw new ApiException("Permission Denied");
  internalModServer(so, modserv, request);
  String domain = request.getParameter("domain");
  if (domain != null)
  {
   // Switching domain
   tgtdomain = getDomainFromNameOrID(so, domain);
   String res = so.MoveObject("server", 0, modserv.getId(), "domain", tgtdomain.getId());
   if (res != null && res != "")
   {
    throw new ApiException(res);
   }
  }
 }

 private void ModEnvironment(DMSession so, String envname, HttpServletRequest request) throws ApiException
 {
  Domain tgtdomain = null;
  if (envname == null || envname.length() == 0)
   throw new ApiException("environment name must be specified");
  Environment modenv = getEnvironmentFromNameOrID(so, envname);
  if (!modenv.isUpdatable())
   throw new ApiException("Permission Denied");
  internalModEnvironment(so, modenv, request);
  String domain = request.getParameter("domain");
  if (domain != null)
  {
   // Switching domain
   tgtdomain = getDomainFromNameOrID(so, domain);
   String res = so.MoveObject("environment", 0, modenv.getId(), "domain", tgtdomain.getId());
   if (res != null && res != "")
   {
    throw new ApiException(res);
   }
  }
 }

 private void ModCredential(DMSession so, String credname, HttpServletRequest request) throws ApiException
 {
  Domain tgtdomain = null;
  if (credname == null || credname.length() == 0)
   throw new ApiException("credential name must be specified");
  Credential modcred = getCredentialFromNameOrID(so, credname);
  if (!modcred.isUpdatable())
   throw new ApiException("Permission Denied");
  String kindString = request.getParameter("kind");
  if (kindString != null)
  {
   throw new ApiException("Cannot change \'kind\' of an existing credential");
  }
  internalModCredential(so, modcred, request, modcred.getDomain());
  String domain = request.getParameter("domain");
  if (domain != null)
  {
   // Switching domain
   tgtdomain = getDomainFromNameOrID(so, domain);
   String res = so.MoveObject("credentials", 0, modcred.getId(), "domain", tgtdomain.getId());
   if (res != null && res != "")
   {
    throw new ApiException(res);
   }
  }
 }

 private void AddUser(DMSession so, String username, HttpServletRequest request) throws ApiException
 {
  // API/new/user?name=<loginname>[&domain=<domain>&realname=<real name>&ldap=<ldap datasource>&tel=<tel no>&email=<email>&cpw=Y|N&locked=Y|N&pw=<password>]
  // Do we have create user permission?
  if (!so.getCreatePermission(ObjectType.USER))
   throw new ApiException("No Create User Permission");
  if (username == null || username.length() == 0)
   throw new ApiException("username must be specified");
  String domain = request.getParameter("domain");
  Domain tgtdomain = null;
  // If domain is specified, use that, otherwise create user in user's home domain
  if (domain != null)
  {
   tgtdomain = getDomainFromNameOrID(so, domain);
   if (tgtdomain == null)
    throw new ApiException("Unable to find domain " + tgtdomain + ": name invalid or no access");
  }
  else
  {
   tgtdomain = so.getUserDomain();
  }
  if (!tgtdomain.isUpdatable())
   throw new ApiException("Permission Denied");

  try
  {
   int newid = so.getID("user");
   so.CreateNewObject("user", username, tgtdomain.getId(), tgtdomain.getId(), newid, -1, -1, "", false);
   // Having created the user, submit the "changes" to set the attributes
   User newuser = so.getUser(newid);
   if (newuser == null)
    throw new ApiException("Failed to create new user");
   internalModUser(so, newuser, request);
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 protected void AddDomain(DMSession so, String domname, HttpServletRequest request) throws ApiException
 {
  // API/new/domain/name[?domain=<domain>&lifecyle=Y]
  // Do we have create domain permission?
  if (!so.getCreatePermission(ObjectType.DOMAIN))
  {
   // No!
   throw new ApiException("No Create Domain Permission");
  }
  if (domname == null || domname.length() == 0)
   throw new ApiException("domain name must be specified");
  String domain = request.getParameter("domain");
  Domain tgtdomain = null;
  // If domain is specified, use that, otherwise create domain in user's home domain
  if (domain != null)
  {
   tgtdomain = getDomainFromNameOrID(so, domain);
   if (tgtdomain == null)
    throw new ApiException("Unable to find domain " + tgtdomain + ": name invalid or no access");
  }
  else
  {
   throw new ApiException("Parent domain is required");
  }

  try
  {
   int newid = so.getID("domain");
   so.CreateNewObject("domain", domname, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", false);
   // Having created the environment, submit the "changes" to set the attributes
   Domain newdom = so.getDomain(newid);
   if (newdom == null)
    throw new ApiException("Failed to create new domain");
   internalModDomain(so, newdom, request, false);
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private static final String HMAC_SHA1_ALGORITHM = "HmacSHA1";

 private static String toHexString(byte[] bytes)
 {
  try (Formatter formatter = new Formatter())
  {

  for (byte b : bytes)
  {
   formatter.format("%02x", b);
  }

  return formatter.toString();
  }
 }

 public static String calculateRFC2104HMAC(String data, String key) throws SignatureException, NoSuchAlgorithmException, InvalidKeyException
 {
  SecretKeySpec signingKey = new SecretKeySpec(key.getBytes(), HMAC_SHA1_ALGORITHM);
  Mac mac = Mac.getInstance(HMAC_SHA1_ALGORITHM);
  mac.init(signingKey);
  return toHexString(mac.doFinal(data.getBytes()));
 }

 public void createDomains4Obj(DMSession so, String objname)
 {
  if (!objname.contains("."))
   return;

  ArrayList<String> parts = new ArrayList<String>(Arrays.asList(objname.split("\\.")));
  if (parts != null && !parts.isEmpty())
   parts.remove(parts.size()-1);

  Domain tgtdomain = null;
  try
  {
   tgtdomain = getDomainFromNameOrID(so, parts.get(0));
  }
  catch (ApiException e1)
  {
   tgtdomain = null;
  }

  String domStr = parts.get(0);

  for (int i = 1; i < parts.size(); i++)
  {
   Domain exists_domain = null;
   String domain = parts.get(i);

   domStr += "." + domain;
   try
   {
    exists_domain = getDomainFromNameOrID(so, domStr);
    tgtdomain = exists_domain;
   }
   catch (Exception e)
   {
   }

   if (tgtdomain != null && exists_domain == null)
   {
    int newid = so.getID("domain");
    so.CreateNewObject("domain", domain, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", true);

    tgtdomain = so.getDomain(newid);
   }
  }
 }

 private void AddEnvironment(DMSession so, String envname, HttpServletRequest request) throws ApiException
 {
  // API/new/environment/name[?domain=<domain>&summary=<summary>]
  // Do we have create environment permission?
  createDomains4Obj(so, envname);

  if (!so.getCreatePermission(ObjectType.ENVIRONMENT))
  {
   // No!
   throw new ApiException("No Create Environment Permission");
  }
  if (envname == null || envname.length() == 0)
   throw new ApiException("environmentname must be specified");
  String domain = request.getParameter("domain");
  Domain tgtdomain = null;
  // If domain is specified, use that, otherwise create environment in user's home domain
  if (domain != null)
  {
   tgtdomain = getDomainFromNameOrID(so, domain);
   if (tgtdomain == null)
    throw new ApiException("Unable to find domain " + tgtdomain + ": name invalid or no access");
  }
  else
  {
   tgtdomain = so.getUserDomain();
  }

  try
  {
   int newid = so.getID("environment");
   so.CreateNewObject("environment", envname, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "environments", false);
   // Having created the environment, submit the "changes" to set the attributes
   Environment newenv = so.getEnvironment(newid, true);
   if (newenv == null)
    throw new ApiException("Failed to create new environment");
   internalModEnvironment(so, newenv, request);
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private void AddCredential(DMSession so, String credname, HttpServletRequest request) throws ApiException
 {
  // API/new/credential/name[?domain=<domain>&summary=<summary>]
  // Do we have create credential permission?
  if (!so.getCreatePermission(ObjectType.CREDENTIALS))
  {
   // No!
   throw new ApiException("No Create Credential Permission");
  }
  if (credname == null || credname.length() == 0)
   throw new ApiException("credentialname must be specified");
  String domain = request.getParameter("domain");
  Domain tgtdomain = null;
  // If domain is specified, use that, otherwise create credential in user's home domain
  if (domain != null)
  {
   tgtdomain = getDomainFromNameOrID(so, domain);
   if (tgtdomain == null)
    throw new ApiException("Unable to find domain " + tgtdomain + ": name invalid or no access");
  }
  else
  {
   tgtdomain = so.getUserDomain();
  }

  try
  {
   int newid = so.getID("credentials");
   String kindString = request.getParameter("kind");
   if (kindString == null)
   {
    throw new ApiException("\'kind\' has to be specified where creating a new credential");
   }
   so.CreateNewObject("credentials", credname, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "environments", false);
   // Having created the credential, submit the "changes" to set the attributes
   Credential newcred = so.getCredential(newid, true);
   if (newcred == null)
    throw new ApiException("Failed to create new credential");
   internalModCredential(so, newcred, request, tgtdomain);
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private void AddAppBaseVersion(DMSession so, String appname, HttpServletRequest request) throws ApiException
 {
  // API/new/environment/name[?domain=<domain>&summary=<summary>]
  // Do we have create environment permission?
  if (!so.getCreatePermission(ObjectType.APPLICATION))
  {
   // No!
   throw new ApiException("No Create Application Permission");
  }

  System.out.println("DOING newappver");

  String name = request.getParameter("name");
  String domain = request.getParameter("domain");
  String fullname = "";

  if (name != null)
   appname = name;

  if (appname == null || appname.length() == 0)
   throw new ApiException("application name must be specified");

  if (domain != null)
   fullname = domain + "." + appname;
   createDomains4Obj(so, fullname);

  Domain tgtdomain = null;
  // If domain is specified, use that, otherwise create environment in user's home domain
  if (domain != null)
  {
   tgtdomain = getDomainFromNameOrID(so, domain);
   if (tgtdomain == null)
    throw new ApiException("Unable to find domain " + tgtdomain + ": name invalid or no access");
  }
  else
  {
   tgtdomain = so.getUserDomain();
  }

  try
  {
   int newid = so.getID("application");
   so.CreateNewObject("application",appname, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "applications", true);
   // Having created the environment, submit the "changes" to set the attributes
   Application newapp = so.getApplication(newid, true);
   if (newapp == null)
    throw new ApiException("Failed to create new application");
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private ComponentItem AddComponentItems(DMSession so, String compname, String compitem, int xpos, int ypos, boolean removeAll, HttpServletRequest request, String targetdir, String repo, String pattern, String uri, String version, String kind) throws ApiException
 {
  // API/new/compitem/name[?component=<component>&xpos=<xpos>&ypos=<ypos>&removeAll=<Y>]

  if (compname == null || compname.length() == 0)
   throw new ApiException("component name must be specified");
  if (compitem == null || compitem.length() == 0)
   throw new ApiException("component item must be specified");

  try
  {
   Component comp = getComponentFromNameOrID(so, compname);

   if (comp == null)
    throw new ApiException("Could not retrieve component '" + compname + "'");

   if (removeAll) // Used when adding the first item
   {
    try
    {
     so.DeleteComponentItems(comp.getId());
    }
    catch (SQLException e)
    {
     throw new ApiException("Could not remove Component Items for '" + compname + "'");
    }
   }

   int newid = so.getID("componentitem");

   so.CreateNewObject("componentitem", compitem, comp.getDomainId(), comp.getId(), newid, xpos, ypos, "components", false);

   ComponentItem compi = so.getComponentItem(newid, true);

   if (compi == null)
    throw new ApiException("Failed to create new component item");

   so.componentItemMoveItem(comp.getId(), compi.getId(), xpos, ypos);

   SummaryChangeSet schanges = new SummaryChangeSet();
   SummaryField field = SummaryField.fromInt(SummaryField.ITEM_REPOSITORY.value());
   if (field != null)
   {
    Repository r = getRepositoryFromNameOrID(so, repo);
    so.processField(so, field, "re" + r.getId(), schanges);
   }

   schanges.add(SummaryField.ITEM_TARGETDIR, targetdir);

   ACDChangeSet<DMProperty> pchanges = new ACDChangeSet<DMProperty>();

   pchanges.addAdded(new DMProperty("pattern", pattern, false, false, false));
   pchanges.addAdded(new DMProperty("URI", uri, false, false, false));
   pchanges.addAdded(new DMProperty("version", version, false, false, false));
   pchanges.addAdded(new DMProperty("XXX", uri, false, false, false));

   ComponentItem ci = so.getComponentItem(compi.getId(), true);
   if (ci.isUpdatable())
   {
    boolean res = true;
    if (!schanges.isEmpty())
    {
     ci.updateSummary(schanges);
    }
    if (res && !pchanges.isEmpty())
    {
     res = ci.updateProperties(pchanges);
    }
   }
   return ci;
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private void AddServer(DMSession so, String servername, HttpServletRequest request) throws ApiException
 {
  // API/new/server/name[?domain=<domain>&env=<env>]
  // Do we have create server permission?
  if (!so.getCreatePermission(ObjectType.SERVER))
  {
   // No!
   throw new ApiException("No Create Server Permission");
  }

  createDomains4Obj(so, servername);

  if (servername == null || servername.length() == 0)
   throw new ApiException("servername must be specified");
  String domain = request.getParameter("domain");
  Domain tgtdomain = null;
  // If domain is specified, use that, otherwise create server in user's home domain
  if (domain != null)
  {
   tgtdomain = getDomainFromNameOrID(so, domain);
   if (tgtdomain == null)
    throw new ApiException("Unable to find domain " + tgtdomain + ": name invalid or no access");
  }
  else
  {
   tgtdomain = so.getUserDomain();
  }

  try
  {
   Environment env = null;
   int newid = so.getID("server");
   String envnameorid = request.getParameter("env");
   if (envnameorid != null)
   {
    env = getEnvironmentFromNameOrID(so, envnameorid);
   }
   int parentid = (env != null) ? env.getId() : tgtdomain.getId();
   //
   // Horrible hack - if env has been set, pass "environments" as the treeid, otherwise pass "servers". Code in
   // CreateNewObject will associate the server with the environment if the treeid != "servers".
   //
   so.CreateNewObject("server", servername, tgtdomain.getId(), parentid, newid, 0, 0, (env != null) ? "environments" : "servers", false);
   // Having created the server, submit the "changes" to set the attributes
   Server newserver = so.getServer(newid, true);
   if (newserver == null)
    throw new ApiException("Failed to create new server");
   internalModServer(so, newserver, request);
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 private void cloneServer(DMSession so, String servername, String newname, HttpServletRequest request) throws ApiException
 {
  Server server = getServerFromNameOrID(so, servername);
  int domainid = server.getDomainId();
  Domain domain = null;
  String domname = request.getParameter("domain");
  if (domname != null)
  {
   domain = getDomainFromNameOrID(so, domname);
   if (domain != null)
   {
    domainid = domain.getId();
   }
  }

  so.CopyObject("server", server.getId());
  DMObject ret = so.PasteObject(domainid, domainid, false);
  if (ret != null)
  {
   System.out.println("server " + ret.getName() + " created");
   Server newserver = so.getServer(ret.getId(), true);
   SummaryChangeSet changes = new SummaryChangeSet();
   changes.add(SummaryField.NAME, newname);
   try
   {
    so.updateServer(newserver, changes);
   }
   catch (RuntimeException ex)
   {
    so.rollback();
    throw new ApiException(ex.getMessage());
   }
  }
  else
  {
   throw new ApiException("Failed to create new server");
  }
 }

 private void cloneEnvironment(DMSession so, String envname, String newname, HttpServletRequest request) throws ApiException
 {
  Environment env = getEnvironmentFromNameOrID(so, envname);
  int domainid = env.getDomainId();
  Domain domain = null;
  String domname = request.getParameter("domain");
  if (domname != null)
  {
   domain = getDomainFromNameOrID(so, domname);
   if (domain != null)
   {
    domainid = domain.getId();
   }
  }
  so.CopyObject("environment", env.getId());
  DMObject ret = so.PasteObject(domainid, domainid, false);
  if (ret != null)
  {
   System.out.println("environment " + ret.getName() + " created");
   Environment newenv = so.getEnvironment(ret.getId(), true);
   SummaryChangeSet changes = new SummaryChangeSet();
   changes.add(SummaryField.NAME, newname);
   try
   {
    so.updateEnvironment(newenv, changes);
   }
   catch (RuntimeException ex)
   {
    so.rollback();
    throw new ApiException(ex.getMessage());
   }
  }
  else
  {
   throw new ApiException("Failed to create new environment");
  }
 }

 private void assignServer(DMSession so, String servername, String envname) throws ApiException
 {
  Server server = getServerFromNameOrID(so, servername);
  Environment env = getEnvironmentFromNameOrID(so, envname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  so.MoveServer(env.getId(), server.getId(), 0, 0);
 }

 private void assignApplication(DMSession so, String appname, String envname, HttpServletRequest request) throws ApiException
 {
  String newname = request.getParameter("name"); // if specified, name of new application version
  if (newname != null)
   appname = newname;

  String newenv = request.getParameter("env"); // if specified, name of new application version
  if (newenv != null)
   envname = newenv;

  Environment env = getEnvironmentFromNameOrID(so, envname);
  Application app = getApplicationFromNameOrID(so, appname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  if (app.getParentId() > 0)
   so.setAppInEnv(env, app, "", -1);
  else
   so.addToAppsAllowedInEnv(env, app);
 }

 private void assignUser(DMSession so, String username, String groupname) throws ApiException
 {
  User user = getUserFromNameOrID(so, username);
  UserGroup group = getGroupFromNameOrID(so, groupname);
  so.AddUserToGroup(group.getId(), user.getId());
 }

 private void deAssignServer(DMSession so, String servername, String envname) throws ApiException
 {
  Server server = getServerFromNameOrID(so, servername);
  Environment env = getEnvironmentFromNameOrID(so, envname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  so.RemoveServerFromEnvironment(env.getId(), server.getId());
 }

 private void deAssignApplication(DMSession so, String appname, String envname) throws ApiException
 {
  Environment env = getEnvironmentFromNameOrID(so, envname);
  Application app = getApplicationFromNameOrID(so, appname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  so.removeFromAppsAllowedInEnv(env, app);
 }

 private void deAssignUser(DMSession so, String username, String groupname) throws ApiException
 {
  User user = getUserFromNameOrID(so, username);
  UserGroup group = getGroupFromNameOrID(so, groupname);
  so.RemoveUserFromGroup(group.getId(), user.getId());
 }

 private Application newAppVersion(DMSession so, String appname, HttpServletRequest request) throws ApiException
 {
  String name = request.getParameter("name");
  String domain = request.getParameter("domain");
  String fullname = "";

  if (name != null)
   fullname = name;

  if (domain != null)
   fullname = domain + "." + fullname;

  createDomains4Obj(so, fullname);

  Application app = this.getApplicationFromNameOrID(so, appname);
  boolean isRelease = false;

  if (app.getIsRelease().equalsIgnoreCase("Y"))
   isRelease = true;

  int verid = so.applicationNewVersion(app.getId(), 100, 100, isRelease);
  app = so.getApplication(verid, false);

  if (name != null)
  {
   SummaryChangeSet changes = new SummaryChangeSet();
   changes.add(SummaryField.NAME, name);
   so.updateApplication(app, changes);
  }

  return app;
 }

 private Application newAppVersion(DMSession so, String appname, HttpServletRequest request, TaskCreateVersion tcv) throws ApiException
 {
  System.out.println("DOING newappver");
  String newname = request.getParameter("name"); // if specified, name of new application version
  String domain = request.getParameter("domain");

  if (newname != null)
  {
   if (domain != null)
    newname = domain + "." + newname;

   System.out.println("Looking up " + newname);
   try
   {
    so.getApplicationByName(newname); // throws exception if not found
    // If we get here then application with that name was found. That's an error
    throw new ApiException("An application version with that name already exists");
   }
   catch (RuntimeException ex)
   {
    // Application not found exception - ok to proceed
   }
  }

  if (domain != null)
   appname = domain + "." + appname;

  createDomains4Obj(so, appname);

  Application app = getApplicationFromNameOrID(so, appname);
  //
  // At this juncture, application is set. We need to get the engine
  // to create the new application version. We therefore need to get and
  // run the "CreateVersion" task.
  //
  if (tcv == null)
  {
   // Create Version Task not specified - find one
   Task t = so.getTaskByType(app.getDomain(), TaskType.CREATE_VERSION);
   if (t == null)
    throw new ApiException("Could not find appropriate Create Version task");
   tcv = so.getTaskCreateVersion(t.getId());
  }

  int pid = app.getParentId();
  if (pid == 0)
  {
   // No parent. This must be a BASE application
   tcv.setApplication(app);
  }
  else
  {
   Application parentapp = so.getApplication(pid, false);
   tcv.setApplication(parentapp);
  }
  tcv.setPredecessor(app); // The version this new version will "hang off"
  if (tcv.run() == false)
  {
   // Version creation failed
   throw new ApiException("Version Creation Failed: " + tcv.getOutput());
  }
  String result = tcv.getOutput();
  //
  // result will be Created new Application Version demoapp;7 (id 180)
  //
  System.out.println("result=[" + result + "]");
  if (result.contains("Created new Application Version"))
  {
   // If we've had a success indication then this message should
   // match the "Created new Application Version" string but let's
   // be safe.
   String appNameAndVersion = result.substring(32);
   System.out.println("appNameAndVersion=[" + appNameAndVersion + "]");
   //
   // Now, the name could (in theory) have brackets in it. So let's
   // work backwards from the end of the string to find the id.
   //
   int idstart = appNameAndVersion.lastIndexOf("(id ") + 4;
   int idend = appNameAndVersion.lastIndexOf(")");
   int newid = 0;
   try
   {
    newid = Integer.parseInt(appNameAndVersion.substring(idstart, idend));
   }
   catch (NumberFormatException e)
   {
    newid = 0;
   }
   System.out.println("newid=" + newid);
   //
   // Okay, now get the new application and output it as JSON
   //
   Application newapp = so.getApplication(newid, false);
   if (newapp == null)
   {
    throw new ApiException("New Application with id " + newid + " not found");
   }
   if (newname != null)
   {
    // Need to rename the application
    System.out.println("Changing name of app with id " + newid + " to " + newname);
    SummaryChangeSet changes = new SummaryChangeSet();
    changes.add(SummaryField.NAME, newname);
    so.updateApplication(newapp, changes);
    newapp = so.getApplication(newid, false);
   }
   return newapp;
  }
  else
  {
   return (Application) null;
  }
 }

 private Component newCompVersion(DMSession so, String compname, HttpServletRequest request) throws ApiException
 {
  Component comp = null;
  String newname = request.getParameter("name"); // if specified, name of new application version
  if (newname != null)
   compname = newname;

  try
  {
   comp = getComponentFromNameOrID(so, compname);
  }
  catch (ApiException e)
  {
   ObjectTypeAndId otid = null;
   int newid = so.getID("component");
   String[] parts = compname.split("\\.");
   ArrayList<String> domparts = new ArrayList<String>(Arrays.asList(parts));
   compname = domparts.get(domparts.size()-1);
   domparts.remove(domparts.size() - 1);
   String domname = StringUtils.join(domparts,".");

   createDomains4Obj(so, domname + "." + compname);

   Domain dom = so.getDomainByName(domname);

   if (parts == null || parts.length == 1)
    throw new ApiException("Component name needs to include full domain path");


   otid = so.CreateNewObject("component",compname,dom.getId(),0,newid,0,0,"");
   ArrayList<DMObject> cats = (ArrayList<DMObject>) so.GetCategories();
   int catid = 10;
   for (int i=0;i<cats.size();i++)
   {
    if (cats.get(i).getName().equalsIgnoreCase("General"))
    {
     catid = cats.get(i).getId();
     break;
    }
   }
   so.addToCategory(catid,otid,true);
   String id = otid.toString().substring(2);
   comp = getComponentFromNameOrID(so, id);
   if (comp.getComptype() == null || comp.getComptype().isEmpty())
   {
    CompType ct = so.getCompTypeByName("Kubernetes");
    if (ct != null)
    {
     comp.setComptype(ct.getName());
     comp.setComptypeId(ct.getId());
     so.updateComponentCompType(comp);
    }
   }
   return comp;
  }

  if (comp.getPredecessorId() == 0)
  {
   // This is a BASE version
  }
  // Get children and find
  int newid = so.componentNewVersion(comp.getId(), comp.getXpos(), comp.getYpos() + 50);
  System.out.println("newid=" + newid);
  Component newComp = so.getComponent(newid, true);
  if (newComp == null)
  {
   throw new ApiException("New Component with id " + newid + " not found");
  }

  Component baseComp = comp;
  // Find Base component
  while (baseComp.getPredecessorId() != 0)
  {
   baseComp = so.getComponent(baseComp.getPredecessorId(), false);
  }
  so.componentAddVersionDependency(baseComp.getId(), comp.getId(), newComp.getId());
  return newComp;
 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  //
  // Handles API interface (D) = documented
  //
  // (D) API/login?user=admin&pass=admin
  // (D) API/environments?all=y|n[&application=appname|appid] (lists all environments - all = show sub-domains)
  // (D) API/environment/envid|envname (lists an individual environment)
  // (D) API/applications?all=y|n (lists all applications - all = show sub-domains)
  // (D) API/application/appid|appname (lists an individual application)
  // (D) API/application/appid|appname?[latest=y&][branch=<name>] (gets latest app version, on branch if specified)
  // (D) API/servers?all=y|n[&environment=envname|envid]
  // (D) API/server/<servername>
  // (D) API/component/compid|compname?[latest=y&][branch=<name>] (gets latest comp version, on branch if specified)
  // (D) API/calendar?[env=<env>&app=<app>][&start=<start>&end=<end>]
  // API/newappver/appid|appname[&taskname=<name>]
  // API/newcompver/compid|compname
  // API/replace/application/component/newcomponent
  // (D) API/deploy/appname?environment=envname&wait=Y|N
  // (D) API/log?id=nnn or...
  // (D) API/log/nnn
  // objname can be fully qualified to differentiate between objects with same name in different domains
  // (D) API/setvar/<objtype>/<objname>?name=<name>&value=<value>
  // getvar gets either a single value or all name/value pairs set against an object
  // (D) API/getvar/<objtype>/<objname>/[&name=<name>]
  // (D) API/del/user/name
  // (D) API/new/user/name[?domain=<domain>&realname=<real name>&ldap=<ldap datasource>&tel=<tel no>&email=<email>&cpw=Y|N&locked=Y|N&pw=<password>]
  // (D) API/mod/user/name[?domain=<domain>&realname=<real name>&ldap=<ldap datasource>&tel=<tel no>&email=<email>&cpw=Y|N&locked=Y|N&pw=<password>]
  // (D) API/del/server/name
  // (D) API/new/server/name[?domain=<domain>&env=<environment>&<type=<type>&hostname=<hostname>&protocol=<protocol>&basedir=<basedir>&credname=<credname>&autoping=Y|N&automd5=Y|N&comptype=<list of comp
  // types>&pinginterval=<pinginterval>&pingstart=<starttime>&pingend=<pingend>&pingtemplate=<tmpl>&md5template=<tmpl>&sshport=<port>
  // (D) API/mod/server/name/ " " "
  // API/del/environment/name
  // API/new/environment/name[?domain=<domain>&summary=<summary>]
  // API/mod/environment/name/ " "
  // API/new/appver/appname[?name=<vername>]
  // API/new/compver/compname[?name=<vername>]
  // (D) API/clone/server/<servername>/<newname>[?domain=dom]
  // (D) API/clone/environment/<envname>/<newname?[?domain=dom]
  // (D) API/assign/server/<servername>/<environmentname>
  // (D) API/assign/application/<appname>/<environmentname>
  // (D) API/assign/user/<username>/<groupname>
  // (D) API/unassign/server/<servername>/<environmentname>
  // (D) API/unassign/application/<appname>/<environmentname>
  // (D) API/unassign/user/<username>/<groupname>
  // (D) API/approve/<appname>?taskname=<name>[&notes=<notes>&approve=Y|N]
  // API/buildid/<compname>/<buildid>
  // API/buildnotify/buildurl=<url>

  String path = request.getPathInfo();
  if (path.length() > 1 && (path.charAt(0) == '/'))
  {
   path = path.substring(1);
  }

  String[] elements = path.split("/");
  // System.out.println("elements[0] = " + elements[0]);
  for (int i = 1; i < elements.length; i++)
  {
   System.out.println("Before elements[" + i + "]=[" + elements[i] + "]");
   elements[i] = java.net.URLDecoder.decode(elements[i], "UTF-8");
   System.out.println("After elements[" + i + "]=[" + elements[i] + "]");
  }

  try (DMSession so = DMSession.getInstance(request))
  {
   JSONObject obj = new JSONObject();

   int deploymentid = 0; // for deployments

   PrintWriter out = null;
   if (!request.getPathInfo().contains("helmchart"))
    out = response.getWriter();

   if (elements[0].equals("doms4user"))
   {
    String[] parts = path.split("/");
    String username = parts[parts.length-1];

    JSONArray doms = so.getDomains4User(username);

    obj.add("domains", doms);
    out.write(doms.getJSON());
    return;
   }

   boolean delop = false;

   try
   {
    if (elements.length < 1)
    {
     throw new ApiException("Invalid request path");
    }

    String user = request.getParameter("user");
    String pass = request.getParameter("pass");
    String provider = request.getParameter("provider");
    String a = request.getParameter("all");
    boolean all = (a == null) ? false : (a.charAt(0) == 'y' || a.charAt(0) == 'Y');

    if (provider == null)
     provider = "";

    boolean authorized = false;
    if (user == null)
    {
     user = ServletUtils.GetCookie(request,"p1");
     if (user != null)
      user = URLDecoder.decode(user);
    }

    String jwt = ServletUtils.GetCookie(request, "token");
    if (jwt != null && jwt.trim().length() > 0)
    {
     Jws<Claims> token = JWTGenerateValidateRSA.parseJwt(jwt);
     String uuid = token.getBody().getId();
     String userid = token.getBody().getSubject();
     authorized = so.validateAuth(Integer.parseInt(userid), uuid);
    }

    if (pass == null)
     pass=ServletUtils.GetCookie(request,"p2");

    response.setContentType("application/json");

    if (!authorized)
    {
     if (elements[0].equals("login"))
     {
      if (user == null)
      {
       throw new ApiException("user must be specified");
      }
      if (pass == null)
      {
       throw new ApiException("password must be specified");
      }

      if (provider != null && provider.length() > 0)
      {
       String j = so.jsonGetRequest(pass, provider);
       if (j == null)
        throw new ApiException("Login failed");

       if (provider.equals("github"))
       {
        JsonObject json = new JsonParser().parse(j).getAsJsonObject();
        String u = json.getAsJsonObject("data").get("alias").getAsString();

        if (!user.equals(u))
         throw new ApiException("Login failed");
       }
       pass = "";
      }
      else
      {
       if (so.Login(user, pass, null).getExceptionType() != LoginExceptionType.LOGIN_OKAY)
       {
        throw new ApiException("Login failed");
       }
      }
      jwt = so.authUser();
      obj.add("success", true);
      obj.add("token", jwt);

      HttpSession session = request.getSession();
      session.setAttribute("session", so);

      if (user == null)
       user = "";

      Cookie loggedinUser = new Cookie("p1", URLEncoder.encode(user));
      Cookie loggedinPw = new Cookie("p2", "");
      Cookie loggedinTime = new Cookie("p3", new Long(new Date().getTime()).toString());
      Cookie jwt_token = new Cookie("token", jwt);

      loggedinUser.setPath("/");
      loggedinPw.setPath("/");
      loggedinTime.setPath("/");
      jwt_token.setPath("/");

      response.addCookie(loggedinUser);
      response.addCookie(loggedinPw);
      response.addCookie(loggedinTime);
      response.addCookie(jwt_token);
      return; // finally will send result
     }

     if (user != null)
     {
      if (pass == null)
       throw new ApiException("password must be specified");
      if (so.Login(user, pass, null).getExceptionType() != LoginExceptionType.LOGIN_OKAY)
      {
       throw new ApiException("Login failed");
      }
     }

     if (so.Login(user, pass, null).getExceptionType() != LoginExceptionType.LOGIN_OKAY)
     {
      throw new ApiException("Login failed");
     }
    }
    else if (elements[0].equals("provides"))
    {
      String[] parts = path.split("/");
      String compstr = parts[parts.length-1];
      int compid = new Integer(compstr).intValue();

      obj.add("data",so.getCompProvides(compid, request, response));
    }
    else if (elements[0].equals("consumes"))
    {
      String[] parts = path.split("/");
      String compstr = parts[parts.length-1];
      int compid = new Integer(compstr).intValue();

      obj.add("data",so.getCompConsumes(compid, request, response));
    }
    else if (elements[0].equals("apps2s"))
    {
      String[] parts = path.split("/");
      String compstr = parts[parts.length-1];
      int appid = new Integer(compstr).intValue();

      JSONArray arr = so.getAppService2Service(appid, request, response);
      obj.add("data", arr);
    }
    else if (elements[0].equals("appedge"))
    {
      String[] parts = path.split("/");
      String compstr = parts[parts.length-1];
      int appid = new Integer(compstr).intValue();

      JSONArray arr = so.getAppEdgeBundle(appid, request, response);
      obj.add("data", arr);
    }
    else if (elements[0].equals("appfilter"))
    {
      String[] parts = path.split("/");
      String filter = parts[parts.length-1];

      obj.add("data",so.getApplicationFilter(filter));
    }
    else if (elements[0].equals("compfilter"))
    {
      String[] parts = path.split("/");
      String filter = parts[parts.length-1];

      obj.add("data",so.getComponentFilter(filter));
    }
    else if (elements[0].equals("envfilter"))
    {
      String[] parts = path.split("/");
      String filter = parts[parts.length-1];

      obj.add("data",so.getEnvironmentFilter(filter));
    }
    else if (elements[0].equals("versionlist"))
    {
     JSONArray arr = new JSONArray();
      String[] parts = path.split("/");
      String objid = parts[parts.length-1];

      if (objid.startsWith("ap") || objid.startsWith("av"))
      {
       Application app = this.getApplicationFromNameOrID(so, objid.substring(2));
       String parentid = String.valueOf(app.getParentId());

       if (app.getParentId() == 0)
        parentid = String.valueOf(app.getId());

       app = this.getApplicationFromNameOrID(so, parentid);
       app = so.getApplication(app.getId(), true);
       arr = so.getApplicationVersionsDropDown(app);
       }
      else
      {
       Component comp = this.getComponentFromNameOrID(so, objid.substring(2));
       String parentid = String.valueOf(comp.getParentId());

       if (comp.getParentId() == 0)
         parentid = String.valueOf(comp.getId());

       comp = this.getComponentFromNameOrID(so, parentid);
       ArrayList<Component> versions = (ArrayList<Component>) so.getComponentVersions(comp);

       JSONObject o = new JSONObject();
       o.add("id", comp.getOtid().toString());
       o.add("name", comp.getName());

       arr.add(o);
       for (int i=0;i<versions.size(); i++)
       {
        Component ver = versions.get(i);
        o = new JSONObject();
        o.add("id", ver.getOtid().toString());
        o.add("name", ver.getName());
        arr.add(o);
       }
      }
      obj.add("data", arr);
    }
    else if (elements[0].equals("apps4comp"))
    {
     String[] parts = path.split("/");
     int compid = Integer.valueOf(parts[parts.length-1]);
     JSONArray arr = so.getApps4Comp(compid);
     obj.add("data", arr);
    }
    else if (elements[0].equals("deploy"))
    {
     // Go invoke a deployment
     System.out.println("API: deploy");
     try
     {
      deploymentid = doDeployment(so, elements, request);
     }
     catch (ApiException e)
     {
      obj.add("deploymentid", -1);
      obj.add("success", false);
      obj.add("error", e.getMessage());
      return;
     }
    }
    else if (elements[0].equals("environment"))
    {
     System.out.println("API: Environment");
     if (elements.length == 1)
     {
      String newname = request.getParameter("name"); // if specified, name of new application version

      if (newname != null)
      {
       String[] newparms = new String[elements.length + 1];
       for (int x=0;x<elements.length;x++)
         newparms[x] = elements[x];
       newparms[elements.length] = newname;
       elements = newparms;
      }
     }

     if (elements.length == 2)
     {
      Environment env = getEnvironmentFromNameOrID(so, elements[1]);
      System.out.println(env.getName());
      obj.add("result", assembleJSONForEnvironment(so, env));
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("environments"))
    {
     System.out.println("DOING environments");
     if (elements.length == 1)
     {
      System.out.println("length is 1");
      String appname = request.getParameter("application");
      JSONArray result = new JSONArray();
      if (appname != null)
      {
       System.out.println("Doing envlist for application");
       // Getting environments for application
       Application app = getApplicationFromNameOrID(so, appname);
       System.out.println("Application is " + app.getName());
       List<Environment> envs = so.getEnvironmentsForApplication(app, app.getDomain());
       System.out.println("environment list is size " + envs.size());
       for (Environment e : envs)
       {
        Environment env = so.getEnvironment(e.getId(), true);
        JSONObject je = assembleJSONForEnvironment(so, env);
        result.add(je);
       }
      }
      else
      {
       System.out.println("Application is null");
       List<DMObject> envs = so.getDMObjects(ObjectType.ENVIRONMENT, all);
       System.out.println("environment list is size " + envs.size());
       for (DMObject env : envs)
       {
        JSONObject je = assembleJSONForEnvironment(so, (Environment) env);
        result.add(je);
       }
      }
      obj.add("result", result);
     }
     else
     {
      throw new ApiException("Path contains too many elements");
     }
    }
    else if (elements[0].equals("compassigned2app"))
    {
     if (elements.length == 3)
     {
      int appid = Integer.parseInt(elements[1]);
      int compid = Integer.parseInt(elements[2]);
      boolean iscompAssigned2App = false;
      List<Component> complist = so.getComponents(ObjectType.APPLICATION, appid, false);

      for (int i=0; i < complist.size(); i++)
      {
       Component comp = complist.get(i);
       if (compid == comp.getId())
       {
        iscompAssigned2App = true;
        break;
       }
      }

      obj.add("result", iscompAssigned2App);
     }
    }
    else if (elements[0].equals("components"))
    {
     if (elements.length == 1)
     {
      System.out.println("length is 1");
      String appname = request.getParameter("application");
      JSONArray result = new JSONArray();
      if (appname != null)
      {
       System.out.println("Doing complist for application");
       // Getting components for application
       Application app = getApplicationFromNameOrID(so, appname);
       System.out.println("Application is " + app.getName());
       List<Component> comps = so.getComponents(ObjectType.APPLICATION, app.getId(), false);
       System.out.println("component list is size " + comps.size());
       for (Component c : comps)
       {
        Component comp = so.getComponent(c.getId(), true);
        JSONObject je = assembleJSONForComponent(so, comp, false);
        result.add(je);
       }
      }
      else
      {
       System.out.println("Application is null");
       List<DMObject> comps = so.getDMObjects(ObjectType.COMPONENT, all);
       System.out.println("component list is size " + comps.size());
       for (DMObject comp : comps)
       {
        JSONObject je = assembleJSONForComponent(so, (Component) comp, false);
        result.add(je);
       }
      }
      obj.add("result", result);
     }
    }
    else if (elements[0].equals("compids"))
    {
     if (elements.length == 1)
     {
      System.out.println("length is 1");
      int appid = Integer.parseInt(request.getParameter("appid"));
      JSONObject result = new JSONObject();

       List<Component> comps = so.getComponents(ObjectType.APPLICATION, appid, false);
       System.out.println("component list is size " + comps.size());
       for (Component c : comps)
       {
        result.add(Integer.toString(c.getId()), c.getFullName());
       }
       obj.add("result", result);
      }
    }
    else if (elements[0].equals("basecomponent"))
    {
     System.out.println("API: basecomponent");
     if (elements.length == 1)
     {
      String newname = request.getParameter("name"); // if specified, name of new application version
      if (newname != null)
      {
       String[] newparms = new String[elements.length + 1];
       for (int x=0;x<elements.length;x++)
         newparms[x] = elements[x];
       newparms[elements.length] = newname;
       elements = newparms;
      }
     }

     if (elements.length == 2)
     {
      String compvariant = request.getParameter("compvariant");
      String compversion = request.getParameter("compversion");
      String compname = elements[1];

      if (compvariant != null)
       compname += ";" + compvariant;

      if (compversion != null)
       compname += ";" + compversion;

      elements[1] = compname;

      Component comp = getComponentFromNameOrID(so, elements[1]);

      while (comp.getPredecessorId() > 0)
       comp = getComponentFromNameOrID(so, new Integer(comp.getPredecessorId()).toString());

      obj.add("result", assembleJSONForComponent(so, comp, true));
     }
    }
    else if (elements[0].equals("helmchart"))
    {
     if (elements.length == 2)
     {
      String deployment = elements[1];
      String format = (request.getParameter("format") == null) ? "zip" : request.getParameter("format");

      obj = so.getHelmChart(new Integer(deployment).intValue(), format);

      if (format.equalsIgnoreCase("zip"))
      {
       JsonObject zipdata = new JsonParser().parse(obj.getJSON()).getAsJsonObject();
       if (zipdata.get("files") != null)
       {
        JsonArray files = (zipdata.get("files") != null) ? (JsonArray) zipdata.get("files") : null;

        if (files != null)
        {
         Path temppath = Files.createTempDirectory("helmzip-");
         String tempdir = temppath.toString() + "/";

         for (int i = 0; i < files.size(); i++)
         {
          JsonObject fileobj = files.get(i).getAsJsonObject();
          String filename = (fileobj.get("filename") != null) ? fileobj.get("filename").getAsString() : "";
          filename = tempdir + filename;
          String data = (fileobj.get("data") != null) ? fileobj.get("data").getAsString() : "";

          Path pathToFile = Paths.get(filename);
          Files.createDirectories(pathToFile.getParent());
          Files.createFile(pathToFile);
          System.out.println(filename);
          byte[] decoded_data = Base64.getDecoder().decode(data);

          try (OutputStream stream = new FileOutputStream(filename))
          {
           stream.write(decoded_data);
          }
          catch (Exception e)
          {
           System.err.println("Couldn't write to file...");
          }
         }

         zipdata.remove("files");
         Gson gson = new GsonBuilder().setPrettyPrinting().create();

         FileWriter chartfile = new FileWriter(tempdir + "charts.json");
         chartfile.write(gson.toJson(zipdata));
         chartfile.close();

         File directoryToCompress = new File(tempdir);
         File outputDirectory = new File(tempdir);
         byte[] zip = zipDirectory(directoryToCompress, outputDirectory);
         response.setContentType("application/zip");
         response.setHeader("Content-Disposition", "attachment; filename=\"helmcharts.zip\"");
         ServletOutputStream sos = response.getOutputStream();
         sos.write(zip);
         sos.flush();
         sos.close();

         Files.walk(temppath).sorted(Comparator.reverseOrder()).map(Path::toFile).forEach(File::delete);
        }
       }
       obj = new JSONObject();
      }
      else
      {
       out = response.getWriter();
       out.print(obj.getJSON());
      }
      delop = true;
      return;
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("comp4tag"))
    {
     System.out.println("API: comp4tag");
     if (elements.length == 1)
     {
      String image = request.getParameter("image"); // if specified, name of new application version
      String digest = request.getParameter("digest");

      int compid = -1;

      if (image != null)
       compid = so.getComp4Tag(so, image);

      if (digest != null)
       compid = so.getComp4Digest(so, digest);

      obj.add("id", compid);
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("component"))
    {
     boolean idonly  = false;
     if (request.getParameter("idonly") != null && request.getParameter("idonly").equalsIgnoreCase("Y"))
        idonly = true;

     System.out.println("API: component");
     if (elements.length == 1)
     {
      String newname = request.getParameter("name"); // if specified, name of new application version
      if (newname != null)
      {
       String[] newparms = new String[elements.length + 1];
       for (int x=0;x<elements.length;x++)
         newparms[x] = elements[x];
       newparms[elements.length] = newname;
       elements = newparms;
      }
     }

     if (elements.length == 2)
     {
      String compvariant = request.getParameter("compvariant");
      String compversion = request.getParameter("compversion");
      String compname = elements[1];

      if (compvariant != null)
       compname += ";" + compvariant;

      if (compversion != null)
       compname += ";" + compversion;

      elements[1] = compname;

      Component comp = getComponentFromNameOrID(so, elements[1]);
      System.out.println(comp.getName());
      // Check for "latest" and "branch" options
      String lstr = request.getParameter("latest");
      String branchname = request.getParameter("branch");
      boolean latest = (lstr != null && lstr.equalsIgnoreCase("Y"));
      if (latest)
      {
       Component lcomp = so.getLatestVersion(comp, branchname);
       if (lcomp != null)
       {
        obj.add("result", assembleJSONForComponent(so, lcomp, idonly));
       }
       else
       {
        throw new ApiException("No Component Found");
       }
      }
      else
      {
       obj.add("result", assembleJSONForComponent(so, comp, idonly));
      }
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("application"))
    {
     boolean idonly  = false;
     boolean lastsuccess = false;

     if (request.getParameter("idonly") != null && request.getParameter("idonly").equalsIgnoreCase("Y"))
        idonly = true;

     if (request.getParameter("lastsuccess") != null && request.getParameter("lastsuccess").equalsIgnoreCase("Y"))
      lastsuccess = true;

     int envid = -1;
     if (request.getParameter("envid") != null)
     {
      envid =  Integer.parseInt(request.getParameter("envid"));
     }

     System.out.println("API: Application");
     if (elements.length == 1)
     {
      String newname = request.getParameter("name"); // if specified, name of new application version

      if (newname != null)
      {
       String[] newparms = new String[elements.length + 1];
       for (int x=0;x<elements.length;x++)
         newparms[x] = elements[x];
       newparms[elements.length] = newname;
       elements = newparms;
      }
     }

     if (elements.length == 2)
     {
      Application app = getApplicationFromNameOrID(so, elements[1]);
      System.out.println(app.getName());
      // Check for "latest" and "branch" options
      String lstr = request.getParameter("latest");
      String branchname = request.getParameter("branch");
      boolean latest = (lstr != null && lstr.equalsIgnoreCase("Y"));
      if (latest)
      {
       Application lapp = so.getLatestVersion(app, branchname);
       if (lapp != null)
       {
        obj.add("result", assembleJSONForApplication(so, lapp, idonly));
       }
       else
       {
        throw new ApiException("No Application Found");
       }
      }
      else if (lastsuccess)
      {
       Application lapp = so.getLastSuccessfulVersion(app, envid);
       if (lapp != null)
       {
        obj.add("result", assembleJSONForApplication(so, lapp, idonly));
       }
       else
       {
        throw new ApiException("No Application Found");
       }
      }
      else
      {
       obj.add("result", assembleJSONForApplication(so, app, idonly));
      }
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }

    }
    else if (elements[0].equals("credential"))
    {
     System.out.println("API: Credential");
     if (elements.length == 2)
     {
      Credential cred = getCredentialFromNameOrID(so, elements[1]);
      System.out.println(cred.getName());
      // Check for "latest" and "branch" options
      obj.add("result", assembleJSONForCredential(so, cred));
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("action"))
    {
     System.out.println("API: Action");
     if (elements.length == 2)
     {
      Action act = getActionFromNameOrID(so, elements[1]);
      System.out.println(act.getName());
      // Check for "latest" and "branch" options
      obj.add("result", assembleJSONForAction(so, act));
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("domains"))
    {
     System.out.println("API: domains");
     String domlist = so.getDomainList();
     System.out.println(domlist);
     obj.add("result", domlist);
    }
    else if (elements[0].equals("server"))
    {
     System.out.println("API: Server");
     if (elements.length == 2)
     {
      Server srv = getServerFromNameOrID(so, elements[1]);
      System.out.println(srv.getName());
      // Check for "latest" and "branch" options
      obj.add("result", assembleJSONForServer(so, srv));
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("testserver"))
    {
     System.out.println("API: TestServer");
     if (elements.length == 2)
     {
      Server srv = getServerFromNameOrID(so, elements[1]);
      System.out.println(srv.getName());
      // Check for "latest" and "branch" options
      obj.add("result", assembleJSONForTestServer(so, srv));
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("servers"))
    {
     System.out.println("DOING servers");
     if (elements.length == 1)
     {
      System.out.println("length is 1");
      List<DMObject> srvs = so.getDMObjects(ObjectType.SERVER, all);
      System.out.println("application list is size " + srvs.size());
      JSONArray result = new JSONArray();
      for (DMObject dob : srvs)
      {
       Server srv = (Server) dob;
       JSONObject je = assembleJSONForServer(so,srv);
       result.add(je);
      }
      obj.add("result", result);
     }
     else
     {
      throw new ApiException("Path contains too many elements");
     }
    }
    else if (elements[0].equals("applications"))
    {
     System.out.println("DOING applications");
     if (elements.length == 1)
     {
      System.out.println("length is 1");
      List<DMObject> apps = so.getDMObjects(ObjectType.APPLICATION, all);
      System.out.println("application list is size " + apps.size());
      JSONArray result = new JSONArray();
      for (DMObject dob : apps)
      {
       Application app = (Application) dob;
       JSONObject je = new JSONObject();
       je.add("id", app.getId());
       je.add("name", app.getName());
       je.add("domain", app.getDomain().getFullDomain());
       je.add("parentid", app.getParentId());
       je.add("predecessorid", app.getPredecessorId());
       je.add("summary", app.getSummary());
       DMObject owner = app.getOwner();
       if (owner != null)
       {
        if (owner.getObjectType() == ObjectType.USER)
         je.add("owneruser", owner.getName());
        if (owner.getObjectType() == ObjectType.USERGROUP)
         je.add("ownergroup", owner.getName());
       }
       result.add(je);
      }
      obj.add("result", result);
     }
     else
     {
      throw new ApiException("Path contains too many elements");
     }
    }
    else if (elements[0].equals("saasclient"))
    {
     SaasClient client = so.getSaasClientByName(elements[1]);
     if (client == null)
     {
      throw new ApiException("Could not find Saas Client for " + elements[1]);
     }
     else
     {
      obj.add("result", assembleJSONForSaasClient(so, client));
     }
    }
    else if (elements[0].equals("domain"))
    {
     Domain domain = getDomainFromNameOrID(so, elements[1]);
     if (domain == null)
     {
      throw new ApiException("Could not find domain");
     }
     else
     {
      obj.add("result", assembleJSONForDomain(so, domain));
     }
    }
    else if (elements[0].equals("user"))
    {
     User u = this.getUserFromNameOrID(so, elements[1]);
     if (u == null)
     {
      throw new ApiException("Could not find user");
     }
     else
     {
      obj.add("result", assembleJSONForUser(so, u));
     }
    }
    else if (elements[0].equals("approve"))
    {
     // Approving an application
     if (elements.length == 2)
     {
      String approve = request.getParameter("approve");
      Application app = getApplicationFromNameOrID(so, elements[1]);
      Task t = null;
      String taskname = request.getParameter("taskname");
      if (taskname == null)
       taskname = request.getParameter("task");
      if (taskname == null)
      {
       // task not specified - find one
       t = so.getTaskByType(app.getDomain(), TaskType.APPROVE);
       if (t == null)
        throw new ApiException("Could not find appropriate Approve Task");
      }
      else
      {
       // task specified - use that
       System.out.println("Looking up task " + taskname);
       try
       {
        t = so.getTaskByName(taskname); // throws exception if not found or no access
        if (t.getTaskType() != TaskType.APPROVE)
         throw new ApiException("Specified task is not an Approve task");
       }
       catch (Exception ex)
       {
        throw new ApiException(ex.getMessage());
       }
      }
      TaskApprove tap = so.getTaskApprove(t.getId());
      if (tap != null)
      {
       tap.setApplication(app);
       if (approve != null && approve.equalsIgnoreCase("n"))
       {
        tap.setApproved(false);
       }
       else
       {
        tap.setApproved(true);
       }
       String notes = request.getParameter("notes");
       if (notes != null)
       {
        tap.setText(notes);
       }
       else
       {
        tap.setText("");
       }
       if (tap.run() == false)
       {
        throw new ApiException("Approval Task Failed: " + tap.getOutput());
       }
      }
      else
      {
       throw new ApiException("Cannot get approve task " + t.getId());
      }
     }
     else
     {
      throw new ApiException("Path contains too many elements");
     }
    }
    else if (elements[0].equals("newappver"))
    {
     // Creating a new application version.
     System.out.println("API: newappver");
     if (elements.length == 2)
     {
      Application newapp = newAppVersion(so, elements[1], request);
      obj.add("result", assembleJSONForApplication(so, newapp, false));
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("newapp"))
    {
     // Creating a new application version.
     System.out.println("API: newapp");
     if (elements.length == 2)
     {
      AddAppBaseVersion(so, null, request);
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements");
     }
    }
    else if (elements[0].equals("newcompver"))
    {
     System.out.println("DOING newcompver");
     if (elements.length == 2)
     {
      Component newComp = newCompVersion(so, elements[1], request);
      obj.add("result", assembleJSONForComponent(so, newComp, false));
     }
     else
     {
      throw new ApiException("Path contains too many elements");
     }
    }
    else if (elements[0].equals("replace"))
    {
     // API/replace/application/component/newcomponent
     if (elements.length == 4)
     {
      Application app = getApplicationFromNameOrID(so, elements[1]);
      Component oldcomp = getComponentFromNameOrID(so, elements[2]);
      Component newcomp = getComponentFromNameOrID(so, elements[3]);
      System.out.println("Replacing component " + oldcomp.getId() + " with " + newcomp.getId() + " for app " + app.getId());
      so.applicationReplaceComponent(app.getId(), oldcomp.getId(), newcomp.getId(), false);
     }
     else
     {
      throw new ApiException("Path contains invalid number of elements");
     }

    }
    else if (elements[0].equals("setaction"))
    {

     System.out.println("DOING setaction");
     if (elements.length == 3)
     {
      String objtype = elements[1];
      String objname = elements[2];

      if (objtype.equals("component"))
      {
       Component comp = getComponentFromNameOrID(so, objname);
       String actiontype = request.getParameter("actiontype");
       String actionname = request.getParameter("actionname");
       Action action = getActionFromNameOrID(so, actionname);

       if (actiontype.equals("pre"))
        comp.setPreAction(action);
       else if (actiontype.equals("post"))
        comp.setPostAction(action);
       else
        comp.setCustomAction(action);

       so.updateComponentAction(comp);
      }
      else if (objtype.equals("application"))
      {
       Application app = getApplicationFromNameOrID(so, objname);
       String actiontype = request.getParameter("actiontype");
       String actionname = request.getParameter("actionname");
       Action action = getActionFromNameOrID(so, actionname);

       if (actiontype.equals("pre"))
        app.setPreAction(action);
       else if (actiontype.equals("post"))
        app.setPostAction(action);
       else
        app.setCustomAction(action);

       so.updateApplicationAction(app);
      }
     }
    }
    else if (elements[0].equals("setvar"))
    {

     System.out.println("DOING setvar");
     if (elements.length == 3)
     {
      String objtype = elements[1];
      String objname = elements[2];
      String varname = request.getParameter("name");
      String varval = request.getParameter("value");
      String delattrs = request.getParameter("delattrs");

      if (varname == null)
       throw new ApiException("name not specified");
      AttributeChangeSet acs = new AttributeChangeSet();
      DMAttribute attr = new DMAttribute();
      attr.setName(varname);
      attr.setValue(varval != null ? varval : "");
      acs.addChanged(attr);

      for (int i = 1; i < 1000; i++)
      {
       varname = request.getParameter("name" + i);
       varval = request.getParameter("value" + i);
       if (varname == null)
        break;

       attr = new DMAttribute();
       attr.setName(varname);
       attr.setValue(varval != null ? varval : "");
       acs.addChanged(attr);
      }

      try
      {
       if (objtype.equalsIgnoreCase("environment"))
       {
        Environment env = getEnvironmentFromNameOrID(so, objname);
        so.updateAttributesForObject(env, acs);
       }
       else if (objtype.equalsIgnoreCase("application"))
       {
        Application app = getApplicationFromNameOrID(so, objname);
        so.updateAttributesForObject(app, acs);
       }
       else if (objtype.equalsIgnoreCase("server"))
       {
        Server serv = getServerFromNameOrID(so, objname);
        ;
        so.updateAttributesForObject(serv, acs);
       }
       else if (objtype.equalsIgnoreCase("component"))
       {
        Component comp = getComponentFromNameOrID(so, objname);

        if (delattrs != null && delattrs.equalsIgnoreCase("y"))
           so.DeleteComponentAttribs(comp.getId());

        so.updateAttributesForObject(comp, acs);
       }
       else
       {
        throw new ApiException("Invalid object type " + objtype);
       }
      }
      catch (RuntimeException ex)
      {
       throw new ApiException(ex.getMessage());
      }
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }
    }
    else if (elements[0].equals("getvar"))
    {
     System.out.println("DOING getvar");
     if (elements.length == 3)
     {
      String objtype = elements[1];
      String objname = elements[2];
      String reqname = request.getParameter("name");
      try
      {
       List<DMAttribute> atts;
       if (objtype.equalsIgnoreCase("environment"))
       {
        Environment env = getEnvironmentFromNameOrID(so, objname);
        atts = env.getAttributes();
       }
       else if (objtype.equalsIgnoreCase("application"))
       {
        Application app = getApplicationFromNameOrID(so, objname);
        atts = app.getAttributes();
       }
       else if (objtype.equalsIgnoreCase("server"))
       {
        Server serv = getServerFromNameOrID(so, objname);
        atts = serv.getAttributes();
       }
       else if (objtype.equalsIgnoreCase("component"))
       {
        Component comp = getComponentFromNameOrID(so, objname);
        atts = comp.getAttributes();
       }
       else
       {
        throw new ApiException("Invalid object type " + objtype);
       }
       if (atts != null)
       {
        JSONArray as = new JSONArray();
        for (int i = 0; i < atts.size(); i++)
        {
         DMAttribute att = atts.get(i);
         if (reqname == null || (reqname != null && reqname.equals(att.getName())))
         {
          JSONObject jo = new JSONObject();
          jo.add(att.getName().replace("\\","\\\\"), att.getValue().replace("\\","\\\\"));
          as.add(jo);
         }
        }
        obj.add("attributes", as);
       }
      }
      catch (RuntimeException ex)
      {
       throw new ApiException(ex.getMessage());
      }
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }

    }
    else if (elements[0].equals("log"))
    {
     System.out.println("DOING deployment log length=" + elements.length);
     int id = 0;
     try
     {
      if (elements.length == 2)
      {
       id = Integer.parseInt(elements[1]);
      }
      else
      {
       String deploylogid = request.getParameter("id");
       if (deploylogid == null)
       {
        throw new ApiException("id not specified");
       }
       id = Integer.parseInt(deploylogid);
      }
     }
     catch (NumberFormatException ex)
     {
      throw new ApiException("id invalid or no access");
     }

     String complete = request.getParameter("checkcomplete");
     if (complete != null && complete.equalsIgnoreCase("Y"))
     {
      Deployment dep = so.getDeployment(id, true);
      if (dep != null)
       obj.add("iscomplete", dep.isComplete());
      else
       obj.add("iscomplete", false);
     }
     else
     {
      if (id == 0 || so.validateDeploymentId(id) == false)
      {
       throw new ApiException("id invalid or no access");
      }
      else
      {

       Deployment dep = so.getDeployment(id, true);
       //
       int envid = dep.getEnvironment().getId();
       int envdomain = so.getEnvironment(envid, true).getDomainId();
       System.out.println("envdomain=" + envdomain);
       if (so.ValidDomain(envdomain, false))
       {
        obj.add("id", dep.getId());
        obj.add("application", dep.getApplication().getName());
        // obj.add("domain",dep.getDomain().getName());
        obj.add("environment", dep.getEnvironment().getName());
        obj.add("exitcode", dep.getExitCode());
        obj.add("exitstatus", dep.getExitStatus());
        obj.add("started", dep.getStarted());
        obj.add("finished", dep.getFinished());
        obj.add("complete", dep.isComplete());

        String filelist = request.getParameter("filelist");

        if (filelist == null || filelist.equalsIgnoreCase("N"))
        {
         List<DeploymentLogEntry> dl = dep.getLog();
         obj.add("loglinecount", dl.size());
         JSONArray logdir = new JSONArray();
         for (int i = 0; i < dl.size(); i++)
         {
          DeploymentLogEntry de = dl.get(i);
          String logline = de.getLine();
          logline = logline.replace("\\r", "");
          logdir.add(logline);
         }
         obj.add("logoutput", logdir);
        }
        else
        {
//         obj.add("files", dep.getFilesListJSON());
        }
       }
       else
       {
        throw new ApiException("id invalid or no access");
       }
      }
     }
    }
    else if (elements[0].equals("list"))
    {
     if (elements.length == 2)
     {
      List<DMObject> objs = null;
      if (elements[1].equals("groups"))
       objs = so.getDMObjects(ObjectType.USERGROUP, true);
      else if (elements[1].equals("users"))
       objs = so.getDMObjects(ObjectType.USER, true);
      else if (elements[1].equals("categories"))
       objs = so.GetCategories();
      else if (elements[1].equals("comptypes"))
       objs = so.getCompTypes();
      else if (elements[1].equals("credentials"))
       objs = so.getCredentials();
      else if (elements[1].equals("endpoints"))
       objs = so.getDMObjects(ObjectType.SERVER, true);
      else if (elements[1].equals("environments"))
       objs = so.getDMObjects(ObjectType.ENVIRONMENT, true);
      else if (elements[1].equals("repositories"))
       objs = so.getDMObjects(ObjectType.REPOSITORY, true);
      else if (elements[1].equals("datasources"))
       objs = so.getDMObjects(ObjectType.DATASOURCE, true);
      else if (elements[1].equals("notifiers"))
       objs = so.getDMObjects(ObjectType.NOTIFY, true);
      else if (elements[1].equals("components"))
       objs = so.getDMObjects(ObjectType.COMPONENT, true);
      else if (elements[1].equals("applications"))
       objs = so.getDMObjects(ObjectType.APPLICATION, true);
      else if (elements[1].equals("releases"))
       objs = so.getDMObjects(ObjectType.RELEASE, true);
      else if (elements[1].equals("domains"))
       objs = so.getDomains();
      JSONArray result = new JSONArray();
      for (DMObject dmobj : objs)
      {
       JSONObject ret = new JSONObject();
       if (dmobj instanceof Category)
         ret = exportJSONForCategory(so, (Category)dmobj);
       else if (dmobj instanceof CompType)
        ret = exportJSONForCompType(so, (CompType)dmobj);
       else if (dmobj instanceof Credential)
        ret = exportJSONForCredential(so, (Credential)dmobj);
       else if (dmobj instanceof Environment)
        ret = exportJSONForEnvironment(so, (Environment)dmobj);
       else if (dmobj instanceof UserGroup)
        ret = exportJSONForGroup(so, (UserGroup)dmobj);
       else if (dmobj instanceof Repository)
        ret = exportJSONForRepository(so, (Repository)dmobj);
       else if (dmobj instanceof Server)
        ret = exportJSONForServer(so, (Server)dmobj);
       else if (dmobj instanceof User)
        ret = exportJSONForUser(so, (User)dmobj);
       else if (dmobj instanceof Component)
        ret = exportJSONForComponent(so, (Component)dmobj);
       else if (dmobj instanceof Action)
        ret = exportJSONForAction(so, (Action)dmobj);
       else if (dmobj instanceof Application)
        ret = exportJSONForApplication(so, (Application)dmobj);
       else if (dmobj instanceof Domain)
        ret = exportJSONForDomain(so, (Domain)dmobj);
       result.add(ret);
      }
      obj.add(elements[1], result);
     }
    }
    else if (elements[0].equals("export"))
    {
     if (elements.length == 2)
     {
      List<DMObject> objs = null;
      if (elements[1].equals("groups"))
       objs = so.getDMObjects(ObjectType.USERGROUP, true);
      else if (elements[1].equals("users"))
       objs = so.getDMObjects(ObjectType.USER, true);
      else if (elements[1].equals("categories"))
       objs = so.GetCategories();
      else if (elements[1].equals("comptypes"))
       objs = so.getCompTypes();
      else if (elements[1].equals("credentials"))
       objs = so.getCredentials();
      else if (elements[1].equals("domains"))
       objs = so.getDomains();
      else if (elements[1].equals("tasks"))
       objs = so.getTasks();
      else if (elements[1].equals("engines"))
       objs = so.getEngines();
      else if (elements[1].equals("endpoints"))
       objs = so.getDMObjects(ObjectType.SERVER, true);
      else if (elements[1].equals("environments"))
       objs = so.getDMObjects(ObjectType.ENVIRONMENT, true);
      else if (elements[1].equals("repositories"))
       objs = so.getDMObjects(ObjectType.REPOSITORY, true);
      else if (elements[1].equals("datasources"))
       objs = so.getDMObjects(ObjectType.DATASOURCE, true);
      else if (elements[1].equals("notifiers"))
       objs = so.getDMObjects(ObjectType.NOTIFY, true);
      else if (elements[1].equals("components"))
       objs = so.getDMObjects(ObjectType.COMPONENT, true);
      else if (elements[1].equals("applications"))
       objs = so.getDMObjects(ObjectType.APPLICATION, true);
      else if (elements[1].equals("releases"))
       objs = so.getDMObjects(ObjectType.RELEASE, true);

      JSONArray result = new JSONArray();
      if (objs != null)
      {

      for (DMObject dmobj : objs)
      {
       JSONObject je = new JSONObject();

       if (dmobj instanceof Category)
       {
        je.add("objtype", ObjectType.CATEGORY.getTypeString());
        je.add("objname", dmobj.getName());
       }
       else
       {
        je.add("objtype", dmobj.getObjectType().getTypeString());

        if (dmobj instanceof Domain)
        {
         Domain dom = (Domain) dmobj;
         if (dom.getParentDomain() == null)
          je.add("objname", dom.getName());
         else
          je.add("objname", dom.getParentDomain() + "." + dom.getName());
        }
        else
         je.add("objname", dmobj.getDomain().getFullDomain() + "." + dmobj.getName());

        if (dmobj instanceof Task)
         je.add("tasktype", ((Task)dmobj).getTaskTypeAsString());

        je.add("summary", dmobj.getSummaryJSON());

        if (dmobj instanceof Environment)
        {
         Environment env = (Environment) dmobj;
         List<Server> sl = so.getServersInEnvironment(env.getId());

         if (sl != null && sl.size() > 0)
         {
          JSONArray arr = new JSONArray();

          for (int i = 0; i < sl.size(); i++)
          {
           Server srv = so.getServer(sl.get(i).getId(), true);

           arr.add(srv.getDomain().getFullDomain() + "." + srv.getName());
          }

          je.add("endpoints", arr);
         }
        }

        if (dmobj instanceof UserGroup)
        {
         UserGroup ug = (UserGroup) dmobj;
         UserList ul = so.getUsersInGroup(ug.getId());

         if (ul != null && ul.size() > 0)
         {
          JSONArray arr = new JSONArray();

          for (int i = 0; i < ul.size(); i++)
          {
           User u = so.getUser(ul.get(i).getId());

           arr.add(u.getDomain().getFullDomain() + "." + ul.get(i).getName());
          }

          je.add("members", arr);
         }
        }

        if (dmobj instanceof Component)
        {
         Component comp = (Component) dmobj;
         List<Component> complist = comp.getVersions();

         if (complist != null && complist.size() > 0)
         {
          JSONArray arr = new JSONArray();

          for (int i = 0; i < complist.size(); i++)
          {
           Component compver = complist.get(i);
           JSONObject ret = new JSONObject();
           ret.add("version", compver.getDomain().getFullDomain() + "." + compver.getName());
           String pname = "";

           Component p = so.getComponent(compver.getPredecessorId(), false);

           if (p != null)
            pname = p.getDomain().getFullDomain() + "." + p.getName();
           ret.add("predecessor", pname);

           pname = "";
           p = so.getComponent(compver.getParentId(), false);
           if (p != null)
            pname = p.getDomain().getFullDomain() + "." + p.getName();
           ret.add("parent", pname);

           arr.add(ret);
          }

          je.add("compvers", arr);
         }

         List<ComponentItem> compitems = so.getComponentItems(comp.getId());

         if (compitems != null && compitems.size() > 0)
         {
          JSONArray arr = new JSONArray();

          for (int i = 0; i < compitems.size(); i++)
          {
           ComponentItem compitem = compitems.get(i);
           compitem.setSession(so);
           arr.add(compitem.getSummaryJSON());
          }

          je.add("compitems", arr);
         }
        }

        if (dmobj instanceof Application)
        {
         Application app = (Application) dmobj;
         List<Application> applist = app.getVersions();

         if (applist != null && applist.size() > 0)
         {
          JSONArray arr = new JSONArray();

          for (int i = 0; i < applist.size(); i++)
          {
           Application appver = applist.get(i);
           JSONObject ret = new JSONObject();
           ret.add("version", appver.getDomain().getFullDomain() + "." + appver.getName());
           String pname = "";

           Application p = so.getApplication(appver.getPredecessorId(), false);

           if (p != null)
            pname = p.getDomain().getFullDomain() + "." + p.getName();
           ret.add("predecessor", pname);

           pname = "";
           p = so.getApplication(appver.getParentId(), false);
           if (p != null)
            pname = p.getDomain().getFullDomain() + "." + p.getName();
           ret.add("parent", pname);

           arr.add(ret);
          }

          je.add("appvers", arr);
         }

         List<Component> complist = so.getComponents(ObjectType.APPLICATION, app.getId(), false);
         List<ComponentLink> complinks = so.getComponentLinks(app.getId());

         if (complist != null && complist.size() > 0)
         {
          JSONArray arr = new JSONArray();

          for (int i = 0; i < complist.size(); i++)
          {
           Component comp = complist.get(i);
           JSONObject ret = new JSONObject();
           ret.add("version", comp.getDomain().getFullDomain() + "." + comp.getName());
           ret.add("xpos", comp.getXpos());
           ret.add("ypos", comp.getYpos());

           if (complinks != null)
           {
            for (int k = 0; k < complinks.size(); k++)
            {
             ComponentLink cl = complinks.get(k);
             if (cl.getCompTo() == comp.getId())
             {
              Component c = so.getComponent(cl.getCompFrom(), false);
              if (c == null)
               ret.add("from", "");
              else
               ret.add("from", c.getDomain().getFullDomain() + "." + c.getName());

              c = so.getComponent(cl.getCompTo(), false);
              if (c == null)
               ret.add("to", "");
              else
               ret.add("to", c.getDomain().getFullDomain() + "." + c.getName());
             }
            }
           }
           arr.add(ret);
          }

          je.add("comps", arr);
         }

         if (app.getObjectType() == ObjectType.RELEASE)
         {
          List<Component> comps = so.getComponents(ObjectType.APPLICATION,app.getId(),true);
          complinks = so.getComponentLinks(app.getId());

          JSONArray arr = new JSONArray();
          for(Component ci: comps) {
           JSONObject vobj = new JSONObject();
           vobj.add("version", ci.getDomain().getFullDomain() + "." + ci.getName());
           vobj.add("xpos", ci.getXpos());
           vobj.add("ypos", ci.getYpos());

           if (complinks != null)
           {
            for (int k = 0; k < complinks.size(); k++)
            {
             ComponentLink cl = complinks.get(k);
             if (cl.getCompTo() == ci.getId())
             {
              Application c = null;
              try
              {
               c = so.getApplication(cl.getCompFrom(), false);
              }
              catch (RuntimeException e)
              {
              }

              if (c == null)
               vobj.add("from", "");
              else
               vobj.add("from", c.getDomain().getFullDomain() + "." + c.getName());

              c = null;
              try
              {
               c = so.getApplication(cl.getCompTo(), false);
              }
              catch (RuntimeException e)
              {
              }

              if (c == null)
               vobj.add("to", "");
              else
               vobj.add("to", c.getDomain().getFullDomain() + "." + c.getName());
             }
            }
           }
           arr.add(vobj);
         }
         je.add("apps", arr);
        }

         if (app.getObjectType() == ObjectType.APPLICATION)
         {
          ArrayList<String> envs = so.getAppVersInEnv(app);

          if (envs != null && envs.size() > 0)
          {
           JSONArray arr = new JSONArray();
           for(int r = 0; r < envs.size(); r++) {
            arr.add(envs.get(r));
           }

           je.add("envs", arr);
          }
         }
        }

        List<DMAttribute> attrs = dmobj.getAttributes();

        if (attrs != null && attrs.size() > 0)
        {
         JSONArray arr = new JSONArray();

         for (int i = 0; i < attrs.size(); i++)
          arr.add(attrs.get(i).toJSON());

         je.add("attrs", arr);
        }

        List<DMProperty> props = null;

        if (dmobj instanceof ProviderObject)
         props = ((ProviderObject) dmobj).getProperties();

        if (props != null && props.size() > 0)
        {
         JSONArray arr = new JSONArray();

         for (int i = 0; i < props.size(); i++)
          arr.add(props.get(i).toJSON());

         je.add("props", arr);
        }
       }
       result.add(je);
      }
     }
      obj.add(elements[1], result);
     }
    }
    else if (elements[0].equals("new"))
    {
     // Adding a new object
     if (elements.length == 2)
     {
      String newname = request.getParameter("name"); // if specified, name of new application version
      if (newname != null)
      {
       String[] new_elements = new String[3];
       new_elements[0] = elements[0];
       new_elements[1] = elements[1];
       new_elements[2] = newname;
       elements = new_elements;
      }
     }

     if (elements.length == 3)
     {
      if (elements[1].equals("user"))
       AddUser(so, elements[2], request);
      else if (elements[1].equals("server"))
       AddServer(so, elements[2], request);
      else if (elements[1].equals("appver"))
       newAppVersion(so, elements[2], request);
      else if (elements[1].equals("compver"))
      {
       Component newComp = newCompVersion(so, elements[2], request);
       obj.add("result", assembleJSONForComponent(so, newComp, false));
      }
      else if (elements[1].equals("environment"))
       AddEnvironment(so, elements[2], request);
      else if (elements[1].equals("domain"))
       AddDomain(so, elements[2], request);
      else if (elements[1].equals("credential"))
       AddCredential(so, elements[2], request);
      else if (elements[1].equals("application"))
       AddAppBaseVersion(so, elements[2], request);
      else if (elements[1].equals("compitem"))
      {
       String compname = request.getParameter("component");
       String xStr = request.getParameter("xpos");
       String yStr = request.getParameter("ypos");
       String removeAllStr = request.getParameter("removeall");
       String repo = request.getParameter("repo");
       String pattern = request.getParameter("pattern");
       String uri = request.getParameter("uri");
       String version = request.getParameter("version");
       String kind = request.getParameter("kind");
       String targetdir = request.getParameter("targetdir");
       int xpos = 0;
       int ypos = 0;
       boolean removeAll = false;

       if (repo == null)
        repo = "";

       if (pattern == null)
        pattern = "";

       if (version == null)
        version = "";

       if (kind == null)
        kind = "file";

       if (uri == null)
        uri = "";

       if (targetdir == null)
        targetdir = "";

       if (xStr == null || xStr.length() == 0)
        xpos = 0;
       else
        xpos = Integer.parseInt(xStr);

       if (yStr == null || yStr.length() == 0)
        ypos = 0;
       else
        ypos = Integer.parseInt(yStr);

       if (removeAllStr != null && removeAllStr.equalsIgnoreCase("Y"))
        removeAll = true;

        ComponentItem ci = AddComponentItems(so, compname, elements[2], xpos, ypos, removeAll, request, targetdir, repo, pattern, uri, version, kind);
        if (ci != null)
        {
        obj.add("result", assembleJSONForComponentItem(so, ci));
        }
      }
      else
       throw new ApiException("Invalid object \"" + elements[1] + "\" for \"new\" operation");

     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }
    }
    else if (elements[0].equals("del"))
    {
     // Deleting an existing object.
     if (elements.length == 3)
     {
      if (elements[1].equals("user"))
      {
       String username = elements[2];
       User usertodel = getUserFromNameOrID(so, username);
       if (!usertodel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("user", usertodel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("application"))
      {
       String appname = elements[2];
       Application apptodel = getApplicationFromNameOrID(so, appname);
       if (!apptodel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("application", apptodel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("release"))
      {
       String appname = elements[2];
       Application apptodel = getApplicationFromNameOrID(so, appname);
       if (!apptodel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("release", apptodel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("component"))
      {
       String appname = elements[2];
       Component comptodel = getComponentFromNameOrID(so, appname);
       if (!comptodel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("component", comptodel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("server") || elements[1].equals("endpoint"))
      {
       String servername = elements[2];
       Server servertodel = getServerFromNameOrID(so, servername);
       if (!servertodel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("server", servertodel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("environment"))
      {
       String envname = elements[2];
       Environment envtodel = getEnvironmentFromNameOrID(so, envname);
       if (!envtodel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("environment", envtodel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("credential"))
      {
       String credname = elements[2];
       Credential credtodel = getCredentialFromNameOrID(so, credname);
       if (!credtodel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("credentials", credtodel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("action") || elements[1].equals("procedure"))
      {
       String name = elements[2];

       if (elements[1].equals("procedure") && name.contains("-"))
       {
        String[] parts = name.split("-");
        name = parts[0];
       }

       Action todel = getActionFromNameOrID(so, name);
       if (!todel .isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("action", todel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("notifier"))
      {
       String name = elements[2];
       Notify todel = getNotifyFromNameOrID(so, name);
       if (!todel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("notify", todel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("template"))
      {
       String name = elements[2];
       NotifyTemplate todel = getTemplateFromNameOrID(so, name);
       if (!todel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("template", todel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("repository"))
      {
       String name = elements[2];
       Repository todel = getRepositoryFromNameOrID(so, name);
       if (!todel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("repository", todel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("datasource"))
      {
       String name = elements[2];
       Datasource todel = getDatasourceFromNameOrID(so, name);
       if (!todel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("datasource", todel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("group"))
      {
       String name = elements[2];
       UserGroup todel = getGroupFromNameOrID(so, name);
       if (!todel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("usergroup", todel.getId(), out, true);
       delop = true;
      }
      else if (elements[1].equals("servercomptype"))
      {
       String name = elements[2];
       CompType todel = getCompTypeFromNameOrID(so, name);
       if (!todel.isUpdatable())
        throw new ApiException("Permission Denied");
       so.RemoveObject("servercomptype", todel.getId(), out, true);
       delop = true;
      }
      else
       throw new ApiException("Invalid object \"" + elements[1] + "\" for \"del\" operation");
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }
    }
    else if (elements[0].equals("mod"))
    {
     // Modifying an existing object
     if (elements.length == 3)
     {
      if (elements[1].equals("user"))
       ModUser(so, elements[2], request);
      else if (elements[1].equals("server"))
       ModServer(so, elements[2], request);
      else if (elements[1].equals("environment"))
       ModEnvironment(so, elements[2], request);
      else if (elements[1].equals("credential"))
       ModCredential(so, elements[2], request);
      else
       throw new ApiException("Invalid object \"" + elements[1] + "\" for \"mod\" operation");
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }
    }
    else if (elements[0].equals("clone"))
    {
     // API/clone/server/<servername>/<newname>
     // API/clone/environment/<envname>/<newname>
     // API/clone/user/<username>/<newname>
     if (elements.length == 4)
     {
      if (elements[1].equals("server"))
       cloneServer(so, elements[2], elements[3], request);
      else if (elements[1].equals("environment"))
       cloneEnvironment(so, elements[2], elements[3], request);
      else
       throw new ApiException("Invalid object \"" + elements[1] + "\" for \"clone\" operation");
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }
    }
    else if (elements[0].equals("assign"))
    {

     // API/assign/server/<servername>/<environmentname>
     // API/assign/application/<appname>/<environmentname>
     if (elements.length == 4)
     {
      if (elements[1].equals("server"))
       assignServer(so, elements[2], elements[3]);
      else if (elements[1].equals("application"))
       assignApplication(so, elements[2], elements[3], request);
      else if (elements[1].equals("user"))
       assignUser(so, elements[2], elements[3]);
      else
       throw new ApiException("Invalid object \"" + elements[1] + "\" for \"assign\" operation");
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }
    }
    else if (elements[0].equals("unassign"))
    {
     // API/unassign/server/<servername>/<environmentname>
     // API/unassign/application/<appname>/<environmentname>
     if (elements.length == 4)
     {
      if (elements[1].equals("server"))
       deAssignServer(so, elements[2], elements[3]);
      else if (elements[1].equals("application"))
       deAssignApplication(so, elements[2], elements[3]);
      else if (elements[1].equals("user"))
       deAssignUser(so, elements[2], elements[3]);
      else
       throw new ApiException("Invalid object \"" + elements[1] + "\" for \"assign\" operation");
     }
     else
     {
      throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
     }
    }
    else if (!elements[0].equals("sql"))
    {
     throw new ApiException("Unrecognised request path: " + path);
    }

    obj.add("success", true);

    if (deploymentid != 0)
    {
     obj.add("deploymentid", deploymentid);
    }
   }
   catch (ApiException e)
   {
    System.out.println("ApiException caught e=" + e.getMessage());
    if (so != null)
    {
     try
     {
      so.GetConnection().rollback();
     }
     catch (SQLException e1)
     {
      // TODO Auto-generated catch block
      e1.printStackTrace();
     }
    }
    obj.add("success", false);
    obj.add("error", e.getMessage());
   }
   catch (Exception e)
   {
    System.out.println("Exception caught, e=" + e.getMessage());
    e.printStackTrace();
    if (so != null)
    {
     try
     {
      so.GetConnection().rollback();
     }
     catch (SQLException e1)
     {
      // TODO Auto-generated catch block
      e1.printStackTrace();
     }
    }
    e.printStackTrace();
    obj.add("success", false);
    obj.add("error", e.toString());
   }
   finally
   {
    if (!delop)
    {
     // Delete operations call RemoveObject which writes success or failure JSON to the PrintWriter...
     out = response.getWriter();
     String ret = obj.getJSON();
     System.out.println(ret);
     out.println(ret);
    }
   }
  }
 }

 private JSONObject exportJSONForRepository(DMSession so, Repository r)
 {
  JSONObject je = new JSONObject();

  ProviderObject x = (ProviderObject)so.getDetailedObject(ObjectType.REPOSITORY, r.getId());

  je.add("name", x.getFullName());
  if (x.getDomain() != null)
   je.add("domain", x.getDomain().getFullDomain());
  else
   je.add("domain", "");
  je.add("summary", x.getSummary());

  DMObject owner = x.getOwner();
  if (owner != null)
  {
   if (owner.getObjectType() == ObjectType.USER)
   {
    User user = so.getUser(owner.getId());
    je.add("owneruser",user.getDomain().getFullDomain() + "." + user.getName());
   }
   if (owner.getObjectType() == ObjectType.USERGROUP)
   {
    UserGroup grp = so.getGroup(owner.getId());
    je.add("ownergroup",grp.getDomain().getFullDomain() + "." + grp.getName());
   }
  }
  Credential cred = x.getCredential();
  String credname = "";

  if (cred != null)
   credname = cred.getFullName();
  je.add("credential", credname);

  ProviderDefinition t = x.getDef();

  je.add("kind", (t.getKind() != null) ? t.getKind().getTypeString() : "");


  List<DMAttribute> attrs = x.getAttributes();
  JSONArray arr = new JSONArray();

  if (attrs != null)
  {
   for (int i=0;i<attrs.size();i++)
    arr.add(attrs.get(i).toJSON());
  }
  je.add("attrs", arr);

  arr = new JSONArray();
  List<DMProperty> props = x.getProperties();

  if (props != null)
  {
   for (int i=0;i<props.size();i++)
   {
    DMProperty prop = props.get(i);
    arr.add(prop.toJSON());
   }
  }
  je.add("props", arr);

  return je;
 }

 private JSONObject exportJSONForEnvironment(DMSession so, Environment x)
 {
  JSONObject je = new JSONObject();
  je.add("name", x.getFullName());
  if (x.getDomain() != null)
   je.add("domain", x.getDomain().getFullDomain());
  else
   je.add("domain", "");
  je.add("summary", x.getSummary());
  je.add("availability",x.getCalUsage());

  DMObject owner = x.getOwner();
  if (owner != null)
  {
   if (owner.getObjectType() == ObjectType.USER)
   {
    User user = so.getUser(owner.getId());
    je.add("owneruser",user.getDomain().getFullDomain() + "." + user.getName());
   }
   if (owner.getObjectType() == ObjectType.USERGROUP)
   {
    UserGroup grp = so.getGroup(owner.getId());
    je.add("ownergroup",grp.getDomain().getFullDomain() + "." + grp.getName());
   }
  }

  List<DMAttribute> attrs = x.getAttributes();
  JSONArray arr = new JSONArray();

  if (attrs != null)
  {
   for (int i=0;i<attrs.size();i++)
    arr.add(attrs.get(i).toJSON());
  }
  je.add("attrs", arr);

  arr = new JSONArray();
  List<Server> srvs = so.getServersInEnvironment(x.getId());

  if (srvs != null)
  {
   for (int i=0;i<srvs.size();i++)
   {
    JSONObject obj = new JSONObject();
    Server s = so.getServer(srvs.get(i).getId(), true);
    obj.add("name", s.getFullName());
    arr.add(obj);
   }
  }
  je.add("endpoints", arr);

  return je;
 }

 private JSONObject exportJSONForServer(DMSession so, Server x)
 {
  x = so.getServer(x.getId(), true);
  JSONObject je = new JSONObject();
  je.add("name", x.getFullName());
  if (x.getDomain() != null)
   je.add("domain", x.getDomain().getFullDomain());
  else
   je.add("domain", "");
  je.add("summary", x.getSummary());

  ServerType t = x.getServerType();
  je.add("type", t.getName());
  je.add("hostname", x.getHostName());
  je.add("protocol", x.getProtocol());
  je.add("sshport", x.getSSHPort());
  je.add("basedir", x.getBaseDir());
  je.add("autoping", x.getAutoPing());
  je.add("automd5", x.getAutoMD5());

  List<DMAttribute> attrs = x.getAttributes();
  JSONArray arr = new JSONArray();

  if (attrs != null)
  {
   for (int i=0;i<attrs.size();i++)
    arr.add(attrs.get(i).toJSON());
  }
  je.add("attrs", arr);

  Credential cred = x.getCredential();
  String credname = "";

  if (cred != null)
   credname = cred.getFullName();
  je.add("credential", credname);

  DMObject owner = x.getOwner();
  if (owner != null)
  {
   if (owner.getObjectType() == ObjectType.USER)
   {
    User user = so.getUser(owner.getId());
    je.add("owneruser",user.getFullName());
   }
   if (owner.getObjectType() == ObjectType.USERGROUP)
   {
    UserGroup grp = so.getGroup(owner.getId());
    je.add("ownergroup",grp.getFullName());
   }
  }

  arr = new JSONArray();
  ArrayList<CompType> comptypes = x.getServerCompType();

  if (comptypes != null)
  {
   for (int i=0;i<comptypes.size();i++)
   {
    JSONObject obj = new JSONObject();
    obj.add("name", comptypes.get(i).getName());
    arr.add(obj);
   }
  }
  je.add("comptypes", arr);
  return je;
 }

 private JSONObject exportJSONForCredential(DMSession so, Credential x)
 {
  x = so.getCredential(x.getId(), true);
  JSONObject je = new JSONObject();
  je.add("name", x.getFullName());
  if (x.getDomain() != null)
   je.add("domain", x.getDomain().getFullDomain());
  else
   je.add("domain", "");
  je.add("summary", x.getSummary());
  je.add("kind", x.getKindAsString());
  DMObject owner = x.getOwner();
  if (owner != null)
  {
   if (owner.getObjectType() == ObjectType.USER)
   {
    User user = so.getUser(owner.getId());
    je.add("owneruser",user.getDomain().getFullDomain() + "." + user.getName());
   }
   if (owner.getObjectType() == ObjectType.USERGROUP)
   {
    UserGroup grp = so.getGroup(owner.getId());
    je.add("ownergroup",grp.getDomain().getFullDomain() + "." + grp.getName());
   }
  }
  return je;
 }


 private JSONObject exportJSONForCompType(DMSession so, CompType x)
 {
  JSONObject je = new JSONObject();
  je.add("name", x.getFullName());
  je.add("summary", x.getSummary());
  je.add("database", x.getDatabase());
  je.add("email", x.getDeletedir());
  return je;
 }

 private JSONObject exportJSONForCategory(DMSession so, Category x)
 {
  JSONObject je = new JSONObject();
  je.add("name", x.getName());
  return je;
 }

 private JSONObject exportJSONForUser(DMSession so, User x)
 {
  JSONObject je = new JSONObject();
  je.add("name", x.getFullName());
  if (x.getDomain() != null)
   je.add("domain", x.getDomain().getFullDomain());
  else
   je.add("domain", "");
  je.add("summary", x.getSummary());
  je.add("realname", x.getRealName());
  je.add("email", x.getEmail());
  je.add("phone", x.getPhone());
  je.add("dateformat", x.getDateFmt());
  je.add("timeformat", x.getTimeFmt());
  UserGroupList ug = so.getGroupsForUser(x.getId());
  JSONArray arr = new JSONArray();
  if (ug != null)
  {
   for (int i=0;i<ug.size();i++)
   {
    UserGroup grp = so.getGroup(ug.get(i).getId());
    arr.add(grp.getFullName());
   }
  }
  je.add("groups", arr);

  return je;
 }

 private JSONObject exportJSONForGroup(DMSession so, UserGroup x)
 {
   JSONObject je = new JSONObject();
   je.add("name", x.getFullName());
   if (x.getDomain() != null)
    je.add("domain", x.getDomain().getFullDomain());
   else
    je.add("domain", "");
   je.add("summary", x.getSummary());
   je.add("email", x.getEmail());
   return je;
 }

 private JSONObject exportJSONForDomain(DMSession so, Domain x)
 {
   JSONObject je = new JSONObject();
   je.add("name", x.getParentDomain() + "." + x.getName());
   if (x.getDomain() != null)
    je.add("domain", x.getDomain().getFullDomain());
   else
    je.add("domain", "");
   return je;
 }
 /*
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  String path = request.getPathInfo();

  if (path.contains("/provides"))
  {
   try (DMSession so = new DMSession(request.getServletContext()))
   {
    String[] parts = path.split("/");
    String compstr = parts[parts.length-1];
    int compid = new Integer(compstr).intValue();

    so.setCompProvides(compid, request, response);
    PrintWriter out = response.getWriter();
    out.println("{}");
   }
   catch (Exception e)
   {
    System.out.println("Exception caught, e=" + e.getMessage());
    e.printStackTrace();
   }
  }
  else if (path.contains("/consumes"))
  {
   try (DMSession so = new DMSession(request.getServletContext()))
   {
    String[] parts = path.split("/");
    String compstr = parts[parts.length-1];
    int compid = new Integer(compstr).intValue();

    so.setCompConsumes(compid, request, response);
    PrintWriter out = response.getWriter();
    out.println("{}");
   }
   catch (Exception e)
   {
    System.out.println("Exception caught, e=" + e.getMessage());
    e.printStackTrace();
   }
  }
  else if (path.contains("/signup"))
  {
   Signup(request, response);
   return;
  }
  else if (path.contains("/adddemo"))
  {
   try (DMSession so = DMSession.getInstance(request))
   {
    so.internalLogin(request.getServletContext());

    String userid = request.getParameter("userid");
    String domname = request.getParameter("domname");
    String clientid = UUID.randomUUID().toString();
    String provider = "DeployHub";
    String providerid = "";
    String lictype = "OSS";
    int liccnt = 9999;

    JSONObject obj = new JSONObject();
    Domain engineDomain = so.getDomainByName(domname);

    String jsonpath="/WEB-INF/schema";
    System.out.println("Taking json scripts from "+jsonpath);
    String installpath = request.getServletContext().getRealPath(jsonpath);

    so.createSaaSClient(clientid, engineDomain, lictype, liccnt, provider, providerid, installpath);
    obj.add("err", "");
    PrintWriter out = response.getWriter();
    out.write(obj.getJSON());
    return;
   }
  }
  else if (path.contains("/getscore"))
  {
   try (DMSession so = new DMSession(request.getServletContext()))
   {
    JSONObject obj = so.getScore4Purl(request, response);
    PrintWriter out = response.getWriter();
    out.write(obj.getJSON());
    return;
   }
  }
  else if (path.contains("/setvar"))
  {
   try (DMSession so = DMSession.getInstance(request))
   {
    if (path.length() > 1 && (path.charAt(0) == '/'))
     path = path.substring(1);

    String[] elements = path.split("/");

    for (int i = 1; i < elements.length; i++)
     elements[i] = java.net.URLDecoder.decode(elements[i], "UTF-8");

    System.out.println("DOING setvar");
    if (elements.length == 3)
    {
     String objtype = elements[1];
     String objname = elements[2];

     String requestData = request.getReader().lines().collect(Collectors.joining());
     JsonObject obj = new JsonParser().parse(requestData).getAsJsonObject();

     AttributeChangeSet acs = new AttributeChangeSet();

     for(Map.Entry<String, JsonElement> entry : obj.entrySet())
     {
      DMAttribute attr = new DMAttribute();
      attr.setName(entry.getKey());
      attr.setValue(entry.getValue() != null ? entry.getValue().getAsString() : "");
      acs.addChanged(attr);
     }

     try
     {
      if (objtype.equalsIgnoreCase("environment"))
      {
       Environment env = getEnvironmentFromNameOrID(so, objname);
       so.updateAttributesForObject(env, acs);
      }
      else if (objtype.equalsIgnoreCase("application"))
      {
       Application app = getApplicationFromNameOrID(so, objname);
       so.updateAttributesForObject(app, acs);
      }
      else if (objtype.equalsIgnoreCase("server"))
      {
       Server serv = getServerFromNameOrID(so, objname);
       ;
       so.updateAttributesForObject(serv, acs);
      }
      else if (objtype.equalsIgnoreCase("component"))
      {
       Component comp = getComponentFromNameOrID(so, objname);
       String delattrs = request.getParameter("delattrs");

       if (delattrs != null && delattrs.equalsIgnoreCase("y"))
          so.DeleteComponentAttribs(comp.getId());

       so.updateAttributesForObject(comp, acs);
      }
      else
      {
       throw new ApiException("Invalid object type " + objtype);
      }
     }
     catch (RuntimeException ex)
     {
      throw new ApiException(ex.getMessage());
     }
    }
    else
    {
     throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
    }
   }
   catch (Exception e)
   {
    e.printStackTrace();
   }

   PrintWriter out = response.getWriter();
   JSONObject obj = new JSONObject();
   out.write(obj.getJSON());

   return;
  }

  if (path.contains("/uploadhelm"))
  {
   try (DMSession so = DMSession.getInstance(request))
   {
    PrintWriter out = response.getWriter();
    JSONObject obj = uploadHelm(so, request, response);
    out.print(obj.getJSON());
   }
   catch (Exception e)
   {
    System.out.println("Exception caught, e=" + e.getMessage());
    e.printStackTrace();

   }
   return;
  }

  if (path.contains("/deploy"))
  {
   try (DMSession so = DMSession.getInstance(request))
   {
    logDeployment(so, request, response);
   }
   catch (Exception e)
   {
    System.out.println("Exception caught, e=" + e.getMessage());
    e.printStackTrace();

   }
   return;
  }

  if (!path.contains("/import"))
  {
   doGet(request, response);
   return;
  }

  try (DMSession so = DMSession.getInstance(request))
  {
   PrintWriter out = response.getWriter();
   importObject(so, request, response);
  }
  catch (Exception e)
  {
   System.out.println("Exception caught, e=" + e.getMessage());
   e.printStackTrace();

  }
 }

 // SQL interface

 private int handleSQLClient(DMSession so, HttpServletRequest request, PrintWriter out) throws ApiException
 {
  String clientid = request.getParameter("clientid");
  System.out.println("Confirming client ID " + clientid);
  if (so.confirmClientID(clientid))
  {
   // Success
   out.println("<result success=\"Y\" />");
  }
  else
  {
   // Failed
   out.println("<result success=\"N\"><error>Invalid Client ID</error></result>");
  }
  return 0;
 }

 private int handleSQLClose(DMSession so, HttpServletRequest request, PrintWriter out) throws ApiException
 {
  String stmtid = request.getParameter("stmtid");
  String xmlres = so.closeSQL(stmtid);
  out.println(xmlres);
  System.out.println(xmlres);
  return 0;
 }

 private int handleSQLQuery(DMSession so, HttpServletRequest request, PrintWriter out) throws ApiException
 {
  // We process the SQL and convert the result set into XML
  String query = request.getParameter("query");
  String prep = request.getParameter("prepare");
  boolean prepare = (prep != null && prep.equalsIgnoreCase("y"));
  System.out.println("sql=" + query);
  System.out.println("prep=" + prep);
  String xmlres = "";
  if (prepare)
  {
   String stmtid = request.getParameter("stmtid");
   System.out.println("stmtid=" + stmtid);
   xmlres = so.prepareSQL(stmtid, query);
  }
  else
  {
   xmlres = so.processSQL(query);
  }
  out.println(xmlres);
  System.out.println(xmlres);
  return 0;
 }

 private int handleSQLBinding(DMSession so, HttpServletRequest request, PrintWriter out) throws ApiException
 {
  String xmlres = "";
  String stmtid = request.getParameter("stmtid");
  String colstring = request.getParameter("colno");
  String coltypestring = request.getParameter("coltype");
  int colno = (colstring != null) ? Integer.parseInt(colstring) : 0;
  int coltype = (coltypestring != null) ? Integer.parseInt(coltypestring) : 0;
  String val = request.getParameter("val");
  switch (coltype)
  {
   case 1:
    // 1 = String
    xmlres = so.BindParameter(stmtid, colno, val);
    break;
   case 2:
    // 2 = Integer
    xmlres = so.BindParameter(stmtid, colno, Integer.parseInt(val));
    break;
  }

  out.println(xmlres);
  System.out.println(xmlres);
  return 0;
 }

 private int handleSQLExecute(DMSession so, HttpServletRequest request, PrintWriter out) throws ApiException
 {
  String stmtid = request.getParameter("stmtid");
  String xmlres = so.ExecuteSQL(stmtid);
  out.println(xmlres);
  System.out.println(xmlres);
  return 0;
 }

 private int handleSQLEndTransaction(DMSession so, HttpServletRequest request, PrintWriter out) throws ApiException
 {
  String stmtid = request.getParameter("stmtid");
  String commit = request.getParameter("end");
  String xmlres = so.EndTransaction(stmtid, commit.equalsIgnoreCase("y"));
  out.println(xmlres);
  System.out.println(xmlres);
  return 0;
 }

 private int handleSQLAutoCommit(DMSession so, HttpServletRequest request, PrintWriter out) throws ApiException
 {
  String autoCommit = request.getParameter("start");
  String xmlres = so.SetAutoCommit(autoCommit.equalsIgnoreCase("y"));
  out.println(xmlres);
  System.out.println(xmlres);
  return 0;
 }
 // END TEMP

 private int doDeployment(DMSession so, String[] elements, HttpServletRequest request) throws ApiException
 {
  String envName = null;
  String appName = null;
  int deploymentid = 0;

  System.out.println("doDeployment");
  if (elements.length < 2)
   appName = request.getParameter("app");
  else
   appName = elements[1];

  System.out.println("APPNAME=" + appName);
  if (appName == null)
   throw new ApiException("application not specified");

  Application app = getApplicationFromNameOrID(so, appName);

  System.out.println("Application found is " + app.getId());
  envName = request.getParameter("env");
  if (envName == null)
  {
   // env not specified as parameter - get from elements array
   if (elements.length < 3)
   {
    throw new ApiException("environment not specified");
   }
   envName = elements[2];
  }

  String waitstr = request.getParameter("wait");
  boolean wait = true;
  if (waitstr != null)
  {
   if (waitstr.charAt(0) == 'n' || waitstr.charAt(0) == 'N')
    wait = false;
  }
  Environment env = getEnvironmentFromNameOrID(so, envName);

  // Hashtable of additional parameters - each extra parameter on the URL
  // is set as a variable on the command-line input
  // For example when invoked from Perforce we can be passed:
  // change, status, review, project, branch, succ and fail
  Hashtable<String, String> cmdline_params = new Hashtable<String, String>();

  @SuppressWarnings("rawtypes")
  Enumeration params = request.getParameterNames();
  while (params.hasMoreElements())
  {
   String param = (String) params.nextElement();
   // Skip parameters that are DM-specific
   if (!param.equals("env") && !param.equals("wait") && !param.equals("task") && !param.equals("taskname"))
   {
    String value = request.getParameter(param);
    if (value != null)
    {
     cmdline_params.put("url_" + param, value);
    }
   }
  }
  TaskDeploy task = null;
  String taskname = request.getParameter("taskname");
  if (taskname == null)
   taskname = request.getParameter("task");
  if (taskname == null)
  {
   // No taskname specified, find a deployment task to use
   Task t = so.getTaskByType(app.getDomain(), TaskType.DEPLOY);
   if (t == null)
    throw new ApiException("Could not find appropriate deploy task");
   task = so.getTaskDeploy(t.getId());
  }
  else
  {
   // Task specified
   try
   {
    Task t = so.getTaskByName(taskname); // Throws RuntimeException is taskname is not unique or does not exist
    if (t.getTaskType() != TaskType.DEPLOY)
     throw new ApiException("Specified Task is not a Deploy task");
    task = so.getTaskDeploy(t.getId());
    if (task == null)
     throw new ApiException("specifed task not found or no access");
   }
   catch (Exception ex)
   {
    throw new ApiException(ex.getMessage());
   }
  }
  System.out.println("using task " + task.getName());
  task.setApplication(app);
  task.setEnvironment(env);
  task.setWaitFor(wait);
  task.setAdditionalParameters(cmdline_params);
  task.setDeploymentSessionId(so.GetSessionId() + "_" + System.currentTimeMillis());

  if(task.run()) {
   Deployment dep = so.getDeploymentBySessionId(task, 30);
   if(dep != null && dep.getId() > 0) {
    deploymentid = dep.getId();
   } else {
    String msg = task.getLastOutputLine();
    String error = "Timed out waiting for deployment to start" + ((msg != null) ? ("\n" + msg) : "");
    throw new ApiException(error);
   }
  } else {
   String msg = task.getLastOutputLine();
   String error = "Failed to start deployment" + ((msg != null) ? ("\n" + msg) : "");
   throw new ApiException(error);
  }
  return deploymentid;
 }

 JSONObject uploadHelm(DMSession so, HttpServletRequest request, HttpServletResponse response)
 {
  JSONObject ret = new JSONObject();

  try
  {
   response.setContentType("application/json");
   PrintWriter out = response.getWriter();

   String requestData = request.getReader().lines().collect(Collectors.joining());
   JsonObject obj = new JsonParser().parse(requestData).getAsJsonObject();

   String deployment = (obj.get("deployid") != null) ? obj.get("deployid").getAsString() : "";
   String compname = (!(obj.get("component") instanceof JsonNull)) ? obj.get("component").getAsString() : "";
   String chartdigest = (obj.get("chartdigest") != null) ? obj.get("chartdigest").getAsString() : "";
   String chartname = (obj.get("chartname") != null) ? obj.get("chartname").getAsString() : "";
   String chartversion = (obj.get("chartversion") != null) ? obj.get("chartversion").getAsString() : "";
   String helmrepo = (obj.get("helmrepo") != null) ? obj.get("helmrepo").getAsString() : "";
   String helmrepourl = (obj.get("helmrepourl") != null) ? obj.get("helmrepourl").getAsString() : "";
   JsonArray files = (obj.get("files") != null) ? (JsonArray) obj.get("files") : null;
   JsonArray images = (obj.get("images") != null) ? (JsonArray) obj.get("images") : null;

   if (deployment == null)
    throw new RuntimeException("Deployment number is required");

   if (files == null)
    throw new RuntimeException("List of files and contents are required");

   ret.add("deployid", deployment);
   int deployid = new Integer(deployment).intValue();
   int envid = so.getEnvironmentForDeployment(deployid);
   int appid = so.getApplicationForDeployment(deployid);
   int compid = -1;

   Application app = null;
   Component comp = null;

   try
   {
    app = this.getApplicationFromNameOrID(so, new Integer(appid).toString());
    boolean isRelease = false;

    if (app.getIsRelease().equalsIgnoreCase("y"))
     isRelease = true;

    List<Component> comps = so.getComponents(ObjectType.APPLICATION, app.getId(), isRelease);

    for (int k=0;k<comps.size();k++)
    {
     if (comps.get(k).getFullName().equals(compname))
     {
      comp = comps.get(k);
      break;
     }
    }
    if (comp != null)
     compid = comp.getId();
   }
   catch (ApiException e)
   {
   }

   if (app == null)
    throw new RuntimeException("Deployment not found");

 //  so.HelmUploadClean(app.getId(), deployid);

   for (int i=0;i<files.size();i++)
   {
    JsonObject fileobj = files.get(i).getAsJsonObject();
    String filename = (fileobj.get("filename") != null) ? fileobj.get("filename").getAsString() : "";
    String data = (fileobj.get("data") != null) ? fileobj.get("data").getAsString() : "";
    ret.add("filename" + i, filename);

    so.HelmUpload(app.getId(), app.getParentId(), compid, envid, deployid, chartname, chartversion, helmrepo, helmrepourl, chartdigest, filename, data);
   }

   if (images != null)
   {
    for (int i=0;i<images.size();i++)
    {
     JsonObject imgobj = images.get(i).getAsJsonObject();
     String registry = (imgobj.get("registry") != null) ? imgobj.get("registry").getAsString() : "";
     String organization = (imgobj.get("organization") != null) ? imgobj.get("organization").getAsString() : "";
     String imagename = (imgobj.get("imagename") != null) ? imgobj.get("imagename").getAsString() : "";
     String imagetag = (imgobj.get("imagetag") != null) ? imgobj.get("imagetag").getAsString() : "";
     String imagedigest = (imgobj.get("imagedigest") != null) ? imgobj.get("imagedigest").getAsString() : "";

     so.HelmImage(app.getId(), compid, envid, deployid, chartdigest, registry, organization, imagename, imagetag, imagedigest);
    }
   }
  }
  catch (IOException e)
  {
   e.printStackTrace();
  }
  return ret;
}

 /**
  * Compress a directory to ZIP file including subdirectories
  * @param directoryToCompress directory to zip
  * @param outputDirectory     where to place the compress file
  */
 public byte[] zipDirectory(File directoryToCompress, File outputDirectory){
     try {
         ByteArrayOutputStream dest = new ByteArrayOutputStream();
         ZipOutputStream zipOutputStream = new ZipOutputStream(dest);

         zipDirectoryHelper(directoryToCompress, directoryToCompress, zipOutputStream);
         zipOutputStream.close();
         return dest.toByteArray();
     } catch (Exception e) {
         e.printStackTrace();
     }
     return null;
 }

 private void zipDirectoryHelper(File rootDirectory, File currentDirectory, ZipOutputStream out) throws Exception {
     byte[] data = new byte[2048];

     File[] files = currentDirectory.listFiles();
     if (files == null) {
       // no files were found or this is not a directory

     } else {
         for (File file : files) {
             if (file.isDirectory()) {
                 zipDirectoryHelper(rootDirectory, file, out);
             } else {
                 FileInputStream fi = new FileInputStream(file);
                 // creating structure and avoiding duplicate file names
                 String name = file.getAbsolutePath().replace(rootDirectory.getAbsolutePath(), "");
                 name = name.replaceAll("^[/\\.]+", "");
                 ZipEntry entry = new ZipEntry(name);
                 out.putNextEntry(entry);
                 int count;
                 BufferedInputStream origin = new BufferedInputStream(fi,2048);
                 while ((count = origin.read(data, 0 , 2048)) != -1){
                     out.write(data, 0, count);
                 }
                 origin.close();
             }
         }
     }

 }

 void logDeployment(DMSession so, HttpServletRequest request, HttpServletResponse response)
 {
  JSONObject ret = new JSONObject();

  try
  {
   response.setContentType("application/json");
   PrintWriter out = response.getWriter();

   String requestData = request.getReader().lines().collect(Collectors.joining());
   JsonObject obj  = new JsonParser().parse(requestData).getAsJsonObject();
   String appname  = (obj.get("application") != null) ? obj.get("application").getAsString() : "";
   JsonArray imagetags  = (obj.get("imagetags") != null) ? obj.get("imagetags").getAsJsonArray() : new JsonArray();
   JsonArray compnames = (obj.get("compversion") != null) ? obj.get("compversion").getAsJsonArray() : new JsonArray();
   String rc = (obj.get("rc") != null) ? obj.get("rc").getAsString() : "-1";
   String log = (obj.get("log") != null) ? obj.get("log").getAsString() : "";
   String skipdeploy = (!(obj.get("skipdeploy") instanceof JsonNull))  ? obj.get("skipdeploy").getAsString() : "N";

   ArrayList<String> endpoints = new ArrayList<String>();

   JsonElement environmentElement = obj.get("environment");

   String envname = "";
   if (environmentElement.isJsonPrimitive())
   {
    envname  = (obj.get("environment") != null) ? obj.get("environment").getAsString() : "";
   }
   else if (environmentElement.isJsonObject())
   {
    // environment is a dictionary
    JsonObject environmentObject = environmentElement.getAsJsonObject();
    Map.Entry<String, JsonElement> firstEntry = environmentObject.entrySet().iterator().next();
    envname = firstEntry.getKey();

    for (JsonElement element : environmentObject.getAsJsonArray(envname))
    {
     endpoints.add(element.getAsString());
    }
   }

   if (endpoints.size() == 0)
    endpoints.add(envname);

   int exitcode = 0;

   if (rc != null)
    exitcode = new Integer(rc).intValue();

   if (envname.length() == 0)
   {
    ret.add("errormsg", "Environment Name required");
    out.println(ret.getJSON());
    return;
   }

   if (appname.length() == 0 && compnames.size() == 0  && imagetags.size() == 0)
   {
    ret.add("errormsg", "Application or Component Names required");
    out.println(ret.getJSON());
    return;
   }

   String shortname = "";
   ArrayList<String> parts = new ArrayList<String>(Arrays.asList(envname.split("\\.")));
   if (parts != null && !parts.isEmpty())
   {
    shortname = parts.get(parts.size()-1);
    parts.remove(parts.size()-1);
   }

   Domain tgtdomain;
   try
   {
    tgtdomain = getDomainFromNameOrID(so, parts.get(0));
   }
   catch (ApiException e1)
   {
    tgtdomain = null;
   }

   String domStr = parts.get(0);
   boolean newDom = false;

   for (int i = 1; i < parts.size(); i++)
   {
    String domain = parts.get(i);
    newDom = false;

    domStr += "." + domain;
    try
    {
     tgtdomain = getDomainFromNameOrID(so, domStr);
    }
    catch (Exception e)
    {
     newDom = true;
    }

    if (newDom)
    {
     int newid = so.getID("domain");
     so.CreateNewObject("domain", domain, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", true);

     tgtdomain = so.getDomain(newid);

     if (tgtdomain == null)
     {
      ret.add("saved", false);
      ret.add("error", "Failed to create domain" + domain);
      String result = ret.getJSON();
      System.out.println(result);
      out.println(result);
      return;
     }
    }
   }

   Environment env = null;
   try
   {
    env = this.getEnvironmentFromNameOrID(so, envname);
   }
   catch (ApiException e)
   {
    int newid = so.getID("environment");
    so.CreateNewObject("environment", shortname, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "environments", true);
    try
    {
     env = this.getEnvironmentFromNameOrID(so, "" + newid);
    }
    catch (ApiException e1)
    {
    }
   }

   if (env == null)
   {
    ret.add("errormsg", "Environment not found");
    out.println(ret.getJSON());
    return;
   }

   for (int j=0;j < endpoints.size();j++)
   {
    String endpoint = endpoints.get(j);

    shortname = "";
    parts = new ArrayList<String>(Arrays.asList(endpoint.split("\\.")));
    if (parts != null && !parts.isEmpty())
    {
     shortname = parts.get(parts.size()-1);
     parts.remove(parts.size()-1);
    }

    try
    {
     tgtdomain = getDomainFromNameOrID(so, parts.get(0));
    }
    catch (ApiException e1)
    {
     tgtdomain = null;
    }

    domStr = parts.get(0);
    newDom = false;

    for (int i = 1; i < parts.size(); i++)
    {
     String domain = parts.get(i);
     newDom = false;

     domStr += "." + domain;
     try
     {
      tgtdomain = getDomainFromNameOrID(so, domStr);
     }
     catch (Exception e)
     {
      newDom = true;
     }

     if (newDom)
     {
      int newid = so.getID("domain");
      so.CreateNewObject("domain", domain, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", true);

      tgtdomain = so.getDomain(newid);

      if (tgtdomain == null)
      {
       ret.add("saved", false);
       ret.add("error", "Failed to create domain" + domain);
       String result = ret.getJSON();
       System.out.println(result);
       out.println(result);
       return;
      }
     }
    }

    Server srv = null;

    try
    {
     srv = this.getServerFromNameOrID(so, endpoint);
    }
    catch (ApiException e)
    {
     int newid = so.getID("server");
     so.CreateNewObject("server", shortname, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "servers", true);
     try
     {
      srv = this.getServerFromNameOrID(so, "" + newid);
     }
     catch (ApiException e1)
     {
     }
    }

    if (srv != null)
     so.MoveServer(env.getId(), srv.getId(), 0, 0);
   }

   // find component image tag or digest

   if (compnames.size() == 0)
   {
    int compid = -1;
    if (imagetags != null)
    {
     for (int k=0; k < imagetags.size(); k++)
     {
      String tag = imagetags.get(k).getAsString();
      compid = so.getComp4Digest(so, tag);

      if (compid < 0)
       compid = so.getComp4Tag(so, tag);

      if (compid > 0)
      {
       Component comp = so.getComponent(compid, true);
       compnames.add(new JsonPrimitive(comp.getFullName()));
      }
     }
    }
   }

   // find app names base on component
   if (appname.length() == 0 && compnames.size() > 0)
   {
    String compname = compnames.get(0).getAsString();
    Component comp = so.getComponentByName(compname);
    List<Application> applist = so.getAppsForComponent(comp);

    for (int i=0;i<applist.size();i++)
    {
     Application app = applist.get(i);

     // get current list of appvers in env

     // see if latest appver in env has component
     //  if so then log deployment
     //  else create new appver base on latest in env with component and log deployment
    }
   }

   // add app domains
   shortname = "";
   parts = new ArrayList<String>(Arrays.asList(appname.split("\\.")));
   if (parts != null && !parts.isEmpty())
   {
    shortname = parts.get(parts.size()-1);
    parts.remove(parts.size()-1);
   }

   try
   {
    tgtdomain = getDomainFromNameOrID(so, parts.get(0));
   }
   catch (ApiException e1)
   {
    tgtdomain = null;
   }

   domStr = parts.get(0);
   newDom = false;

   for (int i = 1; i < parts.size(); i++)
   {
    String domain = parts.get(i);
    newDom = false;

    domStr += "." + domain;
    try
    {
     tgtdomain = getDomainFromNameOrID(so, domStr);
    }
    catch (Exception e)
    {
     newDom = true;
    }

    if (newDom)
    {
     int newid = so.getID("domain");
     so.CreateNewObject("domain", domain, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", true);

     tgtdomain = so.getDomain(newid);

     if (tgtdomain == null)
     {
      ret.add("saved", false);
      ret.add("error", "Failed to create domain" + domain);
      String result = ret.getJSON();
      System.out.println(result);
      out.println(result);
      return;
     }
    }
   }

   Application app = null;

   if (appname != null  && !appname.isEmpty())
   {
    try
    {
     app = this.getApplicationFromNameOrID(so, appname);
     app = so.getLatestVersion(app, null);
    }
    catch (ApiException e)
    {
     String[] p2 = appname.split(";");
     if (p2 != null && p2.length >= 1)
     {
      try
      {
       app = this.getApplicationFromNameOrID(so, p2[0]);
       app = so.getLatestVersion(app, null);
      }
      catch (ApiException e1)
      {
      }
     }
    }
   }

   Component comp = null;
   ArrayList<Component> comps = new ArrayList<Component>();

   if (compnames != null)
   {
    for (int k=0; k < compnames.size(); k++)
    {
     String compname = compnames.get(k).getAsString();
     try
     {
      comp = this.getComponentFromNameOrID(so, compname);
      comps.add(comp);
     }
     catch (ApiException e)
     {
     }

     if (comp == null)
     {
      ret.add("errormsg", "Component not found");
      out.println(ret.getJSON());
      return;
     }
    }
   }
   ret = so.logDeployment(appname, app, comps, env, exitcode, log, skipdeploy);
   String result = ret.getJSON();
   System.out.println(result);
   out.println(result);
  }
  catch (IOException e)
  {
   e.printStackTrace();
  }

}


 void importObject(DMSession so, HttpServletRequest request, HttpServletResponse response)
 {
  String errtext = "";
  int oid = 0;

  try
  {
   response.setContentType("application/json");
   PrintWriter out = response.getWriter();
   JSONObject ret = new JSONObject();

   String requestData = request.getReader().lines().collect(Collectors.joining());
   JsonObject obj = new JsonParser().parse(requestData).getAsJsonObject();
   String ot = obj.get("objtype").getAsString();
   String objname = obj.get("objname").getAsString();

   ObjectType objtype = ObjectType.fromTypeString(ot);
   if (objtype == null)
   {
    throw new RuntimeException("Invalid object type " + ot);
   }

   if (objtype == ObjectType.CATEGORY)
   {
    so.addCategory(objname);
    ret.add("saved", true);
    ret.add("error", "");
    String result = ret.getJSON();
    out.println(result);
    return;
   }

   DMObject dmobj;
   try
   {
     dmobj = so.getObjectFromNameOrID(objtype, objname);
     oid = dmobj.getId();
   }
   catch (GetObjectException e1)
   {
    dmobj = null;
   }

   if (dmobj == null)
   {
    if (!objname.contains("."))
    {
     ret.add("saved", false);
     ret.add("error", "Object name must include domain");
     String result = ret.getJSON();
     System.out.println(result);
     out.println(result);
     return;
    }

    String shortname = "";

    ArrayList<String> parts = new ArrayList<String>(Arrays.asList(objname.split("\\.")));
    if (parts != null && !parts.isEmpty())
    {
     shortname = parts.get(parts.size()-1);
     parts.remove(parts.size()-1);
    }

    Domain tgtdomain;
    try
    {
     tgtdomain = getDomainFromNameOrID(so, parts.get(0));
    }
    catch (ApiException e1)
    {
     tgtdomain = null;
    }

    String domStr = parts.get(0);
    boolean newDom = false;

    for (int i = 1; i < parts.size(); i++)
    {
     String domain = parts.get(i);
     newDom = false;

     domStr += "." + domain;
     try
     {
      tgtdomain = getDomainFromNameOrID(so, domStr);
     }
     catch (Exception e)
     {
      newDom = true;
     }

     if (newDom)
     {
      int newid = so.getID("domain");
      so.CreateNewObject("domain", domain, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", true);

      tgtdomain = so.getDomain(newid);

      if (tgtdomain == null)
      {
       ret.add("saved", false);
       ret.add("error", "Failed to create domain" + domain);
       String result = ret.getJSON();
       System.out.println(result);
       out.println(result);
       return;
      }
     }
    }

    if (tgtdomain == null)
    {
     ret.add("saved", false);
     ret.add("error", "Failed to create domain for " + objname);
     String result = ret.getJSON();
     System.out.println(result);
     out.println(result);
     return;
    }

    so.GetDomains(so.GetUserID());

    if (objtype.getTableName() == "engine")
     System.out.println(objname);

    oid = so.getID(objtype.getTableName());
    if (objtype == ObjectType.SERVER)
     so.CreateNewObject(objtype.getTableName(), shortname, tgtdomain.getId(), -1, oid, -1, -1, "", false);
    else if (objtype == ObjectType.TASK)
    {
     if (obj.has("tasktype"))
      so.CreateNewTask(shortname,obj.get("tasktype").getAsString(),tgtdomain.getId(), oid);
    }
    else
     so.CreateNewObject(objtype.getTableName(), shortname, tgtdomain.getId(), tgtdomain.getId(), oid, -1, -1, "", false);
   }

   dmobj = so.getDetailedObject(objtype, oid);

   SummaryChangeSet changes = new SummaryChangeSet();

   JsonArray det = obj.get("summary").getAsJsonArray();

   CredentialKind credkind = null;
   if (det != null)
   {
    for (int i=0;i<det.size();i++)
    {
     JsonArray fielddet = det.get(i).getAsJsonArray();
     int fieldid = fielddet.get(0).getAsInt();
     SummaryField field = SummaryField.fromInt(fieldid);
     if (field != null && field == SummaryField.CRED_KIND)
     {
      String value = "";

      if (!fielddet.get(4).isJsonObject())
       value = fielddet.get(4).getAsString();
      else
      {
       JsonObject linked = fielddet.get(4).getAsJsonObject();
       if (linked.has("id"))
       {
         int kindid = linked.get("id").getAsInt();
         credkind = CredentialKind.fromInt(kindid);
       }
      }
     }
    }
   }

   if (det != null)
   {
    for (int i=0;i<det.size();i++)
    {
     JsonArray fielddet = det.get(i).getAsJsonArray();
     int fieldid = fielddet.get(0).getAsInt();
     SummaryField field = SummaryField.fromInt(fieldid);
     String value = "";

     if (!fielddet.get(4).isJsonObject())
      value = fielddet.get(4).getAsString();
     else
     {
      JsonObject linked = fielddet.get(4).getAsJsonObject();

      if (field != null && (field == SummaryField.PRE_ACTION || field == SummaryField.POST_ACTION || field == SummaryField.CUSTOM_ACTION))
      {
       value = linked.get("name").getAsString();
       Action action = null;
       try
       {
        action = getActionFromNameOrID(so, value);
        value = "ac" + action.getId();
       }
       catch (ApiException e)
       {
       }

       if (action == null)
       {
        createDomains4Obj(so, value);
        ObjectType atype = field.type();

        String shortname = "";

        ArrayList<String> parts = new ArrayList<String>(Arrays.asList(value.split("\\.")));
        if (parts != null && !parts.isEmpty())
        {
         shortname = parts.get(parts.size()-1);
         parts.remove(parts.size()-1);
        }
        String actdomname = String.join(".", parts);

        try
        {
         Domain actdom = this.getDomainFromNameOrID(so, actdomname);
         int newid = so.getID("action");

         ObjectTypeAndId otypeid = so.CreateNewAction("G", shortname, actdom.getId(), actdom.getId(), newid);
         value = "ac" + otypeid.getId();
        }
        catch (ApiException e)
        {
        }
       }
      }
      else
      {
       if (linked.has("type") && linked.has("id"))
        value = linked.get("type").getAsString() + linked.get("id").getAsString();
       else
       {
        value = linked.get("type").getAsString();
        if (value.equalsIgnoreCase("bb"))
          value = linked.get("value").getAsString();
       }
      }
     }

     if (field != null && (field == SummaryField.READ_ONLY || field == SummaryField.SERVER_PINGSTART || field == SummaryField.SERVER_PINGEND || field == SummaryField.DOMAIN_FULLNAME))
      continue;

     if(field != null)
     {
      if (field == SummaryField.OWNER)
       value = "us" + new Integer(so.GetUserID()).toString();

      if (field == SummaryField.SERVER_COMPTYPE)
      {
       if (value.contains(";"))
       {
        String parts[] = value.split(";");
        value = parts[1];
        try
        {
         CompType ct = this.getCompTypeFromNameOrID(so, value);
         value = ct.getId() + "";
        }
        catch (Exception e)
        {
         continue;
        }
       }
      }

      if (credkind == null)
         errtext = so.processField(dmobj, field, value, changes);
      else
      {
       if (credkind == CredentialKind.IN_DATABASE && (field == SummaryField.CRED_ENCPASSWORD || field == SummaryField.CRED_ENCUSERNAME))
         errtext = so.processField(dmobj, field, value, changes);
       else if (credkind == CredentialKind.FROM_VARS && (field == SummaryField.CRED_VARUSERNAME || field == SummaryField.CRED_VARPASSWORD))
        errtext = so.processField(dmobj, field, value, changes);
       else if (credkind == CredentialKind.PPK && (field == SummaryField.CRED_VARUSERNAME || field == SummaryField.CRED_FILENAME))
        errtext = so.processField(dmobj, field, value, changes);
       else if (credkind == CredentialKind.HARVEST_DFO_IN_FILESYSTEM && (field == SummaryField.CRED_FILENAME))
        errtext = so.processField(dmobj, field, value, changes);
       else if (field != SummaryField.CRED_ENCPASSWORD &&
                field != SummaryField.CRED_ENCUSERNAME &&
                field != SummaryField.CRED_FILENAME &&
                field != SummaryField.CRED_USERNAME &&
                field != SummaryField.CRED_VARPASSWORD &&
                field != SummaryField.CRED_VARUSERNAME)
        errtext = so.processField(dmobj, field, value, changes);
      }
      if (errtext != null) {
       System.err.println(errtext);
       break;
      }
     } else {
      errtext = "ERROR: Unknown summary field " + fieldid;
      System.err.println(errtext);
      break;
     }
    }
   }

   System.out.println("errtext=["+errtext+"]");

   if (errtext != null) {
    ret.add("saved",false);
    ret.add("error",errtext);
   } else {
    try {
     if(dmobj.isUpdatable()) {
      boolean res = dmobj.updateSummary(changes);
      if (res && dmobj.getObjectType() == ObjectType.USER) {
       // Successful save of a user. Have we changed our own password?
       if (dmobj.getId() == so.GetUserID()) {
        String ep = URLEncoder.encode(so.getPassword(),"UTF-8");
        System.out.println("Saved change to self, recoded password cookies");
       }
      }
      ret.add("saved", res);
     } else {
      ret.add("saved", false);
      ret.add("error", "You do not have permission to update this object");
     }
    } catch(Exception e) {
     System.out.println("exception thrown: "+e.getMessage());
     e.printStackTrace();
     ret.add("saved", false);
     ret.add("error", e.getMessage());
    }
   }

   if (obj.has("members"))
   {
    JsonArray members = obj.get("members").getAsJsonArray();

    for (int i=0;i<members.size();i++)
    {
     String member = members.get(i).getAsString();
     User user = so.getUserByName(member);

     if (user != null)
       so.AddUserToGroup(dmobj.getId(),user.getId());
    }
   }

   if (obj.has("compitems"))
   {
    JsonArray compitems = obj.get("compitems").getAsJsonArray();

    for (int i=0;i<compitems.size();i++)
    {
     JsonArray itemfields = compitems.get(i).getAsJsonArray();
     changes = new SummaryChangeSet();

     for (int k=0;k<itemfields.size();k++)
     {
      JsonArray fielddet = itemfields.get(k).getAsJsonArray();
      int fieldid = fielddet.get(0).getAsInt();
      SummaryField field = SummaryField.fromInt(fieldid);
      String value = "";

      if (!fielddet.get(4).isJsonObject())
       value = fielddet.get(4).getAsString();
      else
      {
       JsonObject linked = fielddet.get(4).getAsJsonObject();
       if (linked.has("type") && linked.has("id"))
        value = linked.get("type").getAsString() + linked.get("id").getAsString();
       else
        value = linked.get("type").getAsString();
      }

      if(field != null)
      {
       errtext = so.processField(dmobj, field, value, changes);
      }
    }
    Component comp = (Component)dmobj;
    comp.updateCompItems(changes);
    }
   }

   if (obj.has("compvers"))
   {
    JsonArray compvers = obj.get("compvers").getAsJsonArray();

    for (int i=0;i<compvers.size();i++)
    {
     JsonObject verobj = compvers.get(i).getAsJsonObject();
     String version = "";
     String pred = "";
     String par = "";

      version = verobj.get("version").getAsString();
      pred = verobj.get("predecessor").getAsString();
      par = verobj.get("parent").getAsString();
      Component parent = null;
      Component predecessor = null;
      Component child = null;
      try
      {
       parent  = so.getComponentByName(par);
      }
      catch(RuntimeException e)
      {
       try
       {
        parent = this.newCompVersion(so, par, request);
       }
       catch (ApiException e1)
       {
        // TODO Auto-generated catch block
        e1.printStackTrace();
       }
      }

      try
      {
       predecessor  = so.getComponentByName(pred);
      }
      catch(RuntimeException e)
      {
       try
       {
        predecessor = this.newCompVersion(so, pred, request);
       }
       catch (ApiException e1)
       {
         // TODO Auto-generated catch block
         e1.printStackTrace();
       }
      }

      try
      {
       child  = so.getComponentByName(version);
      }
      catch(RuntimeException e)
      {
       try
       {
        child = this.newCompVersion(so, version, request);
       }
       catch (ApiException e1)
       {
        // TODO Auto-generated catch block
        e1.printStackTrace();
       }
      }

      if (parent != null && child != null && predecessor != null)
       so.assignComponentVersion(parent, predecessor, child);
    }
   }

   if (obj.has("appvers"))
   {
    JsonArray appvers = obj.get("appvers").getAsJsonArray();

    for (int i=0;i<appvers.size();i++)
    {
     JsonObject verobj = appvers.get(i).getAsJsonObject();
     String version = "";
     String pred = "";
     String par = "";
     Application child = null;
     Application predecessor = null;

      version = verobj.get("version").getAsString();
      pred = verobj.get("predecessor").getAsString();
      par = verobj.get("parent").getAsString();
      Application parent = so.getApplicationByName(par);
      try
      {
       predecessor  = so.getApplicationByName(pred);
      }
      catch (RuntimeException e)
      {
        predecessor = newAppVer(so, pred);
      }

      try
      {
       child  = so.getApplicationByName(version);
      }
      catch (RuntimeException e)
      {
        child = newAppVer(so, version);
      }

      if (parent != null && child != null && predecessor != null)
       so.assignApplicationVersion(parent, predecessor, child);
    }
   }

   if (obj.has("comps"))
   {
    JsonArray comps = obj.get("comps").getAsJsonArray();

    for (int i=0;i<comps.size();i++)
    {
     JsonObject comp = comps.get(i).getAsJsonObject();
     String version = "";
     int xpos = 0;
     int ypos = 0;
     String fromname = "";
     String toname   = "";

     version = comp.get("version").getAsString();
     xpos = new Integer(comp.get("xpos").getAsString()).intValue();
     ypos = new Integer(comp.get("ypos").getAsString()).intValue();

     if (comp.has("from"))
        fromname = comp.get("from").getAsString();

     if (comp.has("to"))
        toname   = comp.get("to").getAsString();

     Component ver = so.getComponentByName(version);
     ver.setXpos(xpos);
     ver.setYpos(ypos);

     Component from = new Component();
     try
     {
      from = so.getComponentByName(fromname);
     }
     catch (RuntimeException e)
     {

     }

     Component to = null;
     try
     {
      to = so.getComponentByName(toname);
     }
     catch (RuntimeException e)
     {

     }
     if (from != null && to != null)
       so.assignComp2App((Application)dmobj, ver, from, to);
    }
   }

   if (obj.has("apps"))
   {
    JsonArray apps = obj.get("apps").getAsJsonArray();

    for (int i=0;i<apps.size();i++)
    {
     JsonObject app = apps.get(i).getAsJsonObject();
     String version = "";
     int xpos = 0;
     int ypos = 0;
     String fromname = "";
     String toname   = "";

     version = app.get("version").getAsString();
     xpos = new Integer(app.get("xpos").getAsString()).intValue();
     ypos = new Integer(app.get("ypos").getAsString()).intValue();
     fromname = app.get("from").getAsString();
     toname   = app.get("to").getAsString();

     Application ver = so.getApplicationByName(version);
     ver.setXpos(xpos);
     ver.setYpos(ypos);

     Application from = new Application();
     try
     {
      from = so.getApplicationByName(fromname);
     }
     catch (RuntimeException e)
     {

     }
     Application to = so.getApplicationByName(toname);
     so.assignApp2Rel((Application)dmobj, ver, from, to);
    }
   }

   if (obj.has("envs"))
   {
    JsonArray envs= obj.get("envs").getAsJsonArray();

    for (int i=0;i<envs.size();i++)
    {
     String envname = envs.get(i).getAsString();
     Environment env = null;

     try
     {
      env = so.getEnvironmentByName(envname);
     }
     catch (RuntimeException e)
     {}

     if (env == null)
     {
      createDomains4Obj(so, envname);

      String shortname = "";

      ArrayList<String> parts = new ArrayList<String>(Arrays.asList(envname.split("\\.")));
      if (parts != null && !parts.isEmpty())
      {
       shortname = parts.get(parts.size()-1);
       parts.remove(parts.size()-1);
      }
      String actdomname = String.join(".", parts);

      try
      {
       Domain actdom = this.getDomainFromNameOrID(so, actdomname);
       int newid = so.getID("environment");

       ObjectTypeAndId otypeid = so.CreateNewObject("environment", shortname, actdom.getId(), actdom.getId(), newid, 0, 0, "environments", true);
       env = so.getEnvironment(otypeid.getId(), true);
      }
      catch (ApiException e)
      {
      }
     }
     so.addToAppsAllowedInEnv(env, (Application)dmobj);
    }
   }

   if (obj.has("endpoints"))
   {
    JsonArray srvs= obj.get("endpoints").getAsJsonArray();

    for (int i=0;i<srvs.size();i++)
    {
     String srvname = srvs.get(i).getAsString();
     Server srv = so.getServerByName(srvname);

     try
     {
      assignServer(so, new Integer(srv.getId()).toString(), new Integer(dmobj.getId()).toString());
     }
     catch (ApiException e)
     {
      e.printStackTrace();
     }
    }
   }

   if (obj.has("attrs"))
   {
    JsonArray attrs = obj.get("attrs").getAsJsonArray();
    AttributeChangeSet acs = new AttributeChangeSet();
    for (int i=0;i<attrs.size();i++)
    {
     JsonObject jo = attrs.get(i).getAsJsonObject();

     for (Entry<String, JsonElement> e: jo.entrySet())
     {
      DMAttribute attr = new DMAttribute();
      attr.setName(e.getKey());
      attr.setValue(e.getValue() != null ? e.getValue().getAsString() : "");
      acs.addChanged(attr);
     }
    }
    so.updateAttributesForObject(dmobj, acs);
   }

   if (obj.has("props"))
   {
    JsonArray props = obj.get("props").getAsJsonArray();
    ACDChangeSet<DMProperty> acs = new ACDChangeSet<DMProperty>();
    for (int i=0;i<props.size();i++)
    {
     JsonObject jo = props.get(i).getAsJsonObject();
     String name = "";
     String value = "";
     boolean encrypted = false;
     boolean overridable = false;
     boolean appendable = false;

     if (jo.has("name"))
      name = jo.get("name").getAsString();

     if (jo.has("value"))
      value = jo.get("value").getAsString();

     if (jo.has("appendable"))
      appendable = jo.get("appendable").getAsBoolean();

     if (jo.has("overridable"))
      overridable = jo.get("overridable").getAsBoolean();

     if (jo.has("encrypted"))
      encrypted = jo.get("encrypted").getAsBoolean();

     DMProperty prop = new DMProperty(name, value, encrypted, overridable, appendable);
     acs.addAdded(prop);
    }

    ((ProviderObject)dmobj).updateProperties(acs,true);
   }

   String result = ret.getJSON();
   System.out.println(result);
   out.println(result);
  }
  catch (IOException e)
  {
   // TODO Auto-generated catch block
   e.printStackTrace();
  }
 }

 private Application newAppVer(DMSession so, String objname)
 {
  DMObject dmobj;
  int oid = 0;
  ObjectType objtype = ObjectType.APPVERSION;

  try
  {
    dmobj = so.getObjectFromNameOrID(objtype, objname);
    oid = dmobj.getId();
  }
  catch (GetObjectException e1)
  {
   dmobj = null;
  }

  if (dmobj == null)
  {
   if (!objname.contains("."))
    return null;

   String shortname = "";

   ArrayList<String> parts = new ArrayList<String>(Arrays.asList(objname.split("\\.")));
   if (parts != null && !parts.isEmpty())
   {
    shortname = parts.get(parts.size()-1);
    parts.remove(parts.size()-1);
   }

   Domain tgtdomain;
   try
   {
    tgtdomain = getDomainFromNameOrID(so, parts.get(0));
   }
   catch (ApiException e1)
   {
    tgtdomain = null;
   }

   String domStr = parts.get(0);
   boolean newDom = false;

   for (int i = 1; i < parts.size(); i++)
   {
    String domain = parts.get(i);
    newDom = false;

    domStr += "." + domain;
    try
    {
     tgtdomain = getDomainFromNameOrID(so, domStr);
    }
    catch (Exception e)
    {
     newDom = true;
    }

    if (newDom)
    {
     int newid = so.getID("domain");
     so.CreateNewObject("domain", domain, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", true);

     tgtdomain = so.getDomain(newid);

     if (tgtdomain == null)
      return null;
    }
   }

   if (tgtdomain == null)
    return null;

   so.GetDomains(so.GetUserID());
   oid = so.getID(objtype.getTableName());
   so.CreateNewObject(objtype.getTableName(), shortname, tgtdomain.getId(), tgtdomain.getId(), oid, -1, -1, "", false);
  }

  dmobj = so.getDetailedObject(objtype, oid);
  return (Application)dmobj;
 }

 public void AddClientId(String domname, DMSession so, HttpServletRequest request, String userid, String clientid)
 {
  String parts[] = domname.split("\\.");

  Domain tgtdomain;
  try
  {
   tgtdomain = getDomainFromNameOrID(so, parts[0]);
  }
  catch (ApiException e1)
  {
   System.err.print(e1.getMessage());
   e1.printStackTrace();
   return;
  }
  Domain companyDomain = null;
  String domStr = parts[0];
  boolean newDom = false;

  for (int i = 1; i < parts.length; i++)
  {
   String domain = parts[i];
   newDom = false;

   if (domain.equalsIgnoreCase("My Pipeline"))
    continue;

   domStr += "." + domain;
   try
   {
    tgtdomain = getDomainFromNameOrID(so, domStr);
   }
   catch (Exception e)
   {
    newDom = true;
   }

   if (newDom)
   {
    int newid = so.getID("domain");
    so.CreateNewObject("domain", domain, tgtdomain.getId(), tgtdomain.getId(), newid, 0, 0, "domains", true);

    tgtdomain = so.getDomain(newid);

    if (tgtdomain == null)
     try
     {
      throw new ApiException("Failed to create new domain");
     }
     catch (ApiException e)
     {
      System.err.print(e.getMessage());
      e.printStackTrace();
      return;
     }
   }

   if (i == 1)
    companyDomain = tgtdomain;
  }

  try
  {
   internalModDomain(so, tgtdomain, request, true);
  }
  catch (ApiException e1)
  {
   System.err.print(e1.getMessage());
   e1.printStackTrace();
   return;
  }

  try
  {
   String full_userid = companyDomain.getFullDomain() + "." + userid;
   User newuser = null;

   try
   {
    newuser = so.getUserByName(full_userid);
   }
   catch (RuntimeException e)
   {

   }

   if (newuser == null)
   {
    int newid = so.getID("user");
    so.CreateNewObject("user", userid, companyDomain.getId(), companyDomain.getId(), newid, -1, -1, "", false);
    // Having created the user, submit the "changes" to set the attributes
    newuser = so.getUser(newid);
    if (newuser == null)
     throw new ApiException("Failed to create new user");
   }
   internalModUser(so, newuser, request);

   UserGroup admin = so.getGroupByName("Administrators");

   so.AddUserToGroup(admin.getId(), newuser.getId());
  }
  catch (ApiException e)
  {
   System.err.print(e.getMessage());
   e.printStackTrace();
   return;
  }

  String from = System.getenv("SMTP_USER");
  String to = request.getParameter("email");
  String companyname = request.getParameter("companyname");
  String projectname = request.getParameter("projectname");
  String firstname = request.getParameter("firstname");
  String lastname = request.getParameter("lastname");
  String tel = request.getParameter("tel");
  String subject = "DeployHub Access";
  String password = System.getenv("SMTP_PASS");
  String msg = "";

  msg += "<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n";
  msg += "<head>\n";
  msg += "<title>DeployHub Access</title>\n";
  msg += "</head>\n";
  msg += "<body>\n";
  msg += "<h2>Thank you for becoming a DeployHub Team User!</h2>\n";
  msg += "<p >You're part of a growing group of cloud native specialists who understand the need for cataloging, versioning, configuring and deploying microservices. To get started access the DeployHub console from the following address: <a href=\"https://console.deployhub.com/dmadminweb/Home\">https://console.deployhub.com</a></p>\n";
  msg += "<p >Your userid is: " + userid + "</p>\n";
  msg += "<p >Your CLIENTID is <strong>" + clientid + "</strong> and will be needed when you install your DeployHub Reverse Proxy. See below.</p>\n";
  msg += "<h2>DeployHub Sample Hipster Store</h2>\n";
  msg += "<p >Once you have logged in, review the <a href=\"https://docs.ortelius.io/guides/userguide/\" target=\"_blank\">Hipster Store sample application</a> to take a test drive.</p>\n";
  msg += "<h2>Your own POC</h2>\n";
  msg += "<p >To setup DeployHub for your organization, review <a href=\"https://docs.ortelius.io/guides/userguide/first-steps/\" target=\"_blank\">the First Steps Guide.</a></p>\n";
  msg += "<p >If you would like to chat one-on-one, book some time with our team.<a href=\"https://go.oncehub.com/SteveTaylor\" target=\"_blank\"> Just choose a time from the calendar.</a></p>\n";
  msg += "<p >DeployHub Team is based on the <a href=\"https://www.ortelius.io\" target=\"_blank\">Ortelius Open Source project</a>. Post questions, issues or comments to the <a href=\"https://github.com/ortelius/ortelius/issues\" target=\"_blank\">Ortelius GitHub Issues Page</a>. You can also join the <a href=\"https://discord.gg/mUtF8w\" target=_blank>Ortelius Discord Chatroom to start a conversation on any topic.</a>    </p>\n";
  msg += "<p >We look forward to working with you on making microservices easy. </p>\n";
  msg += "<p >Sincerely, DeployHub Support Team</p>\n";
  msg += "</body>\n";
  msg += "</html>\n";

  String details = "";
  details += "<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n";
  details += "<head>\n";
  details += "<title>New DeployHub User</title>\n";
  details += "</head>\n";
  details += "<body>\n";
  details += "<p >Company Name: " + companyname + "</p>\n";
  details += "<p >Project Name: " + projectname + "</p>\n";
  details += "<p >First Name: " + firstname + "</p>\n";
  details += "<p >Last Name: " + lastname + "</p>\n";
  details += "<p >Email: " + to + "</p>\n";
  details += "<p >Phone: " + tel + "</p>\n";
  details += "<p >Userid: " + userid + "</p>\n";
  details += "</body>\n";
  details += "</html>\n";

  SendMail sendmsg = new SendMail(from, password, to, subject, msg, details);
  sendmsg.start();
 }

 public void Signup(HttpServletRequest request, HttpServletResponse response)
 {
  try
  {
   DMSession so = new DMSession(request.getSession().getServletContext());
   request.getSession().setAttribute("session", so);
   so.internalLogin(request.getServletContext());

   StringBuilder stringBuilder = new StringBuilder();
   BufferedReader bufferedReader = null;

   try
   {
    bufferedReader = request.getReader();
    String line;
    while ((line = bufferedReader.readLine()) != null)
    {
     stringBuilder.append(line);
    }
   }
   finally
   {
    if (bufferedReader != null)
    {
     bufferedReader.close();
    }
   }

   HashMap<String, String> params = new HashMap<String, String>();

   // Split on "&" to get individual key-value pairs
   String[] pairs = stringBuilder.toString().split("&");

   // Split each pair on "=" and add to the HashMap
   for (String pair : pairs)
   {
    String[] keyValue = pair.split("=");
    if (keyValue.length == 2)
    {
     String key = keyValue[0];
     String value = keyValue[1];
     // You may want to URL decode the value if needed
     value = java.net.URLDecoder.decode(value, "UTF-8");
     params.put(key, value);
    }
   }

   // Iterate through the parameter map
   for (Map.Entry<String, String[]> entry : request.getParameterMap().entrySet())
   {
    String paramName = entry.getKey();
    String[] paramValues = entry.getValue();

    // For simplicity, assume a single value for each parameter
    if (paramValues != null && paramValues.length > 0)
    {
     // Store the first value in the map
     params.put(paramName, java.net.URLDecoder.decode(paramValues[0]));
    }
   }

   String companyname = params.get("companyname");
   String projectname = params.get("projectname");

   companyname = companyname.replaceAll("[^a-zA-Z0-9\\s]", "");
   projectname = projectname.replaceAll("[^a-zA-Z0-9\\s]", "");

   String username = params.get("user-name");
   String password = params.get("user-password");
   String firstname = params.get("firstname");
   String lastname = params.get("lastname");

   params.put("username", username);
   params.put("password", password);
   params.put("pw", password);
   params.put("realname", firstname + " " + lastname);

   String domname = "GLOBAL." + companyname + "." + projectname;

   String msg = null;

   Domain tgtdomain = null;

   boolean duplicateDomain = so.checkDomainByName(domname);
   boolean duplicateUser = so.checkUserName(username);

   if (duplicateDomain)
     msg = "Domain " + domname + " already exists and must be unique.";

   if (duplicateUser)
    msg = "User " + username + "already exists and must be unique.";

   if (duplicateDomain && duplicateUser)
    msg = "Domain " + domname + " and User " + username + "already exists and both must be unique.";

   if (!duplicateDomain && !duplicateUser)
   {
    // Create userid is Open Source Domain
    int newid = so.getID("user");
    User newuser = null;
    try
    {
     tgtdomain = so.getDomainByName("GLOBAL.Open Source");
     so.CreateNewObject("user", username, tgtdomain.getId(), tgtdomain.getId(), newid, -1, -1, "", false);
     newuser = so.getUser(newid);
    }
    catch (Exception e)
    {
     msg = e.getMessage();
    }

    if (newuser != null)
    {
     try
     {
      internalModUser(so, newuser, params);
     }
     catch (ApiException e)
     {
      msg = e.getMessage();
      e.printStackTrace();
     }
    }

    createDomains4Obj(so, domname + "." + username);

    tgtdomain = so.getDomainByName(domname);

    if (msg == null)
    {
     newid = so.getID("user");
     newuser = null;
     try
     {
      so.CreateNewObject("user", username, tgtdomain.getId(), tgtdomain.getId(), newid, -1, -1, "", false);
      newuser = so.getUser(newid);
     }
     catch (Exception e)
     {
      msg = e.getMessage();
     }

     if (newuser != null)
     {
      try
      {
       internalModUser(so, newuser, params);
       String pw = params.get("password");
       so.Login(username, pw, null);

       HttpSession hs = request.getSession();

       hs.setAttribute("logininfo", "Login OK");

       UserGroup admingrp = so.getGroupByName("GLOBAL.Administrators");
       so.AddUserToGroup(admingrp.getId(), newid);

       so.setUserId(newid);
       String jwt = so.authUser();

       if (username == null)
        username = "";

       String redirect = "/dmadminweb/Logout";

       response.sendRedirect(redirect);

       return;
      }
      catch (ApiException e)
      {
       msg = e.getMessage();
       e.printStackTrace();
      }
     }
    }
   }

   String redirect = "https://www.deployhub.com/deployhub-team-signup/?msg=" + java.net.URLDecoder.decode(msg, "UTF-8");
   response.getWriter().write("<html>");
   response.getWriter().write("<head>");
   response.getWriter().write("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/3.7.0/jquery.min.js\" integrity=\"sha512-3gJwYpMe3QewGELv8k/BX9vcqhryRdzRMxVfq6ngyWXwo03GFEzjsUm8Q7RZcHPHksttq7/GFoxjCVUjkjvPdw==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>");
   response.getWriter().write("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jqueryui/1.13.2/jquery-ui.min.js\" integrity=\"sha512-57oZ/vW8ANMjR/KQ6Be9v/+/h6bq9/l3f0Oc7vn6qMqyhvPd1cvKBRWWpzu0QoneImqr2SkmO4MSqU+RpHom3Q==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>");
   response.getWriter().write("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery-cookie/1.4.1/jquery.cookie.min.js\" integrity=\"sha512-3j3VU6WC5rPQB4Ld1jnLV7Kd5xr+cq9avvhwqzbH/taCRNURoeEpoPBK9pDyeukwSxwRPJ8fDgvYXd6SkaZ2TA==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>");
   response.getWriter().write("<script type=\"text/javascript\" src=\"https://console.deployhub.com/dmadminweb/js/ao/cookies.js\"></script>\n");
   response.getWriter().write("</head>");
   response.getWriter().write("<body onload=\"DeleteCookies('" + redirect + "')\">\n");
   response.getWriter().write(msg);
   response.getWriter().write("</body>");
   response.getWriter().write("</html>");
  }
  catch (Exception e)
  {

  }
 }
}
