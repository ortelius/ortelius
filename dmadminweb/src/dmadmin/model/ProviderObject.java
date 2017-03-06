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

import java.util.List;

import dmadmin.DMSession;
import dmadmin.ACDChangeSet;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;

public abstract class ProviderObject
	extends DMObject
{
	private static final long serialVersionUID = -8147651036390520017L;
		
	private ProviderDefinition m_def;
	private Credential m_cred;

	public ProviderObject(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public List<DMProperty> getProperties() {
		return m_session.getPropertiesForProviderObject(this);
	}
	
	public ProviderDefinition getDef() {
		if(m_def == null) {
			m_def = m_session.getProviderDefForProviderObject(this);
		}
		if(m_def == null) {
			throw new RuntimeException("Unable to retrieve provider definition for " + getObjectType() + " " + getId());
		}
		return m_def;
	}
	public void setDef(ProviderDefinition def)  {  m_def = def; }
	
	public boolean isTypeUnconfigured() {
		return (getDef().getId() == 0);
	}

	public List<DMPropertyDef> getPropertyDefs() {
		return m_session.getPropertyDefsForProviderDef(getDef());
	}
	
	public Credential getCredential()  { return m_cred; }
	public void setCredential(Credential cred)  { m_cred = cred; }
	
	@Override
	public IJSONSerializable getSummaryJSON() {
		System.out.println("Getting Summary JSON for id "+m_id);
		PropertyDataSet ds = new PropertyDataSet();
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.PROVIDER_TYPE, "Type", (m_id != -1)?getDef().getLinkJSON() : null);
		ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
				: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		addCreatorModifier(ds);
		ds.addProperty(SummaryField.PROVIDER_CRED, "Credential", (m_cred != null) ? m_cred.getLinkJSON() : null);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateProviderObject(this, changes);
	}
	
	public boolean updateProperties(ACDChangeSet<DMProperty> changes) {
		return m_session.updateProviderProperties(this, changes);
	}
}
