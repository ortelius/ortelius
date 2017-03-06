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

public class TaskParameter
{
	private int m_pos;
	private String m_label;
	private String m_var;
	private String m_type;
	private String m_arr;
	
	public TaskParameter(int pos,String label,String var,String type) {
		m_pos=pos;
		m_label=label;
		m_var = var;
		m_type = type;
		m_arr = null;
	}
	
	public TaskParameter(int pos,String label,String var,String type,String arr) {
		m_pos=pos;
		m_label=label;
		m_var = var;
		m_type = type;
		m_arr = arr;
	}
	
	public int getPos() { return m_pos; }
	public String getLabel() { return m_label; }
	public String getVar() { return m_var; }
	public String getType() { return m_type; }
	public String getArr() { return m_arr; }
}
