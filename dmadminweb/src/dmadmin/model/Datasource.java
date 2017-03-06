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
		return "Update/Delete Access";
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
