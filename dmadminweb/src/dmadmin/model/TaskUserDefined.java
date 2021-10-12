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

import java.util.Hashtable;

import dmadmin.DMSession;
import dmadmin.util.CommandLine;

public class TaskUserDefined
	extends Task
{
	private static final long serialVersionUID = -4022657842094840716L;
	
	private CommandLine m_cmd;
	private Application m_app;
	private Component m_comp;
	private Environment m_env;
	
	
	public TaskUserDefined() {
	}
	
	public TaskUserDefined(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public void setApplication(Application app) {
		m_app = app;
	}
	
	public void setComponent(Component comp) {
		m_comp = comp;
	}
	
	public void setEnvironment(Environment env) {
		m_env = env;
	}
	
	@Override
	public boolean run() {
		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
		if (m_app != null) {
			m_cmd = engine.doUserDefined(this,m_app, m_aps);
		} else if (m_comp != null) {
			m_cmd = engine.doUserDefined(this,m_comp, m_aps);
		} else if (m_env != null) {
			m_cmd = engine.doUserDefined(this,m_env, m_aps);
		} else {
			m_cmd = engine.doUserDefined(this, m_aps);
		}
		return (m_cmd.runWithTrilogy(true, null) == 0);
	}

	public String getOutput() {
		return m_cmd.getOutput();
	}
	
	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}
}
