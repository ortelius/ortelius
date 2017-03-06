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
import dmadmin.json.JSONBoolean;

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
}
