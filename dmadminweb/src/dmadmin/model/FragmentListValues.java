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


public class FragmentListValues
{
	private int m_id;
	private String m_name;
	private boolean m_selected;

	
	public FragmentListValues() {
		m_id=0;
		m_name="";
		m_selected=false;
	}	

	public int getId() { return m_id; }
	public String getName() { return m_name; }
	public String getSelected() { return m_selected?"selected":""; }

	public void setName(String name) { m_name = name; }
	public void setSelected(boolean s) { m_selected = s; }
	public void setId(int id ) { m_id = id; }
}
