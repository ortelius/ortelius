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
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;

public class Environment
	extends DMObject
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	private int m_calstart;
	private int m_calend;
	private String m_calusage;
	
	private void init() {
		m_calstart=0;	// midnight
		m_calend=86399;	// 23:59:59
		m_calusage="E";	// Available (E)xcept for Calendar Entries
	}
	public Environment() {
		init();
	}
	
	public Environment(DMSession sess, int id, String name) {
		super(sess, id, name);
		init();
	}
	
	public int getCalStart() { return m_calstart; }
	public int getCalEnd()   { return m_calend; }
	public String getCalUsage() { return m_calusage; }
	
	public void setCalStart(int cs) { m_calstart = cs; }
	public void setCalEnd(int ce) { m_calend = ce; }
	public void setCalUsage(String cu) { m_calusage = cu; }
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.ENVIRONMENT;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_environment";
	}

	@Override
	public String getForeignKey() {
		return "envid";
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
				: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		addCreatorModifier(ds);
		addAttachments(ds);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateEnvironment(this, changes);
	}
	
	@Override
	public String getWriteTitle() {
		return "Deploy Access";
	}
	
	@Override
	public String getReadTitle() {
		return "Create Calendar Entries";
	}
}
