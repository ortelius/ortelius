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

public class FragmentDetails
{
	private String m_typename;
	private String m_typesummary;
	private String m_fragname;
	private String m_fragsummary;
	private int m_windowid;
	private int m_actionid;
	private int m_parentwindow;
	private int m_fragaction;
	private int m_exitpoints;
	private boolean m_drilldown;
	
	public FragmentDetails() {
		m_typename="";
		m_typesummary="";
		m_fragname="";
		m_fragsummary="";
		m_parentwindow=0;
		m_windowid=0;
		m_actionid=0;
		m_exitpoints=0;
		m_drilldown=false;
	}	

	public String getTypeName() { return m_typename; }
	public String getTypeSummary() { return m_typesummary; }
	public String getFragmentName() { return m_fragname; }
	public String getFragmentSummary() { return m_fragsummary; }
	public int getExitPoints() { return m_exitpoints; }
	public boolean isDrilldown() { return m_drilldown; }
	public int getParentWindow() { return m_parentwindow; }
	public int getFragmentAction() { return m_fragaction; }
	public int getWindowId() { return m_windowid; }
	public int getActionId() { return m_actionid; }
	
	

	public void setTypeName(String typename) { m_typename = typename; }
	public void setTypeSummary(String typesummary) { m_typesummary = typesummary; }
	public void setFragmentName(String fragname) { m_fragname = fragname; }
	public void setFragmentSummary(String fragsummary) { m_fragsummary = fragsummary; }
	public void setExitPoints(int exitpoints) { m_exitpoints = exitpoints; }
	public void setDrilldown(boolean drilldown) { m_drilldown = drilldown; }
	public void setParentWindow(int pw) { m_parentwindow = pw; }
	public void setFragmentAction(int action) { m_fragaction = action; }
	public void setWindowId(int w) { m_windowid = w; }
	public void setActionId(int a) { m_actionid = a; }
}
