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

import java.util.List;

import dmadmin.AttributeChangeSet;
import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.ObjectTypeAndId;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.CreatedModifiedField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.json.LinkField;

public abstract class DMObject
	implements java.io.Serializable
{
	private static final long serialVersionUID = -5362995729983900561L;

	protected DMSession m_session;
	protected int m_id;
	protected String m_name;
	private int m_domain;
	protected User m_owner;
	protected UserGroup m_ownerGroup;
	protected String m_summary;
	protected User m_creator;
	protected int m_created;
	protected User m_modifier;
	protected int m_modified;
	protected boolean m_deleted;
	protected boolean m_unconfigured;

	public DMObject() {
		m_name = "";
		m_id = 0;
    }

	public DMObject(DMSession sess, int id, String name) {
		m_session = sess;
		m_id = id;
		m_name = name;
	}

	public void setSession(DMSession sess) {m_session = sess; }

	public int getId()  { return m_id; }
	public void setId(int id)  { m_id = id; }

	public void setName(String name)  { m_name = name; }
	public String getName()  { return m_name; }
 public String getFullName()
 {
  if (this.getDomain() != null)
   return this.getDomain().getFullDomain() + "." + m_name;
  else
   return m_name;
 }

	public abstract ObjectType getObjectType();

	public ObjectTypeAndId getOtid() {
		return new ObjectTypeAndId(getObjectType(), m_id);
	}

	public int getObjectTypeAsInt() {
		return getObjectType().value();
	}

	public abstract String getDatabaseTable();
	public abstract String getForeignKey();

	public int getDomainId()  { return m_domain; }
	public void setDomainId(int id)  { m_domain = id; }
	public Domain getDomain()  { return m_session.getDomain(m_domain); }

	public DMObject getOwner()  { return (m_owner != null) ? m_owner : m_ownerGroup; }
	public void setOwner(User owner)  { m_owner = owner; }
	public void setOwner(UserGroup owner)  { m_ownerGroup = owner; }

	public String getSummary()  { return m_summary; }
	public void setSummary(String summary) { m_summary = summary; }

	public User getCreator()  { return m_creator; }
	public void setCreator(User creator)  { m_creator = creator; }

	public int getCreated()   { return m_created; }
	public void setCreated(int created)   { m_created = created; }

	public User getModifier()  { return m_modifier; }
	public void setModifier(User modifier)  { m_modifier = modifier; }

	public int getModified()   { return m_modified; }
	public void setModified(int modified)   { m_modified = modified; }

	public boolean isDeleted()   { return m_deleted; }
	public void setDeleted(boolean deleted)   { m_deleted = deleted; }

	public boolean isUnconfigured()   { return m_unconfigured; }
	public void setUnconfigured(boolean unconfigured)   { m_unconfigured = unconfigured; }

	public boolean getStatusNotNormal()  { return (m_deleted  || m_unconfigured); }
	public String getStatusText()  { return (m_deleted ? "Deleted" : (m_unconfigured ? "Unconfigured" : "Normal")); }

	protected void addCreatorModifier(PropertyDataSet ds) {
		ds.addProperty(SummaryField.READ_ONLY, "Created", (m_creator != null)
			? new CreatedModifiedField(m_session.formatDateToUserLocale(m_created), m_creator) : null);
		ds.addProperty(SummaryField.READ_ONLY, "Modified", (m_modifier != null)
			? new CreatedModifiedField(m_session.formatDateToUserLocale(m_modified), m_modifier) : null);
	}

	protected void addAttachments(PropertyDataSet ds) {
		List<Attachment> attaches = m_session.getAttachmentsForObject(getOtid());
		if(attaches.size() > 0) {
			JSONArray arr = new JSONArray();
			for(Attachment attach : attaches) {
				arr.add(attach.getJSONObject());
			}
			JSONObject ao = new JSONObject();
			ao.add("type", ObjectType.ATTACHMENT_LIST.getTypeString());
			ao.add("atts", arr);
			ds.addProperty(SummaryField.ATTACHMENTS, "Attachments", ao);
		}

	}

	public List<DMAttribute> getAttributes() {
		return m_session.getAttributesForObject(this);
	}

	public boolean updateAttributes(AttributeChangeSet changes) {
		return m_session.updateAttributesForObject(this, changes);
	}

	public IJSONSerializable getLinkJSON() {
	 if (this instanceof Component)
	 {
	  Component comp = m_session.getComponent(this.getId(), true);
	  return new LinkField(comp.getObjectType(), m_id, comp.getDomain().getFullDomain() + "." + m_name);
	 }
		return new LinkField(getObjectType(), m_id, m_name);
	}

	public abstract IJSONSerializable getSummaryJSON();
	public abstract boolean updateSummary(SummaryChangeSet changes);

	public boolean isViewable(boolean inherit)  {
		// Check domain access first
		if(!m_session.isValidDomainForObject(this,inherit)) {
			System.out.println("isValidDomainForObject fails, returning false");
			return false;
		}

		// The the access control list for the object
		AccessControlList acl = new AccessControlList(m_session, this, false);

		//// Empty ACL defaults to allow access
		//if(acl.isEmpty()) {
		//	return true;
		//}

		// Get the groups that this user is a member of
		List<UserGroup> groups = m_session.getGroupsForCurrentUser();
		if((groups == null) || groups.isEmpty()) {
			return false;
		}

		// Now check each group to see if it has access
		for(UserGroup group : groups) {
			if(acl.hasViewAccess(group)) {
				return true;
			}
		}

		// Nothing found, no access
		return false;
	}

	public boolean isUpdatable(boolean inherit)  {
	 if (this instanceof Category || this instanceof NotifyTemplate)
	  return true;

		// Check domain access first
		if(!m_session.isValidDomainForObject(this)) {
			// System.out.println("a) false");
			return false;
		}

		// The access control list for the object
		AccessControlList acl = new AccessControlList(m_session, this, false);

		//// Empty ACL defaults to allow access
		//if(acl.isEmpty()) {
		//	return true;
		//}

		// Get the groups that this user is a member of
		List<UserGroup> groups = m_session.getGroupsForCurrentUser();
		if((groups == null) || groups.isEmpty()) {
			// System.out.println("b) false");
			return false;
		}

		// Now check each group to see if it has access
		// System.out.println("acl.isEmpty="+acl.isEmpty());
		for(UserGroup group : groups) {
			// System.out.println("Checking group "+group.getName());
			if(acl.hasUpdateAccess(group)) {
				System.out.println("c) true (group "+group.getName()+")");
				return true;
			} // else System.out.println("no update access");
		}

		// Nothing found, no access
		// System.out.println("d) false");
		return false;
	}

	public boolean isReadable(boolean inherit)  {
		// Check domain access first
		if(!m_session.isValidDomainForObject(this)) {
			return false;
		}

		// The the access control list for the object
		AccessControlList acl = new AccessControlList(m_session, this, false);

		//// Empty ACL defaults to allow access
		//if(acl.isEmpty()) {
		//	return true;
		//}

		// Get the groups that this user is a member of
		List<UserGroup> groups = m_session.getGroupsForCurrentUser();
		if((groups == null) || groups.isEmpty()) {
			return false;
		}

		// Now check each group to see if it has access
		for(UserGroup group : groups) {
			if(acl.hasReadAccess(group)) {
				return true;
			}
		}

		// Nothing found, no access
		return false;
	}

	public boolean isWriteable(boolean inherit)  {
		// Check domain access first
		if(!m_session.isValidDomainForObject(this)) {
			return false;
		}
		// The the access control list for the object
		AccessControlList acl = new AccessControlList(m_session, this, false);

		//// Empty ACL defaults to allow access
		//if(acl.isEmpty()) {
		//	return true;
		//}

		// Get the groups that this user is a member of
		List<UserGroup> groups = m_session.getGroupsForCurrentUser();
		if((groups == null) || groups.isEmpty()) {
			return false;
		}
		// Now check each group to see if it has access
		for(UserGroup group : groups) {
			System.out.println("Checking write access for group "+group.getId());
			if(acl.hasWriteAccess(group)) {
				System.out.println("Found it, returning true");
				return true;
			}
		}

		// Nothing found, no access
		return false;
	}

	public boolean isViewable() {
		return isViewable(false);
	}

	public boolean isUpdatable()  {
		return isUpdatable(false);
	}

	public boolean isReadable()  {
		return isReadable(false);
	}

	public boolean isWriteable()  {
		return isWriteable(false);
	}


	public boolean hasReadWrite() {
		return true;
	}

	public String getWriteTitle() {
		return null;
	}

	public String getReadTitle() {
		return null;
	}

	public String getActOrFuncText() {
		return null;
	}

}
