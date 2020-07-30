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

public class JSONObject
	implements IJSONSerializable
{
	private class JSONProperty {
		String key;
		Object value;
		
		JSONProperty(String k, Object v) {
			key = k;
			value = v;
		}
	}
	
	private List<JSONProperty> m_items = new ArrayList<JSONProperty>();
	private boolean m_allString;
	
	public JSONObject()
	{
		this(false);
	}

	public JSONObject(boolean allString)
	{
		m_allString = allString;
	}
	
	public JSONObject add(String key, Object value)
	{
		m_items.add(new JSONProperty(key, value));
		return this;
	}

	@Override
	public String getJSON()
	{
		StringBuffer sb = new StringBuffer();
		boolean first = true;
		sb.append("{");
		for(JSONProperty item : m_items) {
			if(!first) { sb.append(","); }
			sb.append("\"").append(item.key).append("\":");
			Object obj = item.value;
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
		sb.append("}");
		return sb.toString();
	}
	
	@Override
	public String toString()
	{
		return getJSON();
	}
}
