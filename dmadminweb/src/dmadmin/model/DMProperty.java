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
import dmadmin.json.JSONBoolean;
import dmadmin.json.JSONObject;

public class DMProperty
	implements Serializable, IJSONSerializable
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 7703233628885066640L;

	private String m_name;
	private String m_value;
	private boolean m_encrypted;
	private boolean m_overridable;
	private boolean m_appendable;
	private boolean m_addedRequired;

	public DMProperty()
	{}
	
	public DMProperty(String name, String value, boolean encrypted, boolean overridable, boolean appendable)
	{
		this(name, value, encrypted, overridable, appendable, false);
	}
	
	public DMProperty(String name, String value, boolean encrypted, boolean overridable, boolean appendable, boolean addreqd)
	{
		m_name = name;
		m_value = value;
		m_encrypted = encrypted;
		m_overridable = overridable;
		m_appendable = appendable;
		m_addedRequired = addreqd;
	}
	
	public String getName()  { return m_name; }
	public void setName(String name)  { m_name = name; }
	
	public String getValue()  { return m_value; }
	public void setValue(String value)  { m_value = value; }

	public boolean isEncrypted()  { return m_encrypted; }

	public boolean isOverridable()  { return m_overridable; }

	public boolean isAppendable()  { return m_appendable; }
	
 public boolean isRequired()  { return m_addedRequired ; }
	
	private JSONArray toJSONArray() {
		return new JSONArray()
			.add(new JSONBoolean(false))			// dirty
			.add(new JSONBoolean(m_addedRequired)) 	// added - set on required properties that we add automatically
			.add(new JSONBoolean(false))			// deleted
			.add(m_name).add(m_encrypted ? "********" : m_value)
			.add(m_encrypted).add(m_overridable).add(m_appendable);
	}
	
	@Override
	public String getJSON() {
		return toJSONArray().getJSON();
	}

 public JSONObject toJSON()
 {
  JSONObject obj = new JSONObject();
  obj.add("name", getName());
  obj.add("value", getValue());
  obj.add("encrypted", new JSONBoolean(isEncrypted()));
  obj.add("overridable", new JSONBoolean(isOverridable())); 
  obj.add("appendable", new JSONBoolean(isAppendable())); 
  
  return obj;
 }
}
