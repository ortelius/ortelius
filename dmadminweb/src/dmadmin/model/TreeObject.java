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
 private int m_domainid;

	public TreeObject() {
		m_objectKind=0;
		m_domainid = 1;
	}

 public ArrayList<Category> GetCategories() {
  return this.m_categories;
}

public void SetCategories(ArrayList<Category> categories) {
  this.m_categories = categories;
}

	public TreeObject(int id,String name, int domainid) {
		super.setId(id);
		super.setName(name);
		m_objectKind=0;
		m_domainid = domainid;
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

 public int GetDomainId() {
  return m_domainid;
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
