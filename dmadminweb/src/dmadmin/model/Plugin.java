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
