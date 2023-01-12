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
