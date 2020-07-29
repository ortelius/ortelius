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
package dmadmin.model;

import dmadmin.DMSession;

public class SaasClient
{

	private String clientid;
	private String licensetype;
	private int licensecnt;
	private long lastseen;
	
	private DMSession m_session;
	
	public SaasClient() {
	}
	
	public SaasClient(DMSession sess, String name) {
  clientid = name;
		m_session = sess;
	}
	
	public DMSession getSession() {
		return m_session;
	}

 public String getClientId()
 {
  return clientid;
 }

 public void setClientId(String clientid)
 {
  this.clientid = clientid;
 }

 public String getLicenseType()
 {
  return licensetype;
 }

 public void setLicenseType(String licensetype)
 {
  this.licensetype = licensetype;
 }

 public int getLicenseCnt()
 {
  return licensecnt;
 }

 public void setLicenseCnt(int licensecnt)
 {
  this.licensecnt = licensecnt;
 }

 public long getLastseen()
 {
  return lastseen;
 }

 public void setLastseen(long lastseen)
 {
  this.lastseen = lastseen;
 }
	
	
	}
