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
