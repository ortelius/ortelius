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
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

public class Datasource
	extends ProviderObject
{
	private static final long serialVersionUID = -1569186922421732870L;

	public Datasource(DMSession sess, int id, String name)
	{
		super(sess, id, name);
	}

	@Override
	public ObjectType getObjectType() {
		return ObjectType.DATASOURCE;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_datasource";
	}

	@Override
	public String getForeignKey() {
		return "datasourceid";
	}

	@Override
	public boolean hasReadWrite() {
		return true;
	}

	@Override
	public String getReadTitle() {
		return "Select Access";
	}

	@Override
	public String getWriteTitle() {
		return "Delete Access";
	}
	@Override
	public IJSONSerializable getLinkJSON() {
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
