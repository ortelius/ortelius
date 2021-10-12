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

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

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
	private int m_lastbuildnumber;
	private ComponentItemKind m_kind;
	
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

 public String getKindAsString() {
   if (m_kind == ComponentItemKind.DOCKER)
    return "Container";
   else if (m_kind == ComponentItemKind.DATABASE)
    return "Database";
   else
    return "Application File";
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
  ds.addProperty(SummaryField.COMP_KIND, "Kind", new LinkField(ObjectType.COMP_KIND,
    (m_kind != null) ? m_kind.value() : 0, getKindAsString()));
  
  CompType ct = null;
  
  if (m_comptypeid == 0)
	ct =  this.m_session.getCompTypeByName("Kubernetes");
  else  
    ct = this.m_session.getCompTypeByName(m_comptypeid + "");
  
		ds.addProperty(SummaryField.COMPTYPE, "Endpoint Type", ct.getFullName());
		ds.addProperty(SummaryField.COMP_DATASOURCE, "Change Request Data Source",
				(m_datasource != null)?m_datasource.getLinkJSON():null);
		
  ds.addProperty(SummaryField.XPOS, "XPos", getXpos());
  ds.addProperty(SummaryField.YPOS, "YPos", getYpos());
  
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
		
		if ( m_kind == ComponentItemKind.DOCKER || m_kind == ComponentItemKind.FILE)
		{ 
   if (m_lastbuildnumber > 0)
     ds.addProperty(SummaryField.COMP_LASTBUILDNUMBER, "Last Build Number",m_lastbuildnumber);
   else
     ds.addProperty(SummaryField.COMP_LASTBUILDNUMBER, "Last Build Number","");
		} 
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateComponent(this, changes);
	}

 public boolean updateCompItems(SummaryChangeSet changes)
 {
  String kind = "file";
  int xpos = 100;
  int ypos = 100;
  String name = "";
  
  for (SummaryField field : changes)
  {
   switch (field)
   {
    case NAME:
     name = (String)changes.get(field);
     break;
    case DOCKER_BUILDID:
    case DOCKER_BUILDURL:
    case DOCKER_CHART:
    case DOCKER_CHARTVERSION:
    case DOCKER_CHARTNAMESPACE:
    case DOCKER_OPERATOR:
    case DOCKER_BUILDDATE:
    case DOCKER_REPO:
    case DOCKER_SHA:
    case DOCKER_GITCOMMIT:
    case DOCKER_GITREPO:
    case DOCKER_GITTAG:
    case DOCKER_GITURL:
     kind = "docker";
     break; 
    case XPOS:
     xpos = new Integer((String)changes.get(field)).intValue();
     break;
    case YPOS:
     ypos = new Integer((String)changes.get(field)).intValue();
     break;     
    default:
     break;
   }
  } 
  ComponentItem ci = m_session.getComponentItemByName(this.getId(), name);
  int itemid = 0;
  
  if (ci == null)
   itemid = m_session.componentItemNewItem(this.getId(),xpos,ypos, kind);
  else
   itemid = ci.getId();
  
  ci = m_session.getComponentItem(itemid, false);
  
  boolean ret = false;
  if ((ret=m_session.updateComponentItemSummary(ci, changes)) && ci != null)
  {
   return m_session.updateComponentItemRelationship(this.getId(), ci.getId(), changes);
  }
  
  return ret;
 }

 public ComponentItemKind getKind()
 {
  return m_kind;
 }

 public void setKind(ComponentItemKind m_kind)
 {
  this.m_kind = m_kind;
 }
}
