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

public class ActionParameter
{
	private String m_name;
	private String m_type;
	private boolean m_required;

	public ActionParameter(String name,String type) {
		m_name = name;
		m_type = type;
		m_required = false;
	}

	public ActionParameter(String name,String type,boolean required) {
		m_name = name;
		m_type = type;
		m_required = required;
	}

	public String getName() { return m_name; }
	public String getType() { return m_type; }
	public void setRequired(boolean required) { m_required = required; }
	public boolean isRequired() { return m_required; }
}
