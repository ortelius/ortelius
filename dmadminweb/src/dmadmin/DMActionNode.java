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

package dmadmin;

public class DMActionNode implements java.io.Serializable {
	private static final long serialVersionUID = -5362995729983900561L;
	private int m_nodeid;
	private String m_descriptor;
	private int m_xpos;
	private int m_ypos;
	private int m_typeid;
	private int m_exitpoints;
	private int m_procid;
	private int m_functionid;
	private int m_kind;
	private String m_title;
	private String m_summary;
	private boolean m_drilldown;

	public DMActionNode() {
		m_nodeid=0;
		m_descriptor="";
		m_xpos=0;
		m_ypos=0;
		m_typeid=0;
		m_exitpoints=0;
		m_procid=0;
		m_functionid=0;
		m_kind=0;
		m_title="";
		m_summary="";
		m_drilldown=false;
    }
	public void setNodeID(int id) {
		m_nodeid = id;
	}
	public void setDrillDown(String dd) {
		m_drilldown = dd!=null?dd.equalsIgnoreCase("Y"):false;
	}
	public void setDescriptor(String desc) {
		m_descriptor = desc;
	}
	public void setXpos(int xpos) {
		m_xpos = xpos;
	}
	public void setYpos(int ypos) {
		m_ypos = ypos;
	}
	public void setTypeID(int typeid) {
		m_typeid = typeid;
	}
	public void setExitPoints(int exitpoints) {
		m_exitpoints = exitpoints;
	}
	public void setTitle(String title) {
		m_title = title;
	}
	public void setSummary(String summary) {
		m_summary = summary;
	}
	public void setFunctionID(int functionid) {
		m_functionid = functionid;
	}
	public void setProcedureID(int procid) {
		m_procid = procid;
	}
	public void setKind(int kind) {
		m_kind = kind;
	}

	public int getNodeID() {
		return m_nodeid;
	}
	public String getDrillDown() {
		return m_drilldown?"Y":"N";
	}
	public String getDescriptor() {
		return m_descriptor;
	}
	public int getXpos() {
		return m_xpos;
	}
	public int getYpos() {
		return m_ypos;
	}
	public int getTypeID() {
		return m_typeid;
	}
	public int getExitPoints() {
		return m_exitpoints;
	}
	public String getTitle() {
		return m_title;
	}
	public String getSummary() {
		return m_summary;
	}
	public int getFunctionID() {
		return m_functionid;
	}
	public int getProcedureID() {
		return m_procid;
	}
	public int getKind() {
		return m_kind;
	}
}
