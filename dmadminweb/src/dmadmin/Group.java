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
