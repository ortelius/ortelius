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
import dmadmin.PropertyDataSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
import dmadmin.util.CommandLine;

public class TaskRequest
	extends Task
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	private Application m_app;
	private String m_text;
	private Task m_linkedtask;
	private CommandLine m_cmd;
	
	
	public TaskRequest() {
	}
	
	public TaskRequest(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public Application getApplication()  { return m_app; }
	public void setApplication(Application app)  { m_app = app; }
	
	public String getText()  { return m_text; }
	public void setText(String text)  { m_text = text; }
	
	public Task getLinkedTask() { return m_linkedtask; }
	public void setLinkedTask(Task ta) { m_linkedtask = ta; }
	
	
	@Override
	public boolean run() {
		System.out.println("Running TaskMove");

		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
		m_cmd = engine.doMoveCopyRequest(this, m_app, m_aps);
		return (m_cmd.runWithTrilogy(true, m_text + "\n") == 0);
	}

	public String getOutput() {
		return m_cmd.getOutput();
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
  addCreatorModifier(ds);
  ds.addProperty(SummaryField.PRE_ACTION, "Pre-action", ((getPreAction() != null) ? getPreAction().getLinkJSON() : null));
  ds.addProperty(SummaryField.POST_ACTION, "Post-action", ((getPostAction() != null) ? getPostAction().getLinkJSON() : null));
  ds.addProperty(SummaryField.TASK_SHOWOUTPUT, "Show Output", new BooleanField(getShowOutput()));
  ds.addProperty(SummaryField.TASK_AVAILABLE_TO_SUBDOMAINS, "Available in SubDomains", new BooleanField(getSubDomains()));
  ds.addProperty(SummaryField.TASK_LINKED_TASK, "Linked Task", ((getLinkedTask() != null) ? getLinkedTask().getName() : null));
  ds.addProperty(SummaryField.SUCCESS_TEMPLATE, "Request Notification Template", ((getSuccessTemplate() != null) ? getSuccessTemplate().getLinkJSON() : null));
  return ds.getJSON();
 }
	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}
}
