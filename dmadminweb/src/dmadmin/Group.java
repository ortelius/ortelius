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

public class Group implements java.io.Serializable {
	private static final long serialVersionUID = -5362995729983900561L;

	private String m_GroupName;
	private String m_email;
	private String m_Domain;
	private String m_Summary;


	public Group() {
		m_GroupName = "";
		m_Domain = "";
		m_email="";
		m_Summary="";
    }

	public Group(String gname, String gdomain, String gsummary, String gemail) {
        // empty constructor
		m_GroupName = gname;
		m_Domain = gdomain;
		m_email=gemail;
		m_Summary=gsummary;
    }

	public void setGroupName(String userName) {
		m_GroupName = userName;
	}
	public void setDomain(String domain) {
		m_Domain = domain;
	}
	public void setEmail(String email) {
		m_email = email;
	}
	public void setSummary(String summary) {
		m_Summary = summary;
	}

	public String getGroupName() {
		return m_GroupName;
	}
	public String getDomain() {
		return m_Domain;
	}
	public String getEmail() {
		return m_email;
	}
	public String getSummary() {
		return m_Summary;
	}
}
