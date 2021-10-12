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


public class SearchResult
{
	private int	m_id;
	private String m_name;
	private String m_url;
	private String m_iconname;
	private String m_summary;
	
	
	public SearchResult() {
		
	}
	
	public int getId() { return m_id; }
	public String getName() { return m_name; }
	public String getUrl() { return m_url; }
	public String getIconName() { return m_iconname; }
	public String getSummary() { return m_summary; }
	
	public void setId(int id) { m_id = id; }
	public void setName(String name) { m_name = name; }
	public void setUrl(String url) { m_url = url; }
	public void setIconName(String iconname) { m_iconname = iconname; }
	public void setSummary(String summary) { m_summary = summary; }
}
