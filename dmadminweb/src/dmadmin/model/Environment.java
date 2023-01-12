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
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
				: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		ds.addProperty(SummaryField.AVAILABILITY, "Availability", m_calusage);
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
