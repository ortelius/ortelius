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
