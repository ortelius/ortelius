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
