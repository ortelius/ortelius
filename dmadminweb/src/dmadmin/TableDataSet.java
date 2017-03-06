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

import java.util.Hashtable;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;

public class TableDataSet
	implements IJSONSerializable
{
	private int m_cols;
	private int m_maxrow = 0;
	private Hashtable<Integer, Object> m_values = new Hashtable<Integer, Object>();

	public TableDataSet(int cols)
	{
		m_cols = cols;
	}
	
	public TableDataSet put(int row, int col, Object value)
	{
		if((col < 0) || (col >= m_cols)) {
			throw new RuntimeException("Column number out of range 0 < " + col + " < " + m_cols);
		}
		m_values.put((row * m_cols) + col, value);
		if(row > m_maxrow) {
			m_maxrow = row;
		}
		return this;
	}
	
	public int getCols() { return m_cols; }
	public int getRows() { return m_maxrow; }
	
	public Object get(int row, int col) {
		return m_values.get((row * m_cols) + col);
	}

	
	public JSONArray getJSONArray()
	{
		JSONArray ret = new JSONArray();
		for(int r = 0; r <= m_maxrow; r++) {
			JSONArray row = new JSONArray();
			for(int c = 0; c < m_cols; c++) {
				Object value = m_values.get((r*m_cols) + c);
				//System.out.println("("+r+","+c+") = " + value);
				row.add(value);
			}
			ret.add(row);
		}
		return ret;
	}
	
	@Override
	public String getJSON()
	{
		return getJSONArray().getJSON();
	}
}
