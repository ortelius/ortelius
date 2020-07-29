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

//import java.util.ArrayList;
//import java.util.HashMap;
import java.util.List;
//import java.util.Map;

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;

public class Application
	extends DMObject
	implements IPrePostAction
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1327862378913381548L;
	
	private Action m_preAction;
	private Action m_postAction;
	private Action m_customAction;
	private NotifyTemplate m_successTemplate;
	private NotifyTemplate m_failureTemplate;
	private int m_predecessorId;
	private int m_parentId;
	private int m_xpos;
	private int m_ypos;
	private int m_startx = 300;	// component start window position
	private int m_basedomain;	// if this is a version, the domain of the parent "base" application
	private String m_label;
	private String m_parentlabel;
	private String m_isrelease;	
	private List<Application> m_versions;
	private Datasource m_datasource;
	
	public Application() {
		m_isrelease = "N";
	}
	
	public Application(DMSession sess, int id, String name) {
		super(sess, id, name);
		m_isrelease = "N";
	}
	
	
	public String getIsRelease()
	{
		return m_isrelease;
	}

	public void setIsRelease(String isrelease)
	{
		m_isrelease = (isrelease != null) ? isrelease : "N";
	}

	public Action getPreAction()  { return m_preAction; }
	public void setPreAction(Action action)  { m_preAction = action; }
	
	public Action getPostAction()  { return m_postAction; }
	public void setPostAction(Action action)  { m_postAction = action; }
	
	public Action getCustomAction()  { return m_customAction; }
	public void setCustomAction(Action action)  { m_customAction = action; }
	
	public NotifyTemplate getSuccessTemplate()  { return m_successTemplate; }
	public void setSuccessTemplate(NotifyTemplate template)  { m_successTemplate = template; }
	
	public NotifyTemplate getFailureTemplate()  { return m_failureTemplate; }
	public void setFailureTemplate(NotifyTemplate template)  { m_failureTemplate = template; }
	
	public int getPredecessorId() { return m_predecessorId; }
	public void setPredecessorId(int id) { m_predecessorId = id; }
	
	public int getParentId() { return m_parentId; }
	public void setParentId(int id) { m_parentId = id; }
	
	public int getXpos() { return m_xpos; }
	public void setXpos(int xpos) { m_xpos = xpos; }
	
	public int getYpos() { return m_ypos; }
	public void setYpos(int ypos) { m_ypos = ypos; }
	
	public Datasource getDatasource()  { return m_datasource; }
	public void setDatasource(Datasource datasource)  { m_datasource = datasource; }
	
	public List<Application> getVersions()
	{
		if (m_versions == null) {
			// Not yet set - retrieve child versions
			m_versions = m_session.getChildApplicationVersions(this);
		}
		return m_versions;
		
	}
	public void setVersions(List<Application> vs) { m_versions = vs; }
	
	public int getStartX() { return m_startx; }
	public void setStartX(int startx) { m_startx = startx; }
	
	public int getBaseDomain() { return m_basedomain; }
	public void setBaseDomain(int domainid) { m_basedomain = domainid; }
	
	public String getLabel(boolean checkparent) {
		if (checkparent == false || m_label.length() > 0) {
			return m_label;
		} else {
			return m_parentlabel;
		}
	}
	public String getLabel() {
		return getLabel(false);
	}
	public void setLabel(String label) { m_label = label; }
	public void setParentLabel(String plabel) { m_parentlabel = plabel; }
	
	public List<Environment> getPermittedEnvironments(int tid) {
		return m_session.getEnvironmentsForApplication(this,tid);
	}
	
	public List<Environment> getDeployedEnvironments(int tid) {
		return m_session.getDeployedEnvironmentsForApplication(this,tid);
	}

	public String getTitlePrefix() {
		return (m_predecessorId != 0) ? "Application Version" : "Application";
	}

	public String getIcon() {
		return (m_predecessorId != 0) ? "appver" : "application";
	}
	
	@Override
	public ObjectType getObjectType() {
	 if (m_isrelease != null && m_isrelease.equalsIgnoreCase("Y"))
	  return (m_predecessorId != 0) ? ObjectType.RELVERSION : ObjectType.RELEASE;
	 else
		 return (m_predecessorId != 0 || m_parentId != 0) ? ObjectType.APPVERSION : ObjectType.APPLICATION;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_application";
	}

	@Override
	public String getForeignKey() {
		return "appid";
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
		Domain dom = getDomain();
		if (dom == null)
  		ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
		else
		 ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
				: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		addCreatorModifier(ds);
		if (!getIsRelease().equalsIgnoreCase("y")) {
			ds.addProperty(SummaryField.APP_DATASOURCE, "Change Request Data Source",
					(m_datasource != null)?m_datasource.getLinkJSON():null);
		}
		
  ds.addProperty(SummaryField.XPOS, "XPos", getXpos());
  ds.addProperty(SummaryField.YPOS, "YPos", getYpos());
		
		ds.addProperty(SummaryField.PRE_ACTION, "Pre-Action",
			(m_preAction != null) ? m_preAction.getLinkJSON() : null);
		ds.addProperty(SummaryField.POST_ACTION, "Post-Action",
			(m_postAction != null) ? m_postAction.getLinkJSON() : null);
		ds.addProperty(SummaryField.CUSTOM_ACTION, "Custom Action",
			(m_customAction != null) ? m_customAction.getLinkJSON() : null);
		ds.addProperty(SummaryField.SUCCESS_TEMPLATE, "Successful Deployment Template",
				(m_successTemplate != null) ? m_successTemplate.getLinkJSON() : null);
		ds.addProperty(SummaryField.FAILURE_TEMPLATE, "Failed Deployment Template",
				(m_failureTemplate != null) ? m_failureTemplate.getLinkJSON() : null);
		addAttachments(ds);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateApplication(this, changes);
	}
	
	@Override
	public String getWriteTitle() {
		return "Deploy Access";
	}
}
