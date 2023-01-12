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
import dmadmin.json.LinkField;

public class NotifyTemplate
	extends DMObject
{
	private static final long serialVersionUID = 1327862378913381548L;
	private String m_subject;
	private String m_body;
	private int m_notifierid;

	private void init() {
	}

	public NotifyTemplate() {
		init();
	}

	public NotifyTemplate(DMSession sess, int id, String name) {
		super(sess, id, name);
		init();
	}

	public NotifyTemplate(DMSession sess, int id, String name, int domainid) {
		super(sess, id, name);
		setDomainId(domainid);
		init();
	}

	public void setSubject(String s) {
		m_subject = s;
	}

	public void setBody(String b) {
		m_body = b;
	}

	public void setNotifierId(int notifierid) {
		m_notifierid = notifierid;
	}

	public String getSubject() { return m_subject; }
	public String getBody() { return m_body; }
	public int getNotifierId() { return m_notifierid; }

	@Override
	public ObjectType getObjectType() {
		return ObjectType.TEMPLATE;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_template";
	}

	@Override
	public String getForeignKey() {
		return "templateid";
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();

		Notify notifier = null;

		try
		{
   notifier = m_session.getNotify(getNotifierId(),true);
		}
		catch (RuntimeException e)
		{
		}

  if (notifier == null)
    ds.addProperty(SummaryField.NOTIFIER, "Notifier", "");
  else
   ds.addProperty(SummaryField.NOTIFIER, "Notifier", notifier.getDomain().getFullDomain() + "." + notifier.getName());
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		addCreatorModifier(ds);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateTemplate(this, changes);
	}

	@Override
	public boolean hasReadWrite() {
		return false;
	}

	@Override
	public IJSONSerializable getLinkJSON() {
		System.out.println("in getLinkJSON for TEMPLATE object domainid="+getDomainId());
		Domain d = m_session.getDomain(getDomainId());
		String name = m_name;
		if (d!=null) {
			String fd = d.getFullDomain();
			if (fd.length()>0) name = fd+"."+m_name;
		}
		boolean showLink=m_session.ValidDomain(getDomainId());
		return new LinkField(getObjectType(), m_id, name , showLink);
	}
}
