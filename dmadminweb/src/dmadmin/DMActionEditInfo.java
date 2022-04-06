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

public class DMActionEditInfo implements java.io.Serializable {
	private static final long serialVersionUID = -5362995729983900561L;
	private int m_editid;
	private int m_userid;
	public DMActionEditInfo() {
		m_editid=0;
		m_userid=0;
    }
	public void setUserID(int id) {
		m_userid = id;
	}
	public void setEditID(int id) {
		m_editid = id;
	}
	
	public int getUserID() {
		return m_userid;
	}
	public int getEditID() {
		return m_editid;
	}
}