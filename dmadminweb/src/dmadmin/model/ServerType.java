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
