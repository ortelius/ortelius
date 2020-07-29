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

import java.util.ArrayList;
import java.util.List;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONBoolean;
import dmadmin.json.JSONObject;

public class PropertyDataSet
{
	private List<SummaryField> m_fields = new ArrayList<SummaryField>();
	private List<String> m_names = new ArrayList<String>();
	private List<Object> m_values = new ArrayList<Object>();
	private ObjectTypeAndId m_newObject;
	
	public PropertyDataSet()
	{}
	
	public void addProperty(String name, String value)
	{
		addProperty(SummaryField.READ_ONLY, name, value);
	}
	
	public void copyOnlyDeploy(PropertyDataSet ds)
	{
		for (int i=0;i<m_names.size();i++) {
			String v = (String)m_values.get(i);
			if (v.startsWith("Deploy")) {
				// Only add the deploy task if it hasn't already been added. Reason is, we're checking
				// both application and appversion and if we click on a domain the same deploy task
				// will appear in both lists.
				boolean foundit=false;
				for (int x=0;x<i;x++) {
					String v2 = (String)m_values.get(x);
					System.out.println("Comparing "+v2+" with "+v);
					if (v2.equalsIgnoreCase(v)) foundit=true;
				}
				if (!foundit) ds.addProperty(m_fields.get(i), m_names.get(i),(String)m_values.get(i));
			}
		}
	}
	
 public void removeDupDeploy(PropertyDataSet ds)
 {
  boolean addedDeploy = false;
  for (int i=0;i<m_names.size();i++) {
   String v = (String)m_values.get(i);
   if (v.startsWith("Deploy")) 
   {
    if (!addedDeploy)
    { 
     ds.addProperty(m_fields.get(i), m_names.get(i),(String)m_values.get(i));
     addedDeploy = true;
    }
   }
   else
    ds.addProperty(m_fields.get(i), m_names.get(i),(String)m_values.get(i));
  }
 }
	
	public void addProperty(SummaryField field, String name, String value)
	{

//	 if (m_names.contains(name))
//	  return;
	 
		m_fields.add(field);
		m_names.add(name);
		m_values.add(value);
	}
	
	public void addProperty(String name, int value)
	{	
		addProperty(SummaryField.READ_ONLY, name, value);
	}
	
	public void addProperty(SummaryField field, String name, int value)
	{	
		m_fields.add(field);
		m_names.add(name);
		m_values.add(value);
	}
	
	public void addProperty(String name, IJSONSerializable value)
	{
		addProperty(SummaryField.READ_ONLY, name, value);
	}
	
	public void addProperty(SummaryField field, String name, IJSONSerializable value)
	{	
		m_fields.add(field);
		m_names.add(name);
		m_values.add(value);
	}
	
	public ObjectTypeAndId getNewObject()  { return m_newObject; }
	public void setNewObject(ObjectTypeAndId obj)  { m_newObject = obj; }
	
	public IJSONSerializable getJSON()
	{
		JSONArray ret = new JSONArray();
		for(int n = 0; n < m_names.size(); n++) {
			JSONArray entry = new JSONArray(true);
			SummaryField field = m_fields.get(n);
			if(field == null) {
				throw new RuntimeException("No field type specified for summary entry " + n);
			}
			ObjectType type = field.type();
			entry.add(field.value());
			entry.add(new JSONBoolean(false));
			entry.add((type != null) ? type.getTypeString() : null);
			entry.add(m_names.get(n));
			entry.add(m_values.get(n));
			ret.add(entry);
		}
		return ret;
	}
	
	public IJSONSerializable getStepJSON()
	{
		JSONArray ret = new JSONArray();
		for(int n = 0; n < m_names.size(); n++) {
			JSONArray entry = new JSONArray(true);
			SummaryField field = m_fields.get(n);
			if(field == null) {
				throw new RuntimeException("No field type specified for summary entry " + n);
			}
			entry.add(m_names.get(n));
			entry.add(m_values.get(n));
			ret.add(entry);
		}
		return ret;
	}
	
	public IJSONSerializable getNewsJSON()
	{
		JSONObject ret = new JSONObject(true);
		for(int n = 0; n < m_names.size(); n++) {
			ret.add(m_names.get(n), m_values.get(n));
		}
		return ret;
	}
	
	public IJSONSerializable getMenuJSON()
	{
		JSONArray ret = new JSONArray();
		for(int n = 0; n < m_names.size(); n++) {
			JSONObject obj = new JSONObject();
			obj.add(m_names.get(n), m_values.get(n));
			ret.add(obj);
		}
		return ret;
	}
}
