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

public class TaskParameter
{
	private int m_pos;
	private String m_label;
	private String m_var;
	private String m_type;
	private String m_arr;
	
	public TaskParameter(int pos,String label,String var,String type) {
		m_pos=pos;
		m_label=label;
		m_var = var;
		m_type = type;
		m_arr = null;
	}
	
	public TaskParameter(int pos,String label,String var,String type,String arr) {
		m_pos=pos;
		m_label=label;
		m_var = var;
		m_type = type;
		m_arr = arr;
	}
	
	public int getPos() { return m_pos; }
	public String getLabel() { return m_label; }
	public String getVar() { return m_var; }
	public String getType() { return m_type; }
	public String getArr() { return m_arr; }
}
