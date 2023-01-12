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

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;
import dmadmin.json.LinkField;

public class ProviderDefinition
	extends DMObject
{
	private static final long serialVersionUID = 2148695895455356563L;

	public static final int UNCONFIGURED_ID = 0;

	private ObjectType m_kind;

	public ProviderDefinition() {
	}

	public ProviderDefinition(DMSession sess, int id, String name) {
		super(sess, id, name);
	}

	public ObjectType getKind()  { return m_kind; }
	public void setKind(ObjectType kind)  { m_kind = kind; }

	@Override
	public ObjectType getObjectType() {
		return ObjectType.PROVIDERDEF;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_providerdef";
	}

	@Override
	public String getForeignKey() {
		return "defid";
	}

	@Override
	public IJSONSerializable getLinkJSON() {
		JSONObject obj = new LinkField(getObjectType(), m_id, m_name).getJSONObject();
		obj.add("kind", (m_kind != null) ? m_kind.getTypeString() : null);
		return obj;
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

	public PropertyDataSet getDetailsForEngine(Engine engine) {
		return m_session.getProviderDefDetails(this, engine);
	}
}
