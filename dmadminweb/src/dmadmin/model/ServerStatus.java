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
