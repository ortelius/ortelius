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

import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

public class Category  extends DMObject
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -4129395953664594214L;
	private int m_catid;
	private String m_catname;
	private List<Fragment> m_fragments;
	
	public static final Category NO_CATEGORY = new Category(10, "General");
	
	public Category(int id,String name) {
		m_catid=id;
		m_catname=name;
	}
	
	public int getId() { return m_catid; }
	public String getName() { return m_catname; }
	public List<Fragment> getFragments() { return m_fragments; }
	
	public void setId(int catid) { m_catid = catid; }
	public void setName(String catname) { m_catname = catname; }
	public void setFragments(List<Fragment>f) { m_fragments = f; }
	
	public IJSONSerializable getLinkJSON() {
		return new LinkField(ObjectType.ACTION_CATEGORY, m_catid, m_catname, false);
	}

 @Override
 public ObjectType getObjectType()
 {
  // TODO Auto-generated method stub
  return null;
 }

 @Override
 public String getDatabaseTable()
 {
  // TODO Auto-generated method stub
  return null;
 }

 @Override
 public String getForeignKey()
 {
  // TODO Auto-generated method stub
  return null;
 }

 @Override
 public IJSONSerializable getSummaryJSON() {
  PropertyDataSet ds = new PropertyDataSet();

  ds.addProperty(SummaryField.NAME, "Name", getName());
  return ds.getJSON();
 }

 @Override
 public boolean updateSummary(SummaryChangeSet changes)
 {
  // TODO Auto-generated method stub
  return false;
 }
}
