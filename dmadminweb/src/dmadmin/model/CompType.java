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
import dmadmin.json.IJSONSerializable;

public class CompType extends DMObject
{
 private static final long serialVersionUID = 1L;
 private int id;
 private String name;
 private String database;
 private String deletedir;

 public CompType(DMSession sess, int id, String name, String database, String deletedir, int domainid)
 {
  super(sess, id, name);
 
  this.id = id;
  this.name = name;
  this.database = database;
  this.deletedir = deletedir;
  this.setDomainId(domainid);
 }

 @Override
 public ObjectType getObjectType() {
  return ObjectType.SERVERCOMPTYPE;
 }

 @Override
 public String getDatabaseTable() {
  return "dm_type";
 }

 @Override
 public String getForeignKey() {
  return "comptypeid";
 }
 
 public int getId()
 {
  return id;
 }

 public void setId(int id)
 {
  this.id = id;
 }

 public String getName()
 {
  return name;
 }

 public void setName(String name)
 {
  this.name = name;
 }

 public String getDatabase()
 {
  return database;
 }

 public void setDatabase(String database)
 {
  this.database = database;
 }

 public String getDeletedir()
 {
  return deletedir;
 }

 public boolean getDatabaseBoolean()
 {

  if (getDatabase() != null && getDatabase().equalsIgnoreCase("Y"))
   return true;
  
  return false;
 }
 
 public boolean getDeletedirBoolean()
 {
  if (getDeletedir() != null && getDeletedir().equalsIgnoreCase("Y"))
   return true;
  
  return false;
 }
 
 public void setDeletedir(String deletedir)
 {
  this.deletedir = deletedir;
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
  ds.addProperty(SummaryField.DATABASE, "Database Roll-forward/Rollback", new BooleanField(getDatabaseBoolean()));

  return ds.getJSON();
 }

 @Override
 public boolean updateSummary(SummaryChangeSet changes) {
  return m_session.updateCompType(this, changes);
 }

}
