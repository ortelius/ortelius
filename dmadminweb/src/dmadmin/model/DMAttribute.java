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
package dmadmin.model;

import java.io.Serializable;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;

public class DMAttribute
	implements Serializable, IJSONSerializable
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 8820282733974373811L;

	private String m_name;
	private String m_value;
	private int m_arrayId;
	private String m_key;

	public DMAttribute()
	{}
	
	public DMAttribute(String name, String value) {
		m_name = name;
		m_value = value;
		m_arrayId = 0;
		m_key = "";
	}
	
	public DMAttribute(String name, int arrid, String key, String value) {
		m_name = name;
		m_key = key;
		m_value = value;
		m_arrayId = arrid;
	}
	
	public String getName()  { return m_name; }
	public void setName(String name)  { m_name = name; }
	
 public String getKey()  { return m_key; }
 public void setKey(String key)  { m_key = key; }
 
	public String getValue()  { return m_value; }
	public void setValue(String value)  { m_value = value; }
	
	public boolean isArray() {
		return (m_arrayId != 0);
	}
	
	public int getArrayId()  { return m_arrayId; }
	public void setArrayId(int arrayId)  { m_arrayId = arrayId; }
	
	private JSONArray toJSONArray() {
		JSONArray arr = new JSONArray();
		arr.add(false)	// dirty
			.add(false)	// added
			.add(false)	// deleted
			.add(isArray() ? m_name + "[\""+ m_key + "\"]" : m_name)
			.add(m_value)
			.add(isArray() ? m_arrayId : null);
		return arr;
	}
	
	@Override
	public String getJSON() {
		return toJSONArray().getJSON();
	}
}
