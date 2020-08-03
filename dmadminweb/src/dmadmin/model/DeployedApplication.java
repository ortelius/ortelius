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
package dmadmin.model;

import java.util.Date;


public class DeployedApplication
{	
	int		m_appid;			// application id
	String	m_appname;			// application name
	int		m_versionid;		// application version id
	String	m_versionname;		// application version name
	int		m_predecessorid;	// predecessor of app version
	int		m_deploymentid;		// deploymentid
	int		m_exitcode;			// exit code
	Date	m_completedDate;	// Completion Date/Time
	
	
	public DeployedApplication() {
	}
	
	public void setApplicationID(int appid) {
		m_appid = appid;
	}
	
	public void setApplicationName(String appname) {
		m_appname = appname;
	}
	
	public void setVersionID(int versionid) {
		m_versionid = versionid;
	}
	
	public void setVersionName(String versionname) {
		m_versionname = versionname;
	}
	
	public void setPredecessorID(int predecessorid) {
		m_predecessorid = predecessorid;
	}
	
	public void setDeploymentID(int deploymentid) {
		m_deploymentid = deploymentid;
	}
	
	public void setExitCode(int exitcode) {
		m_exitcode = exitcode;
	}
	
	public void setCompletedDate(Date completedDate) {
		m_completedDate = completedDate;
	}
	
	public int getApplicationID() {
		return m_appid;
	}
	
	public String getApplicationName() {
		return m_appname;
	}
	
	public int getVersionID() {
		return m_versionid;
	}
	
	public String getVersionName() {
		return m_versionname;
	}
	
	public int getPredecessorID() {
		return m_predecessorid;
	}
	
	public int getDeploymentID() {
		return m_deploymentid;
	}
	
	public int getExitCode() {
		return m_exitcode;
	}
	
	public Date getCompletedDate() {
		return m_completedDate;
	}
}
