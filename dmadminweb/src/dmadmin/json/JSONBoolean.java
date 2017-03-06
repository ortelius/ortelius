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

/**
 * Magic class to allow a all-string array or object to explicitly output boolean values
 * @author Robert
 */
public class JSONBoolean
	implements IJSONSerializable
{
	private boolean m_value;
		
	public JSONBoolean(boolean value)
	{
		m_value = value;
	}

	@Override
	public String getJSON()
	{
		return m_value ? "true" : "false";
	}
	
	@Override
	public String toString()
	{
		return getJSON();
	}
}
