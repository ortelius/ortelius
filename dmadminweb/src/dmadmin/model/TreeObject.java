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

import java.util.ArrayList;

import dmadmin.ObjectType;
import dmadmin.ObjectTypeAndId;
import dmadmin.SummaryChangeSet;
import dmadmin.json.IJSONSerializable;

public class TreeObject
	extends DMObject
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	private ObjectType m_objectType;
	private int m_objectKind;
 private ArrayList<Category> m_categories;
	
	public TreeObject() {
		m_objectKind=0;
	}
	
 public ArrayList<Category> GetCategories() {
  return this.m_categories;
}

public void SetCategories(ArrayList<Category> categories) {
  this.m_categories = categories;
}

	public TreeObject(int id,String name) {
		super.setId(id);
		super.setName(name);
		m_objectKind=0;
	}
	
	public void SetObjectType(ObjectType ot) {
		m_objectType = ot;
	}
	
	public ObjectType GetObjectType() {
		return m_objectType;
	}
	
	public void SetObjectKind(int kindid) {
		m_objectKind = kindid;
	}
	
	public int GetObjectKind() {
		return m_objectKind;
	}
	
	public ObjectTypeAndId GetOTID() {
		return new ObjectTypeAndId(m_objectType,super.getId());
	}
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.DOMAIN;
	}

	@Override
	public String getDatabaseTable() {
		return null;
	}

	@Override
	public String getForeignKey() {
		return null;
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		return null;
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		// TODO Auto-generated method stub
		return false;
	}
}
