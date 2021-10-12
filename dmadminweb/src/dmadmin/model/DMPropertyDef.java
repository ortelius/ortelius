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
import dmadmin.json.JSONObject;

public class DMPropertyDef
	implements Serializable, IJSONSerializable
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 7703233628885066640L;

	private String m_name;
	private boolean m_required;
	private boolean m_appendable;

	public DMPropertyDef()
	{}
	
	public DMPropertyDef(String name, boolean required, boolean appendable)
	{
//System.out.println("new DMPropertyDef(\"" + name + "\"," + required + "," + appendable + ")");
		m_name = name;
		m_required = required;
		m_appendable = appendable;
	}
	
	public String getName() {
		return m_name;
	}
	
	public void setName(String name) {
		m_name = name;
	}
	
	public boolean isRequired() {
		return m_required;
	}
	
	public void setRequired(boolean required) {
		m_required = required;
	}
	
	public boolean isAppendable() {
		return m_appendable;
	}
	
	public void setAppendable(boolean appendable) {
		m_appendable = appendable;
	}
	
	private JSONObject toJSONObject() {
		JSONObject obj = new JSONObject();
		obj.add("name", m_name).add("reqd", m_required).add("append", m_appendable);
		return obj;
	}
	
	@Override
	public String getJSON() {
		return toJSONObject().getJSON();
	}

public JSONObject toJSON() {
   JSONObject obj = new JSONObject();
   obj.add( "name", getName());
   obj.add( "required", isRequired());
   obj.add( "appendable", isAppendable());
   return obj;
  }
}
