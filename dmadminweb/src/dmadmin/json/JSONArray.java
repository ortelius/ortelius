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
package dmadmin.json;

import java.util.ArrayList;
import java.util.List;

public class JSONArray
	implements IJSONSerializable
{
	private List<Object> m_items = new ArrayList<Object>();
	private boolean m_allString;
	
	public JSONArray()
	{
		this(false);
	}
	
	public int length()
	{
		return m_items.size();
	}
	
	public JSONArray(boolean allString)
	{
		m_allString = allString;
	}
	
	public JSONArray add(Object obj)
	{
		m_items.add(obj);
		return this;
	}

	@Override
	public String getJSON()
	{
		StringBuffer sb = new StringBuffer();
		boolean first = true;
		sb.append("[");
		for(Object obj : m_items) {
			if(!first) { sb.append(","); }
			if(obj == null) {
				sb.append("\"\"");				
			} else if(obj instanceof IJSONSerializable) {
				sb.append(((IJSONSerializable) obj).getJSON());
			} else if(m_allString || (obj instanceof String)) {
				sb.append("\"").append(JSONUtil.escape(obj.toString())).append("\"");
			} else {
				sb.append(obj.toString());
			}
			first = false;
		}
		sb.append("]");
		return sb.toString();
	}
	
	@Override
	public String toString()
	{
		return getJSON();
	}
}
