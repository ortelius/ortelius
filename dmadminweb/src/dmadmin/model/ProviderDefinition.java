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
