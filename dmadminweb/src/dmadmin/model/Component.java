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

import java.util.List;

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;

public class Component
	extends DMObject
	implements IPrePostAction
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	private Action m_preAction;
	private Action m_postAction;
	private Action m_customAction;
	private int m_parentId;
	private int m_PredecessorId;
	private String m_comptype;
	private int m_comptypeid;
	private ComponentFilter m_rollup   = ComponentFilter.OFF;
	private ComponentFilter m_rollback = ComponentFilter.OFF;
	private boolean m_filterItems;
	private int m_xpos;
	private int m_ypos;
	private List<Component> m_versions;
	private boolean m_AlwaysDeploy;
	private boolean m_DeploySequentially;
	private String m_BaseDirectory;
	private String m_label;
	private String m_parentlabel;
	private Category m_category;
	private Datasource m_datasource;
	private BuildJob m_buildjob;
	private int m_lastbuildnumber;
	
	public Component() {
	}
	
	public Component(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public Component(DMSession sess, int id, String name,String summary) {
		super(sess, id, name);
		setSummary(summary);
	}
	
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

	public Category getCategory()  { return m_category; }
	public void setCategory(Category cat)  { m_category = cat; }
 
	public String getComptype()  { return m_comptype; }
	public void setComptype(String comptype)  { m_comptype = comptype; }
	
	public int getComptypeId()  { return m_comptypeid; }
	public void setComptypeId(int comptypeid)  { m_comptypeid = comptypeid; }
 
	public Action getPreAction()  { return m_preAction; }
	public void setPreAction(Action action)  { m_preAction = action; }
	
	public Action getPostAction()  { return m_postAction; }
	public void setPostAction(Action action)  { m_postAction = action; }
	
	public Action getCustomAction()  { return m_customAction; }
	public void setCustomAction(Action action)  { m_customAction = action; }
	
	public int getParentId() { return m_parentId; }
	public void setParentId(int id) { m_parentId = id; }
	
	public int getPredecessorId()  { return m_PredecessorId; }
	public void setPredecessorId(int id)  { m_PredecessorId = id; }
	
	public ComponentFilter getRollup()  { return m_rollup; }
	public void setRollup(ComponentFilter rollup)  { m_rollup = rollup; }
	
	public ComponentFilter getRollback()  { return m_rollback; }
	public void setRollback(ComponentFilter rollback)  { m_rollback = rollback; }

	public boolean getFilterItems()   { return m_filterItems; }
	public void setFilterItems(boolean filterItems)   { m_filterItems = filterItems; }
	
	public int getXpos() { return m_xpos; }
	public void setXpos(int xpos) { m_xpos = xpos; }
	
	public int getYpos() { return m_ypos; }
	public void setYpos(int ypos) { m_ypos = ypos; }
	
	public boolean getAlwaysDeploy() { return m_AlwaysDeploy; }
	public void setAlwaysDeploy(boolean ad) { m_AlwaysDeploy = ad; }
	
	public boolean getDeploySequentially() { return m_DeploySequentially; }
	public void setDeploySequentially(boolean ds) { m_DeploySequentially = ds; }
	
	public String getBaseDirectory() { return m_BaseDirectory; }
	public void setBaseDirectory(String bd) { m_BaseDirectory = bd; }
	
	public Datasource getDatasource()  { return m_datasource; }
	public void setDatasource(Datasource datasource)  { m_datasource = datasource; }
	
	public BuildJob getBuildJob()  { return m_buildjob; }
	public void setBuildJob(BuildJob buildjob)  { m_buildjob = buildjob; }
	
	public int getLastBuildNumber()  { return m_lastbuildnumber; }
	public void setLastBuildNumber(int buildnumber)  { m_lastbuildnumber = buildnumber; }
	
	public List<Component> getVersions() {
		if (m_versions == null) {
			// Not yet set - retrieve child versions
			m_versions = m_session.getComponentVersions(this);
		}
		return m_versions;
	}
	public void setVersions(List<Component> vs) { m_versions = vs; }
	
	@Override
	public ObjectType getObjectType() {
	 if (this.getParentId() > 0)
		 return ObjectType.COMPVERSION;
	 else
	  return ObjectType.COMPONENT;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_component";
	}

	@Override
	public String getForeignKey() {
		return "compid";
	}
	
	@Override
	public boolean hasReadWrite() {
		return false;
	}

	public String getIconSuffix()
	{
		return m_filterItems ? "-item"
			: ((m_rollup != ComponentFilter.OFF) ? "-up" : "")
			+ ((m_rollback != ComponentFilter.OFF) ? "-down" : "");
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
				: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		addCreatorModifier(ds);
		ds.addProperty(SummaryField.COMPTYPE, "Component Type", getComptype());
		ds.addProperty(SummaryField.COMP_BUILDJOB, "Build Job",
				(m_buildjob != null)?m_buildjob.getLinkJSON():null);
		if (m_buildjob != null) {
			ds.addProperty(SummaryField.COMP_LASTBUILDNUMBER, "Last Build Number",m_lastbuildnumber);
		}
		ds.addProperty(SummaryField.ACTION_CATEGORY, "Category",((m_category != null) ? m_category : Category.NO_CATEGORY).getLinkJSON());
		ds.addProperty(SummaryField.FILTER_ITEMS, "Filter Level", getFilterItems() ? "Items" : "Components");
		ds.addProperty(SummaryField.ROLLUP, "Roll Forward", getRollup().toString());
		ds.addProperty(SummaryField.ROLLBACK, "Rollback", getRollback().toString());
		ds.addProperty(SummaryField.DEPLOY_ALWAYS, "Always Deploy", new BooleanField(m_AlwaysDeploy));
		ds.addProperty(SummaryField.DEPLOY_SEQUENTIALLY, "Deploy Sequentially", new BooleanField(m_DeploySequentially));
		ds.addProperty(SummaryField.BASE_DIRECTORY, "Base Directory", getBaseDirectory());
		ds.addProperty(SummaryField.PRE_ACTION, "Pre-Action",
			(m_preAction != null) ? m_preAction.getLinkJSON() : null);
		ds.addProperty(SummaryField.POST_ACTION, "Post-Action",
			(m_postAction != null) ? m_postAction.getLinkJSON() : null);
		ds.addProperty(SummaryField.CUSTOM_ACTION, "Custom Action",
			(m_customAction != null) ? m_customAction.getLinkJSON() : null);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateComponent(this, changes);
	}
}
