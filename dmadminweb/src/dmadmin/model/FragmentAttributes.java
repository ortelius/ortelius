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
package dmadmin.model;

import java.util.List;

public class FragmentAttributes
{
	private int m_attrid;
	private String m_attname;
	private String m_attval;
	private String m_atttype;
	private String m_tabname;
	private boolean m_inherit;
	private boolean m_required;
	private String DefaultValue;
	private List<FragmentListValues> m_flv;
	
	public FragmentAttributes() {
		m_attrid=0;
		m_attname="";
		m_attval="";
		m_atttype="";
		m_tabname="";
		m_inherit=false;
		m_required=false;
	}
	
	public int getAttrId() { return m_attrid; }
	public String getAttrName() { return m_attname; }
	public String getAttrVal() { return m_attval.replace("'", "&#146;"); }
	public String getAttrType() { return m_atttype; }
	public String getTableName() { return m_tabname; }
	public boolean getInherit() { return m_inherit; }
	public boolean isRequired() { return m_required; }
	public List<FragmentListValues> getFragmentListValues() { return m_flv; }
	
	public void setAttrId(int attrid) { m_attrid = attrid; }
	public void setAttrName(String attname) { m_attname = attname; }
	public void setAttrVal(String attval) { m_attval = attval; }
	public void setAttrType(String atttype) { m_atttype = atttype; }
	public void setTableName(String tabname) { m_tabname = tabname; }
	public void setInherit(boolean inherit) { m_inherit = inherit; }
	public void setRequired(boolean required) { m_required = required; }
	public void setFragmentListValues(List<FragmentListValues>flv) { m_flv = flv; }

 public String getDefaultValue()
 {
  return DefaultValue;
 }

 public void setDefaultValue(String defaultValue)
 {
  DefaultValue = defaultValue;
 } 
	
}
