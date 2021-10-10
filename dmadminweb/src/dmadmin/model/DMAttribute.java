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

package dmadmin.model;

import java.io.Serializable;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;

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
	 
	 if (name != null)
	  name = name.trim();
	 
	 if (value != null)
	  value = value.trim();
	 
		m_name = name;
		m_value = value;
		m_arrayId = 0;
		m_key = "";
	}
	
	public DMAttribute(String name, int arrid, String key, String value) {
  if (name != null)
   name = name.trim();
  
  if (value != null)
   value = value.trim();
  
  if (key != null)
   key = key.trim();
	 
		m_name = name;
		
		if (name.contains("["))
		{
		 String[] parts = name.split("\\[|\\]");
		 m_name = parts[0];
		 m_key = parts[1].replaceAll("^[\"']+|[\"']+$", "");
		 m_value = value;
   m_arrayId = arrid;
		}
		else
		{
		 m_key = key;
		 m_value = value;
		 m_arrayId = arrid;
		}
	}
	
	public String getName()  { return m_name; }
	public void setName(String name)  
	{
  if (name != null)
   name = name.trim();
	 
  if (name.contains("["))
  {
   String[] parts = name.split("\\[|\\]");
   m_name = parts[0];
   m_key = parts[1].replaceAll("^[\"']+|[\"']+$", "");;
  }
  else
	  m_name = name; 
	}
	
 public String getKey()  { return m_key; }
 
 public void setKey(String key)  { 
  if (key != null)
  key = key.trim();
  
  m_key = key; 
 }
 
	public String getValue()  { return m_value; }
	public void setValue(String value)  { 
  if (value != null)
  value = value.trim();
  
	 m_value = value; 
	}
	
	public boolean isArray() {
	 if (m_key != null && m_key.length() > 0)
	  return true;
	 
		return (m_arrayId > 0);
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
	
 public JSONObject toJSON() {
  JSONObject obj = new JSONObject();
  if (isArray())
   obj.add( m_name + "['"+ m_key + "']", m_value);
  else
   obj.add(m_name, m_value);
  return obj;
 }
 
	
	@Override
	public String getJSON() {
		return toJSONArray().getJSON();
	}
	
}
