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
