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
package dmadmin.json;

public class BooleanField
	implements IJSONSerializable
{
	private boolean m_value;
	private boolean m_readOnly;

	public BooleanField(boolean value) {
		this(value, false);
	}
	
	public BooleanField(boolean value, boolean readOnly) {
		m_value = value;
		m_readOnly = readOnly;
	}

	@Override
	public String getJSON() {
		return new JSONObject()
			.add("type", "bb")
			.add("value", m_value)
			.add("readonly", m_readOnly)
			.getJSON();
	}
}
