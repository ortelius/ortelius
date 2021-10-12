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
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.List;

/**
 * Makes building dynamic queries easy and removes errors from having parameters not match their positions
 * @author Robert
 *
 */
public class DynamicQueryBuilder
{
	private Connection m_conn;
	private StringBuffer m_sql;
	private List<Object> m_params;
	private PreparedStatement m_stmt;
	
	public enum Null {
		INT,
		STRING
	}
	
	public DynamicQueryBuilder(Connection conn, String baseSql)
	{
		m_conn = conn;
		
		if(countParams(baseSql) != 0) {
			throw new RuntimeException("Base SQL Fragmnent must not contain params");
		}
 		m_sql = new StringBuffer(baseSql);
		m_params = new ArrayList<Object>();
	}

	public void add(String sqlFrag)
	{
		add(0, sqlFrag, null, null, null);
	}

	public void add(String sqlFrag, Object param1)
	{
		add(1, sqlFrag, param1, null, null);
	}

	public void add(String sqlFrag, Object param1, Object param2)
	{
		add(2, sqlFrag, param1, param2, null);
	}

	public void add(String sqlFrag, Object param1, Object param2, Object param3)
	{
		add(3, sqlFrag, param1, param2, param3);
	}
	
	private void add(int count, String sqlFrag, Object param1, Object param2, Object param3)
	{
	 int pcnt = 0;
	 
	 if (param1 != null)
	  pcnt++;
	 
  if (param2 != null)
   pcnt++;
  
  if (param3 != null)
   pcnt++;
  	 
		if(countParams(sqlFrag) != count || pcnt != count) {
			throw new RuntimeException("SQL Fragmnent: " + sqlFrag + " contains wrong number of params (" + countParams(sqlFrag) + "!=" + count + ")");
		}
		m_sql.append(sqlFrag);
		if(count > 0) {
			m_params.add(param1);			
		}
		if(count > 1) {
			m_params.add(param2);			
		}
		if(count > 2) {
			m_params.add(param3);			
		}
	}
	
	private void createStatement()
		throws SQLException
	{
		String sql = m_sql.toString();
		
		if(countParams(sql) != m_params.size()) {
			throw new RuntimeException("Number of paramters does not match SQL statement");
		}
		
		//System.out.println(sql);
		
		m_stmt = m_conn.prepareStatement(sql);
		for(int n = 0; n < m_params.size(); n++) {
			Object param = m_params.get(n);
			if(param == null) {
				throw new RuntimeException("null is not a valid parameter - use Null.INT or Null.STRING");
			} else if(param instanceof Null) {
				//System.out.println(n + ": null");
				switch((Null) param) {
				case INT: m_stmt.setNull(n+1, Types.INTEGER); break;
				case STRING: m_stmt.setNull(n+1, Types.VARCHAR); break;
				}
			} else if(param instanceof Integer) {
				//System.out.println(n + ": " + (Integer) param);
				m_stmt.setInt(n+1, (Integer) param);
			} else if(param instanceof Long) {
				//System.out.println(n + ": " + (Long) param);
				m_stmt.setLong(n+1, (Long) param);
			} else if(param instanceof String) {
				//System.out.println(n + ": '" + (String) param + "'");
				m_stmt.setString(n+1, (String) param);				
			} else {
				throw new RuntimeException("Invalid parameter type " + param.getClass().getName());
			}
		}
		System.out.println("Dyn:" + m_stmt.toString());
	}
	
	public ResultSet executeQuery()
		throws SQLException
	{
		createStatement();
		try {
			return m_stmt.executeQuery();
		} catch(SQLException e) {
			System.err.println(m_sql);
			throw e;
		}
	}
	
	public void execute()
		throws SQLException
	{
		createStatement();
		System.out.println("Calling execute");
		m_stmt.execute();		
		System.out.println("Done");
	}
	
	public String getQueryString()
	{
		return m_sql.toString();
	}
	
 public ArrayList<Object> getQueryParams()
 {
  return (ArrayList<Object>) m_params;
 }
 
	public int getUpdateCount()
		throws SQLException
	{
		return (m_stmt != null) ? m_stmt.getUpdateCount() : -1;
	}
	
	public void close()
		throws SQLException
	{
		if(m_stmt != null) {
			m_stmt.close();
		}
	}
	
	private int countParams(String str)
	{
		int count = 0;
		for(int n = 0; n < str.length(); n++) {
			if(str.charAt(n) == '?') {
				count++;
			}
		}
		return count;
	}
}
