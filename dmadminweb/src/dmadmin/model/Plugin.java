/*
 *
 *  Ortelius for Microservice Configuration Mapping
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
import dmadmin.SummaryChangeSet;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;

public class Plugin
	extends DMObject
	implements IJSONSerializable
{
	private static final long serialVersionUID = 6700786198966622875L;

	private String m_version;
	
	public Plugin() {
	}
	
	public Plugin(DMSession sess, int id, String library) {
		super(sess, id, library);
	}
	
	public String getVersion()  { return m_version; }
	public void setVersion(String version)  { m_version = version; }
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.PLUGIN;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_plugin";
	}

	@Override
	public String getForeignKey() {
		return "pluginid";
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		return null;
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public String getJSON() {
		return new JSONObject()
			.add("id", getId())
			.add("library", getName())
			.add("version", m_version)
			.getJSON();
	}
}
