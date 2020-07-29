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

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;

public class UserGroup	extends DMObject implements Comparable<Object>
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	public static final int EVERYONE_ID = 1;
	public static UserGroup EVERYONE = new UserGroup(null, EVERYONE_ID, "*EVERYONE*");
	
	private String m_email;
	private boolean m_acloverride;
	private boolean m_tabend;
	private boolean	m_tabapp;
	private boolean m_tabaction;
	private boolean	m_tabprov;
	private boolean	m_tabuser;
	private UserPermissions	m_userperms;
	

	public UserGroup() {
	}
	
	public UserGroup(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public UserPermissions getUserPermissions() {
		if(m_userperms == null) {
			m_userperms = new UserPermissions(m_session, m_id);	// set the create object permissions for this group
		}
		return m_userperms;
	}
	
	public String getEmail()  { return m_email; }
	public void setEmail(String email)  { m_email = email; }	
	
	public boolean getAclOverride()  { return m_acloverride; }
	public void setAclOverride(boolean acloverride)  { m_acloverride = acloverride; }
	
	public boolean getTabEnd()  { return m_tabend; }
	public void setTabEnd(boolean tabend)  { m_tabend = tabend; }	
	
	public boolean getTabApp()  { return m_tabapp; }
	public void setTabApp(boolean tabapp)  { m_tabapp = tabapp; }	
	
	public boolean getTabAction()  { return m_tabaction; }
	public void setTabAction(boolean tabapp)  { m_tabaction = tabapp; }	
	
	public boolean getTabProv()  { return m_tabprov; }
	public void setTabProv(boolean tabprov)  { m_tabprov = tabprov; }	
	
	public boolean getTabUser()  { return m_tabuser; }
	public void setTabUser(boolean tabuser)  { m_tabuser = tabuser; }
	
	public String getAclChecked()        { return m_acloverride?"checked":""; }
	public String getTabEndChecked()     { return m_tabend?"checked":""; }
	public String getTabAppChecked()     { return m_tabapp?"checked":""; }
	public String getTabActionChecked()  { return m_tabaction?"checked":""; }
	public String getTabProvChecked()    { return m_tabprov?"checked":""; }
	public String getTabUserChecked()    { return m_tabuser?"checked":""; }
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.USERGROUP;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_usergroup";
	}

	@Override
	public String getForeignKey() {
		return "usergroupid";
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		ds.addProperty(SummaryField.NAME, "Group Name", m_name);
		ds.addProperty(SummaryField.SUMMARY, "Summary", m_summary);
		ds.addProperty(SummaryField.GROUP_EMAIL, "Email", m_email);
		addCreatorModifier(ds);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateGroup(this, changes);
	}
	
	@Override
	public boolean isViewable()  {
		// Check domain access first
		if(!m_session.isValidDomainForObject(this)) {
			return false;
		}
		return true;
	}
	/*
	@Override
	public boolean isUpdatable()  {
		// Check domain access first
		if(!m_session.isValidDomainForObject(this)) {
			return false;
		}
		return true;
	}
	*/

	@Override
	public boolean hasReadWrite() {
		return false;
	}
	
 @Override
 public int compareTo(Object o)
 {
  return this.getName().compareToIgnoreCase(((User)o).getName());
 }
}
