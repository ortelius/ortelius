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
