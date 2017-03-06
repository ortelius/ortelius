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


public class Fragment
{
	private int m_fragid;
	private String m_fragname;
	private String m_summary;
	private int m_exitpoints;
	private boolean m_drilldown;
	private int m_actionid;
	
	
	public Fragment(int id,String name,String summary,int exitpoints,String dd, int actionid) {
		m_fragid=id;
		m_fragname=name;
		m_summary=summary;
		m_exitpoints=exitpoints;
		m_actionid = actionid;
		m_drilldown = dd!=null?dd.equalsIgnoreCase("Y"):false;
	}
	
	public int getId() { return m_fragid; }
 public int getActionId() { return m_actionid; }
	public String getName() { return m_fragname; }
	public String getSummary() { return m_summary; }
	public int getExitpoints() { return m_exitpoints; }
	public String getDrillDown() { return m_drilldown?"Y":"N"; }
	
	
	public void setId(int id) { m_fragid = id; }
 public void setActionId(int id) { m_actionid = id; }
	public void setName(String name) { m_fragname = name; }
	public void setSummary(String summary) { m_summary = summary; }
	public void setExitpoints(int exitpoints) { m_exitpoints = exitpoints; }
	public void setDrillDown(String dd) { m_drilldown = (dd!=null?dd.equalsIgnoreCase("Y"):false); }
}
