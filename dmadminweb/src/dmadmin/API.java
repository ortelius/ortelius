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
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

// import oracle.net.aso.i;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.BuildJob;
import dmadmin.model.Builder;
import dmadmin.model.CompType;
import dmadmin.model.Component;
import dmadmin.model.ComponentItem;
import dmadmin.model.Credential;
import dmadmin.model.CredentialKind;
import dmadmin.model.DMAttribute;
import dmadmin.model.DMCalendarEvent;
import dmadmin.model.DMObject;
import dmadmin.model.DMProperty;
import dmadmin.model.Datasource;
import dmadmin.model.DeployedApplication;
import dmadmin.model.Deployment;
import dmadmin.model.Deployment.DeploymentLogEntry;
import dmadmin.model.Domain;
import dmadmin.model.Engine;
import dmadmin.model.Environment;
import dmadmin.model.LoginException.LoginExceptionType;
import dmadmin.model.NotifyTemplate;
import dmadmin.model.Repository;
import dmadmin.model.Server;
import dmadmin.model.ServerType;
import dmadmin.model.Task;
import dmadmin.model.Task.TaskType;
import dmadmin.model.TaskApprove;
import dmadmin.model.TaskCreateVersion;
import dmadmin.model.TaskDeploy;
import dmadmin.model.Transfer;
import dmadmin.model.User;
import dmadmin.model.UserGroup;

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

 protected JSONObject assembleJSONForAppApprovals(DMSession so, Application app)
 {
  return so.applicationApprovalDomains(app);
 }

 protected JSONObject assembleJSONForComponent(DMSession so, Component comp)
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
  BuildJob cbj = comp.getBuildJob();
  if (cbj != null)
  {
    je.add("buildjob",cbj.getName());
    je.add("project",cbj.getProjectName());
    Builder builder = so.getBuilder(cbj.getBuilderId());
    if (builder != null)
    {
      List<DMProperty> props = builder.getProperties();
      for (DMProperty prop: props)
      {
        if (prop.getName().equalsIgnoreCase("server url"))
        {
          je.add("serverurl",prop.getValue());
        }
      }
    }
  }
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
  //
  // Now add all the applications with which this component is associated
  //
  JSONArray applist = new JSONArray();
  List<Application> applications = so.getAppsForComponent(comp);
  for (Application a : applications)
  {
   JSONObject ao = assembleJSONForApplication(so, a);
   applist.add(ao);
  }
  je.add("applications", applist);
  return je;
 }

 private JSONObject assembleJSONForApplication(DMSession so, Application app)
 {
  JSONObject je = assembleJSONForAppApprovals(so, app);
  je.add("id", app.getId());
  je.add("name", app.getName());
  je.add("branch", app.getLabel(true));
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
   JSONObject co = new JSONObject();
   co.add("id", c.getId());
   co.add("name", c.getName());
   co.add("summary", c.getSummary());
   co.add("domain",c.getDomain().getFullDomain());
   co.add("lastbuild", c.getLastBuildNumber());
   BuildJob cbj = c.getBuildJob();
   if (cbj != null)
   {
     co.add("buildjob",cbj.getName());
     co.add("project",cbj.getProjectName());
     Builder builder = so.getBuilder(cbj.getBuilderId());
     if (builder != null)
     {
       List<DMProperty> props = builder.getProperties(); 
       for (DMProperty prop: props)
       {
         if (prop.getName().equalsIgnoreCase("server url"))
         {
           co.add("serverurl",prop.getValue());
		 }
	   }
      }
	}
   complist.add(co);
  }
  je.add("components", complist);
  return je;
 }

 protected JSONObject assembleJSONForEnvironment(DMSession so, Environment env)
 {
  JSONObject je = new JSONObject();
  je.add("id", env.getId());
  je.add("name", env.getName());
  je.add("domain", env.getDomain().getFullDomain());
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
  return je;
 }

 protected JSONObject assembleJSONForServer(DMSession so, Server server)
 {
  JSONObject je = new JSONObject();
  je.add("id", server.getId());
  je.add("name", server.getName());
  je.add("domain", server.getDomain().getFullDomain());
  je.add("hostname", server.getHostName());
  je.add("protocol", server.getProtocol());
  je.add("basedir", server.getBaseDir());
  Credential cred = server.getCredential();
  if (cred != null)
  {
   je.add("credential", cred.getName());
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

 private DMObject getObjectFromNameOrID(DMSession so, ObjectType ot, String idOrName) throws ApiException
 {
  int id;
  try
  {
   id = Integer.parseInt(idOrName);
  }
  catch (NumberFormatException e)
  {
   id = 0;
  }
  DMObject obj = null;
  try
  {
   switch (ot)
   {
    case COMPONENT:
     obj = (id > 0) ? so.getComponent(id, false) : so.getComponentByName(idOrName);
     break;
    case APPLICATION:
     obj = (id > 0) ? so.getApplication(id, false) : so.getApplicationByName(idOrName);
     break;
    case ENVIRONMENT:
     obj = (id > 0) ? so.getEnvironment(id, true) : so.getEnvironmentByName(idOrName);
     break;
    case SERVER:
     obj = (id > 0) ? so.getServer(id, false) : so.getServerByName(idOrName);
     break;
    case DOMAIN:
     obj = (id > 0) ? so.getDomain(id) : so.getDomainByName(idOrName);
     break;
    case USER:
     obj = (id > 0) ? so.getUser(id) : so.getUserByName(idOrName);
     break;
    case DATASOURCE:
     obj = (id > 0) ? so.getDatasource(id, false) : so.getDatasourceByName(idOrName);
     break;
    case CREDENTIALS:
     obj = (id > 0) ? so.getCredential(id, false) : so.getCredentialByName(idOrName);
     break;
    case TEMPLATE:
     obj = (id > 0) ? so.getTemplate(id) : so.getTemplateByName(idOrName);
     break;
    case USERGROUP:
     obj = (id > 0) ? so.getGroup(id) : so.getGroupByName(idOrName);
     break;
    case REPOSITORY:
        obj = (id>0)?so.getRepository(id,true):so.getRepositoryByName(idOrName);
        break;	 
    default:
     throw new ApiException(ot.toString() + " not supported in getObjectFromNameOrID");
   }
   if (obj == null)
   {
    String d = ot.toString();
    String desc = d.substring(0, 1).toUpperCase() + d.substring(1).toLowerCase();
    throw new ApiException(desc + " with id " + id + " not found");
   }
  }
  catch (RuntimeException ex)
  {
   // getApplicationByName throws runtime exception
   throw new ApiException(ex.getMessage());
  }
  return obj;
 }

 protected Component getComponentFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (Component) getObjectFromNameOrID(so, ObjectType.COMPONENT, idOrName);
 }

 protected Application getApplicationFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (Application) getObjectFromNameOrID(so, ObjectType.APPLICATION, idOrName);
 }

 protected Environment getEnvironmentFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (Environment) getObjectFromNameOrID(so, ObjectType.ENVIRONMENT, idOrName);
 }

 protected Server getServerFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (Server) getObjectFromNameOrID(so, ObjectType.SERVER, idOrName);
 }

 protected Domain getDomainFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (Domain) getObjectFromNameOrID(so, ObjectType.DOMAIN, idOrName);
 }

 protected User getUserFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (User) getObjectFromNameOrID(so, ObjectType.USER, idOrName);
 }

 private Datasource getDatasourceFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (Datasource) getObjectFromNameOrID(so, ObjectType.DATASOURCE, idOrName);
 }

 private Repository getRepositoryFromNameOrID(DMSession so,String idOrName) throws ApiException
 {
  return (Repository)getObjectFromNameOrID(so,ObjectType.REPOSITORY,idOrName);
 }

 protected Credential getCredentialFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (Credential) getObjectFromNameOrID(so, ObjectType.CREDENTIALS, idOrName);
 }

 private NotifyTemplate getTemplateFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (NotifyTemplate) getObjectFromNameOrID(so, ObjectType.TEMPLATE, idOrName);
 }

 private UserGroup getGroupFromNameOrID(DMSession so, String idOrName) throws ApiException
 {
  return (UserGroup) getObjectFromNameOrID(so, ObjectType.USERGROUP, idOrName);
 }

 private void internalModUser(DMSession so, User moduser, HttpServletRequest request) throws ApiException
 {
  Datasource ldap = null;
  SummaryChangeSet changes = new SummaryChangeSet();
  boolean bcpw = false;
  boolean blocked = false;
  String realname = request.getParameter("realname");
  /*
  String ldapname = request.getParameter("ldap");
  if (ldapname != null)
  {
   ldap = getDatasourceFromNameOrID(so, ldapname);
  }
  */
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

 protected JSONObject tsObject(int timein)
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

 protected JSONObject userObject(DMSession so, int uid)
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

 protected long convertDateTime(String timein) throws ApiException
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
 
 private void internalModDomain(DMSession so, Domain moddom, HttpServletRequest request) throws ApiException
 {
  SummaryChangeSet changes = new SummaryChangeSet();
  String summary = request.getParameter("lifecycle");
  if (summary != null && summary.equalsIgnoreCase("Y"))
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
  String comptypes = request.getParameter("comptypes");
  String sshport = request.getParameter("sshport");

  if (comptypes != null)
  {
   String idlist = "";
   String sep = "";
   List<CompType> vts = so.getCompTypes();
   String[] cts = comptypes.split(",");
   for (int i = 0; i < cts.length; i++)
   {
    System.out.println("cts[" + i + "]=[" + cts[i] + "]");
    lookupOK = false;
    for (CompType vt : vts)
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
     throw new ApiException("Invalid Component Type " + cts[i]);
   }
   changes.add(SummaryField.SERVER_COMPTYPE, idlist);
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
    throw new ApiException("Invalid End Point Type \"" + type + "\" specified");
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

 protected void ModUser(DMSession so, String username, HttpServletRequest request) throws ApiException
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

 protected void ModServer(DMSession so, String servername, HttpServletRequest request) throws ApiException
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

 protected void ModEnvironment(DMSession so, String envname, HttpServletRequest request) throws ApiException
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

 protected void ModCredential(DMSession so, String credname, HttpServletRequest request) throws ApiException
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

 protected void AddUser(DMSession so, String username, HttpServletRequest request) throws ApiException
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
   Domain  newdom = so.getDomain(newid);
   if (newdom == null)
    throw new ApiException("Failed to create new domain");
   internalModDomain(so, newdom, request);
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
 }

 
 protected void AddEnvironment(DMSession so, String envname, HttpServletRequest request) throws ApiException
 {
  // API/new/environment/name[?domain=<domain>&summary=<summary>]
  // Do we have create environment permission?
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

 protected void AddCredential(DMSession so, String credname, HttpServletRequest request) throws ApiException
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

 private void AddComponentItems(DMSession so, String compname, String compitem, int xpos, int ypos, boolean removeAll, HttpServletRequest request, String repo, String pattern, String uri) throws ApiException
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

   if (removeAll)  // Used when adding the first item
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

   so.componentItemMoveItem(comp.getId(),compi.getId(),xpos,ypos);
   
   SummaryChangeSet schanges = new SummaryChangeSet();
   SummaryField field = SummaryField.fromInt(SummaryField.ITEM_REPOSITORY.value());
   if(field != null) {
    Repository r = getRepositoryFromNameOrID(so, repo);
    so.processField(so, field, "re" + r.getId(), schanges);
   }
   
   ACDChangeSet<DMProperty> pchanges = new ACDChangeSet<DMProperty>();
   
   pchanges.addAdded(new DMProperty("pattern",pattern,false,false,false));
   pchanges.addAdded(new DMProperty("uri",uri,false,false,false));   

   
    ComponentItem ci = so.getComponentItem(compi.getId(), true);
    if(ci.isUpdatable()) {    
     boolean res = true;
     if(!schanges.isEmpty()) {
      ci.updateSummary(schanges);
     }
     if(res && !pchanges.isEmpty()) {
      res = ci.updateProperties(pchanges);
     }
    } 
  }
  catch (RuntimeException e)
  {
   throw new ApiException(e.getMessage());
  }
  
 }

 protected void AddServer(DMSession so, String servername, HttpServletRequest request) throws ApiException
 {
  // API/new/server/name[?domain=<domain>&env=<env>]
  // Do we have create server permission?
  if (!so.getCreatePermission(ObjectType.SERVER))
  {
   // No!
   throw new ApiException("No Create Server Permission");
  }
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

 protected void cloneServer(DMSession so, String servername, String newname, HttpServletRequest request) throws ApiException
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

 protected void cloneEnvironment(DMSession so, String envname, String newname, HttpServletRequest request) throws ApiException
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

 protected void assignServer(DMSession so, String servername, String envname) throws ApiException
 {
  Server server = getServerFromNameOrID(so, servername);
  Environment env = getEnvironmentFromNameOrID(so, envname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  so.MoveServer(env.getId(), server.getId(), 0, 0);
 }

 protected void assignApplication(DMSession so, String appname, String envname) throws ApiException
 {
  Environment env = getEnvironmentFromNameOrID(so, envname);
  Application app = getApplicationFromNameOrID(so, appname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  so.addToAppsAllowedInEnv(env, app);
 }

 protected void assignUser(DMSession so, String username, String groupname) throws ApiException
 {
  User user = getUserFromNameOrID(so, username);
  UserGroup group = getGroupFromNameOrID(so, groupname);
  so.AddUserToGroup(group.getId(), user.getId());
 }

 protected void deAssignServer(DMSession so, String servername, String envname) throws ApiException
 {
  Server server = getServerFromNameOrID(so, servername);
  Environment env = getEnvironmentFromNameOrID(so, envname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  so.RemoveServerFromEnvironment(env.getId(), server.getId());
 }

 protected void deAssignApplication(DMSession so, String appname, String envname) throws ApiException
 {
  Environment env = getEnvironmentFromNameOrID(so, envname);
  Application app = getApplicationFromNameOrID(so, appname);
  if (!env.isUpdatable())
   throw new ApiException("Permission Denied");
  so.removeFromAppsAllowedInEnv(env, app);
 }

 protected void deAssignUser(DMSession so, String username, String groupname) throws ApiException
 {
  User user = getUserFromNameOrID(so, username);
  UserGroup group = getGroupFromNameOrID(so, groupname);
  so.RemoveUserFromGroup(group.getId(), user.getId());
 }

 private void addBuildNumber(DMSession so, String compname, String buildnumber, HttpServletRequest request) throws ApiException
 {
  // Do some sanity checking
  Component basecomp = getComponentFromNameOrID(so, compname);
  String branch = request.getParameter("branch");
  String taskname = request.getParameter("taskname");
  TaskCreateVersion tcv = null;
  if (taskname == null)
   taskname = request.getParameter("task");
  if (taskname != null)
  {
   // taskname specified. Make sure it exists and is executable.
   try
   {
    Task t = so.getTaskByName(taskname); // Throws RuntimeException is taskname is not unique or does not exist
    if (t.getTaskType() != TaskType.CREATE_VERSION)
     throw new ApiException("Specified task is not a Create Version task");
    tcv = so.getTaskCreateVersion(t.getId());
   }
   catch (Exception ex)
   {
    throw new ApiException(ex.getMessage());
   }
  }
  try
  {
   int buildno = Integer.parseInt(buildnumber); // will throw NumberFormatException if there's a problem
   Component comp = so.getLatestVersion(basecomp, branch);
   BuildJob buildjob = comp.getBuildJob();
   if (buildjob == null)
    throw new ApiException("Component " + compname + " has no associated build job");
   // Get any application associated with this component
   List<Application> approvedApps = new ArrayList<Application>();
   List<Application> applications = so.getAppsForComponent(comp);
   for (Application app : applications)
   {
    List<Domain> domlist = so.getApprovalDomains(app.getId());
    if (domlist.size() > 0)
     approvedApps.add(app);
   }
   if (approvedApps.size() > 0)
   {
    // This application is approved. Need to create a new version of the component
    System.out.println("Component " + comp.getName() + " is associated with the following approved applications:");
    for (Application aa : approvedApps)
    {
     System.out.println(aa.getName());
    }
    System.out.println("Creating new Component Version");
    int newid = so.componentNewVersion(comp.getId(), comp.getXpos(), comp.getYpos() + 50);
    System.out.println("newid=" + newid);
    Component newcomp = so.getComponent(newid, true);
    if (newcomp == null)
     throw new ApiException("Failed to create new component");
    // Now find the applications that were approved and create new versions of those applications,
    // replacing the old component with the new one
    for (Application aa : approvedApps)
    {
     Domain domain = aa.getDomain();
     String fqdn = (domain != null) ? domain.getFullDomain() : "";
     System.out.println("Creating new application version for " + fqdn + "." + aa.getName());
     Application nav = newAppVersion(so, fqdn + "." + aa.getName(), request, tcv);
     if (nav == null)
      throw new ApiException("Could not create new application version");
     System.out.println("Created new application " + nav.getName());
     System.out.println("Replacing component " + comp.getName() + " with component " + newcomp.getName() + " in app " + nav.getName());
     so.applicationReplaceComponent(nav.getId(), comp.getId(), newcomp.getId(), false);
    }
    comp = newcomp;
   }

   // Now add any other details associated with this build
   // There are two ways of passing this information
   // files=<file1>&files=<file2> etc
   // or
   // files=file1,file2 etc

   String filepar = request.getParameter("files");
   String files[] = (filepar != null) ? filepar.split(",") : request.getParameterValues("file");
   String commit = request.getParameter("commit");
   so.AddBuildToComponent(comp, buildno, files, commit, true);
  }
  catch (NumberFormatException ex)
  {
   throw new ApiException("buildnumber " + buildnumber + " is not valid");
  }
 }

 protected JSONArray buildNotify(DMSession so, String encodedurl) throws ApiException
 {
  // buildurl should be in form http://hostname:port/maybe/other/bits/job/<jobname>/<buildno>
  try
  {
   return so.recordJenkinsBuild(encodedurl);
  }
  catch (Exception e)
  {
   // TODO Auto-generated catch block
   throw new ApiException(e.getMessage());
  }
 }

 protected Application newAppVersion(DMSession so, String appname, HttpServletRequest request) throws ApiException
 {
  // This is called from API/newappver (deprecated) and API/new/appver (going forward) - get the
  // create version from any taskname specified
  TaskCreateVersion tcv = null;
  String taskname = request.getParameter("taskname");
  if (taskname == null)
   taskname = request.getParameter("task");
  if (taskname != null)
  {
   // taskname specified - look it up
   try
   {
    Task t = so.getTaskByName(taskname); // throws exception if not found or no access
    if (t.getTaskType() != TaskType.CREATE_VERSION)
     throw new ApiException("Specified task is not a Create Version task");
    tcv = so.getTaskCreateVersion(t.getId());
   }
   catch (Exception ex)
   {
    throw new ApiException(ex.getMessage());
   }
  }
  return newAppVersion(so, appname, request, tcv);
 }

 protected Application newAppVersion(DMSession so, String appname, HttpServletRequest request, TaskCreateVersion tcv) throws ApiException
 {
  System.out.println("DOING newappver");
  String newname = request.getParameter("name");	// if specified, name of new application version
  if (newname != null) {
	  System.out.println("Looking up "+newname);
	  try {
		  so.getApplicationByName(newname);	// throws exception if not found
		  // If we get here then application with that name was found. That's an error
		  throw new ApiException("An application version with that name already exists");
	  } catch(RuntimeException ex) {
		  // Application not found exception - ok to proceed
	  }
  }
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
   if (newname != null) {
	   // Need to rename the application
	   System.out.println("Changing name of app with id "+newid+" to "+newname);
	   SummaryChangeSet changes = new SummaryChangeSet();
	   changes.add(SummaryField.NAME,newname);
	   so.updateApplication(newapp, changes);
	   newapp = so.getApplication(newid,false);
   }
   return newapp;
  }
  else
  {
   return (Application) null;
  }
 }

 protected Component newCompVersion(DMSession so, String compname, HttpServletRequest request) throws ApiException
 {
  Component comp = getComponentFromNameOrID(so, compname);
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
  so.componentAddVersionDependency(baseComp.getId(),comp.getId(),newComp.getId()); 

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
  //     API/components?all=y|n[&application=appname|appid] (lists all components - all = show sub-domains)
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
  // API/new/compver/compname
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

  JSONObject obj = new JSONObject();

  int deploymentid = 0; // for deployments

  PrintWriter out = response.getWriter();
  response.setContentType("application/json");
  boolean delop = false;

  DMSession so = null;

  try
  {
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

   if (elements.length < 1)
   {
    throw new ApiException("Invalid request path");
   }

   String user = request.getParameter("user");
   String pass = request.getParameter("pass");
   String a = request.getParameter("all");
   boolean all = (a == null) ? false : (a.charAt(0) == 'y' || a.charAt(0) == 'Y');

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
    so = new DMSession(request.getSession());
    if (so.Login(user, pass).getExceptionType() != LoginExceptionType.LOGIN_OKAY)
    {
     throw new ApiException("Login failed");
    }
    obj.add("success", true);

    HttpSession session = request.getSession();
    session.setAttribute("session", so);
    return; // finally will send result
   }

   if (user != null)
   {
    so = new DMSession(request.getSession());
    if (pass == null)
     throw new ApiException("password must be specified");
    if (so.Login(user, pass).getExceptionType() != LoginExceptionType.LOGIN_OKAY)
    {
     throw new ApiException("Login failed");
    }
   }

   if (so == null)
   {
    HttpSession session = request.getSession();
    so = (DMSession) session.getAttribute("session");
    System.out.println("so = " + so);
   }

   if (so == null)
   {
    throw new ApiException("Not logged in");
   }

   if (elements[0].equals("deploy"))
   {
    // Go invoke a deployment
    System.out.println("API: deploy");
    deploymentid = doDeployment(so, elements, request);
   }
   else if (elements[0].equals("environment"))
   {
    System.out.println("API: Environment");
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
   else if (elements[0].equals("components"))
   {
	   if (elements.length == 1) {
		   System.out.println("length is 1");
		   String appname = request.getParameter("application");
		   JSONArray result = new JSONArray();
		   if (appname != null) {
			   System.out.println("Doing complist for application");
			   // Getting components for application
			   Application app = getApplicationFromNameOrID(so, appname);
			   System.out.println("Application is " + app.getName());
			   List<Component> comps = so.getComponents(ObjectType.APPLICATION,app.getId(),false);
			   System.out.println("component list is size " + comps.size());
			   for (Component c : comps)
			   {
				   Component comp = so.getComponent(c.getId(), true);
				   JSONObject je = assembleJSONForComponent(so, comp);
				   result.add(je);
			   }
		   } else {
			   System.out.println("Application is null");
			   List<DMObject> comps = so.getDMObjects(ObjectType.COMPONENT, all);
			   System.out.println("component list is size " + comps.size());
			   for (DMObject comp : comps)
			   {
				   JSONObject je = assembleJSONForComponent(so, (Component)comp);
				   result.add(je);
			   }
		   }
		   obj.add("result", result);
	   }
   }
   else if (elements[0].equals("component"))
   {
    System.out.println("API: component");
    if (elements.length == 2)
    {
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
       obj.add("result", assembleJSONForComponent(so, lcomp));
      }
      else
      {
       throw new ApiException("No Component Found");
      }
     }
     else
     {
      obj.add("result", assembleJSONForComponent(so, comp));
     }
    }
    else
    {
     throw new ApiException("Path contains wrong number of elements");
    }
   }
   else if (elements[0].equals("application"))
   {
    System.out.println("API: Application");
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
       obj.add("result", assembleJSONForApplication(so, lapp));
      }
      else
      {
       throw new ApiException("No Application Found");
      }
     }
     else
     {
      obj.add("result", assembleJSONForApplication(so, app));
     }
    }
    else
    {
     throw new ApiException("Path contains wrong number of elements");
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
   else if (elements[0].equals("servers"))
   {
    if (elements.length == 1)
    {
     List<DMObject> servers = so.getDMObjects(ObjectType.SERVER, all);
     JSONArray result = new JSONArray();
     for (DMObject s : servers)
     {
      Server server = (Server) s;
      result.add(assembleJSONForServer(so, server));
     }
     obj.add("result", result);
    }
    else
    {
     throw new ApiException("Path contains too many elements");
    }
   }
   else if (elements[0].equals("server"))
   {
    System.out.println("API: server");
    if (elements.length == 2)
    {
     Server server = getServerFromNameOrID(so, elements[1]);
     System.out.println(server.getName());
     obj.add("result", assembleJSONForServer(so, server));
    }
    else
    {
     throw new ApiException("Path contains wrong number of elements");
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
     obj.add("result", assembleJSONForApplication(so, newapp));
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
     obj.add("result", assembleJSONForComponent(so, newComp));
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
   else if (elements[0].equals("setvar"))
   {

    System.out.println("DOING setvar");
    if (elements.length == 3)
    {
     String objtype = elements[1];
     String objname = elements[2];
     String varname = request.getParameter("name");
     String varval = request.getParameter("value");
     if (varname == null)
      throw new ApiException("name not specified");
     AttributeChangeSet acs = new AttributeChangeSet();
     DMAttribute attr = new DMAttribute();
     attr.setName(varname);
     attr.setValue(varval != null ? varval : "");
     acs.addChanged(attr);
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
         jo.add(att.getName(), att.getValue());
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
    if (id == 0 || so.validateDeploymentId(id) == false)
    {
     throw new ApiException("id invalid or no access");
    }
    else
    {
     Deployment dep = so.getDeployment(id, true);
     //
     // Check that the deployment is to an environment to which we have access
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
      throw new ApiException("id invalid or no access");
     }
    }
   }
   else if (elements[0].equals("new"))
   {
    // Adding a new object
    if (elements.length == 3)
    {
     if (elements[1].equals("user"))
      AddUser(so, elements[2], request);
     else if (elements[1].equals("server"))
      AddServer(so, elements[2], request);
     else if (elements[1].equals("appver"))
      newAppVersion(so, elements[2], request);
     else if (elements[1].equals("compver"))
      newCompVersion(so, elements[2], request);
     else if (elements[1].equals("environment"))
      AddEnvironment(so, elements[2], request);
     else if (elements[1].equals("domain")) 
      AddDomain(so,elements[2],request);
     else if (elements[1].equals("credential"))
      AddCredential(so, elements[2], request);
     else 
	  if (elements[1].equals("compitem")) 
       {
        String compname = request.getParameter("component");
        String xStr     = request.getParameter("xpos");
        String yStr     = request.getParameter("ypos");
        String removeAllStr = request.getParameter("removeall");
        String repo = request.getParameter("repo");
        String pattern = request.getParameter("pattern");
        String uri = request.getParameter("uri");
        int xpos = 0;
        int ypos = 0;
        boolean removeAll = false;
        
        if (repo == null)
          repo = "";
        
        if (pattern == null)
         pattern = "";
        
        if (uri == null)
         uri = "";
        
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
        
        AddComponentItems(so, compname, elements[2], xpos, ypos, removeAll, request,repo,pattern,uri);
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
     else if (elements[1].equals("server"))
     {
      String servername = elements[2];
      Server servertodel = getServerFromNameOrID(so, servername);
      if (!servertodel.isUpdatable())
       throw new ApiException("Permission Denied");
      so.RemoveObject("server", servertodel.getId(), out, true);
      delop = true;
     }
     if (elements[1].equals("environment"))
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
      assignApplication(so, elements[2], elements[3]);
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
   else if (elements[0].equals("buildid"))
   {
    // Associates a build ID with the specified component
    // API/buildid/<compname>/<buildid>
    if (elements.length == 3)
    {
     addBuildNumber(so, elements[1], elements[2], request);
    }
    else
    {
     throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
    }
   }
   else if (elements[0].equals("buildnotify"))
   {
    // API/buildnotify?buildurl=<buildurl>
    if (elements.length == 1)
    {
     String buildurl = request.getParameter("buildurl");
     if (buildurl == null)
      throw new ApiException("buildurl parameter not specified");
     JSONArray info = buildNotify(so, buildurl);
     obj.add("affected_objects", info);
    }
    else
    {
     throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
    }
   }
   else if (elements[0].equals("calendar"))
   {
    // Listing calendar entries
    // API/calendar?[env=<env>&app=<app>][&start=<start>&end=<end>&all=Y&deploy=Y]
    String envname = request.getParameter("env");
    String appname = request.getParameter("app");
    String starttext = request.getParameter("start");
    String endtext = request.getParameter("end");
    long starttime = 0;
    long endtime = 0;
    if (starttext != null)
    {
     starttime = convertDateTime(starttext);
    }
    else
    {
     starttime = so.timeNow();
    }
    if (endtext != null)
     endtime = convertDateTime(endtext);
    Environment env = null;
    Application app = null;

    if (envname != null)
     env = getEnvironmentFromNameOrID(so, envname);
    if (appname != null)
     app = getApplicationFromNameOrID(so, appname);
    List<DMCalendarEvent> events = so.getEvents(env, app, starttime, endtime);
    JSONArray es = new JSONArray();
    for (DMCalendarEvent event : events)
    {
     JSONObject jo = new JSONObject();
     jo.add("id", event.getID());
     jo.add("title", event.getEventTitle());
     jo.add("desc", event.getEventDesc());
     jo.add("type", event.getEventTypeString());
     jo.add("starttime", tsObject(event.getStart()));
     jo.add("endtime", tsObject(event.getEnd()));
     jo.add("allday", event.getAllDayEvent());
     if (event.getApproved() > 0)
     {
      jo.add("approved", tsObject(event.getApproved()));
      jo.add("approver", event.getApprover());
     }
     jo.add("creator", userObject(so, event.getCreatorID()));
     jo.add("created", tsObject(event.getCreated()));
     jo.add("deployid", event.getDeployID());
     jo.add("pending", event.getPending());
     int appid = event.getAppID();
     int envid = event.getEnvID();
     if (appid > 0)
     {
      Application eventapp = so.getApplication(appid, false);
      JSONObject appjo = new JSONObject();
      appjo.add("id", eventapp.getId());
      appjo.add("name", eventapp.getName());
      jo.add("application", appjo);
     }
     if (envid > 0)
     {
      Environment eventenv = so.getEnvironment(envid, false);
      JSONObject envjo = new JSONObject();
      envjo.add("id", eventenv.getId());
      envjo.add("name", eventenv.getName());
      jo.add("environment", envjo);
     }
     es.add(jo);
    }
    obj.add("result", es);
   }
   else
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
    String ret = obj.getJSON();
    System.out.println(ret);
    out.println(ret);
   }
  }
 }

 /**
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  doGet(request, response);
 }

 protected int doDeployment(DMSession so, String[] elements, HttpServletRequest request) throws ApiException
 {
  System.out.println("doDeployment");
  if (elements.length < 2)
  {
   throw new ApiException("No application specified");
  }
  int deploymentid = 0;
  Application app = getApplicationFromNameOrID(so, elements[1]);

  System.out.println("Application found is " + app.getId());
  String envName = request.getParameter("env");
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
  boolean success = task.run();
  System.out.println("Grabbing deploymentid");
  deploymentid = task.getDeploymentID();
  System.out.println("Got it " + deploymentid);

  if (!wait)
  {
   if (!success)
   {
    String msg = task.getLastOutputLine();
    if (msg != null)
    {
     System.out.println(msg);
    }
   }
  }

  if (!success)
  {
   throw new ApiException("Failed to deploy");
  }
  return deploymentid;
 }
}
