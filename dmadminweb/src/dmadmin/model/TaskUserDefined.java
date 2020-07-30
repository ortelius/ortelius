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
