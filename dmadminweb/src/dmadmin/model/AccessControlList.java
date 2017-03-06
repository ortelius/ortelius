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
package dmadmin.model;

import java.util.Hashtable;

import dmadmin.DMSession;

public class AccessControlList
{
	private Hashtable<Integer, ObjectAccess> m_acl;

	/**
	 * forDisplay is set to true to indicate that we will be displaying the
	 * values in the user interface.  If it is set to false, the session will
	 * respect the override access control setting for the user and return an
	 * appropriate set of defaults.
	 */
	public AccessControlList(DMSession session, DMObject object, boolean forDisplay)
	{
		// This is important - the only way get the contents of the list is
		// from the session - we do not have an add() method
		m_acl = session.getAccessForObject(object, forDisplay);
	}
	
	public boolean isEmpty()
	{
		return (m_acl != null) ? m_acl.isEmpty() : true;
	}
	
	public boolean hasViewAccess(UserGroup group)
	{
		ObjectAccess access = (m_acl != null) ? m_acl.get(group.getId()) : null;
		return (access != null) ? access.isViewable() : false;
	}
	
	public boolean hasUpdateAccess(UserGroup group)
	{
		ObjectAccess access = (m_acl != null) ? m_acl.get(group.getId()) : null;
		return (access != null) ? access.isUpdatable() : false;
	}
	
	public boolean hasReadAccess(UserGroup group)
	{
		ObjectAccess access = (m_acl != null) ? m_acl.get(group.getId()) : null;
		return (access != null) ? access.isReadable() : false;
	}
	
	public boolean hasWriteAccess(UserGroup group)
	{
		ObjectAccess access = (m_acl != null) ? m_acl.get(group.getId()) : null;
		if (access == null) System.out.println("Access is null"); else System.out.println("access isWritebale is "+access.isWriteable());
		return (access != null) ? access.isWriteable() : false;
	}
}
