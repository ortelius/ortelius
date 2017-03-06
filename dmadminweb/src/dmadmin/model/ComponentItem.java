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
import dmadmin.ACDChangeSet;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;

public class ComponentItem
	extends DMObject
{
	private static final long serialVersionUID = -5362995729983900561L;
	
	private Component m_parent;
	private Repository m_repo;
	private String m_targetdir = "";
	private int m_xpos;
	private int m_ypos;
	private int m_predecessorId;
	private ComponentFilter m_rollup   = ComponentFilter.OFF;
	private ComponentFilter m_rollback = ComponentFilter.OFF;
	
	public ComponentItem() {
        super.setId(0);
        super.setName("");
    }
	
	public ComponentItem(DMSession sess, int id, String name) {
		super(sess,id,name);
	}
	
	public Component getParent()  { return m_parent; }
	public void setParent(Component comp)  { m_parent = comp; }
	
	public Repository getRepository()  { return m_repo; }
	public void setRepository(Repository repo)  { m_repo = repo; }
	
	public String getTargetDir()  { return m_targetdir; }
	public void setTargetDir(String dir)  { m_targetdir = dir; }
	
	public int getPredecessorId()  { return m_predecessorId; }
	public void setPredecessorId(int id)  { m_predecessorId = id; }
	
	public ComponentFilter getRollup()  { return m_rollup; }
	public void setRollup(ComponentFilter rollup)  { m_rollup = rollup; }

	public ComponentFilter getRollback()  { return m_rollback; }
	public void setRollback(ComponentFilter rollback)  { m_rollback = rollback; }
	
	public int getXpos()  { return m_xpos; }
	public void setXpos(int xpos)  { m_xpos = xpos; }
	
	public int getYpos()  { return m_ypos; }
	public void setYpos(int ypos)  { m_ypos = ypos; }
	
	
	@Override
	public ObjectType getObjectType() {
		System.out.println("component getObjectType Returning "+ObjectType.COMPONENT);
		return ObjectType.COMPONENTITEM;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_componentitem";
	}

	@Override
	public String getForeignKey() {
		return "compid";
	}
	
	@Override
	public boolean isViewable() {
		return (m_parent != null) ? m_parent.isViewable() : false;
	}

	@Override
	public boolean isUpdatable() {
		return (m_parent != null) ? m_parent.isUpdatable() : false;
	}

	public String getIconSuffix()
	{
		return ((m_rollup != ComponentFilter.OFF) ? "-up" : "") +  ((m_rollback != ComponentFilter.OFF) ? "-down" : "");
	}
	
	public List<DMProperty> getProperties() {
		 return m_session.getPropertiesForComponentItem(this);
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		//addCreatorModifier(ds); // TODO: Addind this makes the table too big for dialog
		ds.addProperty(SummaryField.ROLLUP, "Roll Forward", getRollup().toString());
		ds.addProperty(SummaryField.ROLLBACK, "Rollback", getRollback().toString());
  ds.addProperty(SummaryField.ITEM_TARGETDIR, "Target Directory", getTargetDir());
  if (getRepository() == null)
   ds.addProperty(SummaryField.ITEM_REPOSITORY, "Repository", 0);
  else
   ds.addProperty(SummaryField.ITEM_REPOSITORY, "Repository", getRepository().getId());
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateComponentItemSummary(this, changes);
	}
	
	public void deleteProperties()
	{
	 m_session.DeleteProperties(this);
	}
	
	public boolean updateProperties(ACDChangeSet<DMProperty> changes) {
		return m_session.updateComponentItemProperties(this, changes);
	}
}
