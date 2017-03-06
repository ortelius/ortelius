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
package dmadmin;

import dmadmin.model.DMObject;

/**
 * An OTID or Object Type and Id consists of a two letter type followed by an integer id.
 * @author Robert
 */
public class ObjectTypeAndId
{
	private ObjectType m_objtype;
	private int m_id;
	private int m_subid;
	
	public ObjectTypeAndId(String otid) {
		if((otid == null) || (otid.length() < 3)) {
			throw new RuntimeException("Invalid OTID: " + otid);
		}
		String ot = otid.substring(0, 2);
		String oid = otid.substring(2);
		m_objtype = ObjectType.fromTypeString(ot);

		if(m_objtype == null) {
			throw new RuntimeException("Invalid Object Type: " + ot);
		}
		
		if (oid.equalsIgnoreCase("-1")) {
			// Initial creation?
			m_id = -1;
		} else {
			try {
				int dashpoint = oid.indexOf('-');
				if (dashpoint >= 0) {
					m_id = Integer.parseInt(oid.substring(0,dashpoint));
					m_subid = Integer.parseInt(oid.substring(dashpoint+1));
				} else {
					m_id = Integer.parseInt(oid);
				}
			} catch(NumberFormatException e) {
				e.printStackTrace();
				throw new RuntimeException("Invalid Object Id: " + oid);
			}
		}
	}

	public ObjectTypeAndId(ObjectType objtype, int id) {
		m_objtype = objtype;
		m_id = id;
		m_subid = 0;
	}

	public ObjectTypeAndId(DMObject obj) {
		m_objtype = obj.getObjectType();
		m_id = obj.getId();
		m_subid = 0;
	}
	
	public ObjectType getObjectType()  { return m_objtype; }
	public int getId()  { return m_id; }
	public int getSubId() { return m_subid; }
	
	public String toString() {
		if (m_subid>0) {
			return m_objtype.getTypeString() + m_id + "-" + m_subid;
		} else {
			return m_objtype.getTypeString() + m_id;
		}
	}
}
