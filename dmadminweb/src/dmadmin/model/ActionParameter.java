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

public class ActionParameter
{
	private String m_name;
	private String m_type;
	private boolean m_required;
	
	public ActionParameter(String name,String type) {
		m_name = name;
		m_type = type;
		m_required = false;
	}
	
	public ActionParameter(String name,String type,boolean required) {
		m_name = name;
		m_type = type;
		m_required = required;
	}
	
	public String getName() { return m_name; }
	public String getType() { return m_type; }
	public void setRequired(boolean required) { m_required = required; }
	public boolean isRequired() { return m_required; }
}
