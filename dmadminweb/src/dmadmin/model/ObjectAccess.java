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

public class ObjectAccess
{
	private enum SimplePermission {
		UNSET,
		ALLOW,
		DENY;
		
		/**
		 * Converts 'Y', 'N' or null into ALLOW, DENY or UNSET.
		 */
		static SimplePermission fromString(String str) {
			return (str != null) ? (str.equalsIgnoreCase("Y") ? SimplePermission.ALLOW : SimplePermission.DENY) : SimplePermission.UNSET;
		}
		
		/**
		 * Combines an inherited value (str) with the current object
		 * permission.  Inherited values are only taken if the current value is
		 * UNSET.  The new value is marked as being inherited.
		 */
		static SimplePermission combine(SimplePermission obj, String str) {
			switch(fromString(str)) {
			case ALLOW: return ((obj == UNSET) ? ALLOW : obj);
			case DENY:  return ((obj == UNSET) ? DENY : obj);
			default:    return obj;
			}
		}
	}
	
	// Combined values for immediate return
	private SimplePermission m_viewAccess   = SimplePermission.UNSET;
	private SimplePermission m_updateAccess = SimplePermission.UNSET;
	private SimplePermission m_readAccess   = SimplePermission.UNSET;
	private SimplePermission m_writeAccess  = SimplePermission.UNSET;
	// Object values
	private SimplePermission m_viewObject    = SimplePermission.UNSET;
	private SimplePermission m_updateObject  = SimplePermission.UNSET;
	private SimplePermission m_readObject    = SimplePermission.UNSET;
	private SimplePermission m_writeObject   = SimplePermission.UNSET;
	// Combined values for all parents
	private SimplePermission m_viewInherit   = SimplePermission.UNSET;
	private SimplePermission m_updateInherit = SimplePermission.UNSET;
	private SimplePermission m_readInherit   = SimplePermission.UNSET;
	private SimplePermission m_writeInherit  = SimplePermission.UNSET;
	
	public ObjectAccess()
	{}
			
	public void addObjectAccess(String view, String update) {
		m_viewObject   = m_viewAccess   = SimplePermission.fromString(view);
		m_updateObject = m_updateAccess = SimplePermission.fromString(update);
	}
	
	public void addObjectAccess(String view, String update, String read, String write)
	{
		m_viewObject   = m_viewAccess   = SimplePermission.fromString(view);
		m_updateObject = m_updateAccess = SimplePermission.fromString(update);
		m_readObject   = m_readAccess   = SimplePermission.fromString(read);
		m_writeObject  = m_writeAccess  = SimplePermission.fromString(write);
	}
	
	public void addDomainAccess(String view, String update, String read, String write)
	{
		m_viewAccess   = SimplePermission.combine(m_viewAccess,   view);
		m_updateAccess = SimplePermission.combine(m_updateAccess, update);
		m_readAccess   = SimplePermission.combine(m_readAccess,   read);
		m_writeAccess  = SimplePermission.combine(m_writeAccess,  write);
		
		m_viewInherit   = SimplePermission.combine(m_viewInherit,   view);
		m_updateInherit = SimplePermission.combine(m_updateInherit, update);
		m_readInherit   = SimplePermission.combine(m_readInherit,   read);
		m_writeInherit  = SimplePermission.combine(m_writeInherit,  write);
	}
	
	public boolean isViewable()   { return (m_viewAccess   == SimplePermission.ALLOW); }
	public boolean isUpdatable()  { return (m_updateAccess == SimplePermission.ALLOW); }
	public boolean isReadable()   { return (m_readAccess   == SimplePermission.ALLOW); }
	public boolean isWriteable()  { return (m_writeAccess  == SimplePermission.ALLOW); }
	
	public boolean isViewDenied()    { return (m_viewAccess   == SimplePermission.DENY); }
	public boolean isUpdateDenied()  { return (m_updateAccess == SimplePermission.DENY); }
	public boolean isReadDenied()    { return (m_readAccess   == SimplePermission.DENY); }
	public boolean isWriteDenied()   { return (m_writeAccess  == SimplePermission.DENY); }
	
	public boolean isViewInherited()    { return (m_viewObject   == SimplePermission.UNSET) && (m_viewInherit   == SimplePermission.ALLOW); }
	public boolean isUpdateInherited()  { return (m_updateObject == SimplePermission.UNSET) && (m_updateInherit == SimplePermission.ALLOW); }
	public boolean isReadInherited()    { return (m_readObject   == SimplePermission.UNSET) && (m_readInherit   == SimplePermission.ALLOW); }
	public boolean isWriteInherited()   { return (m_writeObject  == SimplePermission.UNSET) && (m_writeInherit  == SimplePermission.ALLOW); }
	
	public String toString() {
		return "view=" + m_viewAccess + ", update=" + m_updateAccess + ", read=" + m_readAccess + ", write=" + m_writeAccess;
	}
}