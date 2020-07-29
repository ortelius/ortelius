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
import dmadmin.json.BooleanField;
import dmadmin.json.DateTimeField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

public class User	extends DMObject implements Comparable<Object> 
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	private String m_realName;
	private String m_email;
	private String m_phone;
	private String m_datefmt;
	private String m_timefmt;
	private int m_lastLogin;
	private boolean m_accountLocked;
	private boolean m_forceChangePass;
	private Datasource m_datasource;

	public User() {
	}
	
	public User(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public User(DMSession sess, int id, String name, String realName) {
		super(sess, id, name);
		m_realName = realName;
	}
	
	public String getRealName()  { return m_realName; }
	public void setRealName(String realName)  { m_realName = realName; }
	
	public String getEmail()  { return m_email; }
	public void setEmail(String email)  { m_email = email; }	
	
	public String getPhone()  { return m_phone; }
	public void setPhone(String phone)  { m_phone = phone; }
	
	public String getDateFmt()  { return (m_datefmt == null)?"MM/dd/yyyy":m_datefmt; }	// US Format Default
	public void setDateFmt(String datefmt)  { m_datefmt = (datefmt == null)?"MM/dd/yyyy":datefmt; }
	
	public String getTimeFmt()  { return (m_timefmt == null)?"hh:mm":m_timefmt; }
	public void setTimeFmt(String timefmt)  {m_timefmt = (timefmt == null)?"hh:mm":timefmt; }
	
	public int getLastLogin()  { System.out.println("lastlogin is "+m_lastLogin);return m_lastLogin; }
	public void setLastLogin(int lastLogin)  { m_lastLogin = lastLogin; }	
	
	public boolean isAccountLocked()  { return m_accountLocked; }
	public void setAccountLocked(boolean locked)  { m_accountLocked = locked; }	
	
	public boolean isForceChangePass()  { return m_forceChangePass; }
	public void setForceChangePass(boolean fcp)  { m_forceChangePass = fcp; }	
	
	public Datasource getDatasource()  { return m_datasource; }
	public void setDatasource(Datasource datasource)  { m_datasource = datasource; }
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.USER;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_user";
	}

	@Override
	public String getForeignKey() {
		return "userid";
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		System.out.println("getSummaryJSON for USER");
		PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		ds.addProperty(SummaryField.NAME, "User Name", m_name);
		ds.addProperty(SummaryField.USER_REALNAME, "Real Name", m_realName);
		ds.addProperty(SummaryField.USER_EMAIL, "Email", m_email);
		ds.addProperty(SummaryField.USER_PHONE, "Phone", m_phone);
		ds.addProperty(SummaryField.USER_DATE_FMT, "Date Format", m_datefmt);
		ds.addProperty(SummaryField.USER_TIME_FMT, "Time Format", m_timefmt);
		ds.addProperty(SummaryField.USER_DATASOURCE, "Validation Data Source",
				(m_datasource != null)?m_datasource.getLinkJSON():null);
		if(isUpdatable()) {
			ds.addProperty(SummaryField.USER_PASSWORD, "Password", (String) null);			
			ds.addProperty(SummaryField.USER_LOCKED, "Account Locked",
				new BooleanField(m_accountLocked));			
			ds.addProperty(SummaryField.USER_CHNG_PASS, "Force Change Password",
				new BooleanField(m_forceChangePass));			
		}
		addCreatorModifier(ds);
		ds.addProperty(SummaryField.READ_ONLY, "Last Login",
			new DateTimeField(m_session, m_lastLogin));
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateUser(this, changes);
	}

	@Override
	public IJSONSerializable getLinkJSON() {
		return new LinkField(ObjectType.USER, m_id, m_name, m_realName);
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
