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
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

public class Domain
	extends DMObject
{
	private static final long serialVersionUID = 1327862378913381548L;

	private String m_parentDomain;
	private Engine m_engine;
	private boolean m_lifecycle;

	public Domain() {
	}

	public Domain(DMSession sess, int id, String name) {
		super(sess, id, name);
	}

	public Engine getEngine()  { return m_engine; }
	public void setEngine(Engine engine)  { m_engine = engine; }
	public Engine findNearestEngine() {
		if(m_engine != null) {
			return m_engine;
		}
		Domain parent = getDomain();
		if(parent != null) {
			return parent.findNearestEngine();
		}
		return null;
	}

	public String getFullDomain()
	{
	//	if (!m_session.ValidDomain(getId())) return "";

		return m_session.getFQDN(getId());

//		String fqdom = getName();
//		Domain parent = getDomain();
//		while (parent != null && m_session.ValidDomain(parent.getId())) {
//			fqdom  = parent.getName() + "." + fqdom;
//			if (parent.getId()==m_session.UserBaseDomain()) break;
//			parent = parent.getDomain();
//		}
//		return fqdom;
	}

	public void setLifecycle(boolean x) { m_lifecycle = x; }
	public boolean getLifecycle() { return m_lifecycle; }

	@Override
	public ObjectType getObjectType() {
		return ObjectType.DOMAIN;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_domain";
	}

	@Override
	public String getForeignKey() {
		return "domainid";
	}

	public void setParentDomain(String domain) {
		m_parentDomain = domain;
	}
	public String getParentDomain() {
		return m_parentDomain;
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "GLOBAL");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		ds.addProperty(SummaryField.NAME, "Name", getName());
//		ds.addProperty(SummaryField.READ_ONLY, "Parent Domain", new ReadOnlyTextField(m_parentDomain));
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
				: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		addCreatorModifier(ds);
		ds.addProperty(SummaryField.ENGINE_HOSTNAME, "Engine", (m_engine != null) ? m_engine.getLinkJSON() : null);

		Domain parent = this.getDomain();
		if (parent != null) {
			if (!parent.m_lifecycle) {
				ds.addProperty(SummaryField.DOMAIN_LIFECYCLE, "LifeCycle Domain", new BooleanField(false));  // remove lifecyle domains BooleanField(m_lifecycle) so everything is a plain domain
			}
		} else {
			ds.addProperty(SummaryField.DOMAIN_LIFECYCLE, "LifeCycle Domain", new BooleanField(false));  // remove lifecyle domains BooleanField(m_lifecycle) so everything is a plain domai
		}
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateDomain(this, changes);
	}

	@Override
 public IJSONSerializable getLinkJSON() {
  return new LinkField(getObjectType(), m_id, this.getFullDomain());
 }
}
