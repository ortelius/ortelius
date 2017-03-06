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

public class ServerLink implements java.io.Serializable {
	private static final long serialVersionUID = -5362995729983900561L;
	private int m_sidefrom;
	private int m_sideto;
	private int m_nodefrom;
	private int m_nodeto;
	private String m_label;
	private String m_style;

	
	public ServerLink() {
		m_sidefrom=1;
		m_sideto=1;
		m_nodefrom=0;
		m_nodeto=0;
		m_label="";
		m_style="";
    }
	
	public ServerLink(int fn,int tn,int fs,int ts)
	{
		m_nodefrom = fn;
		m_nodeto = tn;
		m_sidefrom = fs;
		m_sideto = ts;
	}

	public void setNodeFrom(int node) {
		m_nodefrom = node;
	}
	public void setNodeTo(int node) {
		m_nodeto = node;
	}
	public void setSideFrom(int s) {
		m_sidefrom = s;
	}
	public void setSideTo(int s) {
		m_sideto = s;
	}
	public void setLabel(String label) {
		m_label = label;
	}
	public void setStyle(String style) {
		m_style = style;
	}

	public int getNodeFrom() {
		return m_nodefrom;
	}
	public int getNodeTo() {
		return m_nodeto;
	}
	public int getSideFrom() {
		return m_sidefrom>0?m_sidefrom:1;
	}
	public int getSideTo() {
		return m_sideto>0?m_sideto:1;
	}
	public String getLabel() {
		return m_label;
	}
	public String getStyle() {
		return m_style;
	}
}
