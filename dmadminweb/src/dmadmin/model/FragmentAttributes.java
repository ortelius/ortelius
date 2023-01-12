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
