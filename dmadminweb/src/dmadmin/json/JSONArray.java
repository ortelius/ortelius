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
