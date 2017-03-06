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

public class HomeTabs implements java.io.Serializable {
	private static final long serialVersionUID = -5362995729983900561L;
	private int m_TabNumber;
	private String m_TabName;
	private String m_IconName;
	private String m_TabTitle;
	private String m_ExplorerFrame;
	private String m_FrameID;
	private String m_TreeViewPage;
	
	public HomeTabs() {
		m_TabNumber = 0;
		m_TabName = "";
		m_IconName = "";
		m_TabTitle = "";
		m_ExplorerFrame = "";
		m_FrameID = "";
		m_TreeViewPage = "";
    }
	
	public HomeTabs(int id, boolean admin, String iconName, String tabTitle)
	{
		m_TabNumber = 0;
		m_TabName = "utabs-" + id;
		m_IconName = iconName;
		m_TabTitle = tabTitle;
		m_ExplorerFrame = "explorerframe" + id;
		m_TreeViewPage = "Explorer?id=" + id;		
		m_FrameID = "mainframe" + id;
	}
	
	
	public void setTabNumber(int tabno) {
		m_TabNumber = tabno;
	}
	
	public void setTabName(String tabName) {
		m_TabName = tabName;
	}
	public void setIconName(String iconName) {
		m_IconName = iconName;
	}
	public void setTabTitle(String tabTitle) {
		m_TabTitle = tabTitle;
	}
	public void setExplorerFrame(String explorerFrame) {
		m_ExplorerFrame = explorerFrame;
	}
	public void setTreeViewPage(String treeViewPage) {
		m_TreeViewPage = treeViewPage;
	}
	public void setFrameID(String frameID) {
		m_FrameID = frameID;
	}
	
	public int getTabNumber() {
		return m_TabNumber;
	}
	public String getTabName() {
		return m_TabName;
	}
	public String getIconName() {
		return m_IconName;
	}
	public String getTabTitle() {
		return m_TabTitle;
	}
	public String getExplorerFrame() {
		return m_ExplorerFrame;
	}
	public String getTreeViewPage() {
		return m_TreeViewPage;
	}
	public String getFrameID() {
		return m_FrameID;
	}
}
