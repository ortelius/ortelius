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
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.json.LinkField;

public class BuildJob extends DMObject
{
 private static final long serialVersionUID = 1327862378913381548L;
 private int m_builderid;
 private String m_projectname;

 private void init()
 {
 }

 public BuildJob()
 {
  init();
 }

 public BuildJob(DMSession sess, int id, String name)
 {
  super(sess, id, name);
  init();
 }

 public BuildJob(DMSession sess, int id, String name, int domainid)
 {
  super(sess, id, name);
  setDomainId(domainid);
  init();
 }

 public void setBuilderId(int builderid)
 {
  m_builderid = builderid;
 }

 public void setProjectName(String projectname)
 {
  m_projectname = projectname;
 }

 public int getBuilderId()
 {
  return m_builderid;
 }

 public String getProjectName()
 {
  return m_projectname;
 }

 public String getBuildLog(int buildid)
 {
  return m_session.getBuildLog(m_id, buildid);
 }

 public JSONArray getBuildTargets(int buildid)
 {
  return m_session.getBuildTargets(m_id, buildid);
 }

 public JSONArray getBuildFiles(int buildid)
 {
  return m_session.getBuildFiles(m_id, buildid);
 }

 public JSONObject getBuildDetails(int buildid)
 {
  return m_session.getBuildDetails(m_id, buildid);
 }

 public int getBuildTime(int buildid)
 {
  return m_session.getBuildTime(m_id, buildid);
 }

 public int getBuildDuration(int buildid)
 {
  return m_session.getBuildDuration(m_id, buildid);
 }

 public String getBuildCommitID(int buildid)
 {
  return m_session.getBuildCommitID(m_id, buildid);
 }

 @Override
 public ObjectType getObjectType()
 {
  return ObjectType.BUILDJOB;
 }

 @Override
 public String getDatabaseTable()
 {
  return "dm_buildjob";
 }

 @Override
 public String getForeignKey()
 {
  return "buildjobid";
 }

 @Override
 public IJSONSerializable getSummaryJSON()
 {
  Builder builder = m_session.getBuilder(getBuilderId());
  ProviderDefinition provider = builder.getDef();
  PropertyDataSet ds = new PropertyDataSet();
  ds.addProperty(SummaryField.NAME, "Name", getName());
  ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
  if (provider.getName().equalsIgnoreCase("bamboo")) {
	  // Bamboo uses "Plan Name" and not "Project Name"
	  ds.addProperty(SummaryField.PROJECTNAME,"Plan Name", getProjectName());
  } else {
	  ds.addProperty(SummaryField.PROJECTNAME,"Project Name", getProjectName());
  }
  addCreatorModifier(ds);
  return ds.getJSON();
 }

 @Override
 public boolean updateSummary(SummaryChangeSet changes)
 {
  return m_session.updateBuildJob(this, changes);
 }

 @Override
 public boolean hasReadWrite()
 {
  return false;
 }

 @Override
 public IJSONSerializable getLinkJSON()
 {
  Builder builder = m_session.getBuilder(m_builderid);
  if (builder != null)
  {
   Domain d = null;
   d = builder.getDomain();
   String name = m_name;
   if (d != null)
   {
    String fd = d.getFullDomain();
    if (fd.length() > 0)
     name = fd + "." + m_name;
   }
   System.out.println("in getLinkJSON for Build Job, d.getId()=" + d.getId());
   boolean showLink = m_session.ValidDomain(d.getId());
   System.out.println("showLink=" + showLink);
   return new LinkField(getObjectType(), m_id, name, showLink);
  }
  else
  {
   return new LinkField(getObjectType(), m_id, m_name, false);
  }
 }
}
