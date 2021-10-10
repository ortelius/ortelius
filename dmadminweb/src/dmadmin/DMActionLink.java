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

public class DMActionLink implements java.io.Serializable {
	private static final long serialVersionUID = -5362995729983900561L;
	private int m_flowid;
	private int m_nodefrom;
	private int m_nodeto;
	private int m_pos;

	
	public DMActionLink() {
		m_flowid=0;
		m_nodefrom=0;
		m_nodeto=0;
		m_pos=0;
    }
	public void setFlowID(int id) {
		m_flowid = id;
	}
	public void setNodeFrom(int node) {
		m_nodefrom = node;
	}
	public void setNodeTo(int node) {
		m_nodeto = node;
	}
	public void setPos(int pos) {
		m_pos = pos;
	}
	
	public int getFlowID() {
		return m_flowid;
	}
	public int getNodeFrom() {
		return m_nodefrom;
	}
	public int getNodeTo() {
		return m_nodeto;
	}
	public int getPos() {
		return m_pos;
	}
}
