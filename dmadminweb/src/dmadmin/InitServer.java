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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Timer;
import java.util.concurrent.TimeUnit;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.JSONObject;
import dmadmin.pro.AutoDeploy;
import dmadmin.pro.TimedTask;

public class InitServer extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
	private String os = null;
	private Timer m_timer = null;

	private String getOsName()
	{
	      if (os == null) { os = System.getProperty("os.name"); }
	      return os;
	}

	private boolean isWindows()
	{
		return getOsName().startsWith("Windows") || getOsName().startsWith("Mac");	// For PAG dev
	}

	private boolean isLinux()
	{
		return !isWindows();
	}

	public void destroy()
	{
		System.out.println("destroy method called");
		if (m_timer != null) m_timer.cancel();
	}

 String DMHome = null;

 private Connection m_conn;
 int errorcnt = 0;
 int maxver = 0;
 boolean updatePerformed = false;
 StringBuilder dDSN = new StringBuilder();
 StringBuilder dUserName = new StringBuilder();
 StringBuilder dPassword = new StringBuilder();
 private String ConnectionString;
 private String DriverName;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public InitServer()
 {
  super();
 }

 private ArrayList<File> listFilesForFolder(final File folder)
 {
  final ArrayList<File> output = new ArrayList<File>();

  for (final File fileEntry : folder.listFiles())
  {
   if (!fileEntry.isDirectory() && (fileEntry.getName().endsWith(".sql") || fileEntry.getName().endsWith(".re")))
   {
    output.add(fileEntry);
   }
  }

  return output;
 }

 private byte[] Decrypt3DES(String encodedString, byte[] passphrase)
 {
  try
  {
   byte[] pp1 = javax.xml.bind.DatatypeConverter.parseBase64Binary(encodedString);
   byte[] passphrase1 = new byte[pp1.length - 16]; // take the "Salted__saltsalt" off the front
   byte[] salt = new byte[8];
   for (int n = 8, p = 0; n < pp1.length;)
   {
    if (p < 8)
     salt[p] = pp1[n];
    if (p >= 8)
     passphrase1[p - 8] = pp1[n];
    p++;
    n++;
   }
   final Cipher decipher = Cipher.getInstance("DESede/CBC/PKCS5Padding");
   final MessageDigest md = MessageDigest.getInstance("md5");
   final byte[][] keyAndIV = EVP_BytesToKey(24, // Key len
     decipher.getBlockSize(), // IV len
     md, salt, passphrase, 1);
   SecretKeySpec key = new SecretKeySpec(keyAndIV[0], "DESede");
   IvParameterSpec iv = new IvParameterSpec(keyAndIV[1]);
   decipher.init(Cipher.DECRYPT_MODE, key, iv);
   return decipher.doFinal(passphrase1);
  }
  catch (NoSuchAlgorithmException e)
  {
   System.out.println("NO SUCH ALGORITHM EXCEPTION");
   e.printStackTrace();
  }
  catch (NoSuchPaddingException e)
  {
   System.out.println("NO SUCH PADDING EXCEPTION");
   e.printStackTrace();
  }
  catch (InvalidKeyException e)
  {
   System.out.println("INVALID KEY EXCEPTION");
   e.printStackTrace();
  }
  catch (InvalidAlgorithmParameterException e)
  {
   System.out.println("INVALID ALGORITHM PARAMETER EXCEPTION");
   e.printStackTrace();
  }
  catch (IllegalBlockSizeException e)
  {
   System.out.println("ILLEGAL BLOCK SIZE EXCEPTION");
   e.printStackTrace();
  }
  catch (BadPaddingException e)
  {
   System.out.println("BAD PADDING EXCEPTION");
   e.printStackTrace();
  }
  return null;
 }

 private int getSchemaVersion()
 {
  int res = 0;

  try
  {
   try (PreparedStatement st = m_conn.prepareStatement("SELECT schemaver FROM dm.dm_tableinfo");

   ResultSet rs = st.executeQuery())
   {
    if (rs.next())
    {
     res = rs.getInt(1);
     return res;
    }
   }
  }
  catch (SQLException e)
  {
   rollback();
  }

  return res;
 }

 private boolean checkProTables()
 {
  boolean res = true;

  try
  {
   try (PreparedStatement st = m_conn.prepareStatement("SELECT count(*) FROM dm.dm_defects");

   ResultSet rs = st.executeQuery())
   {
    if (rs.next())
    {
     return false;
    }
   }
  }
  catch (SQLException e)
  {
   rollback();
  }

  return res;
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {

 }

 public void init()
 {
  if (System.getenv("LOAD_DB") == null)
   return;

  try (DMSession session = new DMSession(getServletContext()))
  {
  String absoluteDiskPath = getServletContext().getRealPath("/WEB-INF") + "/localdeploy.reg";
  File regfile = new File(absoluteDiskPath);

  if (regfile.exists())
  {
   String[] cmd = { "regedit", "/S", regfile.getAbsolutePath() };
   try
   {
    Process proc = Runtime.getRuntime().exec(cmd);
    proc.waitFor();
    proc.destroy();
   }
   catch (Exception e)
   {
   }
   regfile.delete();
  }

  long starttime = System.currentTimeMillis();
  System.setProperty("StartTime", (new Long(starttime)).toString());

  int dbtype = InitDBConnection();	// 0 = Postgres, 1 = Oracle

  if (dbtype == 1) {
	  // For Oracle, switch the schema first. This may not work on first install because only the
	  // base install script creates the schema
	  try {
		  Statement st = m_conn.createStatement();
		  st.execute("ALTER SESSION SET current_schema=dm");
		  st.close();
	  } catch (SQLException ex) {
		  System.out.println("Failed to alter session");
	  }
  }
  String sqlpath=(dbtype==1)?"/WEB-INF/schema/oracle":"/WEB-INF/schema";
  System.out.println("Taking SQL Scripts from "+sqlpath);

  absoluteDiskPath = getServletContext().getRealPath(sqlpath);
  File sf = new File(absoluteDiskPath);

  ArrayList<File> files = listFilesForFolder(sf);
  Collections.sort(files, new FileNameComp());


  int schemaver = getSchemaVersion();
  boolean proTablesMissing = checkProTables();

   if (schemaver != 0 && proTablesMissing)
   {
    ArrayList<String> pfiles = new ArrayList<String>();
    pfiles.add("2015123101.sql");

    pfiles.add("2015123101.sql");
    pfiles.add("2016021501.sql");
    pfiles.add("2016092001.sql");
    pfiles.add("2016100801.sql");
    pfiles.add("2016112801.sql");
    pfiles.add("2016112804.sql");
    pfiles.add("2017050801.sql");
    pfiles.add("2017050802.sql");
    pfiles.add("2017050803.sql");
    pfiles.add("2017053101.sql");
    pfiles.add("2017053102.sql");
    pfiles.add("2017053103.sql");
    pfiles.add("2017060201.sql");
    pfiles.add("2017062301.sql");
    pfiles.add("2017111001.sql");

    for (int i = 0; i < files.size(); i++)
    {
     String bn = files.get(i).getName();

     if (!pfiles.contains(bn))
      continue;

     String ext = bn.replaceAll("^.*[.]", "");
     String name = bn.replace(".sql", "").replace(".re", "");
     String basename = name.replace("_linux", "").replace("_windows", "");
     System.out.println("name=[" + name + "] basename=[" + basename + "] ext=[" + ext + "]");
     Integer filever = new Integer(basename);

     if (maxver < filever.intValue())
      maxver = filever.intValue();
     // Handle platform specific files
     if (name.endsWith("_linux") && !isLinux())
      continue;
     if (name.endsWith("_windows") && !isWindows())
      continue;

     if (ext.equalsIgnoreCase("sql"))
     {
      System.out.println("Loading sql from " + name + ".sql");

      try
      {
       BufferedReader br = new BufferedReader(new FileReader(files.get(i)));
       StringBuffer onebigline = new StringBuffer();
       String line = null;
       while ((line = br.readLine()) != null)
       {
        if (!line.startsWith("--") && line.trim().length() > 0)
         onebigline.append(line + "\n");
       }

       br.close();

       line = onebigline.toString();
       String[] parts = line.split("\\);\n");
       if (line.toLowerCase().contains("create ") && line.toLowerCase().contains("function dm."))
       {
        parts = line.split("\n");
        System.out.println("Executing [" + parts[0] + "]");
        Statement st;
        try
        {
         st = m_conn.createStatement();
         st.execute(parts[0]);
         st.close();
         updatePerformed = true;
        }
        catch (SQLException e)
        {
         errorcnt++;
         System.out.println(e.getMessage());
        }
       }
       else
       {
        for (int k = 0; k < parts.length; k++)
        {
         if (parts[k].trim().length() > 0)
          if (parts[k].toLowerCase().contains("update ") || parts[k].toLowerCase().contains("delete from"))
           RunStatement(parts[k]);
          else if (parts[k].toLowerCase().contains("insert ") && parts[k].toLowerCase().contains("select "))
           RunStatement(parts[k]);
          else if (!parts[k].trim().endsWith(";"))
           RunStatement(parts[k] + ");");
          else
           RunStatement(parts[k]);
        }
       }
      }
      catch (FileNotFoundException e)
      {
       e.printStackTrace();
      }
      catch (IOException e)
      {

      }
      if (i == 0 && dbtype == 1)
      {
       // First file through when running Oracle could fail if the "dm" user
       // already exists. Reset fail count.
       errorcnt = 0;
      }
     }
     else if (ext.equalsIgnoreCase("re"))
     {
      // Loading a new function/procedure
      System.out.println("Loading action from " + name + ".re");
      System.out.println("Importing Function from " + absoluteDiskPath + "/" + name + ".re");
      JSONObject res = session.ImportFunction(1, absoluteDiskPath + "/" + name + ".re");
      System.out.println("res=" + res);
      updatePerformed = true;
     }
    }
   }

  System.out.println("SCHEMA=" + schemaver);
  System.setProperty("SCHEMA", (new Integer(schemaver)).toString());

  for (int i = 0; i < files.size(); i++)
  {
   String bn = files.get(i).getName();
   String ext = bn.replaceAll("^.*[.]","");
   String name = bn.replace(".sql", "").replace(".re","");
   String basename = name.replace("_linux","").replace("_windows","");
   System.out.println("name=["+name+"] basename=["+basename+"] ext=["+ext+"]");
   Integer filever = new Integer(basename);
   if (filever.intValue() <= schemaver)
    continue;

   if (maxver < filever.intValue())
    maxver = filever.intValue();
   // Handle platform specific files
   if (name.endsWith("_linux") && !isLinux()) continue;
   if (name.endsWith("_windows") && !isWindows()) continue;

   if (ext.equalsIgnoreCase("sql")) {
	   System.out.println("Loading sql from " + name + ".sql");

	   try
	   {
	    BufferedReader br = new BufferedReader(new FileReader(files.get(i)));
	    StringBuffer onebigline = new StringBuffer();
	    String line = null;
	    while ((line = br.readLine()) != null)
	    {
	     if (!line.startsWith("--") && line.trim().length() > 0)
	      onebigline.append(line + "\n");
	    }

	    br.close();

	    line = onebigline.toString();
	    String[] parts = line.split("\\);\n");
	    if (line.toLowerCase().contains("create ") && line.toLowerCase().contains("function dm."))
	    {
	     parts = line.split("\n");
	     System.out.println("Executing ["+parts[0]+"]");
	     Statement st;
      try
      {
       st = m_conn.createStatement();
       st.execute(parts[0]);
       st.close();
       updatePerformed = true;
      }
      catch (SQLException e)
      {
       errorcnt++;
       System.out.println(e.getMessage());
      }
	    }
	    else
	    {
	    for (int k = 0; k < parts.length; k++)
	    {
	     if (parts[k].trim().length() > 0)
	      if (parts[k].toLowerCase().contains("update ") || parts[k].toLowerCase().contains("delete from"))
	       RunStatement(parts[k]);
	      else if (parts[k].toLowerCase().contains("insert ") && parts[k].toLowerCase().contains("select "))
	       RunStatement(parts[k]);
	      else if (!parts[k].trim().endsWith(";"))
	        RunStatement(parts[k] + ");");
	      else
	       RunStatement(parts[k]);
	    }
	    }
	   }
	   catch (FileNotFoundException e)
	   {
	    e.printStackTrace();
	   }
	   catch (IOException e)
	   {

	   }
	   if (i==0 && dbtype == 1) {
		   // First file through when running Oracle could fail if the "dm" user
		   // already exists. Reset fail count.
		   errorcnt=0;
	   }
   } else if(ext.equalsIgnoreCase("re")) {
	   // Loading a new function/procedure
	   System.out.println("Loading action from "+name+".re");
	   System.out.println("Importing Function from "+absoluteDiskPath+"/"+name+".re");
	   JSONObject res = session.ImportFunction(1,absoluteDiskPath+"/"+name+".re");
	   System.out.println("res="+res);
	   updatePerformed=true;
   }
  }
  System.out.println("errorcnt="+errorcnt+" updatePerformed="+updatePerformed);
  if (errorcnt > 0)
   rollback();
  else
  {
   cleanAttrs();

   try
   {
    if (updatePerformed)
    {
     Statement st = m_conn.createStatement();

     st.execute("UPDATE dm.dm_tableinfo set schemaver = " + maxver);
     st.close();
     st = m_conn.createStatement();
     //
     // For Oracle, switch the schema. For Postgres set the search_path to pick up our
     // own schema first.
     //
     if (dbtype == 0) {
    	 // Postgres
    	 String user = dUserName.toString();

    	 if (user.contains("@"))
    	 {
    	  String parts[] = user.split("@");
    	  st.execute("ALTER USER " + parts[0] + " SET search_path TO dm,public;");
    	 }
    	 else
    	  st.execute("ALTER USER " + dUserName.toString() + " SET search_path TO dm,public;");
     } else {
    	 // Oracle
    	 st.execute("ALTER SESSION set current_schema=dm");
     }

     st.close();
     m_conn.commit();

    }
   }
   catch (SQLException e)
   {
    rollback();
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }

  m_timer = new Timer(); // Instantiate Timer Object
  TimedTask tt = new TimedTask(getServletContext());
  System.out.println("About to start the timer");
  m_timer.schedule(tt, 0, TimeUnit.MINUTES.toMillis(1));	// Now then every 1 mins

  Timer m_autodeploy_timer = new Timer(); // Instantiate Timer Object
  AutoDeploy ad = new AutoDeploy(getServletContext());
  System.out.println("About to start the timer");
  m_autodeploy_timer.schedule(ad, 0, TimeUnit.MINUTES.toMillis(1)); // Now then every 1 mins

  }

  // Exit everything
  if (System.getenv("INIT_ONLY") != null)
  {
   System.out.println("INIT_ONLY - Exiting");
   Runtime.getRuntime().halt(0);
  }
 }


 public void cleanAttrs()
 {
  ArrayList<String> kw = new ArrayList<>(Arrays.asList("buildid", "buildurl", "chart", "operator", "builddate",
				"dockersha", "gitcommit", "gitrepo", "gittag", "giturl", "chartversion", "chartnamespace", "dockertag",
				"chartrepo", "chartrepourl", "serviceowner", "serviceowneremail", "serviceownerphone", "slackchannel",
				"discordchannel", "hipchatchannel", "pagerdutyurl", "pagerdutybusinessurl", "purl"));

  try
  {
   for (int i=0;i<kw.size();i++)
   {
	String key = kw.get(i);
	String sql = "select compid, name, value from dm.dm_componentvars where lower(name) = ?";

	PreparedStatement stmt = m_conn.prepareStatement(sql, ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_UPDATABLE);

	stmt.setString(1,key);
	ResultSet rs = stmt.executeQuery();
	while(rs.next())
	{
	 int compid = rs.getInt(1);
	 String value = rs.getString(3);

	 // see if compitem exists if not insert row
	 String csql = "select count(*) from dm.dm_componentitem where compid = ?";

	 PreparedStatement cstmt = m_conn.prepareStatement(csql);
	 cstmt.setInt(1,compid);
	 ResultSet crs = cstmt.executeQuery();
	 int cnt = 0;
	 if (crs.next())
	 {
	  cnt = crs.getInt(1);
	 }
	 crs.close();
	 cstmt.close();

	 if (cnt <= 0) // insert compitem row for compid
	 {
	  String isql = "INSERT INTO dm.dm_componentitem(id, compid, name, xpos, ypos, creatorid, status, kind) values (nextval('dm_compitem_id_seq'), ?, 'item 1', 140, 80, 1, 'N', 'docker')";
	  PreparedStatement istmt = m_conn.prepareStatement(isql);
	  istmt.setInt(1, compid);
	  istmt.execute();
	  istmt.close();
	 }

	 String usql = "update dm.dm_componentitem set " + key + "=? where compid=? and " + key + " is null";
	 PreparedStatement ustmt = m_conn.prepareStatement(usql);
	 ustmt.setString(1,value);
	 ustmt.setInt(2, compid);
	 ustmt.execute();;
	 ustmt.close();

	 rs.deleteRow(); // delete row from componentvars
	}
	rs.close();
	stmt.close();
	m_conn.commit();
   }
  }
  catch (SQLException e)
  {
		e.printStackTrace();
  }
 }


 String readFile(String filename) throws FileNotFoundException, IOException
 {
  File file = new File(filename);

  FileInputStream fin = new FileInputStream(file);
  byte fileContent[] = new byte[(int) file.length()];
  fin.read(fileContent);
  String strFileContent = new String(fileContent);
  fin.close();
  return strFileContent;
 }

 private void rollback()
 {
  try
  {
   if (m_conn != null)
    m_conn.rollback();
  }
  catch (SQLException e)
  {
   e.printStackTrace();
  }
 }

 private int InitDBConnection()
 {
   int ret=0;	// Postgres default

   //
   // Connect to the database
   //
   DMHome = "";
   ConnectionString = "";
   DriverName = "";

   System.out.println("connectToDatabase()");

   DMHome = System.getenv("DMHome");
   if (DMHome == null)
    DMHome = getServletContext().getInitParameter("DMHOME");

   ConnectionString = System.getenv("DBConnectionString");
   if (ConnectionString == null)
    ConnectionString = getServletContext().getInitParameter("DBConnectionString");

   DriverName = System.getenv("DBDriverName");
   if (DriverName == null)
    DriverName = getServletContext().getInitParameter("DBDriverName");

   System.out.println("DMHOME="+DMHome);

   dUserName = new StringBuilder();

   String DBUserName = System.getenv("DBUserName");
   if (DBUserName != null)
    dUserName.append(DBUserName);

   dPassword = new StringBuilder();

   String DBPassword = System.getenv("DBPassword");
   if (DBPassword != null)
    dPassword.append(DBPassword);

   if (DriverName.toLowerCase().contains("oracle") || DriverName.toLowerCase().contains("mysql")) {
	   ret=1;
   }
   System.out.println("DMHOME=" + DMHome);

   try
   {
    if (dUserName.length() == 0)
    {
     String base64Original = readFile(DMHome + "/dm.odbc");
     String base64passphrase = readFile(DMHome + "/dm.asc");

     final byte[] passphrase = Decrypt3DES(base64passphrase, "dm15k1ng".getBytes("UTF-8"));
     final byte[] plainText = Decrypt3DES(base64Original, passphrase);

     for (int i = 0, d = 0; i < plainText.length; i++)
     {
      if (plainText[i] != 0)
      {
       if (d == 0)
        dDSN.append(String.format("%c", plainText[i]));
       if (d == 1)
        dUserName.append(String.format("%c", plainText[i]));
       if (d == 2)
        dPassword.append(String.format("%c", plainText[i]));
      }
      else
       d++;
     }
    }

    DriverName = DriverName.replaceAll("com\\.impossibl\\.postgres\\.jdbc\\.PGDriver", "org.postgresql.Driver");
    ConnectionString = ConnectionString.replaceAll("jdbc\\:pgsql", "jdbc:postgresql");
    // DSN is ignored for Postgres Driver
    Class.forName(DriverName);

    System.out.println("DMHOME=" + DMHome);
    System.out.println("DRIVERNAME=" + DriverName);
    System.out.println("CONNECTIONSTRING=" + ConnectionString);
    System.out.println("USERNAME=" + dUserName.toString());
    System.out.println("PASSWORDNAME=*******");

    boolean noConnection = true;
    do
    {
     try
     {
      m_conn = DriverManager.getConnection(ConnectionString, dUserName.toString(), dPassword.toString());
      m_conn.setAutoCommit(false);
      noConnection = false;
     }
     catch (Exception e)
     {
      try
      {
       System.out.println("No DB Connection - Retrying");
       Thread.sleep(30000);
      }
      catch (InterruptedException ie)
      {
       Thread.currentThread().interrupt();
      }
     }
    } while (noConnection);
   }
   catch (Exception e)
   {
    e.printStackTrace();
   }
   return ret;	// database type (1 for Oracle)
 }

 private void RunStatement(String sql)
 {
  try
  {
   sql = sql.replace("VAIO", getHostName());

  // System.out.println("Before: "+sql);


  // Loop through each statement - note a ; could be in quotes in which case it's part
  // of a string and NOT a statement terminator
  boolean inString=false;
  String stmt="";
  int sl = sql.length()-1;
  for (int i=0;i<=sl;i++) {
	  if (sql.charAt(i)=='\'') inString = !inString;
	  if (sql.charAt(i)==';' && !inString) {
		  // End of Statement
		  System.out.println("Executing ["+stmt+"]");
		  Statement st = m_conn.createStatement();
		  st.execute(stmt);
		  st.close();
		  stmt="";
		  if (i<sl) i++;	// skip ;
	  }
	  stmt=stmt+sql.charAt(i);
  }
  // mop up anything left over
  if (stmt.trim().length()>1) {
	  sl = stmt.length()-1;
	  int ob=0;
	  inString=false;
	  for (int i=0;i<=sl;i++) {
		  if (stmt.charAt(i)=='\'') inString = !inString;
		  if (stmt.charAt(i)=='(' && !inString) ob++;
		  if (stmt.charAt(i)==')' && !inString) ob--;
	  }
	  for (int x=ob;x>0;x--) stmt=stmt+")";	// add any missing closing brackets
	  System.out.println("Executing ["+stmt+"]");
	  Statement st = m_conn.createStatement();
	  st.execute(stmt);
	  st.close();
  }

  updatePerformed = true;
 }
 catch (SQLException e)
 {
  errorcnt++;
  System.out.println(e.getMessage());
 }

 }

 private static byte[][] EVP_BytesToKey(int key_len, int iv_len, MessageDigest md, byte[] salt, byte[] data, int count)
 {
  byte[][] both = new byte[2][];
  byte[] key = new byte[key_len];
  int key_ix = 0;
  byte[] iv = new byte[iv_len];
  int iv_ix = 0;
  both[0] = key;
  both[1] = iv;
  byte[] md_buf = null;
  int nkey = key_len;
  int niv = iv_len;
  int i = 0;
  if (data == null)
   return both;
  int addmd = 0;
  for (;;)
  {
   md.reset();
   if (addmd++ > 0)
    md.update(md_buf);
   md.update(data);
   if (null != salt)
    md.update(salt, 0, 8);
   md_buf = md.digest();
   for (i = 1; i < count; i++)
   {
    md.reset();
    md.update(md_buf);
    md_buf = md.digest();
   }
   i = 0;
   if (nkey > 0)
   {
    for (;;)
    {
     if (nkey == 0)
      break;
     if (i == md_buf.length)
      break;
     key[key_ix++] = md_buf[i];
     nkey--;
     i++;
    }
   }
   if (niv > 0 && i != md_buf.length)
   {
    for (;;)
    {
     if (niv == 0)
      break;
     if (i == md_buf.length)
      break;
     iv[iv_ix++] = md_buf[i];
     niv--;
     i++;
    }
   }
   if (nkey == 0 && niv == 0)
    break;
  }
  for (i = 0; i < md_buf.length; i++)
   md_buf[i] = 0;
  return both;
 }

 private String getHostName()
 {
  // try InetAddress.LocalHost first;
  // NOTE -- InetAddress.getLocalHost().getHostName() will not work in certain environments.
  try
  {
   String result = InetAddress.getLocalHost().getHostName();
   if (result != null && result.trim().length() > 0)
    return result;
  }
  catch (UnknownHostException e)
  {
   // failed; try alternate means.
  }

  // try environment properties.
  //
  String host = System.getenv("COMPUTERNAME");
  if (host != null)
   return host;
  host = System.getenv("HOSTNAME");
  if (host != null)
   return host;

  // undetermined.
  return "";
 }

 class FileNameComp implements Comparator<File>
 {

  public int compare(File e1, File e2)
  {
   return e1.getName().compareTo(e2.getName());
  }
 }
}
