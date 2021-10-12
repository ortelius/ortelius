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

public class UserPermissions
{
	//
	// General "Create" permissions
	//
	boolean m_createuser;
	boolean m_creategroup;
	boolean m_createdomain;
	boolean m_createenv;
	boolean m_createserver;
	boolean m_createrepos;
	boolean m_createcomp;
	boolean m_createcred;
	boolean m_createapp;
	boolean m_createappver;
	boolean m_createaction;
	boolean m_createproc;
	boolean m_createdatasrc;
	boolean m_createnotifier;
	boolean m_createengine;
 boolean m_createservercomptype;
	
	public UserPermissions(DMSession sess,int groupid) {
		sess.setUserPermissions(groupid,this);
	}
	
	//
	// get/set general create permissions
	//
	public boolean getCreateUsers()      { return m_createuser; }
	public boolean getCreateGroups()     { return m_creategroup; }
	public boolean getCreateDomains()    { return m_createdomain; }
	public boolean getCreateEnvs()       { return m_createenv; }
	public boolean getCreateServers()    { return m_createserver; }
	public boolean getCreateRepos()      { return m_createrepos; }
	public boolean getCreateComps()      { return m_createcomp; }
	public boolean getCreateCreds()      { return m_createcred; }
	public boolean getCreateApps()       { return m_createapp; }
	public boolean getCreateAppvers()    { return m_createappver; }
	public boolean getCreateActions()    { return m_createaction; }
	public boolean getCreateProcs()      { return m_createproc; }
	public boolean getCreateDatasrc()    { return m_createdatasrc; }
	public boolean getCreateNotifiers()  { return m_createnotifier; }
	public boolean getCreateEngines()    { return m_createengine; }
 public boolean getCreateServerCompType()    { return m_createservercomptype; }
 
	public void setCreateUsers(boolean bval)      {  m_createuser = bval; }
	public void setCreateGroups(boolean bval)     {  m_creategroup = bval; }
	public void setCreateDomains(boolean bval)    {  m_createdomain = bval; }
	public void setCreateEnvs(boolean bval)       {  m_createenv = bval; }
	public void setCreateServers(boolean bval)    {  m_createserver = bval; }
	public void setCreateRepos(boolean bval)      {  m_createrepos = bval; }
	public void setCreateComps(boolean bval)      {  m_createcomp = bval; }
	public void setCreateCreds(boolean bval)      {  m_createcred = bval; }
	public void setCreateApps(boolean bval)       {  m_createapp = bval; }
	public void setCreateAppvers(boolean bval)    {  m_createappver = bval; }
	public void setCreateActions(boolean bval)    {  m_createaction = bval; }
	public void setCreateProcs(boolean bval)      {  m_createproc = bval; }
	public void setCreateDatasrc(boolean bval)    {  m_createdatasrc = bval; }
	public void setCreateNotifiers(boolean bval)  {  m_createnotifier = bval; }
	public void setCreateEngines(boolean bval)     {  m_createengine = bval; }
 public void setCreateServerCompType(boolean bval)     {  m_createservercomptype = bval; }
 
	//
	// Returns the word "disabled" if the corresponding permission is not set.
	// Used in "new user group" scenarios to prevent the permission being granted
	// to a group to which the invoking user does not have access
	//
	public String getUsersDisabled()     { return m_createuser?"":"disabled"; } 
	public String getGroupsDisabled()    { return m_creategroup?"":"disabled"; } 
	public String getDomainsDisabled()   { return m_createdomain?"":"disabled"; } 
	public String getEnvsDisabled()      { return m_createenv?"":"disabled"; } 
	public String getServersDisabled()   { return m_createserver?"":"disabled"; } 
	public String getReposDisabled()     { return m_createrepos?"":"disabled"; } 
	public String getCompsDisabled()     { return m_createcomp?"":"disabled"; } 
	public String getCredsDisabled()     { return m_createcred?"":"disabled"; } 
	public String getAppsDisabled()      { return m_createapp?"":"disabled"; } 
	public String getAppversDisabled()   { return m_createappver?"":"disabled"; } 
	public String getActionsDisabled()   { return m_createaction?"":"disabled"; } 
	public String getProcsDisabled()     { return m_createproc?"":"disabled"; } 
	public String getDatasrcDisabled()   { return m_createdatasrc?"":"disabled"; } 
	public String getNotifiersDisabled() { return m_createnotifier?"":"disabled"; } 
	public String getEnginesDisabled()   { return m_createengine?"":"disabled"; }
 public String getServerCompTypeDisabled()   { return m_createservercomptype?"":"disabled"; }
 
	public String getUsersChecked()     { return m_createuser?"Checked":""; } 
	public String getGroupsChecked()    { return m_creategroup?"Checked":""; } 
	public String getDomainsChecked()   { return m_createdomain?"Checked":""; } 
	public String getEnvsChecked()      { return m_createenv?"Checked":""; } 
	public String getServersChecked()   { return m_createserver?"Checked":""; } 
	public String getReposChecked()     { return m_createrepos?"Checked":""; } 
	public String getCompsChecked()     { return m_createcomp?"Checked":""; } 
	public String getCredsChecked()     { return m_createcred?"Checked":""; } 
	public String getAppsChecked()      { return m_createapp?"Checked":""; } 
	public String getAppversChecked()   { return m_createappver?"Checked":""; } 
	public String getActionsChecked()   { return m_createaction?"Checked":""; } 
	public String getProcsChecked()     { return m_createproc?"Checked":""; } 
	public String getDatasrcChecked()   { return m_createdatasrc?"Checked":""; } 
	public String getNotifiersChecked() { return m_createnotifier?"Checked":""; } 
	public String getEnginesChecked()   { return m_createengine?"Checked":""; } 
 public String getServerCompTypeChecked()   { return m_createservercomptype?"Checked":""; } 
}