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
