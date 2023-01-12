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
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

import dmadmin.DMSession;
import dmadmin.ObjectType;
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

  List<Component> comps = m_so.getComponents(ObjectType.APPLICATION, m_app.getId(), false);
  for (Component c : comps)
  {
   if (c.getAlwaysDeploy())
   {
    List<ComponentItem> citems = m_so.getComponentItems(c.getId());

    boolean isDocker = false;
    if (citems != null)
    {
     for (int i=0;i<citems.size();i++)
     {
      if (citems.get(i).getItemkind() == ComponentItemKind.DOCKER)
       isDocker = true;
     }
    }

    if (isDocker)  // place component in env to skip incremental version jumping
    {
     List<Server> srvs = m_so.getServersInEnvironment(m_env.getId());
     if (srvs != null)
     {
      for (int i=0;i<srvs.size();i++)
      {
       System.out.println("Adding component " + c.getName() + " to server " + srvs.get(i).getName());
       m_so.addComponentVersionToServer(srvs.get(i).getId(), c.getId());
      }
     }
    }
   }
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


 private int startDeployment(Application app, Environment env, String userName, String sessionid, boolean waitFor, Map<String, String> cmdline_params)
 {
  Domain domain = env.getDomain();
  System.out.println("domain is " + ((domain != null) ? domain.getName() : "null"));
  Engine engine = (domain != null) ? domain.findNearestEngine() : null;

  if (engine == null)
  {
   System.err.println("Engine was null");
   return -1;
  }
  System.out.println("Calling startDeployment - waitFor is " + (waitFor ? "true" : "false"));

  List<Server> server_list = m_so.getServersInEnvironment(env.getId());

  // add base components to compsallowedonserv

  List<Component> complist = m_so.getComponents(ObjectType.APPLICATION, app.getId(), false);

  for (int i = 0; i < complist.size(); i++)
  {
   Component comp = m_so.getComponent(complist.get(i).getId(), true);
   Component base_comp = m_so.getBaseCompVersion(comp);

   for (int k = 0; k < server_list.size(); k++)
   {
    Server s = m_so.getServer(server_list.get(k).getId(), true);
    ArrayList<CompType> ctlist = s.getServerCompType();

    for (int j = 0; j < ctlist.size(); j++)
    {
     CompType ct = ctlist.get(j);

     if (ct.getId() == comp.getComptypeId())
      m_so.addComponentToServer(s.getId(), base_comp.getId());
    }
   }
  }

  m_cmd = engine.startDeployment(this, app, env, userName, sessionid, cmdline_params);

  System.out.println("Calling runWithTrilogy");

  return m_cmd.runWithTrilogyNoCapture(waitFor, null);
 }

	public int getDeploymentID()
	{
		Deployment dep = m_so.getDeploymentBySessionId(this,30);
		return dep.m_id;
	}
}
