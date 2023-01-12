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

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;
import dmadmin.util.CommandLine;

public class Engine
	extends DMObject
{
	private static final long serialVersionUID = -4798592630748068986L;

	private String m_hostname;
	private DMSession m_session;
	private String m_clientid;	// for SaaS

	public Engine() {
	}

	public Engine(DMSession sess, int id, String name) {
		super(sess, id, name);
		m_session = sess;
	}

	public String getHostname()  { return m_hostname; }
	public void setHostname(String hostname)  { m_hostname = hostname; }

	public String getClientID()  { return m_clientid; }
	public void setClientID(String clientid)  { m_clientid = clientid; }

	public List<Plugin> getPlugins() {
		return m_session.getPluginsForEngine(this);
	}

	public DMSession getSession() {
		return m_session;
	}

	public List<ConfigEntry> getConfig() {
		return m_session.getConfigForEngine(this);
	}

	public List<ProviderDefinition> getProviderDefs() {
		return m_session.getProviderDefsForEngine(this);
	}

	@Override
	public ObjectType getObjectType() {
		return ObjectType.ENGINE;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_engine";
	}

	@Override
	public String getForeignKey() {
		return "engineid";
	}

	@Override
	public boolean hasReadWrite() {
		return false;
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
		ds.addProperty(SummaryField.ENGINE_HOSTNAME, "Hostname", m_hostname);
  ds.addProperty(SummaryField.ENGINE_CLIENTID, "Client Id", m_clientid);
		//ds.addProperty("Owner", (m_owner != null) ? m_owner.getLinkJSON()
		//		: ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
		//ds.addProperty("Summary", getSummary());
		addCreatorModifier(ds);
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateEngine(this, changes);
	}

	public CommandLine startDeployment(
		Task task, Application app, Environment env, String userName,
		String sessionid, Map<String,String> cmdline_params)
	{
		CommandLine cmd = new CommandLine(this)
			.add("dm")
			.add("-usr").add(userName)
			.pw(m_session.getPassword());
		if(sessionid != null) {
			cmd.add("-sessionid").add(sessionid);
		}
		cmd.add("-envid").add(env.getId())
			.add("-appid").add(app.getId())
			.add("-taskid").add(task.getId())
			.add(cmdline_params);
		return cmd;
	}

 public CommandLine auditServer(String home, int serverid)
 {
	 CommandLine cmd = new CommandLine(home + "\\bin\\DeploymentManager.exe").add("-home").add(home).add("-scanserver").add(serverid);
	 return cmd;
 }

	public int doNotification(
		Notify nfy, Application app, Environment env,
		String userName, List<String> toOtids, String from,
		int templateid, Map<String,String> cmdline_params)
	{
		CommandLine cmd = new CommandLine("C:\\Shared\\DM2\\DeploymentManager\\Windows\\Debug\\DeploymentManager.exe");
		cmd.add("-home").add("C:\\Shared\\DM2\\DeploymentManager\\distrib")
			.add("-usr").add(userName)
			.pw(m_session.getPassword())
			.add("-notify").add(nfy.getId())
			.add("-from").add(from)
			.add("-template").add(templateid);
		if(app != null) {
			cmd.add("-appid").add(app.getId());
		}
		if(env != null) {
			cmd.add("-envid").add(env.getId());
		}
		cmd.add(cmdline_params);
		for(String otid : toOtids) {
			cmd.add(otid);
		}
		int ret = cmd.run(true);		// was cmd.run(true, body);
		if(ret != 0) {
			System.out.println(cmd.getLastOutputLine());
		}
		return ret;
	}

	public CommandLine doProviderTest(ProviderObject po)
	{
		CommandLine ret;
		ret = new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-providertest")
			.add(po.getObjectType().toString().toLowerCase())
			.add(po.getId());
		if (po.getObjectType()==ObjectType.NOTIFY) {
			String rcpt = ((Notify)po).getTestRecipient();
			if (rcpt != null) ret.add(rcpt);
		}
		return ret;
	}

	public CommandLine doApprove(Task task, Application app, boolean approved, Map<String,String> aps)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-runtask").add(task.getId())
			.add("-appid").add(app.getId())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("approve_approved=" + (approved ? 1 : 0))
			.add(aps);
	}

	public CommandLine doRunAction(Task task, Map<String,String> aps)
	{
		return new CommandLine(this)
		.add("dm")
		.add("-usr").add(m_session.GetUserName())
		.pw(m_session.getPassword())
		.add("-sessionid").add(m_session.GetSessionId())
		.add("-runtask").add(task.getId())
		.add(aps);
	}

	public CommandLine doMoveCopyRequest(Task task, Application app, Map<String,String> aps)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-runtask").add(task.getId())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("-appid").add(app.getId())
			.add(aps);
	}

	public CommandLine doRemove(Task task, Application app,Environment env, Map<String,String> aps)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-appid").add(app.getId())
			.add("-envid").add(env.getId())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("-remove")
			.add(aps);
	}

	public CommandLine doTestServer(Server server)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-checkserver").add(server.getId());
	}

	public CommandLine doCreateScript()
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-importscript");
	}

	public CommandLine showDMScript(Action action)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-dmscript").add(action.getId());
	}

	public CommandLine doNewAppVer(Task task, Application app, boolean useLatest, Application pred, Map<String,String> aps)
	{
		CommandLine cmd = new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-runtask").add(task.getId())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("-appid").add(app.getId());
		if(useLatest) {
			cmd.add("-app2id").add("+1");
		} else if(pred != null) {
			cmd.add("-app2id").add(pred.getId());
		}
		cmd.add(aps);
		return cmd;
	}

	public CommandLine doUserDefined(Task task, Map<String,String> aps)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("-runtask").add(new Integer(task.getId()).toString())
			.add(aps);
	}

	public CommandLine doUserDefined(Task task,Application app, Map<String,String> aps)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-appid").add(app.getId())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("-runtask").add(new Integer(task.getId()).toString())
			.add(aps);
	}

	public CommandLine doUserDefined(Task task,Component comp, Map<String,String> aps)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-compid").add(comp.getId())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("-runtask").add(new Integer(task.getId()).toString())
			.add(aps);
	}

	public CommandLine doUserDefined(Task task,Environment env, Map<String,String> aps)
	{
		return new CommandLine(this)
			.add("dm")
			.add("-usr").add(m_session.GetUserName())
			.pw(m_session.getPassword())
			.add("-envid").add(env.getId())
			.add("-sessionid").add(m_session.GetSessionId())
			.add("-runtask").add(new Integer(task.getId()).toString())
			.add(aps);
	}

	public String ParseProcedure(String procbody)
	{
		CommandLine cmd = new CommandLine(this)
			.add("dm")
			.add("-parse").add("^");
		System.out.println("procbody="+procbody);
		cmd.runWithTrilogy(true, procbody);
		return cmd.getOutput().trim();
	}

	public String dumpScript(int actionid)
	{
		CommandLine cmd = new CommandLine(this)
		.add("dm")
		.add("-dumpscript").add(actionid);
		int ret = cmd.runWithTrilogy(true, null);
		if (ret == 0) {
			return cmd.getOutput().trim();
		}
		throw new RuntimeException(cmd.getOutput());
	}


public String[] splitInParts(String s, int partLength)
{
    int len = s.length();

    // Number of parts
    int nparts = (len + partLength - 1) / partLength;
    String parts[] = new String[nparts];

    // Break into parts
    int offset= 0;
    int i = 0;
    while (i < nparts)
    {
        parts[i] = s.substring(offset, Math.min(offset + partLength, len));
        offset += partLength;
        i++;
    }

    return parts;
}

 public String encryptValue(String value, String userName)
 {
  if (value.trim().length() == 0)
   return "";

  String[] parts = splitInParts(value, 1000);
  ArrayList<String> encparts = new ArrayList<String>();

  for (int i = 0; i < parts.length; i++)
  {
   value = parts[i];
   CommandLine cmd = new CommandLine(this).add("dm").add("-usr").add(userName).pw(m_session.getPassword()).add("-encrypt");
   int ret = cmd.runWithTrilogy(true, value + "\n");
   if (ret == 0)
    encparts.add(cmd.getOutput().trim());
   else
    throw new RuntimeException("Encryption Failed for " + value);
  }
  return String.join("^", encparts);
 }

	public class ConfigEntry
		implements IJSONSerializable
	{
		private String m_name;
		private String m_value;

		public ConfigEntry(String name, String value) {
			m_name = name;
			m_value = value;
		}

		public String getName()  { return m_name; }
		public String getValue()  { return m_value; }

		public String getJSON()  {
			return new JSONObject()
				.add("name", m_name)
				.add("value", m_value)
				.getJSON();
		}
	}
}
