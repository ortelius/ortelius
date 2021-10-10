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
