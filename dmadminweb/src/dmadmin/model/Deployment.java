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

import java.io.Serializable;
import java.util.Hashtable;
import java.util.List;

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.DateTimeField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.json.LinkField;

public class Deployment
	extends DMObject
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -9012423705055815648L;
	
	private Application m_app;
	private Environment m_env;
	private User m_user;
	private int m_started;
	private int m_finished;
	private int m_exitCode;
	private String m_exitStatus;
	private boolean m_colorize;
	
	public Deployment() {
	}
	
	public Deployment(DMSession sess, int id, int exitCode) {
		super(sess, id, "#" + id);
		m_exitCode = exitCode;
	}

	@Override
	public ObjectType getObjectType() {
		return ObjectType.DEPLOYMENT;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_deployment";
	}

	@Override
	public String getForeignKey() {
		return "deploymentid";
	}
	
	public Application getApplication()  { return m_app; }
	public void setApplication(Application app)  { m_app = app; }

	public Environment getEnvironment()  { return m_env; }
	public void setEnvironment(Environment env)  { m_env = env; }
	
	public User getUser()  { return m_user; }
	public void setUser(User user)  { m_user = user; }
	
	public int getStarted()  { return m_started; }
	public void setStarted(int started)  { m_started = started; }
	
	public int getFinished()  { return m_finished; }
	public void setFinished(int finished)  { m_finished = finished; }
	
	public int getExitCode()  { return m_exitCode; }
	public void setExitCode(int exitCode)  { m_exitCode = exitCode; }
	
	public String getExitStatus()  { return m_exitStatus; }
	public void setExitStatus(String exitStatus)  { m_exitStatus = exitStatus; }

	public String getIcon() {
		if(!isComplete()) { return "deployrun.gif"; }
		return (m_exitCode == 0) ? "deploy.png" : "deployfail.png";
	}

	public String getLargeIcon() {
		if(!isComplete()) { return "deployrun-large.gif"; }
		return (m_exitCode == 0) ? "deploy-large.png" : "deployfail-large.png";
	}
	
	public boolean isComplete() {
		return (m_finished != 0);
	}
	
	//public boolean getColorize()  { return m_colorize; }
	public void setColorize(boolean colorize)  { m_colorize = colorize; }

	@Override
	public IJSONSerializable getSummaryJSON() {
		PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		ds.addProperty("Application Name", (m_app != null) ? m_app.getLinkJSON() : null);
		ds.addProperty("Environment Name", (m_env != null) ? m_env.getLinkJSON() : null);
		ds.addProperty("Deployed By", (m_user != null) ? m_user.getLinkJSON() : null);
		ds.addProperty("Started", new DateTimeField(m_session, m_started));
		if(isComplete()) {
			ds.addProperty("Finished", new DateTimeField(m_session, m_finished));
			ds.addProperty("Time Taken", (m_finished - m_started) + " s");
			ds.addProperty("Exit Code", m_exitCode);
			ds.addProperty("Exit Status", m_exitStatus);
		}
		return ds.getJSON();
	}

	@Override
	public IJSONSerializable getLinkJSON() {
	 LinkField l = new LinkField(ObjectType.DEPLOYMENT, m_id, m_name,
				(m_finished != 0) ? m_session.formatDateToUserLocale(m_finished) : 
					(m_started != 0) ? m_session.formatDateToUserLocale(m_started) : null);
	 
	 l.setDeploymentid(getId());
	 return l;
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		// Can never be updated
		return false;
	}

	public class DeploymentLogEntry
		implements Serializable
	{
		private static final long serialVersionUID = -544942802319970165L;
		
		private int m_lineno;
		private int m_stream;
		private int m_thread;
		private String m_line;
		private int m_color;
		
		public DeploymentLogEntry() {	
		}
		
		public DeploymentLogEntry(int lineno, int stream, int thread, String line) {
			m_lineno = lineno;
			m_stream = stream;
			m_thread = thread;
			m_line = line;
			m_color = 0;
		}
		
		public int getLineNo()  { return m_lineno; }
		public void setLineNo(int lineno)  { m_lineno = lineno; }
		
		public int getStream()  { return m_stream; }
		public void setStream(int stream)  { m_stream = stream; }
		
		public int getThread()  { return m_thread; }
		public void setThread(int thread)  { m_thread = thread; }
		
		public String getLine()  { return m_line; }
		public void setLine(String line)  { m_line = line; }
		
		public int getColor()  { return m_color; }
		public void setColor(int color)  { m_color = color; }

		public String getLineWithBreaks()  {
			return m_line.replace("\n", "<br/>")
					.replace(" ", "&nbsp;")
					.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;");
		}
		
		public IJSONSerializable getJSON() {
			return new JSONObject()
				.add("lineno", m_lineno)
				.add("stream", m_stream)
				.add("line",  getLineWithBreaks())
				.add("color", m_color);
		}
	}
	
	public List<DeploymentLogEntry> getLog() {
		List<DeploymentLogEntry> ret = m_session.getDeploymentLog(this);
		if((ret != null) && m_colorize) {
			// Colorize the log
			Hashtable<Integer,Integer> threadColors = new Hashtable<Integer,Integer>();
			int color = 1;
			for(DeploymentLogEntry entry : ret) {
				int  t= entry.getThread();
				if(t != 0) {
					Integer c = threadColors.get(t);
					if(c == null) {
						c = color;
						threadColors.put(t, c);
						color++;
					}
					entry.setColor(c);
 				}
			}
		}
		return ret;
	}
	
	public List<DeploymentLogEntry> getLogSinceLine(int lineno) {
		return m_session.getDeploymentLogSinceLine(this, lineno);
	}
	
	public class DeploymentXfer
	{
		private int m_step;
		private Repository m_repo;
		private int m_repoInst;
		private String m_repoPath;
		private String m_repoVer;
		private Component m_comp;
		private Server m_target;
		private String m_targetPath;
		private String m_checksum;
		private int m_buildno;
		
		
		public DeploymentXfer(
			int step, Repository repo, int repoInst, String repoPath, String repoVer,
			Component comp, Server target, String targetPath, String checksum, int buildno)
		{
			m_step = step;
			m_repo = repo;
			m_repoInst = repoInst;
			m_repoPath = repoPath;
			m_repoVer = repoVer;
			m_comp = comp;
			m_target = target;
			m_targetPath = targetPath;
			m_checksum = checksum;
			m_buildno = buildno;
		}
		
		public Component getComp()
		{
		 return m_comp; 
		}
		
		public IJSONSerializable getJSON() {
			JSONObject step = new JSONObject();
			step.add("type", "x");
			step.add("id", m_step);
			step.add("inst", m_repoInst);
			step.add("name", "" + m_step);
			JSONArray ret = new JSONArray();
			ret.add(step).add(m_repo.getLinkJSON()).add(m_repoPath).add(m_repoVer);
			if(m_comp != null) {
				ret.add(m_comp.getLinkJSON());
			} else {
				ret.add("");
			}
			ret.add(m_target.getLinkJSON()).add(m_targetPath).add(m_checksum).add(m_buildno);
			return ret;
		}
	}
	
	public class DeploymentScript
	{
		private int m_step;
		private Action m_action;
		
		
		public DeploymentScript(
			int step, Action action)
		{
			m_step = step;
			m_action = action;
		}
		
		public IJSONSerializable getJSON() {
			JSONObject step = new JSONObject();
			step.add("step",m_step);
			step.add("action",m_action.getLinkJSON());
			return step;
		}
	}

	public IJSONSerializable getFilesJSON()
	{
		JSONArray ret = new JSONArray();
		for(DeploymentXfer xfer: m_session.getDeploymentXfers(this)) {
			ret.add(xfer.getJSON());
		}
		return ret;
	}
	
	public IJSONSerializable getScriptsJSON()
	{
		JSONArray ret = new JSONArray();
		for(DeploymentScript ds: m_session.getDeploymentScripts(this)) {
			ret.add(ds.getJSON());
		}
		return ret;
	}
	
	public PropertyDataSet getStepDetails(int stepid, int instid)
	{
		return m_session.getDeploymentProps(this, stepid, instid);
	}
}
