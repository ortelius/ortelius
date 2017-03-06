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

public class Notify
	extends ProviderObject
{
	private static final long serialVersionUID = -1569186922421732870L;
	
	private String m_testrecipient;
	
	public void setTestRecipient(String rcpt) {m_testrecipient = rcpt;}
	public String getTestRecipient() { return m_testrecipient; }

	public Notify(DMSession sess, int id, String name)
	{
		super(sess, id, name);
	}
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.NOTIFY;
	}
	
	@Override
	public String getDatabaseTable() {
		return "dm_notify";
	}

	@Override
	public String getForeignKey() {
		return "notifyid";
	}

	@Override
	public boolean hasReadWrite() {
		return true;
	}

	@Override
	public String getReadTitle() {
		return "Send Access";
	}

	@Override
	public String getWriteTitle() {
		return null;
	}
}
