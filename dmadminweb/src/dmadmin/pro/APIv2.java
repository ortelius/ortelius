package dmadmin.pro;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.StringJoiner;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.API;
import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.ObjectTypeAndId;
import dmadmin.ServletUtils;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;
import dmadmin.model.ComponentItem;
import dmadmin.model.DMAttribute;
import dmadmin.model.Datasource;
import dmadmin.model.Deployment;
import dmadmin.model.Environment;
import dmadmin.model.JWTGenerateValidateRSA;
import dmadmin.model.LoginException.LoginExceptionType;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jws;

public class APIv2 extends API
{

 /**
  *
  */
 private static final long serialVersionUID = -7188687893128840994L;

 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  PrintWriter out = response.getWriter();

  String path = request.getPathInfo();

 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  try (DMSession so = DMSession.getInstance(request))
  {

   JSONObject obj = new JSONObject();

   int deploymentid = 0; // for deployments

   PrintWriter out = null;
   out = response.getWriter();

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
     elements[i] = java.net.URLDecoder.decode(elements[i], "UTF-8");

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
     user = ServletUtils.GetCookie(request, "p1");
     if (user != null)
      user = URLDecoder.decode(user);
    }

    if (pass == null)
     pass = ServletUtils.GetCookie(request, "p2");

    String jwt = ServletUtils.GetCookie(request, "token");
    if (jwt != null && jwt.trim().length() > 0)
    {
     Jws<Claims> token = JWTGenerateValidateRSA.parseJwt(jwt);
     String uuid = token.getBody().getId();
     String userid = token.getBody().getSubject();
     authorized = so.validateAuth(Integer.parseInt(userid), uuid);
    }

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

      if (so.Login(user, pass, null).getExceptionType() != LoginExceptionType.LOGIN_OKAY)
      {
       throw new ApiException("Login failed");
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
      loggedinUser.setPath("/");
      loggedinPw.setPath("/");

      response.addCookie(loggedinUser);
      response.addCookie(loggedinPw);
      response.addCookie(loggedinTime);
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

    if (elements[0].equals("assign"))
    {
     // API/assign/defect/<objid>
     if (elements.length == 3)
     {
      if (elements[1].equals("defect"))
       assignDefect(so, elements[2], request);
     }
    }
    else if (elements[0].equals("defect"))
    {
     Defect defect = null;
     System.out.println("API: defect");
     obj.add("result", assembleJSONForDefect(so, defect));
    }
    else if (elements[0].equals("diff"))
    {
     String leftobj = request.getParameter("left");
     String rightobj = request.getParameter("right");
     String result = "";

     if (leftobj != null && rightobj != null && (leftobj.startsWith("av") || leftobj.startsWith("ap")) && (rightobj.startsWith("av") || rightobj.startsWith("ap")))
     {
      Application left = getApplicationFromNameOrID(so, leftobj.substring(2));
      Application right = getApplicationFromNameOrID(so, rightobj.substring(2));
      String leftyaml = diffForApplication(so, left);
      String rightyaml = diffForApplication(so, right);

      Path temppath = Files.createTempDirectory("dh-");

      File lfile = new File(temppath.toString() + "/" + left.getName());
      FileWriter fileWriter = new FileWriter(lfile);
      fileWriter.write(leftyaml);
      fileWriter.close();

      File rfile = new File(temppath.toString() + "/" + right.getName());
      fileWriter = new FileWriter(rfile);
      fileWriter.write(rightyaml);
      fileWriter.close();

      ProcessBuilder pb = new ProcessBuilder();
      Map<String, String> env = pb.environment();
      String pathenv = "/usr/local/bin:" + env.get("PATH");
      env.put("PATH", pathenv);

      System.out.println(temppath.toString());
      pb.directory(temppath.toFile());
      pb.redirectErrorStream(true);

      ArrayList<String> cmd = new ArrayList<String>();
      cmd.add("diff");
      cmd.add("-U");
      cmd.add("4611686018427387903");
      cmd.add(left.getName());
      cmd.add(right.getName());
      pb.command("bash", "-c", "diff -U 4611686018427387903 \"" + left.getName() + "\" \"" + right.getName() + "\"");
      result = getOutput(pb);
     }
     else if (leftobj != null && rightobj != null && (leftobj.startsWith("cv") || leftobj.startsWith("co")) && (rightobj.startsWith("cv") || rightobj.startsWith("co")))
     {
      Component left = getComponentFromNameOrID(so, leftobj.substring(2));
      Component right = getComponentFromNameOrID(so, rightobj.substring(2));
      String leftyaml = diffForComponent(so, left);
      String rightyaml = diffForComponent(so, right);

      Path temppath = Files.createTempDirectory("dh-");

      File lfile = new File(temppath.toString() + "/" + left.getName());
      FileWriter fileWriter = new FileWriter(lfile);
      fileWriter.write(leftyaml);
      fileWriter.close();

      File rfile = new File(temppath.toString() + "/" + right.getName());
      fileWriter = new FileWriter(rfile);
      fileWriter.write(rightyaml);
      fileWriter.close();

      ProcessBuilder pb = new ProcessBuilder();
      Map<String, String> env = pb.environment();
      String pathenv = "/usr/local/bin:" + env.get("PATH");
      env.put("PATH", pathenv);

      System.out.println(temppath.toString());
      pb.directory(temppath.toFile());
      pb.redirectErrorStream(true);

      ArrayList<String> cmd = new ArrayList<String>();
      cmd.add("diff");
      cmd.add("-U");
      cmd.add("4611686018427387903");
      cmd.add(left.getName());
      cmd.add(right.getName());
      pb.command("bash", "-c", "diff -U 4611686018427387903 \"" + left.getName() + "\" \"" + right.getName() + "\"");
      result = getOutput(pb);
     }
     else if (leftobj != null && rightobj != null && leftobj.startsWith("en") && rightobj.startsWith("en"))
     {
      Environment left = getEnvironmentFromNameOrID(so, leftobj.substring(2));
      Environment right = getEnvironmentFromNameOrID(so, rightobj.substring(2));
      String leftyaml = diffForEnvironment(so, left);
      String rightyaml = diffForEnvironment(so, right);

      Path temppath = Files.createTempDirectory("dh-");

      File lfile = new File(temppath.toString() + "/" + left.getName());
      FileWriter fileWriter = new FileWriter(lfile);
      fileWriter.write(leftyaml);
      fileWriter.close();

      File rfile = new File(temppath.toString() + "/" + right.getName());
      fileWriter = new FileWriter(rfile);
      fileWriter.write(rightyaml);
      fileWriter.close();

      ProcessBuilder pb = new ProcessBuilder();
      Map<String, String> env = pb.environment();
      String pathenv = "/usr/local/bin:" + env.get("PATH");
      env.put("PATH", pathenv);

      System.out.println(temppath.toString());
      pb.directory(temppath.toFile());
      pb.redirectErrorStream(true);

      ArrayList<String> cmd = new ArrayList<String>();
      cmd.add("diff");
      cmd.add("-U");
      cmd.add("4611686018427387903");
      cmd.add(left.getName());
      cmd.add(right.getName());
      pb.command("bash", "-c", "diff -U 4611686018427387903 \"" + left.getName() + "\" \"" + right.getName() + "\"");
      result = getOutput(pb);
     }
     obj.add("result", result);
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
      e1.printStackTrace();
     }
    }
    e.printStackTrace();
    obj.add("success", false);
    obj.add("error", e.toString());
   }
   finally
   {
    out = response.getWriter();
    String ret = obj.getJSON();
    System.out.println(ret);
    out.println(ret);
   }
  }
 }

 private void assignDefect(DMSession so, String objid, HttpServletRequest request)
 {
  String datasource = request.getParameter("ds");
  String bugid = request.getParameter("bugid");
  String del = request.getParameter("del");

  if (del != null)
  {
   try (DMSessionPro session = new DMSessionPro(so))
   {
    ObjectTypeAndId x = new ObjectTypeAndId(objid);
    session.removeDefectFromComponent(x.getId(), "");
   }
   return;
  }

  Datasource ds = so.getDatasourceByName(datasource);

  if (ds != null)
  {
   try (DMSessionPro session = new DMSessionPro(so))
   {
    DefectList dflist = session.getDefectFromDatasource(ds, bugid);

    if (dflist != null)
    {
     List<Defect> defects = dflist.getDefects();

     if (defects.size() > 0)
     {
      Defect d = defects.get(0);
      String szBugId = d.getId();
      String szTitle = d.getTitle();
      String szURL = d.getHtmlUrl();
      String szAPIURL = d.getApiUrl();
      String status = d.getStatus();

      ObjectTypeAndId x = new ObjectTypeAndId(objid);

      if (x.getObjectType() == ObjectType.COMPONENT || x.getObjectType() == ObjectType.COMPVERSION)
      {
       Component comp = session.getComponent(x.getId(), false);
       session.addDefectToComponent(comp, szBugId, szTitle, szURL, szAPIURL, status);
      }
      else if (x.getObjectType() == ObjectType.APPLICATION || x.getObjectType() == ObjectType.APPVERSION)
      {
       Application app = session.getApplication(x.getId(), false);
       session.addDefectToApplication(app, szBugId, szTitle, szURL, szAPIURL, status);
      }
     }
    }
   }
  }
 }

 private String getOutput(ProcessBuilder pb)
 {
  String result = "";
  try
  {
   Process p = pb.start();
   final BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));

   StringJoiner sj = new StringJoiner(System.getProperty("line.separator"));
   reader.lines().iterator().forEachRemaining(sj::add);
   result = sj.toString();

   p.waitFor();
   p.destroy();
  }
  catch (Exception e)
  {
   e.printStackTrace();
  }
  return result;
 }

 private String diffForApplication(DMSession so, Application app)
 {
  String ret = "";

  ret += "Application: " + app.getName() + "\n";
  List<DMAttribute> kvs = app.getAttributes();
  kvs.sort(new Comparator<DMAttribute>()
  {
   @Override
   public int compare(DMAttribute m1, DMAttribute m2)
   {
    return ((m1.getName() + "=" + m1.getValue()).compareTo(m2.getName() + "=" + m2.getValue()));
   }
  });

  for (DMAttribute kv : kvs)
  {
   ret += "\t" + kv.getName() + ": " + kv.getValue() + "\n";
  }

  // List the application components
  List<Component> comps = so.getComponents(ObjectType.APPLICATION, app.getId(), false);

  comps.sort(new Comparator<Component>()
  {
   @Override
   public int compare(Component m1, Component m2)
   {
    return (m1.getName().compareTo(m2.getName()));
   }
  });

  for (Component c : comps)
  {
   if (!so.ValidDomain(c.getDomainId()))
    continue;

   ret += "Component: " + c.getFullName() + "\n";
   ret += "\tlastbuild: " + c.getLastBuildNumber() + "\n";

   List<ComponentItem> ciList = so.getComponentItems(c.getId());

   for (ComponentItem ci : ciList)
   {
    if (ci.getObjectType() == ObjectType.COMPONENTITEM)
    {
     ret += "\tBuild Id: " + getValue(ci.getBuildId()) + "\n";
     ret += "\tBuild URL: " + getValue(ci.getBuildUrl()) + "\n";
     ret += "\tBuild Date: " + getValue(ci.getBuildDate()) + "\n";
     ret += "\tHelm Chart: " + getValue(ci.getChart()) + "\n";
     ret += "\tHelm Chart Version: " + getValue(ci.getChartVersion()) + "\n";
     ret += "\tHelm Chart Namespace: " + getValue(ci.getChartNamespace()) + "\n";
     ret += "\tOperator: " + getValue(ci.getOperator()) + "\n";
     ret += "\tContainer Registry: " + getValue(ci.getDockerRepo()) + "\n";
     ret += "\tContainer Digest: " + getValue(ci.getDockerSha()) + "\n";
     ret += "\tContainer Tag: " + getValue(ci.getDockerTag()) + "\n";
     ret += "\tGit Commit: " + getValue(ci.getGitCommit()) + "\n";
     ret += "\tGit Repo: " + getValue(ci.getGitRepo()) + "\n";
     ret += "\tGit Tag: " + getValue(ci.getGitTag()) + "\n";
     ret += "\tGit URL: " + getValue(ci.getGitUrl()) + "\n";
    }
   }
   kvs = c.getAttributes();
   kvs.sort(new Comparator<DMAttribute>()
   {
    @Override
    public int compare(DMAttribute m1, DMAttribute m2)
    {
     return ((m1.getName() + "=" + m1.getValue()).compareTo(m2.getName() + "=" + m2.getValue()));
    }
   });

   for (DMAttribute kv : kvs)
   {
    ret += "\t" + kv.getName() + ": " + kv.getValue() + "\n";
   }
   ret += "\n";
  }

  // get sbom
  ret += so.getApplicationPackages(app.getId());
  // get CVEs
  ret += so.getApplicationCVEs(app.getId());

  return ret;
 }

 private String diffForEnvironment(DMSession so, Environment env)
 {
  String ret = "";

  ret += "Environment: " + env.getName() + "\n";
  List<DMAttribute> kvs = env.getAttributes();
  kvs.sort(new Comparator<DMAttribute>()
  {
   @Override
   public int compare(DMAttribute m1, DMAttribute m2)
   {
    return ((m1.getName() + "=" + m1.getValue()).compareTo(m2.getName() + "=" + m2.getValue()));
   }
  });

  for (DMAttribute kv : kvs)
  {
   if (kv.getName().equalsIgnoreCase("Config Component"))
   {
    String val = kv.getValue();
    String idstr = val;

    if (idstr.contains("id: 'cv"))
    {
     idstr = idstr.substring(idstr.indexOf("id: 'cv") + "id: 'cv".length());
     idstr = idstr.substring(0, idstr.indexOf("'"));
    }
    else if (idstr.contains("id: 'co"))
    {
     idstr = idstr.substring(idstr.indexOf("id: 'co") + "id: 'co".length());
     idstr = idstr.substring(0, idstr.indexOf("'"));
    }

    Component c = so.getComponent(new Integer(idstr).intValue(), true);
    ret += "\t" + kv.getName() + ": " + c.getFullName() + "\n";
    List<DMAttribute> config_kvs = c.getAttributes();
    config_kvs.sort(new Comparator<DMAttribute>()
    {
     @Override
     public int compare(DMAttribute m1, DMAttribute m2)
     {
      return ((m1.getName() + "=" + m1.getValue()).compareTo(m2.getName() + "=" + m2.getValue()));
     }
    });

    for (DMAttribute config_kv : config_kvs)
    {
     ret += "\t\t" + config_kv.getName() + ": " + config_kv.getValue() + "\n";
    }
    ret += "\n";
   }
   else if (kv.getName().equalsIgnoreCase("Last Deployed Application"))
   {
   }
   else
    ret += "\t" + kv.getName() + ": " + kv.getValue() + "\n";
  }

  List<Application> applist = so.getLastDeployedAppInEnv(env);

  Application app = null;
  int maxdeployid = 0;

  for (int p = 0; p < applist.size(); p++)
  {
   Application a = applist.get(p);

   if (!so.ValidDomain(a.getDomainId()))
    continue;

   Deployment deploy = so.getDeployment(a, env);

   if (deploy.getId() >= maxdeployid)
   {
    maxdeployid = deploy.getId();
    app = a;
   }
  }

  if (app != null)
  {
   ret += "Application: " + app.getFullName() + "\n";
   ret += "\tDeployment: " + maxdeployid  + "\n";
   kvs = app.getAttributes();
   kvs.sort(new Comparator<DMAttribute>()
   {
    @Override
    public int compare(DMAttribute m1, DMAttribute m2)
    {
     return ((m1.getName() + "=" + m1.getValue()).compareTo(m2.getName() + "=" + m2.getValue()));
    }
   });

   for (DMAttribute kv : kvs)
   {
    ret += "\t" + kv.getName() + ": " + kv.getValue() + "\n";
   }

   List<Component> comps = so.getComponents(ObjectType.APPLICATION, app.getId(), false);

   comps.sort(new Comparator<Component>()
   {
    @Override
    public int compare(Component m1, Component m2)
    {
     return (m1.getFullName().compareTo(m2.getFullName()));
    }
   });

   for (Component c : comps)
   {
    if (!so.ValidDomain(c.getDomainId()))
     continue;

    ret += "\tComponent: " + c.getFullName() + "\n";
    ret += "\t\tlastbuild: " + c.getLastBuildNumber() + "\n";

    List<ComponentItem> ciList = so.getComponentItems(c.getId());

    for (ComponentItem ci : ciList)
    {
     if (ci.getObjectType() == ObjectType.COMPONENTITEM)
     {
      ret += "\t\tBuild Id: " + getValue(ci.getBuildId()) + "\n";
      ret += "\t\tBuild URL: " + getValue(ci.getBuildUrl()) + "\n";
      ret += "\t\tBuild Date: " + getValue(ci.getBuildDate()) + "\n";
      ret += "\t\tHelm Chart: " + getValue(ci.getChart()) + "\n";
      ret += "\t\tHelm Chart Version: " + getValue(ci.getChartVersion()) + "\n";
      ret += "\t\tHelm Chart Namespace: " + getValue(ci.getChartNamespace()) + "\n";
      ret += "\t\tOperator: " + getValue(ci.getOperator()) + "\n";
      ret += "\t\tContainer Registry: " + getValue(ci.getDockerRepo()) + "\n";
      ret += "\t\tContainer Digest: " + getValue(ci.getDockerSha()) + "\n";
      ret += "\t\tContainer Tag: " + getValue(ci.getDockerTag()) + "\n";
      ret += "\t\tGit Commit: " + getValue(ci.getGitCommit()) + "\n";
      ret += "\t\tGit Repo: " + getValue(ci.getGitRepo()) + "\n";
      ret += "\t\tGit Tag: " + getValue(ci.getGitTag()) + "\n";
      ret += "\t\tGit URL: " + getValue(ci.getGitUrl()) + "\n";
     }
    }

    kvs = c.getAttributes();
    kvs.sort(new Comparator<DMAttribute>()
    {
     @Override
     public int compare(DMAttribute m1, DMAttribute m2)
     {
      return ((m1.getName() + "=" + m1.getValue()).compareTo(m2.getName() + "=" + m2.getValue()));
     }
    });

    for (DMAttribute kv : kvs)
    {
     ret += "\t\t" + kv.getName() + ": " + kv.getValue() + "\n";
    }
    ret += "\n";
   }
  }
  return ret;
 }

 private String diffForComponent(DMSession so, Component c)
 {
  String ret = "";

  ret += c.getFullName() + "\n";
  ret += "\tlastbuild: " + c.getLastBuildNumber() + "\n";

  List<ComponentItem> ciList = so.getComponentItems(c.getId());

  for (ComponentItem ci : ciList)
  {
   if (ci.getObjectType() == ObjectType.COMPONENTITEM)
   {
    ret += "\tBuild Id: " + getValue(ci.getBuildId()) + "\n";
    ret += "\tBuild URL: " + getValue(ci.getBuildUrl()) + "\n";
    ret += "\tBuild Date: " + getValue(ci.getBuildDate()) + "\n";
    ret += "\tHelm Chart: " + getValue(ci.getChart()) + "\n";
    ret += "\tHelm Chart Version: " + getValue(ci.getChartVersion()) + "\n";
    ret += "\tHelm Chart Namespace: " + getValue(ci.getChartNamespace()) + "\n";
    ret += "\tOperator: " + getValue(ci.getOperator()) + "\n";
    ret += "\tContainer Registry: " + getValue(ci.getDockerRepo()) + "\n";
    ret += "\tContainer Digest: " + getValue(ci.getDockerSha()) + "\n";
    ret += "\tContainer Tag: " + getValue(ci.getDockerTag()) + "\n";
    ret += "\tGit Commit: " + getValue(ci.getGitCommit()) + "\n";
    ret += "\tGit Repo: " + getValue(ci.getGitRepo()) + "\n";
    ret += "\tGit Tag: " + getValue(ci.getGitTag()) + "\n";
    ret += "\tGit URL: " + getValue(ci.getGitUrl()) + "\n";
   }
  }
  List<DMAttribute> kvs = c.getAttributes();
  kvs.sort(new Comparator<DMAttribute>()
  {
   @Override
   public int compare(DMAttribute m1, DMAttribute m2)
   {
    return ((m1.getName() + "=" + m1.getValue()).compareTo(m2.getName() + "=" + m2.getValue()));
   }
  });

  // get sbom
  ret += so.getComponentPackages(c.getId());
  // get CVEs
  ret += so.getComponentCVEs(c.getId());
  return ret;
 }

 private String getValue(String val)
 {
  return (val == null || val == "") ? "" : val;
 }

 private JSONObject assembleJSONForDefect(DMSession so, Defect defect)
 {
  JSONObject res = new JSONObject();
  res.add("id", defect.getId());
  res.add("title", defect.getTitle());
  res.add("apiurl", defect.getApiUrl());
  res.add("htmlurl", defect.getHtmlUrl());
  res.add("status", defect.getStatus());
  return res;
 }

 // else if (elements[0].equals("calendar"))
 // {
 // // Listing calendar entries
 // // API/calendar?[env=<env>&app=<app>][&start=<start>&end=<end>&all=Y&deploy=Y]
 // String envname = request.getParameter("env");
 // String appname = request.getParameter("app");
 // String starttext = request.getParameter("start");
 // String endtext = request.getParameter("end");
 // long starttime = 0;
 // long endtime = 0;
 // if (starttext != null)
 // {
 // starttime = convertDateTime(starttext);
 // }
 // else
 // {
 // starttime = so.timeNow();
 // }
 // if (endtext != null)
 // endtime = convertDateTime(endtext);
 // Environment env = null;
 // Application app = null;
 //
 // if (envname != null)
 // env = getEnvironmentFromNameOrID(so, envname);
 // if (appname != null)
 // app = getApplicationFromNameOrID(so, appname);
 // List<DMCalendarEvent> events = so.getEvents(env, app, starttime, endtime);
 // JSONArray es = new JSONArray();
 // for (DMCalendarEvent event : events)
 // {
 // JSONObject jo = new JSONObject();
 // jo.add("id", event.getID());
 // jo.add("title", event.getEventTitle());
 // jo.add("desc", event.getEventDesc());
 // jo.add("type", event.getEventTypeString());
 // jo.add("starttime", tsObject(event.getStart()));
 // jo.add("endtime", tsObject(event.getEnd()));
 // jo.add("allday", event.getAllDayEvent());
 // if (event.getApproved() > 0)
 // {
 // jo.add("approved", tsObject(event.getApproved()));
 // jo.add("approver", event.getApprover());
 // }
 // jo.add("creator", userObject(so, event.getCreatorID()));
 // jo.add("created", tsObject(event.getCreated()));
 // jo.add("deployid", event.getDeployID());
 // jo.add("pending", event.getPending());
 // int appid = event.getAppID();
 // int envid = event.getEnvID();
 // if (appid > 0)
 // {
 // Application eventapp = so.getApplication(appid, false);
 // JSONObject appjo = new JSONObject();
 // appjo.add("id", eventapp.getId());
 // appjo.add("name", eventapp.getName());
 // jo.add("application", appjo);
 // }
 // if (envid > 0)
 // {
 // Environment eventenv = so.getEnvironment(envid, false);
 // JSONObject envjo = new JSONObject();
 // envjo.add("id", eventenv.getId());
 // envjo.add("name", eventenv.getName());
 // jo.add("environment", envjo);
 // }
 // es.add(jo);
 // }
 // obj.add("result", es);
 // }

}
