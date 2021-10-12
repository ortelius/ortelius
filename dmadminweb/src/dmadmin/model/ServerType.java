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

import dmadmin.ObjectType;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

public class ServerType
{
	public enum LineEndFormat {
		OFF(0),
		UNIX(1),
		WINDOWS(2),
		MAC(3);
		
		private int m_value;
		
		private LineEndFormat(int value) {
			m_value = value;
		}
		
		public int value()  { return m_value; }
		
		public static LineEndFormat fromInt(int value) {
			for(LineEndFormat lef : LineEndFormat.values()) {
				if(lef.value() == value) {
					return lef;
				}
			}
			return null;
		}
	}
	
	private int m_id;
	private String m_name;
	private LineEndFormat m_lineEnds;
	private String m_pathFormat;
	
	public ServerType(int id, String name, LineEndFormat lineEnds, String pathFormat)
	{
		m_id = id;
		m_name = name;
		m_lineEnds = lineEnds;
		m_pathFormat = pathFormat;
	}
	
	public int getId()  { return m_id; }
	public String getName()  { return m_name; }
	public LineEndFormat getLineEnds()  { return m_lineEnds; }
	public String getPathFormat()  { return m_pathFormat; }
	
	public IJSONSerializable getLinkJSON() {
		return new LinkField(ObjectType.SERVER_TYPE, m_id, m_name);
	}
}
