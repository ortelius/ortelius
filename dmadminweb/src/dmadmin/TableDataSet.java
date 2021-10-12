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

import java.util.Hashtable;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;

public class TableDataSet
	implements IJSONSerializable
{
	private int m_cols;
	private int m_maxrow = -1;
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
