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
