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

public class Builder
	extends ProviderObject
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1327862378913381548L;
	
	public Builder(DMSession sess, int id, String name) {
		super(sess, id, name);
	}

	@Override
	public String getDatabaseTable() {
		return "dm_buildengine";
	}

	@Override
	public String getForeignKey() {
		return "builderid";
	}

	/*
	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
				: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		addCreatorModifier(ds);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateBuilder(this, changes);
	}
	*/
	
	@Override
	public String getWriteTitle() {
		return "Start Build Access";
	}

	@Override
	public ObjectType getObjectType() {
		return ObjectType.BUILDER;
	}
}
