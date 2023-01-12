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

import dmadmin.ACDChangeSet;
import dmadmin.DMSession;
import dmadmin.ObjectType;
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

  if (m_def.getName().equalsIgnoreCase("unconfigured"))
  {
   List<ProviderDefinition> deflist = m_session.getProviderDefinitionsOfType(getObjectType());
   if (deflist != null)
   {
    for (int i=0;i < deflist.size(); i++)
    {
     if (getObjectType() == ObjectType.DATASOURCE && deflist.get(i).getName().equalsIgnoreCase("github"))
     {
      m_def = deflist.get(i);
      break;
     }
    }
   }
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
	 ProviderDefinition def = getDef();
		return m_session.getPropertyDefsForProviderDef(def);
	}

	public Credential getCredential()  { return m_cred; }
	public void setCredential(Credential cred)  { m_cred = cred; }

	@Override
	public IJSONSerializable getSummaryJSON() {
		System.out.println("Getting Summary JSON for id "+m_id);
		PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
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
		return m_session.updateProviderProperties(this, changes, false);
	}

 public boolean updateProperties(ACDChangeSet<DMProperty> changes, boolean deleteAll) {
  return m_session.updateProviderProperties(this, changes, deleteAll);
 }
}
