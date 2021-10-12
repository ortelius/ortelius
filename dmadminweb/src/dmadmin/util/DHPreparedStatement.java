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

package dmadmin.util;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Arrays;

import org.apache.commons.lang3.StringUtils;

public class DHPreparedStatement
{
 String sql;
 Connection m_conn;
 Statement s;
 ArrayList<String> parts;
 ArrayList<String> parms;
 ResultSet rs;
 int pcnt=0;
 
 public DHPreparedStatement(Connection m_conn,String sql)
 {
  this.m_conn = m_conn;
  this.sql = sql;
 }

 public static DHPreparedStatement prepareStatement(Connection m_conn, String sql) throws SQLException
 {
  DHPreparedStatement stmt = new DHPreparedStatement(m_conn,sql);

  stmt.parts = new ArrayList<String>(Arrays.asList(sql.split("\\?")));
  stmt.parms = new ArrayList<String>();
  stmt.pcnt = StringUtils.countMatches(sql,"?");
  for (int i=0;i<stmt.pcnt;i++)
  {
   stmt.parms.add("");
  }
  return stmt;
 }

 public void setInt(int parameterIndex, int x) throws SQLException
 {
  parms.set(parameterIndex-1, x + "");
 }

 public void setString(int parameterIndex, String x) throws SQLException
 {
  parms.set(parameterIndex-1, "'" + escapeSql(x) + "'");
 }
 
 public void setNull(int parameterIndex,int x)
 {
  parms.set(parameterIndex-1, "null");
 }
 
 public void setLong(int parameterIndex, long x) throws SQLException
 {
  parms.set(parameterIndex-1, x + "");
 }
 
 public ResultSet executeQuery() throws SQLException
 {
  String sql="";
  
  for (int i=0;i<parts.size();i++)
  {
   sql += parts.get(i);
   
   if (i<parms.size())
    sql += parms.get(i);
  }
  s = m_conn.createStatement();
  System.out.println("### SQL=" + sql);
  return s.executeQuery(sql);
 }

 public boolean execute()
 {
  String sql="";
  
  for (int i=0;i<parts.size();i++)
  {
   sql += parts.get(i);
   
   if (i<parms.size())
    sql += parms.get(i);
  }
  try
  {
  s = m_conn.createStatement();
  System.out.println("### SQL=" + sql);
  rs = s.executeQuery(sql);
  }
  catch (SQLException e)
  {
   if (e.getSQLState().equals("02000"))
    return false;
   
   e.printStackTrace();
   return true;
  }
  return true;
 }
 
 public int getUpdateCount() throws SQLException
 {
  return s.getUpdateCount();
 }
 
 public ResultSet getResultSet() throws SQLException
 {
  return s.getResultSet();
 }
 public void close() throws SQLException
 {
  if (s != null)
   s.close();
 }
 
 public int getParameterCount()
 {
  return pcnt;
 }
 
 public Connection getConnection()
 {
  return m_conn;
 }
 
 private String escapeSql(String s)
 {
     int length = s.length();
     int newLength = length;
     // first check for characters that might
     // be dangerous and calculate a length
     // of the string that has escapes.
     for (int i=0; i<length; i++){
       char c = s.charAt(i);
       switch(c){
         case '\\':
         case '\"':
         case '\'':
         case '\0':{
           newLength += 1;
         } break;
       }
     }
     if (length == newLength){
       // nothing to escape in the string
       return s;
     }
     StringBuffer sb = new StringBuffer(newLength);
     for (int i=0; i<length; i++){
       char c = s.charAt(i);
       switch(c){
         case '\\':{
           sb.append("\\\\");
         } break;
         case '\"':{
           sb.append("\\\"");
         } break;
         case '\'':{
           sb.append("\\\'");
         } break;
         case '\0':{
           sb.append("\\0");
         } break;
         default: {
           sb.append(c);
         }
       }
     }
     return sb.toString();
  }
}
