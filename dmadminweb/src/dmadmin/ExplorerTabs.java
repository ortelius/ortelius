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

package dmadmin;

public class ExplorerTabs implements java.io.Serializable {
	private static final long serialVersionUID = -5362995729983900561L;
	private int m_TabNumber;
	private String m_Admin;
	private String m_TabName;
	private String m_TypeStr;
	private String m_FrameName;
	
	public ExplorerTabs() {
		m_TabNumber = 0;
		m_TabName = "";
		m_TypeStr = "";
		m_FrameName = "";
    }
	
	public ExplorerTabs(int id, int parentId, String tabName, String typeStr, String admin) {
		m_TabNumber = id;
		m_TabName = tabName;
		m_TypeStr = typeStr;
		m_FrameName = "mainframe" + parentId;
		m_Admin = admin;
    }
	
	public void setTabNumber(int tabno) {
		m_TabNumber = tabno;
	}
	public void setAdmin(String admin) {
		m_Admin = admin;
	}
	public void setTabName(String tabName) {
		m_TabName = tabName;
	}
	public void setTypeStr(String typestr) {
		m_TypeStr = typestr;
	}
	public void setFrameName(String fn) {
		m_FrameName = fn;
	}
	
	public int getTabNumber() {
		return m_TabNumber;
	}
	public String getAdmin() {
		return m_Admin;
	}
	public String getTabName() {
		return m_TabName;
	}
	public String getTypeStr() {
		return m_TypeStr;
	}
	public String getFrameName() {
		return m_FrameName;
	}
}
