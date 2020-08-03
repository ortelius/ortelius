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
package dmadmin.json;

import dmadmin.ObjectType;
import dmadmin.model.User;

public class CreatedModifiedField
	extends LinkField
{
	private String m_when;

	public CreatedModifiedField(String when, User user) {
		super(ObjectType.MODIFIED, user.getId(), user.getName(), user.getRealName());
		m_when = when;
	}

	@Override
	public String getJSON() {
		JSONObject obj = super.getJSONObject();
		obj.add("when", m_when);
		obj.add("readonly", true);
		return obj.getJSON();
	}
}
