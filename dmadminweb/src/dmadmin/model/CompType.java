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
