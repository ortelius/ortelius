package dmadmin.pro;

import java.io.IOException;
import java.net.URLEncoder;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.sql.Types;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;
import dmadmin.model.Credential;
import dmadmin.model.Datasource;
import dmadmin.model.Deployment;
import dmadmin.model.Environment;
import dmadmin.model.ProviderDefinition;
import dmadmin.model.Task;
import dmadmin.model.TaskDeploy;
import dmadmin.model.Task.TaskType;

public class DMSessionPro extends dmadmin.DMSession
{
 // For background syncing of defects
 private Hashtable<Integer, int[]> m_pollhash = null;

 public DMSessionPro(DMSession session)
 {
  super();
  this.SetConnection(session.GetConnection());
  this.setUserId(session.GetUserID());
  this.m_domains = session.getDomainMap();
 }

 public DMSessionPro(ServletContext m_ServletContext)
 {
  super(m_ServletContext);
  this.SetConnection(super.GetConnection());
  this.setUserId(super.GetUserID());
 }

 public void syncDefects(ServletContext sc)
 {
  checkConnection(sc);

  if (m_pollhash == null)
  {
   m_pollhash = new Hashtable<Integer, int[]>();
  }
  else
  {
   //
   // Decrement the counter for each datasource
   //
   Enumeration<Integer> enumKey = m_pollhash.keys();
   while (enumKey.hasMoreElements())
   {
    int dsid = enumKey.nextElement();
    int[] pollvals = m_pollhash.get(dsid);
    if (pollvals[1] > 0)
    {
     pollvals[1] -= 1; // decrement count
    }
    m_pollhash.put(dsid, pollvals);
   }
  }

  String sql1 = "select d.bugid,d.apiurl,c.id,a.id,pd.name,ds.id,d.status " + "from dm.dm_defects d " + "left outer join dm.dm_component c on c.id = d.compid and c.status='N' " + "left outer join dm.dm_application a on a.id = d.appid and a.status='N' "
    + "left outer join dm.dm_datasource ds on ds.id = coalesce(d.datasourceid,c.datasourceid,a.datasourceid) " + "left outer join dm.dm_providerdef pd on pd.id = ds.defid " + "where ds.name is not null";
  String sql2 = "update dm.dm_defects set status=? where bugid=? and compid=?";
  String sql3 = "update dm.dm_defects set status=? where bugid=? and appid=?";
  boolean stateChanged = false;

  try
  {
   PreparedStatement st1 = m_conn.prepareStatement(sql1);
   PreparedStatement st2 = m_conn.prepareStatement(sql2);
   PreparedStatement st3 = m_conn.prepareStatement(sql3);
   ResultSet rs1 = st1.executeQuery();
   while (rs1.next())
   {
    String bugid = rs1.getString(1);
    String apiurl = rs1.getString(2);
    int compid = getInteger(rs1, 3, 0);
    int appid = getInteger(rs1, 4, 0);
    if (compid == 0 && appid == 0)
     continue; // no related application or component (poss deleted?)
    String provider = getString(rs1, 5, "");
    int datasourceid = getInteger(rs1, 6, 0);
    if (datasourceid == 0)
     continue; // no datasource
    Datasource ds = this.getDatasource(datasourceid, true);
    String currentStatus = getString(rs1, 7, "");
    Credential cred = ds.getCredential();
    String status = null;
    String pi = getDataSourceAttribute(ds.getId(), "Poll Interval");
    int pollInterval = 10; // default
    if (pi != null)
    {
     try
     {
      pollInterval = Integer.parseInt(pi);
     }
     catch (NumberFormatException ex)
     {
      pollInterval = 10;
     }
    }
    if (pollInterval == 0)
     continue; // poll interval = 0 means do not poll this datasource.
    int[] pollvals = null;
    pollvals = m_pollhash.get(ds.getId());
    if (pollvals == null)
    {
     pollvals = new int[2];
     pollvals[0] = pollInterval;
     pollvals[1] = 0;
     m_pollhash.put(ds.getId(), pollvals);
    }
    else
    {
     if (pollInterval != pollvals[0])
     {
      // Poll Interval has been changed
      pollvals[0] = pollInterval;
      pollvals[1] = 0; // force rescan
     }
    }
    if (pollvals[1] == 0)
    {
     // Time to poll this datasource
     String res = getJSONFromServer(apiurl, cred);
     if (res == null)
      continue; // nothing returned, probably connection failure.
     if (provider.equalsIgnoreCase("jira"))
     {
      JsonObject issueObject = new JsonParser().parse(res).getAsJsonObject();
      JsonArray issues = issueObject.getAsJsonArray("issues");
      if (issues == null)
       continue;
      JsonObject jsonIssue = issues.get(0).getAsJsonObject();
      JsonObject fields = jsonIssue.get("fields").getAsJsonObject();
      JsonObject statusObject = fields.get("status").getAsJsonObject();
      status = statusObject.get("name").getAsString();
     }
     else if (provider.equalsIgnoreCase("bugzilla"))
     {
      JsonObject issueObject = new JsonParser().parse(res).getAsJsonObject();
      JsonArray bugs = issueObject.getAsJsonArray("bugs");
      if (bugs == null)
       continue;
      JsonObject bugObject = bugs.get(0).getAsJsonObject();
      status = bugObject.get("status").getAsString();
     }
     else if (provider.equalsIgnoreCase("github"))
     {
      JsonObject issueObject = new JsonParser().parse(res).getAsJsonObject();
      if (issueObject != null && issueObject.get("state") != null)
       status = issueObject.get("state").getAsString();
     }
     if (!currentStatus.equalsIgnoreCase(status) && status != null)
     {
      // Status has changed
      System.out.println("syncdefects) compid=" + compid + " appid=" + appid + " datasource [" + ds.getName() + "] bugid [" + bugid + "] oldstatus [" + currentStatus + "] newstatus [" + status + "]");
      stateChanged = true;
      if (compid > 0)
      {
       Component comp = getComponent(compid, true);
       st2.setString(1, status);
       st2.setString(2, bugid);
       st2.setInt(3, compid);
       st2.execute();
       updateDefectHistory(comp);
      }
      else if (appid > 0)
      {
       Application app = getApplication(appid, true);
       st3.setString(1, status);
       st3.setString(2, bugid);
       st3.setInt(3, appid);
       st3.execute();
       updateDefectHistory(app);
      }
     }
    }
   }
   rs1.close();
   st1.close();
   st2.close();
   st3.close();
   if (stateChanged)
   {
    m_conn.commit();
   }
   // Now reset any counters that are on zero
   Enumeration<Integer> enumKey = m_pollhash.keys();
   while (enumKey.hasMoreElements())
   {
    int dsid = enumKey.nextElement();
    int[] pollvals = m_pollhash.get(dsid);
    if (pollvals[1] == 0)
    {
     pollvals[1] = pollvals[0]; // reset
     m_pollhash.put(dsid, pollvals);
    }
   }
  }
  catch (SQLException ex)
  {
   System.out.println("syncDefects throws SQL Exception: " + ex.getMessage());
  }
 }

 private DefectList getDefectFromBugzilla(Datasource ds, String bugid)
 {
  DefectList ret = new DefectList(this);
  ArrayList<Defect> defects = new ArrayList<Defect>();
  String ServerURL = getDataSourceAttribute(ds.getId(), "Server URL");
  String ProductName = getDataSourceAttribute(ds.getId(), "Product");
  String ComponentName = getDataSourceAttribute(ds.getId(), "Component");
  try
  {
   String pn = ProductName != null ? "product=" + URLEncoder.encode(ProductName, "UTF-8") + "&" : "";
   String cn = ComponentName != null ? "component=" + URLEncoder.encode(ComponentName, "UTF-8") + "&" : "";
   if (ServerURL == null)
    throw new Exception("No Server URL");
   String res = getJSONFromServer(ServerURL + "/rest/bug/" + bugid + pn + cn + "limit=500&resolution=---&include_fields=id,summary,status", ds.getCredential());
   JsonObject issueObject = new JsonParser().parse(res).getAsJsonObject();
   JsonArray issues = issueObject.getAsJsonArray("bugs");
   for (int i = 0; i < issues.size(); i++)
   {
    JsonObject jsonIssue = issues.get(i).getAsJsonObject();
    System.out.println("jsonIssue = " + jsonIssue);
    String title = jsonIssue.get("summary").getAsString();
    String status = jsonIssue.get("status").getAsString();
    String apiurl = ServerURL + "/rest/bug/" + bugid;
    String htmlurl = ServerURL + "/show_bug.cgi?id=" + bugid;
    Defect defect = new Defect(bugid, title, apiurl, htmlurl);
    defect.setStatus(status);
    defects.add(defect);
   }
   ret.setDefects(defects);
  }
  catch (Exception ex)
  {
   ret.setErrorMessage(ex.getMessage());
  }
  return ret;
 }

 private DefectList getOpenDefectsBugzilla(Datasource ds)
 {
  // Connects to Bugzilla and gets list of open defects (no resolution)
  DefectList ret = new DefectList(this);
  ArrayList<Defect> defects = new ArrayList<Defect>();
  String ServerURL = getDataSourceAttribute(ds.getId(), "Server URL");
  String ProductName = getDataSourceAttribute(ds.getId(), "Product");
  String ComponentName = getDataSourceAttribute(ds.getId(), "Component");
  try
  {
   String pn = ProductName != null ? "product=" + URLEncoder.encode(ProductName, "UTF-8") + "&" : "";
   String cn = ComponentName != null ? "component=" + URLEncoder.encode(ComponentName, "UTF-8") + "&" : "";
   if (ServerURL == null)
    throw new Exception("No Server URL");
   String res = getJSONFromServer(ServerURL + "/rest/bug?" + pn + cn + "limit=500&resolution=---&include_fields=id,summary,status", ds.getCredential());
   JsonObject issueObject = new JsonParser().parse(res).getAsJsonObject();
   JsonArray issues = issueObject.getAsJsonArray("bugs");
   for (int i = 0; i < issues.size(); i++)
   {
    JsonObject jsonIssue = issues.get(i).getAsJsonObject();
    System.out.println("jsonIssue = " + jsonIssue);
    String bugid = jsonIssue.get("id").getAsString();
    String title = jsonIssue.get("summary").getAsString();
    String status = jsonIssue.get("status").getAsString();
    String apiurl = ServerURL + "/rest/bug/" + bugid;
    String htmlurl = ServerURL + "/show_bug.cgi?id=" + bugid;
    Defect defect = new Defect(bugid, title, apiurl, htmlurl);
    defect.setStatus(status);
    defects.add(defect);
   }
   ret.setDefects(defects);
  }
  catch (Exception ex)
  {
   ret.setErrorMessage(ex.getMessage());
  }
  return ret;
 }

 private DefectList getDefectFromGitHub(Datasource ds, String bugid)
 {
  // Connects to github and gets specified bug

  // Datasource Repository = GitHub Orgranization
  // Datasource Product = GitHub Repository

  DefectList ret = new DefectList(this);
  String Organization = getDataSourceAttribute(ds.getId(), "Organization");
  String Repository = getDataSourceAttribute(ds.getId(), "Repository");
  try
  {
   if (Organization == null)
    throw new Exception("No Organization Attribute specified");
   if (Repository == null)
    throw new Exception("No Repository Attribute specified");
   System.out.println("https://api.github.com/repos/" + Organization + "/" + Repository + "/issues/" + bugid);
   String res = getJSONFromServer("https://api.github.com/repos/" + Organization + "/" + Repository + "/issues/" + bugid, ds.getCredential());
   System.out.println("res=" + res);
   ArrayList<Defect> defects = new ArrayList<Defect>();
   JsonObject jsonIssue = new JsonParser().parse(res).getAsJsonObject();
   if (jsonIssue.has("message"))
   {
    String message = jsonIssue.get("message").getAsString();
    System.out.println("message is " + message);
    ret.setErrorMessage(message);
   }
   else
   {
    String title = jsonIssue.get("title").getAsString();
    String htmlurl = jsonIssue.get("html_url").getAsString();
    String apiurl = jsonIssue.get("url").getAsString();
    String status = jsonIssue.get("state").getAsString();
    System.out.println("apiurl=" + apiurl + " status=" + status);
    Defect defect = new Defect(bugid, title, apiurl, htmlurl);
    defect.setStatus(status);
    defects.add(defect);
    System.out.println("defects.size is " + defects.size());
    ret.setDefects(defects);
   }
  }
  catch (Exception ex)
  {
   System.out.println("Caught exception ex=" + ex.getMessage());
   ret.setErrorMessage(ex.getMessage());
  }
  return ret;
 }

 private DefectList getOpenDefectsGitHub(Datasource ds)
 {
  // Connects to github and gets list of open defects

  // Datasource Repository = GitHub Orgranization
  // Datasource Product = GitHub Repository

  DefectList ret = new DefectList(this);
  String Organization = getDataSourceAttribute(ds.getId(), "Organization");
  String Repository = getDataSourceAttribute(ds.getId(), "Repository");
  try
  {
   if (Organization == null)
    throw new Exception("No Organization Attribute specified");
   if (Repository == null)
    throw new Exception("No Repository Attribute specified");
   ArrayList<Defect> defects = new ArrayList<Defect>();
   String res = getJSONFromServer("https://api.github.com/repos/" + Organization + "/" + Repository + "/issues", ds.getCredential());
   System.out.println("res=" + res);
   JsonArray issues = new JsonParser().parse(res).getAsJsonArray();
   System.out.println("Number of issues=" + issues.size());
   for (int i = 0; i < issues.size(); i++)
   {
    JsonObject jsonIssue = issues.get(i).getAsJsonObject();
    String bugid = jsonIssue.get("number").getAsString();
    String title = jsonIssue.get("title").getAsString();
    String htmlurl = jsonIssue.get("html_url").getAsString();
    String apiurl = jsonIssue.get("url").getAsString();
    String status = jsonIssue.get("state").getAsString();
    System.out.println("apiurl=" + apiurl + " status=" + status);
    Defect defect = new Defect(bugid, title, apiurl, htmlurl);
    defect.setStatus(status);
    defects.add(defect);
   }
   ret.setDefects(defects);
  }
  catch (Exception ex)
  {
   ret.setErrorMessage(ex.getMessage());
  }
  return ret;
 }

 DefectList getDefectFromJira(Datasource ds, String bugid)
 {
  DefectList ret = new DefectList(this);
  ArrayList<Defect> defects = new ArrayList<Defect>();
  String ServerURL = getDataSourceAttribute(ds.getId(), "Server URL");
  try
  {
   if (ServerURL == null)
    throw new Exception("No Server URL");
   String ProjectKey = getDataSourceAttribute(ds.getId(), "Project Key");
   // String Filter = getDataSourceAttribute(ds.getId(),"Filter");
   String url = "?fields=key,summary,status";
   if (ProjectKey != null)
    url = "?jql=project=" + ProjectKey + "&" + url.substring(1);
   String res = getJSONFromServer(ServerURL + "/rest/api/2/issue/" + bugid + url, ds.getCredential());
   System.out.println("res=" + res);
   JsonObject jsonIssue = new JsonParser().parse(res).getAsJsonObject();
   JsonObject fields = jsonIssue.get("fields").getAsJsonObject();
   String title = fields.get("summary").getAsString();
   JsonObject statusObject = fields.get("status").getAsJsonObject();
   String status = statusObject.get("name").getAsString();
   String apiurl = ServerURL + "/rest/api/2/search?jql=key=" + bugid;
   String htmlurl = ServerURL + "/browse/" + bugid;
   Defect defect = new Defect(bugid, title, apiurl, htmlurl);
   System.out.println("bugid=" + bugid + " status=" + status);
   if (status != null)
    defect.setStatus(status);
   defects.add(defect);
   ret.setDefects(defects);
  }
  catch (Exception ex)
  {
   ret.setErrorMessage(ex.getMessage());
  }
  return ret;
 }

 DefectList getOpenDefectsJira(Datasource ds)
 {
  // Connects to Jira and gets list of open defects
  // List is based on "Filter" which can be set to !Done (for example)
  // to select all issues with a status other than "Done" or "To Do" to
  // only select those issues with a status of "To Do"

  DefectList ret = new DefectList(this);
  ArrayList<Defect> defects = new ArrayList<Defect>();
  String ServerURL = getDataSourceAttribute(ds.getId(), "Server URL");
  try
  {
   if (ServerURL == null)
    throw new Exception("No Server URL");
   String ProjectKey = getDataSourceAttribute(ds.getId(), "Project Key");
   String Filter = getDataSourceAttribute(ds.getId(), "Filter");
   String url = "?fields=key,summary,status";
   if (ProjectKey != null)
    url = "?jql=project=" + ProjectKey + "&" + url.substring(1);
   String res = getJSONFromServer(ServerURL + "/rest/api/2/search" + url, ds.getCredential());
   System.out.println("res=" + res);
   JsonObject issueObject = new JsonParser().parse(res).getAsJsonObject();
   if (issueObject.has("errormsg"))
   {
    ret.setErrorMessage(issueObject.get("errormsg").getAsString());
   }
   else
   {
    JsonArray issues = issueObject.getAsJsonArray("issues");
    for (int i = 0; i < issues.size(); i++)
    {
     JsonObject jsonIssue = issues.get(i).getAsJsonObject();
     String bugid = jsonIssue.get("key").getAsString();
     JsonObject fields = jsonIssue.get("fields").getAsJsonObject();
     String title = fields.get("summary").getAsString();
     JsonObject statusObject = fields.get("status").getAsJsonObject();
     String status = statusObject.get("name").getAsString();
     String apiurl = ServerURL + "/rest/api/2/search?jql=key=" + bugid;
     String htmlurl = ServerURL + "/browse/" + bugid;
     Defect defect = new Defect(bugid, title, apiurl, htmlurl);
     System.out.println("bugid=" + bugid + " status=" + status);
     if (status != null)
      defect.setStatus(status);
     if (Filter != null)
     {
      if (Filter.startsWith("!"))
      {
       // Not
       if (!status.equalsIgnoreCase(Filter.substring(1)))
       {
        defects.add(defect);
       }
      }
      else
      {
       if (status.equalsIgnoreCase(Filter) || Filter.isEmpty())
       {
        defects.add(defect);
       }
      }
     }
     else
     {
      defects.add(defect);
     }
    }
    ret.setDefects(defects);
   }
  }
  catch (Exception ex)
  {
   ret.setErrorMessage(ex.getMessage());
  }
  return ret;
 }

 public DefectList getDefectsForComponent(int compid)
 {
  System.out.println("getDefectsForComponent(" + compid + ")");
  String sql = "SELECT d.bugid,d.title,d.apiurl,d.htmlurl,d.status,x.defid FROM dm.dm_defects d LEFT OUTER JOIN dm.dm_datasource x on x.id = d.datasourceid WHERE compid=? ORDER BY bugid";
  DefectList ret = new DefectList(this);
  ArrayList<Defect> defects = new ArrayList<Defect>();
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, compid);
   ResultSet rs = stmt.executeQuery();
   while (rs.next())
   {
    Defect d = new Defect(rs.getString(1), rs.getString(2), rs.getString(3), rs.getString(4));
    String status = getString(rs, 5, "Unknown");
    d.setStatus(status);
    System.out.println("bugid=" + rs.getString(1) + " defid=" + rs.getInt(6) + " status=" + status);
    d.setColour(getBugColour(rs.getInt(6), status));
    defects.add(d);
   }
   rs.close();
   stmt.close();
   ret.setDefects(defects);
  }
  catch (SQLException e)
  {
   ret.setErrorMessage(e.getMessage());
  }
  return ret;
 }

 public DefectList getOpenDefects(Datasource ds)
 {
  if (ds != null)
  {
   ProviderDefinition provider = ds.getDef();
   System.out.println("provider name = " + provider.getName());
   if (provider.getName().equalsIgnoreCase("github"))
   {
    return getOpenDefectsGitHub(ds);
   }
   else if (provider.getName().equalsIgnoreCase("bugzilla"))
   {
    return getOpenDefectsBugzilla(ds);
   }
   else if (provider.getName().equalsIgnoreCase("jira"))
   {
    return getOpenDefectsJira(ds);
   }
  }
  return null;
 }

 public DefectList getDefectFromDatasource(Datasource ds, String bugid)
 {
  if (ds != null)
  {
   ProviderDefinition provider = ds.getDef();
   System.out.println("provider name = " + provider.getName());
   if (provider.getName().equalsIgnoreCase("github"))
   {
    return getDefectFromGitHub(ds, bugid);
   }
   else if (provider.getName().equalsIgnoreCase("bugzilla"))
   {
    return getDefectFromBugzilla(ds, bugid);
   }
   else if (provider.getName().equalsIgnoreCase("jira"))
   {
    return getDefectFromJira(ds, bugid);
   }
  }
  return null;
 }

 public DefectList removeDefectFromComponent(int compid, String bugid)
 {
  if (bugid.isEmpty())
  {
   String sql = "DELETE FROM dm.dm_defects WHERE compid=?";
   try
   {
    PreparedStatement stmt = m_conn.prepareStatement(sql);
    stmt.setInt(1, compid);
    stmt.execute();
    m_conn.commit();
   }
   catch (Exception e)
   {
   }
   return new DefectList(this);
  }

  Component comp = getComponent(compid, true);
  String sql = "DELETE FROM dm.dm_defects WHERE compid=? AND bugid=?";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, compid);
   stmt.setString(2, bugid);
   stmt.execute();
   updateDefectHistory(comp);
   RecordObjectUpdateMultiple(comp, "Removed Change Request " + bugid);
   m_conn.commit();
  }
  catch (SQLException e)
  {
   DefectList ret = new DefectList(this);
   ret.setErrorMessage(e.getMessage());
   return ret;
  }
  return getDefectsForComponent(compid);
 }

 public DefectList removeDefectFromApplication(int appid, String bugid)
 {
  Application app = getApplication(appid, true);
  String sql = "DELETE FROM dm.dm_defects WHERE appid=? AND bugid=?";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, appid);
   stmt.setString(2, bugid);
   stmt.execute();
   updateDefectHistory(app);
   RecordObjectUpdateMultiple(app, "Removed Change Request " + bugid);
   m_conn.commit();
  }
  catch (SQLException e)
  {
   System.out.println(e.getMessage());
   e.printStackTrace();
   return null;
  }
  return getDefectsForApplication(appid);
 }

 private void updateDefectHistory(Component comp)
 {
  long now = timeNow();
  String sql1a = "SELECT count(*) FROM dm.dm_defecthistory WHERE compid=?";
  String sql1b = "SELECT count(*) FROM dm.dm_defecthistory WHERE compid=? AND timestamp=?";
  String sql2 = "INSERT INTO dm.dm_defecthistory(compid,timestamp,open,closed) VALUES(?,?,?,?)";
  String sql3 = "UPDATE dm.dm_defecthistory SET open=?,closed=? WHERE compid=? AND timestamp=?";
  String sql4 = "INSERT INTO dm.dm_defecthistory(compid,timestamp,open,closed) " + "SELECT id,created,0,0 FROM dm.dm_component WHERE id=?";
  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(sql1a);
   stmt1.setInt(1, comp.getId());
   ResultSet rs1 = stmt1.executeQuery();
   int c = 0;
   if (rs1.next())
   {
    c = rs1.getInt(1);
   }
   rs1.close();
   stmt1.close();
   if (c == 0)
   {
    // First Record for this component - insert a "starting" record related to
    // its creation time
    PreparedStatement stmt4 = m_conn.prepareStatement(sql4);
    stmt4.setInt(1, comp.getId());
    stmt4.execute();
    stmt4.close();
   }
   //
   // Now insert a record for this time with updated counts
   // First, get the counts to date
   //
   int[] counts = getDefectCountsForComponent(comp);
   // counts[0] = open
   // counts[1] = closed
   // If we add or remove multiple bugs then we're going to be called multiple times
   // Check if a record already exists for this timestamp and update it if so.
   PreparedStatement stmt1b = m_conn.prepareStatement(sql1b);
   stmt1b.setInt(1, comp.getId());
   stmt1b.setLong(2, now);
   ResultSet rs1b = stmt1b.executeQuery();
   c = 0;
   if (rs1b.next())
   {
    c = rs1b.getInt(1);
   }
   rs1b.close();
   stmt1b.close();
   if (c == 0)
   {
    // Nothing else with this timestamp - do an insert
    PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
    stmt2.setInt(1, comp.getId());
    stmt2.setLong(2, now);
    stmt2.setInt(3, counts[0]); // open
    stmt2.setInt(4, counts[1]); // closed
    stmt2.execute();
   }
   else
   {
    // There's already a record with this timestamp - update it
    PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
    stmt3.setInt(1, counts[0]); // open
    stmt3.setInt(2, counts[1]); // closed
    stmt3.setInt(3, comp.getId());
    stmt3.setLong(4, now);
    stmt3.execute();
   }
   m_conn.commit();
  }
  catch (SQLException ex)
  {
   System.out.println("Unable to updateDefectHistory for Component " + comp.getId());
   rollback();
  }
 }

 private void updateDefectHistory(Application app)
 {
  long now = timeNow();
  String sql1a = "SELECT count(*) FROM dm.dm_defecthistory WHERE appid=?";
  String sql1b = "SELECT count(*) FROM dm.dm_defecthistory WHERE appid=? AND timestamp=?";
  String sql2 = "INSERT INTO dm.dm_defecthistory(appid,timestamp,open,closed) VALUES(?,?,?,?)";
  String sql3 = "UPDATE dm.dm_defecthistory SET open=?,closed=? WHERE appid=? AND timestamp=?";
  String sql4 = "INSERT INTO dm.dm_defecthistory(appid,timestamp,open,closed) " + "SELECT id,created,0,0 FROM dm.dm_application WHERE id=?";
  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(sql1a);
   stmt1.setInt(1, app.getId());
   ResultSet rs1 = stmt1.executeQuery();
   int c = 0;
   if (rs1.next())
   {
    c = rs1.getInt(1);
   }
   rs1.close();
   stmt1.close();
   if (c == 0)
   {
    // First Record for this application - insert a "starting" record related to
    // its creation time
    PreparedStatement stmt4 = m_conn.prepareStatement(sql4);
    stmt4.setInt(1, app.getId());
    stmt4.execute();
    stmt4.close();
   }
   //
   // Now insert a record for this time with updated counts
   // First, get the counts to date
   //
   int[] counts = getDefectCountsForApplication(app);
   // counts[0] = open
   // counts[1] = closed
   PreparedStatement stmt1b = m_conn.prepareStatement(sql1b);
   stmt1b.setInt(1, app.getId());
   stmt1b.setLong(2, now);
   ResultSet rs1b = stmt1b.executeQuery();
   c = 0;
   if (rs1b.next())
   {
    c = rs1b.getInt(1);
   }
   rs1b.close();
   stmt1b.close();
   if (c == 0)
   {
    PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
    stmt2.setInt(1, app.getId());
    stmt2.setLong(2, timeNow());
    stmt2.setInt(3, counts[0]); // open
    stmt2.setInt(4, counts[1]); // closed
    stmt2.execute();
   }
   else
   {
    // There's already a record with this timestamp - update it
    PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
    stmt3.setInt(1, counts[0]); // open
    stmt3.setInt(2, counts[1]); // closed
    stmt3.setInt(3, app.getId());
    stmt3.setLong(4, now);
    stmt3.execute();
   }
   m_conn.commit();
  }
  catch (SQLException ex)
  {
   System.out.println("Unable to updateDefectHistory for Application " + app.getId() + ": " + ex.getMessage());
   ex.printStackTrace();
   rollback();
  }
 }

 public DefectList addDefectToComponent(Component comp, Defect defect)
 {
  return addDefectToComponent(comp, defect.getId(), defect.getTitle(), defect.getHtmlUrl(), defect.getApiUrl(), defect.getStatus());
 }

 public DefectList addDefectToComponent(Component comp, String bugid, String title, String url, String apiurl, String status)
 {
  System.out.println("Adding defect to Component " + comp.getId() + " apiurl=" + apiurl);
  String sql1 = "SELECT count(*) FROM dm.dm_defects WHERE compid=? AND bugid=?";
  String sql2 = "INSERT INTO dm.dm_defects(compid,bugid,title,htmlurl,apiurl,status,datasourceid) VALUES(?,?,?,?,?,?,?)";
  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(sql1);
   PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
   stmt1.setInt(1, comp.getId());
   stmt1.setString(2, bugid);
   stmt2.setInt(1, comp.getId());
   stmt2.setString(2, bugid);
   stmt2.setString(3, title);
   stmt2.setString(4, url);
   stmt2.setString(5, apiurl);
   stmt2.setString(6, status);
   stmt2.setInt(7, comp.getDatasource().getId());
   ResultSet rs1 = stmt1.executeQuery();
   if (rs1.next())
   {
    if (rs1.getInt(1) == 0)
    {
     // Bug not already associated - good to go
     stmt2.execute();
     RecordObjectUpdateMultiple(comp, "Added Change Request " + bugid);
     updateDefectHistory(comp);
     m_conn.commit();
    }
   }
   rs1.close();
   stmt2.close();
   stmt1.close();
  }
  catch (SQLException e)
  {
   DefectList ret = new DefectList(this);
   ret.setErrorMessage(e.getMessage());
   return ret;
  }
  return getDefectsForComponent(comp.getId());
 }

 public DefectList addDefectToApplication(Application app, String bugid, String title, String url, String apiurl, String status)
 {
  System.out.println("Adding defect to Application " + app.getId());
  String sql1 = "SELECT count(*) FROM dm.dm_defects WHERE appid=? AND bugid=?";
  String sql2 = "select count(*) from dm.dm_defects WHERE compid in " + "(select compid from dm.dm_applicationcomponent where appid=?) " + "and bugid=?";
  String sql3 = "INSERT INTO dm.dm_defects(appid,bugid,title,htmlurl,apiurl,status,datasourceid) VALUES(?,?,?,?,?,?,?)";
  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(sql1);
   PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
   PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
   stmt1.setInt(1, app.getId());
   stmt1.setString(2, bugid);
   stmt2.setInt(1, app.getId());
   stmt2.setString(2, bugid);
   stmt3.setInt(1, app.getId());
   stmt3.setString(2, bugid);
   stmt3.setString(3, title);
   stmt3.setString(4, url);
   stmt3.setString(5, apiurl);
   stmt3.setString(6, status);
   stmt3.setInt(7, app.getDatasource().getId());
   ResultSet rs1 = stmt1.executeQuery();
   if (rs1.next())
   {
    if (rs1.getInt(1) == 0)
    {
     // Bug is not associated with the app - what about any of the components?
     ResultSet rs2 = stmt2.executeQuery();
     if (rs2.next())
     {
      if (rs2.getInt(1) == 0)
      {
       // Bug is not associated with any associated component either
       stmt3.execute();
       RecordObjectUpdateMultiple(app, "Added Change Request " + bugid);
       updateDefectHistory(app);
       m_conn.commit();
      }
     }
     rs2.close();
    }
   }
   rs1.close();
   stmt3.close();
   stmt2.close();
   stmt1.close();
  }
  catch (SQLException e)
  {
   System.out.println(e.getMessage());
   e.printStackTrace();
   return null;
  }
  return getDefectsForApplication(app.getId());
 }

 public DefectList getDefectsForApplication(int appid)
 {
  System.out.println("getDefectsForApplication(" + appid + ")");
  Application app = getApplication(appid, false);
  String sql = null;
  boolean isRelease = (app.getIsRelease().equalsIgnoreCase("y"));
  if (isRelease)
  {
   sql = "select distinct b.bugid,b.title,b.apiurl,b.htmlurl,b.compid,a.id,b.status,x.defid " + "from  dm.dm_application a " + "left outer join dm.dm_applicationcomponent c on c.appid=a.id " + "left outer join dm.dm_defects b on (b.compid=c.compid or b.appid=a.id) "
     + "left outer join dm.dm_datasource x on x.id = b.datasourceid " + "where  a.id in " + "(SELECT objfrom FROM dm.dm_applicationcomponentflows where objfrom is not null and appid=? " + "UNION " + "SELECT objto FROM dm.dm_applicationcomponentflows where objto is not null and appid=?) "
     + "AND b.bugid IS NOT NULL " + "order by a.id,b.bugid";
  }
  else
  {
   sql = "SELECT d.bugid,d.title,d.apiurl,d.htmlurl,d.compid,0,d.status,x.defid FROM dm.dm_defects d " + "left outer join dm.dm_datasource x on x.id = d.datasourceid " + "WHERE d.compid IN " + "(SELECT compid FROM dm.dm_applicationcomponent WHERE appid=?) " + "UNION "
     + "SELECT e.bugid,e.title,e.apiurl,e.htmlurl,e.compid,0,e.status,z.defid " + "FROM dm.dm_defects e,dm.dm_datasource z WHERE e.appid=? AND z.id=e.datasourceid " + "ORDER BY 1";
  }
  DefectList ret = new DefectList(this);
  ArrayList<Defect> defects = new ArrayList<Defect>();
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, appid);
   stmt.setInt(2, appid);
   ResultSet rs = stmt.executeQuery();
   while (rs.next())
   {
    Defect d = new Defect(rs.getString(1), rs.getString(2), rs.getString(3), rs.getString(4));
    d.setCompId(getInteger(rs, 5, 0));
    if (isRelease)
     d.setAppId(getInteger(rs, 6, 0));
    d.setStatus(rs.getString(7));
    d.setColour(getBugColour(rs.getInt(8), rs.getString(7)));
    defects.add(d);
   }
   rs.close();
   stmt.close();
   ret.setDefects(defects);
  }
  catch (SQLException e)
  {
   ret.setErrorMessage(e.getMessage());
  }
  return ret;
 }

 public DefectList getDefectsJustForApplication(int appid)
 {
  System.out.println("getDefectsJustForApplication(" + appid + ")");
  // Application app = getApplication(appid,false);
  String sql = null;

  sql = "SELECT e.bugid,e.title,e.apiurl,e.htmlurl,e.compid,0,e.status,z.defid " + "FROM dm.dm_defects e,dm.dm_datasource z WHERE e.appid=? AND z.id=e.datasourceid " + "ORDER BY 1";

  DefectList ret = new DefectList(this);
  ArrayList<Defect> defects = new ArrayList<Defect>();
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, appid);
   ResultSet rs = stmt.executeQuery();
   while (rs.next())
   {
    Defect d = new Defect(rs.getString(1), rs.getString(2), rs.getString(3), rs.getString(4));
    d.setCompId(getInteger(rs, 5, 0));

    d.setStatus(rs.getString(7));
    d.setColour(getBugColour(rs.getInt(8), rs.getString(7)));
    defects.add(d);
   }
   rs.close();
   stmt.close();
   ret.setDefects(defects);
  }
  catch (SQLException e)
  {
   ret.setErrorMessage(e.getMessage());
  }
  ret.setDefects(defects);
  return ret;
 }

 private String getBugColour(int defid, String status)
 {
  if (status == "Unknown")
   return "red";
  String sql = "SELECT colour FROM dm.dm_bugtrackingstates WHERE defid=? AND lower(status)=?";
  String res = null;
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, defid);
   stmt.setString(2, status.toLowerCase());
   ResultSet rs = stmt.executeQuery();
   if (rs.next())
   {
    res = rs.getString(1);
   }
   rs.close();
   stmt.close();
  }
  catch (SQLException ex)
  {
   System.out.println("Failed to get colour for status " + status);
  }
  System.out.println("getBugColour(" + defid + "," + status + ") returns " + res);
  return res;
 }

 public DefectList getDefectsForEnvironment(int envid)
 {
  System.out.println("getDefectsForEnvironment(" + envid + ")");
  String sql1 = "  SELECT a.bugid,a.title,a.apiurl,a.htmlurl,b.appid,c.deploymentid,c.modifierid,coalesce(d.finished,c.modified),a.compid,a.status,x.defid" + "  FROM dm.dm_defects   a," + "   dm.dm_applicationcomponent b," + "  dm.dm_datasource  x," + "   dm.dm_appsinenv   c"
    + "  LEFT OUTER JOIN dm.dm_deployment d on d.deploymentid=c.deploymentid" + "  WHERE a.compid = b.compid" + "  AND c.envid = ?" + "  AND b.appid = c.appid" + "  AND x.id = a.datasourceid" + "  UNION"
    + "  SELECT d.bugid,d.title,d.apiurl,d.htmlurl,e.appid,e.deploymentid,e.modifierid,coalesce(f.finished,e.modified),null,d.status,y.defid" + "  FROM dm.dm_defects   d," + "   dm.dm_datasource    y," + "    dm_appsinenv   e" + "  LEFT OUTER JOIN dm.dm_deployment f on f.deploymentid=e.deploymentid"
    + "  WHERE d.appid = e.appid" + "  AND e.envid = ?" + "  AND y.id = d.datasourceid" + "  ORDER BY 6 desc,5,1";

  ArrayList<Defect> defects = new ArrayList<Defect>();
  DefectList ret = new DefectList(this);
  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(sql1);
   stmt1.setInt(1, envid);
   stmt1.setInt(2, envid);
   ResultSet rs1 = stmt1.executeQuery();
   while (rs1.next())
   {
    Defect d = new Defect(rs1.getString(1), rs1.getString(2), rs1.getString(3), rs1.getString(4));
    d.setAppId(getInteger(rs1, 5, 0));
    d.setDeploymentId(getInteger(rs1, 6, 0));
    d.setModifierId(getInteger(rs1, 7, 0));
    d.setModified(getLong(rs1, 8, 0));
    d.setCompId(getInteger(rs1, 9, 0));
    d.setStatus(rs1.getString(10));
    d.setColour(getBugColour(rs1.getInt(11), rs1.getString(10)));
    defects.add(d);
   }
   rs1.close();
   stmt1.close();
   ret.setDefects(defects);
  }
  catch (SQLException e)
  {
   ret.setErrorMessage(e.getMessage());
  }
  return ret;
 }

 // public void AddBuildToComponent(Component comp,int buildno,String [] issues,String [] files,String commit,boolean success)
 // {
 // String sql1="SELECT count(*) FROM dm.dm_buildhistory WHERE compid=? AND buildnumber=?";
 // String sql2="UPDATE dm.dm_buildhistory SET buildjobid=?, commit=?, userid=?, timestamp=?, success=? WHERE compid=? AND buildnumber=?";
 // String sql3="INSERT INTO dm.dm_buildhistory(buildjobid,buildnumber,commit,compid,userid,timestamp,success) VALUES(?,?,?,?,?,?,?)";
 // String sql4="UPDATE dm.dm_component SET lastbuildnumber=? WHERE id=?";
 // String sql5="DELETE FROM dm.dm_buildfiles WHERE buildjobid=? AND buildnumber=?";
 // String sql6="DELETE FROM dm.dm_builddefects WHERE buildjobid=? AND buildnumber=?";
 // String sql7="INSERT INTO dm.dm_buildfiles(buildjobid,buildnumber,filename) VALUES(?,?,?)";
 // String sql8="INSERT INTO dm.dm_builddefects(buildjobid,buildnumber,bugid,title,htmlurl) VALUES(?,?,?,?,?)";
 // BuildJob buildjob = comp.getBuildJob();
 // if (buildjob == null) return; // Already checked for this in the API code - belt and braces
 // try {
 // PreparedStatement stmt1 = m_conn.prepareStatement(sql1);
 // stmt1.setInt(1,comp.getId());
 // stmt1.setInt(2,buildno);
 // ResultSet rs1 = stmt1.executeQuery();
 // if (rs1.next()) {
 // if (rs1.getInt(1)==0) {
 // // First time for this build number against this component
 // PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
 // stmt3.setInt(1,buildjob.getId());
 // stmt3.setInt(2,buildno);
 // if (commit == null) {
 // stmt3.setNull(3,Types.CHAR);
 // } else {
 // stmt3.setString(3,commit);
 // }
 // stmt3.setInt(4,comp.getId());
 // stmt3.setInt(5,m_userID);
 // stmt3.setLong(6,timeNow());
 // stmt3.setString(7,success?"Y":"N");
 // stmt3.execute();
 // stmt3.close();
 // } else {
 // // This build number has already been associated with this component
 // PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
 // stmt2.setInt(1,buildjob.getId());
 // stmt2.setString(2,commit);
 // stmt2.setInt(3,m_userID);
 // stmt2.setLong(4,timeNow());
 // stmt2.setString(5,success?"Y":"N");
 // stmt2.setInt(6,comp.getId());
 // stmt2.setInt(7,buildno);
 // stmt2.execute();
 // stmt2.close();
 // }
 // // Add the defects and files for this build
 // PreparedStatement stmt5 = m_conn.prepareStatement(sql5);
 // PreparedStatement stmt6 = m_conn.prepareStatement(sql6);
 // PreparedStatement stmt7 = m_conn.prepareStatement(sql7);
 // PreparedStatement stmt8 = m_conn.prepareStatement(sql8);
 // stmt5.setInt(1,buildjob.getId());
 // stmt5.setInt(2,buildno);
 // stmt5.execute();
 // stmt6.setInt(1,buildjob.getId());
 // stmt6.setInt(2,buildno);
 // stmt6.execute();
 // if (files != null)
 // {
 // stmt7.setInt(1,buildjob.getId());
 // stmt7.setInt(2,buildno);
 // for (int i=0;i<files.length;i++) {
 // stmt7.setString(3,files[i]);
 // stmt7.execute();
 // }
 // }
 // if (issues != null)
 // {
 // stmt8.setInt(1,buildjob.getId());
 // stmt8.setInt(2,buildno);
 // for (int i=0;i<issues.length;i++) {
 // DefectList defects = getDefectFromDatasource(comp.getDatasource(),issues[i]);
 // if (defects.getStatus()) {
 // // Bug was successfully retrieved
 // List<Defect> dlist = defects.getDefects();
 // Defect defect = dlist.get(0); // only a single defect
 // stmt8.setString(3,issues[i]);
 // stmt8.setString(4,defect.getTitle());
 // stmt8.setString(5,defect.getHtmlUrl());
 // stmt8.execute();
 // addDefectToComponent(comp,defect);
 // }
 // }
 // }
 // if (success) {
 // System.out.println("Updating lastbuildnumber to "+buildno+" for component "+comp.getId());
 // PreparedStatement stmt4 = m_conn.prepareStatement(sql4);
 // stmt4.setInt(1,buildno);
 // stmt4.setInt(2,comp.getId());
 // stmt4.execute();
 // stmt4.close();
 // }
 // m_conn.commit();
 // }
 // rs1.close();
 // stmt1.close();
 // } catch(SQLException e) {
 // e.printStackTrace();
 // rollback();
 // }
 // }

 public List<int[]> getDefectHistory(Component comp)
 {
  System.out.println("getDefectHistory for comp " + comp.getId());
  String sql1 = "select timestamp - (timestamp % 10800),open,closed from dm.dm_defecthistory where compid=? order by timestamp";
  List<int[]> ret = new ArrayList<int[]>();
  LinkedHashMap<Integer, int[]> timehash = new LinkedHashMap<Integer, int[]>();
  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(sql1);
   stmt1.setInt(1, comp.getId());
   ResultSet rs1 = stmt1.executeQuery();
   while (rs1.next())
   {
    int[] vals = new int[3];
    vals[0] = rs1.getInt(1);
    vals[1] = rs1.getInt(2);
    vals[2] = rs1.getInt(3);
    timehash.put(vals[0], vals);
   }
   rs1.close();
   stmt1.close();
   for (Map.Entry<Integer, int[]> entry : timehash.entrySet())
   {
    int timestamp = entry.getKey();
    System.out.println("adding timestamp " + timestamp);
    int[] vals = entry.getValue();
    ret.add(vals);
   }
  }
  catch (SQLException ex)
  {
   System.out.println("Could not get defect history for component " + comp.getName());
   System.out.println(ex.getMessage());
   ex.printStackTrace();
  }
  System.out.println("Returning list of size " + ret.size());
  return ret;
 }

 public List<int[]> getDefectHistory(Application app)
 {
  // getting the defect history for an application requires a bit of clever calculation
  // We essentially need to maintain a running total of the defects associated with each
  // constituent component PLUS the defects associated with the app. We have to derive
  // this last figure because the entry in dm_defecthistory for applications is the total
  // for the app at that point (including all consituent components)
  //
  boolean isRelease = app.getIsRelease().equalsIgnoreCase("y");
  System.out.println("in get defect history for app " + app.getId());
  System.out.println("release is " + isRelease);
  // Create a hash table to hold the running totals for each component
  Hashtable<Integer, int[]> comptotals = new Hashtable<Integer, int[]>();
  Hashtable<Integer, int[]> apptotals = new Hashtable<Integer, int[]>();
  LinkedHashMap<Integer, int[]> timehash = new LinkedHashMap<Integer, int[]>();
  String sql1 = "select timestamp - (timestamp % 10800),appid,compid,open,closed from dm.dm_defecthistory  " + "where appid=? or compid in (select compid from dm.dm_applicationcomponent where appid=?) " + "order by timestamp";

  String sql2 = "select timestamp - (timestamp % 10800),appid,compid,open,closed from dm.dm_defecthistory  " + "where appid in (" + " select objfrom from dm.dm_applicationcomponentflows where appid=? " + " union " + " select objto from dm.dm_applicationcomponentflows where appid=? "
    + ") or compid in ( " + "select compid from dm.dm_applicationcomponent where appid in (" + " select objfrom from dm.dm_applicationcomponentflows where appid=? " + " union " + " select objto from dm.dm_applicationcomponentflows where appid=? " + ")) " + "order by timestamp";
  String sql3 = "select compid from dm.dm_applicationcomponent where appid=?";

  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(isRelease ? sql2 : sql1);
   PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
   stmt1.setInt(1, app.getId());
   stmt1.setInt(2, app.getId());
   if (isRelease)
   {
    stmt1.setInt(3, app.getId());
    stmt1.setInt(4, app.getId());
   }
   ResultSet rs1 = stmt1.executeQuery();
   int totalOpen = 0;
   int totalClosed = 0;
   int o = 0;
   int c = 0;
   int timestamp = 0;
   while (rs1.next())
   {
    timestamp = rs1.getInt(1);
    int appid = getInteger(rs1, 2, 0);
    int compid = getInteger(rs1, 3, 0);
    int open = rs1.getInt(4);
    int closed = rs1.getInt(5);
    if (open == 0 && closed == 0 && comptotals.isEmpty())
     continue; // Ignore the first 0/0 entries (creation times)
    System.out.println("ts=" + timestamp + " appid=" + appid + " compid=" + compid + " open=" + open + " closed=" + closed);
    if (compid > 0 || isRelease)
    {
     // Component Record or Component/App Record for Release
     if (appid > 0)
     {
      // This is an app (we must be in a release)
      // To find the numbers for this app, we have to deduct the current open/closed values
      // for any components associated with this application
      //
      stmt3.setInt(1, appid);
      ResultSet rs3 = stmt3.executeQuery();
      while (rs3.next())
      {
       // For each component...
       int ccid = rs3.getInt(1);
       if (comptotals.containsKey(ccid))
       {
        // Component exists - deduct its totals from the parent app
        int[] cts = comptotals.get(ccid);
        open -= cts[0];
        closed -= cts[1];
       }
      }
      rs3.close();
      totalOpen = 0;
      totalClosed = 0;
      int[] ts = new int[2];
      ts[0] = open;
      ts[1] = closed;
      apptotals.put(appid, ts);
     }
     else
     {
      // Component
      int[] cts = new int[2];
      cts[0] = open;
      cts[1] = closed;
      comptotals.put(compid, cts);
      totalOpen = 0;
      totalClosed = 0;
     }
     // Now calculate total for all components to date
     Enumeration<Integer> enumKey = comptotals.keys();
     while (enumKey.hasMoreElements())
     {
      int cid = enumKey.nextElement();
      int[] ts = comptotals.get(cid);
      totalOpen += ts[0];
      totalClosed += ts[1];
     }
     // And all applications
     enumKey = apptotals.keys();
     while (enumKey.hasMoreElements())
     {
      int aid = enumKey.nextElement();
      int[] ts = apptotals.get(aid);
      totalOpen += ts[0];
      totalClosed += ts[1];
     }
     o = totalOpen;
     c = totalClosed;
    }
    else if (appid > 0)
    {
     // Application Record. Note, the open and closed is accurate at the time of the entry being made
     // since the calculation takes into account the defects associated with the app AND the components
     // at the point where the record is inserted. But we need to work out from this figure the number
     // of defects associated with the app
     int[] ts = new int[2];
     ts[0] = open - totalOpen;
     ts[1] = closed - totalClosed;
     apptotals.put(appid, ts);
     // For an app, the value stored in defecthistory is accurate
     o = open;
     c = closed;
    }
    int[] vals = new int[3];
    vals[0] = timestamp;
    vals[1] = o;
    vals[2] = c;
    timehash.put(timestamp, vals);
   }
   rs1.close();
   stmt1.close();
   stmt3.close();
  }
  catch (SQLException ex)
  {
   System.out.println("Could not get defect history for application " + app.getName());
   System.out.println(ex.getMessage());
   ex.printStackTrace();
  }
  // Finally, convert the timehash into an array and return it.
  List<int[]> ret = new ArrayList<int[]>();
  for (Map.Entry<Integer, int[]> entry : timehash.entrySet())
  {
   int timestamp = entry.getKey();
   System.out.println("adding timestamp " + timestamp);
   int[] vals = entry.getValue();
   ret.add(vals);
  }
  return ret;
 }

 public void AddRequestEntry(int id, int appid, String Description, boolean cal)
 {
  String sql;
  if (cal)
  {
   sql = "INSERT INTO dm.dm_request(id,userid," + getWhenCol() + ",note,calendarid,appid,status) VALUES(?,?,?,?,?,?,'N')";
  }
  else
  {
   sql = "INSERT INTO dm.dm_request(id,userid," + getWhenCol() + ",note,taskid,appid,status) VALUES(?,?,?,?,?,?,'N')";
  }
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, getID("request"));
   stmt.setInt(2, this.GetUserID());
   stmt.setLong(3, timeNow());
   stmt.setString(4, Description);
   stmt.setInt(5, id);
   stmt.setInt(6, appid);
   stmt.execute();
   m_conn.commit();
   stmt.close();
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
 }

 public void AddPendingRequest(int id, int appid, String Description)
 {
  AddRequestEntry(id, appid, Description, true);
 }

 public int AddEvent(DMCalendarEvent e)
 {
  //
  // TO DO : Check Permissions
  // TO DO: If insert is "P"ending then also put an entry into the requests table.
  String sql = "INSERT INTO dm.dm_calendar(id,envid,eventname,eventtype,starttime,endtime,allday,status,appid,description,creatorid,created,modifierid,modified) " + "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
  try
  {
   boolean envowner = userOwnsEnvironment(e.getEnvID());
   System.out.println("In AddEvent, envowner=" + envowner);
   String status = envowner ? "N" : "P"; // Normal (we are the environment owner) or Pending
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   int id = getID("calendar");
   long t = timeNow();
   String EventType = e.getEventTypeString().toString();
   EventType = Character.toUpperCase(EventType.charAt(0)) + EventType.substring(1).toLowerCase();
   System.out.println("EventType=" + EventType);
   stmt.setInt(1, id);
   stmt.setInt(2, e.getEnvID());
   stmt.setString(3, e.getEventTitle());
   stmt.setString(4, EventType);
   stmt.setInt(5, e.getStart());
   stmt.setInt(6, e.getEnd());
   stmt.setString(7, e.getAllDayEvent() ? "Y" : "N");
   stmt.setString(8, status);
   int appid = e.getAppID();
   if (appid != 0)
   {
    stmt.setInt(9, appid);
   }
   else
   {
    stmt.setNull(9, Types.INTEGER);
   }
   stmt.setString(10, e.getEventDesc());
   System.out.println("getEventDesc=" + e.getEventDesc());
   stmt.setInt(11, e.getCreatorID());
   stmt.setLong(12, t);
   stmt.setInt(13, e.getCreatorID());
   stmt.setLong(14, t);
   boolean res = stmt.execute();
   System.out.println("res=" + (res ? "true" : "false"));
   stmt.close();
   Environment env = getEnvironment(e.getEnvID(), false);
   RecordObjectUpdate(env, "Calendar Entry <a href='javascript:SwitchToCalendar(" + id + ");'>\"" + e.getEventTitle() + (status.equalsIgnoreCase("p") ? "\"</a> Requested" : "\"</a> Created"));
   m_conn.commit();
   return id;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  return 0;
 }

 public int DeleteEvent(int eventid)
 {
  //
  // TO DO: Check Permissions
  //
  String sql1 = "UPDATE dm.dm_calendar SET status='D' WHERE id=?";
  String sql2 = "UPDATE dm.dm_request SET status='C', completed=? where calendarid=?";
  String sql3 = "SELECT envid,eventname FROM dm.dm_calendar WHERE id=?";
  try
  {
   int res = 0;
   PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
   stmt3.setInt(1, eventid);
   ResultSet rs3 = stmt3.executeQuery();
   if (rs3.next())
   {
    Environment env = getEnvironment(rs3.getInt(1), false);
    PreparedStatement stmt1 = m_conn.prepareStatement(sql1);
    stmt1.setInt(1, eventid);
    stmt1.execute();
    res = stmt1.getUpdateCount();
    stmt1.close();
    PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
    stmt2.setLong(1, timeNow());
    stmt2.setInt(2, eventid);
    stmt2.execute();
    RecordObjectUpdate(env, "Event \"" + rs3.getString(2) + "\" Deleted");
    m_conn.commit();
   }
   return res;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  return -1;
 }

 private DMEventType getEventType(String et)
 {
  if (et.equalsIgnoreCase("reserved"))
   return DMEventType.RESERVED;
  if (et.equalsIgnoreCase("unavailable"))
   return DMEventType.UNAVAILABLE;
  if (et.equalsIgnoreCase("auto"))
   return DMEventType.AUTO;
  System.out.println("Returning NoEvent");
  return DMEventType.NOEVENT;
 }

 public DMCalendarEvent getCalendarEvent(int eventid)
 {
  DMCalendarEvent ret = new DMCalendarEvent();
  ret.setID(eventid);
  String sql = "SELECT envid,eventname,rtrim(eventtype),starttime,endtime, " + "appid,description,allday,status " + "FROM dm.dm_calendar WHERE id=?";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, eventid);
   ResultSet rs = stmt.executeQuery();
   while (rs.next())
   {
    ret.setEnvID(rs.getInt(1));
    ret.setEventTitle(rs.getString(2));
    System.out.println("event type = " + rs.getString(3));
    ret.setEventType(getEventType(rs.getString(3)));
    DMEventType t = getEventType(rs.getString(3));

    System.out.println("event type stored = " + ret.getEventTypeString() + " or " + t.toString());
    ret.setStart(rs.getInt(4));
    ret.setEnd(rs.getInt(5));
    ret.setAppID(rs.getInt(6));
    ret.setEventDesc(rs.getString(7));
    ret.setAllDayEvent(rs.getString(8).equalsIgnoreCase("y"));
   }
   rs.close();
   stmt.close();
   return ret;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  return null;
 }

 public int ModifyEvent(DMCalendarEvent ce)
 {
  //
  // TO DO: Check Permissions
  //
  String sql = "UPDATE dm.dm_calendar SET eventname=?, eventtype=?,starttime=?, endtime=?, appid=?, description=?, allday=?, modifierid=?, modified=?  WHERE id=?";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setString(1, ce.getEventTitle());
   stmt.setString(2, ce.getEventTypeString());
   stmt.setInt(3, ce.getStart());
   stmt.setInt(4, ce.getEnd());
   stmt.setInt(5, ce.getAppID());
   stmt.setString(6, ce.getEventDesc());
   stmt.setString(7, ce.getAllDayEvent() ? "Y" : "N");
   stmt.setInt(8, this.GetUserID());
   stmt.setLong(9, timeNow());
   stmt.setInt(10, ce.getID());
   System.out.println("id to update = " + ce.getID());
   stmt.execute();
   int res = stmt.getUpdateCount();
   System.out.println("UpdateCount=" + stmt.getUpdateCount() + " ce.getEventTitle=[" + ce.getEventTitle() + "]");
   stmt.close();
   Environment env = getEnvironment(ce.getEnvID(), false);
   RecordObjectUpdate(env, "Calendar Entry \"" + ce.getEventTitle() + "\" Changed");
   m_conn.commit();
   return res;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  return -1;
 }

 public int ApproveEvent(int eventid)
 {
  //
  // TO DO: Check Permissions
  //
  String sql1 = "UPDATE dm.dm_calendar SET status='N' where id=?";
  String sql2 = "UPDATE dm.dm_request SET status='C', completed=? where calendarid=?";
  String sql3 = "SELECT envid,eventname FROM dm.dm_calendar WHERE id=?";
  try
  {
   PreparedStatement stmt1 = m_conn.prepareStatement(sql1);
   stmt1.setInt(1, eventid);
   stmt1.execute();
   int res = stmt1.getUpdateCount();
   System.out.println("UpdateCount=" + stmt1.getUpdateCount());
   stmt1.close();
   PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
   stmt2.setLong(1, timeNow());
   stmt2.setInt(2, eventid);
   stmt2.execute();
   PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
   stmt3.setInt(1, eventid);
   ResultSet rs3 = stmt3.executeQuery();
   if (rs3.next())
   {
    Environment env = getEnvironment(rs3.getInt(1), false);
    this.RecordObjectUpdate(env, "Calendar Event \"" + rs3.getString(2) + "\" Approved");
   }
   rs3.close();
   m_conn.commit();
   return res;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  return -1;
 }

 public List<DMCalendarEvent> getEvents(Environment env, Application app, long starttime, long endtime)
 {
  // Called from API - list all events
  List<DMCalendarEvent> ret = new ArrayList<DMCalendarEvent>();
  String sql1 = "SELECT c.id,c.envid,c.eventname,rtrim(c.eventtype),c.starttime,c.endtime, " + "c.allday,c.status,c.appid,c.description,c.creatorid,c.created,c.modifierid,c.modified, " + "r.completed, r.completedby " + "FROM dm.dm_calendar c "
    + "LEFT OUTER JOIN dm.dm_request r ON r.calendarid = c.id " + "LEFT OUTER JOIN dm.dm_environment e on e.id = c.envid " + "LEFT OUTER JOIN dm.dm_application a on a.id = c.appid " + "WHERE c.status<>'D' AND e.domainid in (" + this.getDomainList() + ") " + "AND a.domainid in ("
    + this.getDomainList() + ") AND starttime>=?";
  int ebv = 0, abv = 0, tbv = 0;
  int nbv = 2;
  if (env != null)
  {
   sql1 = sql1 + " AND c.envid=?";
   ebv = nbv++;
  }
  if (app != null)
  {
   sql1 = sql1 + " AND c.appid=?";
   abv = nbv++;
  }
  if (endtime > 0)
  {
   sql1 = sql1 + " AND c.endtime<=?";
   tbv = nbv++;
  }
  // String sql2 = "SELECT envid,unavailstart,unavailend FROM dm.dm_availability WHERE envid=?";
  String sql3 = "SELECT deploymentid,userid,exitcode,exitstatus,appid,started,finished FROM dm.dm_deployment " + "WHERE started>=? AND finished<=?";
  if (env != null)
   sql3 = sql3 + " AND envid=?";
  if (app != null)
   sql3 = sql3 + " AND appid=?";
  try
  {
   System.out.println("sql1=" + sql1);
   PreparedStatement stmt = m_conn.prepareStatement(sql1);
   stmt.setLong(1, starttime);
   if (env != null)
    stmt.setInt(ebv, env.getId());
   if (app != null)
    stmt.setInt(abv, app.getId());
   if (endtime > 0)
    stmt.setLong(tbv, endtime);
   ResultSet rs = stmt.executeQuery();
   while (rs.next())
   {
    DMCalendarEvent ce = new DMCalendarEvent();
    String status = rs.getString(8);
    String EventType = rs.getString(4); // TODO: This needs to be used!
    if (status != null && status.equalsIgnoreCase("p"))
    {
     // Pending event
     ce.setPending(true);
    }
    else
    {
     ce.setPending(false);
    }
    if (EventType.equalsIgnoreCase("reserved"))
     ce.setEventType(DMEventType.RESERVED);
    else if (EventType.equalsIgnoreCase("unavailable"))
     ce.setEventType(DMEventType.UNAVAILABLE);
    else if (EventType.equalsIgnoreCase("auto"))
     ce.setEventType(DMEventType.AUTO);
    ce.setID(rs.getInt(1));
    ce.setEnvID(rs.getInt(2));
    ce.setEventTitle(getString(rs, 3, ""));
    int eventStart = rs.getInt(5);
    int eventEnd = rs.getInt(6);
    ce.setStart(eventStart);
    if (eventEnd < eventStart + 900)
    {
     eventEnd = eventEnd + (eventEnd % 900);
    }
    ce.setEnd(eventEnd);
    ce.setAllDayEvent(getBoolean(rs, 7, false));

    ce.setAppID(rs.getInt(9));
    ce.setEventDesc(getString(rs, 10, ""));
    ce.setCreatorID(rs.getInt(11));
    ce.setCreated(rs.getInt(12));
    ce.setModifierID(rs.getInt(13));
    ce.setModified(rs.getInt(14));
    ce.setApproved(getInteger(rs, 15, 0));
    ce.setApprover(getInteger(rs, 16, 0));
    ret.add(ce);
   }
   rs.close();
   return ret;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  throw new RuntimeException("Unable to retrieve calendar events from database");

 }

 List<DMCalendarEvent> getCalendarEvents(int envid, int start, int end)
 {
  List<DMCalendarEvent> ret = new ArrayList<DMCalendarEvent>();
  String sql1 = "SELECT c.id,c.envid,c.eventname,rtrim(c.eventtype),c.starttime,c.endtime, " + "c.allday,c.status,c.appid,c.description,c.creatorid,c.created,c.modifierid,c.modified, " + "r.completed, r.completedby " + "FROM dm.dm_calendar c "
    + "LEFT OUTER JOIN dm.dm_request r ON r.calendarid = c.id " + "LEFT OUTER JOIN dm.dm_application a ON a.id = c.appid " + "WHERE ((starttime>=? AND starttime<=?) OR (endtime>=? AND endtime<=?)) AND envid=? AND c.status <>'D' AND (a.status is null or a.status='N')";
  String sql2 = "SELECT envid,unavailstart,unavailend FROM dm.dm_availability WHERE envid=?";
  String sql3 = "SELECT d.deploymentid,d.userid,d.exitcode,d.exitstatus,d.appid,d.started,d.finished FROM dm.dm_deployment d,dm.dm_application a " + "WHERE d.started>=? AND d.finished<=? AND d.envid=? AND d.appid=a.id AND a.status='N'";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql1);
   stmt.setInt(1, start);
   stmt.setInt(2, end);
   stmt.setInt(3, start);
   stmt.setInt(4, end);
   stmt.setInt(5, envid);
   ResultSet rs = stmt.executeQuery();
   while (rs.next())
   {
    DMCalendarEvent ce = new DMCalendarEvent();
    String status = rs.getString(8);
    String EventType = rs.getString(4); // TODO: This needs to be used!
    if (status != null && status.equalsIgnoreCase("p"))
    {
     // Pending event
     ce.setPending(true);
    }
    else
    {
     ce.setPending(false);
    }
    if (EventType.equalsIgnoreCase("reserved"))
     ce.setEventType(DMEventType.RESERVED);
    else if (EventType.equalsIgnoreCase("unavailable"))
     ce.setEventType(DMEventType.UNAVAILABLE);
    else if (EventType.equalsIgnoreCase("auto"))
     ce.setEventType(DMEventType.AUTO);
    ce.setID(rs.getInt(1));
    ce.setEnvID(rs.getInt(2));
    ce.setEventTitle(getString(rs, 3, ""));
    int eventStart = rs.getInt(5);
    int eventEnd = rs.getInt(6);
    ce.setStart(eventStart);
    if (eventEnd < eventStart + 900)
    {
     eventEnd = eventEnd + (eventEnd % 900);
    }
    ce.setEnd(eventEnd);
    ce.setAllDayEvent(getBoolean(rs, 7, false));

    ce.setAppID(rs.getInt(9));
    ce.setEventDesc(getString(rs, 10, ""));
    ce.setCreatorID(rs.getInt(11));
    ce.setCreated(rs.getInt(12));
    ce.setModifierID(rs.getInt(13));
    ce.setModified(rs.getInt(14));
    ce.setApproved(getInteger(rs, 15, 0));
    ce.setApprover(getInteger(rs, 16, 0));
    ret.add(ce);
   }
   rs.close();
   //
   // Now overlay the availability events from the weekly availability calendar
   //
   PreparedStatement stmt2 = m_conn.prepareStatement(sql2);
   stmt2.setInt(1, envid);
   ResultSet rs2 = stmt2.executeQuery();
   int x = 1;
   while (rs2.next())
   {
    DMCalendarEvent ce = new DMCalendarEvent();
    ce.setAllDayEvent(false);
    ce.setID(-1 - x); // negative id = weekly "unavailable" event
    ce.setEnvID(rs2.getInt(1));
    ce.setEventTitle("Unavailable");
    ce.setStart(rs2.getInt(2) + start); // Start is start of week
    ce.setEnd(rs2.getInt(3) + start);
    ce.setAppID(0);
    ce.setEventDesc("");
    ce.setEventType(DMEventType.UNAVAILABLE);
    ret.add(ce);
    x++;
   }
   rs2.close();
   //
   // Now do any historic deployments
   //
   PreparedStatement stmt3 = m_conn.prepareStatement(sql3);
   stmt3.setInt(1, start);
   stmt3.setInt(2, end);
   stmt3.setInt(3, envid);
   ResultSet rs3 = stmt3.executeQuery();
   while (rs3.next())
   {
    DMCalendarEvent ce = new DMCalendarEvent();
    ce.setAllDayEvent(false);
    ce.setID(rs3.getInt(1)); // need to differentiate this from normal calendar events
    ce.setEnvID(envid);
    ce.setEventTitle("Deployed");
    int depStart = rs3.getInt(6);
    int depEnd = rs3.getInt(7);
    int interval = depEnd - depStart;
    System.out.println("depstart=" + depStart + " depend=" + depEnd + " interval=" + interval);
    ce.setStart(depStart);
    if (interval < 900)
    {
     // interval between start & end should be at LEAST 900 secs (15 mins) and end on a 15 minute boundary
     interval = 900 + (interval % 900);
     System.out.println("Adding " + interval + " to depEnd");
     depEnd = depEnd + interval;
     System.out.println("depEnd now " + depEnd);
    }
    ce.setEnd(depEnd);
    ce.setAppID(rs3.getInt(5));
    ce.setEventDesc(rs3.getString(4));
    ce.setEventType(DMEventType.DEPLOYMENT);
    ce.setDeployID(rs3.getInt(1));
    ret.add(ce);
    x++;
   }
   rs3.close();

   return ret;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  throw new RuntimeException("Unable to retrieve calendar events from database");
 }

 List<DMCalendarEvent> getWeeklySchedule(int envid)
 {
  List<DMCalendarEvent> ret = new ArrayList<DMCalendarEvent>();
  String sql = "SELECT unavailstart,unavailend FROM dm.dm_availability WHERE envid=?";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, envid);
   ResultSet rs = stmt.executeQuery();
   int x = 1;
   while (rs.next())
   {
    DMCalendarEvent ce = new DMCalendarEvent();
    ce.setAllDayEvent(false);
    ce.setID(0 - x);
    ce.setEnvID(rs.getInt(1));
    ce.setEventTitle("Unavailable");
    ce.setEventType(DMEventType.UNAVAILABLE);
    ce.setStart(rs.getInt(1) + 345600);
    ce.setEnd(rs.getInt(2) + 345600);
    ret.add(ce);
    x++;
   }
   rs.close();
   return ret;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  throw new RuntimeException("Unable to retrieve availability events from database");
 }

 public void DeleteAllAvailability(int envid)
 {
  String sql = "DELETE FROM dm.dm_availability WHERE envid=?";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, envid);
   stmt.execute();
   stmt.close();
   m_conn.commit();
   return;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  throw new RuntimeException("Unable to delete availability events from database");
 }

 private void UpdateCalendarAttributes(int envid, String colname, int t, String newval)
 {
  String sql = "UPDATE dm.dm_environment SET " + colname + "=? WHERE id=?";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   if (t == 1)
   {
    stmt.setInt(1, Integer.parseInt(newval));
   }
   else
   {
    stmt.setString(1, newval);
   }
   stmt.setInt(2, envid);
   stmt.execute();
   stmt.close();
   m_conn.commit();
   return;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  throw new RuntimeException("Unable to update calendar attributes colname=" + colname + " newval=" + newval);
 }

 public void setCalendarStartTime(int envid, int timeinmins)
 {
  UpdateCalendarAttributes(envid, "calstart", 1, Integer.toString(timeinmins));
 }

 public void setCalendarEndTime(int envid, int timeinmins)
 {
  UpdateCalendarAttributes(envid, "calend", 1, Integer.toString(timeinmins));
 }

 public void setCalendarAvailability(int envid, String na)
 {
  UpdateCalendarAttributes(envid, "calusage", 2, na);
 }

 public void AddAvailability(int envid, int start, int end)
 {
  String sql = "INSERT INTO dm.dm_availability(envid,unavailstart,unavailend) VALUES(?,?,?)";
  try
  {
   PreparedStatement stmt = m_conn.prepareStatement(sql);
   stmt.setInt(1, envid);
   stmt.setInt(2, start);
   stmt.setInt(3, end);
   stmt.execute();
   stmt.close();
   m_conn.commit();
   return;
  }
  catch (SQLException ex)
  {
   ex.printStackTrace();
   rollback();
  }
  throw new RuntimeException("Unable to insert availability event into database");
 }


 public void runAutoDeploy(ServletContext context)
 {
  checkConnection(context);
  internalLogin(context);

  try
  {
   // find events that should run now. and check to see if the eventid is in the dm_deployment table if not run it

   String sql = "select appid, envid, a.id, to_timestamp(starttime)::timestamp with time zone, to_timestamp(endtime)::timestamp with time zone " + "from dm.dm_calendar a where a.id not in (select eventid from dm.dm_deployment where eventid is not null) and "
     + "now()::timestamp with time zone >= to_timestamp(starttime)::timestamp::timestamp with time zone and " + "now()::timestamp with time zone <= to_timestamp(endtime)::timestamp with time zone and a.eventtype = 'Auto'";

   PreparedStatement st = m_conn.prepareStatement(sql);
   ResultSet rs = st.executeQuery();
   while (rs.next())
   {
    int appid = rs.getInt(1);
    int envid = rs.getInt(2);
    int eventid = rs.getInt(3);
    Timestamp starttime = rs.getTimestamp(4);
    Timestamp endtime = rs.getTimestamp(5);

    System.out.println("AutoDeploy: appid=" + appid + " envid=" + envid + " eventid=" + eventid + " starttime=" + starttime.toString() + " endtime=" + endtime.toString());

    Application app = getApplication(appid, true);
    Environment env = getEnvironment(envid, true);

    Task t = getTaskByType(app.getDomain(), TaskType.DEPLOY);
    if (t == null)
    {
     System.out.println("Could not start AutoDeploy - Could not find appropriate deploy task");
     continue;
    }
    TaskDeploy task = getTaskDeploy(t.getId());

    System.out.println("using task " + task.getName());
    task.setApplication(app);
    task.setEnvironment(env);
    task.setWaitFor(false);
    task.setDeploymentSessionId(UUID.randomUUID().toString() + "_" + System.currentTimeMillis());

    int deploymentid = -1;

    if (task.run())
    {
     Deployment dep = getDeploymentBySessionId(task, 30);
     if (dep != null && dep.getId() > 0)
     {
      deploymentid = dep.getId();
      PreparedStatement stmt2 = m_conn.prepareStatement("update dm.dm_deployment set eventid=? where deploymentid=?");
      stmt2.setInt(1, eventid);
      stmt2.setInt(2, deploymentid);
      stmt2.execute();
      stmt2.close();
     }
     else
     {
      String msg = task.getLastOutputLine();
      String error = "Timed out waiting for deployment to start" + ((msg != null) ? ("\n" + msg) : "");
      System.out.println("Could not start AutoDeploy - " + error);
     }
    }
    else
    {
     String msg = task.getLastOutputLine();
     String error = "Failed to start deployment" + ((msg != null) ? ("\n" + msg) : "");
     System.out.println("Could not start AutoDeploy - " + error);
    }
   }
   rs.close();
   st.close();
   m_conn.commit();
  }
  catch (SQLException e)
  {
   e.printStackTrace();
  }
 }

 // Code for adding buildid from jenkins plugin
 // else if (elements[0].equals("buildid"))
 // {
 // // Associates a build ID with the specified component
 // // API/buildid/<compname>/<buildid>
 // if (elements.length == 3)
 // {
 // addBuildNumber(so, elements[1], elements[2], request);
 // }
 // else
 // {
 // throw new ApiException("Path contains wrong number of elements (" + elements.length + ")");
 // }
 // }
 //
 // private void addBuildNumber(DMSession so, String compname, String buildnumber, HttpServletRequest request) throws ApiException
 // {
 // // Do some sanity checking
 // Component basecomp = getComponentFromNameOrID(so, compname);
 // String branch = request.getParameter("branch");
 // String taskname = request.getParameter("taskname");
 // TaskCreateVersion tcv = null;
 // if (taskname == null)
 // taskname = request.getParameter("task");
 // if (taskname != null)
 // {
 // // taskname specified. Make sure it exists and is executable.
 // try
 // {
 // Task t = so.getTaskByName(taskname); // Throws RuntimeException is taskname is not unique or does not exist
 // if (t.getTaskType() != TaskType.CREATE_VERSION)
 // throw new ApiException("Specified task is not a Create Version task");
 // tcv = so.getTaskCreateVersion(t.getId());
 // }
 // catch (Exception ex)
 // {
 // throw new ApiException(ex.getMessage());
 // }
 // }
 // try
 // {
 // int buildno = Integer.parseInt(buildnumber); // will throw NumberFormatException if there's a problem
 // Component comp = so.getLatestVersion(basecomp, branch);
 // BuildJob buildjob = comp.getBuildJob();
 // if (buildjob == null)
 // throw new ApiException("Component " + compname + " has no associated build job");
 // // Get any application associated with this component
 // List<Application> approvedApps = new ArrayList<Application>();
 // List<Application> applications = so.getAppsForComponent(comp);
 // for (Application app : applications)
 // {
 // List<Domain> domlist = so.getApprovalDomains(app.getId());
 // if (domlist.size() > 0)
 // approvedApps.add(app);
 // }
 // if (approvedApps.size() > 0)
 // {
 // // This application is approved. Need to create a new version of the component
 // System.out.println("Component " + comp.getName() + " is associated with the following approved applications:");
 // for (Application aa : approvedApps)
 // {
 // System.out.println(aa.getName());
 // }
 // System.out.println("Creating new Component Version");
 // int newid = so.componentNewVersion(comp.getId(), comp.getXpos(), comp.getYpos() + 50);
 // System.out.println("newid=" + newid);
 // Component newcomp = so.getComponent(newid, true);
 // if (newcomp == null)
 // throw new ApiException("Failed to create new component");
 // // Now find the applications that were approved and create new versions of those applications,
 // // replacing the old component with the new one
 // for (Application aa : approvedApps)
 // {
 // Domain domain = aa.getDomain();
 // String fqdn = (domain != null) ? domain.getFullDomain() : "";
 // System.out.println("Creating new application version for " + fqdn + "." + aa.getName());
 // Application nav = newAppVersion(so, fqdn + "." + aa.getName(), request, tcv);
 // if (nav == null)
 // throw new ApiException("Could not create new application version");
 // System.out.println("Created new application " + nav.getName());
 // System.out.println("Replacing component " + comp.getName() + " with component " + newcomp.getName() + " in app " + nav.getName());
 // so.applicationReplaceComponent(nav.getId(), comp.getId(), newcomp.getId(), false);
 // }
 // comp = newcomp;
 // }
 //
 // // Now add any other details associated with this build
 // // There are two ways of passing this information
 // // issue=<issue1>&issue=<issue> etc
 // // or
 // // issues=issue1,issue2 etc
 // String ispar = request.getParameter("issues");
 // String issues[] = (ispar != null) ? ispar.split(",") : request.getParameterValues("issue");
 // String filepar = request.getParameter("files");
 // String files[] = (filepar != null) ? filepar.split(",") : request.getParameterValues("file");
 // String commit = request.getParameter("commit");
 // so.AddBuildToComponent(comp, buildno, issues, files, commit, true);
 // }
 // catch (NumberFormatException ex)
 // {
 // throw new ApiException("buildnumber " + buildnumber + " is not valid");
 // }
 // }
}
