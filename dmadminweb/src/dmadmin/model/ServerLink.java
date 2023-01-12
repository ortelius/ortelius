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
