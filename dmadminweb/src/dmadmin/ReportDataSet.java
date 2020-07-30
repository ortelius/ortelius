/*
 *
 *  Ortelius for Microservice Configuration Mapping
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

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;

public class ReportDataSet
{
	private class ReportColumn
	{
		private int m_col;
		private String m_name;
		private int m_type;
		
		public ReportColumn(int col, String name, int type)
		{
			m_col = col;
			m_name = name;
			m_type = type;
		}
		
		public Object get(ResultSet rs)
		{
			try {
				switch((m_col != 1) ? m_type : java.sql.Types.VARCHAR) {
				case java.sql.Types.BIGINT:
				case java.sql.Types.INTEGER:
					return rs.getInt(m_col);
				case java.sql.Types.VARCHAR:
					return rs.getString(m_col);
				default: System.out.println("Column type " + m_type + " not recognised"); break;
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
			return null;
		}
		
		public int col()  { return m_col; }
		public String name()  { return m_name; }
	}
	
	private class ReportRow
	{
		private Hashtable<Integer,Object> m_cols = new Hashtable<Integer,Object>();
		
		public ReportRow()
		{}
		
		public void setColumnValue(int col, Object value)
		{
			m_cols.put(col, value);
		}
		
		public Object getColumnValue(int col) {
			return m_cols.get(col);
		}
	}
	
	private List<ReportColumn> m_cols = new ArrayList<ReportColumn>();
	private Hashtable<String,ReportColumn> m_colNames = new Hashtable<String,ReportColumn>();
	private List<ReportRow> m_rows = new ArrayList<ReportRow>();
	
	public ReportDataSet(ResultSet rs,int maxrows)
	{
		int rowcount=0;
		try {
			setMetaData(rs.getMetaData());
			while(rs.next() && rowcount < (maxrows>0?maxrows:rowcount+1)) {
				addRow(rs);
				rowcount++;
			}
		} catch (SQLException e) {
			e.printStackTrace();
			throw new RuntimeException("Failed to handle ResultSet");
		}
	}
	
	public ReportDataSet(ResultSet rs)
	{
		this(rs,0);
	}
	
	public void setMetaData(ResultSetMetaData meta)
	{
		try {
			for(int c = 1; c <= meta.getColumnCount(); c++) {
				ReportColumn rc = new ReportColumn(c, meta.getColumnName(c), meta.getColumnType(c));
				m_cols.add(rc);
				m_colNames.put(rc.name().toUpperCase(), rc);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	public void addRow(ResultSet rs)
	{
		ReportRow row = new ReportRow();
		
		for(ReportColumn c: m_cols) {
			Object value = c.get(rs);
			if(value != null) {
				row.setColumnValue(c.col(), value);
			}
		}
		m_rows.add(0,row);
	}
	
	public IJSONSerializable getColJSON(String colname)
	{
		JSONArray ret = new JSONArray();
		ReportColumn c = m_colNames.get(colname.toUpperCase());
		if(c != null) {
			for(ReportRow r: m_rows) {
				Object value = r.getColumnValue(c.col());
				if(value != null) {
					ret.add(value);
				}
			}
		}
		return ret;
	}
	
	public IJSONSerializable getCatLabelsJSON()
	{
		ReportColumn c = m_cols.get(0);
		return (c != null) ? getColJSON(c.name()) : new JSONArray();
	}
	
	public IJSONSerializable getBarDataSeriesJSON()
	{
		JSONArray ret = new JSONArray();
		for(int n = 1; n < m_cols.size(); n++) {
			ReportColumn c = m_cols.get(n);
			ret.add((c != null) ? getColJSON(c.name()) : new JSONArray());
		}
		return ret;
	}
	
	/**
	 * Takes a set of rows and columns and make line data.
	 * First column is always the x-axis value - all subsequent columns form data series.
	 * Given columns a, b and c and rows 0, 1 and 2, the output is:
	 * [[[a0, b0], [a1, b1], [a2, b2]], [[a0, c0], [a1, c1], [a2, c2]]]
	 * 
	 */
	public IJSONSerializable getLineDataSeriesJSON()
	{
		JSONArray ret = new JSONArray();
		for(int n = 1; n < m_cols.size(); n++) {
			JSONArray arr = new JSONArray();
			for(ReportRow r: m_rows) {
				JSONArray arr2 = new JSONArray();
				ReportColumn c0 = m_cols.get(0);
				arr2.add((c0 != null) ? r.getColumnValue(c0.col()) : "");
				ReportColumn cn = m_cols.get(n);
				arr2.add((cn != null) ? r.getColumnValue(cn.col()) : "");
				arr.add(arr2);
			}
			ret.add(arr);
		}
		return ret;
	}
	
	public IJSONSerializable getTableDataJSON()
	{
		JSONArray ret = new JSONArray();
		for(int n = 0; n < m_rows.size(); n++) {
			JSONArray arr = new JSONArray();
			ReportRow r = m_rows.get(n);
			for(int m = 0; m < m_cols.size(); m++) {
				ReportColumn c = m_cols.get(m);
				Object value = (c != null) ? r.getColumnValue(c.col()) : null;
				ret.add((value != null) ? value : "");
			}
			ret.add(arr);
		}
		return ret;
		
//		StringBuffer ret = new StringBuffer();
//		ret.append("[");
//		boolean firstrow = true;
//		for(int n = 0; n < m_rows.size(); n++) {
//			if(!firstrow) {
//				ret.append(",");
//			}
//			ReportRow r = m_rows.get(n);
//			boolean firstcol = true;
//			ret.append("[");
//			for(int m = 0; m < m_cols.size(); m++) {
//				if(!firstcol) {
//					ret.append(",");
//				}
//				ReportColumn c = m_cols.get(m);
//				if(c != null) {
//					String value = r.getColumnValue(c.col());
//					if(value != null) {
//						ret.append(value);
//					}
//				}
//				firstcol = false;
//			}
//			ret.append("]");
//			firstrow = false;
//		}
//		ret.append("]");
//		return ret.toString();
	}
}
