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

package dmadmin;

public class PanelTabs implements java.io.Serializable
{
	private static final long serialVersionUID = -5362995729983900561L;
	
	private int m_tabNumber;
	private String m_tabName;
	private String m_tabId;
	
	public PanelTabs() {
		m_tabNumber = 0;
		m_tabName = "";
		m_tabId = "";
    }
	
	public PanelTabs(String tabName,String tabId) {
		m_tabNumber = 0;
		m_tabName = tabName;
		m_tabId = tabId;
    }
	
	public void setTabNumber(int tabno) {
		m_tabNumber = tabno;
	}
	public void setTabName(String tabName) {
		m_tabName = tabName;
	}
	public void setTabId(String tabId) {
		m_tabId = tabId;
	}
	
	public int getTabNumber() {
		return m_tabNumber;
	}
	public String getTabName() {
		return m_tabName;
	}
	public String getTabId() {
		return m_tabId;
	}
}
