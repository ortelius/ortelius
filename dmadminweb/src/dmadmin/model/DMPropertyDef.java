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
}
