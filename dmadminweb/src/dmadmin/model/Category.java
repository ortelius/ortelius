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

import java.util.List;

import dmadmin.ObjectType;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

public class Category
{
	private int m_catid;
	private String m_catname;
	private List<Fragment> m_fragments;
	
	public static final Category NO_CATEGORY = new Category(10, "General");
	
	public Category(int id,String name) {
		m_catid=id;
		m_catname=name;
	}
	
	public int getId() { return m_catid; }
	public String getName() { return m_catname; }
	public List<Fragment> getFragments() { return m_fragments; }
	
	public void setId(int catid) { m_catid = catid; }
	public void setName(String catname) { m_catname = catname; }
	public void setFragments(List<Fragment>f) { m_fragments = f; }
	
	public IJSONSerializable getLinkJSON() {
		return new LinkField(ObjectType.ACTION_CATEGORY, m_catid, m_catname, false);
	}
}
