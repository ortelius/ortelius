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

import java.io.Serializable;

public class ServerStatus implements Serializable
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 8820282733974373811L;

	private String m_nameResolution;
	private String m_pingStatus;
	private String m_connectionStatus;
	private String m_baseDirectoryStatus;
	private int m_pingTime; 
	private String m_ipAddress;
	private String m_lastError;
	private int m_lastTime;
	
	public ServerStatus()
	{}
	
	public void setNameResolution(String nr)
	{
		m_nameResolution = nr;
	}
	
	public void setPingStatus(String ps)
	{
		m_pingStatus = ps;
	}
	
	public void setConnectionStatus(String cs)
	{
		m_connectionStatus = cs;
	}
	
	public void setBaseDirStatus(String bds)
	{
		m_baseDirectoryStatus = bds;
	}
	
	public void setPingTime(int pt)
	{	
		m_pingTime = pt;
	}
	
	public void setIPAddress(String ipa)
	{
		m_ipAddress = ipa;
	}
	
	public void setLastError(String es)
	{
		m_lastError = es;
	}
	
	public void setLastTime(int lt)
	{
		m_lastTime = lt;
	}
	
	public String getNameResolution()
	{
		return m_nameResolution;
	}
	
	public String getPingStatus()
	{
		return m_pingStatus;
	}
	
	public String getConnectionStatus()
	{
		return m_connectionStatus;
	}
	
	public String getBaseDirStatus()
	{
		return m_baseDirectoryStatus;
	}
	
	public int getPingTime()
	{	
		return m_pingTime;
	}
	
	public String getIPAddress()
	{
		return m_ipAddress;
	}
	
	public String getLastError()
	{
		return m_lastError;
	}
	
	public int getLastTime()
	{
		return m_lastTime;
	}
}
