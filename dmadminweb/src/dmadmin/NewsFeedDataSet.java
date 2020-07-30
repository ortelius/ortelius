/*
 *
 *  Ortelius for Microservice Configuration Mapping
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
package dmadmin;

import java.util.ArrayList;
import java.util.List;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;

public class NewsFeedDataSet
{
	private List<PropertyDataSet> m_items = new ArrayList<PropertyDataSet>();
	
	public NewsFeedDataSet()
	{}
	
	public void addItem(PropertyDataSet item)
	{	
		m_items.add(item);
	}
	
	public IJSONSerializable getJSON() {
		JSONArray ret = new JSONArray();
		for(PropertyDataSet item : m_items) {
			ret.add(item.getNewsJSON());
		}
		return ret;
	}
}
