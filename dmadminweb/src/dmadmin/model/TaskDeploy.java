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

import java.util.Hashtable;
import java.util.Map;

import dmadmin.DMSession;
import dmadmin.util.CommandLine;

public class TaskDeploy
	extends Task
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	private Application m_app;
	private Environment m_env;
	private String m_deploySessionId;
	private boolean m_waitFor;
	// private Map<String,String> m_cmdlineParams;
	private CommandLine m_cmd;
	private DMSession m_so;
	
	public TaskDeploy() {
	}
	
	public TaskDeploy(DMSession sess, int id, String name) {
		super(sess, id, name);
		m_so = sess;
	}
	
	public Application getApplication()  { return m_app; }
	public void setApplication(Application app)  { m_app = app; }
	
	public Environment getEnvironment()  { return m_env; }
	public void setEnvironment(Environment env)  { m_env = env; }
	
	public String getDeploymentSessionId()  { return m_deploySessionId; }
	public void setDeploymentSessionId(String sessionId)  { m_deploySessionId = sessionId; }
	
	public boolean getWaitFor()  { return m_waitFor; }
	public void setWaitFor(boolean waitFor)  { m_waitFor = waitFor; }
	
	// public Map<String,String> getCmdlineParams()  { return m_cmdlineParams; }
	// public void setCmdlineParams(Map<String,String> params)  { m_cmdlineParams = params; }
	
	@Override
	public boolean run()
	{
		if(m_app == null) {
			System.err.println("Application was null");
			return false;
		}
		if(m_session == null) {
			System.err.println("Session was null");
			return false;			
		}
		int res = startDeployment(m_app, m_env, m_session.GetUserName(), m_deploySessionId, m_waitFor, m_aps);
		return (res == 0);
	}
	
	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}

	public String getLastOutputLine() {
		return (m_cmd != null) ? m_cmd.getLastOutputLine() : null;
	}
	
	public boolean engineRunning() {
		return (m_cmd != null) ? true : false;
	}
	
	private int startDeployment(Application app, Environment env, String userName, String sessionid, boolean waitFor, Map<String,String> cmdline_params)
	{
		Domain domain = env.getDomain();
		System.out.println("domain is " + ((domain != null) ? domain.getName() : "null"));
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		
		if(engine == null) {
			System.err.println("Engine was null");
			return -1;
		}
		System.out.println("Calling startDeployment - waitFor is "+(waitFor?"true":"false"));
		
		 m_cmd = engine.startDeployment(this, app, env, userName, sessionid, cmdline_params);
		 return m_cmd.run(waitFor, null, true);
	}
	
	public int getDeploymentID()
	{
		Deployment dep = m_so.getDeploymentBySessionId(this,30);
		return dep.m_id;
	}
}

