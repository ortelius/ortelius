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

import dmadmin.DMSession;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
//import dmadmin.PropertyDataSet;
import dmadmin.util.CommandLine;

public class TaskRemove
	extends Task
{
	private static final long serialVersionUID = 1327862378913381548L;
	private Environment m_environment;
	private Application m_application;
	private String m_text;
	private CommandLine m_cmd;
	
	public TaskRemove() {
	}
	
	public TaskRemove(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}
	
	public Environment getEnvironment() { return m_environment; }
	public void setEnvironment(Environment env) { m_environment = env; }
	
	public Application getApplication() { return m_application; }
	public void setApplication(Application application) { m_application = application; }

	public String getText()  { return m_text; }
	public void setText(String text)  { m_text = text; }
	
	@Override
	public boolean run()
	{
		System.out.println("Running TaskRemove");

		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
		m_cmd = engine.doRemove(this, m_application,m_environment,m_aps);
		return (m_cmd.run(true, m_text + "\n", true) == 0);
	}

 @Override
 public IJSONSerializable getSummaryJSON() {
  PropertyDataSet ds = new PropertyDataSet();
  ds.addProperty(SummaryField.NAME, "Name", getName());
  addCreatorModifier(ds);
  ds.addProperty(SummaryField.PRE_ACTION, "Pre-action", ((getPreAction() != null) ? getPreAction().getLinkJSON() : null));
  ds.addProperty(SummaryField.POST_ACTION, "Post-action", ((getPostAction() != null) ? getPostAction().getLinkJSON() : null));
  ds.addProperty(SummaryField.TASK_SHOWOUTPUT, "Show Output", new BooleanField(getShowOutput()));
  ds.addProperty(SummaryField.TASK_AVAILABLE_TO_SUBDOMAINS, "Available in SubDomains", new BooleanField(getSubDomains()));
  return ds.getJSON();
 }
 
	public String getOutput() {
		return m_cmd.getOutput();
	}
}
