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

import java.util.List;

import dmadmin.ACDChangeSet;
import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;

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
	private ComponentItemKind m_itemkind = ComponentItemKind.FILE;
	private String BuildId = "";
	private String BuildUrl = "";
	private String Chart = "";
	private String ChartRepo = "";
	private String ChartRepoUrl = "";
	private String ChartVersion = "";
	private String DockerSha = "";
	private String DockerRepo = "";
	private String GitCommit = "";
	private String GitRepo = "";
	private String GitTag = "";
	private String GitUrl = "";
	private String Operator = "";
	private String BuildDate = "";
	private String DockerTag = "";
	private String ChartNamespace = "";
	private String SlackChannel = "";
	private String ServiceOwner = "";
	private String ServiceOwnerEmail = "";
	private String ServiceOwnerPhone = "";
	private String DiscordChannel = "";
	private String HipChatChannel = "";
	private String PagerDutyServiceUrl = "";
	private String PagerDutyBusinessServiceUrl = "";
	
	public ComponentItem() {
        super.setId(0);
        super.setName("");
    }
	
	public ComponentItem(DMSession sess, int id, String name) {
		super(sess,id,name);
	}
	
	public Component getParent()  { return m_parent; }
	public void setParent(Component comp)  { m_parent = comp; }
	
	public int getParentId() 
	{
	 if (m_parent == null)
	  return -1;
	 return m_parent.getId();
	}
	
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
	
	
	
	public String getBuildId()
 {
  return BuildId;
 }

 public void setBuildId(String buildId)
 {
  BuildId = buildId;
 }

 public String getBuildUrl()
 {
  return BuildUrl;
 }

 public void setBuildUrl(String buildUrl)
 {
  BuildUrl = buildUrl;
 }

 public String getChart()
 {
  return Chart;
 }

 public void setChart(String chart)
 {
  Chart = chart;
 }

 public String getChartVersion()
 {
  return ChartVersion;
 }
 
 public String getChartRepo()
 {
  return ChartRepo;
 }
 
 public String getChartRepoUrl()
 {
  return ChartRepoUrl;
 }
 
 
 public void setChartNamespace(String chartnamespace)
 {
  ChartNamespace = chartnamespace;
 }
 
 public String getChartNamespace()
 {
  return ChartNamespace;
 }
 

 public void setChartVersion(String chartversion)
 {
  ChartVersion = chartversion;
 }
 
 public void setChartRepo(String chartrepo)
 {
  ChartRepo = chartrepo;
 }
 
 public void setChartRepoUrl(String chartrepourl)
 {
  ChartRepoUrl = chartrepourl;
 }
 
 
 public String getDockerSha()
 {
  return DockerSha;
 }

 public void setDockerSha(String dockerSha)
 {
  DockerSha = dockerSha;
 }

 public String getDockerRepo()
 {
  return DockerRepo;
 }

 public void setDockerRepo(String dockerRepo)
 {
  DockerRepo = dockerRepo;
 }

 public String getGitCommit()
 {
  return GitCommit;
 }

 public void setGitCommit(String gitCommit)
 {
  GitCommit = gitCommit;
 }

 public String getGitRepo()
 {
  return GitRepo;
 }

 public void setGitRepo(String gitRepo)
 {
  GitRepo = gitRepo;
 }

 public String getGitTag()
 {
  return GitTag;
 }

 public void setGitTag(String gitTag)
 {
  GitTag = gitTag;
 }

 public String getGitUrl()
 {
  return GitUrl;
 }

 public void setGitUrl(String gitUrl)
 {
  GitUrl = gitUrl;
 }
 
 

 public String getOperator()
 {
  return Operator;
 }

 public void setOperator(String operator)
 {
  Operator = operator;
 }

 public String getBuildDate()
 {
  return BuildDate;
 }

 public void setBuildDate(String buildDate)
 {
  BuildDate = buildDate;
 }
 
 

 public String getSlackChannel() {
	return SlackChannel;
}

public void setSlackChannel(String slackChannel) {
	SlackChannel = slackChannel;
}

public String getServiceOwner() {
	return ServiceOwner;
}

public void setServiceOwner(String serviceOwner) {
	ServiceOwner = serviceOwner;
}

public String getServiceOwnerEmail() {
	return ServiceOwnerEmail;
}

public void setServiceOwnerEmail(String serviceOwnerEmail) {
	ServiceOwnerEmail = serviceOwnerEmail;
}

public String getServiceOwnerPhone() {
	return ServiceOwnerPhone;
}

public void setServiceOwnerPhone(String serviceOwnerPhone) {
	ServiceOwnerPhone = serviceOwnerPhone;
}

public String getDiscordChannel() {
	return DiscordChannel;
}

public void setDiscordChannel(String discordChannel) {
	DiscordChannel = discordChannel;
}

public String getHipChatChannel() {
	return HipChatChannel;
}

public void setHipChatChannel(String hipChatChannel) {
	HipChatChannel = hipChatChannel;
}

public String getPagerDutyServiceUrl() {
	return PagerDutyServiceUrl;
}

public void setPagerDutyServiceUrl(String pagerDutyServiceUrl) {
	PagerDutyServiceUrl = pagerDutyServiceUrl;
}

public String getPagerDutyBusinessServiceUrl() {
	return PagerDutyBusinessServiceUrl;
}

public void setPagerDutyBusinessServiceUrl(String pagerDutyBusinessServiceUrl) {
	PagerDutyBusinessServiceUrl = pagerDutyBusinessServiceUrl;
}

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
		
  String pname = "";
  
  ComponentItem p = null;
  try
  {
   p = m_session.getComponentItem(getPredecessorId(), false);
  }
  catch (RuntimeException e)
  {
   
  }
  
  if (p != null)
    pname = p.getName();
  ds.addProperty(SummaryField.PREDECESSOR, "Predecessor", pname);
  
  p = null;
  try
  {
   p = m_session.getComponentItem(getParentId(),true);
  }
  catch (RuntimeException e)
  {
   
  }
		
		//addCreatorModifier(ds); // TODO: Addind this makes the table too big for dialog
  ds.addProperty(SummaryField.COMP_KIND, "Kind", getItemkind().toString());
		if (getItemkind() == ComponentItemKind.DATABASE)
		{
		 ds.addProperty(SummaryField.ROLLUP, "Roll Forward", getRollup().toString());
		 ds.addProperty(SummaryField.ROLLBACK, "Rollback", getRollback().toString());
		}
  
  ds.addProperty(SummaryField.XPOS, "XPos", getXpos());
  ds.addProperty(SummaryField.YPOS, "YPos", getYpos());
  
  if (getItemkind() == ComponentItemKind.DOCKER || getItemkind() == ComponentItemKind.FILE)
  {
   ds.addProperty(SummaryField.DOCKER_BUILDID, "Build Id", getBuildId());
   ds.addProperty(SummaryField.DOCKER_BUILDURL, "Build URL", getBuildUrl());
   ds.addProperty(SummaryField.DOCKER_BUILDDATE, "Build Date", getBuildDate());
  }
  if (getItemkind() == ComponentItemKind.DOCKER)
  {
   ds.addProperty(SummaryField.DOCKER_CHART, "Helm Chart", getChart());
   ds.addProperty(SummaryField.DOCKER_CHARTVERSION, "Helm Chart Version", getChartVersion());
   ds.addProperty(SummaryField.DOCKER_CHARTNAMESPACE, "Helm Chart Namespace", getChartNamespace());
   ds.addProperty(SummaryField.DOCKER_CHARTREPO, "Helm Chart Repo", getChartRepo());
   ds.addProperty(SummaryField.DOCKER_CHARTREPOURL, "Helm Chart Repo Url", getChartRepoUrl());
   ds.addProperty(SummaryField.DOCKER_OPERATOR, "Operator", getOperator());
   ds.addProperty(SummaryField.DOCKER_REPO, "Container Registry", getDockerRepo());
   ds.addProperty(SummaryField.DOCKER_SHA, "Container Digest", getDockerSha());
   ds.addProperty(SummaryField.DOCKER_TAG, "Container Tag", getDockerTag());
   ds.addProperty(SummaryField.DOCKER_GITCOMMIT, "Git Commit", getGitCommit());
   ds.addProperty(SummaryField.DOCKER_GITREPO, "Git Repo", getGitRepo());
   ds.addProperty(SummaryField.DOCKER_GITTAG, "Git Tag", getGitTag());
   ds.addProperty(SummaryField.DOCKER_GITURL, "Git URL", getGitUrl());
   ds.addProperty(SummaryField.SERVICE_OWNER, "Service Owner", getServiceOwner());
   ds.addProperty(SummaryField.SERVICE_OWNER_EMAIL, "Service Owner Email", getServiceOwnerEmail());
   ds.addProperty(SummaryField.SERVICE_OWNER_PHONE, "Service Owner Phone", getServiceOwnerPhone());
   ds.addProperty(SummaryField.SLACK_CHANNEL, "Slack Channel", getSlackChannel());
   ds.addProperty(SummaryField.DISCORD_CHANNEL, "Slack Channel", getDiscordChannel());
   ds.addProperty(SummaryField.HIPCHAT_CHANNEL, "Slack Channel", getHipChatChannel());
   ds.addProperty(SummaryField.PAGERDUTY_SERVICE_URL, "Pager Duty Service Url", getPagerDutyServiceUrl());
   ds.addProperty(SummaryField.PAGERDUTY_BUSINESS_URL, "Pager Duty Business Service Url", getPagerDutyBusinessServiceUrl());
  }
  
  if (getItemkind() == ComponentItemKind.DATABASE || getItemkind() == ComponentItemKind.FILE)
  {  
   String repotype = "";
   
   if (getRollup() == ComponentFilter.ON)
    repotype = "Roll Forward ";
 
   if (getRollback() == ComponentFilter.ON)
    repotype = "Roll Back ";

   ds.addProperty(SummaryField.ITEM_TARGETDIR, repotype + "Target Directory", getTargetDir());
   
   if (getRepository() == null)
    ds.addProperty(SummaryField.ITEM_REPOSITORY, repotype + "Repository", "");
   else
    ds.addProperty(SummaryField.ITEM_REPOSITORY, repotype + "Repository", getRepository().getDomain().getFullDomain() + "." + getRepository().getName());
  }
  
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
  return m_session.updateComponentItemProperties(this, changes, false);
 }
 
	public boolean updateProperties(ACDChangeSet<DMProperty> changes, boolean deleteAll) {
		return m_session.updateComponentItemProperties(this, changes, deleteAll);
	}

 public ComponentItemKind getItemkind()
 {
  return m_itemkind;
 }

 public void setItemkind(ComponentItemKind m_itemkind)
 {
  this.m_itemkind = m_itemkind;
 }

 public JSONObject toJSON()
 {
  JSONObject obj = new JSONObject();
  obj.add("Name", getFullName());
  obj.add("Summary", getSummary());
  
  String pname = "";
  
  ComponentItem p = null;
  try
  {
   p = m_session.getComponentItem(getPredecessorId(), false);
  }
  catch (RuntimeException e)
  {
   
  }
  
  if (p != null)
    pname = p.getName();
  obj.add("Predecessor", pname);
  
  p = null;
  try
  {
   p = m_session.getComponentItem(getParentId(),true);
  }
  catch (RuntimeException e)
  {
   
  }
  
  //addCreatorModifier(ds); // TODO: Addind this makes the table too big for dialog
  if (getItemkind() == ComponentItemKind.DATABASE)
  {
   obj.add("Roll Forward", getRollup().toString());
   obj.add("Rollback", getRollback().toString());
  }
  
  obj.add("XPos", getXpos());
  obj.add("YPos", getYpos());
  
  if (getItemkind() == ComponentItemKind.DATABASE || getItemkind() == ComponentItemKind.FILE)
  {
   obj.add("Target Directory", getTargetDir());
   if (getRepository() == null)
    obj.add("Repository", "");
   else
    obj.add( "Repository", getRepository().getName());
  }
  
  if (getItemkind() == ComponentItemKind.DOCKER)
  {
   obj.add("Build Id", getBuildId());
   obj.add("Build URL", getBuildUrl());
   obj.add("Helm Chart", getChart());
   obj.add("Helm Chart Version", getChartVersion());
   obj.add("Helm Chart Namespace", getChartNamespace());
   obj.add("Helm Chart Repo", getChartRepo());
   obj.add("Helm Chart Repo Url", getChartRepoUrl());
   obj.add("Operator", getOperator());
   obj.add("Build Date", getBuildDate());
   obj.add("Container Registry", getDockerRepo());
   obj.add("Container Digest", getDockerSha());
   obj.add("Container Tag", getDockerTag());
   obj.add("Git Commit", getGitCommit());
   obj.add("Git Repo", getGitRepo());
   obj.add("Git Tag", getGitTag());
   obj.add("Git URL", getGitUrl());
   obj.add("Service Owner", getServiceOwner());
   obj.add("Service Owner Email", getServiceOwnerEmail());
   obj.add("Service Owner Phone", getServiceOwnerPhone());
   obj.add("Slack Channel", getSlackChannel());
   obj.add("Slack Channel", getDiscordChannel());
   obj.add("Slack Channel", getHipChatChannel());
   obj.add("Pager Duty Service Url", getPagerDutyServiceUrl());
   obj.add("Pager Duty Business Service Url", getPagerDutyBusinessServiceUrl());
  }
  
  return obj;
 }

 public void setDockerTag(String tag)
 {
  DockerTag = tag;
 }
 
 public String getDockerTag()
 {
  return DockerTag;
 }
}
