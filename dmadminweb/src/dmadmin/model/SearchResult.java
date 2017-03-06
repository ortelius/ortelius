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
