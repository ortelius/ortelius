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


public class FragmentListValues
{
	private int m_id;
	private String m_name;
	private boolean m_selected;

	
	public FragmentListValues() {
		m_id=0;
		m_name="";
		m_selected=false;
	}	

	public int getId() { return m_id; }
	public String getName() { return m_name; }
	public String getSelected() { return m_selected?"selected":""; }

	public void setName(String name) { m_name = name; }
	public void setSelected(boolean s) { m_selected = s; }
	public void setId(int id ) { m_id = id; }
}
