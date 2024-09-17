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
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
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
import java.sql.Types;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
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

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import us.springett.cvss.Cvss;
import us.springett.cvss.Score;

public class InitVulns extends HttpServletBase
{
	private static final long serialVersionUID = 1L;

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
 public InitVulns()
 {
  super();
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

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {

 }

 public void init()
 {
  try (DMSession session = new DMSession(getServletContext()))
  {

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

  Thread t = new Thread()
  {
   public void run()
   {
    while(true)
    {
     updateVulns();
     try
     {
      TimeUnit.HOURS.sleep(1);
     }
     catch (InterruptedException e)
     {
      e.printStackTrace();
     }
    }
   }
  };
  t.start();
  }
 }


private JsonArray getVulns(String payload)
{
 JsonArray vulns = new JsonArray();
 String url = "https://api.osv.dev/v1/query";
 try
 {
  HttpURLConnection con = (HttpURLConnection) new URL(url).openConnection();

  // Setting basic post request
  con.setRequestMethod("POST");
  con.setRequestProperty("Content-Type","application/json");

  // Send post request
  con.setDoOutput(true);
  DataOutputStream wr = new DataOutputStream(con.getOutputStream());
  wr.writeBytes(payload);
  wr.flush();
  wr.close();

  int responseCode = con.getResponseCode();

  if (responseCode != 200)
   return vulns;

  BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
  String output;
  StringBuffer response = new StringBuffer();

  while ((output = in.readLine()) != null)
   response.append(output);

  in.close();

  output = response.toString();
  JsonObject jsonObject = new JsonParser().parse(output).getAsJsonObject();
  if (jsonObject.has("vulns"))
    vulns = jsonObject.getAsJsonArray("vulns");

  return vulns;
 }
 catch (Exception e)
 {
  e.printStackTrace();
 }
 return vulns;
}

private void updateVulns()
{
 Statement delst;
 try
 {
  delst = m_conn.createStatement();
  delst.execute("delete from dm.dm_componentdeps where deptype = 'cve'");
  delst.close();

  String sql = "select distinct packagename, packageversion, purl from dm.dm_componentdeps where deptype = 'license'";
  PreparedStatement st = m_conn.prepareStatement(sql);
  ResultSet rs = st.executeQuery();
  while (rs.next())
  {
   String packagename = rs.getString(1);
   String packageversion = rs.getString(2);
   String purl = rs.getString(3);
   String payload = "";

   if (packagename.equalsIgnoreCase("PyJWT"))
       System.out.println(packagename);

   if (purl == null || purl.isEmpty())
    payload = "{\"package\": {\"name\":\"" + packagename.toLowerCase() + "\"}, \"version\":\"" + packageversion.toLowerCase() + "\"}";
   else
   {
    if (purl.contains("?"))
     purl = purl.split("\\?")[0];
    payload = "{\"package\": {\"purl\": \"" + purl.toLowerCase() + "\"}}";
   }
   DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
   LocalDateTime now = LocalDateTime.now();

//   System.out.print(dtf.format(now) + " - ");
//   System.out.println(payload);
   JsonArray vulns = getVulns(payload);
//   System.out.print(dtf.format(now) + " - ");
//   System.out.println(vulns.toString());

   for (int i = 0; i < vulns.size(); i++)
   {
    JsonObject obj = vulns.get(i).getAsJsonObject();
    String id = obj.get("id").getAsString();
    String desc = "";
    if (obj.has("summary"))
      desc = obj.get("summary").getAsString();

    if (obj.has("aliases"))
    {
     JsonArray aliaslist = obj.getAsJsonArray("aliases");
     String aliases = "";
     for (int k=0;k<aliaslist.size();k++)
       aliases += aliaslist.get(k).getAsString() + " ";
     if (desc.length() > 0)
      desc = aliases + ": " + desc;
     else
      desc = aliases;
    }

    String risklevel = "";
    String cvss = "";
    if (obj.has("severity"))
    {
     JsonArray sevlist = obj.getAsJsonArray("severity");
     JsonObject sev = sevlist.get(0).getAsJsonObject();
     cvss = sev.get("score").getAsString();
     Cvss vector = Cvss.fromVector(cvss);
     Score score = vector.calculateScore();
     double base = score.getBaseScore();
     if (base == 0.0)
      risklevel = "None";
     else if (base >= 0.1 && base <= 3.9)
      risklevel = "Low";
     else if (base >= 4.0 && base <= 6.9)
      risklevel = "Medium";
     else if (base >= 7.0 && base <= 8.9)
      risklevel = "High";
     else if (base >= 9.0)
      risklevel = "Critical";
    }

    if (risklevel.isEmpty() && obj.has("database_specific"))
    {
     JsonObject sec = obj.get("database_specific").getAsJsonObject();
     if (sec.has("severity"))
      risklevel = sec.get("severity").getAsString();

     risklevel=capitalize(risklevel);

     if (risklevel.equalsIgnoreCase("MODERATE"))
      risklevel = "Medium";
    }

    try
    {
        PreparedStatement ins_st = m_conn.prepareStatement("insert into dm.dm_vulns (packagename, packageversion, purl, id, summary, risklevel, cvss) values (?, ?, ?, ?, ?, ?, ?) ON CONFLICT ON CONSTRAINT dm_vulns_pkey DO NOTHING");
        ins_st.setString(1, packagename);
        ins_st.setString(2, packageversion);
        if (purl == null || purl.isEmpty())
         ins_st.setNull(3, Types.VARCHAR);
        else
         ins_st.setString(3, purl);
        ins_st.setString(4, id);
        ins_st.setString(5, desc);
        ins_st.setString(6, risklevel);
        ins_st.setString(7, cvss);
        ins_st.execute();
        ins_st.close();
        m_conn.commit();
    }
    catch (SQLException e)
    {
     System.out.println("Duplicate Vuln:" + packagename + "," + packageversion +"," + id + "," + desc + "," + risklevel + "," + cvss);
//     m_conn.rollback();
    }
   }
  }
  rs.close();
  st.close();
  m_conn.commit();
 }
 catch (SQLException e)
 {
  // TODO Auto-generated catch block
  e.printStackTrace();
 }
}

public static String capitalize(String str)
{
 if (str == null || str.isEmpty())
 {
  return str;
 }
 str = str.toLowerCase();
 return str.substring(0, 1).toUpperCase() + str.substring(1);
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

}
